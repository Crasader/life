//
//  ClientLogic.cpp
//  owen
//
//  Created by ff on 16/8/9.
//
//

#include "ClientLogic.h"
#include "view/UIManager.h"
#include "service/ServiceManager.h"
#include "time/TimeManager.h"

#include "GameCore.h"
#include "ChapterLogic.h"
#include "SystemLogic.h"

#include "utils/TimeUtil.h"


ClientLogic::ClientLogic()
{
    m_eLastState = GAME_STATE_NONE;
    m_eState = GAME_STATE_NONE;
    
//    delayFunc.clear();
    
    memset(m_UI2LogicFunc, 0, sizeof(m_UI2LogicFunc));
    memset(m_LogicStateFunc, 0, sizeof(m_LogicStateFunc));
    
    m_UI2LogicFunc[e2l_login_click] = &ClientLogic::UIClickLogin;
    m_UI2LogicFunc[e2l_check_net] = &ClientLogic::UICheckNet;
    m_UI2LogicFunc[e2l_click_unlock] = &ClientLogic::UIClickUnlock;
    m_UI2LogicFunc[e2l_monster_hp_empty] = &ClientLogic::FightMonsterHpEmpty;
    m_UI2LogicFunc[e2l_monster_die] = &ClientLogic::FightMonsterDie;
    m_UI2LogicFunc[e2l_start_fight] = &ClientLogic::FightStart;
    m_UI2LogicFunc[e2l_witness_die] = &ClientLogic::FightWitnessDie;
    m_UI2LogicFunc[e2l_witness_arrive] = &ClientLogic::FightWitnessArrive;
    m_UI2LogicFunc[e2l_hero_die] = &ClientLogic::FightHeroDie;
    m_UI2LogicFunc[e2l_time_over] = &ClientLogic::FightTimeOver;
    m_UI2LogicFunc[e2l_time_over_2] = &ClientLogic::FightTimeOver2;
    m_UI2LogicFunc[e2l_fight_star] = &ClientLogic::FightStar;
    m_UI2LogicFunc[e2l_use_hp_bottle] = &ClientLogic::FightUseHpBottle;
    m_UI2LogicFunc[e2l_add_combo] = &ClientLogic::FightAddCombo;
    m_UI2LogicFunc[e2l_clear_combo] = &ClientLogic::FightClearCombo;
    m_UI2LogicFunc[e2l_click_win] = &ClientLogic::FightWinClick;
    m_UI2LogicFunc[e2l_click_fail] = &ClientLogic::FightFailClick;
    m_UI2LogicFunc[e2l_click_not_revive] = &ClientLogic::FightNotRevive;
    m_UI2LogicFunc[e2l_click_revive] = &ClientLogic::FightRevive;
    m_UI2LogicFunc[e2l_pause_fight] = &ClientLogic::FightPause;
    m_UI2LogicFunc[e2l_resume_fight] = &ClientLogic::FightResume;
    
    m_UI2LogicFunc[e2l_click_chapter_name] = &ClientLogic::UIClickChapterName;
    m_UI2LogicFunc[e2l_show_chapter] = &ClientLogic::UIClickShowChapter;
    m_UI2LogicFunc[e2l_show_stage] = &ClientLogic::UIClickShowStage;
    m_UI2LogicFunc[e2l_start_stage] = &ClientLogic::UIClickStartStage;
    m_UI2LogicFunc[e2l_back_chapter] = &ClientLogic::UIBack2Chapter;
    m_UI2LogicFunc[e2l_back_stage] = &ClientLogic::UIBack2Stage;
    m_UI2LogicFunc[e2l_back_battle_chapters] = &ClientLogic::UIBack2BattleChapters;
    m_UI2LogicFunc[e2l_click_star_box] = &ClientLogic::UIClickStarBox;
    
    m_UI2LogicFunc[e2l_click_main_battle] = &ClientLogic::UIClickMainBattle;
    m_UI2LogicFunc[e2l_click_main_hero] = &ClientLogic::UIClickMainHero;
    m_UI2LogicFunc[e2l_click_main_pet] = &ClientLogic::UIClickMainPet;
    m_UI2LogicFunc[e2l_click_main_pet_offer] = &ClientLogic::UIClickMainPetOffer;
    m_UI2LogicFunc[e2l_click_main_holy_sword] = &ClientLogic::UIClickMainHolySword;
    m_UI2LogicFunc[e2l_click_main_arms] = &ClientLogic::UIClickMainArms;
    m_UI2LogicFunc[e2l_click_main_prov] = &ClientLogic::UIClickProv;
    m_UI2LogicFunc[e2l_click_main_abyss] = &ClientLogic::UIClickMainAbyss;
    m_UI2LogicFunc[e2l_click_main_tower] = &ClientLogic::UIClickMainTower;
    m_UI2LogicFunc[e2l_click_main_arena] = &ClientLogic::UIClickMainArena;
    m_UI2LogicFunc[e2l_click_main_castle] = &ClientLogic::UIClickMainCastle;
    m_UI2LogicFunc[e2l_click_main_campaign] = &ClientLogic::UIClickMainCampaign;
    m_UI2LogicFunc[e2l_click_main_lucky] = &ClientLogic::UIShowLucky;
    m_UI2LogicFunc[e2l_click_main_shop] = &ClientLogic::UIShowShop;
    m_UI2LogicFunc[e2l_click_main_novice] = &ClientLogic::UIShowHallNovice;
    m_UI2LogicFunc[e2l_click_main_vip] = &ClientLogic::UIClickMainVIP;
    
    m_UI2LogicFunc[e2l_sel_equip_pos] = &ClientLogic::UISelEquipPos;
    m_UI2LogicFunc[e2l_click_equip_uplevel] = &ClientLogic::UIClickEquipUplevel;
    m_UI2LogicFunc[e2l_click_equip_upgrade] = &ClientLogic::UIClickEquipUpgrade;
    m_UI2LogicFunc[e2l_click_equip_item] = &ClientLogic::UIClickEquipItem;
    m_UI2LogicFunc[e2l_buy_equip_item] = &ClientLogic::UIBuyEquipItem;
    m_UI2LogicFunc[e2l_click_sweep_item] = &ClientLogic::UIShowSweepEquipItem;
    m_UI2LogicFunc[e2l_start_sweep_item] = &ClientLogic::UIStartSweepEquipItem;
    
    m_UI2LogicFunc[e2l_sel_pet_pos] = &ClientLogic::UISelPetPos;
    m_UI2LogicFunc[e2l_click_pet_uplevel] = &ClientLogic::UIClickPetUplevel;
    m_UI2LogicFunc[e2l_add_fight_pet] = &ClientLogic::UIAddFightPet;
    m_UI2LogicFunc[e2l_remove_fight_pet] = &ClientLogic::UIRemoveFightPet;
    
    m_UI2LogicFunc[e2l_update_all_offer_cd] = &ClientLogic::UIUpdateAllOfferCD;
    m_UI2LogicFunc[e2l_click_refresh_offer] = &ClientLogic::UIRefreshBoardOffer;
    m_UI2LogicFunc[e2l_open_board_offer] = &ClientLogic::UIOpenBoardOffer;
    m_UI2LogicFunc[e2l_add_offer_pet] = &ClientLogic::UIAddOfferPet;
    m_UI2LogicFunc[e2l_remove_offer_pet] = &ClientLogic::UIRemoveOfferPet;
    m_UI2LogicFunc[e2l_take_board_offer] = &ClientLogic::UITakeBoardOffer;
    m_UI2LogicFunc[e2l_take_offer_reward] = &ClientLogic::UITakeOfferReward;
    m_UI2LogicFunc[e2l_buy_offer] = &ClientLogic::UIBuyOffer;
    
    m_UI2LogicFunc[e2l_sel_sword] = &ClientLogic::UISelSword;
    m_UI2LogicFunc[e2l_set_fight_sword] = &ClientLogic::UISetFightSword;
    m_UI2LogicFunc[e2l_click_sword_uplevel] = &ClientLogic::UIClickSwordUplevel;
    
    m_UI2LogicFunc[e2l_sel_arms] = &ClientLogic::UIClickArms;
    m_UI2LogicFunc[e2l_uplevel_arms] = &ClientLogic::UIClickArmsUplevel;
    m_UI2LogicFunc[e2l_assemble_gem] = &ClientLogic::UIClickAssembleGem;
    m_UI2LogicFunc[e2l_fill_gem] = &ClientLogic::UIClickFillGem;
    m_UI2LogicFunc[e2l_remove_gem] = &ClientLogic::UIClickRemoveGem;
    m_UI2LogicFunc[e2l_show_fill_board] = &ClientLogic::UIShowFillGemBoard;
    
    m_UI2LogicFunc[e2l_click_prov_start] = &ClientLogic::UIClickProvStart;
    m_UI2LogicFunc[e2l_confirm_prov_fighter] = &ClientLogic::UIProvConfirmFighter;
    m_UI2LogicFunc[e2l_prov_fight] = &ClientLogic::UIProvFight;
    m_UI2LogicFunc[e2l_prov_end_fight] = &ClientLogic::UIProvEndFight;
    m_UI2LogicFunc[e2l_prov_end_win] = &ClientLogic::UIProvEndWin;
    m_UI2LogicFunc[e2l_prov_end_lose] = &ClientLogic::UIProvEndLose;
    m_UI2LogicFunc[e2l_prov_reset] = &ClientLogic::UIProvReset;
    m_UI2LogicFunc[e2l_prov_buy_ticket] = &ClientLogic::UIProvBuyTicket;
    
    m_UI2LogicFunc[e2l_click_abyss_start] = &ClientLogic::UICheckStartAbyss;
    m_UI2LogicFunc[e2l_confirm_start_abyss] = &ClientLogic::UIStartAbyss;
    m_UI2LogicFunc[e2l_click_abyss_reset] = &ClientLogic::UIResetAbyss;
    m_UI2LogicFunc[e2l_show_abyss_reset] = &ClientLogic::UIShowResetAbyss;
    
    m_UI2LogicFunc[e2l_click_tower_sweep] = &ClientLogic::UIShowTowerSweep;
    m_UI2LogicFunc[e2l_click_tower_start] = &ClientLogic::UIStartTower;
    m_UI2LogicFunc[e2l_click_tower_reset] = &ClientLogic::UIResetTower;
    m_UI2LogicFunc[e2l_confirm_tower_sweep] = &ClientLogic::UIStartTowerSweep;
    m_UI2LogicFunc[e2l_tower_take_reward] = &ClientLogic::UITowerTakeReward;
    m_UI2LogicFunc[e2l_click_tower_speedup] = &ClientLogic::UISpeedupTowerSweep;
    m_UI2LogicFunc[e2l_get_tower_sweep_sec] = &ClientLogic::UIUpdateSweepCD;
    
    m_UI2LogicFunc[e2l_click_arena_division] = &ClientLogic::UIShowArenaDivision;
    m_UI2LogicFunc[e2l_click_arena_start] = &ClientLogic::UIArenaStartMatchOpp;
    m_UI2LogicFunc[e2l_arena_match_robot] = &ClientLogic::UIArenaMatchRobot;
    m_UI2LogicFunc[e2l_arena_start_fight] = &ClientLogic::UIArenaStartFight;
    m_UI2LogicFunc[e2l_click_add_arena_ticket] = &ClientLogic::UIArenaBuyTicket;
    m_UI2LogicFunc[e2l_arena_take_reward] = &ClientLogic::UIArenaTakeReward;
    m_UI2LogicFunc[e2l_update_arena_cd] = &ClientLogic::UIUpdateArenaCD;
    
    m_UI2LogicFunc[e2l_show_buy_vit] = &ClientLogic::UIShowBuyVIT;
    m_UI2LogicFunc[e2l_buy_vit] = &ClientLogic::UIBuyVIT;
    m_UI2LogicFunc[e2l_show_hall_buy_vit] = &ClientLogic::UIShowHallBuyVIT;
    
    m_UI2LogicFunc[e2l_trigger_open_layer] = &ClientLogic::UITriggerOpenLayer;
    m_UI2LogicFunc[e2l_trigger_setup_hero] = &ClientLogic::UITriggerSetupHero;
    m_UI2LogicFunc[e2l_trigger_pass_npc] = &ClientLogic::UITriggerPassNpc;
    m_UI2LogicFunc[e2l_trigger_create_monster] = &ClientLogic::UITriggerCreateMonster;
    m_UI2LogicFunc[e2l_trigger_victory] = &ClientLogic::UITriggerFightVictory;
    m_UI2LogicFunc[e2l_trigger_click_button] = &ClientLogic::UITriggerClickButton;
    m_UI2LogicFunc[e2l_trigger_unlock] = &ClientLogic::UITriggerUnlockFunc;
    m_UI2LogicFunc[e2l_complete_plot] = &ClientLogic::UIPlotComplete;
    m_UI2LogicFunc[e2l_next_dialog] = &ClientLogic::UINextDialog;
    
    m_UI2LogicFunc[e2l_hall_in] = &ClientLogic::UIHallIn;
    
    m_UI2LogicFunc[e2l_click_job] = &ClientLogic::UIClickJob;
    m_UI2LogicFunc[e2l_confirm_job] = &ClientLogic::UIChangeJob;
    m_UI2LogicFunc[e2l_active_job] = &ClientLogic::UIActiveJob;
    m_UI2LogicFunc[e2l_change_name] = &ClientLogic::UIChangeName;
    
    m_UI2LogicFunc[e2l_show_everyday] = &ClientLogic::UIShowEveryday;
    m_UI2LogicFunc[e2l_take_everyday_bound] = &ClientLogic::UITakeEverydayBound;
    m_UI2LogicFunc[e2l_normal_draw] = &ClientLogic::UIDrawNormal;
    m_UI2LogicFunc[e2l_special_draw_one] = &ClientLogic::UIDrawSpecial;
    m_UI2LogicFunc[e2l_special_draw_ten] = &ClientLogic::UIDrawSpecialTen;
    m_UI2LogicFunc[e2l_update_lucky_cd] = &ClientLogic::UIGetLuckyCD;
    m_UI2LogicFunc[e2l_buy_special_ticket] = &ClientLogic::UIBuySpecialTicket;
    m_UI2LogicFunc[e2l_show_first_recharge] = &ClientLogic::UIShowFirstRecharge;
    m_UI2LogicFunc[e2l_take_first_recharge] = &ClientLogic::UITakeFirstRecharge;
    m_UI2LogicFunc[e2l_show_campaign_active] = &ClientLogic::UIShowCampaignActive;
    m_UI2LogicFunc[e2l_click_active_bound] = &ClientLogic::UITakeCampaignActiveMissionBound;
    m_UI2LogicFunc[e2l_click_active_box] = &ClientLogic::UIClickCampaignActiveBox;
    m_UI2LogicFunc[e2l_click_active_jump] = &ClientLogic::UIClickCampaignActiveJump;
    m_UI2LogicFunc[e2l_click_free_diamond] = &ClientLogic::UIClickFreeDiamond;
    m_UI2LogicFunc[e2l_show_bound_mission] = &ClientLogic::UIShowCampaignBoundMission;
    m_UI2LogicFunc[e2l_take_mission_reward] = &ClientLogic::UITakeCampaignBoundReward;
    m_UI2LogicFunc[e2l_click_mission_jump] = &ClientLogic::UIClickCampaignBoundJump;
    m_UI2LogicFunc[e2l_show_month_assign] = &ClientLogic::UIShowMonthAssign;
    m_UI2LogicFunc[e2l_take_month_assign_reward] = &ClientLogic::UITakeMonthAssignReward;
    
    m_UI2LogicFunc[e2l_click_shop_button] = &ClientLogic::UIClickShopButton;
    m_UI2LogicFunc[e2l_shop_buy_package] = &ClientLogic::UIBuyShopPackage;
    m_UI2LogicFunc[e2l_shop_package_detail] = &ClientLogic::UIShowShopPackageDetail;
    m_UI2LogicFunc[e2l_show_shop_gold] = &ClientLogic::UIShowShopGold;
    
    m_UI2LogicFunc[e2l_show_vip_detail] = &ClientLogic::UIShowVIPDetail;
    m_UI2LogicFunc[e2l_take_vip_bound] = &ClientLogic::UITakeVIPBound;
    
    m_UI2LogicFunc[e2l_update_discount_state] = &ClientLogic::UIUpdateDiscountState;
    m_UI2LogicFunc[e2l_click_main_discount] = &ClientLogic::UIShowDiscount;
    
    m_UI2LogicFunc[e2l_get_vit_cd] = &ClientLogic::UIGetVITCD;
    m_UI2LogicFunc[e2l_vip_jump_recharge] = &ClientLogic::UIVIPJumpRecharge;
    m_UI2LogicFunc[e2l_exit] = &ClientLogic::UIExit;
    
    m_Service2ClientFunc[s2c_check_connect] = &ClientLogic::ResCheckConnect;
    m_Service2ClientFunc[s2c_pay] = &ClientLogic::ResPay;
    m_Service2ClientFunc[s2c_exit] = &ClientLogic::ResExit;
    m_Service2ClientFunc[s2c_server_time_ack] = &ClientLogic::ResServerTimeAck;
    
    m_Time2ClientFunc[t2l_restore_vit] = &ClientLogic::TimeRestoreVIT;
    m_Time2ClientFunc[t2l_update_offer_all_cd] = &ClientLogic::TimeUpdateOfferAllCD;
    m_Time2ClientFunc[t2l_refresh_offer_board] = &ClientLogic::TimeRefreshBoardOffer;
    m_Time2ClientFunc[t2l_work_offer1_over] = &ClientLogic::TimeWorkOffer1Over;
    m_Time2ClientFunc[t2l_work_offer2_over] = &ClientLogic::TimeWorkOffer2Over;
    m_Time2ClientFunc[t2l_work_offer3_over] = &ClientLogic::TimeWorkOffer3Over;
    m_Time2ClientFunc[t2l_refresh_abyss] = &ClientLogic::TimeRefreshAbyss;
    m_Time2ClientFunc[t2l_refresh_prov_reset] = &ClientLogic::TimeRefreshProvReset;
    m_Time2ClientFunc[t2l_reset_vit_buy_count] = &ClientLogic::TimeResetVITBuyCount;
    m_Time2ClientFunc[t2l_complete_tower_sweep] = &ClientLogic::TimeCompleteTowerSweep;
    m_Time2ClientFunc[t2l_refresh_tower_reset] = &ClientLogic::TimeRefreshTowerReset;
    m_Time2ClientFunc[t2l_update_tower_sweep_cd] = &ClientLogic::TimeUpdateTowerSweepCD;
    m_Time2ClientFunc[t2l_restore_arena_ticket] = &ClientLogic::TimeRestoreArenaTicket;
    m_Time2ClientFunc[t2l_update_arena_ticket_cd] = &ClientLogic::TimeUpdateArenaTicketCD;
    m_Time2ClientFunc[t2l_refresh_arena_reward] = &ClientLogic::TimeRefreshArenaReward;
    m_Time2ClientFunc[t2l_refresh_everyday] = &ClientLogic::TimeRefreshEveryday;
    m_Time2ClientFunc[t2l_refresh_month_assign] = &ClientLogic::TimeRefreshMonthAssign;
    m_Time2ClientFunc[t2l_restore_draw_normal] = &ClientLogic::TimeRestoreDrawNormal;
    m_Time2ClientFunc[t2l_restore_draw_special] = &ClientLogic::TimeRestoreDrawSpecial;
    m_Time2ClientFunc[t2l_update_lucky_cd] = &ClientLogic::TimeUpdateLuckyCD;
    m_Time2ClientFunc[t2l_add_vip_login_day] = &ClientLogic::TimeAddVipLoginDay;
    m_Time2ClientFunc[t2l_refresh_campaign_active] = &ClientLogic::TimeRefreshCampaignActive;
    m_Time2ClientFunc[t2l_reset_free_diamond] = &ClientLogic::TimeResetFreeDiamond;
    m_Time2ClientFunc[t2l_open_discount] = &ClientLogic::TimeOpenDiscount;
    m_Time2ClientFunc[t2L_close_discount] = &ClientLogic::TimeCloseDiscount;
    m_Time2ClientFunc[t2l_update_discount_close_sec] = &ClientLogic::TimeUpdateDiscountCloseSec;
    m_Time2ClientFunc[t2l_update_vit_restore_cd] = &ClientLogic::TimeUpdateVITCD;
    m_Time2ClientFunc[t2l_reset_petoffer_count] = &ClientLogic::TimeResetPetOfferCount;
    
    m_LogicStateFunc[GAME_STATE_INIT] = &ClientLogic::onStateInit;
    m_LogicStateFunc[GAME_STATE_FIGHT] = &ClientLogic::onStateFight;
    m_LogicStateFunc[GAME_STATE_HALL] = &ClientLogic::onStateHall;
}

