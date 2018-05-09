//
//  HallScene.h
//  life
//
//  Created by ff on 17/7/12.
//
//

#ifndef HallScene_h
#define HallScene_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "../layer/BattleChapterLayer.h"
#include "../layer/ChapterStagesLayer.h"
#include "../layer/StageLayer.h"
#include "../layer/HallHeroLayer.h"
#include "../layer/HallPetLayer.h"
#include "../layer/HallPetOfferLayer.h"
#include "../layer/HallHolySwordLayer.h"
#include "../layer/HallArmsLayer.h"
#include "../layer/ProvLayer.h"
#include "../layer/GetTipLayer.h"
#include "../layer/HallAbyssLayer.h"
#include "../layer/HallTowerLayer.h"
#include "../layer/HallArenaLayer.h"
#include "../layer/ArenaMatchOppLayer.h"
#include "../layer/HallCastleLayer.h"
#include "../layer/CampaignLayer.h"
#include "../layer/HallLuckyLayer.h"
#include "../layer/HallShopLayer.h"
#include "../layer/PackageDetailLayer.h"
#include "../layer/HallNoviceLayer.h"
#include "../layer/HallVIPLayer.h"
#include "../layer/HallDiscountLayer.h"

class HallScene : public cocos2d::Scene {
public:
    HallScene();
    ~HallScene();
    
    bool init();
    void onEnter();
    void update(float dt);
    
    void setupView(void *msg);
    
    void showBattle(cocos2d::EventCustom *event);
    void showChapter(cocos2d::EventCustom *event);
    void showStage(cocos2d::EventCustom *event);
    void showHallHero(cocos2d::EventCustom *event);
    void updateGold(cocos2d::EventCustom *event);
    void updateDiamond(cocos2d::EventCustom *event);
    void updateVIT(cocos2d::EventCustom *event);
    void updateExp(cocos2d::EventCustom *event);
    void updateForce(cocos2d::EventCustom *event);
    void showHallPet(cocos2d::EventCustom *event);
    void showHallPetOffer(cocos2d::EventCustom *event);
    void showHallHolySword(cocos2d::EventCustom *event);
    void showHallArms(cocos2d::EventCustom *event);
    void showHallProv(cocos2d::EventCustom *event);
    void showHallAbyss(cocos2d::EventCustom *event);
    void showHallTower(cocos2d::EventCustom *event);
    void showHallArena(cocos2d::EventCustom *event);
    void showArenaMatch(cocos2d::EventCustom *event);
    void startPlot(cocos2d::EventCustom *event);
    void showDialog(cocos2d::EventCustom *event);
    void unlockFunc(cocos2d::EventCustom *event);
    void showHallCastle(cocos2d::EventCustom *event);
    void changeJob(cocos2d::EventCustom *event);
    void showHallEveryday(cocos2d::EventCustom *event);
    void showHallLucky(cocos2d::EventCustom *event);
    void showHallShop(cocos2d::EventCustom *event);
    void showPackageDetail(cocos2d::EventCustom *event);
    void showHallNovice(cocos2d::EventCustom *event);
    void showHallVIP(cocos2d::EventCustom *event);
    void updateFreeDiamond(cocos2d::EventCustom *event);
    void updateName(cocos2d::EventCustom *event);
    void showHallBuyVIT(cocos2d::EventCustom *event);
    void buyVIT(cocos2d::EventCustom *event);
    void updateHallTip(cocos2d::EventCustom *event);
    
    void showDiscount(cocos2d::EventCustom *event);
    void updateDiscount(cocos2d::EventCustom *event);
    
    void showCustomPackage(cocos2d::EventCustom *event);
//    static void setDelayEvent(cocos2d::EventCustom *event);
    
    void clickVip();
    void clickJob();
    void clickBattle();
    void clickAbyss();
    void clickTower();
    void clickPvp();
    void clickProv();
    void clickGold();
    void clickDiamond();
    void clickVIT();
    void clickOnline();
    
    void clickStore();
    void clickReward();
    void clickSoul();
    void clickMonster();
    void clickSkill();
    void clickHero();
    void clickNovice();
    void clickLucky();
    void clickSecret();
    void clickFirst();
    void clickArms();
    void clickCastle();
    void clickBuyVIT();
    void clickCloseBuyVIT();
    
    void clickDiscount();
    
    void endUnlockEffect();
    void endUnlockFunc();
    void endIn();
    
