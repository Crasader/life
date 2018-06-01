//
//  UIManager.cpp
//  owen
//
//  Created by ff on 16/8/9.
//
//

#include "UIManager.h"
#include "logic/ClientLogic.h"
#include "EventDefine.h"
#include "scene/GameScene.h"
#include "scene/HallScene.h"
#include "scene/SystemLoadScene.h"
#include "scene/LoginScene.h"

USING_NS_CC;

UIManager::UIManager()
{
    memset(m_Logic2EngineFunc, 0, sizeof(m_Logic2EngineFunc));

    m_Logic2EngineFunc[l2e_show_login] = &UIManager::showLogin;
    m_Logic2EngineFunc[l2e_show_connected] = &UIManager::updateNetState;
    m_Logic2EngineFunc[l2e_show_load] = &UIManager::showLoadStage;
    m_Logic2EngineFunc[l2e_update_load_stage] = &UIManager::updateLoadStage;
    m_Logic2EngineFunc[l2e_setup_map] = &UIManager::setupTmxMap;
    m_Logic2EngineFunc[l2e_setup_map_effect] = &UIManager::setupTmxEffect;
    m_Logic2EngineFunc[l2e_setup_hero] = &UIManager::setupHero;
    m_Logic2EngineFunc[l2e_setup_witness] = &UIManager::setupWitness;
    m_Logic2EngineFunc[l2e_setup_opponent] = &UIManager::setupOpponent;
    m_Logic2EngineFunc[l2e_show_fight] = &UIManager::showFight;
    m_Logic2EngineFunc[l2e_unlock_view] = &UIManager::unlockView;
    m_Logic2EngineFunc[l2e_setup_monster_team] = &UIManager::setupMonsterTeam;
    m_Logic2EngineFunc[l2e_ready_victory] = &UIManager::readyVictory;
    m_Logic2EngineFunc[l2e_victory] = &UIManager::stageVictory;
    m_Logic2EngineFunc[l2e_fail] = &UIManager::stageFail;
    m_Logic2EngineFunc[l2e_show_revive] = &UIManager::showRevive;
    m_Logic2EngineFunc[l2e_revive] = &UIManager::revive;
    m_Logic2EngineFunc[l2e_fight_time_over] = &UIManager::stageTimeOver;
    m_Logic2EngineFunc[l2e_update_wave_count] = &UIManager::stageUpdateWaveCount;
    m_Logic2EngineFunc[l2e_full_hero_hp] = &UIManager::fullHeroHp;
    m_Logic2EngineFunc[l2e_update_hp_bottle_price] = &UIManager::updateHpBottlePrice;
    m_Logic2EngineFunc[l2e_update_combo] = &UIManager::updateCombo;
    m_Logic2EngineFunc[l2e_clear_combo] = &UIManager::clearCombo;
    m_Logic2EngineFunc[l2e_fight_star] = &UIManager::stageStar;
    m_Logic2EngineFunc[l2e_pause_fight] = &UIManager::pauseFight;
    m_Logic2EngineFunc[l2e_resume_fight] = &UIManager::resumeFight;
    m_Logic2EngineFunc[l2e_preload_monster_res] = &UIManager::preloadMonsterRes;
    
    m_Logic2EngineFunc[l2e_show_battle_chapters] = &UIManager::showBattleChapters;
    m_Logic2EngineFunc[l2e_show_chapter_name] = &UIManager::showChapterName;
    m_Logic2EngineFunc[l2e_show_chapter_stages] = &UIManager::showChapter;
    m_Logic2EngineFunc[l2e_show_stage] = &UIManager::showStage;
    m_Logic2EngineFunc[l2e_back_battle_chapters] = &UIManager::back2BattleChapters;
    m_Logic2EngineFunc[l2e_start_stage] = &UIManager::startStage;
    m_Logic2EngineFunc[l2e_open_star_box] = &UIManager::openStarBox;
    m_Logic2EngineFunc[l2e_show_star_bounds] = &UIManager::showStarBounds;
    
//    m_Logic2EngineFunc[l2e_show_chapter_scene] = &UIManager::showChapterScene;
    m_Logic2EngineFunc[l2e_show_hall_scene] = &UIManager::showHallScene;
    m_Logic2EngineFunc[l2e_show_hall_hero] = &UIManager::showHallHero;
    m_Logic2EngineFunc[l2e_sel_equip_pos] = &UIManager::selEquipPos;
    m_Logic2EngineFunc[l2e_update_equip_level] = &UIManager::updateEquipLevel;
    m_Logic2EngineFunc[l2e_update_equip_star] = &UIManager::updateEquipStar;
    m_Logic2EngineFunc[l2e_show_item_way] = &UIManager::showItemWay;
    m_Logic2EngineFunc[l2e_show_item_sweep] = &UIManager::showItemSweep;
    m_Logic2EngineFunc[l2e_update_item_count] = &UIManager::updateItemCount;
    m_Logic2EngineFunc[l2e_item_sweep] = &UIManager::sweepItem;
    m_Logic2EngineFunc[l2e_update_gold] = &UIManager::updateGold;
    m_Logic2EngineFunc[l2e_update_diamond] = &UIManager::updateDiamond;
    m_Logic2EngineFunc[l2e_update_vit] = &UIManager::updateVIT;
    m_Logic2EngineFunc[l2e_update_exp] = &UIManager::updateExp;
    m_Logic2EngineFunc[l2e_update_force] = &UIManager::updateForce;
    
    m_Logic2EngineFunc[l2e_show_system_load] = &UIManager::showSystemLoad;
    m_Logic2EngineFunc[l2e_update_system_load] = &UIManager::updateSystemLoad;
    
    m_Logic2EngineFunc[l2e_show_hall_pet] = &UIManager::showHallPet;
    m_Logic2EngineFunc[l2e_sel_pet_pos] = &UIManager::selPetPos;
    m_Logic2EngineFunc[l2e_update_pet_level] = &UIManager::updatePetLevel;
    m_Logic2EngineFunc[l2e_update_fight_pet] = &UIManager::updateFightPet;
    
    m_Logic2EngineFunc[l2e_show_hall_pet_offer] = &UIManager::showHallPetOffer;
    m_Logic2EngineFunc[l2e_update_offer_all_cd] = &UIManager::updateHallPetOfferAllCD;
    m_Logic2EngineFunc[l2e_update_board_offer] = &UIManager::updatePetBoardOffer;
    m_Logic2EngineFunc[l2e_open_board_offer] = &UIManager::openBoardOffer;
    m_Logic2EngineFunc[l2e_add_offer_pet] = &UIManager::addOfferPet;
    m_Logic2EngineFunc[l2e_remove_offer_pet] = &UIManager::removeOfferPet;
    m_Logic2EngineFunc[l2e_work_offer_over] = &UIManager::workOfferOver;
    m_Logic2EngineFunc[l2e_take_board_offer] = &UIManager::takeBoardOffer;
    m_Logic2EngineFunc[l2e_take_offer_reward] = &UIManager::takeOfferReward;
    m_Logic2EngineFunc[l2e_show_buy_offer] = &UIManager::showBuyOffer;
    m_Logic2EngineFunc[l2e_buy_offer] = &UIManager::buyOffer;
    m_Logic2EngineFunc[l2e_reset_offer_refresh_count] = &UIManager::resetOfferRefreshCount;
    
    m_Logic2EngineFunc[l2e_show_hall_sword] = &UIManager::showHallHolySword;
    m_Logic2EngineFunc[l2e_update_sword] = &UIManager::updateSword;
    m_Logic2EngineFunc[l2e_update_fight_sword] = &UIManager::updateFightSword;
    m_Logic2EngineFunc[l2e_update_sword_level] = &UIManager::updateSwordLevel;
    
    m_Logic2EngineFunc[l2e_show_hall_arms] = &UIManager::showHallArms;
    m_Logic2EngineFunc[l2e_update_arms] = &UIManager::updateArms;
    m_Logic2EngineFunc[l2e_update_arms_level] = &UIManager::updateArmsLevel;
    m_Logic2EngineFunc[l2e_update_gems_count] = &UIManager::updateGemCount;
    m_Logic2EngineFunc[l2e_show_fill_board] = &UIManager::showFillGemBoard;
    m_Logic2EngineFunc[l2e_update_arms_gem] = &UIManager::updateArmsGem;
    
    m_Logic2EngineFunc[l2e_show_hall_prov] = &UIManager::showHallProv;
    m_Logic2EngineFunc[l2e_prov_confirm_stage] = &UIManager::confirmProvStage;
    m_Logic2EngineFunc[l2e_prov_confirm_fighter] = &UIManager::confirmProvFighter;
    m_Logic2EngineFunc[l2e_prov_fight] = &UIManager::provFight;
    m_Logic2EngineFunc[l2e_prov_win] = &UIManager::provWin;
    m_Logic2EngineFunc[l2e_prov_lose] = &UIManager::provLose;
    m_Logic2EngineFunc[l2e_prov_reset] = &UIManager::provReset;
    m_Logic2EngineFunc[l2e_prov_add_ticket] = &UIManager::provAddTicket;
    m_Logic2EngineFunc[l2e_prov_refresh_reset] = &UIManager::provRefreshReset;
    
    m_Logic2EngineFunc[l2e_show_hall_buy_vit] = &UIManager::showHallBuyVIT;
    m_Logic2EngineFunc[l2e_show_buy_vit] = &UIManager::showBuyVIT;
    m_Logic2EngineFunc[l2e_buy_vit] = &UIManager::buyVIT;
    m_Logic2EngineFunc[l2e_update_vit_buy_count] = &UIManager::updateVITBuyCount;
    m_Logic2EngineFunc[l2e_show_get] = &UIManager::showGetTip;
    
    m_Logic2EngineFunc[l2e_show_hall_abyss] = &UIManager::showHallAbyss;
    m_Logic2EngineFunc[l2e_ready_start_abyss] = &UIManager::checkAbyssStart;
    m_Logic2EngineFunc[l2e_reset_abyss] = &UIManager::resetAbyss;
    m_Logic2EngineFunc[l2e_show_abyss_reset] = &UIManager::showAbyssReset;
    
    m_Logic2EngineFunc[l2e_show_hall_tower] = &UIManager::showHallTower;
    m_Logic2EngineFunc[l2e_show_tower_sweep] = &UIManager::showTowerSweep;
    m_Logic2EngineFunc[l2e_reset_tower] = &UIManager::resetTower;
    m_Logic2EngineFunc[l2e_start_tower_sweep] = &UIManager::startTowerSweep;
    m_Logic2EngineFunc[l2e_complet_tower_sweep] = &UIManager::completeTowerSweep;
    m_Logic2EngineFunc[l2e_tower_take_reward] = &UIManager::towerTakeReward;
    m_Logic2EngineFunc[l2e_speedup_tower_sweep] = &UIManager::speedupTowerSweep;
    m_Logic2EngineFunc[l2e_update_tower_sweep_sec] = &UIManager::updateTowerSweepCD;
    
    m_Logic2EngineFunc[l2e_show_hall_arena] = &UIManager::showHallArena;
    m_Logic2EngineFunc[l2e_show_arena_division] = &UIManager::showArenaDivision;
    m_Logic2EngineFunc[l2e_arena_start_match] = &UIManager::arenaStartMatch;
    m_Logic2EngineFunc[l2e_arena_show_robot] = &UIManager::arenaShowRobot;
    m_Logic2EngineFunc[l2e_update_arena_ticket] = &UIManager::updateArenaTicket;
    m_Logic2EngineFunc[l2e_buy_arena_ticket] = &UIManager::arenaBuyTicket;
    m_Logic2EngineFunc[l2e_update_arena_ticket_cd] = &UIManager::updateArenaTIcketCD;
    m_Logic2EngineFunc[l2e_arena_victory] = &UIManager::arenaVictory;
    m_Logic2EngineFunc[l2e_arena_fail] = &UIManager::arenaFail;
    m_Logic2EngineFunc[l2e_arena_reward] = &UIManager::showArenaReward;
    
    m_Logic2EngineFunc[l2e_start_plot] = &UIManager::startPlot;
    m_Logic2EngineFunc[l2e_show_dialog] = &UIManager::showDialog;
    m_Logic2EngineFunc[l2e_update_dialog] = &UIManager::updateDialog;
    m_Logic2EngineFunc[l2e_complete_dialog] = &UIManager::completeDialog;
    m_Logic2EngineFunc[l2e_complete_plot] = &UIManager::completePlot;
    m_Logic2EngineFunc[l2e_add_npc] = &UIManager::addNPC;
    m_Logic2EngineFunc[l2e_remove_npc] = &UIManager::removeNPC;
    
    m_Logic2EngineFunc[l2e_unlock_func] = &UIManager::unlockFunc;
    
    m_Logic2EngineFunc[l2e_show_hall_castle] = &UIManager::showHallCastle;
    m_Logic2EngineFunc[l2e_show_job] = &UIManager::showCastleJob;
    m_Logic2EngineFunc[l2e_change_job] = &UIManager::changeJob;
    m_Logic2EngineFunc[l2e_active_job] = &UIManager::activeJob;
    m_Logic2EngineFunc[l2e_active_all_job] = &UIManager::activeAllJob;
    m_Logic2EngineFunc[l2e_update_name] = &UIManager::updateName;
    
    m_Logic2EngineFunc[l2e_show_hall_everyday] = &UIManager::showHallEveryday;
    m_Logic2EngineFunc[l2e_show_everyday] = &UIManager::showEveryday;
    m_Logic2EngineFunc[l2e_update_first_recharge] = &UIManager::updateFirstRecharge;
    m_Logic2EngineFunc[l2e_hide_campaign] = &UIManager::hideCampaign;
    m_Logic2EngineFunc[l2e_show_campaign_active] = &UIManager::showCampaignActive;
    m_Logic2EngineFunc[l2e_update_mission_bound] = &UIManager::updateCampaignActiveMission;
    m_Logic2EngineFunc[l2e_update_active_box] = &UIManager::updateCampaignActiveBox;
    m_Logic2EngineFunc[l2e_refresh_campaign_active] = &UIManager::refreshCampaignActive;
    m_Logic2EngineFunc[l2e_update_free_diamond] = &UIManager::updateFreeDiamond;
    m_Logic2EngineFunc[l2e_show_campaign_bound_mission] = &UIManager::showCampaignBoundMission;
    m_Logic2EngineFunc[l2e_jump_locked] = &UIManager::jumpLocked;
    m_Logic2EngineFunc[l2e_show_month_assign] = &UIManager::showMonthAssign;
    
    m_Logic2EngineFunc[l2e_show_hall_lucky] = &UIManager::showHallLucky;
    m_Logic2EngineFunc[l2e_draw_normal_one] = &UIManager::showDrawNormalOne;
    m_Logic2EngineFunc[l2e_draw_special_one] = &UIManager::showDrawSpecialOne;
    m_Logic2EngineFunc[l2e_draw_special_ten] = &UIManager::showDrawSpecialTen;
    m_Logic2EngineFunc[l2e_update_draw_normal] = &UIManager::updateFreeNormal;
    m_Logic2EngineFunc[l2e_update_draw_special] = &UIManager::updateFreeSpecial;
    m_Logic2EngineFunc[l2e_update_lucky_cd] = &UIManager::updateLuckyCD;
    m_Logic2EngineFunc[l2e_update_special_ticket] = &UIManager::updateLuckySpecialTicket;
    
    m_Logic2EngineFunc[l2e_show_hall_shop] = &UIManager::showHallShop;
    m_Logic2EngineFunc[l2e_show_shop] = &UIManager::showShop;
    m_Logic2EngineFunc[l2e_show_package_detail] = &UIManager::showPackageDetail;
    m_Logic2EngineFunc[l2e_show_package_result] = &UIManager::showPackageResult;
    m_Logic2EngineFunc[l2e_show_hall_novice] = &UIManager::showHallNovice;
    
    m_Logic2EngineFunc[l2e_show_hall_vip] = &UIManager::showHallVIP;
    m_Logic2EngineFunc[l2e_show_vip_detail] = &UIManager::showVIPDetail;
    m_Logic2EngineFunc[l2e_take_vip_bound] = &UIManager::takeVIPBound;
    m_Logic2EngineFunc[l2e_hide_vip] = &UIManager::hideVIP;
    m_Logic2EngineFunc[l2e_update_hall_tip] = &UIManager::updateHallTip;
    
    m_Logic2EngineFunc[l2e_update_arms_tip] = &UIManager::updateArmsTip;
    m_Logic2EngineFunc[l2e_update_equip_tip] = &UIManager::updateEquipTip;
    m_Logic2EngineFunc[l2e_update_pet_tip] = &UIManager::updatePetTip;
    m_Logic2EngineFunc[l2e_update_campaign_tip] = &UIManager::updateCampaignTip;
    
    m_Logic2EngineFunc[l2e_update_discount_state] = &UIManager::updateDiscountState;
    m_Logic2EngineFunc[l2e_show_hall_discount] = &UIManager::showHallDiscount;
    
    m_Logic2EngineFunc[l2e_update_vit_cd] = &UIManager::updateVITCD;
    
    m_Logic2EngineFunc[l2e_show_custom_package] = &UIManager::showCustomPackage;
    m_Logic2EngineFunc[l2e_show_unlock_job] = &UIManager::showUnlockAllJob;
    
    replacing = false;
//    delayMsg.clear();
}

