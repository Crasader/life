//
//  ArenaMatchOppLayer.h
//  life
//
//  Created by ff on 17/9/22.
//
//

#ifndef ArenaMatchOppLayer_h
#define ArenaMatchOppLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ArenaMatchOppLayer : public cocos2d::LayerColor {
public:
    ArenaMatchOppLayer();
    ~ArenaMatchOppLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    
    void matchRobot();
    void showConnectBar();
    void removeOff();
    void setupRobotView(cocos2d::EventCustom *event);
    
    
    CREATE_FUNC(ArenaMatchOppLayer);
private:
    cocostudio::timeline::ActionTimeline *rootAction;
    
    cocos2d::ui::ImageView *rightJobIcon;
    cocos2d::ui::LoadingBar *rightBar;
    cocos2d::ui::Text *rightForceText;
    cocos2d::ui::Text *rightNameText;
    cocos2d::ui::ImageView* rightPetImg[3];
    cocos2d::ui::Text* rightPetName[3];
    cocos2d::ui::ImageView *rightSwordImg;
    cocos2d::ui::Text *rightSwordName;
    
    cocos2d::ui::ImageView *leftJobIcon;
    cocos2d::ui::LoadingBar *leftBar;
    cocos2d::ui::Text *leftForceText;
    cocos2d::ui::Text *leftNameText;
    cocos2d::ui::ImageView* leftPetImg[3];
    cocos2d::ui::Text* leftPetName[3];
    cocos2d::ui::ImageView *leftSwordImg;
    cocos2d::ui::Text *leftSwordName;
    
    cocos2d::EventListenerCustom *showRobotListener;
    
    int connectPer1;
    int connectPer2;
    float connectDur1;
    float connectDur2;
    float connectCD1;
    float connectCD2;
};

#endif /* ArenaMatchOppLayer_h */
