//
//  PackageDetailLayer.cpp
//  life
//
//  Created by ff on 17/11/9.
//
//

#include "PackageDetailLayer.h"
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

PackageDetailLayer::PackageDetailLayer()
{
    
}

PackageDetailLayer::~PackageDetailLayer()
{
    
}

bool PackageDetailLayer::init()
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
    
    auto root = CSLoader::createNode(SHOP_PACKAGE_DETAIL_UI);
    rootAction = CSLoader::createTimeline(SHOP_PACKAGE_DETAIL_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(PackageDetailLayer::clickClose, this));
    
    auto okButton = (Button*)rootBg->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(PackageDetailLayer::clickOk, this));
    
    itemList = (ListView*)rootBg->getChildByName("item_list");
    priceText = (Text*)rootBg->getChildByName("price_bg")->getChildByName("price_text");
    titleImg = (ImageView*)rootBg->getChildByName("title_bg")->getChildByName("title_img");
    packageImg = (ImageView*)rootBg->getChildByName("package_img");
    return true;
}

void PackageDetailLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_PACKAGE_DETAIL info = *static_cast<L2E_SHOW_PACKAGE_DETAIL*>(event->getUserData());
    packageId = info.packageId;
    priceText->setString(Convert2String(info.price));
    titleImg->setUnifySizeEnabled(true);
    titleImg->loadTexture(GameUtils::format(SHOP_DIR, info.packageTitle.c_str()));
    packageImg->setUnifySizeEnabled(true);
    packageImg->loadTexture(GameUtils::format(COMMON_DIR, info.packageImg.c_str()));
    

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
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(PackageDetailLayer::endIn, this));
}

void PackageDetailLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(PackageDetailLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void PackageDetailLayer::clickOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_buy_package;
    info.value = packageId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    clickClose();
}

void PackageDetailLayer::removeOff()
{
    removeFromParent();
}

void PackageDetailLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
}
