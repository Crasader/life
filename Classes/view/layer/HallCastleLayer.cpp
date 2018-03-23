//
//  HallCastleLayer.cpp
//  life
//
//  Created by ff on 17/11/1.
//
//

#include "HallCastleLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;


HallCastleLayer::HallCastleLayer():
plotButtonId(0),
plotMissionId(0),
showJobId(0),
buyJobId(0)
{
}

void HallCastleLayer::onEnter()
{
    Layer::onEnter();
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallCastleLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    showJobListener = EventListenerCustom::create(SHOW_CASTLE_JOB, CC_CALLBACK_1(HallCastleLayer::showJob, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showJobListener, -1);
    activeListener = EventListenerCustom::create(ACTIVE_JOB, CC_CALLBACK_1(HallCastleLayer::activeJob, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(activeListener, -1);
    changeNameListener = EventListenerCustom::create(UPDATE_NAME, CC_CALLBACK_1(HallCastleLayer::changeName, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(changeNameListener, -1);
}

HallCastleLayer::~HallCastleLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showJobListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(activeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(changeNameListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallCastleLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_CASTLE_UI);
    rootAction = CSLoader::createTimeline(HALL_CASTLE_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);

    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallCastleLayer::clickClose, this));
    auto confirmButton = (Button*)rootBg->getChildByName("ok_button");
    confirmButton->addClickEventListener(CC_CALLBACK_0(HallCastleLayer::clickConfirm, this));
    nameTextField = (TextField*)rootBg->getChildByName("name_text_field");
    nameButton = (Button*)rootBg->getChildByName("name_button");
    nameButton->addClickEventListener(CC_CALLBACK_0(HallCastleLayer::clickChangeName, this));
    jobImg1 = (ImageView*)rootBg->getChildByName("job_img_1");
    jobImg2 = (ImageView*)rootBg->getChildByName("job_img_2");
    jobDescript = (Text*)rootBg->getChildByName("descript_text");
    for (int i = 0; i < 4; i++) {
        jobButton[i] = (Button*)rootBg->getChildByTag(i+1);
        jobButton[i]->addClickEventListener(CC_CALLBACK_1(HallCastleLayer::clickJob, this));
        jobLock[i] = (ImageView*)jobButton[i]->getChildByName("lock_img");
    }

    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallCastleLayer::endIn, this));
    buyJobLayer = nullptr;
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

    return true;
}

void HallCastleLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_CASTLE info = *static_cast<L2E_SHOW_HALL_CASTLE*>(event->getUserData());
    showJobId = info.currJob;
    for (int i = 0; i < 4; i++) {
        jobLock[i]->setVisible(info.jobActive[i] != 1);
    }
    
    nameTextField->setString(info.name);
    jobImg1->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    jobImg2->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    std::string focus = GameUtils::format("choose%d-1.png", info.currJob);
    jobButton[info.currJob-1]->loadTextureNormal(GameUtils::format(CASTLE_DIR,focus.c_str()));
    jobDescript->setString(info.jobDescript);
    
}
void HallCastleLayer::showJob(cocos2d::EventCustom *event)
{
    L2E_SHOW_JOB info = *static_cast<L2E_SHOW_JOB*>(event->getUserData());
    if (info.errNo != 0) {
//#ifndef TAPTAP
        buyJobId = info.currJob;
        buyJobLayer = CSLoader::createNode(CASTLE_BUY_JOB_UI);
        buyJobAction = CSLoader::createTimeline(CASTLE_BUY_JOB_UI);
        buyJobLayer->runAction(buyJobAction);
        buyJobLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        buyJobLayer->setPosition(GameUtils::winSize/2);
        addChild(buyJobLayer);
        
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, buyJobLayer);
        
        auto rootBg = (ImageView*)buyJobLayer->getChildByName("bg_img");
        auto closeButton = (Button*)rootBg->getChildByName("close_button");
        closeButton->addClickEventListener(CC_CALLBACK_0(HallCastleLayer::clickCloseBuyJob, this));
        auto okButton = (Button*)rootBg->getChildByName("buy_button");
        okButton->addClickEventListener(CC_CALLBACK_0(HallCastleLayer::clickBuyJob, this));
        auto priceText = (Text*)rootBg->getChildByName("price_text");
        priceText->setString(Convert2String(info.price));
        
        buyJobAction->play("in", false);
//#else
//        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("new_version", info.errNo));
//        auto tip = TipsNode::create();
//        tip->setupText(errStr);
//        tip->setPosition(GameUtils::winSize/2);
//        addChild(tip, 100);
//#endif
        return;
    }
    std::string focus = GameUtils::format("choose%d.png", showJobId);
    jobButton[showJobId-1]->loadTextureNormal(GameUtils::format(CASTLE_DIR,focus.c_str()));
    
    showJobId = info.currJob;
    jobImg1->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    jobImg2->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    focus = GameUtils::format("choose%d-1.png", info.currJob);
    jobButton[info.currJob-1]->loadTextureNormal(GameUtils::format(CASTLE_DIR,focus.c_str()));
    jobDescript->setString(info.jobDescript);
}
void HallCastleLayer::activeJob(cocos2d::EventCustom *event)
{
    L2E_SHOW_JOB info = *static_cast<L2E_SHOW_JOB*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    std::string focus = GameUtils::format("choose%d.png", showJobId);
    jobButton[showJobId-1]->loadTextureNormal(GameUtils::format(CASTLE_DIR,focus.c_str()));
    
    showJobId = info.currJob;
    jobImg1->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    jobImg2->loadTexture(GameUtils::format(ARENA_DIR, info.jobImg.c_str()));
    focus = GameUtils::format("choose%d-1.png", info.currJob);
    jobButton[info.currJob-1]->loadTextureNormal(GameUtils::format(CASTLE_DIR,focus.c_str()));
    jobDescript->setString(info.jobDescript);
    jobLock[info.currJob-1]->setVisible(false);
    clickCloseBuyJob();
}

