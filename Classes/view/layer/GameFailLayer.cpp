//
//  GameFailLayer.cpp
//  life
//
//  Created by ff on 17/7/3.
//
//

#include "GameFailLayer.h"
#include "GameDefine.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

GameFailLayer::GameFailLayer()
{
    
}

GameFailLayer::~GameFailLayer()
{
    
}
void GameFailLayer::onEnter()
{
    Layer::onEnter();
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, "lose.mp3").c_str());
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, "lose.mp3").c_str(), true);

}

bool GameFailLayer::init()
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
    
    auto root = CSLoader::createNode(BATTLE_FAIL_UI);
    action = CSLoader::createTimeline(BATTLE_FAIL_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(GameFailLayer::showLayer, this));

    return true;
}

void GameFailLayer::showLayer()
{
    action->play("play", true);
    getEventDispatcher()->removeAllEventListeners();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameFailLayer::onTouchesBegan, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool GameFailLayer::onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    E2L_COMMON info;
    info.eProtocol = e2l_click_fail;
    ClientLogic::instance()->ProcessUIRequest(&info);
    return true;
}
