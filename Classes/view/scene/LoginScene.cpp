//
//  LoginScene.cpp
//  life
//
//  Created by ff on 18/1/23.
//
//

#include "LoginScene.h"
#include "utils/GameUtils.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;

LoginScene::LoginScene():
connected(false)
{
    
}

LoginScene::~LoginScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(netStateListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool LoginScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    bgSpt = Sprite::create("ui/load/loginbg.png");

    float per = GameUtils::winSize.width/bgSpt->getContentSize().width;
    per = MAX(1, per);
    bgSpt->setScaleX(per);
    bgSpt->setPosition(GameUtils::winSize/2);
    addChild(bgSpt);
    bgSpt->setVisible(false);
    
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            "logo",
//                                            "logo");
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            "logo");
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    logoArm = Armature::create("logo");
    logoArm->setPosition(Vec2(0, GameUtils::winSize.height/2));
    addChild(logoArm);
    logoArm->setVisible(false);
    
//    std::string actorResPath = GameUtils::format(ACTOR_DIR,
//                                                 "login",
//                                                 "login");
    std::string actorResPath = GameUtils::format(ACTOR_DIR,
                                                 "login");
    ArmatureDataManager::getInstance()->addArmatureFileInfo(actorResPath);
    actorArm = Armature::create("login");
    actorArm->setPosition(Vec2(GameUtils::winSize.width, GameUtils::winSize.height/2));
    addChild(actorArm);
    actorArm->setVisible(false);

    auto root = CSLoader::createNode(LOGIN_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    rootAction = CSLoader::createTimeline(LOGIN_UI);
    root->runAction(rootAction);
    

    rootAction->play("play1", false);
    rootAction->setAnimationEndCallFunc("play1", CC_CALLBACK_0(LoginScene::endIn, this));
    return true;
}

void LoginScene::onEnter()
{
    Scene::onEnter();
    netStateListener = EventListenerCustom::create(UPDATE_NET_STATE, CC_CALLBACK_1(LoginScene::updateNetState, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(netStateListener, -1);
}

void LoginScene::endIn()
{
    bgSpt->setVisible(true);
    actorArm->setVisible(true);
    logoArm->setVisible(true);
    actorArm->getAnimation()->play("login");
    logoArm->getAnimation()->play("logo1");
    logoArm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(LoginScene::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    rootAction->play("play2", true);
    
    E2L_COMMON info;
    info.eProtocol = e2l_check_net;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

bool LoginScene::onTouchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (connected) {
        E2L_COMMON info;
        info.eProtocol = e2l_login_click;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }else{
        E2L_COMMON info;
        info.eProtocol = e2l_exit;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
    
    return true;
}

void LoginScene::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string &movementID)
{
    if (movementID=="logo1" && movementType == COMPLETE) {
        logoArm->getAnimation()->play("logo2");
    }
}

void LoginScene::updateNetState(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    connected = info.value;
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(LoginScene::onTouchesBegan, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    if (!connected) {
        rootAction->play("play3", true);
    }
}
