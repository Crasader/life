//
//  ItemSweepLayer.cpp
//  life
//
//  Created by ff on 17/7/26.
//
//

#include "ItemSweepLayer.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

ItemSweepLayer::ItemSweepLayer()
{

}
void ItemSweepLayer::onEnter()
{
    Layer::onEnter();
    updateVITListener = EventListenerCustom::create(UPDATE_VIT, CC_CALLBACK_1(ItemSweepLayer::updateVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITListener, -1);
    sweepItemListener = EventListenerCustom::create(SWEEP_ITEM, CC_CALLBACK_1(ItemSweepLayer::sweepItem, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sweepItemListener, -1);
    showBuyVITListener = EventListenerCustom::create(SHOW_BUY_VIT, CC_CALLBACK_1(ItemSweepLayer::showBuyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBuyVITListener, -1);
//    updateVITBuyCountListener = EventListenerCustom::create(UPDATE_VIT_BUY_COUNT, CC_CALLBACK_1(ItemSweepLayer::updateVITBuyCount, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITBuyCountListener, -1);
    buyVITListener = EventListenerCustom::create(BUY_VIT, CC_CALLBACK_1(ItemSweepLayer::buyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyVITListener, -1);
    updateVITCDListener = EventListenerCustom::create(UPDATE_VIT_CD, CC_CALLBACK_1(ItemSweepLayer::updateVITCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITCDListener, -1);
    scheduleUpdate();
    
}
ItemSweepLayer::~ItemSweepLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(sweepItemListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBuyVITListener);
//    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITBuyCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITCDListener);
    unscheduleUpdate();
}

bool ItemSweepLayer::init()
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

    auto root = CSLoader::createNode(ITEM_SWEEP_UI);
    rootAction = CSLoader::createTimeline(ITEM_SWEEP_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(rootAction);
    addChild(root);
    
    auto bg = (ImageView*)root->getChildByName("bg_img");
    auto closeButton = (Button*)bg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickClose, this));
    sweepButton = (Button*)bg->getChildByName("start_button");
    sweepButton->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickSweep, this));
    
    addVITButton = (Button*)bg->getChildByName("VIT_img")->getChildByName("add_button");
    addVITButton->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickAddVIT, this));
    VITText = (Text*)bg->getChildByName("VIT_img")->getChildByName("count_text");
    goldText = (Text*)bg->getChildByName("gold_img")->getChildByName("count_text");
    expText = (Text*)bg->getChildByName("exp_img")->getChildByName("count_text");
    itemImg = (ImageView*)bg->getChildByName("item_img");
    itemCountText = (Text*)itemImg->getChildByName("count_text");
    icon = (ImageView*)bg->getChildByName("item_img_1");
    countText = (Text*)bg->getChildByName("count_text");
    otherText = (Text*)bg->getChildByName("Text_2_0");
    sweepCountText = (Text*)bg->getChildByName("sweep_count");
    multiCheck = (CheckBox*)bg->getChildByName("single_check");
    multiCheck->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickMulti, this));
    multi = true;
    sweepList = (ListView*)bg->getChildByName("sweep_list");
    
    VITCDText = (Text*)bg->getChildByName("VIT_cd_text");
    VITCDTipText = (Text*)bg->getChildByName("VIT_cd_tip_text");
    buyLayer = nullptr;
    vitCD = 0;
    return true;
}

void ItemSweepLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_ITEM_SWEEP info = *static_cast<L2E_SHOW_ITEM_SWEEP*>(event->getUserData());
    VITText->setString(GameUtils::format("%d/%d", info.currVIT, info.maxVIT));
    itemImg->loadTexture(GameUtils::format(COMMON_DIR, info.icon.c_str()));
    icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon.c_str()));
    goldText->setString(Convert2String(info.dropGold));
    expText->setString(Convert2String(info.dropExp));
    itemCountText->setString(Convert2String(info.dropItemCount));
    if (info.requireCount > 0) {
        countText->setString(Convert2String(info.requireCount));
    }else{
        icon->setVisible(false);
        countText->setVisible(false);
        otherText->setVisible(false);
    }
    itemTag = info.itemTag;
    requireCount = info.requireCount;
    dropGold = info.dropGold;
    dropExp = info.dropExp;
    dropItemCount = info.dropItemCount;
    itemIcon = info.icon;
    showing = false;
    sweepTotalCount = 0;
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(ItemSweepLayer::endIn, this));
}
void ItemSweepLayer::update(float dt)
{
    if (vitCD > 0) {
        vitCD -= dt;
        if (vitCD <= 0) {
            VITCDText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            std::string timeStr = TimeUtil::timeFormatToMS(vitCD);
            VITCDText->setString(timeStr);
        }
    }
}
void ItemSweepLayer::showBuyVIT(cocos2d::EventCustom *event)
{
    L2E_SHOW_BUY_VIT info = *static_cast<L2E_SHOW_BUY_VIT*>(event->getUserData());
    buyLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    //    rewardLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //    rewardLayer->setPosition(GameUtils::winSize/2);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, buyLayer);
    
    addChild(buyLayer,100);
    
    auto root = CSLoader::createNode(BUY_VIT_UI);
    buyAction = CSLoader::createTimeline(BUY_VIT_UI);
    root->runAction(buyAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    buyLayer->addChild(root);
    buyAction->play("in", false);
    
    vitBuyCountText = (Text*)root->getChildByName("bg_img")->getChildByName("count_text");
    priceText = (Text*)root->getChildByName("bg_img")->getChildByName("price_text");
    vitBuyCountText->setString(Convert2String(info.buyCount));
    priceText->setString(Convert2String(info.price));
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickCloseBuy, this));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("ok_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(ItemSweepLayer::clickBuy, this));
}

