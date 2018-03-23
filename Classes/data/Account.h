//
//  Account.h
//  life
//
//  Created by ff on 17/7/18.
//
//

#ifndef Account_h
#define Account_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __ACCOUNT_DATA
{
    int             currJob;
    std::string     name;
    int             level;
    int             exp;
    int             hp;
    int             attack;
    int             defence;
    int             crit;
    int             deCrit;
    int             vipLevel;
    int             vipExp;
    int             gold;
    int             diamond;
    int             fame;
}ACCOUNT_DATA;

typedef struct __JOB_CONFIG
{
    int             jobId;
    std::string     name;
    std::string     headIcon;
    std::string     armature;
    std::string     pvpImg;
    int             lock;
    int             active;
    int             fighterId;
    int             opponentId;
    std::string     descript;
    int             price;
}JOB_CONFIG;

typedef struct __JOB_DATA
{
    int             jobId;
    int             lock;
    int             active;
}JOB_DATA;

typedef struct __UPLEVEL_CONFIG
{
    int             level;
    int             expThres;
    int             deltaHp;
    int             deltaAttack;
    int             deltaDefence;
    int             deltaCrit;
    int             deltaDeCrit;
}UPLEVEL_CONFIG;

typedef struct __UNLOCK_CONFIG
{
    int             unlockId;
    int             stageId;
    int             lockType;
    std::string     icon;
}UNLOCK_CONFIG;

typedef struct __UNLOCK_DATA
{
    int             unlockId;
    int             lock;
}UNLOCK_DATA;
class Account : public Singleton<Account> {
public:
    Account();
    ~Account();
    
    void readUnlockConfigFile();
    void readJobConfigFile();
    void readUplevelConfigFile();
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    void deleteUnlockJson();
    bool readUnlockJson();
    bool parseUnlockJson();
    void save2UnlockJson();
    void save2EmptyUnlockStr();
    
    void deleteJobJson();
    bool readJobJson();
    bool parseJobJson();
    void save2JobJson();
    void save2EmptyJobStr();
    
    int getForce();
    int calForce(int hp, int attack, int defence, int crit, int decrit);
    
    void changeName(std::string name);
    void changeHp(int delta);
    void changeAttack(int delta);
    void changeDefence(int delta);
    void changeCrit(int delta);
    void changeDeCrit(int delta);
    void addExp(int val);
    void uplevel();
    inline bool isMaxLevel()
    {
        return data.level >= uplevelConfigIdTable.size();
    }
    
    void clearVipExp();
    void addVipExp(int val);
    void upVipLevel(int val);
    
    void changeGold(int delta);
    void changeDiamond(int delta);
    void changeFame(int delta);
    
    void unlock(int unlockId);
    void openLock(int unlockId);
    
    void unlockJob(int jobId);
    void activeJob(int jobId);
    int changeJob(int jobId);
public:
    static std::map<int, int> unlockConfigIdTable;
    static std::map<int, int> jobConfigIdTable;
    static std::map<int, int> uplevelConfigIdTable;
    
    std::map<int, UNLOCK_CONFIG> unlockConfigMap;
    std::map<int, JOB_CONFIG> jobConfigMap;
    std::map<int, UPLEVEL_CONFIG> uplevelConfigMap;
    std::map<int, UNLOCK_DATA> unlockMap;
    std::map<int, JOB_DATA> jobMap;
private:
    CC_SYNTHESIZE(int, accountVIT, AccountVIT);
    CC_SYNTHESIZE(int, accountMAXVIT, AccountMAXVIT);
    CC_SYNTHESIZE_READONLY(std::string, unlockStr, UnlockStr);
    CC_SYNTHESIZE_READONLY(ACCOUNT_DATA, data, Data);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, jobStr, JobStr);
};

#endif /* Account_h */
