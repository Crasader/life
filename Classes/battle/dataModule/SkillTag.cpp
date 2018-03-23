//
//  SkillTag.cpp
//  cocosGame
//
//  Created by ff on 14-12-18.
//
//

#include "SkillTag.h"
#include "utils/GameReader.h"
#include "utils/GameUtils.h"

using namespace cocostudio;

std::map<int, int> SkillTag::idTable;
std::map<int, SkillTag> SkillTag::skillTags;


void SkillTag::readFileAll(std::string filename)
{
	auto doc = GameReader::getDocInstance(filename);
	if (doc->IsArray()){
		int size = doc->Size();

		for (int i = 0; i < size; i++) {
			const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
			SkillTag skillTag;
			skillTag.skillTagId = DICTOOL->getIntValue_json(dic, "ID");

			skillTag.attack = DICTOOL->getFloatValue_json(dic, "attack");
			skillTag.atkStiffDur = DICTOOL->getFloatValue_json(dic, "atkStiff");
			skillTag.hitStiffDur = DICTOOL->getFloatValue_json(dic, "hurtStiff");
			skillTag.chatterLoop = DICTOOL->getIntValue_json(dic, "chatterLoop");
            skillTag.chatterRange = DICTOOL->getFloatValue_json(dic, "chatterRange");
			skillTag.hitBackSpeed = DICTOOL->getFloatValue_json(dic, "hitBackSpeed");
			skillTag.airAtkStiffDur = DICTOOL->getFloatValue_json(dic, "airAtkStiff");
			skillTag.airHitStiffDur = DICTOOL->getFloatValue_json(dic, "airHurtStiff");
			skillTag.airChatterLoop = DICTOOL->getIntValue_json(dic, "airChatterLoop");
			skillTag.airHitBackSpeed = DICTOOL->getFloatValue_json(dic, "airHitBackSpeed");
			skillTag.airAtkUpVal = DICTOOL->getFloatValue_json(dic, "airAtkUpVal");
			skillTag.floatRate = DICTOOL->getFloatValue_json(dic, "floatRate");
            skillTag.boundUpRate = DICTOOL->getFloatValue_json(dic, "boundUPRate");
			skillTag.hitDownVal = DICTOOL->getFloatValue_json(dic, "hitDownVal");
			skillTag.hitFlyRate = DICTOOL->getFloatValue_json(dic, "hitDownRate");
			skillTag.fullScreenRange = DICTOOL->getIntValue_json(dic, "fullScreenRange");
			skillTag.hitEffect = DICTOOL->getStringValue_json(dic, "hitEffectRes");
//			skillTag.skillSoundRes = "sound/";
//            skillTag.humanSoundRes = "sound/";
//			skillTag.hitSoundRes = "sound/";
			skillTag.skillSoundRes += DICTOOL->getStringValue_json(dic, "sound");
            skillTag.humanSoundRes += DICTOOL->getStringValue_json(dic, "humanSound");
			skillTag.hitSoundRes += DICTOOL->getStringValue_json(dic, "hitSound");
			skillTag.bulletId = DICTOOL->getIntValue_json(dic, "bullet");

			skillTag.iceResist = DICTOOL->getFloatValue_json(dic, "iceResist");
			skillTag.fireResist = DICTOOL->getFloatValue_json(dic, "fireResist");
			skillTag.thunderResist = DICTOOL->getFloatValue_json(dic, "thunderResist");
			skillTag.windResist = DICTOOL->getFloatValue_json(dic, "windResist");
			skillTag.lightResist = DICTOOL->getFloatValue_json(dic, "lightResist");
			skillTag.darkResist = DICTOOL->getFloatValue_json(dic, "darkResist");
            
            for (int i = 1; i <= 2; i++) {
                int buffIdx = 0;
                int buffRatio = 0;
                std::string buffStr("buffId");
                buffStr += Convert2String(i);
                buffIdx = DICTOOL->getIntValue_json(dic, buffStr.c_str());
                std::string buffRateStr("buff");
                buffRateStr += Convert2String(i);
                buffRateStr += "Ratio";
                buffRatio = DICTOOL->getIntValue_json(dic, buffRateStr.c_str());
                if (buffIdx != 0) {
                    skillTag.buffId[buffIdx] = buffRatio;
                }
            }

			
		}
	}
}

void SkillTag::readFile(int idx, std::string filename)
{
	auto doc = GameReader::getDocInstance(filename);
	if (SkillTag::idTable.empty())
	{
		SkillTag::initIdTable(*doc);
	}
	initData(idx, *doc);
}

void SkillTag::initIdTable(rapidjson::Document &_doc){
	if (_doc.IsArray()){
		int size = _doc.Size();
		int objId;
		for (int i = 0; i < size; i++) {
			objId = DICTOOL->getIntValue_json(_doc[i], "ID");
			idTable.insert(std::make_pair(objId, i));
		}
	}
}

