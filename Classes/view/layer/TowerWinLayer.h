//
//  TowerWinLayer.h
//  life
//
//  Created by ff on 17/12/4.
//
//

#ifndef TowerWinLayer_h
#define TowerWinLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class TowerWinLayer : public cocos2d::Layer {
public:
    TowerWinLayer();
    ~TowerWinLayer();
    
    bool init();
    void onEnter();
    
    void showLayer(cocos2d::EventCustom *event);
    
    void showStar();
    
    void getStar();
    
    void openTouch();
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void startPlot(cocos2d::EventCustom *event);
    CREATE_FUNC(TowerWinLayer);
private:
    cocos2d::EventListenerCustom *startPlotListener;
    cocostudio::timeline::ActionTimeline *action;
    cocos2d::EventListenerCustom *starListener;
    cocos2d::ui::Text *boundGoldText;
    cocos2d::ui::Text *boundExpText;
    cocos2d::ui::Text *firstGoldText;
    cocos2d::ui::Text *firstExpText;
    cocos2d::ui::ImageView *firstPassBg;
    cocos2d::ui::ImageView *itemIcon;
    cocos2d::ui::Text *itemCountText;
    
    CC_SYNTHESIZE_READONLY(int, toHall, ToHall);
};

#endif /* TowerWinLayer_h */
