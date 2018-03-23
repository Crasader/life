//
//  HallPetLayer.cpp
//  life
//
//  Created by ff on 17/7/31.
//
//

#include "HallPetLayer.h"
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


HallPetLayer::HallPetLayer():
focusPos(0),
plotButtonId(0),
plotMissionId(0)
{
    
    memset(fightPetId, 0, sizeof(int)*3);
}
void HallPetLayer::onEnter()
{
    Layer::onEnter();
    updateFightPetListener = EventListenerCustom::create(UPDATE_FIGHT_PET, CC_CALLBACK_1(HallPetLayer::updateFightPet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateFightPetListener, -1);
    updatePetLevelListener = EventListenerCustom::create(UPDATE_PET_LEVEL, CC_CALLBACK_1(HallPetLayer::updatePetLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updatePetLevelListener, -1);
    updatePetDetailListener = EventListenerCustom::create(SEL_PET_POS, CC_CALLBACK_1(HallPetLayer::updatePetDetail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updatePetDetailListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallPetLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    updateTipListener = EventListenerCustom::create(UPDATE_PET_TIP, CC_CALLBACK_1(HallPetLayer::updateTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTipListener, -1);
    
}
HallPetLayer::~HallPetLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateFightPetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updatePetLevelListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updatePetDetailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTipListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallPetLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_PET_UI);
    rootAction = CSLoader::createTimeline(HALL_PET_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallPetLayer::clickClose, this));
    
    auto detailBg = (ImageView*)rootBg->getChildByName("detail_bg");
    petIcon = (ImageView*)detailBg->getChildByName("icon");
    uplevelImg = (ImageView*)detailBg->getChildByName("uplevel_text");
    skillNameText = (Text*)detailBg->getChildByName("skill_name");
    maxFlagText = (ImageView*)detailBg->getChildByName("max_flag_text");
    uplevelButton = (Button*)maxFlagText->getChildByName("upgrade_button");
    uplevelButton->addClickEventListener(CC_CALLBACK_0(HallPetLayer::clickUplevel, this));
    fragBar = (LoadingBar*)maxFlagText->getChildByName("bar");
    fragCountText = (Text*)fragBar->getChildByName("count");
    plusHpText = (Text*)maxFlagText->getChildByName("hp_text");
    plusAttackText = (Text*)maxFlagText->getChildByName("attack_text");
    plusDefenceText = (Text*)maxFlagText->getChildByName("defence_text");
    maskNode = (Node*)petIcon->getChildByName("mask_node");
    maskAction = CSLoader::createTimeline(HALL_PET_MASK_UI);
    maskNode->runAction(maskAction);
    
    auto valueBg = (ImageView*)rootBg->getChildByName("value_bg");
    petHpText = (Text*)valueBg->getChildByName("hp_text");
    petAttackText = (Text*)valueBg->getChildByName("attack_text");
    petDefenceText = (Text*)valueBg->getChildByName("defence_text");
    
    fighPetBg = (ImageView*)rootBg->getChildByName("fight_bg");
    for (int i = 0; i < 3; i++) {
        auto petBg = (ImageView*)fighPetBg->getChildByTag(i+1);
        auto removeButton = (Button*)petBg->getChildByName("remove_button");
        removeButton->addClickEventListener(CC_CALLBACK_1(HallPetLayer::clickRemoveFight, this));
    }
    
    petList = (ListView*)rootBg->getChildByName("monster_list");
    for (int i = 0; i < 10; i++) {
        auto pairBg = (ImageView*)petList->getChildByTag(i/2+1);
        petBg[i] = (ImageView*)pairBg->getChildByTag(i+1);
        auto selButton = (Button*)petBg[i]->getChildByName("sel_button");
        selButton->addClickEventListener(CC_CALLBACK_1(HallPetLayer::clickPet, this));
        auto fightButton = (Button*)petBg[i]->getChildByName("fight_button");
        fightButton->addClickEventListener(CC_CALLBACK_1(HallPetLayer::clickFight, this));
    }
    return true;
}

void HallPetLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_PET info = *static_cast<L2E_SHOW_HALL_PET*>(event->getUserData());
    for (int i = 0; i < 10; i++) {
        auto selButton = (Button*)petBg[i]->getChildByName("sel_button");
        auto tip = (ImageView*)selButton->getChildByName("tip");
        tip->setVisible(info.uplevelTip[i]);
        
        auto petIcon = (ImageView*)petBg[i]->getChildByName("icon");
        petIcon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
        auto petName = (Text*)petBg[i]->getChildByName("name_text");
        petName->setString(info.name[i]);
        auto lockImg = (ImageView*)petBg[i]->getChildByName("lock");
        lockImg->setVisible(info.petLevel[i]<1);
        auto fightButton = (Button*)petBg[i]->getChildByName("fight_button");
        fightButton->setTitleText(StringData::shared()->stringFromKey("pet_go_fight"));
        for (int j = 0; j < 5; j++) {
            auto star = (ImageView*)petBg[i]->getChildByName(GameUtils::format("star%d", j+1));
            star->setVisible(j < info.petLevel[i]);
        }
    }
    
    memcpy(fightPetId, info.fightPetId, sizeof(int)*3);
    for (int i = 0; i < 3; i++) {
        auto bg = (ImageView*)fighPetBg->getChildByTag(i+1);
        auto removeButton = (Button*)bg->getChildByName("remove_button");
        if (info.fightPetId[i] == 0) {
            removeButton->setVisible(false);
        }else{
            removeButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.icon[info.fightPetId[i]-1].c_str()));
            removeButton->setVisible(true);
            auto fightButton = (Button*)petBg[info.fightPetId[i]-1]->getChildByName("fight_button");
            fightButton->setTitleText(StringData::shared()->stringFromKey("relax"));
        }
    }
    
    petHpText->setString(Convert2String(info.totalHp));
    petAttackText->setString(Convert2String(info.totalAttack));
    petDefenceText->setString(Convert2String(info.totalDefence));
    
    petIcon->loadTexture(GameUtils::format("ui/monster/%s", info.currPetImg.c_str()));
    for (int i = 0; i < 5; i++) {
        auto starImg = (ImageView*)petIcon->getChildByName(GameUtils::format("star%d", i+1));
        starImg->setVisible(info.currPetLevel>i);
    }
    skillNameText->setString(info.currPetSkillName);
    if (info.currPetMaxLevel) {
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "max.png"));
//        uplevelText->setString(StringData::shared()->stringFromKey("pet_max_level"));
        maxFlagText->setVisible(false);
    }else{
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "otherzi-08.png"));
//        uplevelText->setString(StringData::shared()->stringFromKey("pet_uplevel"));
        maxFlagText->setVisible(true);
        plusHpText->setString(Convert2String(info.currPetLevelhp));
        plusAttackText->setString(Convert2String(info.currPetLevelAttack));
        plusDefenceText->setString(Convert2String(info.currPetLevelDefence));
        fragBar->setPercent(info.currPetFragCount*100.0/info.currPetMaxFragCount);
        fragCountText->setString(GameUtils::format("%d/%d", info.currPetFragCount, info.currPetMaxFragCount));
        if (info.currPetLevel>=1) {
            uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_upstar"));
            maskNode->setVisible(false);
        }else{
            uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_unlock"));
            maskNode->setVisible(true);
            maskAction->play("play1", false);
        }
    }
    focusPos = info.currPetId;
    auto focusIcon = (ImageView*)petBg[focusPos-1]->getChildByName("icon");
    auto focusRoot = CSLoader::createNode(HALL_PET_RELATION_UI);
    auto focusAction = CSLoader::createTimeline(HALL_PET_RELATION_UI);
    focusRoot->runAction(focusAction);
    focusRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    focusRoot->setPosition(focusIcon->getContentSize()/2);
    focusRoot->setTag(1000);
    focusAction->play("play", true);
    focusIcon->addChild(focusRoot);
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.uplevelTip[focusPos-1]);
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallPetLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void HallPetLayer::updatePetDetail(cocos2d::EventCustom *event)
{
    L2E_SEL_PET_POS info = *static_cast<L2E_SEL_PET_POS*>(event->getUserData());
    petBg[focusPos-1]->getChildByName("icon")->removeChildByTag(1000);
    focusPos = info.currPetId;
    auto lockImg = (ImageView*)petBg[focusPos-1]->getChildByName("lock");
    lockImg->setVisible(info.currPetLevel < 1);
    auto focusIcon = (ImageView*)petBg[focusPos-1]->getChildByName("icon");
    auto focusRoot = CSLoader::createNode(HALL_PET_RELATION_UI);
    auto focusAction = CSLoader::createTimeline(HALL_PET_RELATION_UI);
    focusRoot->runAction(focusAction);
    focusRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    focusRoot->setPosition(focusIcon->getContentSize()/2);
    focusRoot->setTag(1000);
    focusAction->play("play", true);
    focusIcon->addChild(focusRoot);

    if (info.currPetLevel>=1) {
        uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_upstar"));
        maskNode->setVisible(false);
    }else{
        uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_unlock"));
        maskNode->setVisible(true);
        maskAction->play("play1", false);
    }
    petIcon->loadTexture(GameUtils::format("ui/monster/%s", info.currPetImg.c_str()));
    for (int i = 0; i < 5; i++) {
        auto starImg = (ImageView*)petIcon->getChildByName(GameUtils::format("star%d", i+1));
        starImg->setVisible(info.currPetLevel>i);
    }
    skillNameText->setString(info.currPetSkillName);
    if (info.currPetMaxLevel) {
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "max.png"));
        //        uplevelText->setString(StringData::shared()->stringFromKey("pet_max_level"));
        maxFlagText->setVisible(false);
    }else{
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "otherzi-08.png"));
        //        uplevelText->setString(StringData::shared()->stringFromKey("pet_uplevel"));
        maxFlagText->setVisible(true);
        plusHpText->setString(Convert2String(info.currPetLevelhp));
        plusAttackText->setString(Convert2String(info.currPetLevelAttack));
        plusDefenceText->setString(Convert2String(info.currPetLevelDefence));
        fragBar->setPercent(info.currPetFragCount*100.0/info.currPetMaxFragCount);
        fragCountText->setString(GameUtils::format("%d/%d", info.currPetFragCount, info.currPetMaxFragCount));
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.uplevelTip[focusPos-1]);
}

