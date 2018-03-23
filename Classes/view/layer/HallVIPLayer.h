//
//  HallVIPLayer.h
//  life
//
//  Created by ff on 17/11/14.
//
//

#ifndef HallVIPLayer_h
#define HallVIPLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallVIPLayer : public cocos2d::Layer {
public:
    HallVIPLayer();
    ~HallVIPLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void updateDetail(cocos2d::EventCustom *event);
    void takeBound(cocos2d::EventCustom *event);
    void hide(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickJump();
    void clickLeft();
    void clickRight();
    void clickTakeBound();
    
    void removeOff();
    void endIn();
    
    CREATE_FUNC(HallVIPLayer);
private:
    cocos2d::EventListenerCustom *updateDetailListener;
    cocos2d::EventListenerCustom *takeBoundListener;
    cocos2d::EventListenerCustom *hideListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ImageView *currLevelImg;
    cocos2d::ui::LoadingBar *dayLoadBar;
    cocos2d::ui::Text *dayCountText;
    cocos2d::ui::Text *rechargeCountText;
    cocos2d::ui::Text *nextLevelText;
    cocos2d::ui::ImageView *icon;
    cocos2d::ui::Text *currLevelText;
    cocos2d::ui::Button *jumpButton;
    
    cocos2d::ui::ImageView *descriptIcon[10];
    cocos2d::ui::Text *descriptText[10];
    cocos2d::ui::ImageView *detailLevelImg;
    cocos2d::ui::Button *leftButton;
    cocos2d::ui::Button *rightButton;
    
    cocos2d::ui::ImageView *boundImg[6];
    cocos2d::ui::Text *boundCount[6];
    cocos2d::ui::Button *takeBoundButton;
    cocos2d::ui::ImageView *takeTip;
    
    int detailIndex;
    
};

#endif /* HallVIPLayer_h */
