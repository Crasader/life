//
//  ProcessUIRequest.cpp
//  owen
//
//  Created by ff on 16/8/9.
//
//

#include "ClientLogic.h"
#include "GameCore.h"
#include "ChapterLogic.h"
#include "SystemLogic.h"

void ClientLogic::ProcessUIRequest(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    E2LProtocol	eProtocol	= *((E2LProtocol*)pMsg);
    if (eProtocol >= e2l_max)
        return;
    //分发到各个协议的处理函数
    if (this->m_UI2LogicFunc[eProtocol] != NULL)
    {
        (this->*m_UI2LogicFunc[eProtocol])(pMsg);
    }
}
void ClientLogic::UICheckNet(void *pMsg)
{
    SystemLogic::instance()->checkNet();
}
void ClientLogic::UIClickLogin(void *pMsg)
{
    ClientLogic::instance()->ChangeState(GAME_STATE_INIT);
}
void ClientLogic::UIClickUnlock(void *pMsg)
{
    GameCore::instance()->unlockView();
}

void ClientLogic::FightMonsterHpEmpty(void *pMsg)
{
    E2L_MONSTER_HP_EMPTY *pMsgInfo = (E2L_MONSTER_HP_EMPTY*)pMsg;
    GameCore::instance()->monsterHpEmpty(*pMsgInfo);
}

void ClientLogic::FightMonsterDie(void *pMsg)
{
    E2L_MONSTER_DIE *pMsgInfo = (E2L_MONSTER_DIE*)pMsg;
    GameCore::instance()->monsterDie(*pMsgInfo);
}
void ClientLogic::FightWitnessDie(void *pMsg)
{
    GameCore::instance()->witnessDie();
}
void ClientLogic::FightStart(void *pMsg)
{
    GameCore::instance()->startFight();
}

void ClientLogic::FightHeroDie(void *pMsg)
{
    GameCore::instance()->heroDie();
}

void ClientLogic::FightTimeOver(void *pMsg)
{
    GameCore::instance()->fightTimeOver();
}

void ClientLogic::FightTimeOver2(void *pMsg)
{
    E2L_FIGHT_TIME_OVER2 *pMsgInfo = (E2L_FIGHT_TIME_OVER2*)pMsg;
    GameCore::instance()->fightTimeOver2(*pMsgInfo);
}

void ClientLogic::FightStar(void *pMsg)
{
    E2L_FIGHT_STAR *pMsgInfo = (E2L_FIGHT_STAR*)pMsg;
    GameCore::instance()->fightStar(*pMsgInfo);
}

void ClientLogic::FightWitnessArrive(void *pMsg)
{
    GameCore::instance()->witnessArrive();
}
void ClientLogic::FightUseHpBottle(void *pMsg)
{
    GameCore::instance()->useHpBottle();
}

void ClientLogic::FightAddCombo(void *pMsg)
{
    E2L_ADD_COMBO *pMsgInfo = (E2L_ADD_COMBO*)pMsg;
    GameCore::instance()->addCombo(*pMsgInfo);
}

void ClientLogic::FightClearCombo(void *pMsg)
{
    GameCore::instance()->clearCombo();
}

void ClientLogic::UIClickChapterName(void *pMsg)
{
    E2L_CLICK_CHAPTER_NAME *pMsgInfo = (E2L_CLICK_CHAPTER_NAME*)pMsg;
    ChapterLogic::instance()->clickChapterName(*pMsgInfo);
}

void ClientLogic::UIClickShowChapter(void *pMsg)
{
    E2L_SHOW_CHAPTER *pMsgInfo = (E2L_SHOW_CHAPTER*)pMsg;
    ChapterLogic::instance()->showChapter(*pMsgInfo);
}

void ClientLogic::UIClickShowStage(void *pMsg)
{
    E2L_SHOW_STAGE *pMsgInfo = (E2L_SHOW_STAGE*)pMsg;
    ChapterLogic::instance()->showStage(*pMsgInfo);
}

