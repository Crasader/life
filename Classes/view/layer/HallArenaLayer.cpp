//
//  HallArenaLayer.cpp
//  life
//
//  Created by ff on 17/9/21.
//
//

#include "HallArenaLayer.h"
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
using namespace cocostudio;
using namespace CocosDenshion;


HallArenaLayer::HallArenaLayer():
cd(0)
{
    divisionLayer = nullptr;
    buyLayer = nullptr;
    rewardLayer = nullptr;
}

HallArenaLayer::~HallArenaLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showDivisionListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startMatchListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyTicketListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTicketListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTicketCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showRewardListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallArenaLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_ARENA_UI);
    rootAction = CSLoader::createTimeline(HALL_ARENA_UI);
    
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickClose, this));

    auto rightBg = (ImageView*)rootBg->getChildByName("right_bg");
    auto startButton = (Button*)rightBg->getChildByName("start_button");
    startButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickStart, this));
    divisionButton = (Button*)rightBg->getChildByName("division_button");
    divisionButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickDivistion, this));
    divisionIcon = (ImageView*)rightBg->getChildByName("division_icon");
    divisionIcon2 = (ImageView*)divisionIcon->getChildByName("division_icon2");
    ticketText = (Text*)rightBg->getChildByName("ticket_text");
    cdText = (Text*)rightBg->getChildByName("cd_text");
    addButton = (Button*)rightBg->getChildByName("add_button");
    addButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickAddTicket, this));
    
    placeText = (Text*)rightBg->getChildByName("place_bg")->getChildByName("place_text");
    scoreText = (Text*)rightBg->getChildByName("score_bg")->getChildByName("score_text");
    forceText = (Text*)rightBg->getChildByName("force_bg")->getChildByName("force_text");
    
    for (int i = 0; i < 3; i++) {
        petBg[i] = (ImageView*)rightBg->getChildByName(GameUtils::format("pet%d_bg", i+1));
        petIcon[i] = (ImageView*)petBg[i]->getChildByName("icon");
    }
    swordIcon = (ImageView*)rightBg->getChildByName("sword_bg")->getChildByName("icon");
    
    auto leftBg = (ImageView*)rootBg->getChildByName("left_bg");
    rankList = (ListView*)leftBg->getChildByName("rank_list");
    return true;
}

