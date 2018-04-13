//
//  HallScene.cpp
//  life
//
//  Created by ff on 17/7/12.
//
//

#include "HallScene.h"
#include "SimpleAudioEngine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "../UIManager.h"
#include "utils/TimeUtil.h"
#include "utils/StringData.h"
#include "logic/ClientLogic.h"
#include "../layer/DialogLayer.h"
#include "../layer/ForceUplevelLayer.h"
#include "../node/TipsNode.h"
#include "../node/EffectNode.h"
#include "../layer/CustomPackageLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

int HallScene::autoPopLayerId = NONE_LAYER;
HallScene::HallScene():
plotButtonId(0),
plotMissionId(0),
unlockId(0)
{
    for (int i = 0; i < 11; i++) {
        lockNode[i] = nullptr;
        lockAction[i] = nullptr;
    }
}

HallScene::~HallScene()
{
    for (int i = 0; i < 11; i++) {
        lockNode[i] = nullptr;
        lockAction[i] = nullptr;
    }
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBattleListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showChapterListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showStageListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallHeroListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateDiamondListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateExpListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateForceListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallPetListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallPetOfferListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallHolySwordListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallArmsListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallProvListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallAbyssListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallTowerListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallArenaListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showArenaMatchListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showDialogListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(unlockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallCastleListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(changeJobListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallEverydayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallLuckyListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallShopListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showPackageDetailListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallNoviceListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showHallVIPListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateFreeDiamondListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateNameListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBuyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(buyVITListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showDiscountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateDiscountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showCustomPackageListener);
    
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}
void HallScene::onEnter()
{
    cocos2d::Scene::onEnter();
    log("HallScene::onEnter()");
    UIManager::instance()->replacing = false;
    showBattleListener = EventListenerCustom::create(SHOW_BATTLE_CHAPTERS, CC_CALLBACK_1(HallScene::showBattle, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBattleListener, -1);
    showChapterListener = EventListenerCustom::create(SHOW_CHAPTER, CC_CALLBACK_1(HallScene::showChapter, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showChapterListener, -1);
    showStageListener = EventListenerCustom::create(SHOW_STAGE, CC_CALLBACK_1(HallScene::showStage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showStageListener, -1);
    showHallHeroListener = EventListenerCustom::create(SHOW_HALL_HERO, CC_CALLBACK_1(HallScene::showHallHero, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallHeroListener, -1);
    updateGoldListener = EventListenerCustom::create(UPDATE_GOLD, CC_CALLBACK_1(HallScene::updateGold, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGoldListener, -1);
    updateDiamondListener = EventListenerCustom::create(UPDATE_DIAMOND, CC_CALLBACK_1(HallScene::updateDiamond, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateDiamondListener, -1);
    updateVITListener = EventListenerCustom::create(UPDATE_VIT, CC_CALLBACK_1(HallScene::updateVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateVITListener, -1);
    updateExpListener = EventListenerCustom::create(UPDATE_EXP, CC_CALLBACK_1(HallScene::updateExp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateExpListener, -1);
    updateForceListener = EventListenerCustom::create(UPDATE_FORCE, CC_CALLBACK_1(HallScene::updateForce, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateForceListener, -1);
    showHallPetListener = EventListenerCustom::create(SHOW_HALL_PET, CC_CALLBACK_1(HallScene::showHallPet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallPetListener, -1);
    showHallPetOfferListener = EventListenerCustom::create(SHOW_HALL_PET_OFFER, CC_CALLBACK_1(HallScene::showHallPetOffer, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallPetOfferListener, -1);
    showHallHolySwordListener = EventListenerCustom::create(SHOW_HALL_HOLY_SWORD, CC_CALLBACK_1(HallScene::showHallHolySword, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallHolySwordListener, -1);
    showHallArmsListener = EventListenerCustom::create(SHOW_HALL_ARMS, CC_CALLBACK_1(HallScene::showHallArms, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallArmsListener, -1);
    showHallProvListener = EventListenerCustom::create(SHOW_HALL_PROV, CC_CALLBACK_1(HallScene::showHallProv, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallProvListener, -1);
    showHallAbyssListener = EventListenerCustom::create(SHOW_HALL_ABYSS, CC_CALLBACK_1(HallScene::showHallAbyss, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallAbyssListener, -1);
    showHallTowerListener = EventListenerCustom::create(SHOW_HALL_TOWER, CC_CALLBACK_1(HallScene::showHallTower, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallTowerListener, -1);
    showHallArenaListener = EventListenerCustom::create(SHOW_HALL_ARENA, CC_CALLBACK_1(HallScene::showHallArena, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallArenaListener, -1);
    showArenaMatchListener = EventListenerCustom::create(ARENA_START_MATCH_OPP, CC_CALLBACK_1(HallScene::showArenaMatch, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showArenaMatchListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallScene::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    showDialogListener = EventListenerCustom::create(SHOW_DIALOG, CC_CALLBACK_1(HallScene::showDialog, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showDialogListener, -1);
    unlockListener = EventListenerCustom::create(UNLOCK_FUNC, CC_CALLBACK_1(HallScene::unlockFunc, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unlockListener, -1);
    showHallCastleListener = EventListenerCustom::create(SHOW_HALL_CASTLE, CC_CALLBACK_1(HallScene::showHallCastle, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallCastleListener, -1);
    changeJobListener = EventListenerCustom::create(CHANGE_JOB, CC_CALLBACK_1(HallScene::changeJob, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(changeJobListener, -1);
    showHallEverydayListener = EventListenerCustom::create(SHOW_HALL_EVERYDAY, CC_CALLBACK_1(HallScene::showHallEveryday, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallEverydayListener, -1);
    showHallLuckyListener = EventListenerCustom::create(SHOW_HALL_LUCKY, CC_CALLBACK_1(HallScene::showHallLucky, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallLuckyListener, -1);
    showHallShopListener = EventListenerCustom::create(SHOW_HALL_SHOP, CC_CALLBACK_1(HallScene::showHallShop, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallShopListener, -1);
    showPackageDetailListener = EventListenerCustom::create(SHOW_PACKAGE_DEATIL, CC_CALLBACK_1(HallScene::showPackageDetail, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showPackageDetailListener, -1);
    showHallNoviceListener = EventListenerCustom::create(SHOW_HALL_NOVICE, CC_CALLBACK_1(HallScene::showHallNovice, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallNoviceListener, -1);
    showHallVIPListener = EventListenerCustom::create(SHOW_HALL_VIP, CC_CALLBACK_1(HallScene::showHallVIP, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showHallVIPListener, -1);
    updateFreeDiamondListener = EventListenerCustom::create(UPDATE_FREE_DIAMOND, CC_CALLBACK_1(HallScene::updateFreeDiamond, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateFreeDiamondListener, -1);
    updateNameListener = EventListenerCustom::create(UPDATE_NAME, CC_CALLBACK_1(HallScene::updateName, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateNameListener, -1);
    showBuyVITListener = EventListenerCustom::create(SHOW_HALL_BUY_VIT, CC_CALLBACK_1(HallScene::showHallBuyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBuyVITListener, -1);
    buyVITListener = EventListenerCustom::create(BUY_VIT, CC_CALLBACK_1(HallScene::buyVIT, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buyVITListener, -1);
    updateTipListener = EventListenerCustom::create(UPDATE_HALL_TIP, CC_CALLBACK_1(HallScene::updateHallTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTipListener, -1);
    showDiscountListener = EventListenerCustom::create(SHOW_HALL_DISCOUNT, CC_CALLBACK_1(HallScene::showDiscount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showDiscountListener, -1);
    updateDiscountListener = EventListenerCustom::create(UPDATE_DISCOUNT_STATE, CC_CALLBACK_1(HallScene::updateDiscount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateDiscountListener, -1);
    showCustomPackageListener = EventListenerCustom::create(SHOW_CUSTOM_PACKAGE, CC_CALLBACK_1(HallScene::showCustomPackage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showCustomPackageListener, -1);
    
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "get.mp3").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "stone.mp3").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(GameUtils::format(SOUND_DIR, "up.mp3").c_str());
    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameUtils::format(SOUND_DIR, "bg.mp3").c_str());
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(GameUtils::format(SOUND_DIR, "bg.mp3").c_str(), true);
    if (discountEnable) {
        E2L_COMMON infoDiscount;
        infoDiscount.eProtocol = e2l_update_discount_state;
        ClientLogic::instance()->ProcessUIRequest(&infoDiscount);
    }
    
    auto listenerKeyPad = EventListenerKeyboard::create();
    listenerKeyPad->onKeyPressed = CC_CALLBACK_2(HallScene::onKeyReleased, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyPad, this);
    
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = HALL_SECEN;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

bool HallScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    bgSpt = Sprite::create("ui/home/zhuye.png");
    bgSpt->setScaleX(GameUtils::winSize.width/bgSpt->getContentSize().width);
    bgSpt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgSpt->setPosition(GameUtils::winSize/2);
    addChild(bgSpt);
    
    auto root = CSLoader::createNode(HOME_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    rootAction = CSLoader::createTimeline(HOME_UI);
    root->runAction(rootAction);
    
    
    auto leftRoot = CSLoader::createNode(HOME_LEFT_UI);
    auto leftAction = CSLoader::createTimeline(HOME_LEFT_UI);
    leftRoot->setPosition(Vec2(0, GameUtils::winSize.height/2));
    leftRoot->runAction(leftAction);
    addChild(leftRoot);
    leftAction->play("play", true);
    
    auto rightRoot = CSLoader::createNode(HOME_RIGHT_UI);
    auto rightAction = CSLoader::createTimeline(HOME_RIGHT_UI);
    rightRoot->setPosition(Vec2(GameUtils::winSize.width, GameUtils::winSize.height/2));
    rightRoot->runAction(rightAction);
    addChild(rightRoot);
    rightAction->play("play", true);
    
    auto vipButton = (Button*)leftRoot->getChildByName("vip_button");
    vipButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickVip, this));
    vipTip = (Sprite*)vipButton->getChildByName("tip");
    vipIcon = (ImageView*)vipButton->getChildByName("vip_icon");
    
    nameText = (Text*)leftRoot->getChildByName("name_bg")->getChildByName("name_text");
    forceText = (Text*)leftRoot->getChildByName("force_bg")->getChildByName("force_text");
    levelText = (Text*)leftRoot->getChildByName("job_bg")->getChildByName("level_bg")->getChildByName("level_text");
    headIcon = (ImageView*)leftRoot->getChildByName("job_bg")->getChildByName("head_icon_img");
    jobButton = (Button*)leftRoot->getChildByName("job_bg")->getChildByName("enter_button");
    jobButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickJob, this));
    
    battleBg = (ImageView*)root->getChildByName("battle_bg");
    battleButton = (Button*)battleBg->getChildByName("enter_button");
    battleButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickBattle, this));
    
    abyssButton = (Button*)rightRoot->getChildByName("abyss_bg")->getChildByName("enter_button");
    abyssButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickAbyss, this));
    abyssLockText = (Text*)rightRoot->getChildByName("abyss_bg")->getChildByName("lock_text");
    
    castleButton = (Button*)root->getChildByName("choose_bg")->getChildByName("enter_button");
    castleButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickCastle, this));
    castleLockText = (Text*)root->getChildByName("choose_bg")->getChildByName("lock_text");
    
    towerButton = (Button*)root->getChildByName("tower_bg")->getChildByName("enter_button");
    towerButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickTower, this));
    towerLockText = (Text*)root->getChildByName("tower_bg")->getChildByName("lock_text");
    
    pvpButton = (Button*)root->getChildByName("pvp_bg")->getChildByName("enter_button");
    pvpButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickPvp, this));
    pvpLockText = (Text*)root->getChildByName("pvp_bg")->getChildByName("lock_text");
    
    provButton = (Button*)rightRoot->getChildByName("prov_bg")->getChildByName("enter_button");
    provButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickProv, this));
    provLockText = (Text*)rightRoot->getChildByName("prov_bg")->getChildByName("lock_text");
    
    goldButton = (Button*)rightRoot->getChildByName("gold_bg")->getChildByName("enter_button");
    goldButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickGold, this));
    goldText = (Text*)rightRoot->getChildByName("gold_bg")->getChildByName("count_text");
    
    diamondButton = (Button*)rightRoot->getChildByName("diamond_bg")->getChildByName("enter_button");
    diamondButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickDiamond, this));
    diamondText = (Text*)rightRoot->getChildByName("diamond_bg")->getChildByName("count_text");
    
    vitButton = (Button*)rightRoot->getChildByName("VIT_bg")->getChildByName("enter_button");
    vitButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickVIT, this));
    VITText = (Text*)rightRoot->getChildByName("VIT_bg")->getChildByName("count_text");
    
    onlineButton = (Button*)rightRoot->getChildByName("online_button");
    onlineButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickOnline, this));
    onlineCountDownText = (Text*)onlineButton->getChildByName("countdown_img")->getChildByName("countdown_text");
    onlineDuration = 0;
    
    
    storeButton = (Button*)rightRoot->getChildByName("store_button");
    storeButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickStore, this));
    
    rewardButton = (Button*)rightRoot->getChildByName("reward_button");
    rewardButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickReward, this));
    rewardTip = (Sprite*)rewardButton->getChildByName("tip");
    
    soulButton = (Button*)rightRoot->getChildByName("soul_button");
    soulButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickSoul, this));
    soulTip = (Sprite*)soulButton->getChildByName("tip");
    
    monsterButton = (Button*)rightRoot->getChildByName("monster_button");
    monsterButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickMonster, this));
    monsterTip = (Sprite*)monsterButton->getChildByName("tip");
    
    heroButton = (Button*)rightRoot->getChildByName("hero_button");
    heroButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickHero, this));
    heroTip = (Sprite*)heroButton->getChildByName("tip");
    
    noviceButton = (Button*)rightRoot->getChildByName("novice_button");
    noviceButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickNovice, this));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    noviceTip = (Sprite*)noviceButton->getChildByName("tip");
#endif
    
    
    luckyButton = (Button*)rightRoot->getChildByName("lucky_button");
    luckyButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickLucky, this));
    luckyTip = (Sprite*)luckyButton->getChildByName("tip");
    
    firstButton = (Button*)rightRoot->getChildByName("first_button");
    firstButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickFirst, this));
    firstTip = (Sprite*)firstButton->getChildByName("tip");
    
    armsButton = (Button*)rightRoot->getChildByName("arm_button");
    armsButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickArms, this));
    armsTip = (Sprite*)armsButton->getChildByName("tip");
    
    discountButton = (Button*)leftRoot->getChildByName("discount_button");
    discountButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickDiscount, this));
    discountTip = (Sprite*)discountButton->getChildByName("tip");
    discountCDText = (Text*)discountButton->getChildByName("cd_text");
    discountTip->setVisible(false);
    discountCDText->setVisible(false);
    
    unlockEffect = CSLoader::createNode(UNLOCK_EFFECT_UI);
    unlockAction = CSLoader::createTimeline(UNLOCK_EFFECT_UI);
    unlockEffect->setPosition(GameUtils::winSize/2);
    unlockEffect->runAction(unlockAction);
    addChild(unlockEffect);
    unlockEffect->setVisible(false);
    
    getTipLayer = GetTipLayer::create();
    addChild(getTipLayer,10000);

    buyLayer = nullptr;
    discountEnable = false;
    discountSec = 0;
    plotButtonId = 0;
    plotMissionId = 0;
    unlockId = 0;
    autoPopLayerId = 0;
    scheduleUpdate();
    return true;
}



