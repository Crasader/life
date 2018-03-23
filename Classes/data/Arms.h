//
//  Arms.h
//  life
//
//  Created by ff on 17/8/8.
//
//

#ifndef Arms_h
#define Arms_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __ARMS_CONFIG
{
    int armsId;
    int type;
    std::string icon;
    std::string name;
    int unlockLevel[6];
}ARMS_CONFIG;

typedef struct __ARMS_LEVEL_CONFIG
{
    int levelId;
    int armsId;
    int level;
    int costFame;
    int deltaHp;
    int deltaAttack;
    int deltaDefence;
    int deltaCrit;
    int deltaDecrit;
}ARMS_LEVEL_CONFIG;

typedef struct __GEM_CONFIG
{
    int gemId;
    int type;
    int level;
    std::string icon;
    int value;
}GEM_CONFIG;

typedef struct __ARMS_INFO
{
    int armsId;
    int gemsId[6];
    int lockPos[6];
    int level;
    int hp;
    int attack;
    int defence;
    int crit;
    int decrit;
}ARMS_INFO;

typedef struct __GEM_INFO
{
    int gemId;
    int count;
}GEM_INFO;

class Arms : public Singleton<Arms> {
public:
    Arms();
    ~Arms();
    
    void readArmsConfigFile();
    void readLevelConfigFile();
    void readGemConfigFile();
    
    void deleteCoreJson();
    bool readCoreJson();
    bool parseCoreJson();
    void save2CoreJson();
    void save2EmptyCoreStr();

    void deleteGemJson();
    bool readGemJson();
    bool parseGemJson();
    void save2GemJson();
    void save2EmptyGemStr();
    
    int getLevelId(int armsId, int level);
    void unlockPos(int armsId, int level, rapidjson::Document &doc);
    bool isMaxLevel(int armsId);
    int uplevelArms(int armsId);
    
    void filterGems(int type, std::map<int, int> &gemIds);
    int assembleGem(int gemId);
    int fillGem(int armsId, int gemId);
    int removeGem(int armsId, int pos);
    void addGem(int gemId, int val);
    
    void checkTip(int fame);
public:
    static std::map<int, int> armsConfigIdTable;
    static std::map<int, int> levelConfigIdTable;
    static std::map<int, int> gemConfigIdTable;
    
    std::map<int, ARMS_CONFIG> armsConfigMap;
    std::map<int, ARMS_LEVEL_CONFIG> levelConfigMap;
    std::map<int, GEM_CONFIG> gemsConfigMap;
    
    std::map<int, ARMS_INFO> armsMap;
    std::map<int, int> gemCountMap;
    std::map<int, int> maxLevel;
    std::vector<bool> armsTip;
    std::vector<bool> armsFameTip;
    std::vector<int> armsGemIdTip;
private:
    CC_SYNTHESIZE_READONLY(int, totalHp, TotalHp);
    CC_SYNTHESIZE_READONLY(int, totalDefence, TotalDefence);
    CC_SYNTHESIZE_READONLY(int, totalAttack, TotalAttack);
    CC_SYNTHESIZE_READONLY(int, totalCrit, TotalCrit);
    CC_SYNTHESIZE_READONLY(int, totalDecrit, TotalDecrit);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, gemStr, GemStr);
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
    
};

#endif /* Arms_h */
