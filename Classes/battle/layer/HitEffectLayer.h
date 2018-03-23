//
//  HitEffectLayer.h
//  cosoul
//
//  Created by ff on 14-10-23.
//
//

#ifndef __cosoul__HitEffectLayer__
#define __cosoul__HitEffectLayer__

#include "cocos2d.h"
#include "../actor/Effect.h"

class HitEffectLayer : public cocos2d::Layer
{
public:
    bool init();
    
    CREATE_FUNC(HitEffectLayer);
    
    void addHurtNum(bool hero, int num, cocos2d::Vec2 pos);
    
    void addCritHurtNum(bool hero, int num, cocos2d::Vec2 pos);
    
    void addCritFlag(cocos2d::Vec2 pos);
    
    void addMissFlag(cocos2d::Vec2 pos);
    
    void addArmatureEff(std::string resPath, cocos2d::Vec2 pos);
    
    void delEffect(Effect *effect);
    
    void delArmEffect(cocostudio::Armature *, cocostudio::MovementEventType, const std::string &);
};

#endif /* defined(__cosoul__HitEffectLayer__) */
