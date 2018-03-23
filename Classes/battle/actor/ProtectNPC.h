//
//  ProtectNPC.h
//  life
//
//  Created by ff on 17/6/30.
//
//

#ifndef ProtectNPC_h
#define ProtectNPC_h

#include "cocos2d.h"
#include "FightActor.h"
#include "cocostudio/CocoStudio.h"

class ProtectNPC : public FightActor {
public:
    ProtectNPC();
    ~ProtectNPC();
    
    bool init();
    void onEnter();
    
    virtual void setupData(int idx);
    virtual void initFightData(int idx);
    
    void addHurtListener();
    void removeHurtListener();
    
    void exitState(int currState);
    void enterState(int nextState);
    
    void updateState();
    void updateAI();
    
    void doFlag(int nextFlag);
    
    virtual void dispatchResetPunchTag();
    
    void hasFadeIn();
    
    virtual void setHp(float hp);
    virtual void setMaxHp(float val);
    
    void pauseFight();
    void resumeFight();

    CREATE_FUNC(ProtectNPC);
private:
    cocos2d::EventListenerCustom *pauseListener;
    cocos2d::EventListenerCustom *resumeListener;
};

#endif /* ProtectNPC_h */
