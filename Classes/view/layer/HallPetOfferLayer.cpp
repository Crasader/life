//
//  HallPetOfferLayer.cpp
//  life
//
//  Created by ff on 17/8/3.
//
//

#include "HallPetOfferLayer.h"
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


HallPetOfferLayer::HallPetOfferLayer():
refreshCD(0)
{
}

void HallPetOfferLayer::onEnter()
{
    Layer::onEnter();
    updateAllCDListener = EventListenerCustom::create(UPDATE_HALL_OFFER_ALL_CD, CC_CALLBACK_1(HallPetOfferLayer::updateAllCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateAllCDListener, -1);
    updateBoardOfferListener = EventListenerCustom::create(UPDATE_PET_BOARD_OFFER, CC_CALLBACK_1(HallPetOfferLayer::updateBoardOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateBoardOfferListener, -1);
    openBoardOfferListener = EventListenerCustom::create(OPEN_PET_BOARD_OFFER, CC_CALLBACK_1(HallPetOfferLayer::openBoardOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(openBoardOfferListener, -1);
    addOfferPetListener = EventListenerCustom::create(ADD_OFFER_PET, CC_CALLBACK_1(HallPetOfferLayer::addOfferPet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addOfferPetListener, -1);
    removeOfferPetListener = EventListenerCustom::create(REMOVE_OFFER_PET, CC_CALLBACK_1(HallPetOfferLayer::removeOfferPet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(removeOfferPetListener, -1);
    workOfferOverListener = EventListenerCustom::create(WORK_OFFER_OVER, CC_CALLBACK_1(HallPetOfferLayer::workOfferOver, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(workOfferOverListener, -1);
    takeBoardOfferListener = EventListenerCustom::create(TAKE_BOARD_OFFER, CC_CALLBACK_1(HallPetOfferLayer::takeBoardOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeBoardOfferListener, -1);
    takeOfferRewardListener = EventListenerCustom::create(TAKE_OFFER_REWARD, CC_CALLBACK_1(HallPetOfferLayer::takeOfferReward, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeOfferRewardListener, -1);
    showBuyOfferListener = EventListenerCustom::create(SHOW_BUY_OFFER, CC_CALLBACK_1(HallPetOfferLayer::showBuyOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBuyOfferListener, -1);
    buyOfferListener = EventListenerCustom::create(BUY_OFFER, CC_CALLBACK_1(HallPetOfferLayer::buyOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyOfferListener, -1);
    resetOfferRefreshCountListener = EventListenerCustom::create(RESET_OFFER_REFRESH_COUNT, CC_CALLBACK_1(HallPetOfferLayer::resetOfferRefreshCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resetOfferRefreshCountListener, -1);
}
HallPetOfferLayer::~HallPetOfferLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateAllCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateBoardOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(openBoardOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addOfferPetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(removeOfferPetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(workOfferOverListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeBoardOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeOfferRewardListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBuyOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resetOfferRefreshCountListener);
    
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallPetOfferLayer::init()
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

    auto root = CSLoader::createNode(HALL_PET_OFFER_UI);
    rootAction = CSLoader::createTimeline(HALL_PET_OFFER_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);

    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallPetOfferLayer::clickClose, this));
    
    petsBg = (ImageView*)rootBg->getChildByName("pet_bg");
    workPetsBg = (ImageView*)rootBg->getChildByName("work_pet_bg");
    auto boardBg = (ImageView*)rootBg->getChildByName("board_img");
    auto workBg = (ImageView*)rootBg->getChildByName("work_bg");
    
    for (int i = 0; i < 3; i++) {
        auto workIcon = (ImageView*)workBg->getChildByTag(i+1);
        workCDText[i] = (Text*)workIcon->getChildByName("cd_text");
        workSelButton[i] = (Button*)workIcon->getChildByName("sel_button");
        workSelButton[i]->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickWorkOffer, this));
    }
    
    boardRefreshCDText = (Text*)boardBg->getChildByName("refresh_cd_text");
    auto refreshButton = (Button*)boardBg->getChildByName("refresh_button");
    refreshButton->addClickEventListener(CC_CALLBACK_0(HallPetOfferLayer::clickRefresh, this));
    for (int i = 0; i < 3; i++) {
        boardOfferBg[i] = (ImageView*)boardBg ->getChildByTag(i+1);
        auto selButton = (Button*)boardOfferBg[i] ->getChildByName("sel_button");
        selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickOpenOffer, this));
    }
    for (int i = 0; i < 10; i++) {
        auto selButton = (Button*)petsBg->getChildByTag(i+1);
        selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickSelPet, this));
    }
    for (int i = 0; i < 3; i++) {
        auto removeButton = (Button*)workPetsBg->getChildByTag(i+1)->getChildByName("remove_button");
        removeButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickRemovePet, this));
    }
    refreshPriceText = (Text*)boardBg->getChildByName("refresh_price_text");
    buyOfferLayer = nullptr;
    buyOfferPos = 0;
    return true;
}

void HallPetOfferLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_PET_OFFER info = *static_cast<L2E_SHOW_HALL_PET_OFFER*>(event->getUserData());
    boardRefreshCDText->setVisible(false);
    for (int i = 0; i < 3; i++) {
        workCDText[i]->setVisible(false);
    }
    petsBg->setVisible(false);
    workPetsBg->setVisible(false);
    memset(petsState, 0, sizeof(int)*10);
    memset(readyPets, 0, sizeof(int)*3);
    openOfferId = 0;
    
    for (int i = 0; i < 3; i++) {
        if(info.workOfferType[i] == 0)
        {
            workSelButton[i]->setVisible(false);
        }else{
            workSelButton[i]->loadTextureNormal(GameUtils::format(OFFER_ICON, info.workOfferType[i]));
            if (info.workOfferState[i] == 2) {
                workCDText[i]->setString(StringData::shared()->stringFromKey("can_take_reward"));
                workCDText[i]->setVisible(true);
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        auto icon = (ImageView*)boardOfferBg[i]->getChildByName("type_img");
        icon->loadTexture(GameUtils::format(OFFER_ICON, info.boardOfferType[i]));
        auto costHourText = (Text*)boardOfferBg[i]->getChildByName("cost_time_text");
        costHourText->setString(GameUtils::format(StringData::shared()->stringFromKey("offer_hours").c_str(), info.boardOfferCostTime[i]));
        auto takeFlag = (Text*)boardOfferBg[i]->getChildByName("take_text");
        auto selButton = (Button*)boardOfferBg[i] ->getChildByName("sel_button");
        selButton->setVisible(info.boardOfferState[i] == 0);
//        if (info.boardOfferState[i] == 0) {
        selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu1.png"));
        selButton->setTitleText(StringData::shared()->stringFromKey("open_offer"));
        selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickOpenOffer, this));
//        }else{
//            selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu2.png"));
//            selButton->setTitleText(StringData::shared()->stringFromKey("accept_offer"));
//            selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickTakeOffer, this));
//        }
        costHourText->setVisible(info.boardOfferState[i] == 0);
        takeFlag->setVisible(info.boardOfferState[i] != 0);
        
        for (int j = 0; j < 3; j++) {
            auto boundIcon = (ImageView*)boardOfferBg[i]->getChildByTag(j+1);
            if (info.boundIcons[i][j] == "") {
                boundIcon->setVisible(false);
            }else{
                boundIcon->loadTexture(GameUtils::format(COMMON_DIR, info.boundIcons[i][j].c_str()));
                boundIcon->setVisible(true);
                auto countText = (Text*)boundIcon->getChildByName("count_text");
                countText->setString(Convert2String(info.boundCount[i][j]));
                auto fragFlag = (ImageView*)boundIcon->getChildByName("frag_flag");
                fragFlag->setVisible(info.boundType[i][j] == 2);
            }
            
        }

    }
    
    refreshPriceText->setString(Convert2String(info.refreshBoardPrice));
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallPetOfferLayer::getAllCD, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);

}

void HallPetOfferLayer::getAllCD()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_COMMON info;
    info.eProtocol = e2l_update_all_offer_cd;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    E2L_TRIGGER_OPEN_LAYER infoTrigger;
    infoTrigger.eProtocol = e2l_trigger_open_layer;
    infoTrigger.index = PET_OFFER_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
}

void HallPetOfferLayer::updateAllCD(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ALL_OFFER_CD info = *static_cast<L2E_UPDATE_ALL_OFFER_CD*>(event->getUserData());
    refreshCD = info.refreshCD;
    for (int i = 0; i < 3; i++) {
        workOfferCD[i] = info.workOfferCD[i];
    }
    if (refreshCD >= 0) {
        boardRefreshCDText->setString(TimeUtil::timeFormatToHMS(refreshCD));
        boardRefreshCDText->setVisible(true);
    }
    for (int i = 0; i < 3; i++) {
        if (workOfferCD[i] > 0) {
            workCDText[i]->setString(TimeUtil::timeFormatToHMS(workOfferCD[i]));
            workCDText[i]->setVisible(true);
        }
    }
    scheduleUpdate();
}

void HallPetOfferLayer::updateBoardOffer(cocos2d::EventCustom *event)
{
    L2E_UPDATE_BOARD_OFFER info = *static_cast<L2E_UPDATE_BOARD_OFFER*>(event->getUserData());
    if (info.errNo == 1) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    if (info.errNo != 0) {
        return;
    }
    petsBg->setVisible(false);
    workPetsBg->setVisible(false);
    memset(petsState, 0, sizeof(int)*10);
    memset(readyPets, 0, sizeof(int)*3);
    openOfferId = 0;

    refreshPriceText->setString(Convert2String(info.refreshPrice));
    refreshCD = info.refreshCD;
    if (refreshCD >= 0) {
        boardRefreshCDText->setString(TimeUtil::timeFormatToHMS(refreshCD));
        boardRefreshCDText->setVisible(true);
    }
    
    for (int i = 0; i < 3; i++) {
        auto icon = (ImageView*)boardOfferBg[i]->getChildByName("type_img");
        icon->loadTexture(GameUtils::format(OFFER_ICON, info.boardOfferType[i]));
        auto costHourText = (Text*)boardOfferBg[i]->getChildByName("cost_time_text");
        costHourText->setString(GameUtils::format(StringData::shared()->stringFromKey("offer_hours").c_str(), info.boardOfferCostTime[i]));
        auto takeFlag = (Text*)boardOfferBg[i]->getChildByName("take_text");
        auto selButton = (Button*)boardOfferBg[i]->getChildByName("sel_button");
        selButton->setVisible(info.boardOfferState[i] == 0);
//        if (info.boardOfferState[i] == 0) {
        selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu1.png"));
        selButton->setTitleText(StringData::shared()->stringFromKey("open_offer"));
        selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickOpenOffer, this));
//        }else{
//            selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu2.png"));
//            selButton->setTitleText(StringData::shared()->stringFromKey("accept_offer"));
//            selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickTakeOffer, this));
//        }
        costHourText->setVisible(info.boardOfferState[i] == 0);
        takeFlag->setVisible(info.boardOfferState[i] != 0);
        for (int j = 0; j < 3; j++) {
            auto boundIcon = (ImageView*)boardOfferBg[i]->getChildByTag(j+1);
            if (info.boundIcons[i][j] == "") {
                boundIcon->setVisible(false);
            }else{
                boundIcon->loadTexture(GameUtils::format(COMMON_DIR, info.boundIcons[i][j].c_str()));
                boundIcon->setVisible(true);
                auto countText = (Text*)boundIcon->getChildByName("count_text");
                countText->setString(Convert2String(info.boundCount[i][j]));
                auto fragFlag = (ImageView*)boundIcon->getChildByName("frag_flag");
                fragFlag->setVisible(info.boundType[i][j] == 2);
            }
            
        }
    }
}

