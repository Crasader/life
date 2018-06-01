//
//  CampaignLayer.cpp
//  life
//
//  Created by ff on 17/11/2.
//
//

#include "CampaignLayer.h"
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

CampaignLayer::CampaignLayer():
plotButtonId(0),
plotMissionId(0)
{
}
void CampaignLayer::onEnter()
{
    Layer::onEnter();
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(CampaignLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    updateEverydayListener = EventListenerCustom::create(UPDATE_EVERYDAY, CC_CALLBACK_1(CampaignLayer::updateEveryday, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateEverydayListener, -1);
    showEverydayListener = EventListenerCustom::create(SHOW_EVERYDAY, CC_CALLBACK_1(CampaignLayer::updateEveryday, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showEverydayListener, -1);
    updateFirstRechargeListener = EventListenerCustom::create(UPDATE_FIRST_RECHARGE, CC_CALLBACK_1(CampaignLayer::updateFirstRecharge, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateFirstRechargeListener, -1);
    hideListener = EventListenerCustom::create(HIDE_CAMPAIGN, CC_CALLBACK_1(CampaignLayer::hide, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideListener, -1);
    showActiveListener = EventListenerCustom::create(SHOW_CAMPAIGN_ACTIVE, CC_CALLBACK_1(CampaignLayer::showActive, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showActiveListener, -1);
    updateActiveBoundListener = EventListenerCustom::create(UPDATE_CAMPAIGN_MISSION_BOUND, CC_CALLBACK_1(CampaignLayer::updateActiveBound, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateActiveBoundListener, -1);
    updateActiveBoxListener = EventListenerCustom::create(UPDATE_CAMPAIGN_ACTIVE_BOX, CC_CALLBACK_1(CampaignLayer::updateActiveBox, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateActiveBoxListener, -1);
    refreshActiveListener = EventListenerCustom::create(UPDATE_CAMPAIGN_ACTIVE, CC_CALLBACK_1(CampaignLayer::updateActive, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(refreshActiveListener, -1);
    showBoundListener = EventListenerCustom::create(SHOW_CAMPAIGN_BOUND_MISSION, CC_CALLBACK_1(CampaignLayer::showBoundMission, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBoundListener, -1);
    updateTipListener = EventListenerCustom::create(UPDATE_CAMPAIGN_TIP, CC_CALLBACK_1(CampaignLayer::updateTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTipListener, -1);
    jumpLockedListener = EventListenerCustom::create(JUMP_LOCKED, CC_CALLBACK_1(CampaignLayer::jumpLocked, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(jumpLockedListener, -1);
    
    updateMonthAssignListener = EventListenerCustom::create(UPDATE_CAMPAIGN_MONTH_ASSIGN, CC_CALLBACK_1(CampaignLayer::updateMonthAssign, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateMonthAssignListener, -1);
}
CampaignLayer::~CampaignLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateEverydayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showEverydayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateFirstRechargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showActiveListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateActiveBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateActiveBoxListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(refreshActiveListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(jumpLockedListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateMonthAssignListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool CampaignLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_CAMPAIGN_UI);
    rootAction = CSLoader::createTimeline(HALL_CAMPAIGN_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickClose, this));
    boundButton = (Button*)rootBg->getChildByName("bound_button");
    boundButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickBound, this));
    boundTip = (ImageView*)boundButton->getChildByName("tip");
    rechargeButton = (Button*)rootBg->getChildByName("recharge_button");
    rechargeButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickRecharge, this));
    missionButton = (Button*)rootBg->getChildByName("everyday_mission_button");
    missionButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickMission, this));
    missionTip = (ImageView*)missionButton->getChildByName("tip");
    everydayButton = (Button*)rootBg->getChildByName("everyday_button");
    everydayButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickEveryday, this));
    monthAssignButton = (Button*)rootBg->getChildByName("month_assign_button");
    monthAssignButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickMonthAssign, this));
    monthAssignTip = (ImageView*)monthAssignButton->getChildByName("tip");
    
    everydayBg = (ImageView*)rootBg->getChildByName("everyday_bound_img");
    missionBg = (ImageView*)rootBg->getChildByName("everyday_mission_img");
    firstRechargeBg = (ImageView*)rootBg->getChildByName("first_rechage_img");
    boundMissionBg = (ImageView*)rootBg->getChildByName("bound_mission_img");
    monthAssignBg = (ImageView*)rootBg->getChildByName("month_assign_img");

    takeFirstRecharge = (Button*)firstRechargeBg->getChildByName("recharge_button");
    takeFirstRecharge->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickTakeRecharge, this));
    
    signButton = (Button*)monthAssignBg->getChildByName("ok_button");
    signButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickMonthSign, this));
    
    auto platform = (Node*)firstRechargeBg->getChildByName("platform_node");
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            "shenyuanboss7",
//                                            "shenyuanboss7");
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            "shenyuanboss7");
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    auto armature = Armature::create("shenyuanboss7");
    armature->setScale(0.7);
    armature->getAnimation()->play("wait");
    armature->setPosition(platform->getContentSize()/2);
    platform->addChild(armature,10);
    
    activeBar = (LoadingBar*)missionBg->getChildByName("active_bg")->getChildByName("active_loadbar");
    missionList = (ListView*)missionBg->getChildByName("active_mission_list");
    totalActiveText = (Text*)missionBg->getChildByName("total_active_text");
    for (int i = 0; i < 4; i++) {
        boxButton[i] = (Button*)missionBg->getChildByName(GameUtils::format("box%d_button", i+1));
        boxParticle[i] = (ParticleSystemQuad*)boxButton[i]->getChildByName("particle");
        boxButton[i]->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickActiveBox, this));
    }
    for (int i = 0; i < 4 ; i++) {
        auto thresText = (Text*)missionBg->getChildByName(GameUtils::format("thres%d_text", i+1));
        thresText->setString(Convert2String(activeBoxVal[i]));
    }
    
    boundMissionList = (ListView*)boundMissionBg->getChildByName("mission_list");

    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(CampaignLayer::endIn, this));
    return true;
}