void HallArenaLayer::onEnter()
{
    Layer::onEnter();
    showDivisionListener = EventListenerCustom::create(SHOW_ARENA_DIVISION, CC_CALLBACK_1(HallArenaLayer::showDivision, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showDivisionListener, -1);
    startMatchListener = EventListenerCustom::create(ARENA_START_MATCH_OPP, CC_CALLBACK_1(HallArenaLayer::startMatchOpponent, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startMatchListener, -1);
    buyTicketListener = EventListenerCustom::create(BUY_ARENA_TICKET, CC_CALLBACK_1(HallArenaLayer::buyTicket, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyTicketListener, -1);
    updateTicketListener = EventListenerCustom::create(UPDATE_ARENA_TICKET, CC_CALLBACK_1(HallArenaLayer::updateTicket, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTicketListener, -1);
    updateTicketCDListener = EventListenerCustom::create(UPDATE_ARENA_TICKET_CD, CC_CALLBACK_1(HallArenaLayer::updateTicketCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTicketCDListener, -1);
    showRewardListener = EventListenerCustom::create(SHOW_ARENA_REWARD, CC_CALLBACK_1(HallArenaLayer::showReward, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showRewardListener, -1);
    scheduleUpdate();
}

void HallArenaLayer::update(float dt)
{
    if (cd > 0) {
        cd -= dt;
        cdText->setString(TimeUtil::timeFormatToMS(cd));
    }
}

void HallArenaLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_ARENA info = *static_cast<L2E_SHOW_HALL_ARENA*>(event->getUserData());
    placeText->setString(Convert2String(info.place));
    scoreText->setString(Convert2String(info.score));
    forceText->setString(Convert2String(info.force));
    ticketText->setString(GameUtils::format("%d/%d", info.ticketCount, info.maxTicketCount));
    cdText->setString(TimeUtil::timeFormatToMS(info.cd));
    if (info.ticketCount == info.maxTicketCount) {
        cdText->setVisible(false);
        addButton->setVisible(false);
    }
//    divisionButton->loadTextureNormal(GameUtils::format(ARENA_DIR, info.divisionIcon1.c_str()));
    divisionIcon->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon1.c_str()));
    divisionIcon2->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon2.c_str()));
    if (info.swordIcon != "") {
        swordIcon->loadTexture(GameUtils::format(COMMON_DIR, info.swordIcon.c_str()));
        swordIcon->setVisible(true);
    }else{
        swordIcon->setVisible(false);
    }
    
    for (int i = 0; i < 3; i++) {
        if (info.petIcon[i] != "") {
            petIcon[i]->loadTexture(GameUtils::format(COMMON_DIR, info.petIcon[i].c_str()));
            for (int j = 0; j < 5; j++) {
                auto star = (ImageView*)petBg[i]->getChildByTag(j+1);
                star->setVisible((j+1) <= info.petLevel[i]);
            }
        }else{
            petIcon[i]->setVisible(false);
            for (int j = 0; j < 5; j++) {
                auto star = (ImageView*)petBg[i]->getChildByTag(j+1);
                star->setVisible(false);
            }
        }
    }
    
    for (int i = 0; i < 20; i++) {
        auto nodeRoot = CSLoader::createNode(ARENA_RANK_NODE_UI);
        auto bgImg = (ImageView*)nodeRoot->getChildByName("bg_img");
        bgImg->retain();
        bgImg->removeFromParent();
        auto placeIcon = (ImageView*)bgImg->getChildByName("place_icon");
        auto placeText = (Text*)bgImg->getChildByName("place_text");
        auto jobIcon = (ImageView*)bgImg->getChildByName("job_bg")->getChildByName("job_icon");
        auto nameText = (Text*)bgImg->getChildByName("name_text");
        auto scoreText = (Text*)bgImg->getChildByName("score_text");
        auto levelAtlas = (TextAtlas*)bgImg->getChildByName("level_atlas");
        auto forceText = (Text*)bgImg->getChildByName("force_text");
        auto swordIcon = (ImageView*)bgImg->getChildByName("sword_bg")->getChildByName("icon");
        ImageView* petBg[3];
        for (int j = 0; j < 3; j++) {
            petBg[j] = (ImageView*)bgImg->getChildByName(GameUtils::format("pet%d_bg", j+1));
        }
        
        if (info.robotPlace[i] <= 3) {
            std::string iconFile = GameUtils::format("common_font_%d.png", info.robotPlace[i]);
            placeIcon->loadTexture(GameUtils::format(ARENA_DIR, iconFile.c_str()));
            placeIcon->setVisible(true);
            placeText->setVisible(false);
        }else{
            placeText->setString(Convert2String(info.robotPlace[i]));
            placeIcon->setVisible(false);
            placeText->setVisible(true);
        }
        jobIcon->loadTexture(GameUtils::format(COMMON_DIR, info.robotJobIcon[i].c_str()));
        nameText->setString(info.robotName[i]);
        scoreText->setString(Convert2String(info.robotScore[i]));
        levelAtlas->setString(Convert2String(info.robotLevel[i]));
        forceText->setString(Convert2String(info.robotForce[i]));
        if (info.robotSwordIcon[i] == "") {
            swordIcon->setVisible(false);
        }else{
            swordIcon->setVisible(true);
            swordIcon->loadTexture(GameUtils::format(COMMON_DIR, info.robotSwordIcon[i].c_str()));
        }
        for(int j = 0; j < 3; j++)
        {
            auto petIcon = (ImageView*)petBg[j]->getChildByName("icon");
            if (info.robotPetIcon[i][j] == "") {
                petIcon->setVisible(false);
                for (int k = 0; k < 5; k++) {
                    auto star = (ImageView*)petBg[j]->getChildByTag(k+1);
                    star->setVisible(false);
                }
            }else{
                petIcon->loadTexture(GameUtils::format(COMMON_DIR, info.robotPetIcon[i][j].c_str()));
                petIcon->setVisible(true);
                for (int k = 0; k < 5; k++) {
                    auto star = (ImageView*)petBg[j]->getChildByTag(k+1);
                    star->setVisible((k+1) <= info.robotPetLevel[i][j]);
                }
                
            }
            
        }
        rankList->addChild(bgImg);
        bgImg->release();
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallArenaLayer::endIn, this));
    
    if (info.rewardState > 0) {
        rewardLayer = LayerColor::create(Color4B(10, 10, 10, 127));
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(HallArenaLayer::onTouchReward, this);
        listener->setSwallowTouches(true);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, rewardLayer);
        
        addChild(rewardLayer);
        
        auto root = CSLoader::createNode(ARENA_REWARD_UI);
        rewardAction = CSLoader::createTimeline(ARENA_REWARD_UI);
        
        root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        root->setPosition(GameUtils::winSize/2);
        rewardLayer->addChild(root);
        root->runAction(rewardAction);
        
        auto goldText = (Text*)root->getChildByName("bg_img")->getChildByName("gold_count");
        goldText->setString(Convert2String(info.rewardGold));
        auto fameText = (Text*)root->getChildByName("bg_img")->getChildByName("fame_count");
        fameText->setString(Convert2String(info.rewardFame));
        auto icon1 = (ImageView*)root->getChildByName("bg_img")->getChildByName("division_icon1");
        icon1->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon1.c_str()));
        auto icon2 = (ImageView*)root->getChildByName("bg_img")->getChildByName("division_icon2");
        icon2->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon2.c_str()));
        
        
        rewardAction->play("in", false);
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}
void HallArenaLayer::showDivision(cocos2d::EventCustom *event)
{
    L2E_SHOW_ARENA_DIVISION info = *static_cast<L2E_SHOW_ARENA_DIVISION*>(event->getUserData());
    divisionLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, divisionLayer);
    
    addChild(divisionLayer);
    
    auto root = CSLoader::createNode(DIVISION_UI);
    divisionAction = CSLoader::createTimeline(DIVISION_UI);
    
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    divisionLayer->addChild(root);
    root->runAction(divisionAction);
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickCloseDivision, this));
    
    auto divisionList = (ListView*)root->getChildByName("bg_img")->getChildByName("division_list");
    divisionList->removeAllChildren();
    for (int i = 0; i < 30; i++) {
        auto root = CSLoader::createNode(DIVISION_NODE_UI);
        auto bgImg = (ImageView*)root->getChildByName("bg_img");
        auto goldText = (Text*)bgImg->getChildByName("gold_text");
        auto fameText = (Text*)bgImg->getChildByName("fame_text");
        auto minScoreText = (Text*)bgImg->getChildByName("min_score_text");
        auto maxScoreText = (Text*)bgImg->getChildByName("max_score_text");
        auto icon1 = (ImageView*)bgImg->getChildByName("division_icon1");
        auto icon2 = (ImageView*)bgImg->getChildByName("division_icon2");
        
        bgImg->retain();
        bgImg->removeFromParent();
        
        goldText->setString(Convert2String(info.goldCount[i]));
        fameText->setString(Convert2String(info.fameCount[i]));
        icon1->loadTexture(GameUtils::format(ARENA_DIR, info.icon1[i].c_str()));
        icon2->loadTexture(GameUtils::format(ARENA_DIR, info.icon2[i].c_str()));
        minScoreText->setString(Convert2String(info.minScore[i]));
        int maxScore = info.maxScore[i];
        if (maxScore > 0) {
            maxScoreText->setVisible(true);
            maxScoreText->setString(Convert2String(maxScore));
        }else{
            maxScoreText->setVisible(false);
        }
        
        divisionList->addChild(bgImg);
        bgImg->release();
    }
    
    divisionAction->play("in", false);
}