void HallScene::setupView(void *msg)
{
    L2E_SHOW_HALL info = *static_cast<L2E_SHOW_HALL*>(msg);
    autoPopLayerId = info.autoPopLayerId;
    vipTip->setVisible(info.tip[0]);
    
    nameText->setString(info.name);
    forceText->setString(Convert2String(info.totalForce));
    levelText->setString(Convert2String(info.level));
    headIcon->loadTexture(GameUtils::format(COMMON_DIR, info.headIcon.c_str()));

    battleButton->setEnabled(true);
    
    abyssLockText->setVisible(info.lock[2] != 0);
    abyssButton->setEnabled(!(info.lock[2] != 0));
    
    if (info.lock[2] != 0) {
        lockNode[2] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[2]));
        lockAction[2] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[2]));
        lockNode[2]->runAction(lockAction[2]);
        lockAction[2]->play("play1", true);
        lockNode[2]->setPosition(abyssButton->getContentSize()/2);
        abyssButton->addChild(lockNode[2]);
    }
    
    castleLockText->setVisible(info.lock[1] != 0);
    castleButton->setEnabled(!(info.lock[1] != 0));
    if (info.lock[1] != 0) {
        lockNode[1] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[1]));
        lockAction[1] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[1]));
        lockNode[1]->runAction(lockAction[1]);
        lockAction[1]->play("play1", true);
        lockNode[1]->setPosition(castleButton->getContentSize()/2);
        castleButton->addChild(lockNode[1]);
    }
    
    towerLockText->setVisible(info.lock[5] == 1);
    towerButton->setEnabled(!(info.lock[5] == 1));
    if (info.lock[5] != 0) {
        lockNode[5] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[5]));
        lockAction[5] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[5]));
        lockNode[5]->runAction(lockAction[5]);
        lockAction[5]->play("play1", true);
        lockNode[5]->setPosition(towerButton->getContentSize()/2);
        towerButton->addChild(lockNode[5]);
    }
    
    pvpLockText->setVisible(info.lock[8] != 0);
    pvpButton->setEnabled(!(info.lock[8] != 0));
    if (info.lock[8] != 0) {
        lockNode[8] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[8]));
        lockAction[8] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[8]));
        lockNode[8]->runAction(lockAction[8]);
        lockAction[8]->play("play1", true);
        lockNode[8]->setPosition(pvpButton->getContentSize()/2);
        pvpButton->addChild(lockNode[8]);
    }
    
    provLockText->setVisible(info.lock[7] != 0);
    provButton->setEnabled(!(info.lock[7] != 0));
    if (info.lock[7] != 0) {
        lockNode[7] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[7]));
        lockAction[7] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[7]));
        lockNode[7]->runAction(lockAction[7]);
        lockAction[7]->play("play1", true);
        lockNode[7]->setPosition(provButton->getContentSize()/2);
        provButton->addChild(lockNode[7]);
    }
    
    if (info.gold > 100000) {
        goldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.gold/10000).c_str());
    }else{
        goldText->setString(Convert2String(info.gold));
    }
    
    diamondText->setString(Convert2String(info.diamond));
    
    VITText->setString(GameUtils::format(VIT_STR, info.VIT, info.maxVIT));
    
    onlineDuration = 0;
    if (info.freeDiamondState == 0) {
        onlineDuration = info.onlineDuration;
        onlineCountDownText->setString(TimeUtil::timeFormatToMS(info.onlineDuration));
    }else if (info.freeDiamondState == 1) {
        onlineDuration = 0;
        onlineCountDownText->setString(StringData::shared()->stringFromKey("can_take_reward"));
    }else{
        onlineButton->setVisible(false);
    }
    
    
    rewardTip->setVisible(info.tip[5]);
    rewardButton->setEnabled(!(info.lock[10] != 0));
    if (info.lock[10] != 0) {
        lockNode[10] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[10]));
        lockAction[10] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[10]));
        lockNode[10]->runAction(lockAction[10]);
        lockAction[10]->play("play1", true);
        lockNode[10]->setPosition(rewardButton->getContentSize()/2);
        rewardButton->addChild(lockNode[10]);
    }
    
    soulTip->setVisible(info.tip[3]);
    soulButton->setEnabled(!(info.lock[6] != 0));
    if (info.lock[6] != 0) {
        lockNode[6] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[6]));
        lockAction[6] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[6]));
        lockNode[6]->runAction(lockAction[6]);
        lockAction[6]->play("play1", true);
        lockNode[6]->setPosition(soulButton->getContentSize()/2);
        soulButton->addChild(lockNode[6]);
    }
    
    monsterTip->setVisible(info.tip[2]);
    monsterButton->setEnabled(!(info.lock[3] != 0));
    if (info.lock[3] != 0) {
        lockNode[3] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[3]));
        lockAction[3] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[3]));
        lockNode[3]->runAction(lockAction[3]);
        lockAction[3]->play("play1", true);
        lockNode[3]->setPosition(monsterButton->getContentSize()/2);
        monsterButton->addChild(lockNode[3]);
    }
    
    heroTip->setVisible(info.tip[1]);
    heroButton->setEnabled(!(info.lock[0] != 0));
    if (info.lock[0] != 0) {
        lockNode[0] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[0]));
        lockAction[0] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[0]));
        lockNode[0]->runAction(lockAction[0]);
        lockAction[0]->play("play1", true);
        lockNode[0]->setPosition(heroButton->getContentSize()/2);
        heroButton->addChild(lockNode[0]);
    }
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (noviceTip != NULL) {
        noviceTip->setVisible(false);
    }
    
