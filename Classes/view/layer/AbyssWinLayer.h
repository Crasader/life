//
//  AbyssWinLayer.h
//  life
//
//  Created by ff on 17/9/8.
//
//

#ifndef AbyssWinLayer_h
#define AbyssWinLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class AbyssWinLayer : public cocos2d::Layer {
public:
    AbyssWinLayer();
    ~AbyssWinLayer();
    
    bool init();
    void onEnter();
    
    void showLayer(cocos2d::EventCustom *event);
    
    void showStar();
    
    void getStar();
    
    void openTouch();
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    //    void back2Chapter();
    void startPlot(cocos2d::EventCustom *event);
    CREATE_FUNC(AbyssWinLayer);
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


#endif /* AbyssWinLayer_h */
