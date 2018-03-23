//
//  HttpManager.h
//  life
//
//  Created by ff on 18/3/23.
//
//

#ifndef HttpManager_h
#define HttpManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "network/HttpClient.h"

class HttpManager : public Singleton<HttpManager>, public cocos2d::Ref{
public:
    HttpManager();
    
    ~HttpManager();
    
    void reqServerTime();
    
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};
#endif /* HttpManager_h */
