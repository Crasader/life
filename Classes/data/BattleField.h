//
//  BattleField.h
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#ifndef BattleField_h
#define BattleField_h

#include "cocos2d.h"

#include "utils/GameUtils.h"
#include "cocostudio/CocoStudio.h"
#include "MonsterWave.h"

typedef struct __BATTLE_CONFIG
{
    int                 stageId;
    std::string         tmx;
    std::string         weather;
    int                 startWaveId;
    int                 waveCount;
    int                 condition;
    int                 secondLimit;
    int                 bossTeamId;
    int                 protectorId;
    int                 buffId[5];
}BATTLE_CONFIG;

class BattleField : public Singleton<BattleField> {
public:
    BattleField();
    ~BattleField();
    
    bool readBattle(int idx);
    
    void readFile(std::string filename);
    
    inline std::string getTiledMapName(){
        return config.tmx;
    };
    
    void loadTmx();
    void load1stMonsterWave();
    
    bool start1stWave();
    bool readCurrWave();
    
    int getNextWaveId();
    bool change2Wave(int waveId);
    
    void update(float dt);
    
public:
    static std::map<int, int> idTable;
    static std::map<int, int> stageIdTable;
    
private:
    CC_SYNTHESIZE_READONLY(BATTLE_CONFIG, config, Config);
    CC_SYNTHESIZE(int, stageId, StageId);
    
    CC_SYNTHESIZE_READONLY(int, secondLimit, SecondLimit);
    CC_SYNTHESIZE_READONLY(int, waveCount, WaveCount);
    
    CC_SYNTHESIZE_READONLY(MonsterWave *, currMonsterWave, CurrMonsterWave);
    
    CC_SYNTHESIZE_READONLY(int, currSecondLeft, CurrSecondLeft);
    CC_SYNTHESIZE_READONLY(int, currWaveId, CurrWaveId);
    CC_SYNTHESIZE_READONLY(int, currWaveCount, CurrWaveCount);
    CC_SYNTHESIZE_READONLY(bool, gameFail, GameFail);
    CC_SYNTHESIZE_READONLY(bool, waveOver, WaveOver);
};

#endif /* BattleField_h */
