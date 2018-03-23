//
//  HallTowerLayer.cpp
//  life
//
//  Created by ff on 17/9/12.
//
//

#include "HallTowerLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;


HallTowerLayer::HallTowerLayer()
{
    sweepLayer = nullptr;
    sweepState = 0;
    sweepSec = 0;
}

HallTowerLayer::~HallTowerLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showSweepListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resetTowerListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startSweepListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(completeSweepListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeRewardListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(speedupSweepListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateSweepCDListener);
    unscheduleUpdate();
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

void HallTowerLayer::onEnter()
{
    Layer::onEnter();
    showSweepListener = EventListenerCustom::create(SHOW_TOWER_SWEEP, CC_CALLBACK_1(HallTowerLayer::showSweep, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showSweepListener, -1);
    resetTowerListener = EventListenerCustom::create(RESET_TOWER, CC_CALLBACK_1(HallTowerLayer::resetTower, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resetTowerListener, -1);
    startSweepListener = EventListenerCustom::create(START_TOWER_SWEEP, CC_CALLBACK_1(HallTowerLayer::startSweep, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startSweepListener, -1);
    completeSweepListener = EventListenerCustom::create(COMPLETE_TOWER_SWEEP, CC_CALLBACK_1(HallTowerLayer::compelteSweep, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(completeSweepListener, -1);
    takeRewardListener = EventListenerCustom::create(TOWER_TAKE_REWARD, CC_CALLBACK_1(HallTowerLayer::takeReward, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeRewardListener, -1);
    speedupSweepListener = EventListenerCustom::create(SPEEDUP_TOWER_SWEEP, CC_CALLBACK_1(HallTowerLayer::speedupSweep, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(speedupSweepListener, -1);
    updateSweepCDListener = EventListenerCustom::create(UPDATE_TOWER_SWEEP_SEC, CC_CALLBACK_1(HallTowerLayer::updateSweepCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateSweepCDListener, -1);
    scheduleUpdate();
}


bool HallTowerLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_TOWER_UI);
    rootAction = CSLoader::createTimeline(HALL_TOWER_UI);
    
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickClose, this));

    auto leftBg = (ImageView*)rootBg->getChildByName("left_bg");
    towerScroll = (ScrollView*)leftBg->getChildByName("tower_scroll");
    auto startButton = (Button*)leftBg->getChildByName("start_button");
    startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickStart, this));
    auto resetButton = (Button*)leftBg->getChildByName("reset_button");
    resetButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickReset, this));
    auto sweepButton = (Button*)leftBg->getChildByName("sweep_button");
    sweepButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickSweep, this));
    
    rightBg = (ImageView*)rootBg->getChildByName("right_bg");
    titleText = (Text*)rightBg->getChildByName("title_bg")->getChildByName("title_text");
    nameText = (Text*)rightBg->getChildByName("monster_name_bg")->getChildByName("name_text");
    descriptText = (Text*)rightBg->getChildByName("descript_bg")->getChildByName("descript_text");
    goldText = (Text*)rightBg->getChildByName("bounds_bg")->getChildByName("gold_bg")->getChildByName("gold_count");
    expText = (Text*)rightBg->getChildByName("bounds_bg")->getChildByName("exp_bg")->getChildByName("exp_count");
    gemIcon = (ImageView*)rightBg->getChildByName("bounds_bg")->getChildByName("stone_bg");
    gemText = (Text*)gemIcon->getChildByName("stone_count");
    
    return true;
}


void HallTowerLayer::update(float dt)
{
    if (sweepState == 1) {
        sweepSec -= dt;
        sweepSec = MAX(0, sweepSec);
        sweepCDText->setString(TimeUtil::timeFormatToHMS(sweepSec));
        if (sweepSec <= 0) {
            sweepState = 2;
        }
    }
}

void HallTowerLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_TOWER info = *static_cast<L2E_SHOW_HALL_TOWER*>(event->getUserData());
    titleText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_flord").c_str(), info.currStage));
    nameText->setString(info.name);
    descriptText->setString(info.descript);
    goldText->setString(Convert2String(info.rewardGold));
    expText->setString(Convert2String(info.rewardExp));
    gemText->setString(Convert2String(info.rewardGemCount));
    gemIcon->loadTexture(GameUtils::format(COMMON_DIR, info.rewardGemIcon.c_str()));
    auto platform = (Node*)rightBg->getChildByName("platform_node");
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            info.animationFile.c_str(),
//                                            info.animationFile.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            info.animationFile.c_str());
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    bossAramture = Armature::create(info.animationFile);
    bossAramture->setScale(0.7);
    bossAramture->getAnimation()->play("wait");
    platform->addChild(bossAramture,10);
    
    stageBgMap.clear();
    auto base = CSLoader::createNode(TOWER_BASE_UI);
    auto baseAction = CSLoader::createTimeline(TOWER_BASE_UI);
    base->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    base->setPosition(Vec2(towerScroll->getContentSize().width/2, 0));
    base->runAction(baseAction);
    towerScroll->addChild(base);
    baseAction->play("play", true);
    stageBgMap[1] = (ImageView*)base->getChildByName("bg_img")->getChildByTag(1);
    stageBgMap[2] = (ImageView*)base->getChildByName("bg_img")->getChildByTag(2);
    
    auto body = CSLoader::createNode(TOWER_BODY_UI);
    auto bodyAction = CSLoader::createTimeline(TOWER_BODY_UI);
    body->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    body->setPosition(Vec2(towerScroll->getContentSize().width/2, 283));
    body->runAction(bodyAction);
    towerScroll->addChild(body);
    bodyAction->play("play", true);
    stageBgMap[3] = (ImageView*)body->getChildByName("bg_img")->getChildByTag(1);
    stageBgMap[4] = (ImageView*)body->getChildByName("bg_img")->getChildByTag(2);
    
    int contentWidth = towerScroll->getInnerContainer()->getContentSize().width;
    int bodyCount = (info.openCount-1)/2;
    for (int i = 2; i <= bodyCount; i++) {
        auto body2 = CSLoader::createNode(TOWER_BODY_UI);
        auto body2Action = CSLoader::createTimeline(TOWER_BODY_UI);
        body2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        body2->setPosition(Vec2(towerScroll->getContentSize().width/2, 283+203*(i-1)));
        body2->runAction(body2Action);
        towerScroll->addChild(body2);
        body2Action->play("play", true);
        stageBgMap[i*2+1] = (ImageView*)body2->getChildByName("bg_img")->getChildByTag(1);
        stageBgMap[i*2+2] = (ImageView*)body2->getChildByName("bg_img")->getChildByTag(2);
    }
    towerScroll->getInnerContainer()->setContentSize(Size(contentWidth,283+203+203*MAX(bodyCount-1,0)));
    if(info.currStage ==1){
        towerScroll->jumpToBottom();
    }else{
        towerScroll->jumpToTop();
    }
    
    
    for (auto stageBg : stageBgMap) {
        auto focus = (ImageView*)stageBg.second->getChildByName("focus");
        focus->setVisible(false);
        auto guang = (ImageView*)stageBg.second->getChildByName("guang");
        guang->setVisible(false);
        auto stageTitleBg = (ImageView*)stageBg.second->getChildByName("Image_4");
        stageTitleBg->setVisible(false);
        auto stageText = (Text*)stageBg.second->getChildByName("title_text");
        stageText->setVisible(false);
        if (stageBg.first % 5 == 0 || stageBg.first == 1) {
            stageTitleBg->setVisible(true);
            stageText->setVisible(true);
            stageText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_node_flord").c_str(), stageBg.first));
        }
    }
    stageBgMap[info.currStage]->getChildByName("focus")->setVisible(true);
    stageBgMap[info.currStage]->getChildByName("guang")->setVisible(true);
    if (info.stageName != "") {
        stageBgMap[info.currStage]->getChildByName("Image_4")->setVisible(true);
        auto stageText = (Text*)stageBgMap[info.currStage]->getChildByName("title_text");
        stageText->setVisible(true);
        stageText->setString(info.stageName);
    }
    
    sweepState = info.sweepState;
    sweepSec = info.sweepSec;
    if (sweepState != 0) {
        sweepLayer = LayerColor::create(Color4B(10, 10, 10, 127));
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sweepLayer);
        
        addChild(sweepLayer);
        
        auto sweepRoot = CSLoader::createNode(TOWER_SWEEP_UI);
        sweepAction = CSLoader::createTimeline(TOWER_SWEEP_UI);
        sweepRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sweepRoot->setPosition(GameUtils::winSize/2);
        sweepLayer->addChild(sweepRoot);
        sweepRoot->runAction(sweepAction);
        
        auto closeButton = (Button*)sweepRoot->getChildByName("bg_img")->getChildByName("close_button");
        closeButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickCloseSweep, this));
        startButton = (Button*)sweepRoot->getChildByName("bg_img")->getChildByName("start_button");
        costImg = (ImageView*)startButton->getChildByName("price_img");
        costText = (Text*)startButton->getChildByName("price_text");
        sweepCDImg = (ImageView*)sweepRoot->getChildByName("bg_img")->getChildByName("sweep_cd_text");
        sweepCDText = (Text*)sweepCDImg->getChildByName("cd_text");
        switch (info.sweepState) {
            case 0:
            {
                startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickStartSweep, this));
                startButton->setTitleText(StringData::shared()->stringFromKey("start_sweep"));
                //            closeButton->setVisible(true);
                costImg->setVisible(false);
                costText->setVisible(false);
                sweepCDImg->setVisible(false);
            }
                break;
            case 1:
            {
                startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickSpeedUpSweep, this));
                startButton->setTitleText(StringData::shared()->stringFromKey("speedup_sweep"));
                //            closeButton->setVisible(false);
                costImg->setVisible(true);
                costText->setVisible(true);
                sweepCDImg->setVisible(true);
                
                sweepSec = info.sweepSec;
                costText->setString(Convert2String(info.speedupPrice));
                sweepCDText->setString(TimeUtil::timeFormatToHMS(info.sweepSec));
            }
                break;
            case 2:
            {
                startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickTakeSweepReward, this));
                startButton->setTitleText(StringData::shared()->stringFromKey("take_sweep_reward"));
                //            closeButton->setVisible(false);
                costImg->setVisible(false);
                costText->setVisible(false);
                sweepCDImg->setVisible(false);
            }
                break;
                
            default:
                break;
        }
        sweepState = info.sweepState;
        
        auto rewardList = (ListView*)sweepRoot->getChildByName("bg_img")->getChildByName("reward_list");
        rewardList->removeAllChildren();
        for (auto reward : info.sweepRewards) {
            auto root = CSLoader::createNode(TOWER_SWEEP_NODE_UI);
            auto icon = (ImageView*)root->getChildByName("Image_1");
            auto countText = (Text*)icon->getChildByName("Text_1_0");
            icon->retain();
            icon->removeFromParent();
//            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, reward.first.c_str()));
            countText->setString(Convert2String(reward.second));
//            auto icon = ImageView::create(GameUtils::format(COMMON_DIR, reward.first.c_str()));
//            auto countText = Text::create(Convert2String(reward.second), "", 36);
//            countText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            countText->setPosition(Vec2(icon->getContentSize().width/2, 0));
//            icon->addChild(countText);
            
            rewardList->addChild(icon);
            icon->release();
        }
        sweepAction->play("in", false);
        sweepAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallTowerLayer::endSweepIn, this));

    }

    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallTowerLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void HallTowerLayer::resetTower(cocos2d::EventCustom *event)
{
    L2E_RESET_TOWER info = *static_cast<L2E_RESET_TOWER*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("reset_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    titleText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_flord").c_str(), info.currStage));
    nameText->setString(info.name);
    descriptText->setString(info.descript);
    goldText->setString(Convert2String(info.rewardGold));
    expText->setString(Convert2String(info.rewardExp));
    gemText->setString(Convert2String(info.rewardGemCount));
    gemIcon->loadTexture(GameUtils::format(COMMON_DIR, info.rewardGemIcon.c_str()));
    auto platform = (Node*)rightBg->getChildByName("platform_node");
    platform->removeAllChildren();
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            info.animationFile.c_str(),
//                                            info.animationFile.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            info.animationFile.c_str());
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    bossAramture = Armature::create(info.animationFile);
    bossAramture->setScale(0.7);
    bossAramture->getAnimation()->play("wait");
    platform->addChild(bossAramture,10);

    for (auto stageBg : stageBgMap) {
        auto focus = (ImageView*)stageBg.second->getChildByName("focus");
        focus->setVisible(false);
        auto guang = (ImageView*)stageBg.second->getChildByName("guang");
        guang->setVisible(false);
        auto stageTitleBg = (ImageView*)stageBg.second->getChildByName("Image_4");
        stageTitleBg->setVisible(false);
        auto stageText = (Text*)stageBg.second->getChildByName("title_text");
        stageText->setVisible(false);
        if (stageBg.first % 5 == 0 || stageBg.first == 1) {
            stageTitleBg->setVisible(true);
            stageText->setVisible(true);
            stageText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_node_flord").c_str(), stageBg.first));
        }
    }
    towerScroll->jumpToBottom();
    stageBgMap[info.currStage]->getChildByName("focus")->setVisible(true);
    stageBgMap[info.currStage]->getChildByName("guang")->setVisible(true);
    if (info.stageName != "") {
        stageBgMap[info.currStage]->getChildByName("Image_4")->setVisible(true);
        auto stageText = (Text*)stageBgMap[info.currStage]->getChildByName("title_text");
        stageText->setVisible(true);
        stageText->setString(info.stageName);
    }

}