void HallPetOfferLayer::takeBoardOffer(cocos2d::EventCustom *event)
{
    L2E_TAKE_BOARD_OFFER info = *static_cast<L2E_TAKE_BOARD_OFFER*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("take_offer_err1");
        if (info.errNo == 4) {
            errStr = StringData::shared()->stringFromKey("take_offer_err2");
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    petsBg->setVisible(false);
    workPetsBg->setVisible(false);
    memset(petsState, 0, sizeof(int)*10);
    memset(readyPets, 0, sizeof(int)*3);
    openOfferId = 0;
    
    auto costHourText = (Text*)boardOfferBg[info.boardOfferPos-1]->getChildByName("cost_time_text");
    auto takeFlag = (Text*)boardOfferBg[info.boardOfferPos-1]->getChildByName("take_text");
    auto selButton = (Button*)boardOfferBg[info.boardOfferPos-1]->getChildByName("sel_button");
    selButton->setVisible(false);
    costHourText->setVisible(false);
    takeFlag->setVisible(true);

    workOfferCD[info.workOfferPos-1] = info.workOfferCD;
    workSelButton[info.workOfferPos-1]->loadTextureNormal(GameUtils::format(OFFER_ICON, info.workOfferType));
    workCDText[info.workOfferPos-1]->setString(TimeUtil::timeFormatToHMS(info.workOfferCD));
    workSelButton[info.workOfferPos-1]->setVisible(true);

}

void HallPetOfferLayer::workOfferOver(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    workCDText[info.value-1]->setString(StringData::shared()->stringFromKey("can_take_reward"));
    workOfferCD[info.value-1] = 0;
    if (buyOfferLayer) {
        clickBuyClose();
    }
}

void HallPetOfferLayer::takeOfferReward(cocos2d::EventCustom *event)
{
    L2E_TAKE_OFFER_REWARD info = *static_cast<L2E_TAKE_OFFER_REWARD*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("take_reward_err");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;

    }
    workSelButton[info.workOfferPos-1]->setVisible(false);
    workCDText[info.workOfferPos-1]->setVisible(false);
    for (int i = 0; i < 10; i++) {
        auto petBg = (Button*)petsBg->getChildByTag(i+1);
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(false);
    }
    for (auto petId : info.workPet) {
        auto petBg = (ImageView*)petsBg->getChildByTag(petId);
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(true);
    }

}

