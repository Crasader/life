//
//  TimeManager.cpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#include "TimeManager.h"
#include "TLProtocol.h"
#include "GameDefine.h"
#include "logic/ClientLogic.h"
#include "utils/TimeUtil.h"

TimeManager::TimeManager()
{
    data = GameTime::instance();
    start = false;
    
    memset(m_Client2TimeFunc, 0, sizeof(m_Client2TimeFunc));
    
    m_Client2TimeFunc[l2t_start_app] = &TimeManager::startApp;
    m_Client2TimeFunc[l2t_start_restore_vit] = &TimeManager::startRestoreVIT;
    m_Client2TimeFunc[l2t_reset_offer_board] = &TimeManager::resetOfferBoardRefresh;
    m_Client2TimeFunc[l2t_get_offer_board_cd] = &TimeManager::getOfferBoardRefreshCD;
    m_Client2TimeFunc[l2t_get_offer_all_cd] = &TimeManager::updateOfferAllCD;
    m_Client2TimeFunc[l2t_start_work_offer] = &TimeManager::startWorkOffer;
    m_Client2TimeFunc[l2t_stop_work_offer] = &TimeManager::stopWorkOffer;
    m_Client2TimeFunc[l2t_refresh_prov_reset] = &TimeManager::refreshProvReset;
    m_Client2TimeFunc[l2t_reset_vit_buy_count] = &TimeManager::resetVITBuyCount;
    m_Client2TimeFunc[l2t_start_tower_sweep] = &TimeManager::startTowerSweep;
    m_Client2TimeFunc[l2t_stop_tower_sweep] = &TimeManager::stopTowerSweep;
    m_Client2TimeFunc[l2t_refresh_tower_reset] = &TimeManager::refreshTowerReset;
    m_Client2TimeFunc[l2t_get_tower_sweep_cd] = &TimeManager::updateTowerSweepCD;
    m_Client2TimeFunc[l2t_refresh_abyss] = &TimeManager::refreshAbyss;
    m_Client2TimeFunc[l2t_restore_arena_ticket] = &TimeManager::restoreArenaTicket;
    m_Client2TimeFunc[l2t_stop_arena_ticket_restore] = &TimeManager::stopArenaTicketRestore;
    m_Client2TimeFunc[l2t_get_arena_ticket_cd] = &TimeManager::getArenaTicketCD;
    m_Client2TimeFunc[l2t_refresh_arena_reward] = &TimeManager::refreshArenaReward;
    m_Client2TimeFunc[l2t_refresh_everyday] = &TimeManager::refreshEveryday;
    m_Client2TimeFunc[l2t_refresh_month_assign] = &TimeManager::refreshMonthAssign;
    m_Client2TimeFunc[l2t_restore_draw_normal] = &TimeManager::restoreDrawNormal;
    m_Client2TimeFunc[l2t_restore_draw_special] = &TimeManager::restoreDrawSpecial;
    m_Client2TimeFunc[l2t_get_lucky_cd] = &TimeManager::getLuckyCD;
    m_Client2TimeFunc[l2t_refresh_campaign_active] = &TimeManager::refreshCampaignActive;
    m_Client2TimeFunc[l2t_reset_free_diamond] = &TimeManager::resetFreeDiamond;
    m_Client2TimeFunc[l2t_wait_discount_open] = &TimeManager::waitDiscountOpen;
    m_Client2TimeFunc[l2t_wait_discount_close] = &TimeManager::waitDiscountClose;
    m_Client2TimeFunc[l2t_get_discount_close_cd] = &TimeManager::getDiscountCloseSec;
    m_Client2TimeFunc[l2t_get_vit_restore_sec] = &TimeManager::getVITRestoreCD;
    m_Client2TimeFunc[l2t_reset_petoffer_count] = &TimeManager::resetPetOfferCount;
}

void TimeManager::processClientQuest(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    L2TProtocol	eProtocol	= *((L2TProtocol*)pMsg);
    if (eProtocol >= l2t_max)
        return;
    if (this->m_Client2TimeFunc[eProtocol] != NULL)
    {
        (this->*m_Client2TimeFunc[eProtocol])(pMsg);
    }
}