    void removeBuy();
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    CREATE_FUNC(HallScene);
//public:
//    static bool isRunning;
//    static std::string delayEventName;
//    static void* delayEventData;
public:
    static int autoPopLayerId;
private:
    cocos2d::EventListenerCustom *showBattleListener;
    cocos2d::EventListenerCustom *showChapterListener;
    cocos2d::EventListenerCustom *showStageListener;
    cocos2d::EventListenerCustom *showHallHeroListener;
    cocos2d::EventListenerCustom *updateGoldListener;
    cocos2d::EventListenerCustom *updateDiamondListener;
    cocos2d::EventListenerCustom *updateVITListener;
    cocos2d::EventListenerCustom *updateExpListener;
    cocos2d::EventListenerCustom *updateForceListener;
    cocos2d::EventListenerCustom *showHallPetListener;
    cocos2d::EventListenerCustom *showHallPetOfferListener;
    cocos2d::EventListenerCustom *showHallHolySwordListener;
    cocos2d::EventListenerCustom *showHallArmsListener;
    cocos2d::EventListenerCustom *showHallProvListener;
    cocos2d::EventListenerCustom *showHallAbyssListener;
    cocos2d::EventListenerCustom *showHallTowerListener;
    cocos2d::EventListenerCustom *showHallArenaListener;
    cocos2d::EventListenerCustom *showArenaMatchListener;
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *showDialogListener;
    cocos2d::EventListenerCustom *unlockListener;
    cocos2d::EventListenerCustom *showHallCastleListener;
    cocos2d::EventListenerCustom *changeJobListener;
    cocos2d::EventListenerCustom *showHallEverydayListener;
    cocos2d::EventListenerCustom *showHallLuckyListener;
    cocos2d::EventListenerCustom *showHallShopListener;
    cocos2d::EventListenerCustom *showPackageDetailListener;
    cocos2d::EventListenerCustom *showHallNoviceListener;
    cocos2d::EventListenerCustom *showHallVIPListener;
    cocos2d::EventListenerCustom *updateFreeDiamondListener;
    cocos2d::EventListenerCustom *updateNameListener;
    cocos2d::EventListenerCustom *showBuyVITListener;
    cocos2d::EventListenerCustom *buyVITListener;
    cocos2d::EventListenerCustom *updateTipListener;
    cocos2d::EventListenerCustom *updateDiscountListener;
    cocos2d::EventListenerCustom *showDiscountListener;
    cocos2d::EventListenerCustom *showCustomPackageListener;
    
    cocos2d::Sprite *bgSpt;
    cocos2d::Sprite *vipTip;
    cocos2d::ui::ImageView *vipIcon;
    cocos2d::ui::Text *forceText;
    cocos2d::ui::Text *levelText;
    cocos2d::ui::ImageView *headIcon;
//    cocos2d::ui::ImageView *jobTip;
    cocos2d::ui::Text *castleLockText;
    cocos2d::ui::Text *abyssLockText;
//    cocos2d::ui::ImageView *abyssLockImg;
    cocos2d::ui::Text *towerLockText;
    cocos2d::ui::ImageView *towerLockImg;
    cocos2d::ui::Text *pvpLockText;
    cocos2d::ui::ImageView *pvpLockImg;
    cocos2d::ui::Text *provLockText;
    cocos2d::ui::ImageView *provLockImg;
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *diamondText;
    cocos2d::ui::Text *VITText;
    cocos2d::ui::Text *onlineCountDownText;
    cocos2d::ui::Text *nameText;
    cocos2d::ui::Text *discountCDText;
    cocos2d::Sprite *rewardTip;
    cocos2d::Sprite *soulTip;
    cocos2d::Sprite *monsterTip;
    cocos2d::Sprite *skillTip;
    cocos2d::Sprite *heroTip;
    cocos2d::Sprite *armsTip;
    cocos2d::Sprite *firstTip;
    cocos2d::Sprite *luckyTip;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    cocos2d::Sprite *noviceTip;
#endif

    
    cocos2d::Sprite *discountTip;
    
    cocos2d::ui::ImageView *battleBg;
    cocos2d::ui::Button *goldButton;
    cocos2d::ui::Button *diamondButton;
    cocos2d::ui::Button *vitButton;
    cocos2d::ui::Button *jobButton;
    cocos2d::ui::Button *vipButton;
    cocos2d::ui::Button *storeButton;
    cocos2d::ui::Button *noviceButton;
    cocos2d::ui::Button *firstButton;
    cocos2d::ui::Button *battleButton;
    cocos2d::ui::Button *heroButton;
    cocos2d::ui::Button *abyssButton;
    cocos2d::ui::Button *towerButton;
    cocos2d::ui::Button *provButton;
    cocos2d::ui::Button *pvpButton;
    cocos2d::ui::Button *rewardButton;
    cocos2d::ui::Button *soulButton;
    cocos2d::ui::Button *monsterButton;
    cocos2d::ui::Button *luckyButton;
    cocos2d::ui::Button *armsButton;
    cocos2d::ui::Button *castleButton;
    cocos2d::ui::Button *onlineButton;
    cocos2d::ui::Button *discountButton;
    
    cocos2d::Node *lockNode[11];
    cocostudio::timeline::ActionTimeline *lockAction[11];
    cocos2d::Node *unlockEffect;
    cocostudio::timeline::ActionTimeline *unlockAction;
    cocostudio::timeline::ActionTimeline *rootAction;
    
    GetTipLayer *getTipLayer;
    cocos2d::LayerColor *buyLayer;
    cocostudio::timeline::ActionTimeline *buyAction;
    cocos2d::ui::Text *buyCountText;
    cocos2d::ui::Text *priceText;
    
    float onlineDuration;
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
    CC_SYNTHESIZE_READONLY(int, unlockId, UnlockId);
    CC_SYNTHESIZE_READONLY(bool, discountEnable, DiscountEnable);
    CC_SYNTHESIZE_READONLY(float, discountSec, DiscountSec);
};

#endif /* HallScene_h */
