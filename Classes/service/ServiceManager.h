//
//  SericeManager.h
//  owen
//
//  Created by ff on 16/8/9.
//
//

#ifndef SericeManager_h
#define SericeManager_h

#include "utils/GameUtils.h"
#include "CSProtocol.h"

class ServiceManager : public Singleton<ServiceManager>
{
public:
    ServiceManager();
    
    void processClientQuest(void *pMsg);
    void (ServiceManager::*m_Client2ServiceFunc[c2s_max])(void *pMsg);
    
private:
    void checkConnect(void *pMsg);
    
    void exitApp(void *pMsg);
    
    void pay(void *pMsg);
    
    void umengTrack(void *pMsg);
    void umengExchange(void *pMsg);
    void umengBuy(void *pMsg);
    void umengUse(void *pMsg);
    void umengUserLevel(void *pMsg);
    void umengEnterStage(void *pMsg);
    void umengFinishStage(void *pMsg);
    void umengFailStage(void *pMsg);
    
    void reqServerTime(void *pMsg);
};

#endif /* SericeManager_h */
