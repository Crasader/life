//
//  HallLuckyLayer.cpp
//  life
//
//  Created by ff on 17/11/6.
//
//

#include "HallLuckyLayer.h"
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

HallLuckyLayer::HallLuckyLayer():
freeNormalCD(0),
freeSpecialCD(0)
{
}

void HallLuckyLayer::onEnter()
{
    Layer::onEnter();
    normalDrawListener = EventListenerCustom::create(SHOW_DRAW_NORMAL_ONE, CC_CALLBACK_1(HallLuckyLayer::updateNormalDraw, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(normalDrawListener, -1);
    specialDrawListener = EventListenerCustom::create(SHOW_DRAW_SPECIAL_ONE, CC_CALLBACK_1(HallLuckyLayer::updateSpecialDrawOne, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(specialDrawListener, -1);
    tenDrawListener = EventListenerCustom::create(SHOW_DRAW_SPECIAL_TEN, CC_CALLBACK_1(HallLuckyLayer::updateSpecialDrawTen, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tenDrawListener, -1);
    freeNormalListener = EventListenerCustom::create(UPDATE_FREE_NORMAL, CC_CALLBACK_1(HallLuckyLayer::updateFreeNormal, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(freeNormalListener, -1);
    freeSpecialListener = EventListenerCustom::create(UPDATE_FREE_SPECIAL, CC_CALLBACK_1(HallLuckyLayer::updateFreeSpecial, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(freeSpecialListener, -1);
    updateLuckyCDListener = EventListenerCustom::create(UPDATE_LUCKY_CD, CC_CALLBACK_1(HallLuckyLayer::updateLuckyCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateLuckyCDListener, -1);
    updateSpecialTicketListener = EventListenerCustom::create(UPDATE_SPECIAL_TICKET, CC_CALLBACK_1(HallLuckyLayer::updateSpecialTicket, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateSpecialTicketListener, -1);
}

HallLuckyLayer::~HallLuckyLayer()
{
    unscheduleUpdate();
    Director::getInstance()->getEventDispatcher()->removeEventListener(normalDrawListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(specialDrawListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(tenDrawListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(freeNormalListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(freeSpecialListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateLuckyCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateSpecialTicketListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallLuckyLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_LUCKY_UI);
    rootAction = CSLoader::createTimeline(HALL_LUCKY_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickClose, this));

    normalButton = (Button*)rootBg->getChildByName("normal_draw_button");
    normalButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickNormalButton, this));
    normalTip = (ImageView*)normalButton->getChildByName("tip");
    specialButton = (Button*)rootBg->getChildByName("special_draw_button");
    specialButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickSpecialButton, this));
    specialTip = (ImageView*)specialButton->getChildByName("tip");
    specialTenButton = (Button*)rootBg->getChildByName("special_ten_draw_button");
    specialTenButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickSpecialTenButton, this));
    
    auto normal = (Button*)rootBg->getChildByName("normal_button");
    normal->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickNormal, this));
    auto special = (Button*)rootBg->getChildByName("special_button");
    special->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickSpecial, this));
    
    auto addButton = (Button*)rootBg->getChildByName("special_ticket_bg")->getChildByName("add_ticket_button");
    addButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickAddSpecialTicket, this));
    specialCountText = (Text*)rootBg->getChildByName("special_ticket_bg")->getChildByName("ticket_count");
    specialTenCountText = (Text*)rootBg->getChildByName("special_ten_bg")->getChildByName("count_text");
    normalCountText = (Text*)rootBg->getChildByName("normal_ticket_bg")->getChildByName("count_text");
    
    normalCDText = (Text*)rootBg->getChildByName("normal_CD");
    specialCDText = (Text*)rootBg->getChildByName("special_CD");
    return true;
}

void HallLuckyLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_LUCKY info = *static_cast<L2E_SHOW_LUCKY*>(event->getUserData());
//    normalCDText->setString(TimeUtil::timeFormatToHMS(info.normalCD));
//    specialCDText->setString(TimeUtil::timeFormatToHMS(info.specialCD));
    normalCountText->setString(GameUtils::format("%d/1", info.normalTicket));
    specialCountText->setString(GameUtils::format("%d/1", info.specialTicket));
    specialTenCountText->setString(GameUtils::format("%d/10", info.specialTicket));
    if (info.normalState == 1) {
        normalButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        normalCDText->setVisible(false);
        normalTip->setVisible(true);
    }else{
        normalButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        normalCDText->setVisible(true);
        normalTip->setVisible(false);
    }
    
    if (info.specialState == 1) {
        specialButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        specialCDText->setVisible(false);
        specialTip->setVisible(true);
    }else{
        specialButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        specialCDText->setVisible(true);
        specialTip->setVisible(false);
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallLuckyLayer::endIn, this));
}

void HallLuckyLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallLuckyLayer::removeOff()
{
    removeFromParent();
}

void HallLuckyLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_COMMON info;
    info.eProtocol = e2l_update_lucky_cd;
    ClientLogic::instance()->ProcessUIRequest(&info);
    rootAction->play("play", true);
}

void HallLuckyLayer::endRewardIn1()
{
    getEventDispatcher()->removeEventListenersForTarget(rewardLayer1);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HallLuckyLayer::touchReward1, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rewardLayer1);
}

void HallLuckyLayer::endRewardIn2()
{
    getEventDispatcher()->removeEventListenersForTarget(rewardLayer2);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HallLuckyLayer::touchReward2, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rewardLayer2);
}

void HallLuckyLayer::updateLuckyCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_LUCKY_CD info = *static_cast<L2E_UPDATE_LUCKY_CD*>(event->getUserData());
    freeNormalCD = (int)info.normalCD;
    if (freeNormalCD > 0) {
        normalCDText->setString(TimeUtil::timeFormatToHMS(freeNormalCD));
        normalCDText->setVisible(true);
    }else{
        normalCDText->setVisible(false);
    }
    freeSpecialCD = (int)info.specialCD;
    if (freeSpecialCD > 0) {
        specialCDText->setString(TimeUtil::timeFormatToHMS(freeSpecialCD));
        specialCDText->setVisible(true);
    }else{
        specialCDText->setVisible(false);
    }
    scheduleUpdate();
}

void HallLuckyLayer::update(float dt)
{
    if (freeNormalCD > 0) {
        freeNormalCD -= dt;
        if (freeNormalCD <= 0) {
            normalCDText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            normalCDText->setString(TimeUtil::timeFormatToHMS(freeNormalCD));
        }
        normalCDText->setVisible(true);
    }
    
    if (freeSpecialCD > 0) {
        freeSpecialCD -= dt;
        if (freeSpecialCD <= 0) {
            specialCDText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            specialCDText->setString(TimeUtil::timeFormatToHMS(freeSpecialCD));
        }
        specialCDText->setVisible(true);
    }
    
}
void HallLuckyLayer::clickNormalButton()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_normal_draw;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallLuckyLayer::clickSpecialButton()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_special_draw_one;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallLuckyLayer::clickSpecialTenButton()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_special_draw_ten;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallLuckyLayer::clickNormal()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    boundsLayer1 = Layer::create();
    auto root = CSLoader::createNode(LUCKY_REWARD1_UI);
    bounds1Action = CSLoader::createTimeline(LUCKY_REWARD1_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(bounds1Action);
    boundsLayer1->addChild(root);
    addChild(boundsLayer1);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,boundsLayer1);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button*)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBounds1Close, this));
    bounds1Action->play("in", false);
}

void HallLuckyLayer::clickSpecial()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    boundsLayer2 = Layer::create();
    auto root = CSLoader::createNode(LUCKY_REWARD2_UI);
    bounds2Action = CSLoader::createTimeline(LUCKY_REWARD2_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(bounds2Action);
    boundsLayer2->addChild(root);
    addChild(boundsLayer2);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,boundsLayer2);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button*)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBounds2Close, this));
    bounds2Action->play("in", false);
}

void HallLuckyLayer::clickAddSpecialTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    showBuyLayer(1);
}

void HallLuckyLayer::updateNormalDraw(cocos2d::EventCustom *event)
{
    L2E_DRAW_ONE info = *static_cast<L2E_DRAW_ONE*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("normal_draw_err%d", info.errNo));

        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    if (info.normalCD != 0) {
        freeNormalCD = info.normalCD;
        normalCDText->setString(TimeUtil::timeFormatToHMS(info.normalCD));
    }
    
    normalCountText->setString(GameUtils::format("%d/1", info.normalTicket));
    if (info.normalState == 1) {
        normalButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        normalCDText->setVisible(false);
        normalTip->setVisible(true);
    }else{
        normalButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        normalCDText->setVisible(true);
        normalTip->setVisible(false);
    }
    
    rewardLayer1 = Layer::create();
    auto root = CSLoader::createNode(LUCKY_GET1_UI);
    reward1Action = CSLoader::createTimeline(LUCKY_GET1_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(reward1Action);
    rewardLayer1->addChild(root);
    addChild(rewardLayer1);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rewardLayer1);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    auto icon = (ImageView*)rootBg->getChildByName("icon");
    auto name = (Text*)icon->getChildByName("name");
    auto count = (Text*)icon->getChildByName("count");
    icon->loadTexture(info.icon);
    if (info.frag) {
        auto fragSprite = Sprite::create("ui/common/Enemy_icon_0.png");
        fragSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fragSprite->setPosition(icon->getContentSize()/2);
        icon->addChild(fragSprite);
    }
    name->setString(info.name);
    count->setString(Convert2String(info.count));
    
    reward1Action->play("play", false);
