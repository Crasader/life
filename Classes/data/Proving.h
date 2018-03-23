//
//  Proving.h
//  life
//
//  Created by ff on 17/8/11.
//
//

#ifndef Proving_h
#define Proving_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __PROVING_CONFIG
{
    int provingId;
    int hp;
    int level;
    std::string stageIcon;
    std::string petIcon;
    int dropGold;
    int dropStone;
}PROVING_CONFIG;

typedef struct __PROVING_DATA
{
    int provingStage;
    int maxResetCount;
    int resetCount;
    int sweepTicketCount;
    int petHp;
    int petAttack;
    int petMaxHp;
    int attackDirect;
}PROVING_DATA;

typedef struct __PROVING_FIGHTER_DATA
{
    int petId;
    int petHp;
    int petLevel;
    int maxHp;
    int petAttack;
    int critRate;
    int critPercent;
}PROVING_FIGHTER_DATA;

class Proving : public Singleton<Proving>
{
public:
    Proving();
    ~Proving();
    
    void readProvConfigFile();
    
    void deleteCoreJson();
    bool readCoreJson();
    bool parseCoreJson();
    void save2CoreJson();
    void save2EmptyCoreStr();
    
    void deleteFighterJson();
    bool readFighterJson();
    bool parseFighterJson();
    void save2FighterJson();
    void save2EmptyFighterStr();

    int refreshProv();
    int resetProv();
    void addMaxResetCount(int val);
    void addTicket(int val);
    
    void pushPet(int petId, int level, int maxHp, int critRate, int critPercent);
    int confirmStage();
    int confirmFighter();
    int getFighterId();
    int fight();
//    int takeReward();

public:
    static std::map<int, int> provConfigIdTable;
    
    std::map<int, PROVING_CONFIG> provConfigMap;
    
    std::map<int, PROVING_FIGHTER_DATA> fighterData;
private:
    CC_SYNTHESIZE_READONLY(PROVING_DATA, provData, ProvData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, fighterStr, FighterStr);
    int fighterIndex;
};

#endif /* Proving_h */
