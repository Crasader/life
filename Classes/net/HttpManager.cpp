//
//  HttpManager.cpp
//  life
//
//  Created by ff on 18/3/23.
//
//

#include "HttpManager.h"
#include <string.h>
#include "GameDefine.h"
#include "CSProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;
using namespace cocos2d::network;

HttpManager::HttpManager()
{
    
}

HttpManager::~HttpManager()
{
    HttpClient::destroyInstance();
}

void HttpManager::reqServerTime()
{
    HttpRequest* request = new (std::nothrow)HttpRequest();
    request->setUrl(SERVER_TIME_URL);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(HttpManager::onHttpRequestCompleted, this));
//    request->setTag("GET server_time");
    request->setTag("WebTime");
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpManager::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response) {
        return;
    }
    
    if (1 != strlen(response->getHttpRequest()->getTag())) {
        log("%s completed", response->getHttpRequest()->getTag());
    }
    
    if (!response->isSucceed()) {
        S2C_SERVER_TIME infoServer;
        infoServer.eProtocol = s2c_server_time_ack;
        infoServer.success = false;
        infoServer.timeStr = "";
        ClientLogic::instance()->ProcessServiceResponse(&infoServer);
        return;
    }
    
    std::vector<char> *data = response->getResponseData();
    std::string res;
    res.insert(res.begin(), data->begin(), data->end());
    S2C_SERVER_TIME info;
    info.eProtocol = s2c_server_time_ack;
    info.success = true;
    info.timeStr = res;
    ClientLogic::instance()->ProcessServiceResponse(&info);
}
