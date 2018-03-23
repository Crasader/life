//
//  ItemWayLayer.cpp
//  life
//
//  Created by ff on 17/7/25.
//
//

#include "ItemWayLayer.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "utils/StringData.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

ItemWayLayer::ItemWayLayer():
plotButtonId(0),
plotMissionId(0)
{
}

void ItemWayLayer::onEnter()
{
    Layer::onEnter();
    updateItemCountListener = EventListenerCustom::create(UPDATE_ITEM_COUNT, CC_CALLBACK_1(ItemWayLayer::updateItemCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateItemCountListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(ItemWayLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
}

ItemWayLayer::~ItemWayLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateItemCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
}
bool ItemWayLayer::init()
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

    auto root = CSLoader::createNode(ITEM_WAY_UI);
    rootAction = CSLoader::createTimeline(ITEM_WAY_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(rootAction);
    addChild(root);
    
    closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ItemWayLayer::clickClose, this));
    auto sweepButton = (Button*)root->getChildByName("bg_img")->getChildByName("sweep_bg")->getChildByName("sweep_button");
    sweepButton->addClickEventListener(CC_CALLBACK_0(ItemWayLayer::clickSweep, this));
    auto buyBg = (ImageView*)root->getChildByName("bg_img")->getChildByName("buy_bg");
    auto buyButton = (Button*)buyBg->getChildByName("buy_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(ItemWayLayer::clickBuy, this));
    auto reduceButton = (Button*)buyBg->getChildByName("reduce_button");
    reduceButton->addClickEventListener(CC_CALLBACK_0(ItemWayLayer::clickReduce, this));
    auto plusButton = (Button*)buyBg->getChildByName("plus_button");
    plusButton->addClickEventListener(CC_CALLBACK_0(ItemWayLayer::clickPlus, this));

    countText = (Text*)buyBg->getChildByName("count_text");
    priceText = (Text*)buyBg->getChildByName("price_text");
    return true;
}

void ItemWayLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_ITEM_WAY info = *static_cast<L2E_SHOW_ITEM_WAY*>(event->getUserData());
    requireCount = info.count;
    count = info.count;
    price = info.price;
    totalDiamond = info.totalDiamond;
    itemTag = info.itemTag;
    countText->setString(Convert2String(count));
    priceText->setString(Convert2String(price*count));
    if (totalDiamond >= price*count) {
        priceText->setTextColor(Color4B::GREEN);
    }else{
        priceText->setTextColor(Color4B::RED);
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(ItemWayLayer::endIn, this));
}

void ItemWayLayer::updateItemCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ITEM_COUNT info = *static_cast<L2E_UPDATE_ITEM_COUNT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr("");
        switch (info.errNo) {
            case 1:
            {
//                errStr = StringData::shared()->stringFromKey("diamond_not_enough").c_str();
                E2L_UPDATE_ONE_VALUE info;
                info.eProtocol = e2l_shop_package_detail;
                info.value = 8;//初级材料礼包
                ClientLogic::instance()->ProcessUIRequest(&info);
            }
                return;
            case 2:
            {
                errStr = StringData::shared()->stringFromKey("buy_item_err").c_str();
            }
                break;
                
            default:
                break;
        }
        
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    requireCount -= count;
    requireCount = MAX(0, requireCount);
    totalDiamond = info.totalDiamond;
    
    if (count >= requireCount) {
        countText->setTextColor(Color4B::GREEN);
    }else{
        countText->setTextColor(Color4B::RED);
    }
    if (totalDiamond >= price*count) {
        priceText->setTextColor(Color4B::GREEN);
    }else{
        priceText->setTextColor(Color4B::RED);
    }

}

void ItemWayLayer::clickClose()
{
    if (plotButtonId == 1) {
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
        infoTrigger.index = ITEM_WAY_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    removeFromParent();
}

void ItemWayLayer::clickReduce()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    count--;
    count = MAX(1, count);
    countText->setString(Convert2String(count));
    if (count >= requireCount) {
        countText->setTextColor(Color4B::GREEN);
    }else{
        countText->setTextColor(Color4B::RED);
    }
    priceText->setString(Convert2String(price*count));
    if (totalDiamond >= price*count) {
        priceText->setTextColor(Color4B::GREEN);
    }else{
        priceText->setTextColor(Color4B::RED);
    }
}

void ItemWayLayer::clickPlus()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    count++;
    countText->setString(Convert2String(count));
    if (count >= requireCount) {
        countText->setTextColor(Color4B::GREEN);
    }else{
        countText->setTextColor(Color4B::RED);
    }
    priceText->setString(Convert2String(price*count));
    if (totalDiamond >= price*count) {
        priceText->setTextColor(Color4B::GREEN);
    }else{
        priceText->setTextColor(Color4B::RED);
    }
}

void ItemWayLayer::clickBuy()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    E2L_BUY_EQUIP_ITEM info;
    info.eProtocol = e2l_buy_equip_item;
    info.count = count;
    info.itemTag = itemTag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ItemWayLayer::clickSweep()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_sweep_item;
    info.value = itemTag;
    ClientLogic::instance()->ProcessUIRequest(&info);
    removeFromParent();
}

void ItemWayLayer::endIn()
{
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = ITEM_WAY_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ItemWayLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] != ITEM_WAY_LAYER)
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
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    pointTip->setPosition(Vec2(150, 35));
                    
                    point->setPosition(closeButton->getContentSize()/2);
                    point->setTag(1000);

                    closeButton->removeChildByTag(1000);
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
