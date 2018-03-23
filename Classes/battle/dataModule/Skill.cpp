//
//  Skill.cpp
//  ssoul
//
//  Created by ff on 14-10-12.
//
//

#include "Skill.h"
#include "utils/GameReader.h"

using namespace cocos2d;
using namespace cocostudio;

std::map<int, int> Skill::idTable;

std::map<int, Skill> Skill::skills;
Skill::Skill():
cooling(false),
coolTime(0),validDis(0)
{

}

Skill::~Skill()
{

}
void Skill::readFileAll(std::string filename)
{
    
    
	auto doc = GameReader::getDocInstance(filename);
	if (doc->IsArray()){
		int size = doc->Size();

		for (int i = 0; i < size; i++) {
			const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
			Skill skill;
			skill.skillID = DICTOOL->getIntValue_json(dic, "ID");
			skill.name = DICTOOL->getStringValue_json(dic, "name");
			skill.action = DICTOOL->getStringValue_json(dic, "action");
			skill.coolDownDur = DICTOOL->getFloatValue_json(dic, "coolDownVal");
			skill.baseSkillID = DICTOOL->getIntValue_json(dic, "baseSkill");
			skill.linkDur = DICTOOL->getFloatValue_json(dic, "linkTime");
			skill.linkID = DICTOOL->getFloatValue_json(dic, "linkID");
			skill.brokeID = DICTOOL->getIntValue_json(dic, "brokeID");
			skill.xuliId = DICTOOL->getIntValue_json(dic, "xuqi");
			skill.xuliDur = DICTOOL->getFloatValue_json(dic, "xuqiDuration");
//			skill.frontEffectStr = DICTOOL->getStringValue_json(dic, "frontEffectRes");
//			skill.backEffectStr = DICTOOL->getStringValue_json(dic, "backEffectRes");
            std::string frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes1");
            skill.frontEffectStr.push_back(frontEffStr);
            frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes2");
            skill.frontEffectStr.push_back(frontEffStr);
            frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes3");
            skill.frontEffectStr.push_back(frontEffStr);
            
            std::string backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes1");
            skill.backEffectStr.push_back(backEffStr);
            backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes2");
            skill.backEffectStr.push_back(backEffStr);
            backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes3");
            skill.backEffectStr.push_back(backEffStr);
            
			skill.canBreak = (DICTOOL->getIntValue_json(dic, "unBreak") == 0 ? true : false);
			skill.iconStr = DICTOOL->getStringValue_json(dic, "icon");
			skill.effectFollowY = (DICTOOL->getIntValue_json(dic, "effFollowY") == 0 ? false : true);

			skill.consumeHP = DICTOOL->getIntValue_json(dic, "consumeHp");
			skill.consumeMP = DICTOOL->getIntValue_json(dic, "consumeMp");
			skill.consumeAP = DICTOOL->getIntValue_json(dic, "consumAp");
            skill.consumeGhost = DICTOOL->getIntValue_json(dic, "consumeGhost");

            skill.skillType = DICTOOL->getIntValue_json(dic, "skillType");
			skills[skill.skillID] = skill;
		}
	}
}

void Skill::readFile(int idx, std::string filename)
{
	auto doc = GameReader::getDocInstance(filename);
	if (Skill::idTable.empty())
	{
		Skill::initIdTable(*doc);
	}
	initData(idx, *doc);
}

void Skill::initIdTable(rapidjson::Document &_doc){
	if (_doc.IsArray()){
		int size = _doc.Size();
		int objId;
		for (int i = 0; i < size; i++) {
			objId = DICTOOL->getIntValue_json(_doc[i], "ID");
			idTable.insert(std::make_pair(objId, i));
		}
	}
}

bool Skill::initData(int idx, rapidjson::Document& _doc) {
	int index = idTable.find(idx)->second;

	const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
	if (dic.IsNull()) {
		return false;
	}


	skillID = DICTOOL->getIntValue_json(dic, "ID");
	name = DICTOOL->getStringValue_json(dic, "name");
	action = DICTOOL->getStringValue_json(dic, "action");
	coolDownDur = DICTOOL->getFloatValue_json(dic, "coolDownVal");
	baseSkillID = DICTOOL->getIntValue_json(dic, "baseSkill");
	linkDur = DICTOOL->getFloatValue_json(dic, "linkTime");
	linkID = DICTOOL->getFloatValue_json(dic, "linkID");
	brokeID = DICTOOL->getIntValue_json(dic, "brokeID");
	xuliId = DICTOOL->getIntValue_json(dic, "xuqi");
	xuliDur = DICTOOL->getFloatValue_json(dic, "xuqiDuration");
    std::string frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes1");
    frontEffectStr.push_back(frontEffStr);
    frontScale.push_back(DICTOOL->getFloatValue_json(dic, "f1scaleFactor"));
    frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes2");
    frontEffectStr.push_back(frontEffStr);
    frontScale.push_back(DICTOOL->getFloatValue_json(dic, "f2scaleFactor"));
    frontEffStr = DICTOOL->getStringValue_json(dic, "frontEffectRes3");
    frontEffectStr.push_back(frontEffStr);
    frontScale.push_back(DICTOOL->getFloatValue_json(dic, "f3scaleFactor"));
    
    std::string backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes1");
    backEffectStr.push_back(backEffStr);
    backScale.push_back(DICTOOL->getFloatValue_json(dic, "b1scaleFactor"));
    backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes2");
    backEffectStr.push_back(backEffStr);
    backScale.push_back(DICTOOL->getFloatValue_json(dic, "b2scaleFactor"));
    backEffStr = DICTOOL->getStringValue_json(dic, "backEffectRes3");
    backEffectStr.push_back(backEffStr);
    backScale.push_back(DICTOOL->getFloatValue_json(dic, "b3scaleFactor"));
    
	canBreak = (DICTOOL->getIntValue_json(dic, "unBreak") == 0 ? true : false);
	iconStr = DICTOOL->getStringValue_json(dic, "icon");
	effectFollowY = (DICTOOL->getIntValue_json(dic, "effFollowY") == 0 ? false : true);

    consumeHP = DICTOOL->getIntValue_json(dic, "consumeHp");
    consumeMP = DICTOOL->getIntValue_json(dic, "consumeMp");
    consumeAP = DICTOOL->getIntValue_json(dic, "consumAp");
    consumeGhost = DICTOOL->getIntValue_json(dic, "consumeGhost");
    
    skillType = DICTOOL->getIntValue_json(dic, "skillType");
//	consumeHP = 0;
//	consumeMP = 0;
//	consumeAP = 0;

//	for (int i = 1; i <= 2; i++) {
//		int buffIdx = 0;
//		int buffRatio = 0;
//		std::string buffStr("buffId");
//		buffStr += i;
//		buffIdx = DICTOOL->getIntValue_json(dic, buffStr.c_str());
//		std::string buffRateStr("buff");
//		buffRateStr += i;
//		buffRateStr += "Ratio";
//		buffRatio = DICTOOL->getIntValue_json(dic, buffRateStr.c_str());
//		if (buffIdx != 0) {
//			buffId[buffIdx] = buffRatio;
//		}
//	}

	return true;
}

void Skill::updateClock(float dt)
{
    if (!cooling) {
        return;
    }
    
    coolTime -= dt;
    if (skillID == 2001) {
        log("skill cd:%f", coolTime);
        log("cooling:%d", cooling);
    }
    
    if (coolTime <= 0) {
        coolTime = -1;
        cooling = false;
    }
}

void Skill::startCool()
{
    cooling = true;
    coolTime = coolDownDur;
}
