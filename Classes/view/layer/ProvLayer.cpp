//
//  ProvLayer.cpp
//  life
//
//  Created by ff on 17/8/24.
//
//

#include "ProvLayer.h"
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


ProvLayer::ProvLayer():
playing(false),
stopFight(false)
{
    
}

void ProvLayer::onEnter()
{
    Layer::onEnter();
    confirmStageListener = EventListenerCustom::create(CONFIRM_PROV_STAGE, CC_CALLBACK_1(ProvLayer::confirmStage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(confirmStageListener, -1);
    confirmFighterListener = EventListenerCustom::create(CONFIRM_PROV_FIGHTER, CC_CALLBACK_1(ProvLayer::confirmFighter, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(confirmFighterListener, -1);
    provFightListener = EventListenerCustom::create(PROV_FIGHT, CC_CALLBACK_1(ProvLayer::provFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(provFightListener, -1);
    provWinListener = EventListenerCustom::create(PROV_WIN, CC_CALLBACK_1(ProvLayer::provWin, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(provWinListener, -1);
    provLoseListener = EventListenerCustom::create(PROV_LOSE, CC_CALLBACK_1(ProvLayer::provLose, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(provLoseListener, -1);
    provResetListener = EventListenerCustom::create(PROV_RESET, CC_CALLBACK_1(ProvLayer::provReset, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(provResetListener, -1);
    addTicketListener = EventListenerCustom::create(PROV_ADD_TICKET, CC_CALLBACK_1(ProvLayer::addTicket, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addTicketListener, -1);
    refreshResetListener = EventListenerCustom::create(PROV_REFRESH_RESET, CC_CALLBACK_1(ProvLayer::refreshReset, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(refreshResetListener, -1);
}

ProvLayer::~ProvLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(confirmStageListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(confirmFighterListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(provFightListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(provWinListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(provLoseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(provResetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addTicketListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(refreshResetListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool ProvLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_PROV_UI);
    rootAction = CSLoader::createTimeline(HALL_PROV_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickClose, this));

    startButton = (Button*)rootBg->getChildByName("start_button");
    startButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickStart, this));
    auto addTicketButton = (Button*)rootBg->getChildByName("add_ticket_button");
    addTicketButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickAddTicket, this));
    auto resetButton = (Button*)rootBg->getChildByName("reset_button");
    resetButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickReset, this));
    
    stageScroll = (ScrollView*)rootBg->getChildByName("stage_scroll");
    resetCountText = (Text*)rootBg->getChildByName("reset_count_text");
    ticketCountText = (Text*)rootBg->getChildByName("ticket_count_text");
    fighterList = (ListView*)rootBg->getChildByName("fighter_list");
    
    for (int i = 0; i < 50; i++) {
        stageNode[i] = stageScroll->getChildByTag(i+1);
        stageAction[i] = CSLoader::createTimeline(PROV_POINT_UI);
        stageNode[i]->runAction(stageAction[i]);
        stageAction[i]->play("stand", false);
        
        auto stageText = (Text*)stageNode[i]->getChildByName("stage_text");
        stageText->setString(
                    GameUtils::format(
                                StringData::shared()->stringFromKey("prov_stage").c_str(),
                                i+1));
    }
    
    rewardLayer = Layer::create();
//    rewardLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    rewardLayer->setPosition(GameUtils::winSize/2);
//    auto callback1 = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener1 = EventListenerTouchOneByOne::create();
//    listener1->onTouchBegan = callback1;
//    listener1->setSwallowTouches(true);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, rewardLayer);
    
    addChild(rewardLayer,100);
    auto rewardNode = CSLoader::createNode(PROV_REWARD_UI);
    rewardAction = CSLoader::createTimeline(PROV_REWARD_UI);
    rewardNode->runAction(rewardAction);
    rewardNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    rewardNode->setPosition(GameUtils::winSize/2);
    rewardLayer->addChild(rewardNode);
    rewardLayer->setVisible(false);
    rewardGoldText = (Text*)rewardNode->getChildByName("bg_img")->getChildByName("gold_img")
        ->getChildByName("count_text");
    rewardStoneText = (Text*)rewardNode->getChildByName("bg_img")->getChildByName("stone_img")
        ->getChildByName("count_text");
    return true;
}

void ProvLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_PROV info = *static_cast<L2E_SHOW_HALL_PROV*>(event->getUserData());
    ticketCountText->setString(Convert2String(info.ticketCount));
    resetCountText->setString(Convert2String(info.resetCount));
    
    currStage = info.stage;
    auto boundsBg = (ImageView*)stageNode[info.stage-1]->getChildByName("bounds_bg");
    auto goldText = (Text*)boundsBg->getChildByName("gold_count_text");
    auto stoneText = (Text*)boundsBg->getChildByName("stone_count_text");
    goldText->setString(Convert2String(info.boundGold));
    stoneText->setString(Convert2String(info.boundStone));
    auto defenceBg = (ImageView*)stageNode[info.stage-1]->getChildByName("defence_bg");
    auto defenceIcon = (ImageView*)defenceBg->getChildByName("icon");
    defenceIcon->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.petIcon.c_str()));
    int defenceHpPercent = info.petHp*100/info.petMaxHp;
    auto defenceBar = (LoadingBar*)defenceBg->getChildByName("hp_bar");
    auto defenceHpText = (Text*)defenceBar->getChildByName("hp_text");
    defenceHpText->setString(GameUtils::format("%d\%%",defenceHpPercent));
    defenceBar->setPercent(defenceHpPercent);
    for (int i = 1; i <= 5; i++) {
        auto star = (ImageView*)defenceBg->getChildByTag(i);
        star->setVisible(info.petLevel>=i);
    }
    stageAction[info.stage-1]->play("play", true);
    
    int contentWith = stageScroll->getContentSize().width;
    int totalWidth = stageScroll->getInnerContainer()->getContentSize().width;
    int posX = stageNode[info.stage-1]->getPositionX();
    int width = totalWidth-contentWith;
    posX -= contentWith/2;
    float barPercent = posX*100/width;
    barPercent = MAX(0, barPercent);
    barPercent = MIN(100, barPercent);
    stageScroll->jumpToPercentHorizontal(barPercent);
    
    
    auto originIcon = (ImageView*)fighterList->getChildByName("icon_img");
    originIcon->retain();
    originIcon->removeFromParent();
    for (int i = 0; i < 10; i++) {
        auto fighterIcon = (ImageView*)originIcon->clone();
        fighterIcon->setTag(info.fighterId[i]);
        fighterIcon->loadTexture(GameUtils::format(COMMON_DIR, info.fighterIcon[i].c_str()));
        if (info.fighterLock[i] == 1) {
            fighterIcon->setColor(Color3B::GRAY);
            fighterIcon->getChildByName("dead_flag1")->setVisible(false);
            fighterIcon->getChildByName("dead_flag2")->setVisible(false);
        }else{
            fighterIcon->setColor(Color3B::WHITE);
            if (info.fighterAlive[i] == 1) {
                fighterIcon->getChildByName("dead_flag1")->setVisible(false);
                fighterIcon->getChildByName("dead_flag2")->setVisible(false);
            }else{
                fighterIcon->getChildByName("dead_flag1")->setVisible(true);
                fighterIcon->getChildByName("dead_flag2")->setVisible(true);
            }
        }

        fighterList->addChild(fighterIcon);
    }
    originIcon->release();
    
    for (int i = 0; i < 50; i++) {
        stageNode[i] = stageScroll->getChildByTag(i+1);
        
        auto stageSprite = (Sprite*)stageNode[i]->getChildByName("Sprite_1");
        stageSprite->setTexture(GameUtils::format(PROV_DIR, info.stageIcon[i].c_str()));
    }
    playing = false;
    stopFight = false;
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(ProvLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void ProvLayer::confirmStage(cocos2d::EventCustom *event)
{
    L2E_PROV_CONFIRM_STAGE info = *static_cast<L2E_PROV_CONFIRM_STAGE*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("prov_stage_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stageAction[currStage-1]->clearFrameEndCallFuncs();
        stageAction[currStage-1]->play("play", true);
        return;
    }
    
    if (currStage != info.stage) {
        stageAction[currStage-1]->play("stand", false);
        currStage = info.stage;
    }
    stageAction[info.stage-1]->play("play", false);
    stageAction[info.stage-1]->setAnimationEndCallFunc("play", CC_CALLBACK_0(ProvLayer::endConfirmStage, this));
    
    currStage = info.stage;
    auto boundsBg = (ImageView*)stageNode[info.stage-1]->getChildByName("bounds_bg");
    auto goldText = (Text*)boundsBg->getChildByName("gold_count_text");
    auto stoneText = (Text*)boundsBg->getChildByName("stone_count_text");
    goldText->setString(Convert2String(info.boundGold));
    stoneText->setString(Convert2String(info.boundStone));
    auto defenceBg = (ImageView*)stageNode[info.stage-1]->getChildByName("defence_bg");
    auto defenceIcon = (ImageView*)defenceBg->getChildByName("icon");
    defenceIcon->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.petIcon.c_str()));
    int defenceHpPercent = info.petHp*100/info.petMaxHp;
    auto defenceBar = (LoadingBar*)defenceBg->getChildByName("hp_bar");
    auto defenceHpText = (Text*)defenceBar->getChildByName("hp_text");
    defenceHpText->setString(GameUtils::format("%d\%%",defenceHpPercent));
    defenceBar->setPercent(defenceHpPercent);
    for (int i = 1; i <= 5; i++) {
        auto star = (ImageView*)defenceBg->getChildByTag(i);
        star->setVisible(info.petLevel>=i);
    }
    
    int contentWith = stageScroll->getContentSize().width;
    int totalWidth = stageScroll->getInnerContainer()->getContentSize().width;
    int posX = stageNode[info.stage-1]->getPositionX();
    int width = totalWidth-contentWith;
    posX -= contentWith/2;
    float barPercent = posX*100/width;
    barPercent = MAX(0, barPercent);
    barPercent = MIN(100, barPercent);
    stageScroll->jumpToPercentHorizontal(barPercent);
}

void ProvLayer::confirmFighter(cocos2d::EventCustom *event)
{
    L2E_PROV_CONFIRM_FIGHTER info = *static_cast<L2E_PROV_CONFIRM_FIGHTER*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("prov_fighter_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stageAction[currStage-1]->clearFrameEndCallFuncs();
        stageAction[currStage-1]->play("play", true);
        return;
    }
    
    auto attackBg = (ImageView*)stageNode[currStage-1]->getChildByName("attack_bg");
    auto icon = (ImageView*)attackBg->getChildByName("icon_bg")->getChildByName("icon");
    icon->loadTexture(GameUtils::format(COMMON_DIR, info.fighterIcon.c_str()));
    int hpPercent = info.fighterHp*100/info.fighterMaxHp;
    auto hpBar = (LoadingBar*)attackBg->getChildByName("hp_bar");
    auto hpText = (Text*)hpBar->getChildByName("hp_text");
    hpText->setString(GameUtils::format("%d\%%",hpPercent));
    hpBar->setPercent(hpPercent);
    for (int i = 1; i <= 5; i++) {
        auto star = (ImageView*)attackBg->getChildByTag(i);
        star->setVisible(info.fighterLevel>=i);
    }

    ticketCountText->setString(Convert2String(info.ticketCount));
    
    stageAction[currStage-1]->play("in", false);
    stageAction[currStage-1]->setAnimationEndCallFunc("in", CC_CALLBACK_0(ProvLayer::endConfirmFighter, this));
}
void ProvLayer::provFight(cocos2d::EventCustom *event)
{
    L2E_PROV_FIGHT info = *static_cast<L2E_PROV_FIGHT*>(event->getUserData());
    if(info.attackDirect == 0){
        auto attackBg = (ImageView*)stageNode[currStage-1]->getChildByName("attack_bg");
        int hpPercent = info.defenceHp*100/info.defenceMaxHp;
        auto hpBar = (LoadingBar*)attackBg->getChildByName("hp_bar");
        auto hpText = (Text*)hpBar->getChildByName("hp_text");
        hpText->setString(GameUtils::format("%d\%%",hpPercent));
        hpBar->setPercent(hpPercent);
        stageAction[currStage-1]->play("fight1", false);
        stageAction[currStage-1]->setAnimationEndCallFunc("fight1", CC_CALLBACK_0(ProvLayer::endFight, this));
    }else{
        auto defenceBg = (ImageView*)stageNode[currStage-1]->getChildByName("defence_bg");
        int defenceHpPercent = info.defenceHp*100/info.defenceMaxHp;
        auto defenceBar = (LoadingBar*)defenceBg->getChildByName("hp_bar");
        auto defenceHpText = (Text*)defenceBar->getChildByName("hp_text");
        defenceHpText->setString(GameUtils::format("%d\%%",defenceHpPercent));
        defenceBar->setPercent(defenceHpPercent);
        stageAction[currStage-1]->play("fight2", false);
        stageAction[currStage-1]->setAnimationEndCallFunc("fight2", CC_CALLBACK_0(ProvLayer::endFight, this));
    }
}

void ProvLayer::provWin(cocos2d::EventCustom *event)
{
    L2E_PROV_WIN info = *static_cast<L2E_PROV_WIN*>(event->getUserData());
    rewardGoldText->setString(Convert2String(info.boundGold));
    rewardStoneText->setString(Convert2String(info.boundStone));
    
    stageAction[currStage-1]->play("cha2", false);
    stageAction[currStage-1]->setAnimationEndCallFunc("cha2", CC_CALLBACK_0(ProvLayer::endKillDefence, this));
}
void ProvLayer::provLose(cocos2d::EventCustom *event)
{
    L2E_PROV_LOSE info = *static_cast<L2E_PROV_LOSE*>(event->getUserData());
    int petId = info.fighterId;
    fighterList->getChildByTag(petId)->getChildByName("dead_flag1")->setVisible(true);
    fighterList->getChildByTag(petId)->getChildByName("dead_flag2")->setVisible(true);

    stageAction[currStage-1]->play("cha1", false);
    stageAction[currStage-1]->setAnimationEndCallFunc("cha1", CC_CALLBACK_0(ProvLayer::endKillAttack, this));
}

void ProvLayer::provReset(cocos2d::EventCustom *event)
{
    L2E_PROV_RESET info = *static_cast<L2E_PROV_RESET*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("prov_reset_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    resetCountText->setString(Convert2String(info.resetCount));
    
    currStage = info.stage;
    auto boundsBg = (ImageView*)stageNode[info.stage-1]->getChildByName("bounds_bg");
    auto goldText = (Text*)boundsBg->getChildByName("gold_count_text");
    auto stoneText = (Text*)boundsBg->getChildByName("stone_count_text");
    goldText->setString(Convert2String(info.boundGold));
    stoneText->setString(Convert2String(info.boundStone));
    auto defenceBg = (ImageView*)stageNode[info.stage-1]->getChildByName("defence_bg");
    auto defenceIcon = (ImageView*)defenceBg->getChildByName("icon");
    defenceIcon->loadTexture(GameUtils::format(CHAPTER_NAME_ICON, info.petIcon.c_str()));
    int defenceHpPercent = info.petHp*100/info.petMaxHp;
    auto defenceBar = (LoadingBar*)defenceBg->getChildByName("hp_bar");
    auto defenceHpText = (Text*)defenceBar->getChildByName("hp_text");
    defenceHpText->setString(GameUtils::format("%d\%%",defenceHpPercent));
    defenceBar->setPercent(defenceHpPercent);
    for (int i = 1; i <= 5; i++) {
        auto star = (ImageView*)defenceBg->getChildByTag(i);
        star->setVisible(info.petLevel>=i);
    }
    stageAction[info.stage-1]->play("play", true);
    
    int contentWith = stageScroll->getContentSize().width;
    int totalWidth = stageScroll->getInnerContainer()->getContentSize().width;
    int posX = stageNode[info.stage-1]->getPositionX();
    int width = totalWidth-contentWith;
    posX -= contentWith/2;
    float barPercent = posX*100/width;
    barPercent = MAX(0, barPercent);
    barPercent = MIN(100, barPercent);
    stageScroll->jumpToPercentHorizontal(barPercent);
    
    for (int i = 0; i < 10; i++) {
        
        auto fighterIcon = (ImageView*)fighterList->getChildByTag(i+1);
        if (info.fighterLock[i] == 1) {
            fighterIcon->setColor(Color3B::GRAY);
            fighterIcon->getChildByName("dead_flag1")->setVisible(false);
            fighterIcon->getChildByName("dead_flag2")->setVisible(false);
        }else{
            fighterIcon->setColor(Color3B::WHITE);
            if (info.fighterAlive[i] == 1) {
                fighterIcon->getChildByName("dead_flag1")->setVisible(false);
                fighterIcon->getChildByName("dead_flag2")->setVisible(false);
            }else{
                fighterIcon->getChildByName("dead_flag1")->setVisible(true);
                fighterIcon->getChildByName("dead_flag2")->setVisible(true);
            }
        }
        
    }
}
void ProvLayer::clickClose()
{
    if (playing) {
        return;
    }
    
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ProvLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void ProvLayer::removeOff()
{
    removeFromParent();
}
void ProvLayer::endConfirmStage()
{
    stageAction[currStage-1]->stop();
//    stageAction[currStage-1]->play("play", true);
//    stageAction[currStage-1]->setAnimationEndCallFunc("play", nullptr);
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    stageAction[currStage-1]->play("play", true);
    if (stopFight) {
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        return;
    }
    E2L_COMMON info;
    info.eProtocol = e2l_confirm_prov_fighter;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ProvLayer::endConfirmFighter()
{
    if (stopFight) {
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stageAction[currStage-1]->clearFrameEndCallFuncs();
        stageAction[currStage-1]->play("play", true);
        return;
    }

    E2L_COMMON info;
    info.eProtocol = e2l_prov_fight;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ProvLayer::endFight()
{
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    if (stopFight) {
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stageAction[currStage-1]->clearFrameEndCallFuncs();
        stageAction[currStage-1]->play("play", true);
        return;
    }

    E2L_COMMON info;
    info.eProtocol = e2l_prov_end_fight;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ProvLayer::endKillDefence()
{
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    stageAction[currStage-1]->play("win", false);
    stageAction[currStage-1]->setAnimationEndCallFunc("win", CC_CALLBACK_0(ProvLayer::endWin, this));
}
void ProvLayer::endKillAttack()
{
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    stageAction[currStage-1]->play("lose", false);
    stageAction[currStage-1]->setAnimationEndCallFunc("lose", CC_CALLBACK_0(ProvLayer::endLose, this));
}
void ProvLayer::endWin()
{
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    stageAction[currStage-1]->play("play", true);
    
    rewardLayer->setVisible(true);
    rewardAction->play("play", false);
    rewardAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(ProvLayer::endReward, this));
//    E2L_COMMON info;
//    info.eProtocol = e2l_prov_end_win;
//    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ProvLayer::endReward()
{
    rewardLayer->setVisible(false);
    rewardAction->stop();
    
    E2L_COMMON info;
    info.eProtocol = e2l_prov_end_win;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ProvLayer::endLose()
{
    stageAction[currStage-1]->clearFrameEndCallFuncs();
    stageAction[currStage-1]->play("play", true);
    if (stopFight) {
        playing = false;
        stopFight = false;
        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stageAction[currStage-1]->clearFrameEndCallFuncs();
        stageAction[currStage-1]->play("play", true);
        return;
    }
    E2L_COMMON info;
    info.eProtocol = e2l_prov_end_lose;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void ProvLayer::clickStart()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (playing) {
//        startButton->setTitleText(StringData::shared()->stringFromKey("start_prov"));
        stopFight = true;
    }else{
        startButton->setTitleText(StringData::shared()->stringFromKey("stop_prov"));
        playing = true;
        stopFight = false;
        E2L_COMMON info;
        info.eProtocol = e2l_click_prov_start;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}

void ProvLayer::clickReset()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (playing) {
        return;
    }
    
    E2L_COMMON info;
    info.eProtocol = e2l_prov_reset;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ProvLayer::clickAddTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (playing) {
        return;
    }
    
    ticketLayer = Layer::create();
    //    rewardLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //    rewardLayer->setPosition(GameUtils::winSize/2);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, ticketLayer);
    
    addChild(ticketLayer,100);
    
    auto ticketNode = CSLoader::createNode(PROV_TICKET_UI);
    ticketAction = CSLoader::createTimeline(PROV_TICKET_UI);
    ticketNode->runAction(ticketAction);
    ticketNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ticketNode->setPosition(GameUtils::winSize/2);
    ticketLayer->addChild(ticketNode);
    ticketAction->play("in", false);
    
    buyTicketCountText = (Text*)ticketNode->getChildByName("bg_img")->getChildByName("buy_bg")
        ->getChildByName("count_text");
    buyTicketPriceText = (Text*)ticketNode->getChildByName("bg_img")->getChildByName("buy_bg")
        ->getChildByName("price_text");
    buyTicketCount = 1;
    buyTicketCountText->setString(Convert2String(buyTicketCount));
    buyTicketPriceText->setString(Convert2String(provTicketPrice*buyTicketCount));
    
    auto reduceButton = (Button*)ticketNode->getChildByName("bg_img")->getChildByName("buy_bg")
        ->getChildByName("reduce_button");
    reduceButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickReduceTicket, this));
    auto plusButton = (Button*)ticketNode->getChildByName("bg_img")->getChildByName("buy_bg")
        ->getChildByName("plus_button");
    plusButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickPlusTicket, this));
    
    auto closeButton = (Button*)ticketNode->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickCloseBuy, this));
    auto buyButton = (Button*)ticketNode->getChildByName("bg_img")->getChildByName("buy_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(ProvLayer::clickBuyTicket, this));
}

void ProvLayer::clickReduceTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyTicketCount--;
    buyTicketCount = MAX(1, buyTicketCount);
    buyTicketCountText->setString(Convert2String(buyTicketCount));
    buyTicketPriceText->setString(Convert2String(provTicketPrice*buyTicketCount));
}

void ProvLayer::clickPlusTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyTicketCount++;
    buyTicketCountText->setString(Convert2String(buyTicketCount));
    buyTicketPriceText->setString(Convert2String(provTicketPrice*buyTicketCount));
}

void ProvLayer::clickCloseBuy()
{
    ticketAction->play("out", false);
    ticketAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(ProvLayer::removeBuyTicket, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void ProvLayer::removeBuyTicket()
{
    removeChild(ticketLayer);
}

void ProvLayer::clickBuyTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_prov_buy_ticket;
    info.value = buyTicketCount;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ProvLayer::addTicket(cocos2d::EventCustom *event)
{
    L2E_PROV_ADD_TICKET info = *static_cast<L2E_PROV_ADD_TICKET*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    ticketCountText->setString(Convert2String(info.ticketCount));
}

void ProvLayer::refreshReset(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    resetCountText->setString(Convert2String(info.value));
}

void ProvLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = PROV_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
