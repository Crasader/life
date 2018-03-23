//
//  FightFormula.h
//  cocosGame
//
//  Created by ff on 14-12-25.
//
//

#ifndef __cocosGame__FightFormula__
#define __cocosGame__FightFormula__

#include "Skill.h"
#include "BlackActor.h"
//#include "BlackBullet.h"
#include "SkillTag.h"

class FightFormula
{
private:
    float static calAttrHurt(float baseHurt, SkillTag *skill, BlackActor *atk, BlackActor *def);
//    float calBulletHurt(float baseHurt, BlackBullet *bullet, BlackActor *def);
    float static calAttrHurt1(float baseHurt, float attackAttr, float defenceAttr);
public:
    float static calSkillHurt(SkillTag *skillTag, int baseAttack, BlackActor *def);
    
//    float calBulletHurt(BlackBullet *bullet, BlackActor *def);
    
    bool static isMiss(int hitRatio, int dodgeRatio);
    
    bool static isCirt(float critRatio, BlackActor *defencer);
    
    bool static isImmuBuff(BlackActor *defencer);
   
    bool static isHit2Air(BlackActor *def);
    
    bool static isHitDown(BlackActor *def);
};

#endif /* defined(__cocosGame__FightFormula__) */
