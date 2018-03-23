//
//  ClsSkill.cpp
//  son
//
//  Created by ff on 15-5-25.
//
//

#include "ClsSkill.h"

using namespace cocostudio;

ClsSkill::ClsSkill() :
coloSkillId(0),
rate(0),
skillId(0)
{
    
}

ClsSkill::~ClsSkill()
{
    
}

void ClsSkill::readFile(const rapidjson::Value &dic)
{
    coloSkillId = DICTOOL->getIntValue_json(dic, "ID");
    rate = DICTOOL->getIntValue_json(dic, "rate");
    skillId = DICTOOL->getIntValue_json(dic, "skillId");
    icon = DICTOOL->getStringValue_json(dic, "icon");
    actionName = DICTOOL->getStringValue_json(dic, "action");
    desc = DICTOOL->getStringValue_json(dic, "desc");
}