//
//  GameWinLayer.cpp
//  life
//
//  Created by ff on 17/6/28.
//
//

#include "GameWinLayer.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

GameWinLayer::GameWinLayer()
{
    toHall = 0;
}

GameWinLayer::~GameWinLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(starListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
}
void GameWinLayer::onEnter()
{
    Layer::onEnter();
    starListener = EventListenerCustom::create(STAGE_STAR, CC_CALLBACK_1(GameWinLayer::showLayer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(starListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(GameWinLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, "win.mp3").c_str());
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, "win.mp3").c_str(), true);

    
}
bool GameWinLayer::init()
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
    
    auto root = CSLoader::createNode(BATTLE_WIN_UI);
    action = CSLoader::createTimeline(BATTLE_WIN_UI);
    root->runAction(action);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    action->play("in", false);
    action->setAnimationEndCallFunc("in", CC_CALLBACK_0(GameWinLayer::getStar, this));
    
    star1Descript = (Text*)root->getChildByName("Text_3");
    star2Descript = (Text*)root->getChildByName("Text_3_0");
    star3Descript = (Text*)root->getChildByName("Text_3_0_0");
    star1Img = (ImageView*)root->getChildByName("Image_9");
    star2Img = (ImageView*)root->getChildByName("Image_9_0");
    star3Img = (ImageView*)root->getChildByName("Image_9_0_0");
    
    auto boundsBg = (ImageView*)root->getChildByName("bound_bg")->getChildByName("bounds_bg");
    firstPassBg = (ImageView*)root->getChildByName("bound_bg")->getChildByName("first_bg");
    boundGoldText= (Text*)boundsBg->getChildByName("gold")->getChildByName("count_text");
    boundExpText = (Text*)boundsBg->getChildByName("exp")->getChildByName("count_text");
    firstGoldText = (Text*)firstPassBg->getChildByName("gold")->getChildByName("count_text");
    firstExpText = (Text*)firstPassBg->getChildByName("exp")->getChildByName("count_text");
    return true;
}

void GameWinLayer::getStar()
{
    E2L_FIGHT_STAR info;
    info.eProtocol = e2l_fight_star;
    info.heroHp = heroHp;
    info.heroMaxHp = heroMaxHp;
    info.leftSecond = leftSecond;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    E2L_COMMON infoTrigger;
    infoTrigger.eProtocol = e2l_trigger_victory;
    ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
//    action->play("play", true);
}

void GameWinLayer::showLayer(EventCustom *event)
{
    L2E_FIGHT_STAR info = *static_cast<L2E_FIGHT_STAR*>(event->getUserData());
    star[0] = true;
    star[1] = info.star[1];
    star[2] = info.star[2];
    
    star1Descript->setString(STAGE_VICTORY_STAR);
    star2Descript->setString(getStarText(info.starType[1], info.thresVal[1]));
    star3Descript->setString(getStarText(info.starType[2], info.thresVal[2]));
    action->play("play1", false);
    action->setAnimationEndCallFunc("play1", CC_CALLBACK_0(GameWinLayer::showStar, this));
    
    boundGoldText->setString(Convert2String(info.boundGold));
    boundExpText->setString(Convert2String(info.boundExp));
    
    if (info.firstGold > 0) {
        firstPassBg->setVisible(true);
        firstGoldText->setString(Convert2String(info.firstGold));
        firstExpText->setString(Convert2String(info.firstExp));
    }else{
        firstPassBg->setVisible(false);
    }
}

std::string GameWinLayer::getStarText(int type, int thres)
{
    std::string tempStr("");
    switch (type) {
        case 1:
        {
            tempStr = GameUtils::format(STAGE_TIME_STAR, thres);
        }
            break;
        case 2:
        {
            tempStr = GameUtils::format(STAGE_HP_STAR, thres);
        }
            break;
        case 3:
        {
            tempStr = STAGE_BOTTLE_STAR;
        }
            break;
            
        default:
            break;
    }
    return tempStr;
}

void GameWinLayer::showStar()
{
    action->play("play2", true);
    auto star1 = CSLoader::createNode(BATTLE_WIN_STAR_UI);
    auto star1Action = CSLoader::createTimeline(BATTLE_WIN_STAR_UI);
    star1->setPosition(star1Img->getContentSize()/2);
    star1->runAction(star1Action);
    star1Img->addChild(star1);
    star1Action->play("paly", false);
    star1Action->setAnimationEndCallFunc("paly", CC_CALLBACK_0(GameWinLayer::openTouch, this));
    
    if (star[1]) {
        auto star2 = CSLoader::createNode(BATTLE_WIN_STAR_UI);
        auto star2Action = CSLoader::createTimeline(BATTLE_WIN_STAR_UI);
        star2->setPosition(star2Img->getContentSize()/2);
        star2->runAction(star2Action);
        star2Img->addChild(star2);
        star2Action->play("paly", false);
    }
    if (star[2]) {
        auto star3 = CSLoader::createNode(BATTLE_WIN_STAR_UI);
        auto star3Action = CSLoader::createTimeline(BATTLE_WIN_STAR_UI);
        star3->setPosition(star3Img->getContentSize()/2);
        star3->runAction(star3Action);
        star3Img->addChild(star3);
        star3Action->play("paly", false);
    }
}

void GameWinLayer::openTouch()
{
    getEventDispatcher()->removeAllEventListeners();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameWinLayer::onTouchesBegan, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
bool GameWinLayer::onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_win;
    info.value = toHall;
    ClientLogic::instance()->ProcessUIRequest(&info);
    return true;
}
//void GameWinLayer::back2Chapter()
//{
//    E2L_COMMON info;
//    info.eProtocol = e2l_click_win;
//    ClientLogic::instance()->ProcessUIRequest(&info);
//}

void GameWinLayer::startPlot(cocos2d::EventCustom *event)
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
