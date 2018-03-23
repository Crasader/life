//
//  TipsNode.cpp
//  life
//
//  Created by ff on 17/7/25.
//
//

#include "TipsNode.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;

bool TipsNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode(TIPS_UI);
    auto action = CSLoader::createTimeline(TIPS_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2::ZERO);
    addChild(root);
    action->play("play", false);
    action->setAnimationEndCallFunc("play", CC_CALLBACK_0(TipsNode::removeOff, this));
    
    tipText = (Text*)root->getChildByName("Image_1")->getChildByName("Text_1");
    return true;
}

void TipsNode::setupText(std::string val)
{
    tipText->setString(val);
}
void TipsNode::removeOff()
{
    removeFromParent();
}