void HallTowerLayer::takeReward(cocos2d::EventCustom *event)
{
    L2E_TOWER_TAKE_REWARD info = *static_cast<L2E_TOWER_TAKE_REWARD*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("complete_sweep_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    sweepState = info.sweepState;
    sweepAction->play("out", false);
    sweepAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallTowerLayer::removeSweep, this));
    
    titleText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_flord").c_str(), info.currStage));
    nameText->setString(info.name);
    descriptText->setString(info.descript);
    goldText->setString(Convert2String(info.rewardGold));
    expText->setString(Convert2String(info.rewardExp));
    gemText->setString(Convert2String(info.rewardGemCount));
    gemIcon->loadTexture(GameUtils::format(COMMON_DIR, info.rewardGemIcon.c_str()));
    auto platform = (Node*)rightBg->getChildByName("platform_node");
    platform->removeAllChildren();
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            info.animationFile.c_str(),
//                                            info.animationFile.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            info.animationFile.c_str());

    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    bossAramture = Armature::create(info.animationFile);
    bossAramture->setScale(0.7);
    bossAramture->getAnimation()->play("wait");
    platform->addChild(bossAramture,10);
    
    for (auto stageBg : stageBgMap) {
        auto focus = (ImageView*)stageBg.second->getChildByName("focus");
        focus->setVisible(false);
        auto guang = (ImageView*)stageBg.second->getChildByName("guang");
        guang->setVisible(false);
        auto stageTitleBg = (ImageView*)stageBg.second->getChildByName("Image_4");
        stageTitleBg->setVisible(false);
        auto stageText = (Text*)stageBg.second->getChildByName("title_text");
        stageText->setVisible(false);
        if (stageBg.first % 5 == 0 || stageBg.first == 1) {
            stageTitleBg->setVisible(true);
            stageText->setVisible(true);
            stageText->setString(GameUtils::format(StringData::shared()->stringFromKey("tower_node_flord").c_str(), stageBg.first));
        }
    }
    towerScroll->jumpToTop();
    stageBgMap[info.currStage]->getChildByName("focus")->setVisible(true);
    stageBgMap[info.currStage]->getChildByName("guang")->setVisible(true);
    if (info.stageName != "") {
        stageBgMap[info.currStage]->getChildByName("Image_4")->setVisible(true);
        auto stageText = (Text*)stageBgMap[info.currStage]->getChildByName("title_text");
        stageText->setVisible(true);
        stageText->setString(info.stageName);
    }
}

