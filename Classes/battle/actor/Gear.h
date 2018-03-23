//
//  Gear.h
//  son
//
//  Created by ff on 15-4-22.
//
//

#ifndef __son__Gear__
#define __son__Gear__

#include "FightActor.h"

class Gear : public FightActor
{
public:
    bool init();
    
    virtual void setupData(int idx);
    virtual void initFightData(int idx);
    
    void addHurtListener(){};
    void removeHurtListener(){};
    
    void exitState(int currState);
    void enterState(int nextState);
    
    void updateState();
    void updateAI(){};
    
    void doFlag(int nextFlag){};
    
    void Hurt(int hurtVal){};
    
    virtual void dispatchResetPunchTag();

    virtual void setHp(float hp){};
    virtual void setMaxHp(float hp){};
    virtual void setMp(float val){};
    virtual void addAv(float val){};
    virtual void useAp(float val){};
    CREATE_FUNC(Gear);
    
private:
//    float waitDuration = 3;
    CC_SYNTHESIZE_READONLY(cocos2d::Size, viewSize, ViewSize);
};

#endif /* defined(__son__Gear__) */
