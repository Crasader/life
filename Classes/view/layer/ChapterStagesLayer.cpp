//
//  ChapterStagesLayer.cpp
//  life
//
//  Created by ff on 17/7/13.
//
//

#include "ChapterStagesLayer.h"
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


ChapterStagesLayer::ChapterStagesLayer():
plotButtonId(0),
plotMissionId(0)
{

}

void ChapterStagesLayer::onEnter()
{
    Layer::onEnter();
    openStarBoxListener = EventListenerCustom::create(OPEN_STAR_BOX, CC_CALLBACK_1(ChapterStagesLayer::openStarBox, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(openStarBoxListener, -1);
    showStarBoundsListener = EventListenerCustom::create(SHOW_STAR_BOUNDS, CC_CALLBACK_1(ChapterStagesLayer::showStarBounds, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showStarBoundsListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(ChapterStagesLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
}

ChapterStagesLayer::~ChapterStagesLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(openStarBoxListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showStarBoundsListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool ChapterStagesLayer::init()
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
    
    root = CSLoader::createNode(CHAPTER_UI);
    rootAction = CSLoader::createTimeline(CHAPTER_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);

    mainScroll = (ListView*)root->getChildByName("bg_img")->getChildByName("stage_scroll");
//    mainScroll->setScrollBarEnabled(true);
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ChapterStagesLayer::clickClose, this));
    for (int i = 1; i <= 3; i++) {
        boxButton[i-1] = (Button*)root->getChildByName("bg_img")->getChildByName("box_text")->getChildByTag(i);
        boxButton[i-1]->addClickEventListener(CC_CALLBACK_1(ChapterStagesLayer::clickBox, this));
    }
    return true;
}

void ChapterStagesLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_CHAPTER_STAGES info = *static_cast<L2E_SHOW_CHAPTER_STAGES*>(event->getUserData());
    auto titleImg = (ImageView*)root->getChildByName("bg_img")->getChildByName("chapter_bg_img")->getChildByName("name_img");
    titleImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.chapterImg.c_str()));
    auto starCountText = (Text*)root->getChildByName("bg_img")->getChildByName("box_text")->getChildByName("star_count_text");
    starCountText->setString(Convert2String(info.totalStarCount));
    for (int i = 0; i < 3; i++) {
        switch (info.boxState[i]) {
            case -1:
            {
                boxButton[i]->setVisible(false);
            }
            break;
            case 0:
            {
//                boxButton[i]->setEnabled(false);
                boxButton[i]->setBright(true);
                boxButton[i]->getChildByName("enable_particle")->setVisible(false);
            }
            break;
            case 1:
            {
//                boxButton[i]->setEnabled(true);
                boxButton[i]->setBright(true);
                boxButton[i]->getChildByName("enable_particle")->setVisible(true);
            }
            break;
            case 2:
            {
//                boxButton[i]->setEnabled(false);
                boxButton[i]->setBright(false);
                boxButton[i]->getChildByName("enable_particle")->setVisible(false);
            }
            break;
           
            default:
            break;
        }
    }
    
//    Vec2 originPos = mainScroll->getChildByName("FileNode_1")->getPosition();
    int stageCount = info.stages.size();
    mainScroll->removeAllChildren();
    for (int i = 0; i < stageCount; i++) {
        auto stageNode = CSLoader::createNode(STAGE_NODE_UI);
        
        auto bgImg = (ImageView*)stageNode->getChildByName("bg_img");
        auto enterButton = (Button*)bgImg->getChildByName("start_button");
        enterButton->setSwallowTouches(false);
        enterButton->setTag(info.stages[i].stageId);
        enterButton->addClickEventListener(CC_CALLBACK_1(ChapterStagesLayer::clickEnter, this));
//        auto nameImg = (ImageView*)bgImg->getChildByName("name_img");
//        nameImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.stages[i].nameImg.c_str()));
//        enterButton->loadTextureNormal(GameUtils::format(CHAPTER_NAME_ICON, info.stages[i].nameImg.c_str()));
        auto titleImg = (ImageView*)bgImg->getChildByName("title_img");
        titleImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.stages[i].nameImg.c_str()));
        auto iconImg = (ImageView*)bgImg->getChildByName("icon_img");
        iconImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.stages[i].iconImg.c_str()));
        int starCount = 0;
        for (int j = 0; j < 3; j++) {
            auto starSprite = (Sprite*)bgImg->getChildByTag(j+1);
            if (info.stages[i].star[j] == 1) {
                starCount++;
            }
            starSprite->setVisible(info.stages[i].star[j] == 1);
        }
        auto flagImg = (ImageView*)bgImg->getChildByName("flag_img");
        flagImg->setVisible(starCount == 0);
        
//        stageNode->setPosition(originPos+Vec2(100*(stageCount-1-i), 0));
        bgImg->retain();
        bgImg->removeFromParent();
        
        mainScroll->addChild(bgImg);
