//
//  BattleChapterLayer.cpp
//  life
//
//  Created by ff on 17/7/12.
//
//

#include "BattleChapterLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
using namespace cocostudio;

BattleChapterLayer::BattleChapterLayer():
focusChapter(0),
plotButtonId(0),
plotMissionId(0)
{
//    showChapterNameListener = EventListenerCustom::create(SHOW_CHAPTER_NAME, CC_CALLBACK_1(BattleChapterLayer::showChapterName, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showChapterNameListener, -1);
//    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(BattleChapterLayer::startPlot, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -2);

}

BattleChapterLayer::~BattleChapterLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showChapterNameListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool BattleChapterLayer::init()
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
    
    auto root = CSLoader::createNode(BATTLE_CHAPTERS_UI);
    rootAction = CSLoader::createTimeline(BATTLE_CHAPTERS_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    mainScroll = (ScrollView*)root->getChildByName("bg_img")->getChildByName("chapter_scroll");
    for (int i = 0; i < 8; i++) {
        auto chapterNode = mainScroll->getChildByTag(i+1);
        chapterAction[i] = CSLoader::createTimeline(CHAPTER_GATE_UI);
        chapterNode->runAction(chapterAction[i]);
        chapterAction[i]->play("play1", false);
        
        auto nameButton = (Button*)mainScroll->getChildByTag(i+1)->getChildByName("name_button");
        nameButton->addClickEventListener(CC_CALLBACK_1(BattleChapterLayer::clickChapterName, this));
        auto enterButton = (Button*)mainScroll->getChildByTag(i+1)->getChildByName("bg_img")->getChildByName("enter_button");
        enterButton->addClickEventListener(CC_CALLBACK_1(BattleChapterLayer::clickEnter, this));
    }
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("Button_1_0");
    closeButton->addClickEventListener(CC_CALLBACK_0(BattleChapterLayer::clickClose, this));
    return true;
}

void BattleChapterLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_BATTLE_CHAPTERS info = *static_cast<L2E_SHOW_BATTLE_CHAPTERS*>(event->getUserData());
    for (int i = 0; i < 8; i++) {
        if (i+1 > info.maxOpenId) {
            chapterAction[i]->play("close", true);
//            mainScroll->getChildByTag(i+1)->setVisible(false);
        }else{
            chapterAction[i]->play("play1", false);
//            mainScroll->getChildByTag(i+1)->setVisible(true);
        }
    }
    float scrollWidth = mainScroll->getInnerContainer()->getContentSize().width;
    scrollWidth -= GameUtils::winSize.width;
    float focusX = mainScroll->getChildByTag(info.focusId)->getPositionX();
    focusX -= GameUtils::winSize.width/2;
    focusX = MAX(0, focusX);
    focusX = MIN(focusX, scrollWidth);
    mainScroll->jumpToPercentHorizontal(focusX*100.0/scrollWidth);
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(BattleChapterLayer::endIn, this));
    focusChapter = info.focusId;
    auto nameImg = (ImageView*)mainScroll->getChildByTag(focusChapter)->getChildByName("bg_img")->getChildByName("Image_2");
    nameImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.imgFile.c_str()));
    chapterAction[focusChapter-1]->play("play2", false);
    
//    E2L_TRIGGER_OPEN_LAYER infoTrigger;
//    infoTrigger.eProtocol = e2l_trigger_open_layer;
//    infoTrigger.index = BATTLE_CHAPTER_LAYER;
//    ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
    
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}
void BattleChapterLayer::onEnter()
{
    Layer::onEnter();
    showChapterNameListener = EventListenerCustom::create(SHOW_CHAPTER_NAME, CC_CALLBACK_1(BattleChapterLayer::showChapterName, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showChapterNameListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(BattleChapterLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);

//    E2L_TRIGGER_OPEN_LAYER info;
//    info.eProtocol = e2l_trigger_open_layer;
//    info.index = BATTLE_CHAPTER_LAYER;
//    ClientLogic::instance()->ProcessUIRequest(&info);
}

void BattleChapterLayer::endIn()
{
    rootAction->play("play", true);
    HallScene::autoPopLayerId = NONE_LAYER;

    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = BATTLE_CHAPTER_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);

}
void BattleChapterLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 3) {
        if(info.param[2] != BATTLE_CHAPTER_LAYER)
        {
            return;
        }
        
        plotButtonId = info.param[3];
        plotMissionId = info.missionId;
        
        auto point = CSLoader::createNode(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
        auto action = CSLoader::createTimeline(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
        point->runAction(action);
        action->play("play", true);
        auto pointTip = CSLoader::createNode(GUIDE_POINT_TIP_UI);

//        switch (info.param[3]) {
//            case 1:
//            {
//                auto chapterNode = (ImageView*)mainScroll->getChildByTag(focusChapter)->getChildByName("bg_img");
//                pointTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//                pointTip->setPosition(Vec2(0, 120));
//
//                point->setPosition(chapterNode->getContentSize()/2);
//                point->setTag(100);
//                chapterNode->addChild(point);
//            }
//                break;
//                
//            default:
//                break;
//        }
        if (info.param[3]>=1 && info.param[3]<=8) {
            auto chapterNode = (ImageView*)mainScroll->getChildByTag(info.param[3])->getChildByName("bg_img");
            pointTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            pointTip->setPosition(Vec2(0, 120));
            
            point->setPosition(chapterNode->getContentSize()/2);
            point->setTag(100);
            chapterNode->addChild(point);
        }
        
        if (info.param[4] != 0) {
            auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
            text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
            point->addChild(pointTip);
        }


    }
}
void BattleChapterLayer::clickChapterName(cocos2d::Ref *gateNode)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    int focus = ((Button*)gateNode)->getParent()->getTag();
    if (focus == focusChapter) {
        return;
    }
    E2L_CLICK_CHAPTER_NAME info;
    info.eProtocol = e2l_click_chapter_name;
    info.tag = focus;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void BattleChapterLayer::showChapterName(cocos2d::EventCustom *event)
{
    L2E_SHOW_CHAPTER_NAME info = *static_cast<L2E_SHOW_CHAPTER_NAME*>(event->getUserData());
    if (focusChapter != 0) {
        chapterAction[focusChapter-1]->stop();
        chapterAction[focusChapter-1]->play("play3", false);
    }
    focusChapter = info.chapterId;
    auto nameImg = (ImageView*)mainScroll->getChildByTag(focusChapter)->getChildByName("bg_img")->getChildByName("Image_2");
    nameImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.imgFile.c_str()));
    chapterAction[focusChapter-1]->play("play2", false);
}

void BattleChapterLayer::clickEnter(cocos2d::Ref *gateNode)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId >= 1 && plotButtonId <= 8) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        
        auto point = ((Button*)gateNode)->getParent()->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();

    }else if (plotButtonId != 0) {
        return;
    }

    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(BattleChapterLayer::removeOff, this));
    int focus = ((Button*)gateNode)->getParent()->getParent()->getTag();
    E2L_SHOW_CHAPTER info;
    info.eProtocol = e2l_show_chapter;
    info.tag = focus;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void BattleChapterLayer::removeOff()
{
    removeFromParent();
}

void BattleChapterLayer::clickClose()
{
    if (plotButtonId != 0) {
        
        return;
    }
    plotButtonId = 0;
    
    rootAction->stop();
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(BattleChapterLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);

}
