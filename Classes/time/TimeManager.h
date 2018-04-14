//
//  TimeManager.hpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#ifndef TimeManager_hpp
#define TimeManager_hpp

#include "utils/GameUtils.h"
#include "TLProtocol.h"
#include "data/GameTime.h"

class TimeManager : public Singleton<TimeManager>
{
public:
    TimeManager();
    
    void mainLoop(float dt);
    
    void processClientQuest(void *pMsg);
    void (TimeManager::*m_Client2TimeFunc[l2t_max])(void *pMsg);
    
    void loadData();
    void saveData();
    
    void startApp(void *pMsg);
    void startRestoreVIT(void *pMsg);
    void resetOfferBoardRefresh(void *pMsg);
    void getOfferBoardRefreshCD(void *pMsg);
    void updateOfferAllCD(void *pMsg);
    void startWorkOffer(void *pMsg);
    void stopWorkOffer(void *pMsg);
    void refreshProvReset(void *pMsg);
    void resetVITBuyCount(void *pMsg);
    void startTowerSweep(void *pMsg);
    void stopTowerSweep(void *pMsg);
    void refreshTowerReset(void *pMsg);
    void updateTowerSweepCD(void *pMsg);
    void refreshAbyss(void *pMsg);
    void restoreArenaTicket(void *pMsg);
    void stopArenaTicketRestore(void *pMsg);
    void getArenaTicketCD(void *pMsg);
    void refreshArenaReward(void *pMsg);
    void refreshEveryday(void *pMsg);
    void refreshMonthAssign(void *pMsg);
    void restoreDrawNormal(void *pMsg);
    void restoreDrawSpecial(void *pMsg);
    void getLuckyCD(void *pMsg);
    void refreshCampaignActive(void *pMsg);
    void resetFreeDiamond(void *pMsg);
    void waitDiscountOpen(void *pMsg);
    void waitDiscountClose(void *pMsg);
    void getDiscountCloseSec(void *pMsg);
    void getVITRestoreCD(void *pMsg);
    void resetPetOfferCount(void *pMsg);
    
private:
    GameTime *data;
    bool start;
};

#endif /* TimeManager_hpp */
