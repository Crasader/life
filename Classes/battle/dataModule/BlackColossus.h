//
//  BlackColossus.h
//  son
//
//  Created by ff on 15-5-25.
//
//

#ifndef __son__BlackColossus__
#define __son__BlackColossus__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "BlackActor.h"
#include "ClsSkill.h"

class BlackColossus
{
public:
  	BlackColossus();
	~BlackColossus();  
	void readColossus(int colossusId, std::string filename);
	std::map<int, ClsSkill> clsSkills;
private:

	CC_SYNTHESIZE_READONLY(int, colossusId, ColossusId);
	CC_SYNTHESIZE_READONLY(int, skillId, SkillId);
	CC_SYNTHESIZE_READONLY(float, rate, Rate);
	CC_SYNTHESIZE_READONLY(int, coloSkillId, ColoSkillId);
	CC_SYNTHESIZE_READONLY(std::string, icon, Icon);
	CC_SYNTHESIZE_READONLY(std::string, actionName, ActionName);
	CC_SYNTHESIZE_READONLY(std::string, desc, Desc);

//	std::map<int, ClsSkill *> getColossusSkills();
//	static BlackColossus* getInstance(){ static BlackColossus _instance; return &_instance;};
};


#endif /* defined(__son__BlackColossus__) */
