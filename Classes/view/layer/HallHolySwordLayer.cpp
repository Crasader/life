//
//  HallHolySwordLayer.cpp
//  life
//
//  Created by ff on 17/8/7.
//
//

#include "HallHolySwordLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;


HallHolySwordLayer::HallHolySwordLayer():
plotButtonId(0),
plotMissionId(0)
{
}

void HallHolySwordLayer::onEnter()
{
    Layer::onEnter();
    updateSwordListener = EventListenerCustom::create(UPDATE_SWORD, CC_CALLBACK_1(HallHolySwordLayer::updateSword, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateSwordListener, -1);
    updateFightListener = EventListenerCustom::create(UPDATE_FIGHT_SWORD, CC_CALLBACK_1(HallHolySwordLayer::updateFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateFightListener, -1);
    updateLevelListener = EventListenerCustom::create(UPDATE_SWORD_LEVEL, CC_CALLBACK_1(HallHolySwordLayer::updateLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateLevelListener, -1);
    updateGoldListener = EventListenerCustom::create(UPDATE_GOLD, CC_CALLBACK_1(HallHolySwordLayer::updateGold, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGoldListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallHolySwordLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    
}

HallHolySwordLayer::~HallHolySwordLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateSwordListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateFightListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateLevelListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallHolySwordLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    auto root = CSLoader::createNode(HALL_HOLY_SWORD_UI);
    rootAction = CSLoader::createTimeline(HALL_HOLY_SWORD_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallHolySwordLayer::clickClose, this));
    
    auto valueBg = (ImageView*)rootBg->getChildByName("value_bg");
    goldText = (Text*)valueBg->getChildByName("gold_bg")->getChildByName("gold")->getChildByName("count_text");
    stoneText = (Text*)valueBg->getChildByName("gold_bg")->getChildByName("stone")->getChildByName("count_text");
    totalHpText = (Text*)valueBg->getChildByName("hp_text");
    totalAttackText = (Text*)valueBg->getChildByName("attack_text");
    totalDefenceText = (Text*)valueBg->getChildByName("defence_text");
    auto fightButton = (Button*)valueBg->getChildByName("fight_button");
    fightButton->addClickEventListener(CC_CALLBACK_0(HallHolySwordLayer::clickFight, this));
    nextLevelText = (ImageView*)valueBg->getChildByName("next_level_text");
    plusHpText = (Text*)nextLevelText->getChildByName("plus_hp_text");
    plusAttackText = (Text*)nextLevelText->getChildByName("plus_attack_text");
    plusDefenceText = (Text*)nextLevelText->getChildByName("plus_defence_text");
    costGoldText = (Text*)nextLevelText->getChildByName("cost_gold_text");
    costStoneText = (Text*)nextLevelText->getChildByName("cost_stone_text");
    uplevelButton = (Button*)nextLevelText->getChildByName("uplevel_button");
    uplevelButton->addClickEventListener(CC_CALLBACK_0(HallHolySwordLayer::clickUplevel, this));
    
    auto swordBg = (ImageView*)rootBg->getChildByName("sword_bg");
    swordImg = (Sprite*)swordBg->getChildByName("sword_img");
    levelAtlas = (TextAtlas*)swordBg->getChildByName("level_atlas");
    lockText = (Text*)swordBg->getChildByName("uplevel_text");
    
    auto listBg = (ImageView*)rootBg->getChildByName("list_bg");
    for (int i = 0; i < 5; i++) {
        swordButton[i] = (Button*)listBg->getChildByTag(i+1);
        swordButton[i]->addClickEventListener(CC_CALLBACK_1(HallHolySwordLayer::clickSword, this));
        fightFlag[i] = (Sprite*)swordButton[i]->getChildByName("fight_flag");
        lockFlag[i] = (ImageView*)swordButton[i]->getChildByName("lock_flag");
    }
    return true;
}

void HallHolySwordLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HOLY_SWORD info = *static_cast<L2E_SHOW_HOLY_SWORD*>(event->getUserData());
    for (int i = 0; i < 5; i++) {
        swordButton[i]->setTitleText(info.swordName[i]);
        lockFlag[i]->setVisible(info.swordLock[i]);
        fightFlag[i]->setVisible(false);
        auto tip = (ImageView*)swordButton[i]->getChildByName("tip");
        tip->setVisible(info.tipIndex == (i+1));
    }
    currSwordId = info.currSwrodId;
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.tipIndex == currSwordId);
    
    fightSwordId = info.fightSwordId;
    swordButton[info.currSwrodId-1]->setContentSize(Size(180, 55));
    swordButton[info.currSwrodId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "guanqia_1.png"));
    fightFlag[info.fightSwordId-1]->setVisible(true);
    
    levelAtlas->setString(Convert2String(info.currSwordLevel));
    swordImg->setTexture(GameUtils::format(HOLY_SWORD_DIR, info.currSwordImg.c_str()));
    if (info.currSwordLevel >= info.nextSwordLockLevel) {
        lockText->setVisible(false);
    }else{
        lockText->setVisible(true);
        lockText->setString(GameUtils::format(StringData::shared()->stringFromKey("holy_sword_lock").c_str(),
                                              info.nextSwordLockLevel));
    }
//    goldText->setString(Convert2String(info.totalGold));
    if (info.totalGold > 100000) {
        goldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.totalGold/10000).c_str());
    }else{
        goldText->setString(Convert2String(info.totalGold));
    }
    stoneText->setString(Convert2String(info.totalStone));
    totalHpText->setString(Convert2String(info.totalHp));
    totalAttackText->setString(Convert2String(info.totalAttack));
    totalDefenceText->setString(Convert2String(info.totalDefence));
    if (info.maxLevel) {
        nextLevelText->setVisible(false);
    }else{
        nextLevelText->setVisible(true);
        plusHpText->setString(Convert2String(info.nextHp));
        plusAttackText->setString(Convert2String(info.nextAttack));
        plusDefenceText->setString(Convert2String(info.nextDefence));
        costGoldText->setString(Convert2String(info.costGold));
        costStoneText->setString(Convert2String(info.costStone));
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallHolySwordLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}
void HallHolySwordLayer::updateSword(cocos2d::EventCustom *event)
{
    L2E_UPDATE_SWORD info  = *static_cast<L2E_UPDATE_SWORD*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("holy_sword_err4");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    swordButton[currSwordId-1]->setContentSize(Size(160, 55));
    swordButton[currSwordId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "guanqia.png"));

    currSwordId = info.currSwordId;
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.tipIndex == currSwordId);
    
    swordButton[info.currSwordId-1]->setContentSize(Size(180, 55));
    swordButton[info.currSwordId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "guanqia_1.png"));
    levelAtlas->setString(Convert2String(info.currSwordLevel));
    swordImg->setTexture(GameUtils::format(HOLY_SWORD_DIR, info.currSwordImg.c_str()));
    if (info.currSwordLevel >= info.nextSwordLockLevel) {
        lockText->setVisible(false);
    }else{
        lockText->setVisible(true);
        lockText->setString(GameUtils::format(StringData::shared()->stringFromKey("holy_sword_lock").c_str(),
                                              info.nextSwordLockLevel));
    }
    
    if (info.maxLevel) {
        nextLevelText->setVisible(false);
    }else{
        nextLevelText->setVisible(true);
        plusHpText->setString(Convert2String(info.nextHp));
        plusAttackText->setString(Convert2String(info.nextAttack));
        plusDefenceText->setString(Convert2String(info.nextDefence));
        costGoldText->setString(Convert2String(info.costGold));
        costStoneText->setString(Convert2String(info.costStone));
    }

}
void HallHolySwordLayer::updateFight(cocos2d::EventCustom *event)
{
    L2E_UPDATE_FIGHT_SWORD info = *static_cast<L2E_UPDATE_FIGHT_SWORD*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("holy_sword_err4");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    fightFlag[fightSwordId-1]->setVisible(false);
    
    fightSwordId = info.fightSwordId;
    fightFlag[fightSwordId-1]->setVisible(true);

}