//        bgImg->setLocalZOrder(i);
        bgImg->release();
    }
    mainScroll->jumpToRight();
//    float scrollWidth = 750;
//    scrollWidth = MAX(originPos.x+100*stageCount, 750);
//    mainScroll->getInnerContainer()->setContentSize(Size(scrollWidth, 173));
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(ChapterStagesLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void ChapterStagesLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = CHAPTER_STAGE_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ChapterStagesLayer::clickClose()
{
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ChapterStagesLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_back_battle_chapters;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ChapterStagesLayer::clickBox(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    int tag = ((Button*)pSender)->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_star_box;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ChapterStagesLayer::clickEnter(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int stageId = ((Button*)pSender)->getTag();
    if (plotButtonId >= 1 && plotButtonId<=10) {
        if (mainScroll->getIndex((ImageView*)((Button*)pSender)->getParent()) != (plotButtonId-1)) {
            return;
        }
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        
        auto point = ((Button*)pSender)->getParent()->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();

    }else if (plotButtonId != 0) {
        return;
    }

    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ChapterStagesLayer::removeOff, this));
    
    E2L_SHOW_STAGE info;
    info.eProtocol = e2l_show_stage;
    info.stageId = stageId;
    ClientLogic::instance()->ProcessUIRequest(&info);

}

void ChapterStagesLayer::removeOff()
{
    removeFromParent();
}

void ChapterStagesLayer::openStarBox(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    int boxIndex = info.value;
//    boxButton[boxIndex-1]->setEnabled(false);
    boxButton[boxIndex-1]->setBright(false);
    boxButton[boxIndex-1]->getChildByName("enable_particle")->setVisible(false);
}

void ChapterStagesLayer::showStarBounds(cocos2d::EventCustom *event)
{
    L2E_SHOW_STAR_BOUNDS info = *static_cast<L2E_SHOW_STAR_BOUNDS*>(event->getUserData());
    starBoundsLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, starBoundsLayer);
    
    addChild(starBoundsLayer,100);
    
    auto root = CSLoader::createNode(STAR_BOUNDS_UI);
    starBoundsAction = CSLoader::createTimeline(STAR_BOUNDS_UI);
    root->runAction(starBoundsAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    starBoundsLayer->addChild(root);
    starBoundsAction->play("in", false);

    auto bound1Bg = (ImageView*)root->getChildByName("bg_img")->getChildByName("gold_img");
    auto bound2Bg = (ImageView*)root->getChildByName("bg_img")->getChildByName("stone_img");
    auto bound3Bg = (ImageView*)root->getChildByName("bg_img")->getChildByName("vit_img");
    auto bound1CountText = (Text*)bound1Bg->getChildByName("count_text");
    auto bound2CountText = (Text*)bound2Bg->getChildByName("count_text");
    auto bound3CountText = (Text*)bound3Bg->getChildByName("count_text");
    
    bound1Bg->loadTexture(info.icon[0].c_str());
    bound1CountText->setString(Convert2String(info.count[0]));
    if (info.count[1] > 0) {
        bound2Bg->loadTexture(info.icon[1].c_str());
        bound2CountText->setString(Convert2String(info.count[1]));
    }else{
        bound2Bg->setVisible(false);
    }
    if (info.count[2] > 0) {
        bound3Bg->loadTexture(info.icon[2].c_str());
        bound3CountText->setString(Convert2String(info.count[2]));
    }else{
        bound3Bg->setVisible(false);
    }
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ChapterStagesLayer::clickCloseBounds, this));
}

void ChapterStagesLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 3) {
        if(info.param[2] != CHAPTER_STAGE_LAYER)
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
//                auto stageBg = (ImageView*)(mainScroll->getItem(0));
//                pointTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//                pointTip->setPosition(Vec2(30, -150));
//
//                point->setPosition(stageBg->getContentSize()/2);
//                point->setTag(100);
//                stageBg->addChild(point);
//            }
//                break;
//                
//            default:
//                break;
//        }
        if (info.param[3]>=1 && info.param[3]<=10) {
//            auto stageBg = (ImageView*)(mainScroll->getItem(mainScroll->getItems().size() - info.param[3]));
            auto stageBg = (ImageView*)(mainScroll->getItem(info.param[3]-1));
            if (!stageBg) {
                return;
            }
            pointTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            if (info.param[3]==1) {
                pointTip->setPosition(Vec2(30, 30));
            }else{
                pointTip->setPosition(Vec2(-50, 30));
            }
            
            
            point->setPosition(stageBg->getContentSize()/2);
            point->setTag(100);
            stageBg->addChild(point);
        }
        
        if (info.param[4] != 0) {
            auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
            text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
            point->addChild(pointTip);
        }

    }
}

void ChapterStagesLayer::clickCloseBounds()
{
    starBoundsAction->play("out", false);
    starBoundsAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ChapterStagesLayer::removeOffBounds, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void ChapterStagesLayer::removeOffBounds()
{
    removeChild(starBoundsLayer);
}
