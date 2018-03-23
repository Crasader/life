//
//  ArenaMatchOppLayer.cpp
//  life
//
//  Created by ff on 17/9/22.
//
//

#include "ArenaMatchOppLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;

ArenaMatchOppLayer::ArenaMatchOppLayer():
connectPer1(0),
connectPer2(0),
connectCD1(0),
connectCD2(0)
{
    
}

ArenaMatchOppLayer::~ArenaMatchOppLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showRobotListener);
    unscheduleUpdate();
}

bool ArenaMatchOppLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 127))) {
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
    
    auto root = CSLoader::createNode(ARENA_MATCH_UI);
    rootAction = CSLoader::createTimeline(ARENA_MATCH_UI);
    
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    rightJobIcon = (ImageView*)root->getChildByName("right_job_icon");
    rightBar = (LoadingBar*)rightJobIcon->getChildByName("bar_bg")->getChildByName("bar");
    rightForceText = (Text*)rightJobIcon->getChildByName("force_text");
    rightNameText = (Text*)rightJobIcon->getChildByName("name_text");
    for (int i = 0; i < 3; i++) {
        rightPetImg[i] = (ImageView*)root->getChildByName(GameUtils::format("right_pet%d_img", i+1));
        rightPetName[i] = (Text*)rightPetImg[i]->getChildByName("name_text");
    }
    rightSwordImg = (ImageView*)root->getChildByName("right_sword_img");
    rightSwordName = (Text*)rightSwordImg->getChildByName("name_text");
    
    leftJobIcon = (ImageView*)root->getChildByName("left_job_icon");
    leftBar = (LoadingBar*)leftJobIcon->getChildByName("bar_bg")->getChildByName("bar");
    leftForceText = (Text*)leftJobIcon->getChildByName("force_text");
    leftNameText = (Text*)leftJobIcon->getChildByName("name_text");
    for (int i = 0; i < 3; i++) {
        leftPetImg[i] = (ImageView*)root->getChildByName(GameUtils::format("left_pet%d_img", i+1));
        leftPetName[i] = (Text*)leftPetImg[i]->getChildByName("name_text");
    }
    leftSwordImg = (ImageView*)root->getChildByName("left_sword_img");
    leftSwordName = (Text*)leftSwordImg->getChildByName("name_text");
    
    rootAction->play("play1", true);
    int playTime = abs(rand())%5+1;
    auto delay = DelayTime::create(playTime);
    auto func = CallFunc::create(CC_CALLBACK_0(ArenaMatchOppLayer::matchRobot, this));
    auto seq = Sequence::create(delay, func, NULL);
    runAction(seq);
    return true;
}

void ArenaMatchOppLayer::onEnter()
{
    LayerColor::onEnter();
    showRobotListener = EventListenerCustom::create(ARENA_SHOW_ROBOT, CC_CALLBACK_1(ArenaMatchOppLayer::setupRobotView, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showRobotListener, -1);
}

void ArenaMatchOppLayer::update(float dt)
{
    if (connectPer1 > 0 && connectPer1 < 100) {
        connectCD1 += dt;
        connectPer1 = connectCD1*100/connectDur1;
        connectPer1 = MIN(100, connectPer1);
        connectPer1 = MAX(1, connectPer1);
        leftBar->setPercent(connectPer1);
    }
    if (connectPer2 > 0 && connectPer2 < 100) {
        connectCD2 += dt;
        connectPer2 = connectCD2*100/connectDur2;
        connectPer2 = MIN(100, connectPer2);
        connectPer2 = MAX(1, connectPer2);
        rightBar->setPercent(connectPer2);
    }
    if (connectPer1 == 100 && connectPer2 == 100) {
        connectPer1 = 0;
        connectPer2 = 0;
        removeOff();
    }
}

void ArenaMatchOppLayer::matchRobot()
{
    E2L_COMMON info;
    info.eProtocol = e2l_arena_match_robot;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ArenaMatchOppLayer::setupRobotView(cocos2d::EventCustom *event)
{
    rootAction->stop();
    L2E_ARENA_SHOW_ROBOT info = *static_cast<L2E_ARENA_SHOW_ROBOT*>(event->getUserData());
    
    leftJobIcon->loadTexture(GameUtils::format(ARENA_DIR, info.heroJobIcon.c_str()));
    leftForceText->setString(Convert2String(info.heroForce));
    leftNameText->setString(info.heroName);
    if (info.heroSwordImg == "") {
        leftSwordImg->setVisible(false);
    }else{
        leftSwordImg->setVisible(true);
        leftSwordImg->loadTexture(GameUtils::format(ARENA_DIR, info.heroSwordImg.c_str()));
        leftSwordName->setString(info.heroSwordName);
    }
    
    for (int i = 0; i < 3; i++) {
        if (info.heroPetImg[i] == "") {
            leftPetImg[i]->setVisible(false);
        }else{
            leftPetImg[i]->setVisible(true);
            leftPetImg[i]->loadTexture(GameUtils::format(ARENA_DIR, info.heroPetImg[i].c_str()));
            leftPetName[i]->setString(info.heroPetName[i]);
        }
    }
    
    rightJobIcon->loadTexture(GameUtils::format(ARENA_DIR, info.oppJobIcon.c_str()));
    rightForceText->setString(Convert2String(info.oppForce));
    rightNameText->setString(info.oppName);
    if (info.oppSwordImg == "") {
        rightSwordImg->setVisible(false);
    }else{
        rightSwordImg->setVisible(true);
        rightSwordImg->loadTexture(GameUtils::format(ARENA_DIR, info.oppSwordImg.c_str()));
        rightSwordName->setString(info.oppSwordName);
    }
    
    for (int i = 0; i < 3; i++) {
        if (info.oppPetImg[i] == "") {
            rightPetImg[i]->setVisible(false);
        }else{
            rightPetImg[i]->setVisible(true);
            rightPetImg[i]->loadTexture(GameUtils::format(ARENA_DIR, info.oppPetImg[i].c_str()));
            rightPetName[i]->setString(info.oppPetName[i]);
        }
    }
    connectPer1 = 0;
    connectPer2 = 0;
    leftBar->setPercent(0);
    rightBar->setPercent(0);

    rootAction->play("play2", false);
    rootAction->setAnimationEndCallFunc("play2", CC_CALLBACK_0(ArenaMatchOppLayer::showConnectBar, this));
}

void ArenaMatchOppLayer::showConnectBar()
{
    connectPer1 = 1;
    connectPer2 = 1;
    connectCD1 = 0;
    connectCD2 = 0;
    connectDur1 = abs(rand())%4+3;
    connectDur2 = abs(rand())%4+4;
    scheduleUpdate();
}

void ArenaMatchOppLayer::removeOff()
{
    E2L_COMMON info;
    info.eProtocol = e2l_arena_start_fight;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
