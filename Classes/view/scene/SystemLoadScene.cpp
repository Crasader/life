//
//  SystemLoadScene.cpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#include "SystemLoadScene.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "../UIManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

SystemLoadScene::SystemLoadScene()
{
    updateLoadListener = EventListenerCustom::create(UPDATE_LOAD, CC_CALLBACK_1(SystemLoadScene::updateLoadBar, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateLoadListener, -1);
}

SystemLoadScene::~SystemLoadScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateLoadListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool SystemLoadScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    auto bgSpt = Sprite::create("ui/load/loadingbg.png");
    float per = GameUtils::winSize.width/bgSpt->getContentSize().width;
    per = MAX(1, per);
    bgSpt->setScaleX(per);
    bgSpt->setPosition(GameUtils::winSize/2);
    addChild(bgSpt);
    
    auto root = CSLoader::createNode(LOAD_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);

    loadBar = (LoadingBar*)root->getChildByName("load_bg")->getChildByName("bar");
//    loadBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    loadBar->setPosition(GameUtils::winSize/2);
    loadBar->setPercent(0);
//    addChild(loadBar);
    
    srand((unsigned)TimeUtil::get_system_tick_s());
    int index = abs(rand())%20;
    std::string tipStr(GameUtils::format("load_tip%d", index));
    auto tipText = (Text*)root->getChildByName("tip_text");
    tipText->setString(StringData::shared()->stringFromKey(tipStr));
    
    scheduleUpdate();
    return true;
}

void SystemLoadScene::onEnter()
{
    Scene::onEnter();
    UIManager::instance()->replacing = false;
}

void SystemLoadScene::update(float dt)
{
    UIManager::instance()->loop(dt);
}
void SystemLoadScene::updateLoadBar(cocos2d::EventCustom *event)
{
    L2E_UPDATE_LOAD info = *static_cast<L2E_UPDATE_LOAD*>(event->getUserData());
    loadBar->setPercent(info.step*100.0/info.count);
}
