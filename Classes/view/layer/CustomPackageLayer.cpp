//
//  CustomPackageLayer.cpp
//  life-mobile
//
//  Created by ff on 2018/4/13.
//

#include "CustomPackageLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

CustomPackageLayer::CustomPackageLayer()
{
    
}

CustomPackageLayer::~CustomPackageLayer()
{
    
}

bool CustomPackageLayer::init()
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

    return true;
}

void CustomPackageLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_CUSTOM_PACKAGE info = *static_cast<L2E_SHOW_CUSTOM_PACKAGE*>(event->getUserData());
    
    auto root = CSLoader::createNode(GameUtils::format("csb/shop/%s", info.showRes.c_str()));
    rootAction = CSLoader::createTimeline(GameUtils::format("csb/shop/%s", info.showRes.c_str()));
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(CustomPackageLayer::clickClose, this));
    
    auto okButton = (Button*)rootBg->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(CustomPackageLayer::clickOk, this));
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(CustomPackageLayer::endIn, this));
}

void CustomPackageLayer::endIn()
{
    rootAction->play("play", true);
}

void CustomPackageLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(CustomPackageLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void CustomPackageLayer::removeOff()
{
    removeFromParent();
}

void CustomPackageLayer::clickOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_buy_package;
    info.value = packageId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    clickClose();
}
