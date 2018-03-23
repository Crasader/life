//
//  HallAbyssLayer.cpp
//  life
//
//  Created by ff on 17/9/8.
//
//

#include "HallAbyssLayer.h"
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


HallAbyssLayer::HallAbyssLayer():
start(false)
{
}

HallAbyssLayer::~HallAbyssLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(readyStartListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBuyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITBuyCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showAbyssResetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITCDListener);
    unscheduleUpdate();

    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

void HallAbyssLayer::onEnter()
{
    Layer::onEnter();
    readyStartListener = EventListenerCustom::create(CHECK_START_ABYSS, CC_CALLBACK_1(HallAbyssLayer::readyStart, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyStartListener, -1);
    resetListener = EventListenerCustom::create(RESET_ABYSS, CC_CALLBACK_1(HallAbyssLayer::reset, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resetListener, -1);
    showBuyVITListener = EventListenerCustom::create(SHOW_BUY_VIT, CC_CALLBACK_1(HallAbyssLayer::showBuyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBuyVITListener, -1);
    buyVITListener = EventListenerCustom::create(BUY_VIT, CC_CALLBACK_1(HallAbyssLayer::buyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyVITListener, -1);
    updateVITBuyCountListener = EventListenerCustom::create(UPDATE_VIT_BUY_COUNT, CC_CALLBACK_1(HallAbyssLayer::updateVITBuyCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITBuyCountListener, -1);
    updateVITListener = EventListenerCustom::create(UPDATE_VIT, CC_CALLBACK_1(HallAbyssLayer::updateVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITListener, -1);
    showAbyssResetListener = EventListenerCustom::create(SHOW_RESET_ABYSS, CC_CALLBACK_1(HallAbyssLayer::showAbyssReset, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showAbyssResetListener, -1);
    updateVITCDListener = EventListenerCustom::create(UPDATE_VIT_CD, CC_CALLBACK_1(HallAbyssLayer::updateVITCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITCDListener, -1);
    scheduleUpdate();
    
}

bool HallAbyssLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_ABYSS_UI);
    rootAction = CSLoader::createTimeline(HALL_ABYSS_UI);

    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickClose, this));
    stageList = (ListView*)rootBg->getChildByName("stage_list");
    auto VITBg = (ImageView*)rootBg->getChildByName("VIT_img");
    currVITText = (Text*)VITBg->getChildByName("curr_VIT_text");
    maxVITText = (Text*)VITBg->getChildByName("max_VIT_text");
    auto addVITButton = (Button*)VITBg->getChildByName("VIT_button");
    addVITButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickAddVIT, this));
    
    VITCDText = (Text*)rootBg->getChildByName("VIT_cd_text");
    VITCDTipText = (Text*)rootBg->getChildByName("VIT_cd_tip_text");

    
    vitCD = 0;

    return true;
}



void HallAbyssLayer::update(float dt)
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


void HallAbyssLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_ABYSS info = *static_cast<L2E_SHOW_HALL_ABYSS*>(event->getUserData());
    currVITText->setString(Convert2String(info.currVIT));
    maxVITText->setString(Convert2String(info.maxVIT));
    
    for (int i = 0; i < 10; i++) {
        auto abyssNode = CSLoader::createNode(ABYSS_NODE_UI);
        auto bgImg = (ImageView*)abyssNode->getChildByName("bg_img");
        bgImg->setTag(info.abyssId[i]);
        auto startButton = (Button*)bgImg->getChildByName("start_button");
        startButton->setSwallowTouches(false);
        startButton->addClickEventListener(CC_CALLBACK_1(HallAbyssLayer::clickStart, this));
        auto nameImg = (ImageView*)bgImg->getChildByName("name_img");
        auto mysteryImg = (ImageView*)bgImg->getChildByName("shimobg");
        mysteryImg->setVisible(i+1 > info.openAbyssCount);
        nameImg->setVisible(i+1 <= info.openAbyssCount);
        if (i+1 <= info.openAbyssCount) {
            nameImg->loadTexture(GameUtils::format(ABYSS_DIR, info.abyssNameImg[i].c_str()));
           
            auto platform = (Node*)bgImg->getChildByName("platform_node");
            
//            std::string resPath = GameUtils::format(ACTOR_DIR,
//                                                    info.abyssAnimation[i].c_str(),
//                                                    info.abyssAnimation[i].c_str());
            std::string resPath = GameUtils::format(ACTOR_DIR,
                                                    info.abyssAnimation[i].c_str());
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            auto armature = Armature::create(info.abyssAnimation[i]);
            armature->setScale(0.7);
            armature->getAnimation()->play("wait");
            //        armature->setPosition(platform->getContentSize()/2);
            platform->addChild(armature,10);
            
            auto resetButton = (Button*)nameImg->getChildByName("reset_button");
            resetButton->addClickEventListener(CC_CALLBACK_1(HallAbyssLayer::clickReset, this));
            resetButton->setVisible(info.fightCount[i]==0);
            auto fightCountText = (Text*)nameImg->getChildByName("fight_count_text");
            fightCountText->setString(Convert2String(info.fightCount[i]));
            auto petIcon = (ImageView*)nameImg->getChildByName("icon");
            petIcon->loadTexture(GameUtils::format(COMMON_DIR, info.petFragIcon[i].c_str()));
            auto petCount = (Text*)petIcon->getChildByName("count_text");
            petCount->setString(Convert2String(info.petFragCount[i]));
            auto goldText = (Text*)nameImg->getChildByName("gold_count");
            goldText->setString(Convert2String(info.boundGold[i]));
            auto expText = (Text*)nameImg->getChildByName("exp_count");
            expText->setString(Convert2String(info.boundExp[i]));
        }
        bgImg->retain();
        bgImg->removeFromParent();
        
        
        stageList->addChild(bgImg);
        bgImg->release();
    }
    
    if (info.focus <= 1) {
        stageList->jumpToLeft();
    }else{
        stageList->jumpToItem(info.focus-1, Vec2(0.5,0), Vec2::ANCHOR_MIDDLE);
//        float marginPercent = stageList->getItemsMargin()*100.0/stageList->getInnerContainerSize().width;
//        float actualTotal = 100.0-marginPercent*(stageList->getItems().size()-1);
//        float cardPercent = (info.focus-1.5)*actualTotal/stageList->getItems().size();
//        cardPercent += marginPercent*info.focus;
//        cardPercent = MAX(0, cardPercent);
//        cardPercent = MIN(100, cardPercent);
//        stageList->jumpToPercentHorizontal(cardPercent);
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallAbyssLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void HallAbyssLayer::readyStart(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if(info.value != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("start_abyss_err%d", info.value));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        start = false;
        return;
    }
    
    VITCDText->setVisible(false);
    VITCDTipText->setVisible(false);
    rootAction->play("start", false);
    rootAction->setAnimationEndCallFunc("start", CC_CALLBACK_0(HallAbyssLayer::endStart, this));
}

void HallAbyssLayer::reset(cocos2d::EventCustom *event)
{
    L2E_RESET_ABYSS info = *static_cast<L2E_RESET_ABYSS*>(event->getUserData());
    if(info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("reset_abyss_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    auto bgImg = (ImageView*)stageList->getChildByTag(info.abyssId);
    auto countText = (Text*)bgImg->getChildByName("name_img")->getChildByName("fight_count_text");
    countText->setString(Convert2String(info.fightCount));
    auto resetButton = (Button*)bgImg->getChildByName("name_img")->getChildByName("reset_button");
    resetButton->setVisible(false);
}

void HallAbyssLayer::showAbyssReset(cocos2d::EventCustom *event)
{
    L2E_SHOW_ABYSS_RESET info = *static_cast<L2E_SHOW_ABYSS_RESET*>(event->getUserData());
    resetLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, resetLayer);
    
    addChild(resetLayer,100);
    
    auto root = CSLoader::createNode(ABYSS_RESET_UI);
    buyResetAction = CSLoader::createTimeline(ABYSS_RESET_UI);
    root->runAction(buyResetAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    resetLayer->addChild(root);

    auto buyCountText = (Text*)root->getChildByName("bg_img")->getChildByName("count_text");
    buyCountText->setString(Convert2String(info.resetCount));
    auto priceText = (Text*)root->getChildByName("bg_img")->getChildByName("price_text");
    priceText->setString(Convert2String(info.price));
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickCloseBuyAbyss, this));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("buy_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickBuyAbyss, this));
    
    resetAbyssId = info.abyssId;
    buyResetAction->play("in", false);
}

void HallAbyssLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallAbyssLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallAbyssLayer::clickBuyAbyss()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    buyResetAction->play("out", false);
    buyResetAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallAbyssLayer::removeReset, this));
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_abyss_reset;
    info.value = resetAbyssId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallAbyssLayer::clickCloseBuyAbyss()
{
    buyResetAction->play("out", false);
    buyResetAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallAbyssLayer::removeReset, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallAbyssLayer::removeReset()
{
    this->removeChild(resetLayer);
    resetLayer = nullptr;
}

void HallAbyssLayer::clickReset(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_show_abyss_reset;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallAbyssLayer::clickStart(cocos2d::Ref *pSender)
{
    if (start) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    start = true;
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_abyss_start;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallAbyssLayer::clickAddVIT()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_show_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallAbyssLayer::endStart()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_confirm_start_abyss;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void HallAbyssLayer::removeOff()
{
    removeFromParent();
}

void HallAbyssLayer::showBuyVIT(cocos2d::EventCustom *event)
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
    closeButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickCloseBuyVIT, this));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("ok_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(HallAbyssLayer::clickBuyVIT, this));
}

void HallAbyssLayer::clickCloseBuyVIT()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallAbyssLayer::removeBuy, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallAbyssLayer::removeBuy()
{
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void HallAbyssLayer::clickBuyVIT()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallAbyssLayer::buyVIT(cocos2d::EventCustom *event)
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

void HallAbyssLayer::updateVITBuyCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (buyLayer) {
        buyCountText->setString(Convert2String(info.value));
    }
}

void HallAbyssLayer::updateVIT(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT info = *static_cast<L2E_UPDATE_VIT*>(event->getUserData());
    currVITText->setString(Convert2String(info.currVIT));
    maxVITText->setString(Convert2String(info.maxVIT));
}


void HallAbyssLayer::updateVITCD(cocos2d::EventCustom *event)
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


void HallAbyssLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    
    E2L_COMMON infocd;
    infocd.eProtocol = e2l_get_vit_cd;
    ClientLogic::instance()->ProcessUIRequest(&infocd);
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = ABYSS_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