void HallCastleLayer::changeName(cocos2d::EventCustom *event)
{
    L2E_UPDATE_NAME info = *static_cast<L2E_UPDATE_NAME*>(event->getUserData());
    nameTextField->setString(info.name);
}
void HallCastleLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallCastleLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallCastleLayer::clickCloseBuyJob()
{
    buyJobAction->play("out", false);
    buyJobAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallCastleLayer::removeBuyJob, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallCastleLayer::clickBuyJob()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_active_job;
    info.value = buyJobId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallCastleLayer::clickConfirm()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_confirm_job;
    info.value = showJobId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    clickClose();
}

void HallCastleLayer::clickJob(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    if (showJobId == tag) {
        return;
    }
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_job;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallCastleLayer::clickChangeName()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_CHANGE_NAME info;
    info.eProtocol = e2l_change_name;
    info.name = nameTextField->getString();
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallCastleLayer::removeOff()
{
    removeFromParent();
}

void HallCastleLayer::removeBuyJob()
{
    removeChild(buyJobLayer);
    buyJobLayer = nullptr;
}

void HallCastleLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = CASTLE_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallCastleLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] == CASTLE_LAYER)
            {
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
//                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//                        pointTip->setPosition(Vec2(150, 35));
//                        
//                        point->setPosition(armsButton[0]->getContentSize()/2);
//                        point->setTag(1000);
//                        auto gemButton = (Button*)gemsBg[0]->getChildByName("gem_button");
//                        gemButton->removeChildByTag(1000);
//                        armsButton[0]->removeChildByTag(1000);
//                        posButton[0]->removeChildByTag(1000);
//                        closeButton->removeChildByTag(1000);
//                        armsButton[0]->addChild(point);
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
            break;
            
        default:
            break;
    }
}