void HallArenaLayer::startMatchOpponent(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if(info.value != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("arena_start_err%d", info.value));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOff, this));
}

void HallArenaLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallArenaLayer::clickCloseDivision()
{
    divisionAction->play("out", false);
    divisionAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOffDivision, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallArenaLayer::clickStart()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_arena_start;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArenaLayer::clickDivistion()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_arena_division;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArenaLayer::clickAddTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, buyLayer);
    
    addChild(buyLayer);
    
    auto root = CSLoader::createNode(ABYSS_RESET_UI);
    buyAction = CSLoader::createTimeline(ABYSS_RESET_UI);
    
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    buyLayer->addChild(root);
    root->runAction(buyAction);
    
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickCloseBuy, this));
    auto priceText = (Text*)root->getChildByName("bg_img")->getChildByName("price_text");
    priceText->setString(Convert2String(arenaTicketPrice));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("buy_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(HallArenaLayer::clickBuyTicket, this));
    
    auto buyCountText = (Text*)root->getChildByName("bg_img")->getChildByName("count_text");
    auto buyCountText1 = (Text*)root->getChildByName("bg_img")->getChildByName("Text_4_0_0");
    buyCountText->setVisible(false);
    buyCountText1->setVisible(false);
    
    buyAction->play("in", false);
}

