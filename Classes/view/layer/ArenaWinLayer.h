//
//  ArenaWinLayer.h
//  life
//
//  Created by ff on 17/9/26.
//
//

#ifndef ArenaWinLayer_h
#define ArenaWinLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ArenaWinLayer : public cocos2d::Layer {
public:
    ArenaWinLayer();
    ~ArenaWinLayer();
    
    bool init();
    void onEnter();
    
    void setupView(cocos2d::EventCustom *event);
    void openTouch();
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void startPlot(cocos2d::EventCustom *event);
    CREATE_FUNC(ArenaWinLayer);
private:
    cocos2d::EventListenerCustom *startPlotListener;
    cocostudio::timeline::ActionTimeline *action;
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *scoreText;
    cocos2d::ui::Text *deltaScoreText;
    cocos2d::ui::Text *divisionText;
    cocos2d::ui::ImageView *divisionIcon1;
    cocos2d::ui::ImageView *divisionIcon2;
    
    CC_SYNTHESIZE_READONLY(int, toHall, ToHall);
};

#endif /* ArenaWinLayer_h */
