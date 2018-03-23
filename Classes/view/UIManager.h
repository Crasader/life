//
//  UIManager.h
//  owen
//
//  Created by ff on 16/8/9.
//
//

#ifndef UIManager_h
#define UIManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"

class UIManager : public Singleton<UIManager>, public cocos2d::Ref
{
public:
    UIManager();
    
    void processLogicResponse(void *pMsg);
    void (UIManager::*m_Logic2EngineFunc[l2e_max])(void *pMsg);
    
    void loop(float dt);
private:
    void showLogin(void *pMsg);
    void updateNetState(void *pMsg);
    void showLoadStage(void *pMsg);
    void updateLoadStage(void *pMsg);
    void setupTmxMap(void *pMsg);
    void setupTmxEffect(void *pMsg);
    void setupHero(void *pMsg);
    void setupWitness(void *pMsg);
    void setupOpponent(void *pMsg);
    void setupMonsterTeam(void *pMsg);
    void showFight(void *pMsg);
    void unlockView(void *pMsg);
    void readyVictory(void *pMsg);
    void stageVictory(void *pMsg);
    void stageFail(void *pMsg);
    void showRevive(void *pMsg);
    void revive(void *pMsg);
    void stageTimeOver(void *pMsg);
    void stageStar(void *pMsg);
    void stageUpdateWaveCount(void *pMsg);
    void fullHeroHp(void *pMsg);
    void updateHpBottlePrice(void *pMsg);
    void updateCombo(void *pMsg);
    void clearCombo(void *pMsg);
    void showStageStar(void *pMsg);
    void pauseFight(void *pMsg);
    void resumeFight(void *pMsg);
    void preloadMonsterRes(void *pMsg);
    
    void showHallScene(void *pMsg);
//    void showChapterScene(void *pMsg);
    void showBattleChapters(void *pMsg);
    void back2BattleChapters(void *pMsg);
    void showChapterName(void *pMsg);
    void showChapter(void *pMsg);
    void showStage(void *pMsg);
    void startStage(void *pMsg);
    void openStarBox(void *pMsg);
    void showStarBounds(void *pMsg);
    
    void showHallHero(void *pMsg);
    void selEquipPos(void *pMsg);
    void updateEquipLevel(void *pMsg);
    void updateEquipStar(void *pMsg);
    void showItemWay(void *pMsg);
    void showItemSweep(void *pMsg);
    void updateItemCount(void *pMsg);
    void sweepItem(void *pMsg);
    void updateGold(void *pMsg);
    void updateDiamond(void *pMsg);
    void updateVIT(void *pMsg);
    void updateVITCD(void *pMsg);
    void updateExp(void *pMsg);
    void updateForce(void *pMsg);
    void updateHallTip(void *pMsg);
    void updateEquipTip(void *pMsg);
    
    void showSystemLoad(void *pMsg);
    void updateSystemLoad(void *pMsg);
    
    void showHallPet(void *pMsg);
    void selPetPos(void *pMsg);
    void updatePetLevel(void *pMsg);
    void updateFightPet(void *pMsg);
    void updatePetTip(void *pMsg);
    
    void showHallPetOffer(void *pMsg);
    void updateHallPetOfferAllCD(void *pMsg);
    void updatePetBoardOffer(void *pMsg);
    void openBoardOffer(void *pMsg);
    void addOfferPet(void *pMsg);
    void removeOfferPet(void *pMsg);
    void takeBoardOffer(void *pMsg);
    void workOfferOver(void *pMsg);
    void takeOfferReward(void *pMsg);
    void showBuyOffer(void *pMsg);
    void buyOffer(void *pMsg);
    void resetOfferRefreshCount(void *pMsg);
    
    void showHallHolySword(void *pMsg);
    void updateSword(void *pMsg);
    void updateFightSword(void *pMsg);
    void updateSwordLevel(void *pMsg);
    