void HallPetOfferLayer::buyOffer(cocos2d::EventCustom *event)
{
    L2E_BUY_OFFER info = *static_cast<L2E_BUY_OFFER*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr("");
        switch (info.errNo) {
            case 1:
            {
                errStr = StringData::shared()->stringFromKey("buy_offer_err1");
            }
                break;
            case 2:
            {
                errStr = StringData::shared()->stringFromKey("diamond_not_enough");
            }
                
            default:
                break;
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
        
    }

    clickBuyClose();
    workSelButton[info.workOfferPos-1]->setVisible(false);
    workCDText[info.workOfferPos-1]->setVisible(false);
    workOfferCD[info.workOfferPos-1] = 0;
    for (int i = 0; i < 10; i++) {
        auto petBg = (Button*)petsBg->getChildByTag(i+1);
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(false);
    }
    for (auto petId : info.workPet) {
        auto petBg = (ImageView*)petsBg->getChildByTag(petId);
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(true);
    }

}
void HallPetOfferLayer::openBoardOffer(cocos2d::EventCustom *event)
{
    L2E_OPEN_BOARD_OFFER info = *static_cast<L2E_OPEN_BOARD_OFFER*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("work_offer_full");
        if (info.errNo == 2) {
            errStr = StringData::shared()->stringFromKey("board_offer_invalid");
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);

        return;
    }
    
    for (int i = 0; i < 3; i++) {
        auto selButton = (Button*)boardOfferBg[i]->getChildByName("sel_button");
        if (info.offerPos-1 == i) {
            selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu2.png"));
            selButton->setTitleText(StringData::shared()->stringFromKey("accept_offer"));
            selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickTakeOffer, this));
        }else{
            selButton->loadTextureNormal(GameUtils::format(COMMON_DIR, "changanniu1.png"));
            selButton->setTitleText(StringData::shared()->stringFromKey("open_offer"));
            selButton->addClickEventListener(CC_CALLBACK_1(HallPetOfferLayer::clickOpenOffer, this));
        }
    }
    
    petsBg->setVisible(true);
    workPetsBg->setVisible(true);
    memset(petsState, 0, sizeof(int)*10);
    memset(readyPets, 0, sizeof(int)*3);
    openOfferId = info.offerId;
    readyCount = 0;
    needCount = info.needCount;
    
    for (int i = 0; i < 10; i++) {
        auto petBg = (Button*)petsBg->getChildByTag(i+1);
        petBg->loadTextureNormal(GameUtils::format(COMMON_DIR, info.petsIcon[i].c_str()));
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(false);
        auto ready = (ImageView*)petBg->getChildByName("ready");
        ready->setVisible(false);
        auto lock = (ImageView*)petBg->getChildByName("lock");
        lock->setVisible(!info.unlock[i]);
    }
    for (auto petId : info.workPet) {
        auto petBg = (ImageView*)petsBg->getChildByTag(petId);
        auto flag = (ImageView*)petBg->getChildByName("busy_flag");
        flag->setVisible(true);
    }
    
    for (int i = 0; i < 3; i++) {
        auto petBg = (ImageView*)workPetsBg->getChildByTag(i+1);
        petBg->setVisible(i<info.needCount);
        auto removeButton = (Button*)petBg->getChildByName("remove_button");
        removeButton->setVisible(false);
    }
    auto tipText = (Text*)petsBg->getChildByName("descript_text");
    std::string tipStr("");
    if (info.offerType == 4) {
        tipStr = GameUtils::format(StringData::shared()->stringFromKey("pet_relation_offer_tip").c_str(), info.needCount);
    }else{
        tipStr = GameUtils::format(StringData::shared()->stringFromKey("pet_offer_tip").c_str(), info.needCount);
    }
    tipText->setString(tipStr);
}
void HallPetOfferLayer::showBuyOffer(cocos2d::EventCustom *event)
{
    L2E_SHOW_BUY_OFFER info = *static_cast<L2E_SHOW_BUY_OFFER*>(event->getUserData());
    buyOfferLayer = (Layer*)CSLoader::createNode(HALL_BUY_OFFER_UI);
    buyAction = CSLoader::createTimeline(HALL_BUY_OFFER_UI);
    buyOfferLayer->runAction(buyAction);
    addChild(buyOfferLayer);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,buyOfferLayer);
    
    buyOfferCDText = (Text*)buyOfferLayer->getChildByName("bg_img")->getChildByName("cd_text");
    buyOfferPos = info.workOfferPos;
    auto closeButton = (Button*)buyOfferLayer->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallPetOfferLayer::clickBuyClose, this));
    auto okButton = (Button*)buyOfferLayer->getChildByName("bg_img")->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(HallPetOfferLayer::clickBuyOk, this));
    auto priceText = (Text*)okButton->getChildByName("price_text");
    priceText->setString(Convert2String(info.price));
    for (int i = 0; i < 3; i++) {
        auto petIcon = (ImageView*)buyOfferLayer->getChildByName("bg_img")->getChildByName(GameUtils::format("pet%d_img",i+1));
        if (info.petIcons[i] == "") {
            petIcon->setVisible(false);
        }else{
            petIcon->loadTexture(GameUtils::format(COMMON_DIR, info.petIcons[i].c_str()));
            petIcon->setVisible(true);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        auto boundIcon = (ImageView*)buyOfferLayer->getChildByName("bg_img")->getChildByName(GameUtils::format("bound%d_img",i+1));
        if (info.boundIcons[i] == "") {
            boundIcon->setVisible(false);
        }else{
            boundIcon->loadTexture(GameUtils::format(COMMON_DIR, info.boundIcons[i].c_str()));
            boundIcon->setVisible(true);
            auto countText = (Text*)boundIcon->getChildByName("count_text");
            countText->setString(Convert2String(info.boundCount[i]));
            auto fragFlag = (ImageView*)boundIcon->getChildByName("frag_flag");
            fragFlag->setVisible(info.boundType[i] == 2);
        }
        
    }
    
    auto icon = (ImageView*)buyOfferLayer->getChildByName("bg_img")->getChildByName("type_img");
    icon->loadTexture(GameUtils::format(OFFER_ICON, info.workOfferType));

    buyAction->play("in", false);
}
void HallPetOfferLayer::clickBuyClose()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallPetOfferLayer::removeOffBuy, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}
void HallPetOfferLayer::clickBuyOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_buy_offer;
    info.value = buyOfferPos;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
