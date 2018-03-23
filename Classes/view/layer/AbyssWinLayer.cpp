//
//  AbyssWinLayer.cpp
//  life
//
//  Created by ff on 17/9/8.
//
//

#include "AbyssWinLayer.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

AbyssWinLayer::AbyssWinLayer()
{
    toHall = 0;
}

AbyssWinLayer::~AbyssWinLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(starListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
}
void AbyssWinLayer::onEnter()
{
    Layer::onEnter();
    starListener = EventListenerCustom::create(STAGE_STAR, CC_CALLBACK_1(AbyssWinLayer::showLayer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(starListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(AbyssWinLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, "win.mp3").c_str());
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, "win.mp3").c_str(), true);

}
bool AbyssWinLayer::init()
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
    
    auto root = CSLoader::createNode(ABYSS_WIN_UI);
    action = CSLoader::createTimeline(ABYSS_WIN_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(AbyssWinLayer::getStar, this));
    
    
    auto boundsBg = (ImageView*)root->getChildByName("bound_bg")->getChildByName("bounds_bg");
    firstPassBg = (ImageView*)root->getChildByName("bound_bg")->getChildByName("first_bg");
    boundGoldText= (Text*)boundsBg->getChildByName("gold")->getChildByName("count_text");
    boundExpText = (Text*)boundsBg->getChildByName("exp")->getChildByName("count_text");
    firstGoldText = (Text*)firstPassBg->getChildByName("gold")->getChildByName("count_text");
    firstExpText = (Text*)firstPassBg->getChildByName("exp")->getChildByName("count_text");
    itemIcon = (ImageView*)boundsBg->getChildByName("gold_0");
    itemCountText = (Text*)itemIcon->getChildByName("count_text");
    firstPassBg->setVisible(false);
    return true;
}

void AbyssWinLayer::getStar()
{
    E2L_FIGHT_STAR info;
    info.eProtocol = e2l_fight_star;
    info.heroHp = 0;
    info.heroMaxHp = 0;
    info.leftSecond = 0;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void AbyssWinLayer::showLayer(EventCustom *event)
{
    L2E_FIGHT_STAR info = *static_cast<L2E_FIGHT_STAR*>(event->getUserData());

    action->play("play1", false);
    action->setAnimationEndCallFunc("play1", CC_CALLBACK_0(AbyssWinLayer::showStar, this));
    
    boundGoldText->setString(Convert2String(info.boundGold));
    boundExpText->setString(Convert2String(info.boundExp));
    itemIcon->loadTexture(GameUtils::format(COMMON_DIR, info.boundIcon.c_str()));
    itemCountText->setString(Convert2String(info.boundIconCount));
    

    firstPassBg->setVisible(false);

}

void AbyssWinLayer::showStar()
{
    openTouch();
}

void AbyssWinLayer::openTouch()
{
    getEventDispatcher()->removeAllEventListeners();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(AbyssWinLayer::onTouchesBegan, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
bool AbyssWinLayer::onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_win;
    info.value = toHall;
    ClientLogic::instance()->ProcessUIRequest(&info);
    return true;
}

void AbyssWinLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 2) {
        toHall = 1;
        
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = info.missionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    }
}
