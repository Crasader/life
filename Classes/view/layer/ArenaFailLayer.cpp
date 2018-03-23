//
//  ArenaFailLayer.cpp
//  life
//
//  Created by ff on 17/9/26.
//
//

#include "ArenaFailLayer.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

ArenaFailLayer::ArenaFailLayer()
{
    
}

ArenaFailLayer::~ArenaFailLayer()
{
    
}

void ArenaFailLayer::onEnter()
{
    Layer::onEnter();
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, "lose.mp3").c_str());
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, "lose.mp3").c_str(), true);

}

bool ArenaFailLayer::init()
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
    
    auto root = CSLoader::createNode(ARENA_LOSE_UI);
    action = CSLoader::createTimeline(ARENA_LOSE_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(ArenaFailLayer::openTouch, this));
    
    auto bgImg = (ImageView*)root->getChildByName("bg_img");
    goldText = (Text*)bgImg->getChildByName("gold_text");
    scoreText = (Text*)bgImg->getChildByName("score_text");
    deltaScoreText = (Text*)bgImg->getChildByName("delta_score_text");
    divisionText = (Text*)bgImg->getChildByName("division_text");
    divisionIcon1 = (ImageView*)divisionText->getChildByName("division_icon1");
    divisionIcon2 = (ImageView*)divisionText->getChildByName("division_icon2");
    return true;
}

void ArenaFailLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_ARENA_RESULT info = *static_cast<L2E_ARENA_RESULT*>(event->getUserData());
    goldText->setString(Convert2String(info.gold));
    scoreText->setString(Convert2String(info.totalScore));
    deltaScoreText->setString(Convert2String(info.deltaScore));
    if (info.changeDivision == 1) {
        divisionText->setVisible(true);
        divisionIcon1->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon1.c_str()));
        divisionIcon2->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon2.c_str()));
    }else{
        divisionText->setVisible(false);
    }
}

void ArenaFailLayer::openTouch()
{
    getEventDispatcher()->removeAllEventListeners();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ArenaFailLayer::onTouchesBegan, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    action->play("play2", true);
}

bool ArenaFailLayer::onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    E2L_COMMON info;
    info.eProtocol = e2l_click_fail;
    ClientLogic::instance()->ProcessUIRequest(&info);
    return true;
}