void CampaignLayer::updateEveryday(cocos2d::EventCustom *event)
{
    L2E_SHOW_EVERYDAY info = *static_cast<L2E_SHOW_EVERYDAY*>(event->getUserData());
    everydayBg->setVisible(true);
    monthAssignBg->setVisible(false);
    missionBg->setVisible(false);
    firstRechargeBg->setVisible(false);
    boundMissionBg->setVisible(false);
    everydayButton->setEnabled(false);
    everydayButton->setBright(false);
    monthAssignButton->setEnabled(true);
    monthAssignButton->setBright(true);
    missionButton->setEnabled(true);
    missionButton->setBright(true);
    rechargeButton->setEnabled(true);
    rechargeButton->setBright(true);
    boundButton->setEnabled(true);
    boundButton->setBright(true);
    boundTip->setVisible(info.boundTip);
    missionTip->setVisible(info.activeTip);
    monthAssignTip->setVisible(info.monthTip);
    for (int i = 0; i < 7; i++) {
        auto button = (Button*)everydayBg->getChildByTag(i+1);
        button->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickTakeEveryday, this));
        auto countText = (Text*)button->getChildByName("count_text");
        countText->setString(Convert2String(info.count[i]));
        auto icon = (ImageView*)button->getChildByName("icon");
        icon->setUnifySizeEnabled(true);
        icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
