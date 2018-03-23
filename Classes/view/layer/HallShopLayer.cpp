//
//  HallShopLayer.cpp
//  life
//
//  Created by ff on 17/11/8.
//
//

#include "HallShopLayer.h"
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

HallShopLayer::HallShopLayer()
{
}

void HallShopLayer::onEnter()
{
    Layer::onEnter();
    updateShopListener = EventListenerCustom::create(SHOW_SHOP, CC_CALLBACK_1(HallShopLayer::updateShop, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateShopListener, -1);
    updateResultListener = EventListenerCustom::create(SHOW_PACKAGE_RESULT, CC_CALLBACK_1(HallShopLayer::updateBuyResult, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateResultListener, -1);
    
}

HallShopLayer::~HallShopLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateShopListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateResultListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallShopLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_SHOP_UI);
    rootAction = CSLoader::createTimeline(HALL_SHOP_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallShopLayer::clickClose, this));
    
    buttonBg = (ImageView*)rootBg->getChildByName("button_bg");
    typeButton[0] = (Button*)buttonBg->getChildByTag(1);
    typeButton[0]->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickShopButton, this));
    typeButton[1] = (Button*)buttonBg->getChildByTag(2);
    typeButton[1]->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickShopButton, this));
    typeButton[2] = (Button*)buttonBg->getChildByTag(3);
    typeButton[2]->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickShopButton, this));
    itemList = (ListView*)rootBg->getChildByName("item_list");
    return true;
}

void HallShopLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_SHOP info = *static_cast<L2E_SHOW_HALL_SHOP*>(event->getUserData());
    shopType = info.type;
    if (info.type == 5) {
        shopType = 3;
    }
    for (int i = 0; i < 3; i++) {
        if (i == shopType-1) {
            typeButton[i]->setBright(false);
            typeButton[i]->setEnabled(false);
        }else{
            typeButton[i]->setBright(true);
            typeButton[i]->setEnabled(true);
        }
    }
    std::string itemPath("");
    if (shopType == 1) {
        itemPath = SHOP_PACKAGE1_UI;
    }else if (shopType == 2) {
        itemPath = SHOP_PACKAGE2_UI;
    }else{
        itemPath = SHOP_PACKAGE3_UI;
    }

    itemList->removeAllItems();
