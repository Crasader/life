//
//  ProcessServerResponse.cpp
//  owen
//
//  Created by ff on 16/8/9.
//
//

#include "ClientLogic.h"
//#include "HallLogic.h"
#include "SystemLogic.h"

void ClientLogic::ProcessServiceResponse(void *pMsg)
{
//    cocos2d::log("ClientLogic::ProcessServiceResponse");
    if (NULL == pMsg)
        return;
    
//    cocos2d::log("NULL != pMsg");
    S2CProtocol	eProtocol	= *((S2CProtocol*)pMsg);
    if (eProtocol >= s2c_max)
        return;
    
//    cocos2d::log("eProtocol < s2c_max");
    //分发到各个协议的处理函数
    if (this->m_Service2ClientFunc[eProtocol] != NULL)
    {
//        cocos2d::log("分发到各个协议的处理函数");
        (this->*m_Service2ClientFunc[eProtocol])(pMsg);
    }
}

void ClientLogic::ResPay(void *pMsg)
{
    S2C_PAY *pMsgInfo = (S2C_PAY*)pMsg;
    SystemLogic::instance()->responseBuy(*pMsgInfo);
}

void ClientLogic::ResExit(void *pMsg)
{
    SystemLogic::instance()->exitApp();
}

void ClientLogic::ResCheckConnect(void *pMsg)
{
    cocos2d::log("ClientLogic::ResCheckConnect");
    S2C_CHECK_CONNECT *info = (S2C_CHECK_CONNECT*)pMsg;
    SystemLogic::instance()->responseNetCheck(*info);
//    L2E_SHOW_LOGIN L2Einfo;
//    
//    if (info.result) {
//        cocos2d::log("ClientLogic::l2e_show_login");
//        L2Einfo.eProtocol = l2e_show_login;
//    }else{
//        L2Einfo.eProtocol = l2e_show_unconnected;
//    }
//    ClientLogic::instance()->pass2Engine(&L2Einfo);
}

