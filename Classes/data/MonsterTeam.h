//
//  MonsterWave.h
//  cocosGame
//
//  Created by ff on 14-11-24.
//
//

#ifndef __cocosGame__MonsterTeam__
#define __cocosGame__MonsterTeam__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

struct mon{
    mon()
    {
        monId = 0;
        teamId = 0;
        posType = 0;
        fromTmx = false;
        x = 0;
        y = 0;
        memset(buffId, 0, sizeof(int)*5);
    }
    int monId;
    int teamId;
    int posType;
    bool fromTmx;
    float x;
    float y;
    int buffId[5];
};

class MonsterTeam
{
public:
    MonsterTeam();
    ~MonsterTeam();
    
    void sendMonster();

    bool someOneDead();
    
    bool readTeam(int idx, rapidjson::Document& _doc);
    
    static void initIdTable(rapidjson::Document &_doc);
    
    bool readFile(int idx, std::string filename);
public:
    static std::map<int, int> idTable;
    
private:
    int buffId[5];
    CC_SYNTHESIZE_READONLY(int, teamId, TeamId);
    CC_SYNTHESIZE_READONLY(int, monsterId, MonsterId);
    CC_SYNTHESIZE_READONLY(int, monsterCount, MonsterCount);
    CC_SYNTHESIZE_READONLY(int, displayPos, DisplayPos);
    CC_SYNTHESIZE_READONLY(int, nextTeamId, NextTeamId);
    CC_SYNTHESIZE_READONLY(float, teamInterval, TeamInterval);
    CC_SYNTHESIZE_READONLY(bool, fromTmx, fromTmx);
    CC_SYNTHESIZE_READONLY(int, waveId, waveId);
//    CC_SYNTHESIZE_READONLY(int, buff1Id, Buff1Id);
//    CC_SYNTHESIZE_READONLY(int, buff2Id, Buff2Id);
//    CC_SYNTHESIZE_READONLY(int, buff3Id, Buff3Id);
//    CC_SYNTHESIZE_READONLY(int, buff4Id, Buff4Id);
//    CC_SYNTHESIZE_READONLY(int, buff5Id, Buff5Id);
    CC_SYNTHESIZE_READONLY(int, dieTeamId, DieTeamId);
    
    CC_SYNTHESIZE_READONLY(bool, hasSend, HasSend);
    CC_SYNTHESIZE_READONLY(std::vector<mon>, mons, Mons);
    
    CC_SYNTHESIZE_READONLY(cocos2d::experimental::TMXTiledMap *, tiledMap, TiledMap);
    CC_SYNTHESIZE_READONLY(int, surviveCount, SurviveCount);
    
    CC_SYNTHESIZE_READONLY(bool, intervalTrigger, InterValTrigger);
    CC_SYNTHESIZE_READONLY(bool, overTrigger, OverTrigger);

//	CC_SYNTHESIZE_READONLY(int, BeginDialogueID, BeginDialogueID);
//    CC_SYNTHESIZE_READONLY(int, beginGroupId, BeginGroupId);
//	CC_SYNTHESIZE_READONLY(int, EndDialogueID, EndDialogueID);
//    CC_SYNTHESIZE_READONLY(int, endGroupID, EndGroupID);
    
//    std::vector<mon> mons;
};

#endif /* defined(__cocosGame__MonsterTeam__) */
