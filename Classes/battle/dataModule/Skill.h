//
//  Skill.h
//  ssoul
//
//  Created by ff on 14-10-12.
//
//

#ifndef __ssoul__Skill__
#define __ssoul__Skill__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

enum SKILL_USE_ERROR
{
    SKILL_NONE_ERROR = 0,
    SKILL_CD_ERROR = 1,
    SKILL_MP_ERROR = 2,
    SKILL_AP_ERROR = 3,
    SKILL_HP_ERROR = 4,
    SKILL_GHOST_ERROR = 5,
    SKILL_PET_COUNT_ERROR = 6,
    SKILL_OTHER_ERROR = 7
};
class Skill
{
public:
	Skill();
	~Skill();

	static void readFileAll(std::string filename);

	static void initIdTable(rapidjson::Document &_doc);

	void readFile(int idx, std::string filename);

	bool initData(int idx, rapidjson::Document& _doc);
    
    void updateClock(float dt);
    
    void startCool();
private:
	CC_SYNTHESIZE_READONLY(std::string, name, Name);
	CC_SYNTHESIZE_READONLY(int, skillID, SkillID);

	CC_SYNTHESIZE_READONLY(std::string, action, Action);
	CC_SYNTHESIZE_READONLY(float, coolDownDur, CoolDownDur);
	CC_SYNTHESIZE_READONLY(float, baseSkillID, BaseSkillID);
	CC_SYNTHESIZE_READONLY(float, linkDur, LinkDur);
	CC_SYNTHESIZE(int, linkID, LinkID);
	CC_SYNTHESIZE_READONLY(int, brokeID, BrokeID);
	CC_SYNTHESIZE_READONLY(int, xuliId, XuliId);
	CC_SYNTHESIZE_READONLY(float, xuliDur, XuliDur);
	CC_SYNTHESIZE_READONLY(bool, canBreak, CanBreak);
    CC_SYNTHESIZE_READONLY(std::vector<std::string>, frontEffectStr, FrontEffectStr);
	CC_SYNTHESIZE_READONLY(std::vector<std::string>, backEffectStr, BackEffectStr);
    CC_SYNTHESIZE_READONLY(std::vector<float>, frontScale, FrontScale);
    CC_SYNTHESIZE_READONLY(std::vector<float>, backScale, BackScale);
	CC_SYNTHESIZE_READONLY(std::string, iconStr, iconStr);
	CC_SYNTHESIZE_READONLY(bool, effectFollowY, EffectFollowY);


	CC_SYNTHESIZE_READONLY(float, consumeMP, ConsumeMP);
	CC_SYNTHESIZE_READONLY(float, consumeHP, ConsumeHP);
	CC_SYNTHESIZE_READONLY(float, consumeAP, ConsumeAP);
    CC_SYNTHESIZE_READONLY(float, consumeGhost, ConsumeGhost);

    CC_SYNTHESIZE_READONLY(int, skillType, SkillType);
    CC_SYNTHESIZE(bool, cooling, Cooling);
    
    CC_SYNTHESIZE(float, coolTime, CoolTime);
    
    CC_SYNTHESIZE(float, validDis, ValidDis);
    CC_SYNTHESIZE(bool, hasBullet, HasBullet);
    
	//    CC_SYNTHESIZE_READONLY(float, iceResist, IceResist);
	//    CC_SYNTHESIZE_READONLY(float, fireResist, FireResist);
	//    CC_SYNTHESIZE_READONLY(float, thunderResist, ThunderResist);
	//    CC_SYNTHESIZE_READONLY(float, windResist, WindResist);
	//    CC_SYNTHESIZE_READONLY(float, lightResist, LightResist);
	//    CC_SYNTHESIZE_READONLY(float, darkResist, DarkResist);

	//    CC_SYNTHESIZE_READONLY(float, attack, Attack);
	//    CC_SYNTHESIZE_READONLY(int, hitCount, HitCount);
	//    CC_SYNTHESIZE_READONLY(float, atkStiffDur, AtkStiffDur);
	//    CC_SYNTHESIZE_READONLY(float, hitStiffDur, HitStiffDur);
	//    CC_SYNTHESIZE_READONLY(int, chatterLoop, ChatterLoop);
	//    CC_SYNTHESIZE_READONLY(float, hitBackSpeed, HitBackSpeed);
	//    CC_SYNTHESIZE_READONLY(float, airAtkStiffDur, AirAtkStiffDur);
	//    CC_SYNTHESIZE_READONLY(float, airHitStiffDur, AirHitStiffDur);
	//    CC_SYNTHESIZE_READONLY(int, airChatterLoop, AirChatterLoop);
	//    CC_SYNTHESIZE_READONLY(float, airHitBackSpeed, AirHitBackSpeed);
	//    CC_SYNTHESIZE_READONLY(float, airAtkUpVal, AirAtkUpVal);
	//    CC_SYNTHESIZE_READONLY(float, floatRate, FloatRate);
	//    CC_SYNTHESIZE_READONLY(float, hitDownVal, HitDownVal);
	//    CC_SYNTHESIZE_READONLY(std::string, hitEffect, HitEffect);
	//    CC_SYNTHESIZE_READONLY(float, fullScreenRange, FullScreenRange);
	//    CC_SYNTHESIZE_READONLY(std::string, skillSoundRes, SkillSoundRes);
	//    CC_SYNTHESIZE_READONLY(std::string, hitSoundRes, HitSoundRes);
	//    CC_SYNTHESIZE_READONLY(int, bulletId, BulletID);

public:
	static std::map<int, Skill> skills;

	static std::map<int, int> idTable;

};

#endif /* defined(__ssoul__Skill__) */
