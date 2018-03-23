//
//  PlotVideoLayer.cpp
//  life
//
//  Created by ff on 17/10/14.
//
//

#include "PlotVideoLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;

PlotVideoLayer::PlotVideoLayer():
plotMissionId(0)
{
    videoPlayer = nullptr;
}

PlotVideoLayer::~PlotVideoLayer()
{
    
}

bool PlotVideoLayer::init()
{
    if (!LayerColor::initWithColor(Color4B::BLACK)) {
        return false;
    }
    skipLayer = Layer::create();
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, skipLayer);
    
    auto skipButton = Button::create(GameUtils::format(COMMON_DIR, "changanniu1.png"));
    skipButton->addClickEventListener(CC_CALLBACK_0(PlotVideoLayer::clickSkip, this));
    skipButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    skipButton->setPosition(Vec2(GameUtils::winSize.width-50, GameUtils::winSize.height-50));
    skipLayer->addChild(skipButton);
    addChild(skipLayer, 100);
    
    return true;
}

void PlotVideoLayer::clickSkip()
{
    videoPlayer->pause();
    videoPlayer->setVisible(false);
    this->setVisible(false);;
    
    E2L_COMPLETE_PLOT infoPlot;
    infoPlot.eProtocol = e2l_complete_plot;
    infoPlot.missionId = plotMissionId;
    infoPlot.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    
}
void PlotVideoLayer::videoPlayOverCallback()
{
    videoPlayer->pause();
    videoPlayer->setVisible(false);
    
    this->setVisible(false);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(skipLayer);
    
    
    E2L_COMPLETE_PLOT infoPlot;
    infoPlot.eProtocol = e2l_complete_plot;
    infoPlot.missionId = plotMissionId;
    infoPlot.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    
}
void PlotVideoLayer::showVideo(int videoId, int missionId)
{
    Size size = Director::getInstance()->getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setContentSize(Size(size.width , size.height));
    this->addChild(videoPlayer);
    if (videoPlayer)
    {
        videoPlayer->setFileName(GameUtils::format(VIDEO_DIR, videoId));
        videoPlayer->play();
    }
    videoPlayer->addEventListener(CC_CALLBACK_2(PlotVideoLayer::videoEventCallback, this));
#endif
    plotMissionId = missionId;
}

void PlotVideoLayer::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
    switch (eventType) {
        case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
            videoPlayOverCallback();
            break;
        default:
            break;
    }
}
