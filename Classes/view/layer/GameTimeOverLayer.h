//
//  GameTimeOverLayer.h
//  life
//
//  Created by ff on 17/7/3.
//
//

#ifndef GameTimeOverLayer_h
#define GameTimeOverLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class GameTimeOverLayer : public cocos2d::Layer {
public:
    GameTimeOverLayer();
    ~GameTimeOverLayer();
    
    bool init();
    
    void showLayer();
    
    CREATE_FUNC(GameTimeOverLayer);
private:
    cocostudio::timeline::ActionTimeline *action;

    CC_SYNTHESIZE(int, heroHp, HeroHp);
    CC_SYNTHESIZE(int, maxHeroHp, MaxHeroHp);
    CC_SYNTHESIZE(int, opponentHp, OpponentHp);
    CC_SYNTHESIZE(int, maxOpponentHp, MaxOpponentHp);
};

#endif /* GameTimeOverLayer_h */