void ClientLogic::UIClickStartStage(void *pMsg)
{
    ChapterLogic::instance()->startStage();
}

void ClientLogic::UIBack2Chapter(void *pMsg)
{
    ChapterLogic::instance()->back2Chapter();
}

void ClientLogic::UIBack2Stage(void *pMsg)
{
    ChapterLogic::instance()->back2Stage();
}

void ClientLogic::UIBack2BattleChapters(void *pMsg)
{
    ChapterLogic::instance()->back2Battle();
}

void ClientLogic::UIClickStarBox(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    ChapterLogic::instance()->openStarBox(*pMsgInfo);
}

void ClientLogic::FightWinClick(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    if (pMsgInfo->value == 0) {
        if (GameCore::instance()->getAbyssId() != 0) {
            SystemLogic::instance()->setPopLayerId(ABYSS_LAYER);
        }else if (GameCore::instance()->getAreanId() != 0){
            SystemLogic::instance()->setPopLayerId(PVP_LAYER);
        }else if (GameCore::instance()->getTowerId() != 0){
            SystemLogic::instance()->setPopLayerId(TOWER_LAYER);
        }else{
            SystemLogic::instance()->setPopLayerId(CHAPTER_STAGE_LAYER);
        }
        SystemLogic::instance()->setToHall(false);
    }else{
        SystemLogic::instance()->setPopLayerId(NONE_LAYER);
        SystemLogic::instance()->setToHall(true);
    }
    ClientLogic::instance()->ChangeState(GAME_STATE_HALL);

//    if (pMsgInfo->value == 0) {
//        if (GameCore::instance()->getAbyssId() != 0) {
//            delayFunc.push_back(CC_CALLBACK_0(SystemLogic::showHallAbyss, SystemLogic::instance()));
//        }else if (GameCore::instance()->getAreanId() != 0){
//            delayFunc.push_back(CC_CALLBACK_0(SystemLogic::showHallArena, SystemLogic::instance()));
//        }else if (GameCore::instance()->getTowerId() != 0){
//            delayFunc.push_back(CC_CALLBACK_0(ChapterLogic::showTower, ChapterLogic::instance()));
//        }else{
//            delayFunc.push_back(CC_CALLBACK_0(ChapterLogic::back2Chapter, ChapterLogic::instance()));
//        }
//        SystemLogic::instance()->setToHall(false);
//    }else{
//        SystemLogic::instance()->setToHall(true);
//    }
}
void ClientLogic::FightFailClick(void *pMsg)
{
    if (GameCore::instance()->getAbyssId() != 0) {
        SystemLogic::instance()->setPopLayerId(ABYSS_LAYER);
    }else if (GameCore::instance()->getAreanId() != 0){
        SystemLogic::instance()->setPopLayerId(PVP_LAYER);
    }else if (GameCore::instance()->getTowerId() != 0){
        SystemLogic::instance()->setPopLayerId(TOWER_LAYER);
    }else{
        SystemLogic::instance()->setPopLayerId(STAGE_LAYER);
    }

    ClientLogic::instance()->ChangeState(GAME_STATE_HALL);
    
//    if (GameCore::instance()->getAbyssId() != 0) {
//        delayFunc.push_back(CC_CALLBACK_0(SystemLogic::showHallAbyss, SystemLogic::instance()));
//    }else if (GameCore::instance()->getAreanId() != 0){
//        delayFunc.push_back(CC_CALLBACK_0(SystemLogic::showHallArena, SystemLogic::instance()));
//    }else if (GameCore::instance()->getTowerId() != 0){
//        delayFunc.push_back(CC_CALLBACK_0(ChapterLogic::showTower, ChapterLogic::instance()));
//    }else{
//        delayFunc.push_back(CC_CALLBACK_0(ChapterLogic::back2Stage, ChapterLogic::instance()));
//    }
}

void ClientLogic::FightNotRevive(void *pMsg)
{
    GameCore::instance()->notRevive();
}