#endif
    
    luckyTip->setVisible(info.tip[6]);
    luckyButton->setEnabled(!(info.lock[9] != 0));
    if (info.lock[9] != 0) {
        lockNode[9] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[9]));
        lockAction[9] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[9]));
        lockNode[9]->runAction(lockAction[9]);
        lockAction[9]->play("play1", true);
        lockNode[9]->setPosition(luckyButton->getContentSize()/2);
        luckyButton->addChild(lockNode[9]);
    }
    
    
    firstTip->setVisible(info.tip[7]);
    
    armsTip->setVisible(info.tip[4]);
    armsButton->setEnabled(!(info.lock[4] != 0));
    if (info.lock[4] != 0) {
        lockNode[4] = CSLoader::createNode(GameUtils::format(LOCK_UI, info.lockType[4]));
        lockAction[4] = CSLoader::createTimeline(GameUtils::format(LOCK_UI, info.lockType[4]));
        lockNode[4]->runAction(lockAction[4]);
        lockAction[4]->play("play1", true);
        lockNode[4]->setPosition(armsButton->getContentSize()/2);
        armsButton->addChild(lockNode[4]);
    }
    
    discountEnable = info.discountTip;
    discountTip->setVisible(discountEnable);
    discountCDText->setVisible(false);
    
    unlockEffect = CSLoader::createNode(UNLOCK_EFFECT_UI);
    unlockAction = CSLoader::createTimeline(UNLOCK_EFFECT_UI);
    unlockEffect->setPosition(GameUtils::winSize/2);
    unlockEffect->runAction(unlockAction);
    addChild(unlockEffect);
    unlockEffect->setVisible(false);
    
    getTipLayer = GetTipLayer::create();
    addChild(getTipLayer,10000);
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallScene::endIn, this));
}

