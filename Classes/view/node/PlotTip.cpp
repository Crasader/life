//
//  PlotTip.cpp
//  life
//
//  Created by ff on 17/10/19.
//
//

#include "PlotTip.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;

bool PlotTip::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode(PLOT_TIPS_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2::ZERO);
    addChild(root);
    
    tipText = (Text*)root->getChildByName("bg_img")->getChildByName("count_text");
    return true;
}

void PlotTip::setupText(std::string val)
{
    tipText->setString(val);
}

void PlotTip::removeOff()
{
    removeFromParent();
}
