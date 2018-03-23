//
//  GameScene.cpp
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#include "GameScene.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "EventDefine.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "view/UIManager.h"
#include "battle/actor/Guardian.h"
#include "battle/actor/Colossus.h"
#include "SimpleAudioEngine.h"
#include "../layer/GameWinLayer.h"
#include "../layer/GameFailLayer.h"
#include "../layer/GameTimeOverLayer.h"
#include "../layer/AbyssWinLayer.h"
#include "../layer/TowerWinLayer.h"
#include "../layer/ArenaWinLayer.h"
#include "../layer/ArenaFailLayer.h"
#include "../layer/PlotVideoLayer.h"
#include "../layer/DialogLayer.h"
#include "../layer/ReviveLayer.h"
#include "../node/EffectNode.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateLoadListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showFightListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(setupTmxMapListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(setupTmxEffectListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(setupHeroListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(setupWitnessListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(setupOpponentListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(preloadMonsterResListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(unlockViewListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(stageVictoryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(stageFailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showReviveListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(reviveListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(stageTimeOverListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseFightListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeFightListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(arenaVictoryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(arenaFailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showDialogListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(completePlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateExpListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showWarningListener);

    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}
void GameScene::onEnter()
{
    cocos2d::Scene::onEnter();
    UIManager::instance()->replacing = false;
    updateLoadListener = EventListenerCustom::create(UPDATE_LOAD_STAGE, CC_CALLBACK_1(GameScene::updateLoad, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateLoadListener, -1);
    showFightListener = EventListenerCustom::create(SHOW_FIGHT, CC_CALLBACK_1(GameScene::showFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showFightListener, -1);
    unlockViewListener = EventListenerCustom::create(UNLOCK_VIEW, CC_CALLBACK_0(GameScene::unlockViewWindow, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unlockViewListener, -1);
    setupTmxMapListener = EventListenerCustom::create(SETUP_TMX_MAP, CC_CALLBACK_1(GameScene::setupTmxMap, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(setupTmxMapListener, -1);
    setupTmxEffectListener = EventListenerCustom::create(SETUP_TMX_MAP_EFFECT, CC_CALLBACK_1(GameScene::setupTmxEffect, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(setupTmxEffectListener, -1);
    setupHeroListener = EventListenerCustom::create(SETUP_HERO, CC_CALLBACK_1(GameScene::setupHero, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(setupHeroListener, -1);
    setupWitnessListener = EventListenerCustom::create(SETUP_WITNESS, CC_CALLBACK_1(GameScene::setupWitness, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(setupWitnessListener, -1);
    setupOpponentListener = EventListenerCustom::create(SETUP_OPPONENT, CC_CALLBACK_1(GameScene::setupOpponent, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(setupOpponentListener, -1);
    preloadMonsterResListener = EventListenerCustom::create(PRELOAD_MONSTER_RES, CC_CALLBACK_1(GameScene::preloadMonsterRes, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(preloadMonsterResListener, -1);
    stageVictoryListener = EventListenerCustom::create(STAGE_VICTORY, CC_CALLBACK_1(GameScene::stageVictory, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(stageVictoryListener, -1);
    stageFailListener = EventListenerCustom::create(STAGE_FAIL, CC_CALLBACK_0(GameScene::stageFail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(stageFailListener, -1);
    showReviveListener = EventListenerCustom::create(SHOW_REVIVE, CC_CALLBACK_0(GameScene::showRevive, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showReviveListener, -1);
    reviveListener = EventListenerCustom::create(FIGHT_REVIVE, CC_CALLBACK_0(GameScene::revive, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(reviveListener, -1);


    stageTimeOverListener = EventListenerCustom::create(STAGE_TIME_OVER, CC_CALLBACK_1(GameScene::stageTimeOver, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(stageTimeOverListener, -1);
    pauseFightListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(GameScene::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseFightListener, -1);
    resumeFightListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(GameScene::resumtFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeFightListener, -1);
    arenaVictoryListener = EventListenerCustom::create(ARENA_VICTORY, CC_CALLBACK_1(GameScene::arenaVictory, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(arenaVictoryListener, -1);
    arenaFailListener = EventListenerCustom::create(ARENA_FAIL, CC_CALLBACK_1(GameScene::arenaFail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(arenaFailListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(GameScene::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    showDialogListener = EventListenerCustom::create(SHOW_DIALOG, CC_CALLBACK_1(GameScene::showDialog, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showDialogListener, -1);
    completePlotListener = EventListenerCustom::create(COMPLETE_PLOT, CC_CALLBACK_0(GameScene::completePlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(completePlotListener, -1);
    updateExpListener = EventListenerCustom::create(UPDATE_EXP, CC_CALLBACK_1(GameScene::updateExp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateExpListener, -1);
    showWarningListener = EventListenerCustom::create(SHOW_WARNING, CC_CALLBACK_1(GameScene::showWarning, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showWarningListener, -1);

    bulletManager = BulletManager::create();
    addChild(bulletManager);
}
bool GameScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    gameState = VIEW_GAME_NONE;
    originState = VIEW_GAME_NONE;
    
    backLayer = Layer::create();
    addChild(backLayer);
    
    midLayer = Layer::create();
    addChild(midLayer);
    
    flashLayer = LayerColor::create(Color4B::GRAY);
    addChild(flashLayer);
    flashLayer->setVisible(false);
    
    actorLayer = ActorLayer::create();
    addChild(actorLayer);
    
    scrFrtLayer = Layer::create();
    scrFrtLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scrFrtLayer->setPosition(GameUtils::winSize/2);
    addChild(scrFrtLayer);
    
    frontLayer = Layer::create();
    addChild(frontLayer);
    
    weatherLayer = Layer::create();
    addChild(weatherLayer);
    
    uiLayer = GameUILayer::create();
    addChild(uiLayer);
    
//    auto leftButton = Button::create("ui/zuo.png");
//    leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    leftButton->setPosition(Vec2(50, 50));
//    addChild(leftButton);
//    leftButton->setZoomScale(0.8);
//    leftButton->addClickEventListener(CC_CALLBACK_0(GameScene::clickLeft, this));
//    
//    auto rightButton = Button::create("ui/you.png");
//    rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    rightButton->setPosition(Vec2(150, 50));
//    addChild(rightButton);
//    rightButton->setZoomScale(0.8);
//    rightButton->addClickEventListener(CC_CALLBACK_0(GameScene::clickRight, this));
    speed = 0;
    plotCamera = false;
    plotCameraPosX = 0;
    plotCameraSpeed = 0;
    plotCameraMission = 0;
    
    loadLayer = LayerColor::create(Color4B::BLACK);
    addChild(loadLayer, 999);
    auto listener=EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Ref* pSender, Event *unused_event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, loadLayer);
    auto bgSpt = Sprite::create("ui/load/loadingbg.png");
    float per = GameUtils::winSize.width/bgSpt->getContentSize().width;
    per = MAX(1, per);
    bgSpt->setScaleX(per);
    bgSpt->setPosition(GameUtils::winSize/2);
    loadLayer->addChild(bgSpt);
    auto root = CSLoader::createNode(LOAD_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    loadLayer->addChild(root);
    
    loadBar = (LoadingBar*)root->getChildByName("load_bg")->getChildByName("bar");
    //    loadBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //    loadBar->setPosition(GameUtils::winSize/2);
    loadBar->setPercent(0);
    
    srand((unsigned)TimeUtil::get_system_tick_s());
    int index = abs(rand())%20;
    std::string tipStr(GameUtils::format("load_tip%d", index));
    auto tipText = (Text*)root->getChildByName("tip_text");
    tipText->setString(StringData::shared()->stringFromKey(tipStr));

//    loadBar = LoadingBar::create("ui/loading_bar.png");
//    loadBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    loadBar->setPosition(Vec2(GameUtils::winSize.width/2, 100));
//    loadBar->setPercent(0);
//    loadLayer->addChild(loadBar);
    loadLayer->setVisible(false);
    
    plotTip = PlotTip::create();
    plotTip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height-200));
    addChild(plotTip);
    plotTip->setVisible(false);
    scheduleUpdate();
    return true;
}

void GameScene::update(float dt)
{
    UIManager::instance()->loop(dt);
    switch (gameState) {
        case VIEW_GAME_PLOT:
        {
            std::string ghost1Anim = ghost1->getAnimation()->getCurrentMovementID();
            std::string ghost2Anim = ghost2->getAnimation()->getCurrentMovementID();
            if (ghost1Anim != ghostAnim[2]) {
                ghost1Anim = ghostAnim[2];
                ghost1->getAnimation()->play(ghost1Anim);
            }
            if (ghost2Anim != ghostAnim[5]) {
                ghost2Anim = ghostAnim[5];
                ghost2->getAnimation()->play(ghost2Anim);
            }
            
            float sx = hero->getScaleRate();
            ghost2->setScaleX((ghostFilpX[2]) ? -sx : sx);
            ghost1->setScaleX((ghostFilpX[5]) ? -sx : sx);
            
            std::string heroAnim = hero->getArmature()->getAnimation()->getCurrentMovementID();
            ghost2->setPosition(ghostPos[5]);
            ghost1->setPosition(ghostPos[2]);
            
            if (ghostPause[5]) {
                ghost2->getAnimation()->pause();
            }else{
                ghost2->getAnimation()->resume();
            }
            if (ghostPause[2]) {
                ghost1->getAnimation()->pause();
            }else{
                ghost1->getAnimation()->resume();
            }
            ghostPos[0] = (hero->getPosition()+hero->getArmature()->getPosition());
            ghostFilpX[0] = (hero->getFlipX());
            ghostPause[0] = hero->getArmature()->getAnimation()->isPause();
            ghostAnim[0] = heroAnim;
            for (int i = 5; i > 0; i--) {
                ghostPause[i] = ghostPause[i-1];
                ghostPos[i] = ghostPos[i-1];
                ghostFilpX[i] = ghostFilpX[i-1];
                ghostAnim[i] = ghostAnim[i-1];
            }

            if (plotCamera) {
                if (abs(camera->getPositionX()-plotCameraPosX) < abs((int)plotCameraSpeed)) {
                    camera->focusPos(Vec2(plotCameraPosX, GroundY));
                    setViewWindow();
                    plotCamera = false;
                    plotCameraPosX = 0;
                    plotCameraSpeed = 0;
                    
                    E2L_COMPLETE_PLOT infoPlot;
                    infoPlot.eProtocol = e2l_complete_plot;
                    infoPlot.missionId = plotCameraMission;
                    infoPlot.value = 0;
                    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
                }else {
                    camera->setCameraCenter(Vec2(plotCameraPosX, GroundY), plotCameraSpeed, false);
                    setViewWindow();
                }
            }
            
        }
            break;
        case VIEW_GAME_NORMAL:
        {
            if (!uiLayer->getWaitResult()) {
                int dir = uiLayer->decodeJoyStick(dt);
                    
                if (uiLayer->getJoyStick()->getPress()) {
                    hero->setDir(dir);
                    hero->startMove();
                }
                if (uiLayer->getLastRushCmd() != uiLayer->getRushCmd()) {
                    uiLayer->setLastRushCmd(uiLayer->getRushCmd());
                    hero->resetWalkOrRush(uiLayer->getRushCmd(), dir);
                }
                hero->setKeyCommand(uiLayer->getKeyCmd());
                hero->setCurrKey(uiLayer->getCurrKey());
                hero->startXuli(uiLayer->getCurrKey());
    //            uiLayer->clearKey();
            }else{
                hero->clearKey();
                hero->resetWalkOrRush(CMD_STOPMOVE, -1);
            }

            std::string ghost1Anim = ghost1->getAnimation()->getCurrentMovementID();
            std::string ghost2Anim = ghost2->getAnimation()->getCurrentMovementID();
            if (ghost1Anim != ghostAnim[2]) {
                ghost1Anim = ghostAnim[2];
                ghost1->getAnimation()->play(ghost1Anim);
            }
            if (ghost2Anim != ghostAnim[5]) {
                ghost2Anim = ghostAnim[5];
                ghost2->getAnimation()->play(ghost2Anim);
            }

            float sx = hero->getScaleRate();
            ghost2->setScaleX((ghostFilpX[2]) ? -sx : sx);
            ghost1->setScaleX((ghostFilpX[5]) ? -sx : sx);
            
            hero->logic(dt);
            hero->adjustHurtEffectPos();
            hero->skillCDClock(dt);
            
            std::string heroAnim = hero->getArmature()->getAnimation()->getCurrentMovementID();
            ghost2->setPosition(ghostPos[5]);
            ghost1->setPosition(ghostPos[2]);
            
            if (ghostPause[5]) {
                ghost2->getAnimation()->pause();
            }else{
                ghost2->getAnimation()->resume();
            }
            if (ghostPause[2]) {
                ghost1->getAnimation()->pause();
            }else{
                ghost1->getAnimation()->resume();
            }
            ghostPos[0] = (hero->getPosition()+hero->getArmature()->getPosition());
            ghostFilpX[0] = (hero->getFlipX());
            ghostPause[0] = hero->getArmature()->getAnimation()->isPause();
            ghostAnim[0] = heroAnim;
            for (int i = 5; i > 0; i--) {
                ghostPause[i] = ghostPause[i-1];
                ghostPos[i] = ghostPos[i-1];
                ghostFilpX[i] = ghostFilpX[i-1];
                ghostAnim[i] = ghostAnim[i-1];
            }
            
            
            bulletManager->update(dt);
            if (witness != nullptr) {
                if (!actorLayer->getPlotPause()) {
                    witness->logic(dt);
                }
                
            }
            camera->setCameraCenter(Vec2(hero->getPositionX(), hero->getPositionY()+hero->getArmature()->getPositionY()));
            setViewWindow();
            if (hero->getBlackActor()->getHp() > 0) {
                uiLayer->countDownTime(dt);
            }
            
        }
            break;
            
        default:
            break;
    }
    
}

void GameScene::showLoad()
{
    gameState = VIEW_GAME_LOAD;
    loadLayer->setVisible(true);
    loadBar->setPercent(0);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameScene::updateLoad(cocos2d::EventCustom *event)
{
    L2E_UPDATE_LOAD info = *static_cast<L2E_UPDATE_LOAD*>(event->getUserData());
    loadBar->setPercent(info.step*100.0/info.count);
}

void GameScene::setupTmxMap(cocos2d::EventCustom *event)
{
    L2E_SETUP_MAP info = *static_cast<L2E_SETUP_MAP*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
    
    
    auto mapGroup = tmx->getObjectGroup("map");
    auto mapObjs = mapGroup->getObjects();
    auto mapInfo = TMXMapInfo::create(info.tmxPath);
    auto tilesetVec = mapInfo->getTilesets();
    
    bgSprite = nullptr;
    midSprite = nullptr;
    frontSprite = nullptr;
    for (auto map : mapObjs) {
        auto dict = map.asValueMap();
        auto gid = dict["gid"].asInt();
        auto name = dict["name"].asString();
        if (name == "back") {
            for (auto tileset : tilesetVec)
            {
                if(tileset->_firstGid == gid)
                {
                    bgSprite = Sprite::create("scene/"+tileset->_originSourceImage);
                    bgSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    bgSprite->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
                    backLayer->addChild(bgSprite);
                    break;
                }
            }
        }else if (name == "mid") {
            for (auto tileset : tilesetVec)
            {
                if(tileset->_firstGid == gid)
                {
                    midSprite = Sprite::create("scene/"+tileset->_originSourceImage);
                    midSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    midSprite->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
                    midLayer->addChild(midSprite);
                    break;
                }
            }
        }else if (name == "front") {
            for (auto tileset : tilesetVec)
            {
                if(tileset->_firstGid == gid)
                {
                    frontSprite = Sprite::create("scene/"+tileset->_originSourceImage);
                    frontSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    frontSprite->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
                    frontLayer->addChild(frontSprite);
                    break;
                }
            }
        }
    }
}

void GameScene::setupTmxEffect(cocos2d::EventCustom *event)
{
    L2E_SETUP_MAP info = *static_cast<L2E_SETUP_MAP*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
    
    auto backEffectGroup = tmx->getObjectGroup("backeffect");
    auto backEffectObjs = backEffectGroup->getObjects();
    
    for (auto effect : backEffectObjs) {
        auto dict = effect.asValueMap();
        auto type = dict["type"].asString();
        if (type == "particle") {
            auto path = GameUtils::format("Particle/%s.plist", dict["name"].asString().c_str());
            auto particle = ParticleSystemQuad::create(path);
            particle->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
            particle->setPositionType(ParticleSystem::PositionType::GROUPED);
            backLayer->addChild(particle);
        }else if (type == "armature") {
            std::string resPath = GameUtils::format(ACTOR_DIR,
                                                    dict["name"].asString().c_str());
//            std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                    dict["name"].asString().c_str(),
//                                                    dict["name"].asString().c_str());
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            auto particle = Armature::create(dict["name"].asString());
            float posX = dict["x"].asFloat();
            float posY = dict["y"].asFloat();
            posX += dict["width"].asInt()/2;
            posY += dict["height"].asInt()/2*3;

            particle->setPosition(Vec2(posX, posY));
            particle->getAnimation()->playWithIndex(0);
            backLayer->addChild(particle);
        }
    }
    
    auto midEffectGroup = tmx->getObjectGroup("mideffect");
    auto midEffectObjs = midEffectGroup->getObjects();
    
    for (auto effect : midEffectObjs) {
        auto dict = effect.asValueMap();
        auto type = dict["type"].asString();
        if (type == "particle") {
            auto path = GameUtils::format("Particle/%s.plist", dict["name"].asString().c_str());
            auto particle = ParticleSystemQuad::create(path);
            particle->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
            particle->setPositionType(ParticleSystem::PositionType::GROUPED);
            midLayer->addChild(particle);
        }else if (type == "armature") {
//            std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                    dict["name"].asString().c_str(),
//                                                    dict["name"].asString().c_str());
            std::string resPath = GameUtils::format(ACTOR_DIR,
                                                    dict["name"].asString().c_str());
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            auto particle = Armature::create(dict["name"].asString());
            float posX = dict["x"].asFloat();
            float posY = dict["y"].asFloat();
            posX += dict["width"].asInt()/2;
            posY += dict["height"].asInt()/2*3;
            particle->setPosition(Vec2(posX, posY));
            particle->getAnimation()->playWithIndex(0);
            midLayer->addChild(particle);
        }
    }
    auto frontEffectGroup = tmx->getObjectGroup("fronteffect");
    auto frontEffectObjs = frontEffectGroup->getObjects();
    
    for (auto effect : frontEffectObjs) {
        auto dict = effect.asValueMap();
        auto type = dict["type"].asString();
        if (type == "particle") {
            auto path = GameUtils::format("Particle/%s.plist", dict["name"].asString().c_str());
            auto particle = ParticleSystemQuad::create(path);
            particle->setPosition(Vec2(dict["x"].asFloat(), dict["y"].asFloat()));
            particle->setPositionType(ParticleSystem::PositionType::GROUPED);
            frontLayer->addChild(particle);
        }else if (type == "armature") {
//            std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                    dict["name"].asString().c_str(),
//                                                    dict["name"].asString().c_str());
            std::string resPath = GameUtils::format(ACTOR_DIR,
                                                    dict["name"].asString().c_str());
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            auto particle = Armature::create(dict["name"].asString());
            float posX = dict["x"].asFloat();
            float posY = dict["y"].asFloat();
            posX += dict["width"].asInt()/2;
            posY += dict["height"].asInt()/2*3;
            particle->setPosition(Vec2(posX, posY));
            particle->getAnimation()->playWithIndex(0);
            frontLayer->addChild(particle);
        }
    }
}

void GameScene::setupHero(cocos2d::EventCustom *event)
{
    L2E_SETUP_HERO info = *static_cast<L2E_SETUP_HERO*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
    
    auto doorGroup = tmx->getObjectGroup("door");
    auto doorObjs = doorGroup->getObjects();
    
    for (auto door : doorObjs) {
        auto dict = door.asValueMap();
        auto name = dict["name"].asString();
        if (name == "born") {
            float posX = dict["x"].asFloat();
            float posY = GroundY;
//            std::string resPath = GameUtils::format(ACTOR_DIR, info.armatureFile.c_str(), info.armatureFile.c_str());
//            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            for (int i = 0; i < 6; i++) {
                ghostPos[i] = Vec2(posX, posY);
            }
            for (int i = 0; i < 6; i++) {
                ghostPause[i] = false;
            }
            
            
            hero = Hero::create();
            hero->setupData(info.configId);
            hero->setupValues(info.hp, info.attack, info.defence, info.crit, info.deCrit);
            
            ghost1 = Armature::create(hero->getBlackActor()->getResName());
            ghost1->setScale(hero->getBlackActor()->getScaleRate());
            ghost1->setOpacity(180);
            ghost1->setColor(Color3B(106, 113, 209));
            ghost1->setPosition(ghostPos[2]);
            actorLayer->addChild(ghost1, 98);
            
            ghost2 = Armature::create(hero->getBlackActor()->getResName());
            ghost2->setScale(hero->getBlackActor()->getScaleRate());
            ghost2->setOpacity(100);
            ghost2->setColor(Color3B(106, 113, 209));
            ghost2->setPosition(ghostPos[5]);
            actorLayer->addChild(ghost2, 97);
            
            hero->addHurtListener();
            hero->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            hero->setPosX(posX);
            hero->setPosY(posY);
            hero->setPosition(Vec2(posX, posY));
            hero->getBlackActor()->initPlayerData();
            hero->getBlackActor()->initSkills(info.holySwordId, info.petId);
            hero->setGhost(3);
            hero->getBlackActor()->calSkillDis();
            //            hero->initSkills(info.atkSkillId);
            hero->preloadSkillRes();
            for (int i = 0; i < 5; i++) {
                if (info.buffId[i] <= 0) {
                    continue;
                }
                
                hero->addSingleBUff(true, info.buffId[i], 100);
            }
            EventCustom event(UPDATE_PET_COUNT);
            PET_COUNT_DATA data;
            data.count = hero->getBlackActor()->getPetCount();
            event.setUserData(&data);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

            
            actorLayer->addChild(hero, 99);
            actorLayer->setHero(hero);
            actorLayer->setPosition(Vec2::ZERO);
            actorLayer->fillHitEffect();
        }
    }
    
    if (hero->getBlackActor()->getGuardId() > 0) {
        auto guard = Guardian::create();
        hero->Guard = guard;
        guard->setupData(hero->getBlackActor()->getGuardId());
        guard->setMan(hero);
        guard->setPosX(hero->getPositionX()-100);
        guard->setPosY(hero->getPositionY());
        guard->setPosition(Vec2(guard->getPosX(), guard->getPosY()));
        guard->preloadSkillRes();
        
        actorLayer->addChild(guard);
        actorLayer->addActorHitEffect(guard);
    }
    
    if (hero->getBlackActor()->getColossusId() > 0) {
        auto col = Colossus::create();
        col->setupData(hero->getBlackActor()->getColossusId());
        col->setMan(hero);
        col->setPosX(hero->getPositionX()-110);
        col->setPosY(hero->getPositionY());
        col->setPosition(Vec2(col->getPosX(), col->getPosY()));
        col->preloadSkillRes();
        hero->colossus = col;
        actorLayer->addChild(col);
        actorLayer->addActorHitEffect(col);
    }
//    SimpleAudioEngine::getInstance()->preloadEffect("sound/hit1.mp3");
//    SimpleAudioEngine::getInstance()->preloadEffect("sound/hit2.mp3");
//    SimpleAudioEngine::getInstance()->preloadEffect("sound/hit3.mp3");
//    SimpleAudioEngine::getInstance()->preloadEffect("sound/yasuo_A1.mp3");
//    SimpleAudioEngine::getInstance()->preloadEffect("sound/yasuo_A2.mp3");

    E2L_COMMON infoTrigger;
    infoTrigger.eProtocol = e2l_trigger_setup_hero;
    ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
    
    auto gearGroup = tmx->getObjectGroup("trap");
    if (gearGroup == nullptr)
    {
        return;
    }
    auto gearObjs = gearGroup->getObjects();
    for (auto npc : gearObjs) {
        auto dict = npc.asValueMap();
        auto npcType = dict["actorId"].asInt();
        auto x = dict["x"].asFloat();
        auto y = dict["y"].asFloat();
        auto flipX = (dict["direct"].asInt() == 1);
        
        actorLayer->addGear(npcType, x, y, flipX);
    }
    
}
void GameScene::setupWitness(cocos2d::EventCustom *event)
{
    
    witness = nullptr;
    L2E_SETUP_WITNESS info = *static_cast<L2E_SETUP_WITNESS*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
        
    auto doorGroup = tmx->getObjectGroup("door");
    auto doorObjs = doorGroup->getObjects();
    
    Vec2 doorPos(Vec2::ZERO);
    for (auto door : doorObjs) {
        auto dict = door.asValueMap();
        auto name = dict["name"].asString();
        
        if (name == "guard") {
            float posX = dict["x"].asFloat();
            float posY = GroundY;
            
            witness = ProtectNPC::create();
            witness->setupData(info.configId);
            witness->addHurtListener();
            witness->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            witness->setPosX(posX);
            witness->setPosY(posY);
            witness->setPosition(Vec2(posX, posY));

            
            actorLayer->addChild(witness);
            actorLayer->setWitness(witness);
            actorLayer->addActorHitEffect(witness);
        } else if (name == "door") {
            doorPos.x = dict["x"].asFloat();
            doorPos.y = GroundY;
            witness->setTargetMovePos(doorPos);
        }
    }
}
void GameScene::setupOpponent(cocos2d::EventCustom *event)
{
    actorLayer->setupOpponent(event);
}

void GameScene::preloadMonsterRes(cocos2d::EventCustom *event)
{
    if (actorLayer->getOppRobot().jobId != 0) {
        int monId = actorLayer->getOppRobot().fighterId;
        std::string res("");
        BlackActor::getActorRes(monId, FIGHTER_CONFIG_FILE, res);
        if (res != "") {
            std::string resPath = GameUtils::format(ACTOR_DIR, res.c_str());
//            std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                    res.c_str(),
//                                                    res.c_str());
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
        }
        return;
    }
    L2E_PRELOAD_MONSTER_RES info = *static_cast<L2E_PRELOAD_MONSTER_RES*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
    
    auto groups = tmx->getObjectGroups();
    for (auto group : groups) {
        std::string name = group->getGroupName();
        if(strncmp(name.c_str(), "wave", strlen("wave")) != 0)
        {
            continue;
        }
        auto objs = group->getObjects();
        for (auto obj : objs)
        {
            
            int monId = obj.asValueMap()["monId"].asInt();
            std::string res("");
            BlackActor::getActorRes(monId, FIGHTER_CONFIG_FILE, res);
            if (res != "") {
//                std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                        res.c_str(),
//                                                        res.c_str());
                std::string resPath = GameUtils::format(ACTOR_DIR,
                                                        res.c_str());
                ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            }
        }
    }
}
void GameScene::showFight(cocos2d::EventCustom *event)
{
    loadLayer->removeFromParent();
    L2E_SHOW_FIGHT info = *static_cast<L2E_SHOW_FIGHT*>(event->getUserData());
    auto tmx = experimental::TMXTiledMap::create(info.tmxPath);
    
    camera = YNCamera::create();
    camera->setMapAreaWidth(tmx->getTileSize().width);
    // 先确定无锁屏的区域
    camera->setMapAreaLeft(0);
    camera->setMapAreaRight(tmx->getTileSize().width);
    camera->focusPos(hero->getPosition());
    addChild(camera);
    
    // 建立锁屏区
    auto lockGroup = tmx->getObjectGroup("lock");
    auto lockObjs = lockGroup->getObjects();

    if (lockObjs.size() > 0) {
        std::vector<LOCK_AREA_DATA> lockX;
        lockX.clear();
        // 暂存锁屏数据
        for (auto lock : lockObjs) {
            auto dict = lock.asValueMap();
            LOCK_AREA_DATA data;
            data.x = dict["x"].asFloat();
            data.left = data.x - dict["left"].asFloat();
            data.right = data.x + dict["right"].asFloat();
            if (data.right-data.left < GameUtils::winSize.width) {
                data.left = data.right-GameUtils::winSize.width;
                data.left = MAX(0, data.left);
            }
            lockX.push_back(data);
        }
        // 根据x排序
        for (int i = 0; i < lockX.size()-1; i++) {
            for (int j = i+1; j < lockX.size(); j++) {
                if (lockX[j].x < lockX[i].x) {
                    std::swap(lockX[i], lockX[j]);
                }
            }
        }
        // 往相机加入锁屏数据
        int index = 0;
        for (auto lockData : lockX) {
            camera->addLock(index, lockData);
            index++;
        }
        
    }
    
    
    if (info.weatherPath != "null") {
        auto weather = ParticleSystemQuad::create(info.weatherPath);
        weather->setPosition(GameUtils::winSize/2);
        weather->setPositionType(ParticleSystem::PositionType::GROUPED);
        weatherLayer->addChild(weather);
    }
    
    uiLayer->setTime(info.totalTime);
    uiLayer->setHeroJob(hero->getBlackActor()->getJob());
    uiLayer->setGoalText(info.goalStr);
    uiLayer->setupByType(info.battleType, info.stageFight);
    int skillAId = hero->getBlackActor()->equipSkills[SKILLA_KEY];
    std::string iconA = hero->getBlackActor()->selSkills[skillAId]->geticonStr();
    uiLayer->getSkill1Sprite()->loadTexture(GameUtils::format(BATTLE_DIR, iconA.c_str()));
    int skillBId = hero->getBlackActor()->equipSkills[SKILLB_KEY];
    std::string iconB = hero->getBlackActor()->selSkills[skillBId]->geticonStr();
    uiLayer->getSkill2Sprite()->loadTexture(GameUtils::format(BATTLE_DIR, iconB.c_str()));
    int skillSPId = hero->getBlackActor()->equipSkills[SKILLS_KEY];
    std::string iconS = hero->getBlackActor()->selSkills[skillSPId]->geticonStr();
    uiLayer->getSPSprite()->loadTexture(GameUtils::format(BATTLE_DIR, iconS.c_str()));
    if (hero->getBlackActor()->getHolySwordSkillId() == 0) {
        uiLayer->getHolySwordDisable()->setVisible(true);
    }else{
        uiLayer->getHolySwordDisable()->setVisible(false);
    }
    battleType = info.battleType;
    bossTeamId = info.bossTeamId;
    if (info.totalWave > 0) {
        uiLayer->showWaveCount(info.currWave, info.totalWave);
    }
    
    actorLayer->setPause(false);
    actorLayer->scheduleUpdate();
    gameState = VIEW_GAME_NORMAL;
    originState = VIEW_GAME_NORMAL;

    if (info.musicFile != "" && info.musicFile != "null") {
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, info.musicFile.c_str()).c_str());
        SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, info.musicFile.c_str()).c_str(), true);
    }
    
    
    E2L_COMMON infoOut;
    infoOut.eProtocol = e2l_start_fight;
    ClientLogic::instance()->ProcessUIRequest(&infoOut);
    
    E2L_TRIGGER_OPEN_LAYER infoTrigger;
    infoTrigger.eProtocol = e2l_trigger_open_layer;
    infoTrigger.index = FIGHT_SCENE;
    ClientLogic::instance()->ProcessUIRequest(&infoTrigger);

}

void GameScene::updateExp(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EXP info = *static_cast<L2E_UPDATE_EXP*>(event->getUserData());
    
    if (info.upLevel) {
        auto effect = EffectNode::create();
        effect->initUplevel(info.level);
        addChild(effect);
#ifdef TAPTAP
        if (info.level == 10) {
            E2L_UPDATE_ONE_VALUE infoPack;
            infoPack.eProtocol = e2l_shop_package_detail;
            infoPack.value = 8;
            ClientLogic::instance()->ProcessUIRequest(&infoPack);
        }
#endif
        C2S_UMENG_USER_LEVEL umengInfo;
        umengInfo.eProtocol = c2s_umeng_user_level;
        umengInfo.level = info.level;
        ClientLogic::instance()->pass2Service(&umengInfo);
}

}

void GameScene::setViewWindow()
{
    Vec2 off;
    auto winSize = Director::getInstance()->getWinSize();
    
    off.x = fmax(camera->getPositionX() - winSize.width / 2, 0);
    off.x = fmin(off.x, camera->getMapAreaRight() - winSize.width);
    off.y =  YNCamera::bShake?camera->vShake():camera->getPositionY();
    
    if (bgSprite != nullptr) {
        int bgWidth = bgSprite->getTexture()->getPixelsWide();
        float bgOffx = fmax(camera->getMapAreaWidth() - bgWidth, 0) / (camera->getMapAreaWidth() - winSize.width)*off.x;
        backLayer->setPosition(-off + Vec2(bgOffx, 0));
    }
    midLayer->setPosition(-off);
    actorLayer->setPosition(-off);
#ifdef FOR_HUA
    if (frontSprite != nullptr) {
        int bgWidth = frontSprite->getTexture()->getPixelsWide();
        float bgOffx = fmax(camera->getMapAreaWidth() - bgWidth, 0) / (camera->getMapAreaWidth() - winSize.width)*off.x;
        frontLayer->setPosition(-off + Vec2(bgOffx, 0));
    }
#else
    frontLayer->setPosition(-off);
#endif
    if (YNCamera::flashCount > 0) {
        if (YNCamera::flashCount % 2 == 0) {
            backLayer->setVisible(false);
            midLayer->setVisible(false);
            frontLayer->setVisible(false);
            flashLayer->setVisible(true);
        }else{
            backLayer->setVisible(true);
            midLayer->setVisible(true);
            frontLayer->setVisible(true);
            flashLayer->setVisible(false);
        }
        YNCamera::flashCount--;
    }
}

void GameScene::clickLeft()
{
    speed = -3;
}

void GameScene::clickRight()
{
    speed = 3;
}

void GameScene::unlockViewWindow()
{
    if (camera) {
        camera->unlock();
    }
}

void GameScene::stageVictory(cocos2d::EventCustom *event)
{
    gameState = VIEW_GAME_VICTORY;
    actorLayer->unscheduleUpdate();
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if(info.value == 1) {
        auto winLayer = GameWinLayer::create();
        winLayer->setLeftSecond(uiLayer->getStageTime());
        winLayer->setHeroHp(hero->getBlackActor()->getHp());
        winLayer->setHeroMaxHp(hero->getBlackActor()->getMaxHp());
        
        addChild(winLayer);
    }else if(info.value == 2) {
        auto winLayer = TowerWinLayer::create();
        addChild(winLayer);
    }else{
        auto winLayer = AbyssWinLayer::create();
        addChild(winLayer);
    }
    
}

void GameScene::stageFail()
{
    gameState = VIEW_GAME_FAIL;
//    actorLayer->unscheduleUpdate();
    auto failLayer = GameFailLayer::create();
    addChild(failLayer);
}

void GameScene::showRevive()
{
    auto reviveLayer = ReviveLayer::create();
    addChild(reviveLayer);
}

void GameScene::revive()
{
    hero->revive();
}

void GameScene::arenaVictory(cocos2d::EventCustom *event)
{
    gameState = VIEW_GAME_VICTORY;
    actorLayer->unscheduleUpdate();
    auto winLayer = ArenaWinLayer::create();
    winLayer->setupView(event);
    addChild(winLayer);
}
void GameScene::arenaFail(cocos2d::EventCustom *event)
{
    gameState = VIEW_GAME_FAIL;
    auto failLayer = ArenaFailLayer::create();
    failLayer->setupView(event);
    addChild(failLayer);
}
void GameScene::stageTimeOver(cocos2d::EventCustom *event)
{
    L2E_FIGHT_TIME_OVER info = *static_cast<L2E_FIGHT_TIME_OVER*>(event->getUserData());
    auto timeOverLayer = GameTimeOverLayer::create();
    timeOverLayer->setHeroHp(hero->getBlackActor()->getHp());
    timeOverLayer->setMaxHeroHp(hero->getBlackActor()->getMaxHp());
    auto opponent = actorLayer->getMonsterWithTeamId(info.teamId);
    if (opponent == nullptr) {
        timeOverLayer->setOpponentHp(0);
        timeOverLayer->setMaxOpponentHp(100);
    }else{
        timeOverLayer->setOpponentHp(opponent->getBlackActor()->getHp());
        timeOverLayer->setMaxOpponentHp(opponent->getBlackActor()->getMaxHp());
    }
    
    addChild(timeOverLayer);
}

void GameScene::pauseFight()
{
    gameState = VIEW_GAME_PAUSE;
    actorLayer->setPause(true);
//    ghost1->getAnimation()->pause();
//    ghost2->getAnimation()->pause();
}

void GameScene::resumtFight()
{
    gameState = VIEW_GAME_NORMAL;
    actorLayer->setPause(false);
//    ghost1->getAnimation()->resume();
//    ghost2->getAnimation()->resume();
}

void GameScene::startPlot(cocos2d::EventCustom *event)
{
    if (gameState != VIEW_GAME_PLOT) {
        originState = gameState;
        gameState = VIEW_GAME_PLOT;
        actorLayer->setPlotPause(true);
    }
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 10) {
        auto videoLayer = PlotVideoLayer::create();
        videoLayer->showVideo(info.param[0], info.missionId);
        addChild(videoLayer);
    }else if (info.type == 15 || info.type == 16) {
        hero->startPlot(event);
    }else if (info.type == 6) {
        if (info.param[1] == 0) {
            camera->focusPos(Vec2(info.param[0], GroundY));
            setViewWindow();
            plotCamera = false;
            plotCameraPosX = 0;
            plotCameraSpeed = 0;
            plotCameraMission = 0;
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        }else{
            plotCamera = true;
            plotCameraPosX = info.param[0];
            plotCameraSpeed = info.param[1];
            plotCameraMission = info.missionId;
        }
//        camera->setCameraCenter(Vec2(info.param[0], GroundY), (info.param[2]-camera->getPositionX())/)
    }else if (info.type == 4) {
        if (info.param[0] == 3) {
            actorLayer->moveNPC(info.param[1], info.param[2], info.param[3], info.missionId);
        }else if (info.param[0] == 2) {
            actorLayer->moveMonster(info.param[1], info.param[2], info.param[3], info.missionId);
        }else if (info.param[0] == 1) {
            hero->plotMove(info.param[2], info.param[3], info.missionId);
        }
    }else if (info.type == 5) {
        if (info.param[0] == 3) {
            actorLayer->changeNPCAction(info.param[1], info.param[2], (info.param[3]==1), info.missionId);
        }else if (info.param[0] == 2) {
            actorLayer->changeMonsterAction(info.param[1], info.param[2], (info.param[3]==1), info.missionId);
        }else if (info.param[0] == 1) {
            hero->plotChangeAction(info.param[2], info.param[3], info.missionId);
        }
    }else if (info.type == 11) {
        if (info.param[1] == 0) {
            plotTip->setVisible(false);
        }else{
            plotTip->setVisible(true);
            plotTip->setupText(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[0])));
            
        }
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = info.missionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    }
}

void GameScene::showDialog(cocos2d::EventCustom *event)
{
    if (gameState != VIEW_GAME_PLOT) {
        originState = gameState;
        gameState = VIEW_GAME_PLOT;
        actorLayer->setPlotPause(true);
    }
    auto dialogLayer = DialogLayer::create();
    dialogLayer->setupView(event);
    addChild(dialogLayer);
}

void GameScene::completePlot()
{
    gameState = originState;
//    originState = VIEW_GAME_NONE;
    actorLayer->setPlotPause(false);
}

void GameScene::showWarning(cocos2d::EventCustom *event)
{
    auto effect = EffectNode::create();
    effect->initWithRes(WARNING_UI);
    effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    effect->setPosition(GameUtils::winSize/2);
    addChild(effect);
}