void ClientLogic::FightRevive(void *pMsg)
{
    SystemLogic::instance()->buyRevive();
//    GameCore::instance()->revive();
}

void ClientLogic::UIClickMainBattle(void *pMsg)
{
//    ClientLogic::instance()->ChangeState(GAME_STATE_CHAPTER);
//    if (!delayFunc.empty()) {
//        return;
//    }
    ChapterLogic::instance()->showBattle();
}

void ClientLogic::UIClickMainHero(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallHero();
}

void ClientLogic::UIClickMainPet(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallPet();
}

void ClientLogic::UISelEquipPos(void *pMsg)
{
    E2L_SEL_EQUIP_POS *pMsgInfo = (E2L_SEL_EQUIP_POS*)pMsg;
    SystemLogic::instance()->selEquipPos(*pMsgInfo);
}

void ClientLogic::UIClickEquipUplevel(void *pMsg)
{
    SystemLogic::instance()->uplevelCurrEquip();
}

void ClientLogic::UIClickEquipUpgrade(void *pMsg)
{
    SystemLogic::instance()->upgradeCurrEquip();
}

void ClientLogic::UIClickEquipItem(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->selEquipItem(*pMsgInfo);
}

void ClientLogic::UIBuyEquipItem(void *pMsg)
{
    E2L_BUY_EQUIP_ITEM *pMsgInfo = (E2L_BUY_EQUIP_ITEM*)pMsg;
    SystemLogic::instance()->buyEquipItem(*pMsgInfo);
}

void ClientLogic::UIShowSweepEquipItem(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showSweepItem(*pMsgInfo);
}

void ClientLogic::UIStartSweepEquipItem(void *pMsg)
{
    E2L_START_ITEM_SWEEP *pMsgInfo = (E2L_START_ITEM_SWEEP*)pMsg;
    SystemLogic::instance()->startSweepItem(*pMsgInfo);
}

void ClientLogic::UISelPetPos(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->selPetPos(*pMsgInfo);
}

void ClientLogic::UIClickPetUplevel(void *pMsg)
{
    SystemLogic::instance()->uplevelCurrPet();
}

void ClientLogic::UIAddFightPet(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->addFightPet(*pMsgInfo);
}

void ClientLogic::UIRemoveFightPet(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->removeFightPet(*pMsgInfo);
}

void ClientLogic::UIClickMainPetOffer(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallPetOffer();
}

void ClientLogic::UIUpdateAllOfferCD(void *pMsg)
{
    SystemLogic::instance()->getAllOfferCD();
}

void ClientLogic::UIRefreshBoardOffer(void *pMsg)
{
    SystemLogic::instance()->refreshOptBoardOffer();
}

void ClientLogic::UIOpenBoardOffer(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->openBoardOffer(*pMsgInfo);
}

void ClientLogic::UIAddOfferPet(void *pMsg)
{
    E2L_ADD_OFFER_PET *pMsgInfo = (E2L_ADD_OFFER_PET*)pMsg;
    SystemLogic::instance()->addOfferPet(*pMsgInfo);
}

void ClientLogic::UIRemoveOfferPet(void *pMsg)
{
    E2L_REMOVE_OFFER_PET *pMsgInfo = (E2L_REMOVE_OFFER_PET*)pMsg;
    SystemLogic::instance()->removeOfferPet(*pMsgInfo);
}

void ClientLogic::UITakeBoardOffer(void *pMsg)
{
    E2L_TAKE_BOARD_OFFER *pMsgInfo = (E2L_TAKE_BOARD_OFFER*)pMsg;
    SystemLogic::instance()->takeBoardOffer(*pMsgInfo);
}

void ClientLogic::UITakeOfferReward(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->takeOfferReward(*pMsgInfo);
}

void ClientLogic::UIBuyOffer(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->buyOffer(*pMsgInfo);
}

void ClientLogic::UIClickMainHolySword(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallHolySword();
}

void ClientLogic::UISelSword(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->selSword(*pMsgInfo);
}

