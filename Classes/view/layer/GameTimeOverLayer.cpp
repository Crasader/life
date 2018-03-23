//
//  GameTimeOverLayer.cpp
//  life
//
//  Created by ff on 17/7/3.
//
//

#include "GameTimeOverLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

GameTimeOverLayer::GameTimeOverLayer()
{
    
}

GameTimeOverLayer::~GameTimeOverLayer()
{
    
}

bool GameTimeOverLayer::init()
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
    
    auto root = CSLoader::createNode(BATTLE_TIMEOVER_UI);
    action = CSLoader::createTimeline(BATTLE_TIMEOVER_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(GameTimeOverLayer::showLayer, this));

    return true;
}

void GameTimeOverLayer::showLayer()
{
    E2L_FIGHT_TIME_OVER2 info;
    info.eProtocol = e2l_time_over_2;
    info.heroHp = heroHp;
    info.heroMaxHp = maxHeroHp;
    info.opponentHp = opponentHp;
    info.opponentMaxHp = maxOpponentHp;
    ClientLogic::instance()->ProcessUIRequest(&info);
    removeFromParent();
}