void HallPetLayer::updatePetLevel(cocos2d::EventCustom *event)
{
    L2E_UPDATE_PET_LEVEL info = *static_cast<L2E_UPDATE_PET_LEVEL*>(event->getUserData());
    if(info.errNo > 0)
    {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("pet_uplevel_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }

    for (int j = 0; j < 5; j++) {
        auto star = (ImageView*)petBg[info.currPetId-1]->getChildByName(GameUtils::format("star%d", j+1));
        star->setVisible(j < info.currPetLevel);
    }
    
    petIcon->loadTexture(GameUtils::format("ui/monster/%s", info.currPetImg.c_str()));
    for (int i = 0; i < 5; i++) {
        auto starImg = (ImageView*)petIcon->getChildByName(GameUtils::format("star%d", i+1));
        starImg->setVisible(info.currPetLevel>i);
    }
    skillNameText->setString(info.currPetSkillName);
    if (info.currPetMaxLevel) {
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "max.png"));
        //        uplevelText->setString(StringData::shared()->stringFromKey("pet_max_level"));
        maxFlagText->setVisible(false);
    }else{
        uplevelImg->loadTexture(GameUtils::format(COMMON_DIR, "otherzi-08.png"));
        //        uplevelText->setString(StringData::shared()->stringFromKey("pet_uplevel"));
        maxFlagText->setVisible(true);
        plusHpText->setString(Convert2String(info.currPetLevelhp));
        plusAttackText->setString(Convert2String(info.currPetLevelAttack));
        plusDefenceText->setString(Convert2String(info.currPetLevelDefence));
        fragBar->setPercent(info.currPetFragCount*100.0/info.currPetMaxFragCount);
        fragCountText->setString(GameUtils::format("%d/%d", info.currPetFragCount, info.currPetMaxFragCount));
    }
    petHpText->setString(Convert2String(info.totalHp));
    petAttackText->setString(Convert2String(info.totalAttack));
    petDefenceText->setString(Convert2String(info.totalDefence));
    
    auto lockImg = (ImageView*)petBg[focusPos-1]->getChildByName("lock");
    lockImg->setVisible(info.currPetLevel < 1);
    if (info.currPetLevel>=1) {
        uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_upstar"));
        if (info.currPetLevel==1) {
            maskNode->setVisible(true);
            maskAction->play("play2", false);
        }else{
            maskNode->setVisible(false);
        }
        
    }else{
        uplevelButton->setTitleText(StringData::shared()->stringFromKey("pet_unlock"));
        maskNode->setVisible(true);
        maskAction->play("play1", false);
    }
}

