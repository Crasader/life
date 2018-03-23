//
//  SkillTag.h
//  cocosGame
//
//  SkillTag.h
//  cocosGame
//
//  Created by ff on 14-12-18.
//
//

#ifndef __cocosGame__SkillTag__
#define __cocosGame__SkillTag__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class SkillTag
{
public:
	static void readFileAll(std::string filename);

	static void initIdTable(rapidjson::Document &_doc);

	void readFile(int idx, std::string filename);

	bool initData(int idx, rapidjson::Document& _doc);
    
    
public:
    int skillTagId;
    inline int getSkillTagId(){
        return skillTagId;
    };
private:
    
//	CC_SYNTHESIZE_READONLY(int, skillTagId, SkillTagId);

	CC_SYNTHESIZE_READONLY(float, attack, Attack);
	CC_SYNTHESIZE_READONLY(int, hitCount, HitCount);
	CC_SYNTHESIZE_READONLY(float, atkStiffDur, AtkStiffDur);
	CC_SYNTHESIZE_READONLY(float, hitStiffDur, HitStiffDur);
	CC_SYNTHESIZE_READONLY(int, chatterLoop, ChatterLoop);
    CC_SYNTHESIZE_READONLY(float, chatterRange, ChatterRange)
	CC_SYNTHESIZE_READONLY(float, hitBackSpeed, HitBackSpeed);
	CC_SYNTHESIZE_READONLY(float, airAtkStiffDur, AirAtkStiffDur);
	CC_SYNTHESIZE_READONLY(float, airHitStiffDur, AirHitStiffDur);
	CC_SYNTHESIZE_READONLY(int, airChatterLoop, AirChatterLoop);
	CC_SYNTHESIZE_READONLY(float, airHitBackSpeed, AirHitBackSpeed);
	CC_SYNTHESIZE_READONLY(float, airAtkUpVal, AirAtkUpVal);
	CC_SYNTHESIZE_READONLY(float, floatRate, FloatRate);
    CC_SYNTHESIZE_READONLY(float, boundUpRate, BoundUpRate);
	CC_SYNTHESIZE_READONLY(float, hitDownVal, HitDownVal);
	CC_SYNTHESIZE_READONLY(float, hitFlyRate, hitFlyRate);
	CC_SYNTHESIZE_READONLY(std::string, hitEffect, HitEffect);
	CC_SYNTHESIZE_READONLY(float, fullScreenRange, FullScreenRange);
	CC_SYNTHESIZE_READONLY(std::string, skillSoundRes, SkillSoundRes);
    CC_SYNTHESIZE_READONLY(std::string, humanSoundRes, HumanSoundRes);
	CC_SYNTHESIZE_READONLY(std::string, hitSoundRes, HitSoundRes);
	CC_SYNTHESIZE_READONLY(int, bulletId, BulletID);

	CC_SYNTHESIZE_READONLY(float, iceResist, IceResist);
	CC_SYNTHESIZE_READONLY(float, fireResist, FireResist);
	CC_SYNTHESIZE_READONLY(float, thunderResist, ThunderResist);
	CC_SYNTHESIZE_READONLY(float, windResist, WindResist);
	CC_SYNTHESIZE_READONLY(float, lightResist, LightResist);
	CC_SYNTHESIZE_READONLY(float, darkResist, DarkResist);
	CC_SYNTHESIZE_READONLY(cocos2d::Rect, atkrect, AttackRect);
    CC_SYNTHESIZE_READONLY(float, atkAreaThick, AtkAreaThick);
    CC_SYNTHESIZE_READONLY(float, atkMove, AtkMove);

    CC_SYNTHESIZE_READONLY(int, buff1Id, Buff1Id);
    CC_SYNTHESIZE_READONLY(float, buff1Rate, Buff1Rate);
    CC_SYNTHESIZE_READONLY(int, buff2Id, Buff2Id);
    CC_SYNTHESIZE_READONLY(float, buff2Rate, Buff2Rate);
    
//    CC_SYNTHESIZE_READONLY(float, hit2AirRate, Hit2AirRate);
//    CC_SYNTHESIZE_READONLY(float, hitDownRate, HitDownRate);
//    CC_SYNTHESIZE_READONLY(float, hitFaintRate, HitFaintRate);
//    CC_SYNTHESIZE_READONLY(float, hitFrozenRate, HitFrozenRate);
//    CC_SYNTHESIZE_READONLY(float, hitFixRate, HitFixRate);
	std::map<int, int> buffId;
    std::vector<int> coloSkill;
public:
    static float DealWithRectString(std::string strTemp, cocos2d::Rect *pRect);		   // ´¦Àí·¶Î§×Ö·û´®

public:
	static std::map<int, SkillTag> skillTags;

	static std::map<int, int> idTable;   
};

#endif /* defined(__cocosGame__SkillTag__) */