void HallTowerLayer::speedupSweep(cocos2d::EventCustom *event)
{
    L2E_SPEEDUP_TOWER_SWEEP info = *static_cast<L2E_SPEEDUP_TOWER_SWEEP*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("speedup_sweep_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    sweepState = info.sweepState;
    sweepSec = 0;
    
    startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickTakeSweepReward, this));
    startButton->setTitleText(StringData::shared()->stringFromKey("take_sweep_reward"));
//    closeButton->setVisible(false);
    costImg->setVisible(false);
    costText->setVisible(false);
    sweepCDImg->setVisible(false);

}
void HallTowerLayer::compelteSweep(cocos2d::EventCustom *event)
{
    L2E_COMPLETE_TOWER_SWEEP info = *static_cast<L2E_COMPLETE_TOWER_SWEEP*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("complete_sweep_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    sweepState = info.sweepState;
    sweepSec = 0;
    
    startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickTakeSweepReward, this));
    startButton->setTitleText(StringData::shared()->stringFromKey("take_sweep_reward"));
//    closeButton->setVisible(false);
    costImg->setVisible(false);
    costText->setVisible(false);
    sweepCDImg->setVisible(false);
}

void HallTowerLayer::startSweep(cocos2d::EventCustom *event)
{
    L2E_START_TOWER_SWEEP info = *static_cast<L2E_START_TOWER_SWEEP*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("start_sweep_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    sweepState = info.sweepState;
    sweepSec = info.sweepSec;

    startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickSpeedUpSweep, this));
    startButton->setTitleText(StringData::shared()->stringFromKey("speedup_sweep"));