//    reward1Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::removeReward1, this));
    reward1Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::endRewardIn1, this));
}

void HallLuckyLayer::updateSpecialDrawOne(cocos2d::EventCustom *event)
{

    L2E_DRAW_ONE info = *static_cast<L2E_DRAW_ONE*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("special_draw_err%d", info.errNo));
        
        if (info.errNo == 3) {
            showBuyLayer(1);
            return;
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    if (info.specialCD != 0) {
        freeSpecialCD = info.specialCD;
        specialCDText->setString(TimeUtil::timeFormatToHMS(info.specialCD));
    }
    
    specialCountText->setString(GameUtils::format("%d/1", info.specialTicket));
    specialTenCountText->setString(GameUtils::format("%d/10", info.specialTicket));
    
    if (info.specialState == 1) {
        specialButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        specialCDText->setVisible(false);
        specialTip->setVisible(true);
    }else{
        specialButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        specialCDText->setVisible(true);
        specialTip->setVisible(false);
    }
    
    rewardLayer1 = Layer::create();
    auto root = CSLoader::createNode(LUCKY_GET1_UI);
    reward1Action = CSLoader::createTimeline(LUCKY_GET1_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(reward1Action);
    rewardLayer1->addChild(root);
    addChild(rewardLayer1);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rewardLayer1);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    auto icon = (ImageView*)rootBg->getChildByName("icon");
    auto name = (Text*)icon->getChildByName("name");
    auto count = (Text*)icon->getChildByName("count");
    icon->loadTexture(info.icon);
    if (info.frag) {
        auto fragSprite = Sprite::create("ui/common/Enemy_icon_0.png");
        fragSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fragSprite->setPosition(icon->getContentSize()/2);
        icon->addChild(fragSprite);
    }
    name->setString(info.name);
    count->setString(Convert2String(info.count));
    
    reward1Action->play("play", false);
//    reward1Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::removeReward1, this));
    reward1Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::endRewardIn1, this));
}

void HallLuckyLayer::updateSpecialDrawTen(cocos2d::EventCustom *event)
{
    L2E_DRAW_TEN info = *static_cast<L2E_DRAW_TEN*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("special_draw_err%d", info.errNo));
        
        if (info.errNo == 2) {
            showBuyLayer(10-info.specialTicket);
            return;
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    specialCountText->setString(GameUtils::format("%d/1", info.specialTicket));
    specialTenCountText->setString(GameUtils::format("%d/10", info.specialTicket));
    
    if (info.specialState == 1) {
        specialButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        specialCDText->setVisible(false);
        specialTip->setVisible(true);
    }else{
        specialButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        specialCDText->setVisible(true);
        specialTip->setVisible(false);
    }

    rewardLayer2 = Layer::create();
    auto root = CSLoader::createNode(LUCKY_GET2_UI);
    reward2Action = CSLoader::createTimeline(LUCKY_GET2_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(reward2Action);
    rewardLayer2->addChild(root);
    addChild(rewardLayer2);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rewardLayer2);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    for (int i = 0; i < 10; i++) {
        auto icon = (ImageView*)rootBg->getChildByTag(i+1);
        auto name = (Text*)icon->getChildByName("name");
        auto count = (Text*)icon->getChildByName("count");
        icon->loadTexture(info.icon[i]);
        if (info.frag[i]) {
            auto fragSprite = Sprite::create("ui/common/Enemy_icon_0.png");
            fragSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            fragSprite->setPosition(icon->getContentSize()/2);
            icon->addChild(fragSprite);
        }
        name->setString(info.name[i]);
        count->setString(Convert2String(info.count[i]));
    }
    
    
    reward2Action->play("play", false);
//    reward2Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::removeReward2, this));
    reward2Action->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallLuckyLayer::endRewardIn2, this));

}

void HallLuckyLayer::updateFreeNormal(cocos2d::EventCustom *event)
{
    L2E_UPDATE_DRAW info = *static_cast<L2E_UPDATE_DRAW*>(event->getUserData());
    normalCDText->setString(TimeUtil::timeFormatToHMS(info.normalCD));
    if (info.normalState == 1) {
        normalButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        normalCDText->setVisible(false);
        normalTip->setVisible(true);
    }else{
        normalButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        normalCDText->setVisible(true);
        normalTip->setVisible(false);
    }
}

