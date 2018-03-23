//
//  ArenaFailLayer.h
//  life
//
//  Created by ff on 17/9/26.
//
//

#ifndef ArenaFailLayer_h
#define ArenaFailLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ArenaFailLayer : public cocos2d::Layer {
public:
    ArenaFailLayer();
    ~ArenaFailLayer();
    
    bool init();
    void onEnter();
    
    void setupView(cocos2d::EventCustom *event);
    void openTouch();
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    CREATE_FUNC(ArenaFailLayer);
private:
    cocostudio::timeline::ActionTimeline *action;
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *scoreText;
    cocos2d::ui::Text *deltaScoreText;
    cocos2d::ui::Text *divisionText;
    cocos2d::ui::ImageView *divisionIcon1;
    cocos2d::ui::ImageView *divisionIcon2;

};

#endif /* ArenaFailLayer_h */
