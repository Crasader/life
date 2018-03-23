//
//  Equip.hpp
//  life
//
//  Created by ff on 17/7/21.
//
//

#ifndef Equip_hpp
#define Equip_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __WEAR_DATA
{
    int pos;
    int equipId;
    int level;
    int star;
    int attack;
    int defence;
    int hp;
}WEAR_DATA;

typedef struct __EQUIP_ITEM_DATA
{
    int                 itemId;
    int                 count;
}EQUIP_ITEM_DATA;

typedef struct __EQUIP_CONFIG
{
    int                 equipId;
    std::string         icon;
    std::string         name;
    int                 maxStar;
    int                 nextEquipId;
    int                 pos;
    int                 starItem[6];
    int                 starItemCount[6];
    int                 deltaStarHp;
    int                 deltaStarAttack;
    int                 deltaStarDefence;
}EQUIP_CONFIG;

typedef struct __EQUIP_LEVEL_CONFIG
{
    int                 levelId;
    int                 pos;
    int                 level;
    int                 costGold;
    int                 deltaHp;
    int                 deltaAttack;
    int                 deltaDefence;
}EQUIP_LEVEL_CONFIG;

typedef struct __EQUIP_ITEM_CONFIG
{
    int                 itemId;
    std::string         icon;
    int                 price;
    int                 dropRate;
    int                 dropGold;
    int                 dropExp;
    int                 dropCount;
}EQUIP_ITEM_CONFIG;

class Equip : public Singleton<Equip> {
public:
    Equip();
    ~Equip();
    
    void readEquipConfigFile();
    void readEquipLevelConfigFile();
    void readEquipItemConfigFile();
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    void deleteItemJson();
    bool readItemJson();
    bool parseItemJson();
    void save2ItemJson();
    void save2EmptyItemStr();
    
    int getTotalHp();
    int getTotalAttack();
    int getTotalDefence();
    
    bool uplevel(int pos);
    bool upgrade(int pos);
    
    bool addItem(int itemId, int value);
    
    bool isMaxLevel(int pos);
    bool isMaxGrade(int pos);
    bool checkUpgradeItem(int pos);
    void checkTip(int accountLevel);
public:
    static std::map<int, int> equipItemConfigIdTable;
    static std::map<int, int> equipLevelConfigIdTable;
    static std::map<int, int> equipConfigIdTable;
    
    std::map<int, EQUIP_ITEM_CONFIG> equipItemConfigMap;
    std::map<int, EQUIP_LEVEL_CONFIG> equipLevelConfigMap;
    std::map<int, EQUIP_CONFIG> equipConfigMap;
    
    std::map<int, WEAR_DATA> wearMap;
    std::map<int, EQUIP_ITEM_DATA> itemMap;
    std::vector<bool> posTip;
    std::vector<bool> posUplevelTip;
    std::vector<bool> posUpgradeTip;
private:
    int maxLevel[6];
    CC_SYNTHESIZE_READONLY(WEAR_DATA, wearData, WearData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, itemStr, ItemStr);
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
};
#endif /* Equip_hpp */