void HallScene::update(float dt)
{
    UIManager::instance()->loop(dt);
    getTipLayer->update(dt);
    
    if (onlineDuration > 0) {
        onlineDuration -= dt;
        if (onlineDuration <= 0) {
            onlineDuration = 0;
            onlineCountDownText->setString(StringData::shared()->stringFromKey("ready_refresh"));
        }else{
            onlineCountDownText->setString(TimeUtil::timeFormatToMS(onlineDuration));
        }
    }
    
    if (discountSec > 0) {
        discountSec -= dt;
        if (discountSec <= 0) {
            discountSec = 0;
            discountCDText->setString(StringData::shared()->stringFromKey("ready_close"));
        }else{
            discountCDText->setString(TimeUtil::timeFormatToMS(discountSec));
        }
    }
}

void HallScene::showBattle(cocos2d::EventCustom *event)
{
    auto battleLayer = BattleChapterLayer::create();
    battleLayer->setupView(event);
    addChild(battleLayer);
}

void HallScene::showChapter(cocos2d::EventCustom *event)
{
    auto chapterLayer = ChapterStagesLayer::create();
    chapterLayer->setupView(event);
    addChild(chapterLayer);
}

void HallScene::showStage(cocos2d::EventCustom *event)
{
    auto stageLayer = StageLayer::create();
    stageLayer->setupView(event);
    addChild(stageLayer);
}

