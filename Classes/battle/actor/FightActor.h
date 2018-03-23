//
//  FightActor.h
//  soul
//
//  Created by ff on 14-9-15.
//
//

#ifndef __soul__FightActor__
#define __soul__FightActor__

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../dataModule/BlackActor.h"
#include "../dataModule/Skill.h"
#include "../dataModule/SkillTag.h"
#include "Actor.h"
#include "Bullet.h"

#include "../buff/Buff_Common.h"
#include "../buff/BuffManager.h"
#include "../buff/Buff.h"

typedef struct fightPair
{
    Actor *attacker;
    Actor *defender;
}FightPair;

class FightActor : public Actor
{
public:

	FightActor();
	~FightActor();
    void onEnter();
//    void update(float dt);
    virtual void setupData(int idx){};
    virtual void initFightData(int idx){};
    void initViewRes();
    void initFightState();
    void preloadSkillRes();
    
    virtual void addHurtListener(){};
	static void removeHurtListener();
    
    
    /**
     *	@brief	切换技能数据
     *
     *	@param 	event 	int
     */
    void changeSkill(int event);
    int getSkillAction(int event);
    void backSkill2Base(int skillId, int key);
    
    /**
     *	@brief	切换动作
     *
     *	@param 	nextAction 	int 执行动作
     */
    virtual void changeActionWithName(int nextAction);
    
//    virtual void changeAction(int nextAction);
    
    virtual void dispatchResetPunchTag(){};
    
    cocos2d::Rect getAtkRect(bool flip);
    cocos2d::Rect getBombAtkRect();
    void DeStiff();

    void startHurtStiff(float duration);
    void startAtkStiff(float duration);

    
    void clearCombo();
    
    void addCombo();

    int getCurrAction();
    
    bool isLandGround();
    
    void resetPunchTag(cocos2d::EventCustom* event);
    
    void parseHurtEvent(cocos2d::EventCustom* event);
    
    void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    
    void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
    
    void effAnimEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    void adjustHurtEffectPos();
    
    void updateFrontEffect();
    void updateBackEffect();
    
    void hurt();
//    void bulletHurt(Bullet *bullet);
    void bodyHurt();
    void skillBulletHurt();
    void readyVictory();
    
    void DealAttackRectByFlip(const bool flip, cocos2d::Rect * pRect);			// 通过是否翻转处理攻击范围
    
	virtual void setHp(float hp){};
    virtual void setMaxHp(float hp){};
    virtual void setMp(float mp){};
    virtual void addAv(float val){};
    virtual void useAp(float val){};
    virtual void setGhost(float val){};
    
    void addBuff(bool toMyself, SkillTag *skillTag);
    void addSingleBUff(bool toMyself, int buffId, float rate);
    void addBuffFromFriend(cocos2d::EventCustom *event);
    void buffChangeHp(Buff *buff);

    void buffChangeMaxHp(Buff *buff);
    void buffChangeHpRecover(Buff *buff);
    void buffChangeMp(Buff *buff);
    void buffChangeMaxMp(Buff *buff);
    void buffChangeMpRecover(Buff *buff);
    void buffChangeAp(Buff *buff);
    void buffChangeMaxAp(Buff *buff);
    void buffChangeApRecover(Buff *buff);
    void buffChangeAttack(Buff *buff);
    void buffChangeDefence(Buff *buff);
    void buffChangeHitRatio(Buff *buff);
    void buffChangeDodgeRatio(Buff *buff);
    void buffChangeCritRatio(Buff *buff);
    void buffChangeTenacity(Buff *buff);
    void buffChangeIceResist(Buff *buff);
    void buffChangeFireResist(Buff *buff);
    void buffChangeThunderResist(Buff *buff);
    void buffChangeWindResist(Buff *buff);
    void buffChangeLightResist(Buff *buff);
    void buffChangeDarkResist(Buff *buff);
    void buffChangeMoveSpeed(Buff *buff);
    void buffChangeDeHitDown(Buff *buff);
    void buffChangeDeFloat(Buff *buff);
    void buffFaintState(Buff *buff);
    void buffImmuFaint(Buff *buff);
    void buffFixState(Buff *buff);
    void buffImmuFix(Buff *buff);
    void buffFrozenState(Buff *buff);
    void buffImmuFrozen(Buff *buff);
    void buffImmuAll(Buff *buff);
    void buffInvincible(Buff *buff);
    void buffPA(Buff *buff);
    void buffDisableSkill(Buff *buff);
    void buffReverseMove(Buff *buff);
    void buffMad(Buff *buff);
    
