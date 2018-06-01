//
//  UnlockJobTipLayer.cpp
//  life-mobile
//
//  Created by ff on 2018/5/31.
//

#include "UnlockJobTipLayer.h"
#include "GameDefine.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;

bool UnlockJobTipLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 127))) {
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
    
    auto root = CSLoader::createNode(UNLOCK_JOB_UI);
    auto rootAction = CSLoader::createTimeline(UNLOCK_JOB_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    rootAction->play("play", false);
    rootAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(UnlockJobTipLayer::removeOff, this));
    
    return true;
}

void UnlockJobTipLayer::removeOff()
{
    removeFromParent();
}
