//
//  SystemLogic.h
//  life
//
//  Created by ff on 17/7/21.
//
//

#ifndef SystemLogic_h
#define SystemLogic_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "utils/RandomName.h"
#include "cocostudio/CocoStudio.h"
#include "ELProtocol.h"
#include "TLProtocol.h"
#include "CSProtocol.h"
#include "data/Account.h"
#include "data/Equip.h"
#include "data/VIT.h"
#include "data/Pet.h"
#include "data/PetOffer.h"
#include "data/HolySword.h"
#include "data/Arms.h"
#include "data/Proving.h"
#include "data/Abyss.h"
#include "data/Chapter.h"
#include "data/Tower.h"
#include "data/Arena.h"
#include "data/PlotMission.h"
#include "data/Plot.h"
#include "data/Dialog.h"
#include "data/Campaign.h"
#include "data/Lucky.h"
#include "data/Shop.h"
#include "data/VIP.h"
#include "Load.h"

class SystemLogic : public Singleton<SystemLogic>, public Load {
public:
    SystemLogic();
    ~SystemLogic();
    
    void logic(float dt);

    void arrangeLoad();
    void loadAccount();
    void loadEquip();
    void loadVIT();
    void loadPet();
    void loadPetOffer();
    void loadHolySword();
    void loadArms();
    void loadProving();
    void loadAbyss();
    void loadTower();
    void loadRandomNames();
    void loadArena();
    void loadPlotMission();
    void loadCampaign();
    void loadLucky();
    void loadShop();
    void loadVIP();
    void load();
    void showHall();
    void initArenaTopRobot();
    void matchArenaOpponent();
    
    void checkNet();
    void responseNetCheck(S2C_CHECK_CONNECT info);
    void showLogo();
    void showHallHero();
    void showHallPet();
    void showHallPetOffer();
    void showHallHolySword();
    void showHallArms();
    void showHallProv();
    void showHallAbyss();
    void showHallArena();
    void showHallCastle();
    void showHallShop(E2L_UPDATE_ONE_VALUE info);
    
    void restoreVIT(T2L_COMMON info);
    void selEquipPos(E2L_SEL_EQUIP_POS info);
    void selEquipItem(E2L_UPDATE_ONE_VALUE info);
    void buyEquipItem(E2L_BUY_EQUIP_ITEM info);
    void showSweepItem(E2L_UPDATE_ONE_VALUE info);
    void startSweepItem(E2L_START_ITEM_SWEEP info);
    void uplevelCurrEquip();
    void upgradeCurrEquip();
    
    void selPetPos(E2L_UPDATE_ONE_VALUE info);
    void uplevelCurrPet();
    void addFightPet(E2L_UPDATE_ONE_VALUE info);
    void removeFightPet(E2L_UPDATE_ONE_VALUE info);
    
    void getAllOfferCD();
    void updateAllOfferCD(T2L_UPDATE_OFFER_ALL_CD info);
    void refreshBoardOffer();
    void refreshOptBoardOffer();
    void openBoardOffer(E2L_UPDATE_ONE_VALUE info);
    void addOfferPet(E2L_ADD_OFFER_PET info);
    void removeOfferPet(E2L_REMOVE_OFFER_PET info);
    void takeBoardOffer(E2L_TAKE_BOARD_OFFER info);
    void workOfferOver(int pos);
    void takeOfferReward(E2L_UPDATE_ONE_VALUE info);
    void buyOffer(E2L_UPDATE_ONE_VALUE info);
    void resetOfferCount();
    
    void selSword(E2L_UPDATE_ONE_VALUE info);
    void setFightSword();
    void uplevelSword();
    
    void selArms(E2L_UPDATE_ONE_VALUE info);
    void uplevelArms();
    void assembleGem(E2L_UPDATE_ONE_VALUE info);
    void fillGem(E2L_UPDATE_ONE_VALUE info);
    void removeGem(E2L_UPDATE_ONE_VALUE info);
    void showFillGemBoard(E2L_UPDATE_ONE_VALUE info);
    
    void confirmProvStage();
    void confirmProvFighter();
    void provFight();
    void provEndFight();
    void provEndWin();
    void provEndLose();
    void provReset();
    void provBuyTicket(E2L_UPDATE_ONE_VALUE info);
    void provRefreshReset();
    
    void showArenaDivision();
    void startMatchOpp();
    void arenaMatchRobot();
    void restoreArenaTicket(T2L_COMMON info);
    void buyArenaTicket();
    void updateArenaTicketCD(T2L_COMMON info);
    void refreshArenaReward();
    void takeArenaReward();
    void updateArenaRank();
    void getArenaCD();
    