void ClientLogic::UISetFightSword(void *pMsg)
{
    SystemLogic::instance()->setFightSword();
}

void ClientLogic::UIClickSwordUplevel(void *pMsg)
{
    SystemLogic::instance()->uplevelSword();
}

void ClientLogic::UIClickMainArms(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallArms();
}

void ClientLogic::UIClickArms(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->selArms(*pMsgInfo);
}

void ClientLogic::UIClickArmsUplevel(void *pMsg)
{
    SystemLogic::instance()->uplevelArms();
}

void ClientLogic::UIClickAssembleGem(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->assembleGem(*pMsgInfo);
}

void ClientLogic::UIClickFillGem(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->fillGem(*pMsgInfo);
}

void ClientLogic::UIClickRemoveGem(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->removeGem(*pMsgInfo);
}

void ClientLogic::UIShowFillGemBoard(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showFillGemBoard(*pMsgInfo);
}

void ClientLogic::UIClickProv(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallProv();
}

void ClientLogic::UIClickProvStart(void *pMsg)
{
    SystemLogic::instance()->confirmProvStage();
}

void ClientLogic::UIProvConfirmFighter(void *pMsg)
{
    SystemLogic::instance()->confirmProvFighter();
}

void ClientLogic::UIProvFight(void *pMsg)
{
    SystemLogic::instance()->provFight();
}

void ClientLogic::UIProvEndFight(void *pMsg)
{
    SystemLogic::instance()->provEndFight();
}

void ClientLogic::UIProvEndWin(void *pMsg)
{
    SystemLogic::instance()->provEndWin();
}

void ClientLogic::UIProvEndLose(void *pMsg)
{
    SystemLogic::instance()->provEndLose();
}

void ClientLogic::UIProvReset(void *pMsg)
{
    SystemLogic::instance()->provReset();
}

void ClientLogic::UIProvBuyTicket(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->provBuyTicket(*pMsgInfo);
}

void ClientLogic::FightPause(void *pMsg)
{
    GameCore::instance()->pauseFight();
}

void ClientLogic::FightResume(void *pMsg)
{
    GameCore::instance()->resumeFight();
}

void ClientLogic::UIShowHallBuyVIT(void *pMsg)
{
    SystemLogic::instance()->showHallBuyVIT();
}

void ClientLogic::UIShowBuyVIT(void *pMsg)
{
    SystemLogic::instance()->showBuyVIT();
}

void ClientLogic::UIBuyVIT(void *pMsg)
{
    SystemLogic::instance()->buyVIT();
}

void ClientLogic::UIClickMainAbyss(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallAbyss();
}

void ClientLogic::UICheckStartAbyss(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    ChapterLogic::instance()->checkCheckAbyss(*pMsgInfo);
}

void ClientLogic::UIStartAbyss(void *pMsg)
{
    ChapterLogic::instance()->startAbyss();
}

void ClientLogic::UIResetAbyss(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    ChapterLogic::instance()->resetAbyss(*pMsgInfo);
}

void ClientLogic::UIShowResetAbyss(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    ChapterLogic::instance()->showAyssReset(*pMsgInfo);
}

void ClientLogic::UIClickMainTower(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    ChapterLogic::instance()->showTower();
}

void ClientLogic::UIShowTowerSweep(void *pMsg)
{
    ChapterLogic::instance()->showTowerSweep();
}

void ClientLogic::UIStartTower(void *pMsg)
{
    ChapterLogic::instance()->startTower();
}

void ClientLogic::UIResetTower(void *pMsg)
{
    ChapterLogic::instance()->resetTower();
}

void ClientLogic::UIStartTowerSweep(void *pMsg)
{
    ChapterLogic::instance()->startTowerSweep();
}

void ClientLogic::UITowerTakeReward(void *pMsg)
{
    ChapterLogic::instance()->towerTakeReward();
}

void ClientLogic::UISpeedupTowerSweep(void *pMsg)
{
    ChapterLogic::instance()->speedupTowerSweep();
}