void UIManager::loop(float dt)
{
//    if (!replacing) {
//        if (!delayMsg.empty()) {
//            void* delay = delayMsg[delayMsg.size()-1];
//            processLogicResponse(delay);
//            delay = nullptr;
//            delayMsg.pop_back();
//        }
//    }
    ClientLogic::instance()->mainLoop(dt);
}

void UIManager::processLogicResponse(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    L2EProtocol	eProtocol	= *((L2EProtocol*)pMsg);
    if (eProtocol >= l2e_max)
        return;
    if (this->m_Logic2EngineFunc[eProtocol] != NULL)
    {
//        if (replacing) {
//            void* delay = new void*(pMsg);
//            memcpy(delay, pMsg, sizeof(pMsg));
//            delayMsg.push_back(delay);
//        }else{
            (this->*m_Logic2EngineFunc[eProtocol])(pMsg);
//        }
        
    }
}

void UIManager::showLogin(void *pMsg)
{
    auto scene = LoginScene::create();
    Director::getInstance()->runWithScene(scene);
}

void UIManager::updateNetState(void *pMsg)
{
    EventCustom event(UPDATE_NET_STATE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showLoadStage(void *pMsg)
{
    auto scene = GameScene::create();
    Director::getInstance()->replaceScene(scene);
    scene->showLoad();
    scene->replaceLoadImg(((L2E_SHOW_LOAD*)pMsg)->loadTipImg);
    replacing = true;
}

void UIManager::updateLoadStage(void *pMsg)
{
    EventCustom event(UPDATE_LOAD_STAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showFight(void *pMsg)
{
    EventCustom event(SHOW_FIGHT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::unlockView(void *pMsg)
{
    EventCustom event(UNLOCK_VIEW);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupTmxMap(void *pMsg)
{
    EventCustom event(SETUP_TMX_MAP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupTmxEffect(void *pMsg)
{
    EventCustom event(SETUP_TMX_MAP_EFFECT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupHero(void *pMsg)
{
    EventCustom event(SETUP_HERO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupWitness(void *pMsg)
{
    EventCustom event(SETUP_WITNESS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupOpponent(void *pMsg)
{
    EventCustom event(SETUP_OPPONENT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::preloadMonsterRes(void *pMsg)
{
    EventCustom event(PRELOAD_MONSTER_RES);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::setupMonsterTeam(void *pMsg)
{
    EventCustom event(SETUP_MONSTER_TEAM);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::readyVictory(void *pMsg)
{
    EventCustom event(READY_VICTORY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::stageVictory(void *pMsg)
{
    EventCustom event(STAGE_VICTORY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::stageFail(void *pMsg)
{
    EventCustom event(STAGE_FAIL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showRevive(void *pMsg)
{
    EventCustom event(SHOW_REVIVE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::revive(void *pMsg)
{
    EventCustom event(FIGHT_REVIVE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::stageTimeOver(void *pMsg)
{
    EventCustom event(STAGE_TIME_OVER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::stageStar(void *pMsg)
{
    EventCustom event(STAGE_STAR);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::stageUpdateWaveCount(void *pMsg)
{
    EventCustom event(STAGE_UPDATE_WAVE_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::fullHeroHp(void *pMsg)
{
    EventCustom event(FULL_HERO_HP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateHpBottlePrice(void *pMsg)
{
    EventCustom event(UPDATE_HP_BOTTLE_PRICE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateCombo(void *pMsg)
{
    EventCustom event(UPDATE_COMBO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::clearCombo(void *pMsg)
{
    EventCustom event(CLEAR_COMBO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::pauseFight(void *pMsg)
{
    EventCustom event(PAUSE_FIGHT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::resumeFight(void *pMsg)
{
    EventCustom event(RESUME_FIGHT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
//void UIManager::showChapterScene(void *pMsg)
//{
//    auto scene = HallScene::create();
////    if (Director::getInstance()->getRunningScene() != nullptr) {
//        Director::getInstance()->replaceScene(scene);
////    }else{
////        Director::getInstance()->runWithScene(scene);
////    }
//    replacing = true;
//}

void UIManager::showSystemLoad(void *pMsg)
{
    auto scene = SystemLoadScene::create();
    Director::getInstance()->replaceScene(scene);
    replacing = true;
}
void UIManager::updateSystemLoad(void *pMsg)
{
    EventCustom event(UPDATE_LOAD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallScene(void *pMsg)
{
    auto scene = HallScene::create();
    Director::getInstance()->replaceScene(scene);
    replacing = true;
    
    scene->setupView(pMsg);
    
}
void UIManager::showBattleChapters(void *pMsg)
{    
    EventCustom event(SHOW_BATTLE_CHAPTERS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//    HallScene::setDelayEvent(&event);
//    ((HallScene*)Director::getInstance()->getRunningScene())->showBattle(&event);
}
void UIManager::back2BattleChapters(void *pMsg)
{
    EventCustom event(SHOW_BATTLE_CHAPTERS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showChapterName(void *pMsg)
{
    EventCustom event(SHOW_CHAPTER_NAME);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showChapter(void *pMsg)
{
    EventCustom event(SHOW_CHAPTER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//    HallScene::setDelayEvent(&event);
//    ((HallScene*)Director::getInstance()->getRunningScene())->showChapter(&event);
}

void UIManager::showStage(void *pMsg)
{
    EventCustom event(SHOW_STAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//    HallScene::setDelayEvent(&event);
//    ((HallScene*)Director::getInstance()->getRunningScene())->showStage(&event);
}

void UIManager::startStage(void *pMsg)
{
    EventCustom event(START_STAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::showHallHero(void *pMsg)
{
    EventCustom event(SHOW_HALL_HERO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::selEquipPos(void *pMsg)
{
    EventCustom event(SEL_EQUIP_POS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateEquipLevel(void *pMsg)
{
    EventCustom event(UPDATE_EQUIP_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::updateEquipStar(void *pMsg)
{
    EventCustom event(UPDATE_EQUIP_STAR);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateItemCount(void *pMsg)
{
    EventCustom event(UPDATE_ITEM_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::sweepItem(void *pMsg)
{
    EventCustom event(SWEEP_ITEM);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::updateGold(void *pMsg)
{
    EventCustom event(UPDATE_GOLD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateDiamond(void *pMsg)
{
    EventCustom event(UPDATE_DIAMOND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateVIT(void *pMsg)
{
    EventCustom event(UPDATE_VIT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::updateExp(void *pMsg)
{
    EventCustom event(UPDATE_EXP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateForce(void *pMsg)
{
    EventCustom event(UPDATE_FORCE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showItemWay(void *pMsg)
{
    EventCustom event(SHOW_ITEM_WAY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showItemSweep(void *pMsg)
{
    EventCustom event(SHOW_ITEM_SWEEP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallPet(void *pMsg)
{
    EventCustom event(SHOW_HALL_PET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::selPetPos(void *pMsg)
{
    EventCustom event(SEL_PET_POS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updatePetLevel(void *pMsg)
{
    EventCustom event(UPDATE_PET_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFightPet(void *pMsg)
{
    EventCustom event(UPDATE_FIGHT_PET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallPetOffer(void *pMsg)
{
    EventCustom event(SHOW_HALL_PET_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateHallPetOfferAllCD(void *pMsg)
{
    EventCustom event(UPDATE_HALL_OFFER_ALL_CD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updatePetBoardOffer(void *pMsg)
{
    EventCustom event(UPDATE_PET_BOARD_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::openBoardOffer(void *pMsg)
{
    EventCustom event(OPEN_PET_BOARD_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::addOfferPet(void *pMsg)
{
    EventCustom event(ADD_OFFER_PET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::removeOfferPet(void *pMsg)
{
    EventCustom event(REMOVE_OFFER_PET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::workOfferOver(void *pMsg)
{
    EventCustom event(WORK_OFFER_OVER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::takeBoardOffer(void *pMsg)
{
    EventCustom event(TAKE_BOARD_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::takeOfferReward(void *pMsg)
{
    EventCustom event(TAKE_OFFER_REWARD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showBuyOffer(void *pMsg)
{
    EventCustom event(SHOW_BUY_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::buyOffer(void *pMsg)
{
    EventCustom event(BUY_OFFER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::resetOfferRefreshCount(void *pMsg)
{
    EventCustom event(RESET_OFFER_REFRESH_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallHolySword(void *pMsg)
{
    EventCustom event(SHOW_HALL_HOLY_SWORD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateSword(void *pMsg)
{
    EventCustom event(UPDATE_SWORD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFightSword(void *pMsg)
{
    EventCustom event(UPDATE_FIGHT_SWORD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateSwordLevel(void *pMsg)
{
    EventCustom event(UPDATE_SWORD_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallArms(void *pMsg)
{
    EventCustom event(SHOW_HALL_ARMS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArms(void *pMsg)
{
    EventCustom event(UPDATE_ARMS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArmsLevel(void *pMsg)
{
    EventCustom event(UPDATE_ARMS_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateGemCount(void *pMsg)
{
    EventCustom event(UPDATE_GEM_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::showFillGemBoard(void *pMsg)
{
    EventCustom event(SHOW_FILL_GEM);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArmsGem(void *pMsg)
{
    EventCustom event(UPDATE_ARMS_GEM);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallProv(void *pMsg)
{
    EventCustom event(SHOW_HALL_PROV);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::confirmProvStage(void *pMsg)
{
    EventCustom event(CONFIRM_PROV_STAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::confirmProvFighter(void *pMsg)
{
    EventCustom event(CONFIRM_PROV_FIGHTER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provFight(void *pMsg)
{
    EventCustom event(PROV_FIGHT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provWin(void *pMsg)
{
    EventCustom event(PROV_WIN);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provLose(void *pMsg)
{
    EventCustom event(PROV_LOSE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provReset(void *pMsg)
{
    EventCustom event(PROV_RESET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provAddTicket(void *pMsg)
{
    EventCustom event(PROV_ADD_TICKET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::provRefreshReset(void *pMsg)
{
    EventCustom event(PROV_REFRESH_RESET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::showHallBuyVIT(void *pMsg)
{
    EventCustom event(SHOW_HALL_BUY_VIT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showBuyVIT(void *pMsg)
{
    EventCustom event(SHOW_BUY_VIT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::buyVIT(void *pMsg)
{
    EventCustom event(BUY_VIT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateVITBuyCount(void *pMsg)
{
    EventCustom event(UPDATE_VIT_BUY_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showGetTip(void *pMsg)
{
    EventCustom event(SHOW_GET_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::openStarBox(void *pMsg)
{
    EventCustom event(OPEN_STAR_BOX);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showStarBounds(void *pMsg)
{
    EventCustom event(SHOW_STAR_BOUNDS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallAbyss(void *pMsg)
{
    EventCustom event(SHOW_HALL_ABYSS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::checkAbyssStart(void *pMsg)
{
    EventCustom event(CHECK_START_ABYSS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::resetAbyss(void *pMsg)
{
    EventCustom event(RESET_ABYSS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showAbyssReset(void *pMsg)
{
    EventCustom event(SHOW_RESET_ABYSS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallTower(void *pMsg)
{
    EventCustom event(SHOW_HALL_TOWER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showTowerSweep(void *pMsg)
{
    EventCustom event(SHOW_TOWER_SWEEP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::resetTower(void *pMsg)
{
    EventCustom event(RESET_TOWER);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::startTowerSweep(void *pMsg)
{
    EventCustom event(START_TOWER_SWEEP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::completeTowerSweep(void *pMsg)
{
    EventCustom event(COMPLETE_TOWER_SWEEP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::towerTakeReward(void *pMsg)
{
    EventCustom event(TOWER_TAKE_REWARD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::speedupTowerSweep(void *pMsg)
{
    EventCustom event(SPEEDUP_TOWER_SWEEP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateTowerSweepCD(void *pMsg)
{
    EventCustom event(UPDATE_TOWER_SWEEP_SEC);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallArena(void *pMsg)
{
    EventCustom event(SHOW_HALL_ARENA);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showArenaDivision(void *pMsg)
{
    EventCustom event(SHOW_ARENA_DIVISION);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::arenaStartMatch(void *pMsg)
{
    EventCustom event(ARENA_START_MATCH_OPP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::arenaShowRobot(void *pMsg)
{
    EventCustom event(ARENA_SHOW_ROBOT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArenaTicket(void *pMsg)
{
    EventCustom event(UPDATE_ARENA_TICKET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::arenaBuyTicket(void *pMsg)
{
    EventCustom event(BUY_ARENA_TICKET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArenaTIcketCD(void *pMsg)
{
    EventCustom event(UPDATE_ARENA_TICKET_CD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::arenaVictory(void *pMsg)
{
    EventCustom event(ARENA_VICTORY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::arenaFail(void *pMsg)
{
    EventCustom event(ARENA_FAIL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showArenaReward(void *pMsg)
{
    EventCustom event(SHOW_ARENA_REWARD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::startPlot(void *pMsg)
{
    EventCustom event(START_PLOT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showDialog(void *pMsg)
{
    EventCustom event(SHOW_DIALOG);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateDialog(void *pMsg)
{
    EventCustom event(UPDATE_DIALOG);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::completeDialog(void *pMsg)
{
    EventCustom event(COMPLETE_DIALOG);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::completePlot(void *pMsg)
{
    EventCustom event(COMPLETE_PLOT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::addNPC(void *pMsg)
{
    EventCustom event(ADD_NPC);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::removeNPC(void *pMsg)
{
    EventCustom event(REMOVE_NPC);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::unlockFunc(void *pMsg)
{
    EventCustom event(UNLOCK_FUNC);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallCastle(void *pMsg)
{
    EventCustom event(SHOW_HALL_CASTLE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCastleJob(void *pMsg)
{
    EventCustom event(SHOW_CASTLE_JOB);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::changeJob(void *pMsg)
{
    EventCustom event(CHANGE_JOB);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::activeJob(void *pMsg)
{
    EventCustom event(ACTIVE_JOB);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::activeAllJob(void *pMsg)
{
    EventCustom event(ACTIVE_ALL_JOB);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallEveryday(void *pMsg)
{
    EventCustom event(SHOW_HALL_EVERYDAY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::showEveryday(void *pMsg)
{
    EventCustom event(SHOW_EVERYDAY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallLucky(void *pMsg)
{
    EventCustom event(SHOW_HALL_LUCKY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showDrawNormalOne(void *pMsg)
{
    EventCustom event(SHOW_DRAW_NORMAL_ONE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showDrawSpecialOne(void *pMsg)
{
    EventCustom event(SHOW_DRAW_SPECIAL_ONE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showDrawSpecialTen(void *pMsg)
{
    EventCustom event(SHOW_DRAW_SPECIAL_TEN);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFreeNormal(void *pMsg)
{
    EventCustom event(UPDATE_FREE_NORMAL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFreeSpecial(void *pMsg)
{
    EventCustom event(UPDATE_FREE_SPECIAL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateLuckyCD(void *pMsg)
{
    EventCustom event(UPDATE_LUCKY_CD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateLuckySpecialTicket(void *pMsg)
{
    EventCustom event(UPDATE_SPECIAL_TICKET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallShop(void *pMsg)
{
    EventCustom event(SHOW_HALL_SHOP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showShop(void *pMsg)
{
    EventCustom event(SHOW_SHOP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showPackageDetail(void *pMsg)
{
    EventCustom event(SHOW_PACKAGE_DEATIL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showPackageResult(void *pMsg)
{
    EventCustom event(SHOW_PACKAGE_RESULT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallNovice(void *pMsg)
{
    EventCustom event(SHOW_HALL_NOVICE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFirstRecharge(void *pMsg)
{
    EventCustom event(UPDATE_FIRST_RECHARGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::hideCampaign(void *pMsg)
{
    EventCustom event(HIDE_CAMPAIGN);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallVIP(void *pMsg)
{
    EventCustom event(SHOW_HALL_VIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showVIPDetail(void *pMsg)
{
    EventCustom event(SHOW_VIP_DETAIL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::takeVIPBound(void *pMsg)
{
    EventCustom event(TAKE_VIP_BOUND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::hideVIP(void *pMsg)
{
    EventCustom event(HIDE_VIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCampaignActive(void *pMsg)
{
    EventCustom event(SHOW_CAMPAIGN_ACTIVE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateCampaignActiveMission(void *pMsg)
{
    EventCustom event(UPDATE_CAMPAIGN_MISSION_BOUND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);    
}

void UIManager::updateCampaignActiveBox(void *pMsg)
{
    EventCustom event(UPDATE_CAMPAIGN_ACTIVE_BOX);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::refreshCampaignActive(void *pMsg)
{
    EventCustom event(UPDATE_CAMPAIGN_ACTIVE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateFreeDiamond(void *pMsg)
{
    EventCustom event(UPDATE_FREE_DIAMOND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCampaignBoundMission(void *pMsg)
{
    EventCustom event(SHOW_CAMPAIGN_BOUND_MISSION);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}


void UIManager::jumpLocked(void *pMsg)
{
    EventCustom event(JUMP_LOCKED);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showMonthAssign(void *pMsg)
{
    EventCustom event(UPDATE_CAMPAIGN_MONTH_ASSIGN);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateName(void *pMsg)
{
    EventCustom event(UPDATE_NAME);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateHallTip(void *pMsg)
{
    EventCustom event(UPDATE_HALL_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateArmsTip(void *pMsg)
{
    EventCustom event(UPDATE_ARMS_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateEquipTip(void *pMsg)
{
    EventCustom event(UPDATE_EQUIP_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updatePetTip(void *pMsg)
{
    EventCustom event(UPDATE_PET_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateCampaignTip(void *pMsg)
{
    EventCustom event(UPDATE_CAMPAIGN_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateDiscountState(void *pMsg)
{
    EventCustom event(UPDATE_DISCOUNT_STATE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallDiscount(void *pMsg)
{
    EventCustom event(SHOW_HALL_DISCOUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateVITCD(void *pMsg)
{
    EventCustom event(UPDATE_VIT_CD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCustomPackage(void *pMsg)
{
    EventCustom event(SHOW_CUSTOM_PACKAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showUnlockAllJob(void *pMsg)
{
    EventCustom event(SHOW_UNLOCK_JOB);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