    void removeCurrHpBuff(Buff *buff);
    void removeMaxHpBuff(Buff *buff);
    void removeHpRecoverBuff(Buff *buff);
    void removeCurrMpBuff(Buff *buff);
    void removeMaxMpBuff(Buff *buff);
    void removeMpRecoverBuff(Buff *buff);
    void removeCurrApBuff(Buff *buff);
    void removeMaxApBuff(Buff *buff);
    void removeApRecoverBuff(Buff *buff);
    void removeAttackBuff(Buff *buff);
    void removeDefenceBuff(Buff *buff);
    void removeHitRatioBuff(Buff *buff);
    void removeDodgeRatioBuff(Buff *buff);
    void removeCritRatioBuff(Buff *buff);
    void removeTenacityBuff(Buff *buff);
    void removeIceResistBuff(Buff *buff);
    void removeFireResistBuff(Buff *buff);
    void removeThunderResistBuff(Buff *buff);
    void removeWindResistBuff(Buff *buff);
    void removeLightResistBuff(Buff *buff);
    void removeDarkResistBuff(Buff *buff);
    void removeMoveSpeedBuff(Buff *buff);
    void removeDehitDownBuff(Buff *buff);
    void removeDefloatBuff(Buff *buff);
    void removeFaintBuff(Buff *buff);
    void removeFixBuff(Buff *buff);
    void removeFrozenBuff(Buff *buff);
    void removeImmuFaintBuff(Buff *buff);
    void removeImmuFixBuff(Buff *buff);
    void removeImmuFrozenBuff(Buff *buff);
    void removeImmuAllDeBuff(Buff *buff);
    void removeInvincible(Buff *buff);
    void removePA(Buff *buff);
    void removeDisableSkill(Buff *buff);
    void removeReverseMove(Buff *buff);
    void removeMad(Buff *buff);
    
    bool canFindOpponent();
//    bool foundSomeOneOutView();
    int canUseSomeSkill();
    bool canAttackTarget();
//    bool canRushAttack();
    int canUseSkill(int skillId);
    void useSkill(int skillId);
    int selValidSkill();///////todo ff
    int needBack();
    int selChaseMethod();
    bool waitNotSafe();
    void calBackTarget();
    
    bool lostTarget();
    bool needSneak();
    bool canChase();
    
    void setTargetToActor(Actor *actor);
    
    void waitTimeEnough();
    
    void skillCDClock(float dt);
    void recoverAttr(float dt);
    void stageVictory();
protected:
    FightActor* selectOpponent(std::vector<FightActor *> enemies);
protected:

    int ATK_RATE[SKEY_MAX]=
    {
        10,
        30,
        30,
        30,
        30,
        20,
        20,
        20,
        20,
        100,
        0
    };
    
    CC_SYNTHESIZE_READONLY(BlackActor *, blackActor, BlackActor);
    int killerTag;
    int killerAction;
    
    float fallSpeedY;
    float fallAccY;
    float fallAccX;
    
    float startFallSpeedY;
    
    float chatterOffX[3];
    int chatterLoop ;
    int chatterIndex ;

	float totalComboDuration ;
	float comboDuration ;
    
    bool reverseMove;
    
    cocos2d::Layer *backEffectLayer;
    cocos2d::Layer *frontEffectLayer;

    CC_SYNTHESIZE(cocos2d::Layer *, hurtEffectLayer, HurtEffectLayer);
    
    cocos2d::EventListenerCustom *hurtListener;
    cocos2d::EventListenerCustom *punchListener;
    cocos2d::EventListenerCustom *readyVictoryListener;
    cocos2d::EventListenerCustom *stageVictoryListener;
    cocos2d::EventListenerCustom *friendBuffListener;
    
    FightActor *killer;
    std::map<int, int > killers;
    Bullet *killBullet;
    
    int validSkillId[SKEY_MAX];
    CC_SYNTHESIZE_READONLY(float, maxAtkDis, MaxAtkDis);
    CC_SYNTHESIZE_READONLY(float, minAtkDis, MinAtkDis);
    CC_SYNTHESIZE_READONLY(int, comboFactor, ComboFactor);
    
    CC_SYNTHESIZE_READONLY(Skill *, currSkill, CurrSkill);
    CC_SYNTHESIZE_READONLY(SkillTag *, currSkillTag, CurrSkillTag);
    CC_SYNTHESIZE_READONLY(int, comboNum, ComboNum);
    CC_SYNTHESIZE_READONLY(int, maxCombo, MaxCombo);
    CC_SYNTHESIZE_READONLY(bool, showCombo, ShowCombo);
    //击退速度
    CC_SYNTHESIZE_READONLY(float, hurtMoveSpeed, HurtMoveSpeed);