void TimeManager::mainLoop(float dt)
{
    if (!start)
    {
        return;
    }
    long currSec = TimeUtil::get_system_tick_s();
    for (auto event : data->eventMap) {
        if (event.second.timeFlag <= currSec) {
            data->invalidEvent(event.first);
            T2L_COMMON info;
            info.eProtocol = event.second.eProtocol;
            info.passSec = currSec-event.second.timeFlag;
            ClientLogic::instance()->ProcessTimeResponse(&info);
        }
    }
    if(data->removeDeadEvent()) {
        saveData();
    }
}

void TimeManager::loadData()
{
//    data->deleteLoginJson();
//    data->deleteFlagJson();
//    data->readLoginJson();
    data->readFlagJson();
}

void TimeManager::saveData()
{
//    data->save2LoginJson();
    data->save2FlagJson();
}

void TimeManager::startApp(void *pMsg)
{
    loadData();
    long currSec = TimeUtil::get_system_tick_s();
    bool changed = false;
    if (data->eventMap.find(t2l_refresh_offer_board) == data->eventMap.end()) {
        changed = true;
        data->addEvent(t2l_refresh_offer_board, currSec + offerBoardRefreshSec);
    }
    if (data->eventMap.find(t2l_refresh_prov_reset) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_prov_reset, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_reset_vit_buy_count) == data->eventMap.end()) {
        data->addEvent(t2l_reset_vit_buy_count, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_refresh_tower_reset) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_tower_reset, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_refresh_abyss) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_abyss, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_refresh_arena_reward) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_arena_reward, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_add_vip_login_day) == data->eventMap.end()) {
        data->addEvent(t2l_add_vip_login_day, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_refresh_campaign_active) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_campaign_active, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_reset_free_diamond) == data->eventMap.end()) {
        data->addEvent(t2l_reset_free_diamond, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (data->eventMap.find(t2l_reset_petoffer_count) == data->eventMap.end()) {
        data->addEvent(t2l_reset_petoffer_count, TimeUtil::getTimeWithTomorrowHour(1, 0));
        changed = true;
    }
    if (changed) {
        saveData();
    }
    start = true;
}

void TimeManager::startRestoreVIT(void *pMsg)
{
    if(data->eventMap.find(t2l_restore_vit) != data->eventMap.end())
    {
        if (data->eventMap[t2l_restore_vit].alive) {
            return;
        }
    }
    L2T_RESTORE_VIT info = *static_cast<L2T_RESTORE_VIT*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_restore_vit, currSec+info.restoreSec);
    saveData();
}


void TimeManager::resetOfferBoardRefresh(void *pMsg)
{
    L2T_RESET_OFFER_BOARD info = *static_cast<L2T_RESET_OFFER_BOARD*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_refresh_offer_board, currSec+info.refreshSec);
    saveData();
}

void TimeManager::startWorkOffer(void *pMsg)
{
    L2T_START_WORK_OFFER info = *static_cast<L2T_START_WORK_OFFER*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    T2LProtocol eProtocol;
    switch (info.pos) {
        case 1:
        {
            eProtocol = t2l_work_offer1_over;
        }
            break;
        case 2:
        {
            eProtocol = t2l_work_offer2_over;
        }
            break;
        case 3:
        {
            eProtocol = t2l_work_offer3_over;
        }
            break;
            
        default:
            break;
    }
    data->addEvent(eProtocol, currSec+info.cd);
    saveData();
}

