//
//  Campaign.h
//  life
//
//  Created by ff on 17/11/2.
//
//

#ifndef Campaign_h
#define Campaign_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __EVERYDAY_CONFIG
{
    int everydayId;
    int boundType;
    int boundId;
    int boundCount;
}EVERYDAY_CONFIG;

typedef struct __CAMPAIGN_DATA
{
    int everydayId;
    int everydayState;
    int monthAssignId;
    int monthAssignState;
    int totalActiveVal;
    int boxState[4];
    int freeDiamondId;
    int freeDiamondState;
    float onlineDuration;
    int killMonsterCount;
}CAMPAIGN_DATA;

typedef struct __MONTH_ASSIGN_CONFIG
{
    int configId;
    int dayId;
    int boundType;
    int boundId;
    int boundCount;
    int vipLimit;
}MONTH_ASSIGN_CONFIG;

typedef struct __ACTIVE_MISSION_CONFIG
{
    int missionId;
    std::string descript;
    int count;
    int activeVal;
    int boundType;
    int boundId;
    int boundCount;
    int activeType;
    int activeThres;
}ACTIVE_MISSION_CONFIG;

typedef struct __ACTIVE_BOX_CONFIG
{
    int itemId;
    int packageId;
    int boundType;
    int boundId;
    int boundCount;
}ACTIVE_BOX_CONFIG;

typedef struct __ACTIVE_MISSION_DATA
{
    int missionId;
    int count;
    int boundState;
    int maxThres;
}ACTIVE_MISSION_DATA;

typedef struct __BOUND_MISSION_CONFIG
{
    int missionId;
    bool open;
    int activeType;
    int activeThres;
    int activeId;
    std::string descript;
    int count;
    int packageId;
    int nextId;
    std::string icon;
}BOUND_MISSION_CONFIG;

class Campaign : public Singleton<Campaign> {
public:
    Campaign();
    ~Campaign();
    
    void readMonthAssignConfig();
    void readEverydayConfig();
    void readActiveMissionConfig();
    void readActiveBoxConfig();
    void readBoundMissionConfig();
    void readBoundPackageConfig();
    
    void deleteCampaignJson();
    bool readCampaignJson();
    bool parseCampaignJson();
    void save2CampaignJson();
    void save2EmptyCampaignJson();
    
    void deleteActiveJson();
    void save2ActiveJson();
    void save2EmptyActiveJson();
    bool readActiveJson();
    bool parseActiveJson();
    
    void deleteMissionJson();
    void save2MissionJson();
    void save2EmptyMissionJson();
    bool readMissionJson();
    bool parseMissionJson();
    
    
    void enableEveryday();
    void takeEverydayBound();
    
    void enableMonthAssign();
    void takeMonthAssignBound();
    
    void resetActive();
    void activeMission(int activeType, int activeThres);
    int takeActiveMissionBound(int missionId);
    int takeBoxBound(int boxId);
    void getPackageById(int packageId, std::vector<int> &packageIdVec);
    
    void activeBoundMission(int activeType, int activeId, int activeThres);
    int takeBoundMissionReward(int missionId);
    void refreshBoundMission(int missionId, rapidjson::Document &_doc);
    void getBoundPackageById(int packageId, std::vector<int> &packageIdVec);
    void cleanBoundStr();
    void currBoundMission(std::vector<int> &missionIdVec);
    
    int takeFreeDiamond();
    void enableFreeDiamond();
    void resetFreeDiamond();
    bool addDiamond(float dt);
    void saveDiamond();
    
    void checkTip();
public:
    static std::map<int, int> monthAssignConifgIdTable;
    static std::map<int, int> everydayConfigIdTable;
    static std::map<int, int> activeMissionIdTable;
    static std::map<int, int> activeBoxIdTable;
    static std::map<int, int> boundMissionIdTable;
    static std::map<int, int> boundPackageIdTable;
    
    static std::map<int, int> boundDocIdTable;
    
    std::map<int, MONTH_ASSIGN_CONFIG> monthAssignConfigMap;
    std::map<int, EVERYDAY_CONFIG> everydayConfigMap;
    std::map<int, ACTIVE_MISSION_CONFIG> activeMissionMap;
    std::map<int, ACTIVE_BOX_CONFIG> activeBoxMap;
    std::map<int, BOUND_MISSION_CONFIG> boundMissionMap;
    std::map<int, ACTIVE_BOX_CONFIG> boundPackageMap;
    
    std::map<int, ACTIVE_MISSION_DATA> activeDataMap;
    std::map<int, ACTIVE_MISSION_DATA> boundDataMap;
private:
    CC_SYNTHESIZE_READONLY(CAMPAIGN_DATA, coreData, CoreData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, activeStr, ActiveStr);
    CC_SYNTHESIZE_READONLY(std::string, boundStr, BoundStr);
    
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
    CC_SYNTHESIZE_READONLY(bool, activeTip, ActiveTip);
    CC_SYNTHESIZE_READONLY(bool, boundTip, BoundTip);
    CC_SYNTHESIZE_READONLY(bool, monthTip, MonthTip);
};

#endif /* Campaign_h */
