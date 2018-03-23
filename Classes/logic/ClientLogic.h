//
//  ClientLogic.h
//  owen
//
//  Created by ff on 16/8/9.
//
//

#ifndef ClientLogic_h
#define ClientLogic_h


#include "ELProtocol.h"
#include "CSProtocol.h"
#include "TLProtocol.h"
#include "utils/GameUtils.h"

enum E_GAME_STATE
{
    GAME_STATE_NONE,
    GAME_STATE_INIT,
    GAME_STATE_LOGO,
    GAME_STATE_FIGHT,
//    GAME_STATE_CHAPTER,
    GAME_STATE_HALL,
    GAME_STATE_MAX,
};

class ClientLogic : public Singleton<ClientLogic>
{
public:
    ClientLogic();
    
    void mainLoop(float dt);
    
    void (ClientLogic::*m_UI2LogicFunc[e2l_max])(void *pMsg);
    void (ClientLogic::*m_Service2ClientFunc[s2c_max])(void *pMsg);
    void (ClientLogic::*m_Time2ClientFunc[t2l_max])(void *pMsg);
    void (ClientLogic::*m_LogicStateFunc[GAME_STATE_MAX])(float dt);
    
    void ChangeState(E_GAME_STATE eState);
    
    //向显示层发送结构体消息
    void pass2Engine(void *pMsg);
    //处理显示层请求，实现见ProcessUIRequest.cpp
    void ProcessUIRequest(void *pMsg);
    
    //向服务层发送结构体消息
    void pass2Service(void *pMsg);
    //处理服务层响应，实现见ProcessServerResponse.cpp
    void ProcessServiceResponse(void *pMsg);
    
    //向计时层发送结构体消息
    void pass2Time(void *pMsg);
    //处理计时层响应，实现见ProcessTimeResponse.cpp
    void ProcessTimeResponse(void *pMsg);

private:
    bool enterGameMode(E_GAME_STATE next);
    void exitGameMode(E_GAME_STATE currMode);
    void onStateFight(float dt);
    void onStateHall(float dt);
    void onStateInit(float dt);

    void ResCheckConnect(void *pMsg);
    void ResPay(void *pMsg);
    void ResExit(void *pMsg);
    
    void UIClickLogin(void *pMsg);
    void UICheckNet(void *pMsg);
    void UIClickUnlock(void *pMsg);
    void FightMonsterHpEmpty(void *pMsg);
    void FightMonsterDie(void *pMsg);
    void FightStart(void *pMsg);
    void FightWitnessDie(void *pMsg);
    void FightHeroDie(void *pMsg);
    void FightTimeOver(void *pMsg);
    void FightTimeOver2(void *pMsg);
    void FightStar(void *pMsg);
    void FightWitnessArrive(void *pMsg);
    void FightUseHpBottle(void *pMsg);
    void FightAddCombo(void *pMsg);
    void FightClearCombo(void *pMsg);
    void FightWinClick(void *pMsg);
    void FightFailClick(void *pMsg);
    void FightPause(void *pMsg);
    void FightResume(void *pMsg);
    void FightNotRevive(void *pMsg);
    void FightRevive(void *pMsg);
    
    void UIClickChapterName(void *pMsg);
    void UIClickShowChapter(void *pMsg);
    void UIClickShowStage(void *pMsg);
    void UIClickStartStage(void *pMsg);
    void UIBack2Chapter(void *pMsg);
    void UIBack2BattleChapters(void *pMsg);
    void UIBack2Stage(void *pMsg);
    void UIClickStarBox(void *pMsg);
    
    void UIClickMainBattle(void *pMsg);
    void UIClickMainHero(void *pMsg);
    void UIClickMainPet(void *pMsg);
    void UIClickMainPetOffer(void *pMsg);
    void UIClickMainHolySword(void *pMsg);
    void UIClickMainArms(void *pMsg);
    void UIClickMainAbyss(void *pMsg);
    void UIClickMainTower(void *pMsg);
    void UIClickMainArena(void *pMsg);
    void UIClickMainCastle(void *pMsg);
    void UIClickMainCampaign(void *pMsg);
    void UIClickMainVIP(void *pMsg);
    
    void UISelEquipPos(void *pMsg);
    void UIClickEquipUplevel(void *pMsg);
    void UIClickEquipUpgrade(void *pMsg);
    void UIClickEquipItem(void *pMsg);
    void UIBuyEquipItem(void *pMsg);
    void UIShowSweepEquipItem(void *pMsg);
    void UIStartSweepEquipItem(void *pMsg);
    
    void UISelPetPos(void *pMsg);
    void UIClickPetUplevel(void *pMsg);
    void UIAddFightPet(void *pMsg);
    void UIRemoveFightPet(void *pMsg);
    
    void UIUpdateAllOfferCD(void *pMsg);
    void UIRefreshBoardOffer(void *pMsg);
    void UIOpenBoardOffer(void *pMsg);
    void UIAddOfferPet(void *pMsg);
    void UIRemoveOfferPet(void *pMsg);
    void UITakeBoardOffer(void *pMsg);
    void UITakeOfferReward(void *pMsg);
    void UIBuyOffer(void *pMsg);
    
    void UISelSword(void *pMsg);
    void UISetFightSword(void *pMsg);
    void UIClickSwordUplevel(void *pMsg);
    
    void UIClickArms(void *pMsg);
    void UIClickArmsUplevel(void *pMsg);
    void UIClickAssembleGem(void *pMsg);
    void UIClickFillGem(void *pMsg);
    void UIClickRemoveGem(void *pMsg);
    void UIShowFillGemBoard(void *pMsg);
    
