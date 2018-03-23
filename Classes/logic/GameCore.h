//
//  GameCore.h
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#ifndef GameCore_h
#define GameCore_h

#include "utils/GameUtils.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Load.h"
#include "ELProtocol.h"
#include "data/BattleField.h"
#include "data/Account.h"
//#include "data/FighterManager.h"

enum FIGHT_GAME_STATES
{
    FIGHT_GAME_NONE = 0,
    FIGHT_GAME_INIT = 1,
    FIGHT_GAME_NORMAL = 2,
    FIGHT_GAME_LOAD = 3,
    FIGHT_GAME_PAUSE = 4
};

class GameCore : public Singleton<GameCore>, public Load {
public:
    GameCore();
    ~GameCore();
    
    //改变战斗状态
    void changeGameState(FIGHT_GAME_STATES nextState);
    void enterGameState(FIGHT_GAME_STATES nextState);
    void exitGameState(FIGHT_GAME_STATES state);
    void backGameState();
    
    //根据状态分发逻辑
    void logic(float dt);
    
    //加载过程逻辑处理
    void load();
    
    void loadBattleField();
    
    void loadTmxMap();
    void loadTmxEffect();
    void loadHero();
    void loadWitness();
    void loadOpponent();
    void preLoadMonsterRes();
    void startMonsterTeam();
    
    void unlockView();
    void monsterHpEmpty(E2L_MONSTER_HP_EMPTY info);
    void monsterDie(E2L_MONSTER_DIE info);
    void witnessDie();
    void witnessArrive();
    void heroDie();
    void notRevive();
    void revive();
    void fightTimeOver();
    void fightTimeOver2(E2L_FIGHT_TIME_OVER2 info);
    void fightStar(E2L_FIGHT_STAR info);
    void useHpBottle();
    void startFight();
    void addCombo(E2L_ADD_COMBO info);
    void clearCombo();
    
    void pauseFight();
    void resumeFight();
    
    void plotOpenSendMonster(int val, int missionId);
protected:
    virtual void onLoadFinish();
private:
    BattleField *battleField;
    Account *account;
//    FighterManager *fighterManager;
    
    cocos2d::experimental::TMXTiledMap *stageTmx;
    
    CC_SYNTHESIZE(int, stageId, StageId);
    CC_SYNTHESIZE(int, abyssId, AbyssId);
    CC_SYNTHESIZE(int, towerId, TowerId);
    CC_SYNTHESIZE(int, arenaId, AreanId);
    CC_SYNTHESIZE(int, stageResult, StageResult);
    CC_SYNTHESIZE_READONLY(int, useBottleCount, UseBottleCount);
    
    CC_SYNTHESIZE_READONLY(FIGHT_GAME_STATES, currState, CurrState);
    CC_SYNTHESIZE_READONLY(FIGHT_GAME_STATES, lastState, LastState);
    CC_SYNTHESIZE_READONLY(FIGHT_GAME_STATES, originState, OriginState);
};

#endif /* GameCore_h */
