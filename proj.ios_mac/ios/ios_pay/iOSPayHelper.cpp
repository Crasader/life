//
//  iOSHelper.cpp
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#include "iOSPayHelper.h"
#include "IOSIAP_Bridge.h"
//#include "JZVerManager.h"

#include "CSProtocol.h"
#include "logic/ClientLogic.h"



iOSPayHelper *iOSPayHelper::instance = nullptr;

iOSPayHelper *iOSPayHelper::getInstance()
{
    if (instance == nullptr) {
        instance = new iOSPayHelper;
        instance->pidMap.clear();
    }
    
    return instance;
}

void iOSPayHelper::pay(int idx)
{
//    paySuccess(idx);
    auto iap = new IOSiAP_Bridge;
    iap->initPidMap(pidMap);
    if(!iap->canMakePay())
    {
        return;
    }
    iap->requestProducts(idx);
}

void iOSPayHelper::pushPid(int pid, std::string idenStr)
{
    pidMap[pid] = idenStr;
}

void iOSPayHelper::paySuccess(int idx)
{
//    cocos2d::ValueMap params;
//    params["data"] = result;
//    PublicSendLuaData::getInstance()->callLuaFuncPar("out/app/layers/PaymentLayer.luac", "onIAPPayResult", params);
//    if (result.status == 0) {
//        CGlobalUserInfo   * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
//        pGlobalUserInfo->GetUserInsureInfo()->lUserScore += result.money;
//        pGlobalUserInfo->GetGlobalUserData()->iTelephoneBill += result.phoneFee;
//        pGlobalUserInfo->GetGlobalUserData()->iTreasureMapNum += result.Treature;
//        pGlobalUserInfo->GetGlobalUserData()->iVip = result.vip;
//
//        char str[255] = {0};
//
//        sprintf(str, "%lld", pGlobalUserInfo->GetUserInsureInfo()->lUserScore);
//        G_NOTIFY(UPDATE_SCORE_NOTIFY, MTData::create(0, 0, 0, str));
//
//        sprintf(str, "%.1f", pGlobalUserInfo->GetGlobalUserData()->iTelephoneBill);
//        G_NOTIFY(UPDATE_PHONE_FEE_NOTIFY, MTData::create(0, 0, 0, str));
//
//        G_NOTIFY(UPDATE_VIP_NOTIFY, MTData::create(pGlobalUserInfo->GetGlobalUserData()->iVip));
//        
//        G_NOTIFY(ON_RECHARGE_UPDATE_RMBCOST, MTData::create(result.RechargeAll));
//        
//        if (result.Treature) {
//            G_NOTIFY(ON_VIP_GIFT_CHARGE_COMPLETE, 0);
//        }
//        
//        if (result.phoneFee) {
//            G_NOTIFY(ON_FIRST_RECHARGE_COMPLETE, 0);
//        }
        
//        G_NOTIFY(UPDATE_SCORE_IN_GAME, MTData::create(result.money));
//    }
//    else {
//        //notify hide shield
//        G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
//    }

    S2C_PAY info;
    info.eProtocol = s2c_pay;
    info.packageId = idx;
    info.success = true;
    ClientLogic::instance()->ProcessServiceResponse(&info);
}

void iOSPayHelper::payError(int idx)
{
    S2C_PAY info;
    info.eProtocol = s2c_pay;
    info.packageId = idx;
    info.success = false;
    ClientLogic::instance()->ProcessServiceResponse(&info);
}