    void showHallArms(void *pMsg);
    void updateArms(void *pMsg);
    void updateArmsLevel(void *pMsg);
    void updateGemCount(void *pMsg);
    void showFillGemBoard(void *pMsg);
    void updateArmsGem(void *pMsg);
    void updateArmsTip(void *pMsg);
    
    void showHallProv(void *pMsg);
    void confirmProvStage(void *pMsg);
    void confirmProvFighter(void *pMsg);
    void provFight(void *pMsg);
    void provWin(void *pMsg);
    void provLose(void *pMsg);
    void provReset(void *pMsg);
    void provAddTicket(void *pMsg);
    void provRefreshReset(void *pMsg);
    
    void showHallBuyVIT(void *pMsg);
    void showBuyVIT(void *pMsg);
    void buyVIT(void *pMsg);
    void updateVITBuyCount(void *pMsg);
    void showGetTip(void *pMsg);
    
    void showHallAbyss(void *pMsg);
    void checkAbyssStart(void *pMsg);
    void resetAbyss(void *pMsg);
    void showAbyssReset(void *pMsg);
    
    void showHallTower(void *pMsg);
    void showTowerSweep(void *pMsg);
    void resetTower(void *pMsg);
    void startTowerSweep(void *pMsg);
    void completeTowerSweep(void *pMsg);
    void towerTakeReward(void *pMsg);
    void speedupTowerSweep(void *pMsg);
    void updateTowerSweepCD(void *pMsg);
    
    void showHallArena(void *pMsg);
    void showArenaDivision(void *pMsg);
    void arenaStartMatch(void *pMsg);
    void arenaShowRobot(void *pMsg);
    void arenaBuyTicket(void *pMsg);
    void updateArenaTicket(void *pMsg);
    void updateArenaTIcketCD(void *pMsg);
    void arenaVictory(void *pMsg);
    void arenaFail(void *pMsg);
    void showArenaReward(void *pMsg);
    
    void startPlot(void *pMsg);
    void showDialog(void *pMsg);
    void updateDialog(void *pMsg);
    void completeDialog(void *pMsg);
    void completePlot(void *pMsg);
    void removeNPC(void *pMsg);
    void addNPC(void *pMsg);
    
    void unlockFunc(void *pMsg);
    
    void showHallCastle(void *pMsg);
    void showCastleJob(void *pMsg);
    void changeJob(void *pMsg);
    void activeJob(void *pMsg);
    void updateName(void *pMsg);
    
    void showHallEveryday(void *pMsg);
    void showEveryday(void *pMsg);
    void updateFirstRecharge(void *pMsg);
    void hideCampaign(void *pMsg);
    void showCampaignActive(void *pMsg);
    void updateCampaignActiveMission(void *pMsg);
    void updateCampaignActiveBox(void *pMsg);
    void refreshCampaignActive(void *pMsg);
    void updateFreeDiamond(void *pMsg);
    void showCampaignBoundMission(void *pMsg);
    void updateCampaignTip(void *pMsg);
    void jumpLocked(void *pMsg);
    
    void showHallLucky(void *pMsg);
    void showDrawNormalOne(void *pMsg);
    void showDrawSpecialOne(void *pMsg);
    void showDrawSpecialTen(void *pMsg);
    void updateFreeNormal(void *pMsg);
    void updateFreeSpecial(void *pMsg);
    void updateLuckyCD(void *pMsg);
    void updateLuckySpecialTicket(void *pMsg);
    
    void showHallShop(void *pMsg);
    void showShop(void *pMsg);
    void showPackageDetail(void *pMsg);
    void showPackageResult(void *pMsg);
    void showHallNovice(void *pMsg);
    
    void showHallVIP(void *pMsg);
    void showVIPDetail(void *pMsg);
    void takeVIPBound(void *pMsg);
    void hideVIP(void *pMsg);
    
    void updateDiscountState(void *pMsg);
    void showHallDiscount(void *pMsg);
public:
    bool replacing;
//    std::vector<void*> delayMsg;
};

#endif /* UIManager_h */
