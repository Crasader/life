//
//  GameWinLayer.h
//  life
//
//  Created by ff on 17/6/28.
//
//

#ifndef GameWinLayer_h
#define GameWinLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class GameWinLayer : public cocos2d::Layer {
public:
    GameWinLayer();
    ~GameWinLayer();
    
    bool init();
    void onEnter();
    
    void showLayer(cocos2d::EventCustom *event);
    
    void showStar();
    
    void getStar();
    
    std::string getStarText(int type, int thres);
    
    void openTouch();
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
//    void back2Chapter();
    void startPlot(cocos2d::EventCustom *event);
    CREATE_FUNC(GameWinLayer);
private:
    cocos2d::EventListenerCustom *startPlotListener;
    cocostudio::timeline::ActionTimeline *action;
    cocos2d::EventListenerCustom *starListener;
    cocos2d::ui::Text *boundGoldText;
    cocos2d::ui::Text *boundExpText;
    cocos2d::ui::Text *firstGoldText;
    cocos2d::ui::Text *firstExpText;
    cocos2d::ui::ImageView *firstPassBg;
    
    bool star[3];
    CC_SYNTHESIZE(int, leftSecond, LeftSecond);
    CC_SYNTHESIZE(int, heroHp, HeroHp);
    CC_SYNTHESIZE(int, heroMaxHp, HeroMaxHp)
    CC_SYNTHESIZE_READONLY(int, toHall, ToHall);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, star1Descript, Star1Descript);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, star2Descript, Star2Descript);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, star3Descript, Star3Descript);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, star1Img, Star1Img);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, star2Img, Star2Img);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, star3Img, Star3Img);
};

#endif /* GameWinLayer_h */