void HallScene::showHallHero(cocos2d::EventCustom *event)
{
    auto heroLayer = HallHeroLayer::create();
    heroLayer->setupView(event);
    addChild(heroLayer);
}

void HallScene::showHallPet(cocos2d::EventCustom *event)
{
    auto petLayer = HallPetLayer::create();
    petLayer->setupView(event);
    addChild(petLayer);
}

void HallScene::showHallPetOffer(cocos2d::EventCustom *event)
{
    auto offerLayer = HallPetOfferLayer::create();
    offerLayer->setupView(event);
    addChild(offerLayer);
}
void HallScene::showHallHolySword(cocos2d::EventCustom *event)
{
    auto holySwordLayer = HallHolySwordLayer::create();
    holySwordLayer->setupView(event);
    addChild(holySwordLayer);
}
void HallScene::showHallArms(cocos2d::EventCustom *event)
{
    auto armsLayer = HallArmsLayer::create();
    armsLayer->setupView(event);
    addChild(armsLayer);
}

void HallScene::showHallProv(cocos2d::EventCustom *event)
{
    auto provLayer = ProvLayer::create();
    provLayer->setupView(event);
    addChild(provLayer);
}

void HallScene::showHallAbyss(cocos2d::EventCustom *event)
{
    auto abyssLayer = HallAbyssLayer::create();
    abyssLayer->setupView(event);
    addChild(abyssLayer);
}

