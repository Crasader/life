//
//  HolySword.h
//  life
//
//  Created by ff on 17/8/7.
//
//

#ifndef HolySword_h
#define HolySword_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __HOLY_SWORD_CONFIG
{
    int                     swordId;
    std::string             name;
    std::string             image;
    std::string             icon;
    std::string             pvpImg;
    int                     next;
    int                     lockLevel;
    int                     fighterId;
}HOLY_SWORD_CONFIG;

typedef struct __HOLY_SWORD_LEVEL_CONFIG
{
    int                     levelId;
    int                     swordId;
    int                     level;
    int                     costGold;
    int                     costStone;
    int                     deltaHp;
    int                     deltaAttack;
    int                     deltaDefence;
}HOLY_SWORD_LEVEL_CONFIG;

typedef struct __HOLY_SWORD_DATA
{
    int                     swordId;
    int                     level;
    int                     lock;
}HOLY_SWORD_DATA;

typedef struct __HOLY_SWORD_INFO
{
    int                     stoneCount;
    int                     hp;
    int                     attack;
    int                     defence;
    int                     fightSwordId;
}HOLY_SWORD_INFO;

class HolySword : public Singleton<HolySword>
{
public:
    HolySword();
    ~HolySword();
    
    void readSwordConfigFile();
    void readLevelConfigFile();
    
    void deleteInfoJson();
    bool readInfoJson();
    bool parseInfoJson();
    void save2InfoJson();
    void save2EmptyCoreStr();
    
    void deleteSwordsJson();
    bool readSwordsJson();
    bool parseSwordsJson();
    void save2SwordsJson();
    void save2EmptySwordsStr();
    
    int addStone(int val);
    int setFightSword(int swordId);
    int uplevel(int swordId);
    void unlockSword(int swordId, rapidjson::Document &coreDoc, rapidjson::Document &swordsDoc);
    bool isMaxLevel(int swordId);
    int checkStone(int swordId);
    int getLevelId(int swordId, int level);
    void unlockAndSave(int swordId);
    
    void checkTip(int gold);
public:
    static std::map<int, int> swordConfigIdTable;
    static std::map<int, int> levelConfigIdTable;
    
    std::map<int, HOLY_SWORD_CONFIG> swordConfigMap;
    std::map<int, HOLY_SWORD_LEVEL_CONFIG> levelConfigMap;
    
    std::map<int, HOLY_SWORD_DATA> swordsMap;
    std::map<int, int> maxLevel;

private:
    CC_SYNTHESIZE_READONLY(HOLY_SWORD_INFO, info, Info);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, swordsStr, SwordsStr);
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
    CC_SYNTHESIZE_READONLY(int, uplevelTip, UplevelTip);

};
#endif /* HolySword_h */
