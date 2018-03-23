//
//  BlackBullet.h
//  cocosGame
//
//  Created by ff on 14-12-15.
//
//

#ifndef __cocosGame__BlackBullet__
#define __cocosGame__BlackBullet__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "SkillTag.h"

class BlackBullet
{
public:
    void readFile(int idx, std::string filename);
    
    bool initData(int idx, rapidjson::Document& _doc);
    
    static void initIdTable(rapidjson::Document &_doc);
private:
    CC_SYNTHESIZE_READONLY(int, bulletId, BulletId);
    CC_SYNTHESIZE_READONLY(float, speed, Speed);
    CC_SYNTHESIZE_READONLY(std::string, resName, ResName);
    CC_SYNTHESIZE_READONLY(float, scaleRate, ScaleRate);
//    CC_SYNTHESIZE_READONLY(bool, isSplash, Splash);
    CC_SYNTHESIZE_READONLY(float, width, Width);
    CC_SYNTHESIZE_READONLY(float, height, Height);
//    CC_SYNTHESIZE_READONLY(int, bufferId, BufferId);
//    CC_SYNTHESIZE(float, attack, Attack);
    
//    CC_SYNTHESIZE(float, iceResist, IceResist);
//    CC_SYNTHESIZE(float, fireResist, FireResist);
//    CC_SYNTHESIZE(float, thunderResist, ThunderResist);
//    CC_SYNTHESIZE(float, windResist, WindResist);
//    CC_SYNTHESIZE(float, lightResist, LightResist);
//    CC_SYNTHESIZE(float, darkResist, DarkResist);

    CC_SYNTHESIZE_READONLY(int, bulletType, BulletType);
    
    CC_SYNTHESIZE_READONLY(int, count, Count);
    CC_SYNTHESIZE_READONLY(float, interval, Interval);
    CC_SYNTHESIZE_READONLY(int, skillId, SkillId);
    CC_SYNTHESIZE_READONLY(bool, copyReverse, CopyReverse);
    CC_SYNTHESIZE_READONLY(bool, through, Through);
    CC_SYNTHESIZE_READONLY(int, distDis, DistDis);
    CC_SYNTHESIZE_READONLY(int, distRange, distRange);
    CC_SYNTHESIZE_READONLY(bool, calAngel, CalAngel);
public:
    std::map<int, SkillTag *> selSkillTags;
    
    static std::map<int, int> idTable;
};
#endif /* defined(__cocosGame__BlackBullet__) */
