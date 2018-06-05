//
//  SericeManager.cpp
//  owen
//
//  Created by ff on 16/8/9.
//
//

#include "ServiceManager.h"
#include "logic/ClientLogic.h"

#include "net/HttpManager.h"

#ifdef UMENG
#include "MobClickCpp.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSPayHelper.h"
#include "iOSHelper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AndroidHelper.h"
#endif

ServiceManager::ServiceManager()
{
    memset(m_Client2ServiceFunc, 0, sizeof(m_Client2ServiceFunc));
    
    m_Client2ServiceFunc[c2s_check_connect] = &ServiceManager::checkConnect;
    m_Client2ServiceFunc[c2s_exit_app] = &ServiceManager::exitApp;
    m_Client2ServiceFunc[c2s_pay] = &ServiceManager::pay;
    m_Client2ServiceFunc[c2s_umeng_track] = &ServiceManager::umengTrack;
    m_Client2ServiceFunc[c2s_umeng_exchange] = &ServiceManager::umengExchange;
    m_Client2ServiceFunc[c2s_umeng_buy] = &ServiceManager::umengBuy;
    m_Client2ServiceFunc[c2s_umeng_use] = &ServiceManager::umengUse;
    m_Client2ServiceFunc[c2s_umeng_user_level] = &ServiceManager::umengUserLevel;
    m_Client2ServiceFunc[c2s_umeng_enter_stage_level] = &ServiceManager::umengEnterStage;
    m_Client2ServiceFunc[c2s_umeng_finish_stage_level] = &ServiceManager::umengFinishStage;
    m_Client2ServiceFunc[c2s_umeng_fail_stage_level] = &ServiceManager::umengFailStage;
    
    m_Client2ServiceFunc[c2s_server_time_req] = &ServiceManager::reqServerTime;
}

void ServiceManager::processClientQuest(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    C2SProtocol	eProtocol	= *((C2SProtocol*)pMsg);
    if (eProtocol >= c2s_max)
        return;
    if (this->m_Client2ServiceFunc[eProtocol] != NULL)
    {
        (this->*m_Client2ServiceFunc[eProtocol])(pMsg);
    }
}

void ServiceManager::checkConnect(void *pMsg)
{
    S2C_CHECK_CONNECT info;
    info.eProtocol = s2c_check_connect;
#ifdef CHECK_NET
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        info.result= iOSHelper::isConnected;
    #elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        info.result = AndroidHelper::checkConnect();
    //    info.result = true;
    #endif
#else
    info.result = true;//暂定无须检查
#endif

    cocos2d::log("ServiceManager::checkConnect");
    ClientLogic::instance()->ProcessServiceResponse(&info);
}

void ServiceManager::exitApp(void *pMsg)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    cocos2d::Director::getInstance()->end();
    exit(0);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    AndroidHelper::exitGame();
    S2C_EXIT_APP info;
    info.eProtocol = s2c_exit;
    ClientLogic::instance()->ProcessServiceResponse(&info);
#endif

}

void ServiceManager::pay(void *pMsg)
{
    C2S_PAY infoIn = *static_cast<C2S_PAY*>(pMsg);
#ifdef PAY
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        S2C_PAY info;
        info.eProtocol = s2c_pay;
        info.success = true;
        info.packageId = infoIn.payId;
        ClientLogic::instance()->ProcessServiceResponse(&info);
    //    iOSPayHelper::getInstance()->pay(infoIn.payId);
    #elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        AndroidHelper::pay(infoIn.payId, infoIn.orderId, infoIn.price*100, infoIn.name);
    //    S2C_PAY info;
    //    info.eProtocol = s2c_pay;
    //    info.success = true;
    //    info.packageId = infoIn.payId;
    //    ClientLogic::instance()->ProcessServiceResponse(&info);
    
    #endif
#else
    S2C_PAY info;
    info.eProtocol = s2c_pay;
    info.success = true;
    info.packageId = infoIn.payId;
    ClientLogic::instance()->ProcessServiceResponse(&info);
#endif
}

void ServiceManager::umengTrack(void *pMsg)
{
    C2S_UMENG_TRACK info = *static_cast<C2S_UMENG_TRACK*>(pMsg);
#ifdef UMENG
    umeng::eventDict dict;
    for (auto attr : info.attrMap) {
        dict[attr.first] = attr.second;
    }
    if (dict.empty()) {
        if (info.count <= 0) {
            umeng::MobClickCpp::event(info.eventName.c_str());
        }
    }else{
        if (info.count <= 0) {
            umeng::MobClickCpp::event(info.eventName.c_str(), &dict);
        }else{
            umeng::MobClickCpp::event(info.eventName.c_str(), &dict, info.count);
        }
    }
#endif
}

void ServiceManager::umengExchange(void *pMsg)
{
    C2S_UMENG_EXCHANGE info = *static_cast<C2S_UMENG_EXCHANGE*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::exchange(info.orderId.c_str(),info.price,"CNY",info.count,info.channel);
#endif
}

void ServiceManager::umengBuy(void *pMsg)
{
    C2S_UMENG_BUY info = *static_cast<C2S_UMENG_BUY*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::buy(info.itemName.c_str(), 1, info.price);
#endif
}

void ServiceManager::umengUse(void *pMsg)
{
    C2S_UMENG_USE info = *static_cast<C2S_UMENG_USE*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::use(info.itemName.c_str(), info.count, info.price);
#endif
}

void ServiceManager::umengUserLevel(void *pMsg)
{
    C2S_UMENG_USER_LEVEL info = *static_cast<C2S_UMENG_USER_LEVEL*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::setUserLevel(info.level);
#endif
}

void ServiceManager::umengEnterStage(void *pMsg)
{
    C2S_UMENG_STAGE_LEVEL info = *static_cast<C2S_UMENG_STAGE_LEVEL*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::startLevel(info.stageName.c_str());
#endif
}

void ServiceManager::umengFinishStage(void *pMsg)
{
    C2S_UMENG_STAGE_LEVEL info = *static_cast<C2S_UMENG_STAGE_LEVEL*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::finishLevel(info.stageName.c_str());
#endif
}

void ServiceManager::umengFailStage(void *pMsg)
{
    C2S_UMENG_STAGE_LEVEL info = *static_cast<C2S_UMENG_STAGE_LEVEL*>(pMsg);
#ifdef UMENG
    umeng::MobClickCpp::failLevel(info.stageName.c_str());
#endif
}

void ServiceManager::reqServerTime(void *pMsg)
{
    HttpManager::instance()->reqServerTime();
}
