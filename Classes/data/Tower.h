//
//  Tower.h
//  life
//
//  Created by ff on 17/9/11.
//
//

#ifndef Tower_h
#define Tower_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __TOWER_CONFIG
{
    int towerId;
    std::string name;
    std::string animationFile;
    std::string descirpt;
    int gemId;
    int gemCount;
    int stageId;
}TOWER_CONFIG;

typedef struct __TOWER_DATA
{
    int currTower;
    int openCount;
    int resetCount;
    int maxResetCount;
    int sweepState;
}TOWER_DATA;

class Tower : public Singleton<Tower> {
public:
    Tower();
    ~Tower();
    
    void readTowerConfigFile();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();

    int refreshTower();
    int resetTower();
    int checkSweepTower();
    int startSweepTower();
    int endSweepTower();
    int takeSweepReward();
    int start(int towerId);
    int win(int towerId);
    void addMaxResetCount(int val);
    
    void getSweepGems(std::map<int, int> &gems);
public:
    static std::map<int, int> towerConfigIdTable;
    std::map<int, TOWER_CONFIG> towerConfigMap;
private:
    CC_SYNTHESIZE_READONLY(TOWER_DATA, towerData, TowerData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
};
#endif /* Tower_h */
