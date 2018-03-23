//
//  HallHeroLayer.cpp
//  life
//
//  Created by ff on 17/7/24.
//
//

#include "HallHeroLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "../node/TipsNode.h"
#include "ItemWayLayer.h"
#include "ItemSweepLayer.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;


HallHeroLayer::HallHeroLayer():
focusPos(0),
plotButtonId(0),
plotMissionId(0)
{
}

void HallHeroLayer::onEnter()
{
    Layer::onEnter();
    updateEquipDetailListener = EventListenerCustom::create(SEL_EQUIP_POS, CC_CALLBACK_1(HallHeroLayer::updateEquipDetail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateEquipDetailListener, -1);
    updateEquipLevelListener = EventListenerCustom::create(UPDATE_EQUIP_LEVEL, CC_CALLBACK_1(HallHeroLayer::updateEquipLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateEquipLevelListener, -1);
    updateEquipStarListener = EventListenerCustom::create(UPDATE_EQUIP_STAR, CC_CALLBACK_1(HallHeroLayer::updateEquipStar, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateEquipStarListener, -1);
    updateItemCountListener = EventListenerCustom::create(UPDATE_ITEM_COUNT, CC_CALLBACK_1(HallHeroLayer::updateItemCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateItemCountListener, -1);
    showItemWayListener = EventListenerCustom::create(SHOW_ITEM_WAY, CC_CALLBACK_1(HallHeroLayer::showItemWay, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showItemWayListener, -1);
    showItemSweepListener = EventListenerCustom::create(SHOW_ITEM_SWEEP, CC_CALLBACK_1(HallHeroLayer::showItemSweep, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showItemSweepListener, -1);
    updateGoldListener = EventListenerCustom::create(UPDATE_GOLD, CC_CALLBACK_1(HallHeroLayer::updateTotalGold, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGoldListener, -1);
    updateExpListener = EventListenerCustom::create(UPDATE_EXP, CC_CALLBACK_1(HallHeroLayer::updateExp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateExpListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallHeroLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    updateTipListener = EventListenerCustom::create(UPDATE_EQUIP_TIP, CC_CALLBACK_1(HallHeroLayer::updateTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTipListener, -1);
}

HallHeroLayer::~HallHeroLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateEquipDetailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateEquipLevelListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateEquipStarListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateItemCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showItemWayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showItemSweepListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateExpListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTipListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallHeroLayer::init()
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

    root = CSLoader::createNode(HALL_HERO_UI);
    rootAction = CSLoader::createTimeline(HALL_HERO_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallHeroLayer::clickClose, this));
    return true;
}
void HallHeroLayer::updateTotalGold(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (info.value > 100000) {
        totalGoldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.value/10000).c_str());
    }else{
        totalGoldText->setString(Convert2String(info.value));
    }
}
void HallHeroLayer::updateExp(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EXP info = *static_cast<L2E_UPDATE_EXP*>(event->getUserData());
    float expPercent = info.currExp*100.0/info.maxExp;
    expBar->setPercent(expPercent);
    levelAtlas->setString(Convert2String(info.level));
    forceText->setString(Convert2String(info.force));
    hpText->setString(Convert2String(info.hp));
    attackText->setString(Convert2String(info.attack));
    defenceText->setString(Convert2String(info.defence));
    critText->setString(Convert2String(info.crit));
    decritText->setString(Convert2String(info.deCrit));
}
void HallHeroLayer::updateEquipDetail(cocos2d::EventCustom *event)
{
    L2E_SEL_EQUIP_POS info = *static_cast<L2E_SEL_EQUIP_POS*>(event->getUserData());
    equipIcon->loadTexture(GameUtils::format(COMMON_DIR, info.currEquipIcon.c_str()));
    equipLevel->setString(Convert2String(info.currEquipLevel));
    equipNameText->setString(Convert2String(info.currEquipName));
    equipHpText->setString(Convert2String(info.currEquipHp));
    equipAttackText->setString(Convert2String(info.currEquipAttack));
    equipDefenceText->setString(Convert2String(info.currEquipDefence));
    for (int j = 0; j < 3; j++) {
        auto starImg = (ImageView*)equipIcon->getChildByName(GameUtils::format("star%d", j+1));
        starImg->setVisible(info.currEquipStar >= j+1);
    }

    
    uplevelText->setVisible(!info.maxLevel);
    plusHpText->setString(Convert2String(info.plusEquipHp));
    plusAttackText->setString(Convert2String(info.plusEquipAttack));
    plusDefenceText->setString(Convert2String(info.plusEquipDefence));
    costGoldText->setString(Convert2String(info.costGold));

    for (int i = 0; i < 6; i++) {
        auto sweepButton = (Button*)upGradeBg->getChildByTag(i+1);
        if (info.starItemIcon[i] == "") {
            sweepButton->setVisible(false);
        }else{
            sweepButton->setVisible(true);
            sweepButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.starItemIcon[i].c_str()));
            auto countText = (Text*)sweepButton->getChildByName("count_text");
            countText->setString(GameUtils::format("%d/%d", info.starItemCount[i], info.starItemRequire[i]));
            if (info.starItemCount[i] < info.starItemRequire[i]) {
                countText->setTextColor(Color4B::RED);
            }else{
                countText->setTextColor(Color4B(0, 223, 40, 255));
            }
        }
        
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    auto upgradeTip = (ImageView*)upGradeButton->getChildByName("tip");
    uplevelTip->setVisible(info.posUplevelTip[focusPos-1]);
    upgradeTip->setVisible(info.posUpgradeTip[focusPos-1]);
}

void HallHeroLayer::updateItemCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ITEM_COUNT info = *static_cast<L2E_UPDATE_ITEM_COUNT*>(event->getUserData());
    if (info.errNo != 0) {
        return;
    }
    auto sweepButton = (Button*)upGradeBg->getChildByTag(info.itemTag);
    auto countText = (Text*)sweepButton->getChildByName("count_text");
    countText->setString(GameUtils::format("%d/%d", info.count, info.requireCount));
    if (info.count < info.requireCount) {
        countText->setTextColor(Color4B::RED);
    }else{
        countText->setTextColor(Color4B(0, 223, 40, 255));
    }

    
}
void HallHeroLayer::updateEquipStar(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EQUIP_STAR info = *static_cast<L2E_UPDATE_EQUIP_STAR*>(event->getUserData());
    if (info.errNo != 0) {
        auto errStr = StringData::shared()->stringFromKey(GameUtils::format("equip_upgrade_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    equipIcon->loadTexture(GameUtils::format(COMMON_DIR, info.currEquipIcon.c_str()));
    equipNameText->setString(Convert2String(info.currEquipName));
    equipHpText->setString(Convert2String(info.currEquipHp));
    equipAttackText->setString(Convert2String(info.currEquipAttack));
    equipDefenceText->setString(Convert2String(info.currEquipDefence));
    for (int j = 0; j < 3; j++) {
        auto starImg = (ImageView*)equipIcon->getChildByName(GameUtils::format("star%d", j+1));
        starImg->setVisible(info.currEquipStar >= j+1);
    }

    
    for (int i = 0; i < 6; i++) {
        auto sweepButton = (Button*)upGradeBg->getChildByTag(i+1);
        if (info.starItemIcon[i] == "") {
            sweepButton->setVisible(false);
        }else{
            sweepButton->setVisible(true);
            sweepButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.starItemIcon[i].c_str()));
            auto countText = (Text*)sweepButton->getChildByName("count_text");
            countText->setString(GameUtils::format("%d/%d", info.starItemCount[i], info.starItemRequire[i]));
            if (info.starItemCount[i] < info.starItemRequire[i]) {
                countText->setTextColor(Color4B::RED);
            }else{
                countText->setTextColor(Color4B(0, 223, 40, 255));
            }
        }
        
    }

    auto equipImg = (ImageView*)armatureBg->getChildByTag(info.currPos);
    equipImg->loadTexture(GameUtils::format(COMMON_DIR, info.currEquipIcon.c_str()));
    for (int j = 0; j < 3; j++) {
        auto starImg = (ImageView*)equipImg->getChildByName(GameUtils::format("star%d", j+1));
        starImg->setVisible(info.currEquipStar >= j+1);
    }
    
    forceText->setString(Convert2String(info.force));
    hpText->setString(Convert2String(info.totalHp));
    attackText->setString(Convert2String(info.totalAttack));
    defenceText->setString(Convert2String(info.totalDefence));

}
void HallHeroLayer::updateEquipLevel(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EQUIP_LEVEL info = *static_cast<L2E_UPDATE_EQUIP_LEVEL*>(event->getUserData());
    if (info.errNo != 0) {
        auto errStr = StringData::shared()->stringFromKey(GameUtils::format("equip_uplevel_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    equipLevel->setString(Convert2String(info.currEquipLevel));
    equipHpText->setString(Convert2String(info.currEquipHp));
    equipAttackText->setString(Convert2String(info.currEquipAttack));
    equipDefenceText->setString(Convert2String(info.currEquipDefence));
    
    uplevelText->setVisible(!info.maxLevel);
    plusHpText->setString(Convert2String(info.plusEquipHp));
    plusAttackText->setString(Convert2String(info.plusEquipAttack));
    plusDefenceText->setString(Convert2String(info.plusEquipDefence));
    costGoldText->setString(Convert2String(info.costGold));

    auto equipImg = (ImageView*)armatureBg->getChildByTag(info.currPos);
    auto levelText = (Text*)equipImg->getChildByName("level_text");
    levelText->setString(Convert2String(info.currEquipLevel));
    forceText->setString(Convert2String(info.force));
    hpText->setString(Convert2String(info.totalHp));
    attackText->setString(Convert2String(info.totalAttack));
    defenceText->setString(Convert2String(info.totalDefence));

}

void HallHeroLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_HERO info = *static_cast<L2E_SHOW_HALL_HERO*>(event->getUserData());
    leftBg = (ImageView*)root->getChildByName("bg_img")->getChildByName("left_bg");
    armatureBg = (ImageView*)leftBg->getChildByName("armature_bg");
    levelAtlas = (TextAtlas*)armatureBg->getChildByName("level_atlas");
    auto platform = (ImageView*)armatureBg->getChildByName("platform_img");
    expBar = (LoadingBar*)armatureBg->getChildByName("exp_bg")->getChildByName("exp_bar");
    forceText = (Text*)leftBg->getChildByName("force_text");
    levelAtlas->setString(Convert2String(info.level));
    forceText->setString(Convert2String(info.force));
    float expPercent = info.exp*100.0/info.maxExp;
    expBar->setPercent(expPercent);
    
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            info.armature.c_str(),
//                                            info.armature.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            info.armature.c_str());

    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    auto armature = Armature::create(info.armature);
    armature->setScale(0.7);
    armature->getAnimation()->play("wait1");
    armature->setPosition(platform->getContentSize()/2);
    platform->addChild(armature,10);
    
    for (int i = 0; i < 6; i++) {
        auto equipImg = (ImageView*)armatureBg->getChildByTag(i+1);
        equipImg->loadTexture(GameUtils::format(COMMON_DIR, info.equipIcon[i].c_str()));
        auto levelText = (Text*)equipImg->getChildByName("level_text");
        levelText->setString(Convert2String(info.equipLevel[i]));
        for (int j = 0; j < 3; j++) {
            auto starImg = (ImageView*)equipImg->getChildByName(GameUtils::format("star%d", j+1));
            starImg->setVisible(info.equipStar[i] >= j+1);
        }
        
        auto selButton = (Button*)equipImg->getChildByName("sel_button");
        selButton->addClickEventListener(CC_CALLBACK_1(HallHeroLayer::clickPosButton, this));
        if (i+1 == info.currPos) {
            auto focus = CSLoader::createNode(EQUIP_FOCUS_UI);
            auto focusAction = CSLoader::createTimeline(EQUIP_FOCUS_UI);
            focus->runAction(focusAction);
            focus->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            focus->setPosition(equipImg->getContentSize()/2);
//            focus->setTag(100);
            equipImg->addChild(focus,10,100);
            focusPos = i+1;
            focusAction->play("animation0", true);
        }
    }
    
    
    valueBg = (ImageView*)leftBg->getChildByName("value_bg");
    hpText = (Text*)valueBg->getChildByName("hp_bg")->getChildByName("value_text");
    attackText = (Text*)valueBg->getChildByName("attack_bg")->getChildByName("value_text");
    defenceText = (Text*)valueBg->getChildByName("defence_bg")->getChildByName("value_text");
    critText = (Text*)valueBg->getChildByName("crit_bg")->getChildByName("value_text");
    decritText = (Text*)valueBg->getChildByName("decrit_bg")->getChildByName("value_text");
    hpText->setString(Convert2String(info.totalHp));
    attackText->setString(Convert2String(info.totalAttack));
    defenceText->setString(Convert2String(info.totalDefence));
    critText->setString(Convert2String(info.totalCrit));
    decritText->setString(Convert2String(info.totalDeCrit));
    
    rightBg = (ImageView*)root->getChildByName("bg_img")->getChildByName("right_bg");
    totalGoldText = (Text*)rightBg->getChildByName("gold_bg")->getChildByName("gold")->getChildByName("count_text");
    if (info.totalGold > 100000) {
        totalGoldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.totalGold/10000).c_str());
    }else{
        totalGoldText->setString(Convert2String(info.totalGold));
    }
    
    equipIcon = (ImageView*)rightBg->getChildByName("icon_img");
    equipLevel = (Text*)equipIcon->getChildByName("level_text");
    equipNameText = (Text*)rightBg->getChildByName("name_text");
    equipHpText = (Text*)rightBg->getChildByName("hp_text");
    equipAttackText = (Text*)rightBg->getChildByName("attack_text");
    equipDefenceText = (Text*)rightBg->getChildByName("defence_text");
    equipIcon->loadTexture(GameUtils::format(COMMON_DIR, info.currEquipIcon.c_str()));
    equipLevel->setString(Convert2String(info.currEquipLevel));
    equipNameText->setString(Convert2String(info.currEquipName));
    equipHpText->setString(Convert2String(info.currEquipHp));
    equipAttackText->setString(Convert2String(info.currEquipAttack));
    equipDefenceText->setString(Convert2String(info.currEquipDefence));
    for (int j = 0; j < 3; j++) {
        auto starImg = (ImageView*)equipIcon->getChildByName(GameUtils::format("star%d", j+1));
        starImg->setVisible(info.currEquipStar >= j+1);
    }
    
    uplevelText = (ImageView*)rightBg->getChildByName("uplevel_text");
    plusHpText = (Text*)uplevelText->getChildByName("plus_hp_text");
    plusAttackText = (Text*)uplevelText->getChildByName("plus_attack_text");
    plusDefenceText = (Text*)uplevelText->getChildByName("plus_defence_text");
    costGoldText = (Text*)uplevelText->getChildByName("cost_gold")->getChildByName("count_text");
    uplevelButton = (Button*)uplevelText->getChildByName("uplevel_button");
    uplevelButton->addClickEventListener(CC_CALLBACK_0(HallHeroLayer::clickUpLevelButton, this));
    uplevelText->setVisible(!info.maxLevel);
    plusHpText->setString(Convert2String(info.plusEquipHp));
    plusAttackText->setString(Convert2String(info.plusEquipAttack));
    plusDefenceText->setString(Convert2String(info.plusEquipDefence));
    costGoldText->setString(Convert2String(info.costGold));
    
    upGradeBg = (ImageView*)rightBg->getChildByName("upgrade_bg");
    upGradeButton = (Button*)upGradeBg->getChildByName("tip_text")->getChildByName("upgrade_button");
    upGradeButton->addClickEventListener(CC_CALLBACK_0(HallHeroLayer::clickUpGradeButton, this));
    for (int i = 0; i < 6; i++) {
        auto sweepButton = (Button*)upGradeBg->getChildByTag(i+1);
        if (info.starItemIcon[i] == "") {
            sweepButton->setVisible(false);
        }else{
            sweepButton->setVisible(true);
            sweepButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.starItemIcon[i].c_str()));
            sweepButton->addClickEventListener(CC_CALLBACK_1(HallHeroLayer::clickItemButton, this));
            auto countText = (Text*)sweepButton->getChildByName("count_text");
            countText->setString(GameUtils::format("%d/%d", info.starItemCount[i], info.starItemRequire[i]));
            if (info.starItemCount[i] < info.starItemRequire[i]) {
                countText->setTextColor(Color4B::RED);
            }else{
                countText->setTextColor(Color4B(0, 223, 40, 255));
            }
        }
    }
    
    for (int i = 0; i < 6; i++) {
        auto equipImg = (ImageView*)armatureBg->getChildByTag(i+1);
        auto selButton = (Button*)equipImg->getChildByName("sel_button");
        auto tip = (ImageView*)selButton->getChildByName("tip");
        tip->setVisible(info.posTip[i]);
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    auto upgradeTip = (ImageView*)upGradeButton->getChildByName("tip");
    uplevelTip->setVisible(info.posUplevelTip[focusPos-1]);
    upgradeTip->setVisible(info.posUpgradeTip[focusPos-1]);

    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallHeroLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void HallHeroLayer::showItemWay(cocos2d::EventCustom *event)
{
    auto layer = ItemWayLayer::create();
    layer->setupView(event);
    addChild(layer);
}
void HallHeroLayer::showItemSweep(cocos2d::EventCustom *event)
{
    auto layer = ItemSweepLayer::create();
    layer->setupView(event);
    addChild(layer);
}
void HallHeroLayer::clickClose()
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
        infoTrigger.index = HERO_LAYER;
        infoTrigger.buttonId = 3;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);

    }else if (plotButtonId != 0) {
        return;
    }

    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallHeroLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallHeroLayer::removeOff()
{
    removeFromParent();
}

void HallHeroLayer::clickPosButton(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    auto button = ((Button*)pSender);
    int tag = button->getParent()->getTag();
    if (plotButtonId == 1) {
        armatureBg->getChildByTag(focusPos)->removeChildByTag(1000);

        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = HERO_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    if (tag == focusPos) {
        return;
    }
    
    armatureBg->getChildByTag(focusPos)->removeChildByTag(100);
    auto focus = CSLoader::createNode(EQUIP_FOCUS_UI);
    auto focusAction = CSLoader::createTimeline(EQUIP_FOCUS_UI);
    focus->runAction(focusAction);
    focus->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    focus->setPosition(button->getParent()->getContentSize()/2);
//    focus->setTag(100);
    button->getParent()->addChild(focus,10,100);
    focusAction->play("animation0", true);
    focusPos = tag;
    
    E2L_SEL_EQUIP_POS info;
    info.eProtocol = e2l_sel_equip_pos;
    info.pos = focusPos;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHeroLayer::clickUpLevelButton()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId == 2) {
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
        infoTrigger.index = HERO_LAYER;
        infoTrigger.buttonId = 2;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);

    }else if (plotButtonId != 0) {
        return;
    }

    E2L_COMMON info;
    info.eProtocol = e2l_click_equip_uplevel;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHeroLayer::clickUpGradeButton()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId == 4) {
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
        infoTrigger.index = HERO_LAYER;
        infoTrigger.buttonId = 4;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    E2L_COMMON info;
    info.eProtocol = e2l_click_equip_upgrade;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHeroLayer::clickItemButton(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    auto button = (Button*)pSender;
    int tag = button->getTag();
    if (plotButtonId == 5) {
        if (tag != 1) {
            return;
        }
        button->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = HERO_LAYER;
        infoTrigger.buttonId = 5;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_equip_item;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHeroLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] != HERO_LAYER)
            {
                return;
            }
            
            plotButtonId = info.param[3];
            plotMissionId = info.missionId;
            
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
                    auto button = armatureBg->getChildByTag(focusPos);
                    auto sweepButton = (Button*)upGradeBg->getChildByTag(1);
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(button->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    upGradeButton->removeChildByTag(1000);
                    sweepButton->removeChildByTag(1000);
                    button->addChild(point);
                }
                    break;
                case 2:
                {
                    auto button = armatureBg->getChildByTag(focusPos);
                    auto sweepButton = (Button*)upGradeBg->getChildByTag(1);
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(uplevelButton->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    upGradeButton->removeChildByTag(1000);
                    sweepButton->removeChildByTag(1000);
                    uplevelButton->addChild(point);
                    
                }
                    break;
                case 3:
                {
                    auto button = armatureBg->getChildByTag(focusPos);
                    auto sweepButton = (Button*)upGradeBg->getChildByTag(1);
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(closeButton->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    upGradeButton->removeChildByTag(1000);
                    sweepButton->removeChildByTag(1000);
                    closeButton->addChild(point);
                }
                    break;
                case 4:
                {
                    auto button = armatureBg->getChildByTag(focusPos);
                    auto sweepButton = (Button*)upGradeBg->getChildByTag(1);
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(upGradeButton->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    upGradeButton->removeChildByTag(1000);
                    sweepButton->removeChildByTag(1000);
                    upGradeButton->addChild(point);
                }
                    break;
                case 5:
                {
                    auto button = armatureBg->getChildByTag(focusPos);
                    auto sweepButton = (Button*)upGradeBg->getChildByTag(1);
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(sweepButton->getContentSize()/2);
                    point->setTag(1000);
                    button->removeChildByTag(1000);
                    uplevelButton->removeChildByTag(1000);
                    closeButton->removeChildByTag(1000);
                    upGradeButton->removeChildByTag(1000);
                    sweepButton->removeChildByTag(1000);
                    sweepButton->addChild(point);
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

void HallHeroLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = HERO_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallHeroLayer::updateTip(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EQUIP_TIP info = *static_cast<L2E_UPDATE_EQUIP_TIP*>(event->getUserData());
    for (int i = 0; i < 6; i++) {
        auto equipImg = (ImageView*)armatureBg->getChildByTag(i+1);
        auto selButton = (Button*)equipImg->getChildByName("sel_button");
        auto tip = (ImageView*)selButton->getChildByName("tip");
        tip->setVisible(info.posTip[i]);
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    auto upgradeTip = (ImageView*)upGradeButton->getChildByName("tip");
    uplevelTip->setVisible(info.posUplevelTip[focusPos-1]);
    upgradeTip->setVisible(info.posUpgradeTip[focusPos-1]);
}