void HallHolySwordLayer::updateLevel(cocos2d::EventCustom *event)
{
    L2E_UPDATE_SWORD_LEVEL info = *static_cast<L2E_UPDATE_SWORD_LEVEL*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("holy_sword_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    for (int i = 0; i < 5; i++) {
        lockFlag[i]->setVisible(info.swordLock[i]);
        auto tip = (ImageView*)swordButton[i]->getChildByName("tip");
        tip->setVisible(info.tipIndex == (i+1));
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.tipIndex == currSwordId);

    if (info.currSwordLevel >= info.nextSwordLockLevel) {
        lockText->setVisible(false);
    }else{
        lockText->setVisible(true);
        lockText->setString(GameUtils::format(StringData::shared()->stringFromKey("holy_sword_lock").c_str(),
                                              info.nextSwordLockLevel));
    }
    levelAtlas->setString(Convert2String(info.currSwordLevel));
    stoneText->setString(Convert2String(info.totalStone));
    totalHpText->setString(Convert2String(info.totalHp));
    totalAttackText->setString(Convert2String(info.totalAttack));
    totalDefenceText->setString(Convert2String(info.totalDefence));
    if (info.maxLevel) {
        nextLevelText->setVisible(false);
    }else{
        nextLevelText->setVisible(true);
        plusHpText->setString(Convert2String(info.nextHp));
        plusAttackText->setString(Convert2String(info.nextAttack));
        plusDefenceText->setString(Convert2String(info.nextDefence));
        costGoldText->setString(Convert2String(info.costGold));
        costStoneText->setString(Convert2String(info.costStone));
    }

}