void HallScene::showHallTower(cocos2d::EventCustom *event)
{
    auto towerLayer = HallTowerLayer::create();
    towerLayer->setupView(event);
    addChild(towerLayer);
}

void HallScene::showHallArena(cocos2d::EventCustom *event)
{
    auto arenaLayer = HallArenaLayer::create();
    arenaLayer->setupView(event);
    addChild(arenaLayer);
}

void HallScene::showHallCastle(cocos2d::EventCustom *event)
{
    auto castleLayer = HallCastleLayer::create();
    castleLayer->setupView(event);
    addChild(castleLayer);
}

void HallScene::showArenaMatch(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (info.value != 0) {
        return;
    }
    
    auto matchLayer = ArenaMatchOppLayer::create();
    addChild(matchLayer);
    autoPopLayerId = NONE_LAYER;
}

void HallScene::showHallEveryday(cocos2d::EventCustom *event)
{
    auto campaignLayer = CampaignLayer::create();
    campaignLayer->updateEveryday(event);
    addChild(campaignLayer);
}

void HallScene::showHallLucky(cocos2d::EventCustom *event)
{
    auto luckyLayer = HallLuckyLayer::create();
    luckyLayer->setupView(event);
    addChild(luckyLayer);
}

void HallScene::showHallShop(cocos2d::EventCustom *event)
{
    auto shopLayer = HallShopLayer::create();
    shopLayer->setupView(event);
    addChild(shopLayer);
}

void HallScene::showHallNovice(cocos2d::EventCustom *event)
{
    auto noviceLayer = HallNoviceLayer::create();
    noviceLayer->setupView(event);
    addChild(noviceLayer);
}

void HallScene::showHallVIP(cocos2d::EventCustom *event)
{
    auto vipLayer = HallVIPLayer::create();
    vipLayer->setupView(event);
    addChild(vipLayer);
}

void HallScene::showPackageDetail(cocos2d::EventCustom *event)
{
    auto packageLayer = PackageDetailLayer::create();
    packageLayer->setupView(event);
    addChild(packageLayer);
}

void HallScene::updateGold(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    if (info.value > 100000) {
        goldText->setString(GameUtils::format(StringData::shared()->stringFromKey("price_wan").c_str(), info.value/10000).c_str());
    }else{
        goldText->setString(Convert2String(info.value));
    }
    
}
void HallScene::updateDiamond(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    diamondText->setString(Convert2String(info.value));
}
void HallScene::updateVIT(cocos2d::EventCustom *event)
{
    L2E_UPDATE_VIT info = *static_cast<L2E_UPDATE_VIT*>(event->getUserData());
    VITText->setString(GameUtils::format("%d/%d", info.currVIT, info.maxVIT));
}

void HallScene::updateExp(cocos2d::EventCustom *event)
{
    L2E_UPDATE_EXP info = *static_cast<L2E_UPDATE_EXP*>(event->getUserData());
    levelText->setString(Convert2String(info.level));
    forceText->setString(Convert2String(info.force));
    
    if (info.upLevel) {
        SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "up.mp3").c_str(),false,1,0,0.5);
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
void HallScene::updateForce(cocos2d::EventCustom *event)
{
    L2E_UPDATE_FORCE info = *static_cast<L2E_UPDATE_FORCE*>(event->getUserData());
    forceText->setString(Convert2String(info.forceVal));
    if (info.upVal <= 0) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "up.mp3").c_str(),false,1,0,0.5);
    auto forceLayer = ForceUplevelLayer::create();
    forceLayer->showLayer(event);
    addChild(forceLayer);
}

void HallScene::updateName(cocos2d::EventCustom *event)
{
    L2E_UPDATE_NAME info = *static_cast<L2E_UPDATE_NAME*>(event->getUserData());
    nameText->setString(info.name);
}

void HallScene::updateFreeDiamond(cocos2d::EventCustom *event)
{
    L2E_UPDATE_FREE_DIAMOND info = *static_cast<L2E_UPDATE_FREE_DIAMOND*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey(GameUtils::format("online_err%d", info.errNo));
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
    }
    
    if (info.freeDiamondState == 0) {
        onlineDuration = info.onlineDuration;
        onlineCountDownText->setString(TimeUtil::timeFormatToMS(info.onlineDuration));
    }else if (info.freeDiamondState == 1) {
        onlineDuration = 0;
        onlineCountDownText->setString(StringData::shared()->stringFromKey("can_take_reward"));
    }else{
        onlineButton->setVisible(false);
    }
}

void HallScene::updateHallTip(cocos2d::EventCustom *event)
{
    L2E_UPDATE_HALL_TIP info = *static_cast<L2E_UPDATE_HALL_TIP*>(event->getUserData());
    switch (info.tipIndex) {
        case 0:
        {
            vipTip->setVisible(info.tipValue);
        }
            break;
        case 1:
        {
            heroTip->setVisible(info.tipValue);
        }
            break;
        case 2:
        {
            monsterTip->setVisible(info.tipValue);
        }
            break;
        case 3:
        {
            soulTip->setVisible(info.tipValue);
        }
            break;
        case 4:
        {
            armsTip->setVisible(info.tipValue);
        }
            break;
        case 5:
        {
            rewardTip->setVisible(info.tipValue);
        }
            break;
        case 6:
        {
            luckyTip->setVisible(info.tipValue);
        }
            break;
        case 7:
        {
            firstTip->setVisible(info.tipValue);
        }
            break;
            
        default:
            break;
    }
}