void ItemSweepLayer::updateVIT(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT info = *static_cast<L2E_UPDATE_VIT*>(event->getUserData());
    VITText->setString(GameUtils::format("%d/%d", info.currVIT, info.maxVIT));
}

void ItemSweepLayer::updateVITCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT_CD info = *static_cast<L2E_UPDATE_VIT_CD*>(event->getUserData());
    if (info.state) {
        vitCD = info.leftSec;
        VITCDTipText->setVisible(true);
        VITCDText->setVisible(true);
    }else{
        vitCD = 0;
        VITCDTipText->setVisible(false);
        VITCDText->setVisible(false);
    }
}

void ItemSweepLayer::sweepItem(cocos2d::EventCustom *event)
{
    L2E_ITEM_SWEEP info = *static_cast<L2E_ITEM_SWEEP*>(event->getUserData());
    if(info.errNo > 0)
    {
        std::string errStr = StringData::shared()->stringFromKey("vit_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    sweepButton->setTitleText(StringData::shared()->stringFromKey("sweep_again"));
    sweepCountText->setString(GameUtils::format(StringData::shared()->stringFromKey("sweep_count").c_str(), info.count).c_str());
    
    showing = true;
    sweepList->removeAllItems();
    sweepTotalCount = info.count;
    for (int i = 0; i < 10; i++) {
        valid[i] = info.getItem[i];
    }
    showCount = 0;
    
    auto func = CallFunc::create(CC_CALLBACK_0(ItemSweepLayer::pushResult2List, this));
    auto seq = Sequence::create(DelayTime::create(0.2), func, NULL);
    runAction(seq);
}

void ItemSweepLayer::pushResult2List()
{
    auto sweepNode = CSLoader::createNode(ITEM_SWEEP_NODE_UI);
    auto sweepBg = (ImageView*)sweepNode->getChildByName("bg_img");
    auto countText = (Text*)sweepBg->getChildByName("Text_40");
    auto goldCount = (Text*)sweepBg->getChildByName("gold_img")->getChildByName("count_text");
    auto expCount = (Text*)sweepBg->getChildByName("exp_img")->getChildByName("count_text");
    auto icon = (ImageView*)sweepBg->getChildByName("item_img");
    auto itemCount = (Text*)icon->getChildByName("count_text");
    countText->setString(GameUtils::format(StringData::shared()->stringFromKey("count").c_str(), showCount+1));
    goldCount->setString(Convert2String(dropGold));
    expCount->setString(Convert2String(dropExp));
    itemCount->setString(Convert2String(dropItemCount));
    icon->loadTexture(GameUtils::format(COMMON_DIR, itemIcon.c_str()));
    icon->setVisible(valid[showCount]);
    
    sweepBg->removeFromParent();
    sweepList->insertCustomItem(sweepBg, showCount);
    sweepList->scrollToBottom(0.5, false);
    
    if (requireCount > 0 && valid[showCount]) {
        requireCount -= dropItemCount;
        requireCount = MAX(0, requireCount);
        this->countText->setString(Convert2String(requireCount));
    }
    showCount++;
    sweepTotalCount--;
    if (sweepTotalCount > 0) {
        auto func = CallFunc::create(CC_CALLBACK_0(ItemSweepLayer::pushResult2List, this));
        auto seq = Sequence::create(DelayTime::create(1), func, NULL);
        runAction(seq);
    }else{
        showing = false;
    }
}

void ItemSweepLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ItemSweepLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void ItemSweepLayer::removeOff()
{
    removeFromParent();
}

void ItemSweepLayer::clickSweep()
{
    if (showing) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_START_ITEM_SWEEP info;
    info.eProtocol = e2l_start_sweep_item;
    info.itemTag = itemTag;
    info.count = (multi?10:1);
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ItemSweepLayer::clickMulti()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    multi = !multi;
}

void ItemSweepLayer::clickAddVIT()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ItemSweepLayer::clickCloseBuy()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ItemSweepLayer::removeBuy, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void ItemSweepLayer::removeBuy()
{
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void ItemSweepLayer::clickBuy()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ItemSweepLayer::updateVITBuyCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (buyLayer) {
        vitBuyCountText->setString(Convert2String(info.value));
    }
}

void ItemSweepLayer::buyVIT(cocos2d::EventCustom *event)
{
    L2E_BUY_VIT info = *static_cast<L2E_BUY_VIT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("buy_vit_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    vitBuyCountText->setString(Convert2String(info.buyCount));
}

void ItemSweepLayer::endIn()
{
    rootAction->play("play", true);
    E2L_COMMON info;
    info.eProtocol = e2l_get_vit_cd;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
