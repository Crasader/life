//
//  BlackActor.h
//  ssoul
//
//  Created by ff on 14-10-14.
//
//

#ifndef __ssoul__BlackActor__
#define __ssoul__BlackActor__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Skill.h"
#include "SkillTag.h"
#include "GameDefine.h"

typedef struct __OPPONENT_ROBOT
{
    std::string                 name;
    int                         jobId;
    int                         fighterId;
    int                         holySwordId;
    int                         petId[3];
    int                         hp;
    int                         attack;
    int                         defence;
    int                         crit;
    int                         decrit;
}OPPONENT_ROBOT;

class BlackColossus;

class BlackActor
{
public:
    BlackActor();
    ~BlackActor();
    
    void readFile(int idx, std::string filename);
    
    bool initData(int idx, rapidjson::Document& _doc);
    
    static void initIdTable(rapidjson::Document &_doc);
    
    void initPlayerData();
    void initSkills(int holySwordSkillId, int petSkillId[3]);
    
    void initSelSkill();
    
    void initSelSkillTag();
    
    void calSkillDis();
    
    void readRelativeSkill(int skillId);
    
    void readRelativeSkillTag(int skillId);
    
    void addAv();
    
    void initClsSelSkill(BlackColossus *cls);
    void initOppRobot(OPPONENT_ROBOT &robot);
    void setupDataWithRobot();
    
    static void getActorRes(int idx, std::string filename, std::string &res);
private:
    CC_SYNTHESIZE_READONLY(int, actorId, ActorID);
    CC_SYNTHESIZE_READONLY(std::string, name, Name);
    CC_SYNTHESIZE_READONLY(std::string, resName, ResName);
    CC_SYNTHESIZE_READONLY(float, scaleRate, ScaleRate);
    CC_SYNTHESIZE_READONLY(float, moveSpeed, MoveSpeed);
    CC_SYNTHESIZE_READONLY(float, rushSpeed, RushSpeed);
    CC_SYNTHESIZE(float, hp, Hp);
    CC_SYNTHESIZE(float, maxHp, MaxHp);
    CC_SYNTHESIZE_READONLY(int, barCount, BarCount);
    CC_SYNTHESIZE(float, mp, Mp);
    CC_SYNTHESIZE_READONLY(float, maxMp, MaxMp);
    CC_SYNTHESIZE(float, ghost, Ghost);
    CC_SYNTHESIZE(float, maxGhost, MaxGhost);
    CC_SYNTHESIZE(float, attack, Attack);
    CC_SYNTHESIZE(float, buffAttack, BuffAttack)
    CC_SYNTHESIZE(float, defence, Defence);
    CC_SYNTHESIZE(float, buffDefence, BuffDefence);
    CC_SYNTHESIZE_READONLY(float, maxAp, MaxAp);
    CC_SYNTHESIZE(float, ap, Ap);
    CC_SYNTHESIZE_READONLY(float, maxAV, MaxAV);
    CC_SYNTHESIZE(float, av, AV);
    CC_SYNTHESIZE(int, petCount, PetCount);
    CC_SYNTHESIZE(int, maxPetCount, MaxPetCount);
    CC_SYNTHESIZE_READONLY(float, atkAThreshold, AtkAThreshold);
    CC_SYNTHESIZE_READONLY(float, hurtAThreshold, HurtAThreshold);
    
    CC_SYNTHESIZE_READONLY(float, waitTime, WaitTime);

    CC_SYNTHESIZE_READONLY(float, hitRatio, HitRatio);
    CC_SYNTHESIZE_READONLY(float, dodgeRatio, DodgeRatio);
    CC_SYNTHESIZE(float, critRatio, CritRatio);
    CC_SYNTHESIZE(float, tenacity, Tenacity);
    CC_SYNTHESIZE_READONLY(float, iceResist, IceResist);
    CC_SYNTHESIZE_READONLY(float, fireResist, FireResist);
    CC_SYNTHESIZE_READONLY(float, thunderResist, ThunderResist);
    CC_SYNTHESIZE_READONLY(float, windResist, WindResist);
    CC_SYNTHESIZE_READONLY(float, lightResist, LightResist);
    CC_SYNTHESIZE_READONLY(float, darkResist, DarkResist);

    
    CC_SYNTHESIZE_READONLY(int, level, Level);
    CC_SYNTHESIZE_READONLY(int, job, Job);
    CC_SYNTHESIZE_READONLY(std::string, stateMachineFile, StateMachineFile);
    CC_SYNTHESIZE_READONLY(float, deHitDownVal, DeHitDownVal);
    
    CC_SYNTHESIZE_READONLY(float, de2AirRate, De2AirRate);
    CC_SYNTHESIZE_READONLY(float, deDownRate, DeDownRate);
    CC_SYNTHESIZE_READONLY(float, immuBuffRate, ImmuBuffRate);
//    CC_SYNTHESIZE_READONLY(float, frozenRate, FrozenRate);
//    CC_SYNTHESIZE_READONLY(float, fixRate, FixRate);
    CC_SYNTHESIZE(int, atkskillId, AtkSkillID);
    CC_SYNTHESIZE_READONLY(int, atk1skillId, Atk1SkillID);
    CC_SYNTHESIZE_READONLY(int, atk2skillId, Atk2SkillID);
    CC_SYNTHESIZE_READONLY(int, atk3skillId, Atk3SkillID);
    CC_SYNTHESIZE_READONLY(int, atk4skillId, Atk4SkillID);
    CC_SYNTHESIZE(int, skillAId, SkillAID);
    CC_SYNTHESIZE(int, skillBId, SkillBID);
    CC_SYNTHESIZE(int, skillCId, SkillCID);
    CC_SYNTHESIZE(int, skillDId, SkillDID);
    CC_SYNTHESIZE(int, runAttack1ID, RunAttack1ID);
    CC_SYNTHESIZE(int, runAttack2ID, RunAttack2ID);
    CC_SYNTHESIZE(int, runAttackID, RunAttackID);
    CC_SYNTHESIZE(int, skillSPId, SkillSPID);
    CC_SYNTHESIZE_READONLY(float, mpRecover, MpRecover);
    
    CC_SYNTHESIZE_READONLY(int, colossusId, ColossusId);
    CC_SYNTHESIZE_READONLY(int, guardId, GuardId);
    CC_SYNTHESIZE_READONLY(int, holySwordSkillId, HolySwordSkillId);
    
    CC_SYNTHESIZE_READONLY(OPPONENT_ROBOT, robot, Robot);
    CC_SYNTHESIZE_READONLY(std::string, playerName, PlayerName);
//    CC_SYNTHESIZE_READONLY(std::map<int, Skill *>, selSkills, SelSkills);
//    CC_SYNTHESIZE_READONLY(std::map<int, SkillTag *>, selSkillTags, SelSkillTags);
    CC_SYNTHESIZE(bool, isOpponent, IsOpponent);
    CC_SYNTHESIZE_READONLY(float, safeDis, SafeDis);
    
    CC_SYNTHESIZE(bool, plotProtect, PlotProtect);
public:
    int petSkillId[3];
    std::map<int, Skill *> selSkills;
    std::map<int, SkillTag *> selSkillTags;
    std::map<int, int> equipSkills;

    static std::map<int, int> idTable;
    
    
};
#endif /* defined(__ssoul__BlackActor__) */
