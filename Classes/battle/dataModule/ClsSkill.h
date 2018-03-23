//
//  ClsSkill.h
//  son
//
//  Created by ff on 15-5-25.
//
//

#ifndef __son__ClsSkill__
#define __son__ClsSkill__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class ClsSkill
{
public:
    ClsSkill();
    ~ClsSkill();
    
    void readFile(const rapidjson::Value& dic);
private:
    CC_SYNTHESIZE_READONLY(int, colossusId, ColossusId);
    CC_SYNTHESIZE_READONLY(int, skillId, SkillId);
    CC_SYNTHESIZE_READONLY(float, rate, Rate);
	CC_SYNTHESIZE_READONLY(int, coloSkillId, ColoSkillId);
	CC_SYNTHESIZE_READONLY(std::string, icon, Icon);
	CC_SYNTHESIZE_READONLY(std::string, actionName, ActionName);
	CC_SYNTHESIZE_READONLY(std::string, desc, Desc);

};

#endif /* defined(__son__ClsSkill__) */