void TimeManager::startTowerSweep(void *pMsg)
{
    if(data->eventMap.find(t2l_complete_tower_sweep) != data->eventMap.end())
    {
        if (data->eventMap[t2l_complete_tower_sweep].alive) {
            return;
        }
    }
    L2T_START_TOWER_SWEEP info = *static_cast<L2T_START_TOWER_SWEEP*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_complete_tower_sweep, currSec+info.sweepSec);
    saveData();
}
void TimeManager::stopWorkOffer(void *pMsg)
{
    L2T_STOP_WORK_OFFER info = *static_cast<L2T_STOP_WORK_OFFER*>(pMsg);
    T2LProtocol eProtocol;
    switch (info.pos) {
        case 1:
        {
            eProtocol = t2l_work_offer1_over;
        }
            break;
        case 2:
        {
            eProtocol = t2l_work_offer2_over;
        }
            break;
        case 3:
        {
            eProtocol = t2l_work_offer3_over;
        }
            break;
            
        default:
            break;
    }
    data->invalidEvent(eProtocol);
}
void TimeManager::stopTowerSweep(void *pMsg)
{
    if(data->eventMap.find(t2l_complete_tower_sweep) == data->eventMap.end())
    {
        return;
    }
    data->invalidEvent(t2l_complete_tower_sweep);
}
void TimeManager::getOfferBoardRefreshCD(void *pMsg)
{
    long currSec = TimeUtil::get_system_tick_s();
    long cd = data->eventMap[t2l_refresh_offer_board].timeFlag - currSec;
    T2L_COMMON info;
    info.eProtocol = t2l_offer_board_cd;
    info.passSec = cd;
    ClientLogic::instance()->ProcessTimeResponse(&info);
}
void TimeManager::updateOfferAllCD(void *pMsg)
{
    long currSec = TimeUtil::get_system_tick_s();
    T2L_UPDATE_OFFER_ALL_CD info;
    info.eProtocol = t2l_update_offer_all_cd;
    if (data->eventMap.find(t2l_refresh_offer_board) == data->eventMap.end()) {
        info.refreshLeftSec = -1;
    }else{
        info.refreshLeftSec = data->eventMap[t2l_refresh_offer_board].timeFlag - currSec;
    }
    if (data->eventMap.find(t2l_work_offer1_over) == data->eventMap.end()) {
        info.work1LeftSec = -1;
    }else{
        info.work1LeftSec = data->eventMap[t2l_work_offer1_over].timeFlag - currSec;
    }
    if (data->eventMap.find(t2l_work_offer2_over) == data->eventMap.end()) {
        info.work2LeftSec = -1;
    }else{
        info.work2LeftSec = data->eventMap[t2l_work_offer2_over].timeFlag - currSec;
    }
    if (data->eventMap.find(t2l_work_offer3_over) == data->eventMap.end()) {
        info.work3LeftSec = -1;
    }else{
        info.work3LeftSec = data->eventMap[t2l_work_offer3_over].timeFlag - currSec;
    }
    ClientLogic::instance()->ProcessTimeResponse(&info);
}
void TimeManager::getLuckyCD(void *pMsg)
{
    long currSec = TimeUtil::get_system_tick_s();
    T2L_UPDATE_LUCKY_CD info;
    info.eProtocol = t2l_update_lucky_cd;
    if (data->eventMap.find(t2l_restore_draw_normal) == data->eventMap.end()) {
        info.normalLeftCD = 0;
    }else{
        info.normalLeftCD = data->eventMap[t2l_restore_draw_normal].timeFlag - currSec;
    }
    if (data->eventMap.find(t2l_restore_draw_special) == data->eventMap.end()) {
        info.specialLeftCD = 0;
    }else{
        info.specialLeftCD = data->eventMap[t2l_restore_draw_special].timeFlag - currSec;
    }
    
    ClientLogic::instance()->ProcessTimeResponse(&info);
}