void ClientLogic::mainLoop(float dt)
{
//    if (!UIManager::instance()->replacing) {
//        if (!delayFunc.empty()) {
//            delayFunc[delayFunc.size()-1]();
//            delayFunc.pop_back();
//        }
//    }

    TimeUtil::updateServerTime(dt);
    
    if (this->m_LogicStateFunc[m_eState] == nullptr) {
        return;
    }
    (this->*m_LogicStateFunc[m_eState])(dt);
}

//改变游戏状态
void ClientLogic::ChangeState(E_GAME_STATE eState)
{
    if(m_eState == eState)
        return;
    exitGameMode(m_eState);
    m_eLastState	= m_eState;
    m_eState		= eState;
    enterGameMode(eState);
}

//进入游戏状态前的准备处理
bool ClientLogic::enterGameMode(E_GAME_STATE next)
{
    switch(next)
    {
        case GAME_STATE_LOGO:
        {
            SystemLogic::instance()->showLogo();
        }
            break;
        case GAME_STATE_INIT:
        {
            SystemLogic::instance()->arrangeLoad();
            L2T_COMMON info;
            info.eProtocol = l2t_start_app;
            pass2Time(&info);
        }
            break;
        case GAME_STATE_FIGHT:
        {
            GameCore::instance()->setStageId(ChapterLogic::instance()->getCurrStage());
            GameCore::instance()->setAbyssId(ChapterLogic::instance()->getCurrAbyss());
            GameCore::instance()->setTowerId(ChapterLogic::instance()->getCurrTower());
            GameCore::instance()->setAreanId(ChapterLogic::instance()->getCurrArena());
            GameCore::instance()->changeGameState(FIGHT_GAME_LOAD);
        }
            break;
//        case GAME_STATE_CHAPTER:
//        {
//            ChapterLogic::instance()->showBattle();
//            L2E_COMMON info;
//            info.eProtocol = l2e_show_chapter_scene;
//            pass2Engine(&info);
//        }
//            break;
        case GAME_STATE_HALL:
        {
            SystemLogic::instance()->showHall();            
        }
            break;
        default:
            break;
    }
    return true;
}

//离开游戏状态的结束处理
void ClientLogic::exitGameMode(E_GAME_STATE currMode)
{
    switch (currMode) {
            
        default:
            break;
    }
}

void ClientLogic::onStateFight(float dt)
{
    GameCore::instance()->logic(dt);
    TimeManager::instance()->mainLoop(dt);
}

void ClientLogic::onStateHall(float dt)
{
    SystemLogic::instance()->logic(dt);
    TimeManager::instance()->mainLoop(dt);
}

void ClientLogic::onStateInit(float dt)
{
    SystemLogic::instance()->load();
}

//发送结构体通知给显示层
void ClientLogic::pass2Engine(void *pMsg)
{
    UIManager::instance()->processLogicResponse(pMsg);
}

void ClientLogic::pass2Service(void *pMsg)
{
    ServiceManager::instance()->processClientQuest(pMsg);
}

void ClientLogic::pass2Time(void *pMsg)
{
    TimeManager::instance()->processClientQuest(pMsg);
}