bool SkillTag::initData(int idx, rapidjson::Document& _doc) {
	int index = idTable.find(idx)->second;

	const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
	if (dic.IsNull()) {
		return false;
	}

	skillTagId = DICTOOL->getIntValue_json(dic, "ID");

	attack = DICTOOL->getFloatValue_json(dic, "attack");
	atkStiffDur = DICTOOL->getFloatValue_json(dic, "atkStiff");
	hitStiffDur = DICTOOL->getFloatValue_json(dic, "hurtStiff");
	chatterLoop = DICTOOL->getIntValue_json(dic, "chatterLoop");
    chatterRange = DICTOOL->getFloatValue_json(dic, "chatterRange");
	hitBackSpeed = DICTOOL->getFloatValue_json(dic, "hitBackSpeed");
	airAtkStiffDur = DICTOOL->getFloatValue_json(dic, "airAtkStiff");
	airHitStiffDur = DICTOOL->getFloatValue_json(dic, "airHurtStiff");
	airChatterLoop = DICTOOL->getIntValue_json(dic, "airChatterLoop");
	airHitBackSpeed = DICTOOL->getFloatValue_json(dic, "airHitBackSpeed");
	airAtkUpVal = DICTOOL->getFloatValue_json(dic, "airAtkUpVal");
	floatRate = DICTOOL->getFloatValue_json(dic, "floatRate");
    boundUpRate = DICTOOL->getFloatValue_json(dic, "boundUPRate");
	hitDownVal = DICTOOL->getFloatValue_json(dic, "hitDownVal");
	hitFlyRate = DICTOOL->getFloatValue_json(dic, "hitDownRate");
	fullScreenRange = DICTOOL->getIntValue_json(dic, "fullScreenRange");
	hitEffect = DICTOOL->getStringValue_json(dic, "hitEffectRes");
//	skillSoundRes = "sound/";
//    humanSoundRes = "sound/";
//	hitSoundRes = "sound/";
	skillSoundRes += DICTOOL->getStringValue_json(dic, "sound");
    humanSoundRes += DICTOOL->getStringValue_json(dic, "humanSound");
	hitSoundRes += DICTOOL->getStringValue_json(dic, "hitSound");
	bulletId = DICTOOL->getIntValue_json(dic, "bullet");

	iceResist = DICTOOL->getFloatValue_json(dic, "iceResist");
	fireResist = DICTOOL->getFloatValue_json(dic, "fireResist");
	thunderResist = DICTOOL->getFloatValue_json(dic, "thunderResist");
	windResist = DICTOOL->getFloatValue_json(dic, "windResist");
	lightResist = DICTOOL->getFloatValue_json(dic, "lightResist");
	darkResist = DICTOOL->getFloatValue_json(dic, "darkResist");
    atkMove = DICTOOL->getFloatValue_json(dic, "atkMove");
    const char *szTemp = DICTOOL->getStringValue_json(dic, "AtkRect");
    atkAreaThick = DealWithRectString(szTemp, &atkrect);
    
    for (int i = 1; i <= 2; i++) {
        int buffIdx = 0;
        int buffRatio = 0;
        std::string buffStr("buffId");
        buffStr += Convert2String(i);
        buffIdx = DICTOOL->getIntValue_json(dic, buffStr.c_str());
        std::string buffRateStr("buff");
        buffRateStr += Convert2String(i);
        buffRateStr += "Ratio";
        buffRatio = DICTOOL->getIntValue_json(dic, buffRateStr.c_str());
        if (buffIdx != 0) {
            buffId[buffIdx] = buffRatio;
        }
    }
    
    std::string clsSkillStr = DICTOOL->getStringValue_json(dic, "ColSkill");
    if (clsSkillStr != "null") {
        std::vector<std::string> splitVec = GameUtils::splitStr(clsSkillStr, ";");
        for (auto str : splitVec) {
            if (atoi(str.c_str()) == 0) {
                continue;
            }
            coloSkill.push_back(atoi(str.c_str()));
        }
    }
    return true;
}

float SkillTag::DealWithRectString(std::string strTemp, cocos2d::Rect *pRect)
{
	/************************************************************************/
	// 处理范围字符串
	// 1. 判断字符串的格式是否正确
	int nTemp = static_cast<int>(count(strTemp.begin(), strTemp.end(), ','));
	if (nTemp != 4)
		return 0;

	// 2. 替换字符串中的"(" , ")" , ","为空的字符
	replace(strTemp.begin(), strTemp.end(), '(', ' ');
	replace(strTemp.begin(), strTemp.end(), ')', ' ');
	replace(strTemp.begin(), strTemp.end(), ',', ' ');

	// 3. 解析该字符串并输出到Rect中去
	float fTemp[5] = { 0 };
	std::istringstream stream(strTemp);
	for (int nIndex = 0; nIndex < 5; ++nIndex)
	{
		stream >> fTemp[nIndex];
	}
	
	pRect->setRect(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
    
    return fTemp[4];
}