void HallLuckyLayer::updateFreeSpecial(cocos2d::EventCustom *event)
{
    L2E_UPDATE_DRAW info = *static_cast<L2E_UPDATE_DRAW*>(event->getUserData());
    specialCDText->setString(TimeUtil::timeFormatToHMS(info.specialCD));
    if (info.specialState == 1) {
        specialButton->setTitleText(StringData::shared()->stringFromKey("free_draw_one"));
        specialCDText->setVisible(false);
        specialTip->setVisible(true);
    }else{
        specialButton->setTitleText(StringData::shared()->stringFromKey("draw_one"));
        specialCDText->setVisible(true);
        specialTip->setVisible(false);
    }
}

void HallLuckyLayer::updateSpecialTicket(cocos2d::EventCustom *event)
{
    L2E_UPDATE_SPECIAL_TICKET info = *static_cast<L2E_UPDATE_SPECIAL_TICKET*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    specialCountText->setString(GameUtils::format("%d/1", info.count));
    specialTenCountText->setString(GameUtils::format("%d/10", info.count));
}

void HallLuckyLayer::showBuyLayer(int count)
{
    buyLayer = Layer::create();
    auto root = CSLoader::createNode(LUCKY_BUY_TICKET_UI);
    buyAction = CSLoader::createTimeline(LUCKY_BUY_TICKET_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(buyAction);
    buyLayer->addChild(root);
    addChild(buyLayer);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,buyLayer);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button*)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBuyClose, this));
    auto okButton = (Button*)rootBg->getChildByName("buy_button");
    okButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBuyOk, this));
    auto buyBg = (ImageView*)rootBg->getChildByName("buy_bg");
    countText = (Text*)buyBg->getChildByName("count_text");
    countText->setString(Convert2String(count));
    buyCount = count;
    priceText = (Text*)buyBg->getChildByName("price_text");
    priceText->setString(Convert2String(buyCount*luckyTicketPrice));
    auto buyPlusButton = (Button*)buyBg->getChildByName("plus_button");
    buyPlusButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBuyPlus, this));
    auto buyReduceButton = (Button*)buyBg->getChildByName("reduce_button");
    buyReduceButton->addClickEventListener(CC_CALLBACK_0(HallLuckyLayer::clickBuyReduce, this));
    buyAction->play("in", false);
}

void HallLuckyLayer::clickBuyPlus()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyCount++;
    countText->setString(Convert2String(buyCount));
    priceText->setString(Convert2String(buyCount*luckyTicketPrice));
}

void HallLuckyLayer::clickBuyReduce()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyCount--;
    buyCount = MAX(1, buyCount);
    countText->setString(Convert2String(buyCount));
    priceText->setString(Convert2String(buyCount*luckyTicketPrice));
}

void HallLuckyLayer::clickBuyClose()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeBuyOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallLuckyLayer::clickBuyOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_buy_special_ticket;
    info.value = buyCount;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallLuckyLayer::clickBounds1Close()
{
    bounds1Action->play("out", false);
    bounds1Action->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeBounds1, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallLuckyLayer::clickBounds2Close()
{
    bounds2Action->play("out", false);
    bounds2Action->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeBounds2, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}
void HallLuckyLayer::removeBuyOff()
{
    buyCount = 0;
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void HallLuckyLayer::removeBounds1()
{
    removeChild(boundsLayer1);
    boundsLayer1 = nullptr;
}

void HallLuckyLayer::removeBounds2()
{
    removeChild(boundsLayer2);
    boundsLayer2 = nullptr;
}

bool HallLuckyLayer::touchReward1(cocos2d::Touch *touch, cocos2d::Event *event)
{
    reward1Action->play("out", false);
    reward1Action->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeReward1, this));
    return true;
}
bool HallLuckyLayer::touchReward2(cocos2d::Touch *touch, cocos2d::Event *event)
{
    reward2Action->play("out", false);
    reward2Action->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallLuckyLayer::removeReward2, this));
    return true;
}
void HallLuckyLayer::removeReward1()
{
    getEventDispatcher()->removeEventListenersForTarget(rewardLayer1);

    removeChild(rewardLayer1);
    rewardLayer1 = nullptr;
}

void HallLuckyLayer::removeReward2()
{
    getEventDispatcher()->removeEventListenersForTarget(rewardLayer2);

    removeChild(rewardLayer2);
    rewardLayer2 = nullptr;
}
