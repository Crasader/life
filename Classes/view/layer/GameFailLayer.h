//
//  GameFailLayer.h
//  life
//
//  Created by ff on 17/7/3.
//
//

#ifndef GameFailLayer_h
#define GameFailLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class GameFailLayer : public cocos2d::Layer {
public:
    GameFailLayer();
    ~GameFailLayer();
    
    bool init();
    void onEnter();
    
    void showLayer();
    
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    CREATE_FUNC(GameFailLayer);
private:
    cocostudio::timeline::ActionTimeline *action;
};

#endif /* GameFailLayer_h */