void HallPetLayer::updateFightPet(cocos2d::EventCustom *event)
{
    L2E_UPDATE_FIGHT_PET info = *static_cast<L2E_UPDATE_FIGHT_PET*>(event->getUserData());
    if(info.errNo > 0)
    {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("pet_fight_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    for (int i = 0; i < 10; i++) {
        auto fightButton = (Button*)petBg[i]->getChildByName("fight_button");
        fightButton->setTitleText(StringData::shared()->stringFromKey("pet_go_fight"));
    }

    memcpy(fightPetId, info.fightPetId, sizeof(int)*3);

    for (int i = 0; i < 3; i++) {
        auto bg = (ImageView*)fighPetBg->getChildByTag(i+1);
        auto removeButton = (Button*)bg->getChildByName("remove_button");
        if (info.fightPetId[i] == 0) {
            removeButton->setVisible(false);
        }else{
            removeButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            removeButton->setVisible(true);
            auto fightButton = (Button*)petBg[info.fightPetId[i]-1]->getChildByName("fight_button");
            fightButton->setTitleText(StringData::shared()->stringFromKey("relax"));
        }
    }

}

void HallPetLayer::clickClose()
{
    if (plotButtonId == 3) {
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
        infoTrigger.index = PET_LAYER;
        infoTrigger.buttonId = 3;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallPetLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallPetLayer::removeOff()
{
    removeFromParent();
}

void HallPetLayer::clickPet(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    int tag = ((Button*)pSender)->getParent()->getTag();
    if (focusPos == tag) {
        return;
    }
    
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_sel_pet_pos;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetLayer::clickUplevel()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
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
        infoTrigger.index = PET_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    E2L_COMMON info;
    info.eProtocol = e2l_click_pet_uplevel;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetLayer::clickFight(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    if (plotButtonId == 2) {
        if (tag != focusPos) {
            return;
        }
        ((Button*)pSender)->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = PET_LAYER;
        infoTrigger.buttonId = 2;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    int pos = 0;
    for (int i = 0; i < 3; i++) {
        if (tag == fightPetId[i]) {
            pos = i+1;
            break;
        }
    }
    
    E2L_UPDATE_ONE_VALUE info;
    if (pos != 0) {
        info.eProtocol = e2l_remove_fight_pet;
        info.value = pos;
    }else{
        info.eProtocol = e2l_add_fight_pet;
        info.value = tag;
    }
    ClientLogic::instance()->ProcessUIRequest(&info);
    
}

void HallPetLayer::clickRemoveFight(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    int tag = ((Button*)pSender)->getParent()->getTag();

    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_remove_fight_pet;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
}

void HallPetLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] != PET_LAYER)
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
                    
                    auto button = (Button*)petBg[focusPos-1]->getChildByName("fight_button");
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    uplevelButton->addChild(point);
                }
                    break;
                case 2:
                {
                    auto button = (Button*)petBg[focusPos-1]->getChildByName("fight_button");
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(button->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    button->addChild(point);
                    
                }
                    break;
                case 3:
                {
                    auto button = (Button*)petBg[focusPos-1]->getChildByName("fight_button");
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(closeButton->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
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

void HallPetLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = PET_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetLayer::updateTip(cocos2d::EventCustom *event)
{
    L2E_UPDATE_PET_TIP info = *static_cast<L2E_UPDATE_PET_TIP*>(event->getUserData());
    for (int i = 0; i < 10; i++) {
        auto pairBg = (ImageView*)petList->getChildByTag(i/2+1);
        petBg[i] = (ImageView*)pairBg->getChildByTag(i+1);
        auto selButton = (Button*)petBg[i]->getChildByName("sel_button");
        auto tip = (ImageView*)selButton->getChildByName("tip");
        tip->setVisible(info.uplevelTip[i]);
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.uplevelTip[focusPos-1]);
}
