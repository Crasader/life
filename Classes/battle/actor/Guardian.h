//
//  Guardian.h
//  son
//
//  Created by ff on 15-5-18.
//
//

#ifndef __son__Guardian__
#define __son__Guardian__

#include "FightActor.h"
#include "ItemSpirit.h"

class Guardian : public FightActor
{
public:
    bool init();
    
    
    virtual void setupData(int idx);
    virtual void initFightData(int idx);
    
    void exitState(int currState);
    void enterState(int nextState);
    
    void updateState();
    void updateAI();
    void doFlag(int nextFlag);
    
    bool lostHero();
    bool farFromHero();
    bool pressSkill();
    virtual void setGhost(float val);
    
    void updateGhostPoint();
    void add2GhostList(ItemSpirit *ghost);
    CREATE_FUNC(Guardian);
private:
    std::vector<ItemSpirit *> ghostList;

    CC_SYNTHESIZE(Actor *, man, Man);
    CC_SYNTHESIZE(int, currKey, CurrKey);
};

#endif /* defined(__son__Guardian__) */