void ClientLogic::UIUpdateSweepCD(void *pMsg)
{
    ChapterLogic::instance()->getTowerSweepCD();
}

void ClientLogic::UIClickMainArena(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallArena();
}

void ClientLogic::UIShowArenaDivision(void *pMsg)
{
    SystemLogic::instance()->showArenaDivision();
}

void ClientLogic::UIArenaStartMatchOpp(void *pMsg)
{
    SystemLogic::instance()->startMatchOpp();
}

void ClientLogic::UIArenaMatchRobot(void *pMsg)
{
    SystemLogic::instance()->arenaMatchRobot();
}

void ClientLogic::UIArenaStartFight(void *pMsg)
{
    ChapterLogic::instance()->startArenaFight();
}

void ClientLogic::UIArenaBuyTicket(void *pMsg)
{
    SystemLogic::instance()->buyArenaTicket();
}

void ClientLogic::UIArenaTakeReward(void *pMsg)
{
    SystemLogic::instance()->takeArenaReward();
}

void ClientLogic::UIUpdateArenaCD(void *pMsg)
{
    SystemLogic::instance()->getArenaCD();
}

void ClientLogic::UITriggerOpenLayer(void *pMsg)
{
    E2L_TRIGGER_OPEN_LAYER *pMsgInfo = (E2L_TRIGGER_OPEN_LAYER *)pMsg;
    SystemLogic::instance()->triggerOpenLayer(*pMsgInfo);
}

void ClientLogic::UITriggerSetupHero(void *pMsg)
{
    SystemLogic::instance()->triggerSetupHero();
}

void ClientLogic::UITriggerPassNpc(void *pMsg)
{
    E2L_TRIGGER_PASS_NPC *pMsgInfo = (E2L_TRIGGER_PASS_NPC*)pMsg;
    SystemLogic::instance()->triggerPassNpc(*pMsgInfo);
}

void ClientLogic::UITriggerCreateMonster(void *pMsg)
{
    E2L_TRIGGER_CREATE_MONSTER *pMsgInfo = (E2L_TRIGGER_CREATE_MONSTER*)pMsg;
    SystemLogic::instance()->triggerCreateMonster(*pMsgInfo);
}

void ClientLogic::UITriggerFightVictory(void *pMsg)
{
    SystemLogic::instance()->triggerFightVictory();
}

void ClientLogic::UITriggerClickButton(void *pMsg)
{
    E2L_TRIGGER_CLICK_BUTTON *pMsgInfo = (E2L_TRIGGER_CLICK_BUTTON*)pMsg;
    SystemLogic::instance()->triggerClickButton(*pMsgInfo);
}

void ClientLogic::UITriggerUnlockFunc(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->triggerUnlockFunc(*pMsgInfo);
}

void ClientLogic::UIPlotComplete(void *pMsg)
{
    E2L_COMPLETE_PLOT *pMsgInfo = (E2L_COMPLETE_PLOT *)pMsg;
    SystemLogic::instance()->plotComplete(*pMsgInfo);
}

void ClientLogic::UINextDialog(void *pMsg)
{
    SystemLogic::instance()->nextDialog();
}

void ClientLogic::UIHallIn(void *pMsg)
{
    SystemLogic::instance()->HallIn();
}

void ClientLogic::UIClickMainCastle(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showHallCastle();
}

void ClientLogic::UIClickJob(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showJob(*pMsgInfo);
}

void ClientLogic::UIChangeJob(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->changeJob(*pMsgInfo);
}

void ClientLogic::UIActiveJob(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->activeJob(*pMsgInfo);
}

void ClientLogic::UIShowEveryday(void *pMsg)
{
    SystemLogic::instance()->showEveryday();
}

void ClientLogic::UIClickMainCampaign(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showCampaignEveryday();
}

void ClientLogic::UITakeEverydayBound(void *pMsg)
{
    SystemLogic::instance()->takeEverydayBound();
}

void ClientLogic::UIShowLucky(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showLucky();
}

