//
//  VIP.h
//  life
//
//  Created by ff on 17/11/13.
//
//

#ifndef VIP_h
#define VIP_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __VIP_CONFIG
{
    int vipId;
    int uplevelRecharge;
    int uplevelDay;
    int packageId;
    std::string descript[10];
}VIP_CONFIG;

typedef struct __VIP_PACKAGE_CONFIG
{
    int itemId;
    int packageId;
    int boundType;
    int boundId;
    int boundCount;
}VIP_PACKAGE_CONFIG;

typedef struct __VIP_DATA
{
    int vipId;
//    uint64_t lastLoginSec;
    int loginDay;
    int boundState;
}VIP_DATA;

class VIP : public Singleton<VIP> {
public:
    VIP();
    ~VIP();
    
    void readVIPConfig();
    void readVIPPackageConfig();
    
    void deleteJson();
    void save2Json();
    void save2EmptyCoreStr();
    bool readJson();
    bool parseJson();
    
    void addVIPDay();
    void clearVIPDay();
//    void setVIPLoginSec(uint64_t sec);
    void uplevelByRecharge(int totalRecharge);
    
    void enableBound();
    int takeBound();
    void getPackageById(int packageId, std::vector<int> &packageIdVec);
public:
    static std::map<int, int> configIdTable;
    static std::map<int, int> packageIdTable;
    
    std::map<int, VIP_CONFIG> vipConfigMap;
    std::map<int, VIP_PACKAGE_CONFIG> packageConfigMap;
private:
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
    CC_SYNTHESIZE_READONLY(VIP_DATA, coreData, CoreData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
};

#endif /* VIP_h */
