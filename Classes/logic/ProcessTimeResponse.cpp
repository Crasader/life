//
//  ProcessTimeResponse.cpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#include "ClientLogic.h"
#include "SystemLogic.h"
#include "ChapterLogic.h"

void ClientLogic::ProcessTimeResponse(void *pMsg)
{
    //    cocos2d::log("ClientLogic::ProcessServiceResponse");
    if (NULL == pMsg)
        return;
    
    //    cocos2d::log("NULL != pMsg");
    T2LProtocol	eProtocol	= *((T2LProtocol*)pMsg);
    if (eProtocol >= t2l_max)
        return;
    
    //    cocos2d::log("eProtocol < s2c_max");
    //分发到各个协议的处理函数
    if (this->m_Time2ClientFunc[eProtocol] != NULL)
    {
        //        cocos2d::log("分发到各个协议的处理函数");
        (this->*m_Time2ClientFunc[eProtocol])(pMsg);
    }
}

void ClientLogic::TimeRestoreVIT(void *pMsg)
{
    T2L_COMMON *pMsgInfo = (T2L_COMMON*)pMsg;
    SystemLogic::instance()->restoreVIT(*pMsgInfo);
}

void ClientLogic::TimeRestoreArenaTicket(void *pMsg)
{
    T2L_COMMON *pMsgInfo = (T2L_COMMON*)pMsg;
    SystemLogic::instance()->restoreArenaTicket(*pMsgInfo);
}

void ClientLogic::TimeUpdateOfferAllCD(void *pMsg)
{
    T2L_UPDATE_OFFER_ALL_CD *pMsgInfo = (T2L_UPDATE_OFFER_ALL_CD*)pMsg;
    SystemLogic::instance()->updateAllOfferCD(*pMsgInfo);
}

void ClientLogic::TimeRefreshBoardOffer(void *pMsg)
{
    SystemLogic::instance()->refreshBoardOffer();
}

void ClientLogic::TimeWorkOffer1Over(void *pMsg)
{
    SystemLogic::instance()->workOfferOver(1);
}

void ClientLogic::TimeWorkOffer2Over(void *pMsg)
{
    SystemLogic::instance()->workOfferOver(2);
}
void ClientLogic::TimeWorkOffer3Over(void *pMsg)
{
    SystemLogic::instance()->workOfferOver(3);
}

void ClientLogic::TimeRefreshProvReset(void *pMsg)
{
    SystemLogic::instance()->provRefreshReset();
}

void ClientLogic::TimeResetVITBuyCount(void *pMsg)
{
    SystemLogic::instance()->resetVITBuyCount();
}

void ClientLogic::TimeCompleteTowerSweep(void *pMsg)
{
    ChapterLogic::instance()->completeTowerSweep();
}

void ClientLogic::TimeRefreshTowerReset(void *pMsg)
{
    ChapterLogic::instance()->refreshTowerReset();
}

void ClientLogic::TimeUpdateTowerSweepCD(void *pMsg)
{
    T2L_COMMON *pMsgInfo = (T2L_COMMON*)pMsg;
    ChapterLogic::instance()->updateTowerSweepCD(*pMsgInfo);
}
void ClientLogic::TimeUpdateArenaTicketCD(void *pMsg)
{
    T2L_COMMON *pMsgInfo = (T2L_COMMON*)pMsg;
    SystemLogic::instance()->updateArenaTicketCD(*pMsgInfo);
}

void ClientLogic::TimeRefreshArenaReward(void *pMsg)
{
    SystemLogic::instance()->refreshArenaReward();
}

void ClientLogic::TimeRefreshEveryday(void *pMsg)
{
    SystemLogic::instance()->refreshEveryday();
}

void ClientLogic::TimeRefreshMonthAssign(void *pMsg)
{
    SystemLogic::instance()->refreshMonthAssign();
}

void ClientLogic::TimeRestoreDrawNormal(void *pMsg)
{
    SystemLogic::instance()->restoreDrawNormal();
}

void ClientLogic::TimeRestoreDrawSpecial(void *pMsg)
{
    SystemLogic::instance()->restoreDrawSpecial();
}

void ClientLogic::TimeUpdateLuckyCD(void *pMsg)
{
    T2L_UPDATE_LUCKY_CD *pMsgInfo = (T2L_UPDATE_LUCKY_CD*)pMsg;
    SystemLogic::instance()->updateLuckyCD(*pMsgInfo);
}

void ClientLogic::TimeAddVipLoginDay(void *pMsg)
{
    SystemLogic::instance()->addVipLoginDay();
}

void ClientLogic::TimeRefreshCampaignActive(void *pMsg)
{
    SystemLogic::instance()->refreshCampaignActive();
}

void ClientLogic::TimeResetFreeDiamond(void *pMsg)
{
    SystemLogic::instance()->resetFreeDiamond();
}

void ClientLogic::TimeRefreshAbyss(void *pMsg)
{
    ChapterLogic::instance()->refreshAbyss();
}

void ClientLogic::TimeOpenDiscount(void *pMsg)
{
    SystemLogic::instance()->openDiscount();
}

void ClientLogic::TimeCloseDiscount(void *pMsg)
{
    SystemLogic::instance()->closeDiscount();
}

void ClientLogic::TimeUpdateDiscountCloseSec(void *pMsg)
{
    T2L_COMMON info = *static_cast<T2L_COMMON*>(pMsg);
    SystemLogic::instance()->updateDiscountState(info.passSec);
}

void ClientLogic::TimeUpdateVITCD(void *pMsg)
{
    T2L_COMMON *pMsgInfo = (T2L_COMMON*)pMsg;
    SystemLogic::instance()->updateVITCD(*pMsgInfo);
}

void ClientLogic::TimeResetPetOfferCount(void *pMsg)
{
    SystemLogic::instance()->resetOfferCount();
}
