//
//  ItemSpirit.h
//  son
//
//  Created by ff on 15-5-28.
//
//

#ifndef __son__ItemSpirit__
#define __son__ItemSpirit__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/RotateAction.h"

enum ITEM_SPIRIT_ACT
{
    RELEASE = 0,
    ABSORB  = 1
};


class ItemSpirit : public cocos2d::Node
{
public:
    bool init();
    
    
    void initPos(float x, float y);
    
    void updatePos();
    
    void setOriginPos(float x, float y);
    
    bool hit();
    
    void setAct(int action);
    CREATE_FUNC(ItemSpirit);
private:
    float speed;
    float addAngle;
    
    cocos2d::ParticleSystemQuad*    _emitter;
    cocos2d::Sprite *sprite;
    cocostudio::Armature *armature;
    
    RotateAction *rAction;
    CC_SYNTHESIZE_READONLY(int, act, Act);
    CC_SYNTHESIZE_READONLY(int, rCount, Count);
};

#endif /* defined(__son__ItemSpirit__) */
