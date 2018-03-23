//
//  Arena.h
//  life
//
//  Created by ff on 17/9/18.
//
//

#ifndef Arena_h
#define Arena_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __ARENA_ROBOT_CONFIG
{
    int robotId;
    int level;
    int holySwordLevel;
    int petCount[5];
    int petLevel[5][2];
    int otherAttack;
    int otherCrit;
    int otherHp;
    int otherDefence;
    int otherDecrit;
    int score;
}ARENA_ROBOT_CONFIG;

typedef struct __ARENA_DIVISION_CONFIG
{
    int divisionId;
    std::string icon1;
    std::string icon2;
    int minScore;
    int maxScore;
    int adviceHp;
    int adviceDefence;
    int adviceAttack;
    int adviceCrit;
    int adviceDecrit;
    int robotCount;
    int rewardFame;
    int rewardGold;
    int winGold;
    int loseGold;
    int maxSwordId;
    int petCount[5];
    int minPlace;
}ARENA_DIVISION_CONFIG;

typedef struct __ROBOT_DATA
{
    int place;
    std::string name;
    int jobId;
    int level;
    int score;
    int holySwordId;
    int petId[3];
    int petLevel[3];
    int hp;
    int attack;
    int defence;
    int crit;
    int decrit;
    int robot;
}ROBOT_DATA;

typedef struct __ARENA_DATA
{
    int score;
    int divisionId;
    int place;
    int ticketCount;
    int maxTicketCount;
    int rewardState;
}ARENA_DATA;

class  Arena : public Singleton<Arena> {
public:
    Arena();
    ~Arena();
    
    void readRobotConfig();
    void readDivisionConfig();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    void deleteRobotJson();
    bool readRobotJson();
    bool parseRobotJson();
    void save2RobotJson();
    void save2EmptyRobotStr();
    
    bool compRobotScore(ROBOT_DATA first, ROBOT_DATA second);
    void addRobot(int place, int swordCount, std::map<int, int> totalPets, std::string name, int job,
                    int hp, int attack, int defence, int crit, int decrit);

    void refreshSelfRobot(int job, std::string name, int level, int holySwordId,
                          int petId[3], int petLevel[3],
                          int hp, int attack, int defence, int crit, int decrit);
    void refreshTop30();
    void sortTop30();
    void addRobot2Top30();
    void matchRobot(ROBOT_DATA robot);
    void matchRobotInTop30(ROBOT_DATA &robot);
    void matchRobotByDivision(int division, int jobId, std::string name, int swordId, std::vector<int> totalPets, ROBOT_DATA &robot);
    int startFight();
    int win();
    int lose();
    int addTicket(int val);
    int addMaxTicket(int val);
    int reward();
    int takeReward();
public:
    static std::map<int, int> robotConfigIdTable;
    static std::map<int, int> divisionConfigIdTable;
    std::map<int, ARENA_ROBOT_CONFIG> robotConfigMap;
    std::map<int, ARENA_DIVISION_CONFIG> divistionConfigMap;
    std::vector<ROBOT_DATA> robotMap;
private:
    CC_SYNTHESIZE_READONLY(ROBOT_DATA, oppRobot, OppRobot);
    CC_SYNTHESIZE_READONLY(ARENA_DATA, coreData, CoreData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, robotStr, RobotStr);
};

#endif /* Arena_h */