void HallHolySwordLayer::updateGold(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
//    goldText->setString(Convert2String(info.value));
    if (info.value > 100000) {
        goldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.value/10000).c_str());
    }else{
        goldText->setString(Convert2String(info.value));
    }
}
void HallHolySwordLayer::endIn()
{
    rootAction->play("play", true);
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = HOLY_SWORD_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHolySwordLayer::clickClose()
{
    if (plotButtonId == 2) {
        closeButton->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = HOLY_SWORD_LAYER;
        infoTrigger.buttonId = 2;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallHolySwordLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallHolySwordLayer::removeOff()
{
    removeFromParent();
}

void HallHolySwordLayer::clickFight()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_set_fight_sword;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHolySwordLayer::clickSword(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_sel_sword;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHolySwordLayer::clickUplevel()
{
    if (plotButtonId == 1) {
        uplevelButton->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = HOLY_SWORD_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_sword_uplevel;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHolySwordLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] != HOLY_SWORD_LAYER)
            {
                return;
            }
            
            plotButtonId = info.param[3];
            plotMissionId = info.missionId;
            
            auto point = CSLoader::createNode(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
            auto action = CSLoader::createTimeline(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
            point->runAction(action);
            action->play("play", true);
            
            auto pointTip = CSLoader::createNode(GUIDE_POINT_TIP_UI);
            
            switch (info.param[3]) {
                case 1:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(uplevelButton->getContentSize()/2);
                    point->setTag(1000);
                    
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    uplevelButton->addChild(point);
                }
                    break;
                case 2:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(closeButton->getContentSize()/2);
                    point->setTag(1000);
                    closeButton->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->addChild(point);
                    
                }
                    break;
                    
                default:
                    break;
            }
            
            if (info.param[4] != 0) {
                auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
                text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
                point->addChild(pointTip);
            }
        }
            break;
            
        default:
            break;
    }
}