    void showBuyVIT();
    void buyVIT();
    void resetVITBuyCount();
    void showHallBuyVIT();
    
    void triggerOpenLayer(E2L_TRIGGER_OPEN_LAYER info);
    void triggerSetupHero();
    void triggerPassNpc(E2L_TRIGGER_PASS_NPC info);
    void triggerCreateMonster(E2L_TRIGGER_CREATE_MONSTER info);
    bool triggerKillMonster(E2L_TRIGGER_KILL_MONSTER info);
    void triggerFightVictory();
    void triggerClickButton(E2L_TRIGGER_CLICK_BUTTON info);
    void triggerUnlockFunc(E2L_UPDATE_ONE_VALUE info);
    void plotComplete(E2L_COMPLETE_PLOT info);
    void nextDialog();
    
    void HallIn();
    
    void showJob(E2L_UPDATE_ONE_VALUE info);
    void changeJob(E2L_UPDATE_ONE_VALUE info);
    void activeJob(E2L_UPDATE_ONE_VALUE info);
    void changeName(E2L_CHANGE_NAME info);
    
    void showEveryday();
    void showCampaignEveryday();
    void takeEverydayBound();
    void refreshEveryday();
    void showFirstRecharge();
    void takeFirstRecharge();
    void showCampaignActive();
    void takeCampaignActiveBound(E2L_UPDATE_ONE_VALUE info);
    void takeCampaignActiveBoxBound(E2L_UPDATE_ONE_VALUE info);
    void campaignActiveJump(E2L_UPDATE_ONE_VALUE info);
    void refreshCampaignActive();
    void resetFreeDiamond();
    void takeFreeDiamond();
    void showCampaignBoundMission();
    void takeCampaignMissionReward(E2L_UPDATE_ONE_VALUE info);
    void campaignMissionJump(E2L_UPDATE_ONE_VALUE info);
    
    void showLucky();
    void drawNormal();
    void drawSpecial();
    void drawSpecialTen();
    void restoreDrawNormal();
    void restoreDrawSpecial();
    void getLuckyCD();
    void updateLuckyCD(T2L_UPDATE_LUCKY_CD info);
    void buyLuckySpecialTicket(E2L_UPDATE_ONE_VALUE info);
    
    void showShop(E2L_UPDATE_ONE_VALUE info);
    void buyShopPackage(E2L_UPDATE_ONE_VALUE info);
    void buyRevive();
    void showShopPackageDetail(E2L_UPDATE_ONE_VALUE info);
    void responseBuy(S2C_PAY info);
    void takePackageBound(int packageId);
    void showNovice();
    
    void showVIP();
    void showVIPDetail(E2L_UPDATE_ONE_VALUE info);
    void uplevelVip(int originVip, int currVip);
    void addVipLoginDay();
    void takeVipBound();
    void vipJumpRecharge();
    
    void openDiscount();
    void closeDiscount();
    void getDiscountState();
    void updateDiscountState(long val = 0);
    void showDiscount();
    
    void getVITCD();
    void updateVITCD(T2L_COMMON infoTime);
    
    void clickExit();
    void exitApp();
protected:
    virtual void onLoadFinish();
private:
    Account *account;
    Equip *equip;
    VIT *vit;
    Pet *pet;
    PetOffer *petOffer;
    HolySword *sword;
    Arms *arms;
    Proving *prov;
    Abyss *abyss;
    Tower *tower;
    RandomName *randomName;
    Arena *arena;
    PlotMission *plotMission;
    Plot *plot;
    Dialog *dialog;
    Campaign *campaign;
    Lucky *lucky;
    Shop *shop;
    VIP *vip;
    
    CC_SYNTHESIZE_READONLY(ROBOT_DATA, robot, Robot);
    
    CC_SYNTHESIZE_READONLY(int, currEquipPos, CurrEquipPos);
    CC_SYNTHESIZE_READONLY(int, currPetId, CurrPetId);
    CC_SYNTHESIZE_READONLY(int, currSwordId, CurrSwordId);
    CC_SYNTHESIZE_READONLY(int, currArmsId, CurrArmsId);
    
    CC_SYNTHESIZE_READONLY(int, everydayShow, EverydayShow);
    CC_SYNTHESIZE(bool, toHall, ToHall);
    CC_SYNTHESIZE(int, popLayerId, PopLayerId);
};

#endif /* SystemLogic_h */
