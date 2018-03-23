//
//  FightFormula.cpp
//  cocosGame
//
//  Created by ff on 14-12-25.
//
//

#include "FightFormula.h"

float FightFormula::calSkillHurt(SkillTag *skillTag, int baseAttack, BlackActor *def)
{
    float result = 1;
    float attackVal = 1;
    attackVal = baseAttack;
    float defence = def->getDefence() + def->getBuffDefence();
    defence = MAX(1, defence);
    float baseHurt = (attackVal * attackVal)/(attackVal + defence) * 2 * skillTag->getAttack();
    
//    float attrHurt = calAttrHurt(baseHurt, skillTag, atk, def);
    float attrHurt = 0;
    
    result = MAX(1, (baseHurt + attrHurt));
    return result;
//    return 15;
}

float FightFormula::calAttrHurt(float baseHurt, SkillTag *skill, BlackActor *atk, BlackActor *def)
{
    float attrHurt = 0;
    if (skill->getIceResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getIceResist() + atk->getIceResist(), def->getIceResist());
    }
    if (skill->getFireResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getFireResist() + atk->getFireResist(), def->getFireResist());
    }
    if (skill->getThunderResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getThunderResist() + atk->getThunderResist(), def->getThunderResist());
    }
    if (skill->getWindResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getWindResist() + atk->getWindResist(), def->getWindResist());
    }
    if (skill->getLightResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getLightResist() + atk->getLightResist(), def->getLightResist());
    }
    if (skill->getDarkResist() != 0) {
        attrHurt += calAttrHurt1(baseHurt, skill->getDarkResist() + atk->getDarkResist(), def->getDarkResist());
    }
    return attrHurt;
}

//float FightFormula::calBulletHurt(float baseHurt, BlackBullet *bullet, BlackActor *def)
//{
//    float attrHurt = 0;
////    if (bullet->getIceResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getIceResist(), def->getIceResist());
////    }
////    if (bullet->getFireResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getFireResist(), def->getFireResist());
////    }
////    if (bullet->getThunderResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getThunderResist(), def->getThunderResist());
////    }
////    if (bullet->getWindResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getWindResist(), def->getWindResist());
////    }
////    if (bullet->getLightResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getLightResist(), def->getLightResist());
////    }
////    if (bullet->getDarkResist() != 0) {
////        attrHurt += calAttrHurt1(baseHurt, bullet->getDarkResist(), def->getDarkResist());
////    }
//    return attrHurt;
//}

float FightFormula::calAttrHurt1(float baseHurt, float attackAttr, float defenceAttr)
{
    float result = 0;
    float temp1 = (attackAttr - defenceAttr)/100;
    result = atan(temp1)*0.1;
    return result * baseHurt;
//    return 1;
}

//float FightFormula::calBulletHurt(BlackBullet *bullet, BlackActor *def)
//{
//    float result = 1;
//    float attackVal = bullet->getAttack();
//    float baseHurt = (attackVal * attackVal)/(attackVal + def->getDefence()) * 2;
//    
//    float attrHurt = calBulletHurt(baseHurt, bullet, def);
//    
//    result = MAX(1, (baseHurt + attrHurt));
//    return result;
////    return 50;
//}

bool FightFormula::isMiss(int hitRatio, int dodgeRatio)
{
    float threshold = hitRatio - dodgeRatio;
    float r = abs(rand())%100;
    if (r > threshold) {
        return true;
    }
    return false;
}

bool FightFormula::isCirt(float critRatio, BlackActor *defencer)
{
    float threshold = critRatio - defencer->getTenacity();
    threshold = threshold/(2.1*threshold+3000)*10000;
    threshold = MAX(1, threshold);
    
    float r = abs(rand())%10000;
    if (r < threshold) {
        return true;
    }
    return false;
}

bool FightFormula::isImmuBuff(BlackActor *defencer)
{
    float threshold = defencer->getImmuBuffRate();
    float r = abs(rand()) % 10000;
    if (r < threshold) {
        return true;
    }
    return false;
    
}

bool FightFormula::isHit2Air(BlackActor *def)
{

    float threshold = def->getDe2AirRate();
    float r = abs(rand()) % 10000;
    if (r > threshold) {
        return true;
    }
    return false;
}

bool FightFormula::isHitDown(BlackActor *def)
{
    float threshold = def->getDeDownRate();
    float r = abs(rand()) % 10000;
    if (r > threshold) {
        return true;
    }
    return false;
}