void TimeManager::resetPetOfferCount(void *pMsg)
{
    if (data->eventMap.find(t2l_reset_petoffer_count) == data->eventMap.end()) {
        data->addEvent(t2l_reset_petoffer_count, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}
void TimeManager::refreshProvReset(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_prov_reset) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_prov_reset, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::resetVITBuyCount(void *pMsg)
{
    if (data->eventMap.find(t2l_reset_vit_buy_count) == data->eventMap.end()) {
        data->addEvent(t2l_reset_vit_buy_count, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshTowerReset(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_tower_reset) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_tower_reset, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshAbyss(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_abyss) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_abyss, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshArenaReward(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_arena_reward) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_arena_reward, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshEveryday(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_everyday) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_everyday, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshMonthAssign(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_month_assign) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_month_assign, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::refreshCampaignActive(void *pMsg)
{
    if (data->eventMap.find(t2l_refresh_campaign_active) == data->eventMap.end()) {
        data->addEvent(t2l_refresh_campaign_active, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::resetFreeDiamond(void *pMsg)
{
    if (data->eventMap.find(t2l_reset_free_diamond) == data->eventMap.end()) {
        data->addEvent(t2l_reset_free_diamond, TimeUtil::getTimeWithTomorrowHour(1, 0));
        saveData();
    }
}

void TimeManager::waitDiscountOpen(void *pMsg)
{
    L2T_WAIT_DISCOUNT_OPEN info = *static_cast<L2T_WAIT_DISCOUNT_OPEN*>(pMsg);
    if (data->eventMap.find(t2l_open_discount) == data->eventMap.end()) {
        data->addEvent(t2l_open_discount, info.openSec);
        saveData();
    }
}

void TimeManager::waitDiscountClose(void *pMsg)
{
    L2T_WAIT_DISCOUNT_CLOSE info = *static_cast<L2T_WAIT_DISCOUNT_CLOSE*>(pMsg);
    if (data->eventMap.find(t2L_close_discount) == data->eventMap.end()) {
        data->addEvent(t2L_close_discount, info.closeSec);
        saveData();
    }
}

void TimeManager::getDiscountCloseSec(void *pMsg)
{
    T2L_COMMON info;
    long currSec = TimeUtil::get_system_tick_s();
    long cd = 0;
    info.eProtocol = t2l_update_discount_close_sec;
    if (data->eventMap.find(t2L_close_discount) == data->eventMap.end()) {
        info.passSec = 0;
    }else{
        cd = data->eventMap[t2L_close_discount].timeFlag - currSec;
        info.passSec = cd;
    }
    ClientLogic::instance()->ProcessTimeResponse(&info);
}

void TimeManager::getVITRestoreCD(void *pMsg)
{
    T2L_COMMON info;
    long currSec = TimeUtil::get_system_tick_s();
    long cd = 0;
    info.eProtocol = t2l_update_vit_restore_cd;
    if (data->eventMap.find(t2l_restore_vit) == data->eventMap.end()) {
        info.passSec = 0;
    }else{
        cd = data->eventMap[t2l_restore_vit].timeFlag - currSec;
        info.passSec = cd;
    }
    ClientLogic::instance()->ProcessTimeResponse(&info);

}

void TimeManager::restoreArenaTicket(void *pMsg)
{
    if(data->eventMap.find(t2l_restore_arena_ticket) != data->eventMap.end())
    {
        if (data->eventMap[t2l_restore_arena_ticket].alive) {
            return;
        }
    }
    L2T_RESTORE_ARENA_TICKET info = *static_cast<L2T_RESTORE_ARENA_TICKET*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_restore_arena_ticket, currSec+info.restoreSec);
    saveData();
}

void TimeManager::restoreDrawNormal(void *pMsg)
{
    if(data->eventMap.find(t2l_restore_draw_normal) != data->eventMap.end())
    {
        if (data->eventMap[t2l_restore_draw_normal].alive) {
            return;
        }
    }
    L2T_RESTORE_DRAW_TICKET info = *static_cast<L2T_RESTORE_DRAW_TICKET*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_restore_draw_normal, currSec+info.restoreSec);
    saveData();
}

void TimeManager::restoreDrawSpecial(void *pMsg)
{
    if(data->eventMap.find(t2l_restore_draw_special) != data->eventMap.end())
    {
        if (data->eventMap[t2l_restore_draw_special].alive) {
            return;
        }
    }
    L2T_RESTORE_DRAW_TICKET info = *static_cast<L2T_RESTORE_DRAW_TICKET*>(pMsg);
    long currSec = TimeUtil::get_system_tick_s();
    data->addEvent(t2l_restore_draw_special, currSec+info.restoreSec);
    saveData();
}

void TimeManager::stopArenaTicketRestore(void *pMsg)
{
    if(data->eventMap.find(t2l_restore_arena_ticket) == data->eventMap.end())
    {
        return;
    }
    data->invalidEvent(t2l_restore_arena_ticket);
}

void TimeManager::getArenaTicketCD(void *pMsg)
{
    long currSec = TimeUtil::get_system_tick_s();
    long cd = -1;
    if(data->eventMap.find(t2l_restore_arena_ticket) != data->eventMap.end())
    {
        cd = data->eventMap[t2l_restore_arena_ticket].timeFlag - currSec;
    }
    
    T2L_COMMON info;
    info.eProtocol = t2l_update_arena_ticket_cd;
    info.passSec = cd;
    ClientLogic::instance()->ProcessTimeResponse(&info);
}

void TimeManager::updateTowerSweepCD(void *pMsg)
{
    long currSec = TimeUtil::get_system_tick_s();
    long cd = -1;
    if(data->eventMap.find(t2l_complete_tower_sweep) != data->eventMap.end())
    {
        cd = data->eventMap[t2l_complete_tower_sweep].timeFlag - currSec;
    }
    
    T2L_COMMON info;
    info.eProtocol = t2l_update_tower_sweep_cd;
    info.passSec = cd;
    ClientLogic::instance()->ProcessTimeResponse(&info);
}