    CC_SYNTHESIZE_READONLY(float, lieShockSpeed, LieShockSpeed);
    
    CC_SYNTHESIZE_READONLY(bool, backInAir, BackInAir);
    
    CC_SYNTHESIZE_READONLY(bool, bHero, BHero);
    
    
    CC_SYNTHESIZE_READONLY(bool, atkStiff, AtkStiff);
    CC_SYNTHESIZE_READONLY(bool, hurtStiff, hurtStiff);
    CC_SYNTHESIZE_READONLY(int, attackTag, AttackTag);
    
    CC_SYNTHESIZE(float, deHitDownVal, DeHitDownVal);
    CC_SYNTHESIZE(float, currHitDownVal, CurrHitDownVal);

//    CC_SYNTHESIZE_READONLY(std::string, localBodyName, LocalBodyName);
    CC_SYNTHESIZE_READONLY(bool, atking, Atking);
    CC_SYNTHESIZE_READONLY(bool, attackHead, AttackHead);
    CC_SYNTHESIZE_READONLY(bool, attackWaist, AttackWaist);
    CC_SYNTHESIZE_READONLY(bool, attackFoot, AttackFoot);
    CC_SYNTHESIZE_READONLY(bool, hit2Air, Hit2Air);
    CC_SYNTHESIZE_READONLY(bool, hit2Ground, Hit2Ground);
    CC_SYNTHESIZE_READONLY(bool, hit2Wall, Hit2Wall);
    
    CC_SYNTHESIZE(bool, isValidAtk, IsValidAtk);
    CC_SYNTHESIZE(bool, isValidWait, IsValidWait);
    CC_SYNTHESIZE_READONLY(int, frameCommand, FrameCommand);
    
    CC_SYNTHESIZE(bool, dieLie, DieLie);
    CC_SYNTHESIZE_READONLY(bool, dying, Dying);
    CC_SYNTHESIZE(bool, isFaint, IsFaint);
    CC_SYNTHESIZE(bool, isFix, IsFix);
    CC_SYNTHESIZE(bool, isFrozen, IsFrozen);
    CC_SYNTHESIZE(bool, isInvincible, IsInvincible);
    CC_SYNTHESIZE(bool, isPA, IsPA);
    
//    CC_SYNTHESIZE(bool, changeCombo, ChangeCombo);

    Skill *killSkill;
    SkillTag *killSkillTag;
    
    CC_SYNTHESIZE_READONLY(bool, asBomb, AsBomb);
    CC_SYNTHESIZE_READONLY(float, bombHitDownVal, BombHitDownVal);
    CC_SYNTHESIZE_READONLY(float, bombHitVal, BombHitVal);
    
    BuffManager* buffManager;
    CC_SYNTHESIZE(FightActor *, targetActor, TargetActor);
    CC_SYNTHESIZE(bool, waitTimeOut, WaitTimeOut);
    CC_SYNTHESIZE(cocos2d::Vector<FightActor *>, enemiesVec, EnemiesVec);
    CC_SYNTHESIZE_READONLY(int, weaponId, WeaponId);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, hpBarBg, HpBarBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, hpBar, HpBar);
    bool hpEmpty = false;
