//
//  Bullet.h
//  cocosGame
//
//  Created by ff on 14-12-15.
//
//

#ifndef __cocosGame__Bullet__
#define __cocosGame__Bullet__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
//#include "../dataModule/BlackBullet.h"
#include "../dataModule/SkillTag.h"
#include "../dataModule/Skill.h"
#include "Actor.h"
//#include "utils/sharde/ShardeArmature.h"

class FightActor;

enum BULLET_STATE
{
    FLY,
    BOOM,
    LAND
};

class FindSkillTag
{
public:
    FindSkillTag(const int val):skillVal(val){};
    bool operator () (const std::map<int, int>::value_type &pair) const
    {
        return pair.first/100 == skillVal;
    }
private:
    int skillVal;
};

class Bullet : public Actor
{
public:
    virtual bool init();
    void setupData(cocos2d::EventCustom *event);
    bool changeState(BULLET_STATE nextState);
    void fillSkillTags(int skillId);
    void initWithId(int idx, FightActor *owner, float gunPosX, float gunPosY);
    void initAttr(SkillTag *skill, FightActor *owner);
    void Logic(float dt);
    cocos2d::Rect getAtkRect();
    
    bool isCollide(cocos2d::Rect bRect);
    
    bool isOutsideCamera(float cameraX);
    bool isOutsideMap();
    
    bool isAnimationOver();
    
    void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
    CREATE_FUNC(Bullet);
private:
    std::map<int, SkillTag*> selSkillTags;
    CC_SYNTHESIZE_READONLY(BULLET_STATE, state, State);
    CC_SYNTHESIZE_READONLY(bool, flipX, FlipX);
    CC_SYNTHESIZE_READONLY(float, speed, Speed);
    
    CC_SYNTHESIZE(int, bulletType, BulletType);
    
//    CC_SYNTHESIZE_READONLY(cocostudio::Armature *, armature, Armature);
    
    CC_SYNTHESIZE_READONLY(float, scaleRate, ScaleRate);

    CC_SYNTHESIZE_READONLY(float, width, Width);
    CC_SYNTHESIZE_READONLY(float, height, Height);
    CC_SYNTHESIZE_READONLY(bool, through, Through);
    CC_SYNTHESIZE_READONLY(bool, calAngel, CalAngel);
    CC_SYNTHESIZE_READONLY(float, distPosX, DistPosX);
    CC_SYNTHESIZE_READONLY(int, ownerType, OwnerType);
    CC_SYNTHESIZE_READONLY(bool, spurFly, SpurFly);
    CC_SYNTHESIZE_READONLY(float, baseAttack, BaseAttack);
    CC_SYNTHESIZE_READONLY(float, baseCritRatio, BaseCritRatio);
    CC_SYNTHESIZE_READONLY(float, distY, DistY);
    CC_SYNTHESIZE_READONLY(int, comboFactor, ComboFactor);
    
    CC_SYNTHESIZE(bool, isAlive, Alive);
    
    CC_SYNTHESIZE_READONLY(int, skillId, SkillId);

    CC_SYNTHESIZE_READONLY(int, attackTag, AttackTag);
    CC_SYNTHESIZE_READONLY(bool, atking, Atking);
    CC_SYNTHESIZE_READONLY(bool, attackHead, AttackHead);
    CC_SYNTHESIZE_READONLY(bool, attackWaist, AttackWaist);
    
    CC_SYNTHESIZE_READONLY(bool, hit2Air, Hit2Air);
    CC_SYNTHESIZE_READONLY(bool, hit2Ground, Hit2Ground);
    CC_SYNTHESIZE_READONLY(bool, hit2Wall, Hit2Wall);
    
    CC_SYNTHESIZE_READONLY(SkillTag*, currSkillTag, CurrSkillTag);
};

#endif /* defined(__cocosGame__Bullet__) */