void HallArenaLayer::buyTicket(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if(info.value != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOffBuy, this));
}

void HallArenaLayer::updateTicket(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ARENA_TICKET info = *static_cast<L2E_UPDATE_ARENA_TICKET*>(event->getUserData());
    ticketText->setString(GameUtils::format("%d/%d", info.currCount, info.maxCount));
    if (info.currCount == info.maxCount) {
        cdText->setVisible(false);
        addButton->setVisible(false);
    }else{
        cdText->setVisible(true);
        addButton->setVisible(true);
//        cd = arenaTicketRestoreSec;
//        cdText->setString(TimeUtil::timeFormatToMS(cd));
    }
}

void HallArenaLayer::updateTicketCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (info.value < 0) {
        cdText->setVisible(false);
        addButton->setVisible(false);
    }else{
        cdText->setVisible(true);
        addButton->setVisible(true);
        cd = info.value;
        cdText->setString(TimeUtil::timeFormatToMS(info.value));
    }
}

void HallArenaLayer::clickCloseBuy()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOffBuy, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}


void HallArenaLayer::clickBuyTicket()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_add_arena_ticket;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArenaLayer::removeOff()
{
    removeFromParent();
}

void HallArenaLayer::removeOffDivision()
{
    removeChild(divisionLayer);
    divisionLayer = nullptr;
}

void HallArenaLayer::removeOffBuy()
{
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void HallArenaLayer::showReward(cocos2d::EventCustom *event)
{
    L2E_ARENA_REWARD info = *static_cast<L2E_ARENA_REWARD*>(event->getUserData());
    rewardLayer = LayerColor::create(Color4B(10, 10, 10, 127));
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, rewardLayer);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HallArenaLayer::onTouchReward, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, rewardLayer);
    
    addChild(rewardLayer);
    
    auto root = CSLoader::createNode(ARENA_REWARD_UI);
    rewardAction = CSLoader::createTimeline(ARENA_REWARD_UI);
    
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    rewardLayer->addChild(root);
    root->runAction(rewardAction);
    
    auto goldText = (Text*)root->getChildByName("bg_img")->getChildByName("gold_count");
    goldText->setString(Convert2String(info.gold));
    auto fameText = (Text*)root->getChildByName("bg_img")->getChildByName("fame_count");
    fameText->setString(Convert2String(info.fame));
    auto icon1 = (ImageView*)root->getChildByName("bg_img")->getChildByName("division_icon1");
    icon1->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon1.c_str()));
    auto icon2 = (ImageView*)root->getChildByName("bg_img")->getChildByName("division_icon2");
    icon2->loadTexture(GameUtils::format(ARENA_DIR, info.divisionIcon2.c_str()));
    

    rewardAction->play("in", false);
}

bool HallArenaLayer::onTouchReward(cocos2d::Touch *touch, cocos2d::Event *event)
{
    rewardAction->play("out", false);
    rewardAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArenaLayer::removeOffReward, this));
    return true;
}

void HallArenaLayer::removeOffReward()
{
    E2L_COMMON info;
    info.eProtocol = e2l_arena_take_reward;
    ClientLogic::instance()->ProcessUIRequest(&info);
    removeChild(rewardLayer);
    rewardLayer = nullptr;
}

void HallArenaLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_COMMON infoCD;
    infoCD.eProtocol = e2l_update_arena_cd;
    ClientLogic::instance()->ProcessUIRequest(&infoCD);
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = PVP_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