public:
    std::map<int, std::function<void(Buff*)>> buffFuncs = {
        {BUFF_TYPE::BUFF_TYPE_CUR_HP, std::bind(&FightActor::buffChangeHp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_HP, std::bind(&FightActor::buffChangeMaxHp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HP_RECOVER_SPEED, std::bind(&FightActor::buffChangeHpRecover, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CUR_MP, std::bind(&FightActor::buffChangeMp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_MP, std::bind(&FightActor::buffChangeMaxMp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MP_RECOVER_SPEED, std::bind(&FightActor::buffChangeMpRecover, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CUR_AP, std::bind(&FightActor::buffChangeAp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_AP, std::bind(&FightActor::buffChangeMaxAp, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_AP_RECOVER_SPEED, std::bind(&FightActor::buffChangeApRecover, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_ATTACK, std::bind(&FightActor::buffChangeAttack, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DEFINE, std::bind(&FightActor::buffChangeDefence, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HIT_RATIO, std::bind(&FightActor::buffChangeHitRatio, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_AVOIDANCE, std::bind(&FightActor::buffChangeDodgeRatio, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CRIT_RATE, std::bind(&FightActor::buffChangeCritRatio, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_TENACITY, std::bind(&FightActor::buffChangeTenacity, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MOVE_SPEED_RATE, std::bind(&FightActor::buffChangeMoveSpeed, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FLOAT, std::bind(&FightActor::buffChangeDeFloat, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HIT_DOWN, std::bind(&FightActor::buffChangeDeHitDown, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FAINT, std::bind(&FightActor::buffFaintState, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FIX, std::bind(&FightActor::buffFixState, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FROZEN, std::bind(&FightActor::buffFrozenState, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_ICE_RESIST, std::bind(&FightActor::buffChangeIceResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FIRE_RESIST, std::bind(&FightActor::buffChangeFireResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_THUNDER_RESIST, std::bind(&FightActor::buffChangeThunderResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_WIND_RESIST, std::bind(&FightActor::buffChangeWindResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_LIGHT_RESIST, std::bind(&FightActor::buffChangeLightResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DARK_RESIST, std::bind(&FightActor::buffChangeDarkResist, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_ALL, std::bind(&FightActor::buffImmuAll, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_DEBUFF, std::bind(&FightActor::buffImmuFaint, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_FIX, std::bind(&FightActor::buffImmuFix, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_FROZEN, std::bind(&FightActor::buffImmuFrozen, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_INVINCIBLE, std::bind(&FightActor::buffInvincible, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_PA, std::bind(&FightActor::buffPA, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DISABLE_SKILL, std::bind(&FightActor::buffDisableSkill, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_REVERSE_MOVE, std::bind(&FightActor::buffReverseMove, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAD, std::bind(&FightActor::buffMad, this, std::placeholders::_1)}
    };
    
    std::map<int, std::function<void(Buff *)>> removeFuncs = {
        {BUFF_TYPE::BUFF_TYPE_CUR_HP, std::bind(&FightActor::removeCurrHpBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_HP, std::bind(&FightActor::removeMaxHpBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HP_RECOVER_SPEED, std::bind(&FightActor::removeHpRecoverBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CUR_MP, std::bind(&FightActor::removeCurrMpBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_MP, std::bind(&FightActor::removeMaxMpBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MP_RECOVER_SPEED, std::bind(&FightActor::removeMpRecoverBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CUR_AP, std::bind(&FightActor::removeCurrApBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAX_AP, std::bind(&FightActor::removeMaxApBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_AP_RECOVER_SPEED, std::bind(&FightActor::removeApRecoverBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_ATTACK, std::bind(&FightActor::removeAttackBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DEFINE, std::bind(&FightActor::removeDefenceBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HIT_RATIO, std::bind(&FightActor::removeHitRatioBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_AVOIDANCE, std::bind(&FightActor::removeDodgeRatioBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_CRIT_RATE, std::bind(&FightActor::removeCritRatioBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_TENACITY, std::bind(&FightActor::removeTenacityBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MOVE_SPEED_RATE, std::bind(&FightActor::removeMoveSpeedBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FLOAT, std::bind(&FightActor::removeDefloatBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_HIT_DOWN, std::bind(&FightActor::removeDehitDownBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FAINT, std::bind(&FightActor::removeFaintBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FIX, std::bind(&FightActor::removeFixBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FROZEN, std::bind(&FightActor::removeFrozenBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_ICE_RESIST, std::bind(&FightActor::removeIceResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_FIRE_RESIST, std::bind(&FightActor::removeFireResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_THUNDER_RESIST, std::bind(&FightActor::removeThunderResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_WIND_RESIST, std::bind(&FightActor::removeWindResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_LIGHT_RESIST, std::bind(&FightActor::removeLightResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DARK_RESIST, std::bind(&FightActor::removeDarkResistBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_ALL, std::bind(&FightActor::removeImmuAllDeBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_DEBUFF, std::bind(&FightActor::removeImmuFaintBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_FIX, std::bind(&FightActor::removeImmuFixBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_IMMU_FROZEN, std::bind(&FightActor::removeImmuFrozenBuff, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_INVINCIBLE, std::bind(&FightActor::removeInvincible, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_PA, std::bind(&FightActor::removePA, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_DISABLE_SKILL, std::bind(&FightActor::removeDisableSkill, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_REVERSE_MOVE, std::bind(&FightActor::removeReverseMove, this, std::placeholders::_1)},
        {BUFF_TYPE::BUFF_TYPE_MAD, std::bind(&FightActor::removeMad, this, std::placeholders::_1)}
    };
    FightActor * colossus;
    FightActor * Guard;
};

#endif /* defined(__soul__FightActor__) */