void ClientLogic::UIDrawNormal(void *pMsg)
{
    SystemLogic::instance()->drawNormal();
}

void ClientLogic::UIDrawSpecial(void *pMsg)
{
    SystemLogic::instance()->drawSpecial();
}

void ClientLogic::UIDrawSpecialTen(void *pMsg)
{
    SystemLogic::instance()->drawSpecialTen();
}

void ClientLogic::UIGetLuckyCD(void *pMsg)
{
    SystemLogic::instance()->getLuckyCD();
}

void ClientLogic::UIBuySpecialTicket(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->buyLuckySpecialTicket(*pMsgInfo);
}

void ClientLogic::UIShowShop(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showHallShop(*pMsgInfo);
}

void ClientLogic::UIClickShopButton(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showShop(*pMsgInfo);
}

void ClientLogic::UIShowShopGold(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showShop(*pMsgInfo);
}

void ClientLogic::UIBuyShopPackage(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->buyShopPackage(*pMsgInfo);
}

void ClientLogic::UIShowShopPackageDetail(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showShopPackageDetail(*pMsgInfo);
}

void ClientLogic::UIShowHallNovice(void *pMsg)
{
    SystemLogic::instance()->showNovice();
}

void ClientLogic::UIShowFirstRecharge(void *pMsg)
{
    SystemLogic::instance()->showFirstRecharge();
}

void ClientLogic::UITakeFirstRecharge(void *pMsg)
{
    SystemLogic::instance()->takeFirstRecharge();
}

void ClientLogic::UIClickMainVIP(void *pMsg)
{
//    if (!delayFunc.empty()) {
//        return;
//    }
    SystemLogic::instance()->showVIP();
}

void ClientLogic::UIShowVIPDetail(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->showVIPDetail(*pMsgInfo);
}

void ClientLogic::UITakeVIPBound(void *pMsg)
{
    SystemLogic::instance()->takeVipBound();
}

void ClientLogic::UIVIPJumpRecharge(void *pMsg)
{
    SystemLogic::instance()->vipJumpRecharge();
}

void ClientLogic::UIShowCampaignActive(void *pMsg)
{
    SystemLogic::instance()->showCampaignActive();
}

void ClientLogic::UITakeCampaignActiveMissionBound(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->takeCampaignActiveBound(*pMsgInfo);
}

void ClientLogic::UIClickCampaignActiveBox(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->takeCampaignActiveBoxBound(*pMsgInfo);
}

void ClientLogic::UIClickCampaignActiveJump(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->campaignActiveJump(*pMsgInfo);
}

void ClientLogic::UIClickFreeDiamond(void *pMsg)
{
    SystemLogic::instance()->takeFreeDiamond();
}

void ClientLogic::UIShowCampaignBoundMission(void *pMsg)
{
    SystemLogic::instance()->showCampaignBoundMission();
}

void ClientLogic::UITakeCampaignBoundReward(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->takeCampaignMissionReward(*pMsgInfo);
}

void ClientLogic::UIClickCampaignBoundJump(void *pMsg)
{
    E2L_UPDATE_ONE_VALUE *pMsgInfo = (E2L_UPDATE_ONE_VALUE*)pMsg;
    SystemLogic::instance()->campaignMissionJump(*pMsgInfo);
}


void ClientLogic::UIUpdateDiscountState(void *pMsg)
{
    SystemLogic::instance()->getDiscountState();
}

void ClientLogic::UIShowDiscount(void *pMsg)
{
    SystemLogic::instance()->showDiscount();
}

void ClientLogic::UIGetVITCD(void *pMsg)
{
    SystemLogic::instance()->getVITCD();
}

void ClientLogic::UIChangeName(void *pMsg)
{
    E2L_CHANGE_NAME *pMsgInfo = (E2L_CHANGE_NAME*)pMsg;
    SystemLogic::instance()->changeName(*pMsgInfo);
}

void ClientLogic::UIExit(void *pMsg)
{
    SystemLogic::instance()->clickExit();
}
