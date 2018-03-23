//
//  ReviveLayer.cpp
//  life
//
//  Created by ff on 17/12/15.
//
//

#include "ReviveLayer.h"
#include "GameDefine.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

ReviveLayer::ReviveLayer()
{
    
}

ReviveLayer::~ReviveLayer()
{
    
}

bool ReviveLayer::init()
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

    auto root = CSLoader::createNode(BATTLE_REVIVE_UI);
    action = CSLoader::createTimeline(BATTLE_REVIVE_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ReviveLayer::clickClose, this));
    auto okButton = (Button*)root->getChildByName("bg_img")->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(ReviveLayer::clickOk, this));
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(ReviveLayer::endIn, this));
    return true;
}
void ReviveLayer::endIn()
{
    action->play("play", true);
}
void ReviveLayer::clickClose()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_not_revive;
    ClientLogic::instance()->ProcessUIRequest(&info);
    action->play("out", false);
    action->setAnimationEndCallFunc("out", CC_CALLBACK_0(ReviveLayer::removeOff, this));
}

void ReviveLayer::clickOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_revive;
    ClientLogic::instance()->ProcessUIRequest(&info);
    action->play("out", false);
    action->setAnimationEndCallFunc("out", CC_CALLBACK_0(ReviveLayer::removeOff, this));
}

void ReviveLayer::removeOff()
{
    removeFromParent();
}
