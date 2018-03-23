//
//  Monster.h
//  soul
//
//  Created by ff on 14-9-15.
//
//

#ifndef __soul__Monster__
#define __soul__Monster__

#include "FightActor.h"
#include "ItemSpirit.h"


//enum MONSTERACTION
//{
//	STAND = 0,
//	QISHEN,
//	HEADHURT,
//	WAISTHURT,
//	FOOTATTACK,
//	NORMALATTACK,
//	SKILLATTACK,
//	MOVE,
//	JUMP_UP,
//	JUMPFORWARD,
//	ATTACKPART3,
//	ATTACKPART2,
//	ATTACKPART1
//};

//enum MONSTERACTION
//{
//	STAND = 0,
//	STANDUP,
//	HEADHURT,
//	WAISTHURT,
//	FOOTATTACK,
//	NORMALATTACK,
//	SKILLATTACK,
//	MOVE,
//	JUMP_UP,
//	JUMPFORWARD,
//    ATTACKPART4,
//	ATTACKPART2,
//	ATTACKPART1,
//	ATTACKPART3,
//    JUMPATK,
//    FWDJUMPATTACK,
//    JUMPDOWN,
//    LANDGROUND,
//    HEADFELLDOWN,
//    WAISTFELLDOWN
//};

class Monster : public FightActor
{
public:
	~Monster();

    bool init();
    void onEnter();
    
    virtual void setupData(int idx);
    virtual void initFightData(int idx);
    
    void addHurtListener();
	void removeHurtListener();
    
    void exitState(int currState);
    void enterState(int nextState);
    
    void updateState();
    void updateAI();
    
    void pauseFight();
    void resumeFight();

    void doFlag(int nextFlag);
    
    void Hurt(int hurtVal);
    
//    virtual void changeSkill(int event);
    virtual void dispatchResetPunchTag();
    
//    void idleTimeEnough();
//    void waitTimeEnough();
    void hasFadeIn();
    
//    std::vector<FightActor *> findEnemies(cocos2d::Vector<FightActor *> actors);
//    FightActor* selectOpponent(std::vector<FightActor *> enemies);
//    void setTargetToActor(FightActor *);
//    bool canFindOpponent();

//    bool lostTarget();
//    bool canAttackTarget();
//    bool canChase();
//    bool needSneak();

    virtual void setHp(float hp);
    virtual void setMaxHp(float val);
    virtual void setMp(float val);
    virtual void addAv(float val);
    virtual void useAp(float val);
    virtual void setGhost(float val);
    
    void addPartolPoint(cocos2d::Point p1, cocos2d::Point p2);
    void popItem();
    void updateGhostPoint();
    bool addGhost2Guard(ItemSpirit *ghost);
    void addAllGhost2Guard();
    
    void initOppRobot(OPPONENT_ROBOT &opp);
    
    void plotChangeAction(int action, bool flipX, int missionId);
    
    void plotMove(float duration, float targetX, int missionId);
    void plotAnimationEvent(cocostudio::Armature *armature,
                            cocostudio::MovementEventType movementType,
                            const std::string& movementID);
    void plotMoveEnd();
    
    CREATE_FUNC(Monster);

public:
//    float waitDuration = 0.8;
    int partolIndex = 0;
    bool partolForward = true;
    
    cocos2d::EventListenerCustom *pauseListener;
    cocos2d::EventListenerCustom *resumeListener;
    CC_SYNTHESIZE(std::vector<cocos2d::Point>, partolPosVec, PartolPosVec);
    CC_SYNTHESIZE_READONLY(cocos2d::Point, leavePartolPos, LeavePartolPos);
//    CC_SYNTHESIZE(cocos2d::Vector<FightActor *>, enemiesVec, EnemiesVec);


    CC_SYNTHESIZE_READONLY(cocos2d::Size, viewSize, ViewSize);
    CC_SYNTHESIZE(int, teamId, TeamId);
    CC_SYNTHESIZE(bool, opponentFlag, OpponentFlag);
    CC_SYNTHESIZE_READONLY(OPPONENT_ROBOT, oppRobot, OppRobot);
    
    CC_SYNTHESIZE_READONLY(int, plotActionIdx, PlotActionIdx);
    CC_SYNTHESIZE_READONLY(int, plotActionMission, PlotActionMission);
    CC_SYNTHESIZE_READONLY(int, plotMoveMission, PlotMoveMission);
    CC_SYNTHESIZE_READONLY(std::string, plotAction, PlotAction);
private:

    std::vector<cocos2d::Point> sneakPath;
    std::vector<ItemSpirit *> ghostList;
    
};
#endif /* defined(__soul__Monster__) */
