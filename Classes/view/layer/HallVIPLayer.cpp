//
//  HallVIPLayer.cpp
//  life
//
//  Created by ff on 17/11/14.
//
//

#include "HallVIPLayer.h"
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

HallVIPLayer::HallVIPLayer()
{
}

void HallVIPLayer::onEnter()
{
    Layer::onEnter();
    updateDetailListener = EventListenerCustom::create(SHOW_VIP_DETAIL, CC_CALLBACK_1(HallVIPLayer::updateDetail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateDetailListener, -1);
    takeBoundListener = EventListenerCustom::create(TAKE_VIP_BOUND, CC_CALLBACK_1(HallVIPLayer::takeBound, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeBoundListener, -1);
    hideListener = EventListenerCustom::create(HIDE_VIP, CC_CALLBACK_1(HallVIPLayer::hide, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideListener, -1);
    
}

HallVIPLayer::~HallVIPLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateDetailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallVIPLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_VIP_UI);
    rootAction = CSLoader::createTimeline(HALL_VIP_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallVIPLayer::clickClose, this));

    auto leftBg = (ImageView*)rootBg->getChildByName("left_bg");
    auto rightBg = (ImageView*)rootBg->getChildByName("right_bg");
    leftButton = (Button*)rootBg->getChildByName("left_button");
    rightButton = (Button*)rootBg->getChildByName("right_button");
    leftButton->addClickEventListener(CC_CALLBACK_0(HallVIPLayer::clickLeft, this));
    rightButton->addClickEventListener(CC_CALLBACK_0(HallVIPLayer::clickRight, this));
    
    for (int i = 0; i < 6; i++) {
        boundImg[i] = (ImageView*)rootBg->getChildByName(GameUtils::format("bound_img_%d", i+1));
        boundCount[i] = (Text*)boundImg[i]->getChildByName("count_text");
    }
    takeBoundButton = (Button*)rootBg->getChildByName("take_bound_button");
    takeBoundButton->addClickEventListener(CC_CALLBACK_0(HallVIPLayer::clickTakeBound, this));
    takeTip = (ImageView*)takeBoundButton->getChildByName("tip");
    
    currLevelImg = (ImageView*)leftBg->getChildByName("level_img");
    dayLoadBar = (LoadingBar*)leftBg->getChildByName("day_loadbar");
    dayCountText = (Text*)dayLoadBar->getChildByName("day_count_text");
    rechargeCountText = (Text*)leftBg->getChildByName("recharge_count");
    nextLevelText = (Text*)rechargeCountText->getChildByName("next_level_text");
    icon = (ImageView*)leftBg->getChildByName("icon");
    currLevelText = (Text*)icon->getChildByName("level_text");
    jumpButton = (Button*)icon->getChildByName("jump_button");
    jumpButton->addClickEventListener(CC_CALLBACK_0(HallVIPLayer::clickJump, this));
    
    
    detailLevelImg = (ImageView*)rightBg->getChildByName("level_img");
    for (int i = 0; i < 10; i++) {
        descriptIcon[i] = (ImageView*)rightBg->getChildByName(GameUtils::format("descript_icon_%d", i+1));
        descriptText[i] = (Text*)descriptIcon[i]->getChildByName("descript_text");
    }
    return true;
}

void HallVIPLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_VIP info = *static_cast<L2E_SHOW_VIP*>(event->getUserData());
    detailIndex = info.currVipLevel-1;
    currLevelText->setString(Convert2String(detailIndex));
    icon->loadTexture(GameUtils::format("ui/noble/noble%d.png", detailIndex));
    currLevelImg->loadTexture(GameUtils::format("ui/noble/v%d.png", detailIndex));
    
    if (detailIndex < 1) {
        dayLoadBar->setVisible(false);
        leftButton->setVisible(false);
        rechargeCountText->setString(Convert2String(info.nextRechargeCount));
        nextLevelText->setString(Convert2String(info.currVipLevel));
    }else if (detailIndex > 2){
        dayLoadBar->setVisible(false);
        jumpButton->setVisible(false);
        rechargeCountText->setVisible(false);
        rightButton->setVisible(false);
    }else{
        dayLoadBar->setPercent(info.currLoginDay*100.0/info.totalLoginDay);
        dayCountText->setString(GameUtils::format("%d/%d", info.currLoginDay, info.totalLoginDay));
        rechargeCountText->setString(Convert2String(info.nextRechargeCount));
        nextLevelText->setString(Convert2String(info.currVipLevel));
    }
    
    for (int i = 0; i < 10; i++) {
        if (info.detailDescript[i] == "null")
        {
            descriptIcon[i]->setVisible(false);
        }else{
            descriptText[i]->setString(info.detailDescript[i]);
        }
    }
    detailLevelImg->loadTexture(GameUtils::format("ui/noble/v%dtequan.png", detailIndex));
    
    for (int i = 0; i < 6; i++) {
        if (info.boundCount[i] <= 0) {
            boundImg[i]->setVisible(false);
        }else{
            boundImg[i]->loadTexture(info.boundIcon[i]);
            boundCount[i]->setString(Convert2String(info.boundCount[i]));
        }
    }
    if (info.boundState == 1) {
        takeBoundButton->setTitleText(StringData::shared()->stringFromKey("vip_take"));
    }else{
        takeBoundButton->setTitleText(StringData::shared()->stringFromKey("take_complete"));
    }
    takeTip->setVisible(info.boundState == 1);
    takeBoundButton->setBright(info.boundState == 1);
    takeBoundButton->setEnabled(info.boundState == 1);
    
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallVIPLayer::endIn, this));
}

void HallVIPLayer::updateDetail(cocos2d::EventCustom *event)
{
    L2E_SHOW_VIP_DETAIL info = *static_cast<L2E_SHOW_VIP_DETAIL*>(event->getUserData());
    detailIndex = info.detailVipLevel-1;
    
    if (detailIndex < 1) {
        rightButton->setVisible(true);
        leftButton->setVisible(false);
    }else if (detailIndex > 2){
        rightButton->setVisible(false);
        leftButton->setVisible(true);
    }else{
        rightButton->setVisible(true);
        leftButton->setVisible(true);
    }
    
    for (int i = 0; i < 10; i++) {
        if (info.detailDescript[i] == "null")
        {
            descriptIcon[i]->setVisible(false);
        }else{
            descriptIcon[i]->setVisible(true);
            descriptText[i]->setString(info.detailDescript[i]);
        }
    }
    detailLevelImg->loadTexture(GameUtils::format("ui/noble/v%dtequan.png", detailIndex));

}

void HallVIPLayer::takeBound(cocos2d::EventCustom *event)
{
    L2E_TAKE_VIP_BOUND info = *static_cast<L2E_TAKE_VIP_BOUND*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("vip_take_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    takeTip->setVisible(false);
    takeBoundButton->setTitleText(StringData::shared()->stringFromKey("take_complete"));
    takeBoundButton->setBright(false);
    takeBoundButton->setEnabled(false);
}

void HallVIPLayer::hide(cocos2d::EventCustom *event)
{
    removeFromParent();
}

void HallVIPLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallVIPLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallVIPLayer::removeOff()
{
    removeFromParent();
}

void HallVIPLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = VIP_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallVIPLayer::clickJump()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_vip_jump_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallVIPLayer::clickLeft()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    detailIndex--;
    detailIndex = MAX(0, detailIndex);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_show_vip_detail;
    info.value = detailIndex+1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallVIPLayer::clickRight()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    detailIndex++;
    detailIndex = MIN(3, detailIndex);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_show_vip_detail;
    info.value = detailIndex+1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallVIPLayer::clickTakeBound()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_take_vip_bound;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
