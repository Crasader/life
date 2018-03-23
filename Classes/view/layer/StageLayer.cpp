//
//  StageLayer.cpp
//  life
//
//  Created by ff on 17/7/13.
//
//

#include "StageLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
using namespace cocostudio;


StageLayer::StageLayer():
plotButtonId(0),
plotMissionId(0)
{

}

void StageLayer::onEnter()
{
    Layer::onEnter();
    startStageListener = EventListenerCustom::create(START_STAGE, CC_CALLBACK_1(StageLayer::startStage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startStageListener, -1);
    updateVITListener = EventListenerCustom::create(UPDATE_VIT, CC_CALLBACK_1(StageLayer::updateVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITListener, -1);
    showBuyVITListener = EventListenerCustom::create(SHOW_BUY_VIT, CC_CALLBACK_1(StageLayer::showBuyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBuyVITListener, -1);
    buyVITListener = EventListenerCustom::create(BUY_VIT, CC_CALLBACK_1(StageLayer::buyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyVITListener, -1);
    updateVITBuyCountListener = EventListenerCustom::create(UPDATE_VIT_BUY_COUNT, CC_CALLBACK_1(StageLayer::updateVITBuyCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITBuyCountListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(StageLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    updateVITCDListener = EventListenerCustom::create(UPDATE_VIT_CD, CC_CALLBACK_1(StageLayer::updateVITCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITCDListener, -1);
    scheduleUpdate();

}

StageLayer::~StageLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(startStageListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBuyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITBuyCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITCDListener);
    unscheduleUpdate();

    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}
void StageLayer::update(float dt)
{
    if (vitCD > 0) {
        vitCD -= dt;
        if (vitCD <= 0) {
            VITCDText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            std::string timeStr = TimeUtil::timeFormatToMS(vitCD);
            VITCDText->setString(timeStr);
        }
    }
}

bool StageLayer::init()
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
    
    root = CSLoader::createNode(STAGE_UI);
    rootAction = CSLoader::createTimeline(STAGE_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(StageLayer::clickClose, this));
    startButton = (Button*)root->getChildByName("bg_img")->getChildByName("start_button");
    startButton->addClickEventListener(CC_CALLBACK_0(StageLayer::clickStart, this));
    
    auto VITbg = (ImageView*)root->getChildByName("bg_img")->getChildByName("VIT_img");
    currVITText = (Text*)VITbg->getChildByName("curr_VIT_text");
    maxVITText = (Text*)VITbg->getChildByName("max_VIT_text");
    auto addVITButton = (Button*)VITbg->getChildByName("VIT_button");
    addVITButton->addClickEventListener(CC_CALLBACK_0(StageLayer::clickAddVIT, this));
    
    auto boundsBg = (ImageView*)root->getChildByName("bg_img")->getChildByName("bounds_bg");
    boundGoldText = (Text*)boundsBg->getChildByName("gold")->getChildByName("count_text");
    boundExpText = (Text*)boundsBg->getChildByName("exp")->getChildByName("count_text");
    firstPassBg = (ImageView*)root->getChildByName("bg_img")->getChildByName("first_bg");
    firstGoldText = (Text*)firstPassBg->getChildByName("gold")->getChildByName("count_text");
    firstExpText = (Text*)firstPassBg->getChildByName("exp")->getChildByName("count_text");
    selButton = (Button*)root->getChildByName("bg_img")->getChildByName("sel_button");
    
    VITCDText = (Text*)root->getChildByName("bg_img")->getChildByName("VIT_cd_text");
    VITCDTipText = (Text*)root->getChildByName("bg_img")->getChildByName("VIT_cd_tip_text");
    vitCD = 0;

    
    buyLayer = nullptr;
    return true;
}

void StageLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_STAGE info = *static_cast<L2E_SHOW_STAGE*>(event->getUserData());
    auto titleImg = (ImageView*)root->getChildByName("bg_img")->getChildByName("name_bg_img")->getChildByName("name_img");
    titleImg->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.nameImg.c_str()));
    selButton->loadTextureNormal(GameUtils::format(HERO_HEAD_DIR, info.heroImg.c_str()));
    selButton->loadTexturePressed(GameUtils::format(HERO_HEAD_DIR, info.heroImg.c_str()));
    currVITText->setString(Convert2String(info.currVIT));
    maxVITText->setString(Convert2String(info.maxVIT));
    boundGoldText->setString(Convert2String(info.boundGold));
    boundExpText->setString(Convert2String(info.boundExp));
    if (info.firstGold > 0) {
        firstPassBg->setVisible(true);
        firstGoldText->setString(Convert2String(info.firstGold));
        firstExpText->setString(Convert2String(info.firstExp));
    }else{
        firstPassBg->setVisible(false);
    }

    for (int i = 0; i < 3; i++) {
        auto flagImg = (ImageView*)root->getChildByName("bg_img")->getChildByName(GameUtils::format("star%d_flag", i+1));
        auto starText = (Text*)root->getChildByName("bg_img")->getChildByName(GameUtils::format("star%d_text", i+1));
        flagImg->setVisible(info.star[i] == 1);
        starText->setString(getStarText(info.starType[i], info.thresVal[i]));
    }
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(StageLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}
void StageLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 3) {
        if(info.param[2] != STAGE_LAYER)
        {
            return;
        }
        
        plotButtonId = info.param[3];
        plotMissionId = info.missionId;
        
        plotButtonId = info.param[3];
        plotMissionId = info.missionId;
        auto point = CSLoader::createNode(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
        auto action = CSLoader::createTimeline(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
        point->runAction(action);
        action->play("play", true);
        
        auto pointTip = CSLoader::createNode(GUIDE_POINT_TIP_UI);

        switch (info.param[3]) {
            case 1:
            {
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));

                point->setPosition(startButton->getContentSize()/2);
                point->setTag(100);
                startButton->addChild(point);
            }
                break;
                
            default:
                break;
        }
        
        if (info.param[4] != 0) {
            auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
            text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
            point->addChild(pointTip);
        }

    }
}

std::string StageLayer::getStarText(int type, int thres)
{
    std::string tempStr("");
    switch (type) {
        case 0:
        {
            tempStr = STAGE_VICTORY_STAR;
        }
        break;
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

void StageLayer::clickClose()
{
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(StageLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_back_chapter;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void StageLayer::clickStart()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId == 1) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        
        auto point = startButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();

    }else if (plotButtonId != 0) {
        return;
    }

    E2L_COMMON info;
    info.eProtocol = e2l_start_stage;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void StageLayer::removeOff()
{
    removeFromParent();
}
void StageLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    
    E2L_COMMON infocd;
    infocd.eProtocol = e2l_get_vit_cd;
    ClientLogic::instance()->ProcessUIRequest(&infocd);
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = STAGE_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
}

void StageLayer::startStage(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (info.value != 0) {
        std::string errStr = StringData::shared()->stringFromKey("vit_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(StageLayer::removeOff, this));
    
}

void StageLayer::clickAddVIT()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_show_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void StageLayer::updateVIT(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT info = *static_cast<L2E_UPDATE_VIT*>(event->getUserData());
    currVITText->setString(Convert2String(info.currVIT));
    maxVITText->setString(Convert2String(info.maxVIT));
}

void StageLayer::updateVITCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT_CD info = *static_cast<L2E_UPDATE_VIT_CD*>(event->getUserData());
    if (info.state) {
        vitCD = info.leftSec;
        VITCDTipText->setVisible(true);
        VITCDText->setVisible(true);
    }else{
        vitCD = 0;
        VITCDTipText->setVisible(false);
        VITCDText->setVisible(false);
    }
}

void StageLayer::clickCloseBuy()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(StageLayer::removeBuy, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void StageLayer::removeBuy()
{
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void StageLayer::clickBuy()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void StageLayer::buyVIT(cocos2d::EventCustom *event)
{
    L2E_BUY_VIT info = *static_cast<L2E_BUY_VIT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("buy_vit_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    buyCountText->setString(Convert2String(info.buyCount));
}

void StageLayer::showBuyVIT(cocos2d::EventCustom *event)
{
    L2E_SHOW_BUY_VIT info = *static_cast<L2E_SHOW_BUY_VIT*>(event->getUserData());
    buyLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    //    rewardLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //    rewardLayer->setPosition(GameUtils::winSize/2);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, buyLayer);
    
    addChild(buyLayer,100);
    
    auto root = CSLoader::createNode(BUY_VIT_UI);
    buyAction = CSLoader::createTimeline(BUY_VIT_UI);
    root->runAction(buyAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    buyLayer->addChild(root);
    buyAction->play("in", false);
    
    buyCountText = (Text*)root->getChildByName("bg_img")->getChildByName("count_text");
    priceText = (Text*)root->getChildByName("bg_img")->getChildByName("price_text");
    buyCountText->setString(Convert2String(info.buyCount));
    priceText->setString(Convert2String(info.price));
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(StageLayer::clickCloseBuy, this));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("ok_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(StageLayer::clickBuy, this));
}

void StageLayer::updateVITBuyCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (buyLayer) {
        buyCountText->setString(Convert2String(info.value));
    }
}
