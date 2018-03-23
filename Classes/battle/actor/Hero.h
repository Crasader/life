#pragma once

#include "cocos2d.h"
#include "FightActor.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"
#include "../stateMachine/StateMachine.h"

class Hero : public FightActor
{
public:
    Hero();
	~Hero();

    bool init();
    void onEnter();
    
    virtual void setupData(int idx);
    virtual void initFightData(int idx);
    
    void setupValues(int hpVal, int atkVal, int defVal, int critVal, int deCritVal);
    void addHurtListener();
	void removeHurtListener();


    void exitState(int currState);
    void enterState(int nextState);
    
    void updateState();
    void updateAI();
    void doFlag(int nextFlag);
    
    void pauseFight();
    void resumeFight();
    
//    virtual void changeSkill(int event);
    virtual void dispatchResetPunchTag();
    
    void backSkillAtkBase();
    
//    void backSkill2Base(int skillId, int key);
    
    void startXuli(int key);
    void endXuli(int key);
    void xuliEnough();
    
	/**
     *	@brief	由UI按键转成逻辑按键值
     *
     *	@param 	key 	int 当前按键，不包含方向键
     */
    bool setCurrKey(int key);


	void clearKey();
    
    void readyRushAtk();

//    void clearCombo();
    
//    void addCombo();
    
    void revive();
    void fullHp();
    virtual void setHp(float hp);
    virtual void setMaxHp(float hp);
    virtual void setMp(float val);
    virtual void addAv(float val);
    virtual void useAp(float val);
    virtual void setGhost(float val);
    
    void updateComboFactor(cocos2d::EventCustom *event);
    void clearCombo();
    void resetWalkOrRush(int, int);
    
    void startPlot(cocos2d::EventCustom *event);
    
    void plotChangeAction(int action, bool flipX, int missionId);
    
    void plotMove(float duration, float targetX, int missionId);
    void plotAnimationEvent(cocostudio::Armature *armature,
                        cocostudio::MovementEventType movementType,
                        const std::string& movementID);
    void plotMoveEnd();
    
    void animEvent(cocostudio::Armature *armature,
                        cocostudio::MovementEventType movementType,
                        const std::string& movementID);


//    virtual void addBuff();
    
    CREATE_FUNC(Hero);
private:

//    std::string localBodyName;

//    bool rushReady;

    float fallStartY = 0;
    
//    cocostudio::Armature *ghost1;
//    cocostudio::Armature *ghost2;
//    cocos2d::Vec2 ghostPos[6];
//    //    cocos2d::Vec2 ghost2Pos;
//    bool ghostFilpX[6];
//    int ghostDir[6];
//    std::string ghostAnim[6];

    cocos2d::EventListenerCustom *fullHpListener;
    cocos2d::EventListenerCustom *updateComboListener;
    cocos2d::EventListenerCustom *clearComboListener;
    cocos2d::EventListenerCustom *pauseListener;
    cocos2d::EventListenerCustom *resumeListener;
//    cocos2d::EventListenerCustom *startPlotListener;

    CC_SYNTHESIZE(bool, rushAtkReady, RushAtkReady);
	CC_SYNTHESIZE_READONLY(int, currKey, CurrKey);
    CC_SYNTHESIZE(int, keyCommand, KeyCommand);
    CC_SYNTHESIZE_READONLY(int, joyCommand, JoyCommand);
    CC_SYNTHESIZE_READONLY(bool, isXuliEnough, IsXuliEnough);
    CC_SYNTHESIZE(bool, autoControll, AutoControll);
    
    CC_SYNTHESIZE_READONLY(int, plotActionIdx, PlotActionIdx);
    CC_SYNTHESIZE_READONLY(int, plotActionMission, PlotActionMission);
    CC_SYNTHESIZE_READONLY(int, plotMoveMission, PlotMoveMission);

};