//    itemList->getInnerContainer()->setContentSize(Size::ZERO);
    for (int i = 0; i < info.count; i++) {
        auto item = CSLoader::createNode(itemPath);
        auto itemAction = CSLoader::createTimeline(itemPath);
        auto itemBg = (ImageView*)item->getChildByName("bg_img");
        itemBg->retain();
        itemBg->removeFromParent();
        itemBg->setTag(info.shopId[i]);
        if (shopType == 1) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto countText1 = (Text*)itemBg->getChildByName("count_text1");
            auto countText2 = (Text*)itemBg->getChildByName("count_text2");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            auto priceBg = (ImageView*)itemBg->getChildByName("price_bg");
            auto priceText = (Text*)priceBg->getChildByName("price_text");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            priceText->setString(Convert2String(info.price[i]));
            countText1->setString(Convert2String(info.boundCount[i]/2));
            countText2->setString(Convert2String(info.boundCount[i]/2));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            okButton->setSwallowTouches(false);
        }else if (shopType == 2) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto priceBg = (ImageView*)itemBg->getChildByName("price_bg");
            auto priceText = (Text*)priceBg->getChildByName("price_text");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            auto detailButton = (Button*)itemBg->getChildByName("detail_button");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            priceText->setString(Convert2String(info.price[i]));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            detailButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageDetail, this));
            okButton->setSwallowTouches(false);
            detailButton->setSwallowTouches(false);
        }else if (shopType == 3) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto countText = (Text*)itemBg->getChildByName("count_text");
            auto priceText = (Text*)itemBg->getChildByName("price_text");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            if (info.boundCount[i] > 100000) {
                countText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.boundCount[i]/10000).c_str());
            }else{
                countText->setString(Convert2String(info.boundCount[i]));
            }
            
            priceText->setString(Convert2String(info.price[i]));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            okButton->setSwallowTouches(false);
        }
        itemBg->runAction(itemAction);
        itemAction->play("play", true);
        itemList->addChild(itemBg);
        itemBg->release();
    }
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallShopLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);
}
void HallShopLayer::updateBuyResult(cocos2d::EventCustom *event)
{
    L2E_SHOW_PACKAGE_RESULT info = *static_cast<L2E_SHOW_PACKAGE_RESULT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        if (info.errNo == 1) {
            errStr = StringData::shared()->stringFromKey("buy_err");
            return;
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
}
void HallShopLayer::updateShop(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_SHOP info = *static_cast<L2E_SHOW_HALL_SHOP*>(event->getUserData());
    shopType = info.type;
    if (info.type == 5) {
        shopType = 3;
    }
    for (int i = 0; i < 3; i++) {
        if (i == shopType-1) {
            typeButton[i]->setBright(false);
            typeButton[i]->setEnabled(false);
        }else{
            typeButton[i]->setBright(true);
            typeButton[i]->setEnabled(true);
        }
    }
    std::string itemPath("");
    if (shopType == 1) {
        itemPath = SHOP_PACKAGE1_UI;
    }else if (shopType == 2) {
        itemPath = SHOP_PACKAGE2_UI;
    }else{
        itemPath = SHOP_PACKAGE3_UI;
    }
    itemList->removeAllItems();
//    itemList->getInnerContainer()->setContentSize(Size::ZERO);
    for (int i = 0; i < info.count; i++) {
        auto item = CSLoader::createNode(itemPath);
        auto itemAction = CSLoader::createTimeline(itemPath);
        auto itemBg = (ImageView*)item->getChildByName("bg_img");
        itemBg->retain();
        itemBg->removeFromParent();
        itemBg->setTag(info.shopId[i]);
        if (shopType == 1) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto countText1 = (Text*)itemBg->getChildByName("count_text1");
            auto countText2 = (Text*)itemBg->getChildByName("count_text2");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            auto priceBg = (ImageView*)itemBg->getChildByName("price_bg");
            auto priceText = (Text*)priceBg->getChildByName("price_text");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            priceText->setString(Convert2String(info.price[i]));
            countText1->setString(Convert2String(info.boundCount[i]/2));
            countText2->setString(Convert2String(info.boundCount[i]/2));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            okButton->setSwallowTouches(false);
        }else if (shopType == 2) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto priceBg = (ImageView*)itemBg->getChildByName("price_bg");
            auto priceText = (Text*)priceBg->getChildByName("price_text");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            auto detailButton = (Button*)itemBg->getChildByName("detail_button");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            priceText->setString(Convert2String(info.price[i]));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            detailButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageDetail, this));
            okButton->setSwallowTouches(false);
            detailButton->setSwallowTouches(false);
        }else if (shopType == 3) {
            auto nameText = (Text*)itemBg->getChildByName("name_text");
            auto icon = (ImageView*)itemBg->getChildByName("icon");
            auto countText = (Text*)itemBg->getChildByName("count_text");
            auto priceText = (Text*)itemBg->getChildByName("price_text");
            auto okButton = (Button*)itemBg->getChildByName("ok_button");
            nameText->setString(info.name[i]);
            icon->setUnifySizeEnabled(true);
            icon->loadTexture(GameUtils::format(COMMON_DIR, info.icon[i].c_str()));
            if (info.boundCount[i] > 100000) {
                countText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.boundCount[i]/10000).c_str());
            }else{
                countText->setString(Convert2String(info.boundCount[i]));
            }
            
            priceText->setString(Convert2String(info.price[i]));
            okButton->addClickEventListener(CC_CALLBACK_1(HallShopLayer::clickPackageOk, this));
            okButton->setSwallowTouches(false);
        }
        itemBg->runAction(itemAction);
        itemAction->play("play", true);
        itemList->addChild(itemBg);
        itemBg->release();
    }
}
void HallShopLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallShopLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallShopLayer::removeOff()
{
    removeFromParent();
}

void HallShopLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
}

void HallShopLayer::clickShopButton(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    if (tag == shopType) {
        return;
    }
    if (tag == 3) {
        tag = 5;
    }
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_shop_button;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallShopLayer::clickPackageOk(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_buy_package;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallShopLayer::clickPackageDetail(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_package_detail;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