void HallPetOfferLayer::removeOffBuy()
{
    removeChild(buyOfferLayer);
    buyOfferLayer = nullptr;
    buyOfferPos = 0;
}
void HallPetOfferLayer::update(float dt)
{
    if (refreshCD > 0) {
        refreshCD -= dt;
        if (refreshCD <= 0) {
            boardRefreshCDText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            boardRefreshCDText->setString(TimeUtil::timeFormatToHMS(refreshCD));
        }
        boardRefreshCDText->setVisible(true);
        
    }
    for (int i = 0; i < 3; i++) {
        if (workOfferCD[i] > 0) {
            workOfferCD[i] -= dt;
            workCDText[i]->setString(TimeUtil::timeFormatToHMS(workOfferCD[i]));
            workCDText[i]->setVisible(true);
            if (buyOfferLayer && (buyOfferPos-1 == i)) {
                buyOfferCDText->setString(TimeUtil::timeFormatToHMS(workOfferCD[i]));
            }
        }
    }
}

void HallPetOfferLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallPetOfferLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallPetOfferLayer::removeOff()
{
    removeFromParent();
}

void HallPetOfferLayer::clickWorkOffer(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_take_offer_reward;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::clickOpenOffer(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_open_board_offer;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::clickTakeOffer(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_TAKE_BOARD_OFFER info;
    info.eProtocol = e2l_take_board_offer;
    memcpy(info.petsId, readyPets, sizeof(int)*3);
    info.offerPos = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::clickRefresh()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_click_refresh_offer;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::clickSelPet(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    
    if (petsState[tag-1] == 1) {
        removeReadyPet(tag);
        return;
    }
    
    if (readyCount >= needCount) {
        std::string errStr = StringData::shared()->stringFromKey("pet_count_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    E2L_ADD_OFFER_PET info;
    info.eProtocol = e2l_add_offer_pet;
    info.petId = tag;
    memcpy(info.readyPets, readyPets, sizeof(int)*3);
    info.offerId = openOfferId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::clickRemovePet(cocos2d::Ref *pSender)
{
    int tag = ((Button*)pSender)->getParent()->getTag();
    removeReadyPet(readyPets[tag-1]);
}

void HallPetOfferLayer::removeReadyPet(int petId)
{
    petsState[petId-1] = 0;
    
    
    auto petBg = (Button*)petsBg->getChildByTag(petId);
    auto ready = (ImageView*)petBg->getChildByName("ready");
    ready->setVisible(false);
    
    for (int i = 0; i < 3; i++) {
        auto petBg = (ImageView*)workPetsBg->getChildByTag(i+1);
        auto removeButton = (Button*)petBg->getChildByName("remove_button");
        if (readyPets[i] == petId) {
            removeButton->setVisible(false);
            readyPets[i] = 0;
            readyCount--;
        }
    }
    for (int i = 0; i < 10; i++) {
        auto petBg = (Button*)petsBg->getChildByTag(i+1);
        petBg->removeChildByTag(1000);
    }
    
    E2L_REMOVE_OFFER_PET info;
    info.eProtocol = e2l_remove_offer_pet;
    memcpy(info.readyPets, readyPets, sizeof(int)*3);
    info.offerId = openOfferId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallPetOfferLayer::removeOfferPet(cocos2d::EventCustom *event)
{
    L2E_REMOVE_OFFER_PET info = *static_cast<L2E_REMOVE_OFFER_PET*>(event->getUserData());
    for (auto petId : info.relationPets) {
        if (petsState[petId-1] == 1) {
            continue;
        }
        auto petBg = (Button*)petsBg->getChildByTag(petId);
        auto focusRoot = CSLoader::createNode(HALL_PET_RELATION_UI);
        auto focusAction = CSLoader::createTimeline(HALL_PET_RELATION_UI);
        focusRoot->runAction(focusAction);
        focusRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        focusRoot->setPosition(petBg->getContentSize()/2);
        focusRoot->setTag(1000);
        focusAction->play("play", true);
        petBg->addChild(focusRoot);
    }
}

void HallPetOfferLayer::addOfferPet(cocos2d::EventCustom *event)
{
    L2E_ADD_OFFER_PET info = *static_cast<L2E_ADD_OFFER_PET*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("add_offer_pet_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    
    for (int i = 0; i < 3; i++) {
        if (readyPets[i] == 0) {
            readyPets[i] = info.petId;
            readyCount++;
            auto petBg = (ImageView*)workPetsBg->getChildByTag(i+1);
            auto removeButton = (Button*)petBg->getChildByName("remove_button");
            removeButton->loadTextureNormal(GameUtils::format(COMMON_DIR, info.petIcon.c_str()));
            removeButton->setVisible(true);
            break;
        }
    }
    petsState[info.petId-1] = 1;
    auto petBg = (Button*)petsBg->getChildByTag(info.petId);
    auto ready = (ImageView*)petBg->getChildByName("ready");
    ready->setVisible(true);
    for (int i = 0; i < 10; i++) {
        auto petBg = (Button*)petsBg->getChildByTag(i+1);
        petBg->removeChildByTag(1000);
    }
    
    for (auto petId : info.relationPets) {
        if (petsState[petId-1] == 1) {
            continue;
        }
        auto petBg = (Button*)petsBg->getChildByTag(petId);
        auto focusRoot = CSLoader::createNode(HALL_PET_RELATION_UI);
        auto focusAction = CSLoader::createTimeline(HALL_PET_RELATION_UI);
        focusRoot->runAction(focusAction);
        focusRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        focusRoot->setPosition(petBg->getContentSize()/2);
        focusRoot->setTag(1000);
        focusAction->play("play", true);
        petBg->addChild(focusRoot);
    }
}

void HallPetOfferLayer::resetOfferRefreshCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    refreshPriceText->setString(Convert2String(info.value));
}