void HallScene::showHallBuyVIT(cocos2d::EventCustom *event)
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
    closeButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickCloseBuyVIT, this));
    auto buyButton = (Button*)root->getChildByName("bg_img")->getChildByName("ok_button");
    buyButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickBuyVIT, this));
    autoPopLayerId = NONE_LAYER;
}

void HallScene::clickCloseBuyVIT()
{
    buyAction->play("out", false);
    buyAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallScene::removeBuy, this));
}

void HallScene::removeBuy()
{
    removeChild(buyLayer);
    buyLayer = nullptr;
}

void HallScene::clickBuyVIT()
{
    E2L_COMMON info;
    info.eProtocol = e2l_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::buyVIT(cocos2d::EventCustom *event)
{
    if (buyLayer == nullptr) {
        return;
    }
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

void HallScene::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if (info.type == 3) {
        if(info.param[2] != HALL_SECEN)
        {
            return;
        }
        
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
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(battleBg->getContentSize()/2);
                point->setTag(100);
                battleBg->addChild(point);
            }
                break;
            case 2:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(heroButton->getContentSize()/2);
                point->setTag(100);
                heroButton->addChild(point);
            }
                break;
            case 3:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(monsterButton->getContentSize()/2);
                point->setTag(100);
                monsterButton->addChild(point);
            }
                break;
            case 4:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(armsButton->getContentSize()/2);
                point->setTag(100);
                armsButton->addChild(point);
            }
                break;
            case 5:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(soulButton->getContentSize()/2);
                point->setTag(100);
                soulButton->addChild(point);
            }
                break;
            case 6:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(rewardButton->getContentSize()/2);
                point->setTag(100);
                rewardButton->addChild(point);
            }
                break;
            case 7:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(abyssButton->getContentSize()/2);
                point->setTag(100);
                abyssButton->addChild(point);
            }
                break;
            case 8:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(towerButton->getContentSize()/2);
                point->setTag(100);
                towerButton->addChild(point);
            }
                break;
            case 9:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(provButton->getContentSize()/2);
                point->setTag(100);
                provButton->addChild(point);
            }
                break;
            case 10:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(pvpButton->getContentSize()/2);
                point->setTag(100);
                pvpButton->addChild(point);
            }
                break;
            case 11:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                luckyButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(luckyButton->getContentSize()/2);
                point->setTag(100);
                luckyButton->addChild(point);
            }
                break;
            case 12:
            {
                battleBg->removeChildByTag(100);
                heroButton->removeChildByTag(100);
                monsterButton->removeChildByTag(100);
                armsButton->removeChildByTag(100);
                soulButton->removeChildByTag(100);
                rewardButton->removeChildByTag(100);
                abyssButton->removeChildByTag(100);
                towerButton->removeChildByTag(100);
                provButton->removeChildByTag(100);
                pvpButton->removeChildByTag(100);
                luckyButton->removeChildByTag(100);
                castleButton->removeChildByTag(100);
                pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                pointTip->setPosition(Vec2(150, 35));
                point->setPosition(castleButton->getContentSize()/2);
                point->setTag(100);
                castleButton->addChild(point);
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
    }else if (info.type == 20) {
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = info.missionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    }
}
void HallScene::clickVip()
{
    if (plotButtonId != 0) {
        return;
    }
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
//    if (UIManager::instance()->replacing) {
//        return;
//    }
    plotButtonId = 0;
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_vip;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickJob()
{
    
}

void HallScene::clickBattle()
{
    log("HallScene::clickBattle >>>>>>>>>>>");
    if (autoPopLayerId != NONE_LAYER) {
        log("autoPopLayerId:%d", autoPopLayerId);
        log("Can't in battle >>>>>>>>>>>");
        return;
    }
//    if (UIManager::instance()->replacing) {
//        return;
//    }
    if (plotButtonId == 1) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = battleBg->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
//        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        log("plotButtonId:%d", plotButtonId);
        log("Can't in battle >>>>>>>>>>>");
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_battle;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickCastle()
{
    log("HallScene::clickCastle >>>>>>>>>>>");
    if (autoPopLayerId != NONE_LAYER) {
        log("autoPopLayerId:%d", autoPopLayerId);
        log("Can't in battle >>>>>>>>>>>");
        return;
    }
    if (plotButtonId == 12) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = castleButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        log("plotButtonId:%d", plotButtonId);
        log("Can't in battle >>>>>>>>>>>");
        return;
    }
    plotButtonId = 0;
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_castle;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickAbyss()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 7) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = abyssButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_abyss;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickTower()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 8) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = towerButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_tower;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickPvp()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 10) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = pvpButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_arena;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickProv()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 9) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = provButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_prov;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickGold()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_main_shop;
    info.value = 5;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickDiamond()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    clickStore();
}

