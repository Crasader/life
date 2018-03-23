//
//  EffectNode.cpp
//  life
//
//  Created by ff on 17/12/4.
//
//

#include "EffectNode.h"
#include "GameDefine.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

bool EffectNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

bool EffectNode::initWithRes(std::string res)
{
    auto root = CSLoader::createNode(res);
    auto action = CSLoader::createTimeline(res);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2::ZERO);
    addChild(root);
    action->play("play", false);
    action->setAnimationEndCallFunc("play", CC_CALLBACK_0(EffectNode::removeOff, this));
    return true;
}

bool EffectNode::initUplevel(int level)
{
    auto root = CSLoader::createNode(LEVELUP_TIP_UI);
    auto action = CSLoader::createTimeline(LEVELUP_TIP_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto levelText = (TextAtlas*)root->getChildByName("bg_img")->getChildByName("level_img")->getChildByName("level_atlas");
    levelText->setString(Convert2String(level));
    action->play("play", false);
    action->setAnimationEndCallFunc("play", CC_CALLBACK_0(EffectNode::removeOff, this));
    return true;
}

void EffectNode::removeOff()
{
    removeFromParent();
}

