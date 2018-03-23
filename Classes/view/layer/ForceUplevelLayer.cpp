//
//  ForceUplevelLayer.cpp
//  life
//
//  Created by ff on 17/12/4.
//
//

#include "ForceUplevelLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool ForceUplevelLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    auto root = CSLoader::createNode(FORCE_UPLEVEL_UI);
    rootAction = CSLoader::createTimeline(FORCE_UPLEVEL_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    totalForceText = (Text*)root->getChildByName("bg_img")->getChildByName("total_force");
    addForceText = (Text*)root->getChildByName("add_text")->getChildByName("count_text");
    return true;
}

void ForceUplevelLayer::showLayer(cocos2d::EventCustom *event)
{
    L2E_UPDATE_FORCE info = *static_cast<L2E_UPDATE_FORCE*>(event->getUserData());
    
    totalForceText->setString(Convert2String(info.forceVal));
    addForceText->setString(Convert2String(info.upVal));
    rootAction->play("play", false);
    rootAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(ForceUplevelLayer::removeFromParent, this));
}