//    closeButton->setVisible(false);
    costImg->setVisible(true);
    costText->setVisible(true);
    sweepCDImg->setVisible(true);
    
    costText->setString(Convert2String(info.speedupPrice));
    sweepCDText->setString(TimeUtil::timeFormatToHMS(info.sweepSec));
}

void HallTowerLayer::showSweep(cocos2d::EventCustom *event)
{
    L2E_SHOW_TOWER_SWEEP info = *static_cast<L2E_SHOW_TOWER_SWEEP*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("start_sweep_tower_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    sweepLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sweepLayer);

    addChild(sweepLayer);
    
    auto sweepRoot = CSLoader::createNode(TOWER_SWEEP_UI);
    sweepAction = CSLoader::createTimeline(TOWER_SWEEP_UI);
    sweepRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sweepRoot->setPosition(GameUtils::winSize/2);
    sweepLayer->addChild(sweepRoot);
    sweepRoot->runAction(sweepAction);
    
    auto closeButton = (Button*)sweepRoot->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickCloseSweep, this));
    startButton = (Button*)sweepRoot->getChildByName("bg_img")->getChildByName("start_button");
    costImg = (ImageView*)startButton->getChildByName("price_img");
    costText = (Text*)startButton->getChildByName("price_text");
    sweepCDImg = (ImageView*)sweepRoot->getChildByName("bg_img")->getChildByName("sweep_cd_text");
    sweepCDText = (Text*)sweepCDImg->getChildByName("cd_text");
    switch (info.sweepState) {
        case 0:
        {
            startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickStartSweep, this));
            startButton->setTitleText(StringData::shared()->stringFromKey("start_sweep"));