void HallScene::clickVIT()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_show_hall_buy_vit;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickOnline()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_click_free_diamond;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickStore()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_click_main_shop;
    info.value = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickReward()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 6) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = rewardButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_pet_offer;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickSoul()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 5) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = soulButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_holy_sword;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickArms()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 4) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = armsButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_arms;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickMonster()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 3) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = monsterButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_pet;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickSkill()
{
    
}

void HallScene::clickHero()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 2) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = heroButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;

    E2L_COMMON info;
    info.eProtocol = e2l_click_main_hero;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickNovice()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_novice;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickLucky()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId == 11) {
        plotButtonId = 0;
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = plotMissionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        plotMissionId = 0;
        auto point = luckyButton->getChildByTag(100);
        point->stopAllActions();
        point->removeFromParent();
        //        battleBg->removeAllChildren();
    }else if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_lucky;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::clickSecret()
{
    
}

void HallScene::clickFirst()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_campaign;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::showDialog(cocos2d::EventCustom *event)
{
    auto dialogLayer = DialogLayer::create();
    dialogLayer->setupView(event);
    addChild(dialogLayer);
    autoPopLayerId = NONE_LAYER;
}

void HallScene::unlockFunc(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ONE_VALUE info = *static_cast<L2E_UPDATE_ONE_VALUE*>(event->getUserData());
    unlockId = info.value;
    unlockEffect->setVisible(true);
    auto icon = (ImageView*)unlockEffect->getChildByName("func_bg")->getChildByName("icon");
    icon->loadTexture(GameUtils::format(FUNC_ICON_DIR, GameUtils::format("function%d.png", info.value).c_str()));
    unlockAction->play("play", false);
    unlockAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(HallScene::endUnlockEffect, this));
}

void HallScene::endUnlockEffect()
{
    unlockEffect->setVisible(false);
    lockAction[unlockId-1]->play("play2", false);
    lockAction[unlockId-1]->setAnimationEndCallFunc("play2", CC_CALLBACK_0(HallScene::endUnlockFunc, this));
}

void HallScene::endUnlockFunc()
{
    switch (unlockId) {
        case 1:
        {
            heroButton->setEnabled(true);
        }
            break;
        case 2:
        {
            castleButton->setEnabled(true);
        }
            break;
        case 3:
        {
            abyssButton->setEnabled(true);
        }
            break;
        case 4:
        {
            monsterButton->setEnabled(true);
        }
            break;
        case 5:
        {
            armsButton->setEnabled(true);
        }
            break;
        case 6:
        {
            towerButton->setEnabled(true);
        }
            break;
        case 7:
        {
            soulButton->setEnabled(true);
        }
            break;
        case 8:
        {
            provButton->setEnabled(true);
        }
            break;
        case 9:
        {
            pvpButton->setEnabled(true);
        }
            break;
        case 10:
        {
            luckyButton->setEnabled(true);
        }
            break;
        case 11:
        {
            rewardButton->setEnabled(true);
        }
            break;
           
        default:
            break;
    }
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_trigger_unlock;
    info.value = unlockId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    unlockId = 0;
}

void HallScene::endIn()
{
    rootAction->play("play", true);
    
    E2L_COMMON info;
    info.eProtocol = e2l_hall_in;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    switch (autoPopLayerId) {
        case ABYSS_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_click_main_abyss;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case PVP_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_click_main_arena;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case TOWER_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_click_main_tower;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case BATTLE_CHAPTER_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_click_main_battle;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case CHAPTER_STAGE_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_back_chapter;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case STAGE_LAYER:
        {
            if (plotButtonId != 0) {
                return;
            }
            plotButtonId = 0;
            E2L_COMMON info;
            info.eProtocol = e2l_back_stage;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

void HallScene::changeJob(cocos2d::EventCustom *event)
{
    L2E_CHANGE_JOB info = *static_cast<L2E_CHANGE_JOB*>(event->getUserData());
    headIcon->loadTexture(GameUtils::format(COMMON_DIR, info.jobIcon.c_str()));
}

void HallScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    log("press keyCode :%d", keyCode);
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        log("press back!!!");
        E2L_COMMON info;
        info.eProtocol = e2l_exit;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}

void HallScene::updateDiscount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_DISCOUNT_STATE info = *static_cast<L2E_UPDATE_DISCOUNT_STATE*>(event->getUserData());
    if (info.state == 0) {
        discountSec = 0;
        discountEnable = false;
        discountCDText->setVisible(false);
        discountTip->setVisible(false);
    }else{
        discountSec = info.leftSec;
        discountEnable = true;
        discountCDText->setVisible(true);
        discountTip->setVisible(true);
    }
}

void HallScene::clickDiscount()
{
    if (autoPopLayerId != NONE_LAYER) {
        return;
    }
    if (plotButtonId != 0) {
        return;
    }
    plotButtonId = 0;
    
    E2L_COMMON info;
    info.eProtocol = e2l_click_main_discount;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::showDiscount(cocos2d::EventCustom *event)
{
    auto discountLayer = HallDiscountLayer::create();
    discountLayer->setupView(event);
    addChild(discountLayer);
}

void HallScene::showCustomPackage(cocos2d::EventCustom *event)
{
    auto packageLayer = CustomPackageLayer::create();
    packageLayer->setupView(event);
    addChild(packageLayer);
}