    void UIClickProv(void *pMsg);
    void UIClickProvStart(void *pMsg);
    void UIProvConfirmFighter(void *pMsg);
    void UIProvFight(void *pMsg);
    void UIProvEndFight(void *pMsg);
    void UIProvEndWin(void *pMsg);
    void UIProvEndLose(void *pMsg);
    void UIProvReset(void *pMsg);
    void UIProvBuyTicket(void *pMsg);
    
    void UICheckStartAbyss(void *pMsg);
    void UIStartAbyss(void *pMsg);
    void UIResetAbyss(void *pMsg);
    void UIShowResetAbyss(void *pMsg);
    
    void UIShowTowerSweep(void *pMsg);
    void UIStartTower(void *pMsg);
    void UIResetTower(void *pMsg);
    void UIStartTowerSweep(void *pMsg);
    void UITowerTakeReward(void *pMsg);
    void UISpeedupTowerSweep(void *pMsg);
    void UIUpdateSweepCD(void *pMsg);
    
    void UIShowArenaDivision(void *pMsg);
    void UIArenaStartMatchOpp(void *pMsg);
    void UIArenaMatchRobot(void *pMsg);
    void UIArenaStartFight(void *pMsg);
    void UIArenaBuyTicket(void *pMsg);
    void UIArenaTakeReward(void *pMsg);
    void UIUpdateArenaCD(void *pMsg);
    
    void UIShowBuyVIT(void *pMsg);
    void UIBuyVIT(void *pMsg);
    void UIShowHallBuyVIT(void *pMsg);
    
    void UITriggerOpenLayer(void *pMsg);
    void UITriggerSetupHero(void *pMsg);
    void UITriggerPassNpc(void *pMsg);
    void UITriggerCreateMonster(void *pMsg);
    void UITriggerFightVictory(void *pMsg);
    void UITriggerClickButton(void *pMsg);
    void UITriggerUnlockFunc(void *pMsg);
    void UIPlotComplete(void *pMsg);
    void UINextDialog(void *pMsg);
    
    void UIHallIn(void *pMsg);
    
    void UIClickJob(void *pMsg);
    void UIChangeJob(void *pMsg);
    void UIActiveJob(void *pMsg);
    void UIChangeName(void *pMsg);
    
    void UIShowEveryday(void *pMsg);
    void UITakeEverydayBound(void *pMsg);
    void UIShowFirstRecharge(void *pMsg);
    void UITakeFirstRecharge(void *pMsg);
    void UIShowCampaignActive(void *pMsg);
    void UITakeCampaignActiveMissionBound(void *pMsg);
    void UIClickCampaignActiveBox(void *pMsg);
    void UIClickCampaignActiveJump(void *pMsg);
    void UIClickFreeDiamond(void *pMsg);
    void UIShowCampaignBoundMission(void *pMsg);
    void UIClickCampaignBoundJump(void *pMsg);
    void UITakeCampaignBoundReward(void *pMsg);
    
    void UIShowLucky(void *pMsg);
    void UIDrawNormal(void *pMsg);
    void UIDrawSpecial(void *pMsg);
    void UIDrawSpecialTen(void *pMsg);
    void UIGetLuckyCD(void *pMsg);
    void UIBuySpecialTicket(void *pMsg);
    
    void UIShowShop(void *pMsg);
    void UIClickShopButton(void *pMsg);
    void UIBuyShopPackage(void *pMsg);
    void UIShowShopPackageDetail(void *pMsg);
    void UIShowHallNovice(void *pMsg);
    void UIShowShopGold(void *pMsg);
    
    void UIShowVIPDetail(void *pMsg);
    void UITakeVIPBound(void *pMsg);
    
    void UIUpdateDiscountState(void *pMsg);
    void UIVIPJumpRecharge(void *pMsg);
    void UIExit(void *pMsg);
    
    void UIShowDiscount(void *pMsg);
    void UIGetVITCD(void *pMsg);
    
    void TimeRestoreVIT(void *pMsg);
    void TimeUpdateOfferAllCD(void *pMsg);
    void TimeRefreshBoardOffer(void *pMsg);
    void TimeWorkOffer1Over(void *pMsg);
    void TimeWorkOffer2Over(void *pMsg);
    void TimeWorkOffer3Over(void *pMsg);
    void TimeRefreshAbyss(void *pMsg);
    void TimeRefreshProvReset(void *pMsg);
    void TimeResetVITBuyCount(void *pMsg);
    void TimeCompleteTowerSweep(void *pMsg);
    void TimeRefreshTowerReset(void *pMsg);
    void TimeUpdateTowerSweepCD(void *pMsg);
    void TimeRestoreArenaTicket(void *pMsg);
    void TimeUpdateArenaTicketCD(void *pMsg);
    void TimeRefreshArenaReward(void *pMsg);
    void TimeRefreshEveryday(void *pMsg);
    void TimeRestoreDrawNormal(void *pMsg);
    void TimeRestoreDrawSpecial(void *pMsg);
    void TimeUpdateLuckyCD(void *pMsg);
    void TimeAddVipLoginDay(void *pMsg);
    void TimeRefreshCampaignActive(void *pMsg);
    void TimeResetFreeDiamond(void *pMsg);
    void TimeOpenDiscount(void *pMsg);
    void TimeCloseDiscount(void *pMsg);
    void TimeUpdateDiscountCloseSec(void *pMsg);
    void TimeUpdateVITCD(void *pMsg);
    void TimeResetPetOfferCount(void *pMsg);
    void test();
private:
    E_GAME_STATE m_eLastState;
    E_GAME_STATE m_eState;
    E_GAME_STATE cacheGameMode;

//    std::vector<std::function<void()>> delayFunc;
};

#endif /* ClientLogic_h */