//        bool unify = icon->isUnifySizeEnabled();
        auto nameText = (Text*)button->getChildByName("name_text");
        nameText->setString(info.name[i]);
        auto stateImg = (ImageView*)button->getChildByName("state_img");
        stateImg->setVisible((i+1) < info.everydayId);
        if (info.everydayId == (i+1))
        {
            if (info.everydayState == 1) {
                button->loadTextureNormal(GameUtils::format(COMMON_DIR, "7daydiban1.png"));
                button->setEnabled(true);
            }else{
                button->loadTextureNormal(GameUtils::format(COMMON_DIR, "7daydiban2.png"));
                button->setEnabled(false);
            }
        }else{
            button->loadTextureNormal(GameUtils::format(COMMON_DIR, "7daydiban2.png"));
            button->setEnabled(false);
        }
    }
}
void CampaignLayer::updateMonthAssign(cocos2d::EventCustom *event)
{
    everydayBg->setVisible(false);
    missionBg->setVisible(false);
    monthAssignBg->setVisible(true);
    firstRechargeBg->setVisible(false);
    boundMissionBg->setVisible(false);
    everydayButton->setEnabled(true);
    everydayButton->setBright(true);
    monthAssignButton->setEnabled(false);
    monthAssignButton->setBright(false);
    missionButton->setEnabled(true);
    missionButton->setBright(true);
    rechargeButton->setEnabled(true);
    rechargeButton->setBright(true);
    boundButton->setEnabled(true);
    boundButton->setBright(true);
    
    L2E_SHOW_MONTH_ASSIGN info = *static_cast<L2E_SHOW_MONTH_ASSIGN*>(event->getUserData());
    for (int i = 0; i < info.configCount; i++) {
        if(monthAssignBg->getChildByTag(i+1) == nullptr) {
            break;
        }
        auto boundIcon = (ImageView*)monthAssignBg->getChildByName(GameUtils::format("bound_icon_%d",i+1));
        boundIcon->loadTexture(GameUtils::format(COMMON_DIR,info.icon[i].c_str()));
        
        auto takeFlag = (ImageView*)boundIcon->getChildByName("taken_flag");
        if (info.monthAssignId == i+1) {
            takeFlag->setVisible(info.monthAssignState == 2);
        }else{
            takeFlag->setVisible(info.monthAssignId>i+1);
        }
        
        
        auto nameLabel = (Text*)boundIcon->getChildByName("name_label");
        auto countLabel = (Text*)boundIcon->getChildByName("count_label");
        nameLabel->setString(info.name[i]);
        countLabel->setString(GameUtils::format("X%d",info.count[i]));


        auto vipLable = (Text*)boundIcon->getChildByName("vip_limit_label");
        auto doubleImg = (ImageView*)boundIcon->getChildByName("vip_bg");
        if (info.vipLimit[i] < 1) {
            vipLable->setVisible(false);
            doubleImg->setVisible(false);
        }else{
            vipLable->setVisible(true);
            doubleImg->setVisible(true);
            vipLable->setString(GameUtils::format("V%d", info.vipLimit[i]));
        }
    }
    
    auto countText = (Text*)monthAssignBg->getChildByName("count_text");
    int signedCount = info.monthAssignId;
    if (info.monthAssignState == 1) {
        signedCount = info.monthAssignId-1;
    }
    countText->setString(GameUtils::format("%d/%d", signedCount, info.configCount));
    
    signButton->setEnabled(info.monthAssignState == 1);
    signButton->setBright(info.monthAssignState == 1);
    
}

void CampaignLayer::updateFirstRecharge(cocos2d::EventCustom *event)
{
    L2E_SHOW_FIRST_RECHARGE info = *static_cast<L2E_SHOW_FIRST_RECHARGE*>(event->getUserData());
    everydayBg->setVisible(false);
    missionBg->setVisible(false);
    monthAssignBg->setVisible(false);
    firstRechargeBg->setVisible(true);
    boundMissionBg->setVisible(false);
    everydayButton->setEnabled(true);
    everydayButton->setBright(true);
    monthAssignButton->setEnabled(true);
    monthAssignButton->setBright(true);
    missionButton->setEnabled(true);
    missionButton->setBright(true);
    rechargeButton->setEnabled(false);
    rechargeButton->setBright(false);
    boundButton->setEnabled(true);
    boundButton->setBright(true);

    switch (info.firstRechargeState) {
        case 0:
        {
            takeFirstRecharge->setBright(true);
            takeFirstRecharge->setEnabled(true);
            takeFirstRecharge->setTitleText(StringData::shared()->stringFromKey("jump_recharge"));
        }
            break;
        case 1:
        {
            takeFirstRecharge->setBright(true);
            takeFirstRecharge->setEnabled(true);
            takeFirstRecharge->setTitleText(StringData::shared()->stringFromKey("take_sweep_reward"));
        }
            break;
        case 2:
        {
            takeFirstRecharge->setBright(false);
            takeFirstRecharge->setEnabled(false);
            takeFirstRecharge->setTitleText(StringData::shared()->stringFromKey("take_complete"));
        }
            break;
    }
}

