//
//  HallDiscountLayer.cpp
//  life
//
//  Created by ff on 18/1/15.
//
//

#include "HallDiscountLayer.h"
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

HallDiscountLayer::HallDiscountLayer():
packageId(0),
openSec(0)
{
    
}

HallDiscountLayer::~HallDiscountLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateStateListener);
    unscheduleUpdate();
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallDiscountLayer::init()
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
    
    auto root = CSLoader::createNode(DISCOUNT_UI);
    rootAction = CSLoader::createTimeline(DISCOUNT_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallDiscountLayer::clickClose, this));
    
    titleImg = (ImageView*)rootBg->getChildByName("title_img");
    okButton = (Button*)titleImg->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(HallDiscountLayer::clickOk, this));

    
    itemList = (ListView*)rootBg->getChildByName("item_list");
    priceText = (Text*)titleImg->getChildByName("price_text");
    doublePriceText = (Text*)titleImg->getChildByName("double_price_text");
    
    cdText = (Text*)rootBg->getChildByName("cd_bg")->getChildByName("cd_text");
    scheduleUpdate();
    return true;
}

void HallDiscountLayer::onEnter()
{
    Layer::onEnter();
    updateStateListener = EventListenerCustom::create(UPDATE_DISCOUNT_STATE, CC_CALLBACK_1(HallDiscountLayer::updateState, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateStateListener, -1);
}

void HallDiscountLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_DISCOUNT info = *static_cast<L2E_SHOW_DISCOUNT*>(event->getUserData());
    if(info.state == 0)
    {
        openSec = 0;
        cdText->setString(StringData::shared()->stringFromKey("discount_close"));
        itemList->removeAllItems();
        titleImg->setVisible(false);
//        okButton->setVisible(false);
        //        priceText->setVisible(false);
        //        doublePriceText->setVisible(false);
    }else{
        packageId = info.packageId;
        openSec = info.leftSec;
        std::string timeStr = TimeUtil::timeFormatToMS(openSec);
        std::string cdStr = StringUtils::format(StringData::shared()->stringFromKey("discount_open").c_str(), timeStr.c_str());
        cdText->setString(cdStr);
        
        titleImg->setVisible(true);
//        okButton->setVisible(true);
        //        priceText->setVisible(true);
        //        doublePriceText->setVisible(true);
        priceText->setString(Convert2String(info.price));
        doublePriceText->setString(Convert2String(info.price*2));
        titleImg->loadTexture(StringUtils::format(HOME_DIR, info.packageTitle.c_str()));
        
        itemList->removeAllItems();
        for (int i = 0; i < info.itemCount; i++) {
            auto item = CSLoader::createNode(SHOP_PACKAGE_ITEM_UI);
            auto itemBg = (ImageView*)item->getChildByName("bg_img");
            itemBg->retain();
            itemBg->removeFromParent();
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(info.icon[i]);
            auto count = (Text*)itemBg->getChildByName("count_text");
            count->setString(Convert2String(info.count[i]));
            itemList->addChild(itemBg);
            itemBg->release();
        }
    }
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallDiscountLayer::endIn, this));
}

void HallDiscountLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_COMMON info;
    info.eProtocol = e2l_update_discount_state;
    ClientLogic::instance()->ProcessUIRequest(&info);
    rootAction->play("play", true);
}

void HallDiscountLayer::updateState(cocos2d::EventCustom *event)
{
    L2E_UPDATE_DISCOUNT_STATE info = *static_cast<L2E_UPDATE_DISCOUNT_STATE*>(event->getUserData());
    if(info.state == 0)
    {
        openSec = 0;
        cdText->setString(StringData::shared()->stringFromKey("discount_close"));
        itemList->removeAllItems();
        titleImg->setVisible(false);
//        okButton->setVisible(false);
//        priceText->setVisible(false);
//        doublePriceText->setVisible(false);
    }else{
        packageId = info.packageId;
        openSec = info.leftSec;
        std::string timeStr = TimeUtil::timeFormatToMS(openSec);
        std::string cdStr = StringUtils::format(StringData::shared()->stringFromKey("discount_open").c_str(), timeStr.c_str());
        cdText->setString(cdStr);
        
        titleImg->setVisible(true);
//        okButton->setVisible(true);
//        priceText->setVisible(true);
//        doublePriceText->setVisible(true);
        priceText->setString(Convert2String(info.price));
        doublePriceText->setString(Convert2String(info.price*2));
        titleImg->loadTexture(StringUtils::format(HOME_DIR, info.packageTitle.c_str()));
        
        itemList->removeAllItems();
        for (int i = 0; i < info.itemCount; i++) {
            auto item = CSLoader::createNode(SHOP_PACKAGE_ITEM_UI);
            auto itemBg = (ImageView*)item->getChildByName("bg_img");
            itemBg->retain();
            itemBg->removeFromParent();
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(info.icon[i]);
            auto count = (Text*)itemBg->getChildByName("count_text");
            count->setString(Convert2String(info.count[i]));
            itemList->addChild(itemBg);
            itemBg->release();
        }
    }
}

void HallDiscountLayer::update(float dt)
{
    if (openSec > 0) {
        openSec -= dt;
        if (openSec <= 0) {
            cdText->setString(StringData::shared()->stringFromKey("discount_close"));
            itemList->removeAllItems();
            titleImg->setVisible(false);
//            okButton->setVisible(false);
//            priceText->setVisible(false);
//            doublePriceText->setVisible(false);
        }else{
            std::string timeStr = TimeUtil::timeFormatToMS(openSec);
            std::string cdStr = StringUtils::format(StringData::shared()->stringFromKey("discount_open").c_str(), timeStr.c_str());
            cdText->setString(cdStr);
        }
    }
}

void HallDiscountLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallDiscountLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallDiscountLayer::clickOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_buy_package;
    info.value = packageId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
//    clickClose();
}

void HallDiscountLayer::removeOff()
{
    removeFromParent();
}
