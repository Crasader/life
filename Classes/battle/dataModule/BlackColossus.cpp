//
//  BlackColossus.cpp
//  son
//
//  Created by ff on 15-5-25.
//
//

#include "BlackColossus.h"
#include "utils/GameReader.h"

using namespace cocostudio;

BlackColossus::BlackColossus()
{
    clsSkills.clear();
}

BlackColossus::~BlackColossus()
{
    clsSkills.clear();
}

void BlackColossus::readColossus(int colossusId, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        clsSkills.clear();
        for (int i = 0; i < size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            
            int clsId = DICTOOL->getIntValue_json(dic, "Colossus");
            if (clsId != colossusId) {
                continue;
            }
            int clsSkillId = DICTOOL->getIntValue_json(dic, "ID");
            ClsSkill skill;
            skill.readFile(dic);
            clsSkillId /= 1000;
            clsSkills[clsSkillId] = skill;
        }
    }
}

//std::map<int, ClsSkill *> BlackColossus::getColossusSkills()
//{
//    return clsSkills;
//}