void CampaignLayer::showActive(cocos2d::EventCustom *event)
{
    L2E_SHOW_CAMPAIGN_ACTIVE info = *static_cast<L2E_SHOW_CAMPAIGN_ACTIVE*>(event->getUserData());
    everydayBg->setVisible(false);
    monthAssignBg->setVisible(false);
    missionBg->setVisible(true);
    firstRechargeBg->setVisible(false);
    boundMissionBg->setVisible(false);
    everydayButton->setEnabled(true);
    everydayButton->setBright(true);
    monthAssignButton->setEnabled(true);
    monthAssignButton->setBright(true);
    missionButton->setEnabled(false);
    missionButton->setBright(false);
    rechargeButton->setEnabled(true);
    rechargeButton->setBright(true);
    boundButton->setEnabled(true);
    boundButton->setBright(true);
    
    for (int i = 0; i < 4; i++) {
        if (info.boxState[i] == 1) {
            boxParticle[i]->setVisible(true);
        } else {
            boxParticle[i]->setVisible(false);
        }
        boxButton[i]->setBright(info.boxState[i] != 2);
    }
    totalActiveText->setString(Convert2String(info.totalActiveVal));
    int percent = MIN(100, info.totalActiveVal);
    activeBar->setPercent(percent);
    
    missionList->removeAllItems();
    for (int i = 0; i < info.missionCount; i++) {
        if (info.boundState[i] != 1)
        {
            continue;
        }
        auto root = CSLoader::createNode(ACTIVE_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionBound, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto activeValText = (Text*)rootBg->getChildByName("active_val");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        auto boundCountText = (Text*)rootBg->getChildByName("bound_count_text");
        descriptText->setString(info.descript[i]);
        activeValText->setString(Convert2String(info.activeVal[i]));
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        boundCountText->setString(Convert2String(info.boundCount[i]));
        
//        if (info.boundState[i] == 0) {
//            takeBoundButton->setVisible(false);
//            jumpButton->setVisible(true);
//            countText->setVisible(true);
//        }else if (info.boundState[i] == 1) {
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(true);
            takeBoundButton->setBright(true);
            countText->setVisible(false);
            jumpButton->setVisible(false);
//        }else if (info.boundState[i] == 2) {
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(false);
//            takeBoundButton->setBright(false);
//            countText->setVisible(false);
//            jumpButton->setVisible(false);
//        }
        missionList->addChild(rootBg);
        rootBg->release();
    }
    
    for (int i = 0; i < info.missionCount; i++) {
        if (info.boundState[i] != 0) {
            continue;
        }
        auto root = CSLoader::createNode(ACTIVE_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionBound, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto activeValText = (Text*)rootBg->getChildByName("active_val");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        auto boundCountText = (Text*)rootBg->getChildByName("bound_count_text");
        descriptText->setString(info.descript[i]);
        activeValText->setString(Convert2String(info.activeVal[i]));
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        boundCountText->setString(Convert2String(info.boundCount[i]));
        
//        if (info.boundState[i] == 0) {
            takeBoundButton->setVisible(false);
            jumpButton->setVisible(true);
            countText->setVisible(true);
//        }else if (info.boundState[i] == 1) {
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(true);
//            takeBoundButton->setBright(true);
//            countText->setVisible(false);
//            jumpButton->setVisible(false);
//        }else if (info.boundState[i] == 2) {
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(false);
//            takeBoundButton->setBright(false);
//            countText->setVisible(false);
//            jumpButton->setVisible(false);
//        }
        missionList->addChild(rootBg);
        rootBg->release();
    }
    for (int i = 0; i < info.missionCount; i++) {
        if (info.boundState[i] != 2) {
            continue;
        }
        auto root = CSLoader::createNode(ACTIVE_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionBound, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto activeValText = (Text*)rootBg->getChildByName("active_val");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        auto boundCountText = (Text*)rootBg->getChildByName("bound_count_text");
        descriptText->setString(info.descript[i]);
        activeValText->setString(Convert2String(info.activeVal[i]));
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        boundCountText->setString(Convert2String(info.boundCount[i]));
        
//        if (info.boundState[i] == 0) {
//            takeBoundButton->setVisible(false);
//            jumpButton->setVisible(true);
//            countText->setVisible(true);
//        }else if (info.boundState[i] == 1) {
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(true);
//            takeBoundButton->setBright(true);
//            countText->setVisible(false);
//            jumpButton->setVisible(false);
//        }else if (info.boundState[i] == 2) {
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(false);
            takeBoundButton->setBright(false);
            countText->setVisible(false);
            jumpButton->setVisible(false);
//        }
        missionList->addChild(rootBg);
        rootBg->release();
    }
}

void CampaignLayer::updateActive(cocos2d::EventCustom *event)
{
    L2E_SHOW_CAMPAIGN_ACTIVE info = *static_cast<L2E_SHOW_CAMPAIGN_ACTIVE*>(event->getUserData());
    if (!missionBg->isVisible()) {
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        if (info.boxState[i] == 1) {
            boxParticle[i]->setVisible(true);
        } else {
            boxParticle[i]->setVisible(false);
        }
        boxButton[i]->setBright(info.boxState[i] != 2);
    }
    totalActiveText->setString(Convert2String(info.totalActiveVal));
    int percent = MIN(100, info.totalActiveVal);
    activeBar->setPercent(percent);
    
    missionList->removeAllItems();
    for (int i = 0; i < info.missionCount; i++) {
        auto root = CSLoader::createNode(ACTIVE_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickMissionBound, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto activeValText = (Text*)rootBg->getChildByName("active_val");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        auto boundCountText = (Text*)rootBg->getChildByName("bound_count_text");
        descriptText->setString(info.descript[i]);
        activeValText->setString(Convert2String(info.activeVal[i]));
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        boundCountText->setString(Convert2String(info.boundCount[i]));
        
        if (info.boundState[i] == 0) {
            takeBoundButton->setVisible(false);
            jumpButton->setVisible(true);
            countText->setVisible(true);
        }else if (info.boundState[i] == 1) {
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(true);
            takeBoundButton->setBright(true);
            countText->setVisible(false);
            jumpButton->setVisible(false);
        }else if (info.boundState[i] == 2) {
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(false);
            takeBoundButton->setBright(false);
            countText->setVisible(false);
            jumpButton->setVisible(false);
        }
        missionList->addChild(rootBg);
        rootBg->release();
    }
}
void CampaignLayer::jumpLocked(cocos2d::EventCustom *event)
{
    std::string errStr = StringData::shared()->stringFromKey("func_locked");
    auto tip = TipsNode::create();
    tip->setupText(errStr);
    tip->setPosition(GameUtils::winSize/2);
    addChild(tip, 100);
}

void CampaignLayer::updateActiveBound(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CAMPAIGN_ACTIVE_MISSION info = *static_cast<L2E_UPDATE_CAMPAIGN_ACTIVE_MISSION*>(event->getUserData());

    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("active_take_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        if (info.boxState[i] == 1) {
            boxParticle[i]->setVisible(true);
        } else {
            boxParticle[i]->setVisible(false);
        }
        boxButton[i]->setBright(info.boxState[i] != 2);
    }
    totalActiveText->setString(Convert2String(info.totalActiveVal));
    int percent = MIN(100, info.totalActiveVal);
    activeBar->setPercent(percent);
    auto missionBg = (ImageView*)missionList->getChildByTag(info.missionId);
    auto jumpButton = (Button*)missionBg->getChildByName("jump_button");
    auto takeBoundButton = (Button*)missionBg->getChildByName("take_bound_button");
    auto countText = (Text*)missionBg->getChildByName("count_text");
    
    if (info.boundState == 0) {
        takeBoundButton->setVisible(false);
        jumpButton->setVisible(true);
        countText->setVisible(true);
    }else if (info.boundState == 1) {
        takeBoundButton->setVisible(true);
        takeBoundButton->setEnabled(true);
        takeBoundButton->setBright(true);
        countText->setVisible(false);
        jumpButton->setVisible(false);
    }else if (info.boundState == 2) {
        takeBoundButton->setVisible(true);
        takeBoundButton->setEnabled(false);
        takeBoundButton->setBright(false);
        countText->setVisible(false);
        jumpButton->setVisible(false);
    }

}

void CampaignLayer::updateActiveBox(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CAMPAIGN_ACTIVE_BOX info = *static_cast<L2E_UPDATE_CAMPAIGN_ACTIVE_BOX*>(event->getUserData());
    if (info.errNo != 0) {
        boxLayer = Layer::create();
        auto root = CSLoader::createNode(ACTIVE_BOX_REWARD_UI);
        auto boxAction = CSLoader::createTimeline(ACTIVE_BOX_REWARD_UI);
        root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        root->setPosition(GameUtils::winSize/2);
        root->runAction(boxAction);
        
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, boxLayer);
        
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        auto closeButton = (Button*)rootBg->getChildByName("close_button");
        closeButton->addClickEventListener(CC_CALLBACK_0(CampaignLayer::clickBoxClose, this));
        for (int i = 0; i<4; i++) {
            auto icon = (ImageView*)rootBg->getChildByName(GameUtils::format("icon%d", i+1));
            auto countText = (Text*)rootBg->getChildByName(GameUtils::format("count_text%d", i+1));
            if (i < info.boundCount) {
                icon->setVisible(true);
                countText->setVisible(true);
                icon->loadTexture(info.icon[i]);
                countText->setString(Convert2String(info.count[i]));
            }else{
                icon->setVisible(false);
                countText->setVisible(false);
            }
        }
        boxLayer->addChild(root);
        addChild(boxLayer);
        boxAction->play("in", false);
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (info.boxState[i] == 1) {
            boxParticle[i]->setVisible(true);
        } else {
            boxParticle[i]->setVisible(false);
        }
        boxButton[i]->setBright(info.boxState[i] != 2);
    }
}

void CampaignLayer::showBoundMission(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CAMPAIGN_BOUND_MISSION info = *static_cast<L2E_UPDATE_CAMPAIGN_BOUND_MISSION*>(event->getUserData());
    everydayBg->setVisible(false);
    missionBg->setVisible(false);
    firstRechargeBg->setVisible(false);
    boundMissionBg->setVisible(true);
    monthAssignBg->setVisible(false);
    everydayButton->setEnabled(true);
    everydayButton->setBright(true);
    monthAssignButton->setEnabled(true);
    monthAssignButton->setBright(true);
    missionButton->setEnabled(true);
    missionButton->setBright(true);
    rechargeButton->setEnabled(true);
    rechargeButton->setBright(true);
    boundButton->setEnabled(false);
    boundButton->setBright(false);
    
    boundMissionList->removeAllItems();
    for (int i = 0; i < info.missionCount; i++) {
        if (info.missionState[i] != 1) {
            continue;
        }
        auto root = CSLoader::createNode(BOUND_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickBoundMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickTakeBoundMissionReward, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        descriptText->setString(info.descript[i]);
        auto missionIcon = (ImageView*)rootBg->getChildByName("icon");
        missionIcon->loadTexture(GameUtils::format(CAMPAIGN_DIR, info.missionIcon[i].c_str()));
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        for (int j = 0; j < 4; j++) {
            auto icon = (ImageView*)rootBg->getChildByName(GameUtils::format("icon%d", j+1));
            auto countText = (Text*)icon->getChildByName("count_text");
            if (j < info.totalBound[i]) {
                icon->setVisible(true);
                icon->loadTexture(info.icon[i][j]);
                countText->setString(Convert2String(info.boundCount[i][j]));
            }else{
                icon->setVisible(false);
            }
        }
        auto goImg = (ImageView*)rootBg->getChildByName("go");

//        if (info.missionState[i] == 0) {
//            jumpButton->setVisible(true);
//            takeBoundButton->setVisible(false);
//            goImg->setVisible(true);
//        }else if (info.missionState[i] == 1){
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(true);
            takeBoundButton->setBright(true);
            jumpButton->setVisible(false);
            goImg->setVisible(false);
//        }else if (info.missionState[i] == 2){
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(false);
//            takeBoundButton->setBright(false);
//            jumpButton->setVisible(false);
//            goImg->setVisible(false);
//        }

        boundMissionList->addChild(rootBg);
        rootBg->release();
    }
    for (int i = 0; i < info.missionCount; i++) {
        if (info.missionState[i] != 0) {
            continue;
        }
        auto root = CSLoader::createNode(BOUND_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickBoundMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickTakeBoundMissionReward, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        descriptText->setString(info.descript[i]);
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        
        auto missionIcon = (ImageView*)rootBg->getChildByName("icon");
        missionIcon->loadTexture(GameUtils::format(CAMPAIGN_DIR, info.missionIcon[i].c_str()));
        for (int j = 0; j < 4; j++) {
            auto icon = (ImageView*)rootBg->getChildByName(GameUtils::format("icon%d", j+1));
            auto countText = (Text*)icon->getChildByName("count_text");
            if (j < info.totalBound[i]) {
                icon->setVisible(true);
                icon->loadTexture(info.icon[i][j]);
                countText->setString(Convert2String(info.boundCount[i][j]));
            }else{
                icon->setVisible(false);
            }
        }
        auto goImg = (ImageView*)rootBg->getChildByName("go");
        
//        if (info.missionState[i] == 0) {
            jumpButton->setVisible(true);
            takeBoundButton->setVisible(false);
            goImg->setVisible(true);
//        }else if (info.missionState[i] == 1){
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(true);
//            takeBoundButton->setBright(true);
//            jumpButton->setVisible(false);
//            goImg->setVisible(false);
//        }else if (info.missionState[i] == 2){
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(false);
//            takeBoundButton->setBright(false);
//            jumpButton->setVisible(false);
//            goImg->setVisible(false);
//        }
        
        boundMissionList->addChild(rootBg);
        rootBg->release();
    }
    for (int i = 0; i < info.missionCount; i++) {
        if (info.missionState[i] != 2) {
            continue;
        }
        auto root = CSLoader::createNode(BOUND_MISSION_UI);
        auto rootBg = (ImageView*)root->getChildByName("bg_img");
        rootBg->retain();
        rootBg->removeFromParent();
        rootBg->setTag(info.missionId[i]);
        
        auto jumpButton = (Button*)rootBg->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickBoundMissionJump, this));
        auto takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
        takeBoundButton->addClickEventListener(CC_CALLBACK_1(CampaignLayer::clickTakeBoundMissionReward, this));
        auto descriptText = (Text*)rootBg->getChildByName("descript_text");
        auto countText = (Text*)rootBg->getChildByName("count_text");
        descriptText->setString(info.descript[i]);
        countText->setString(GameUtils::format("%d/%d", info.count[i], info.totalCount[i]));
        
        auto missionIcon = (ImageView*)rootBg->getChildByName("icon");
        missionIcon->loadTexture(GameUtils::format(CAMPAIGN_DIR, info.missionIcon[i].c_str()));

        for (int j = 0; j < 4; j++) {
            auto icon = (ImageView*)rootBg->getChildByName(GameUtils::format("icon%d", j+1));
            auto countText = (Text*)icon->getChildByName("count_text");
            if (j < info.totalBound[i]) {
                icon->setVisible(true);
                icon->loadTexture(info.icon[i][j]);
                countText->setString(Convert2String(info.boundCount[i][j]));
            }else{
                icon->setVisible(false);
            }
        }
        auto goImg = (ImageView*)rootBg->getChildByName("go");
        
//        if (info.missionState[i] == 0) {
//            jumpButton->setVisible(true);
//            takeBoundButton->setVisible(false);
//            goImg->setVisible(true);
//        }else if (info.missionState[i] == 1){
//            takeBoundButton->setVisible(true);
//            takeBoundButton->setEnabled(true);
//            takeBoundButton->setBright(true);
//            jumpButton->setVisible(false);
//            goImg->setVisible(false);
//        }else if (info.missionState[i] == 2){
            takeBoundButton->setVisible(true);
            takeBoundButton->setEnabled(false);
            takeBoundButton->setBright(false);
            jumpButton->setVisible(false);
            goImg->setVisible(false);
//        }
        
        boundMissionList->addChild(rootBg);
        rootBg->release();
    }
}
void CampaignLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 3) {
        if(info.param[2] != CAMPAIGN_LAYER)
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
                everydayButton->removeChildByTag(100);
                monthAssignButton->removeChildByTag(100);
                boundButton->removeChildByTag(100);
                rechargeButton->removeChildByTag(100);
                missionButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(everydayButton->getContentSize()/2);
                point->setTag(100);
                everydayButton->addChild(point);
            }
                break;
            case 2:
            {
                everydayButton->removeChildByTag(100);
                monthAssignButton->removeChildByTag(100);
                boundButton->removeChildByTag(100);
                rechargeButton->removeChildByTag(100);
                missionButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(monthAssignButton->getContentSize()/2);
                point->setTag(100);
                monthAssignButton->addChild(point);
            }
                break;
            case 3:
            {
                everydayButton->removeChildByTag(100);
                monthAssignButton->removeChildByTag(100);
                boundButton->removeChildByTag(100);
                rechargeButton->removeChildByTag(100);
                missionButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(boundButton->getContentSize()/2);
                point->setTag(100);
                boundButton->addChild(point);
            }
                break;
            case 4:
            {
                everydayButton->removeChildByTag(100);
                monthAssignButton->removeChildByTag(100);
                boundButton->removeChildByTag(100);
                rechargeButton->removeChildByTag(100);
                missionButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(rechargeButton->getContentSize()/2);
                point->setTag(100);
                rechargeButton->addChild(point);
            }
                break;
            case 5:
            {
                everydayButton->removeChildByTag(100);
                monthAssignButton->removeChildByTag(100);
                boundButton->removeChildByTag(100);
                rechargeButton->removeChildByTag(100);
                missionButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(missionButton->getContentSize()/2);
                point->setTag(100);
                missionButton->addChild(point);
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
}

void CampaignLayer::clickClose()
{
    if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(CampaignLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void CampaignLayer::clickEveryday()
{
    if (plotButtonId == 1) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = everydayButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
    }else if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_everyday;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickMonthAssign()
{
    if (plotButtonId == 2) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = monthAssignButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
    }else if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_month_assign;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickMission()
{
    if (plotButtonId == 5) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = missionButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
    }else if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_campaign_active;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickRecharge()
{
    if (plotButtonId == 4) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = rechargeButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
    }else if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_first_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickBound()
{
    if (plotButtonId == 3) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = boundButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
    }else if (plotButtonId != 0) {
        return;
    }
    
    plotButtonId = 0;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_bound_mission;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickTakeEveryday()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_take_everyday_bound;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickTakeRecharge()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_take_first_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = CAMPAIGN_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::removeOff()
{
    removeFromParent();
}

void CampaignLayer::hide(cocos2d::EventCustom *event)
{
    removeFromParent();
}

void CampaignLayer::clickMissionJump(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_active_jump;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickMissionBound(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_active_bound;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickActiveBox(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_active_box;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickBoxClose()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    removeChild(boxLayer);
    boxLayer = nullptr;
//    boxAction->play("out", false);
//    boxAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(CampaignLayer::removeBoxOff, this));
}

void CampaignLayer::removeBoxOff()
{
    removeChild(boxLayer);
    boxLayer = nullptr;
}

void CampaignLayer::clickBoundMissionJump(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_mission_jump;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::clickTakeBoundMissionReward(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_take_mission_reward;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CampaignLayer::updateTip(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CAMPAIGN_TIP info = *static_cast<L2E_UPDATE_CAMPAIGN_TIP*>(event->getUserData());
    boundTip->setVisible(info.boundTip);
    missionTip->setVisible(info.activeTip);
    monthAssignTip->setVisible(info.boundTip);
}

void CampaignLayer::clickMonthSign()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_take_month_assign_reward;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