//            closeButton->setVisible(true);
            costImg->setVisible(false);
            costText->setVisible(false);
            sweepCDImg->setVisible(false);
        }
            break;
        case 1:
        {
            startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickSpeedUpSweep, this));
            startButton->setTitleText(StringData::shared()->stringFromKey("speedup_sweep"));
//            closeButton->setVisible(false);
            costImg->setVisible(true);
            costText->setVisible(true);
            sweepCDImg->setVisible(true);
            
            sweepSec = info.sweepSec;
            sweepSec = 1;
            costText->setString(Convert2String(info.speedupPrice));
            sweepCDText->setString(TimeUtil::timeFormatToHMS(info.sweepSec));
        }
            break;
        case 2:
        {
            startButton->addClickEventListener(CC_CALLBACK_0(HallTowerLayer::clickTakeSweepReward, this));
            startButton->setTitleText(StringData::shared()->stringFromKey("take_sweep_reward"));
//            closeButton->setVisible(false);
            costImg->setVisible(false);
            costText->setVisible(false);
            sweepCDImg->setVisible(false);
        }
            break;
            
        default:
            break;
    }
    sweepState = info.sweepState;
    
    auto rewardList = (ListView*)sweepRoot->getChildByName("bg_img")->getChildByName("reward_list");
    rewardList->removeAllChildren();
    for (auto reward : info.sweepRewards) {
        auto root = CSLoader::createNode(TOWER_SWEEP_NODE_UI);
        auto icon = (ImageView*)root->getChildByName("Image_1");
        auto countText = (Text*)icon->getChildByName("Text_1_0");
        icon->retain();
        icon->removeFromParent();
//        icon->setUnifySizeEnabled(true);
        icon->loadTexture(GameUtils::format(COMMON_DIR, reward.first.c_str()));
        countText->setString(Convert2String(reward.second));
        //            auto icon = ImageView::create(GameUtils::format(COMMON_DIR, reward.first.c_str()));
        //            auto countText = Text::create(Convert2String(reward.second), "", 36);
//        countText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        countText->setPosition(Vec2(icon->getContentSize().width/2, 0));
//        icon->addChild(countText);
        rewardList->addChild(icon);
        icon->release();
//        auto icon = ImageView::create(GameUtils::format(COMMON_DIR, reward.first.c_str()));
//        auto countText = Text::create(Convert2String(reward.second), "", 36);
//        countText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        countText->setPosition(Vec2(icon->getContentSize().width/2, 0));
//        icon->addChild(countText);
//        rewardList->addChild(icon);
    }
    
    sweepAction->play("in", false);
    sweepAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallTowerLayer::endSweepIn, this));
}

void HallTowerLayer::clickCloseSweep()
{
    if (sweepState == 0) {
        sweepAction->play("out", false);
        sweepAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallTowerLayer::removeSweep, this));
        SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
        return;
    }
    clickClose();
}
void HallTowerLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallTowerLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallTowerLayer::clickStart()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_tower_start;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::clickReset()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_tower_reset;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::clickSweep()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_tower_sweep;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::clickStartSweep()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_confirm_tower_sweep;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::clickSpeedUpSweep()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_tower_speedup;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::clickTakeSweepReward()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_tower_take_reward;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::removeOff()
{
    removeFromParent();
}

void HallTowerLayer::removeSweep()
{
    if (sweepLayer) {
        removeChild(sweepLayer);
        sweepLayer = nullptr;
    }
}

void HallTowerLayer::endIn()
{
    rootAction->play("play", true);
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = TOWER_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallTowerLayer::endSweepIn()
{
    if (sweepState == 1) {
        E2L_COMMON info;
        info.eProtocol = e2l_get_tower_sweep_sec;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}

void HallTowerLayer::updateSweepCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    sweepSec = info.value;
    sweepCDText->setString(TimeUtil::timeFormatToHMS(sweepSec));
}
