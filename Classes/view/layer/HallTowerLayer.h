//
//  HallTowerLayer.h
//  life
//
//  Created by ff on 17/9/12.
//
//

#ifndef HallTowerLayer_h
#define HallTowerLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallTowerLayer : public cocos2d::Layer {
public:
    HallTowerLayer();
    ~HallTowerLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    
    void setupView(cocos2d::EventCustom *event);
    void showSweep(cocos2d::EventCustom *event);
    void resetTower(cocos2d::EventCustom *event);
    void startSweep(cocos2d::EventCustom *event);
    void compelteSweep(cocos2d::EventCustom *event);
    void takeReward(cocos2d::EventCustom *event);
    void speedupSweep(cocos2d::EventCustom *event);
    void updateSweepCD(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickStart();
    void clickReset();
    void clickSweep();
    void clickCloseSweep();
    void clickStartSweep();
    void clickSpeedUpSweep();
    void clickTakeSweepReward();
    
    void removeOff();
    void removeSweep();
    
    void endIn();
    void endSweepIn();
    
    CREATE_FUNC(HallTowerLayer);
private:
    cocos2d::ui::ScrollView *towerScroll;
    cocos2d::ui::Text *titleText;
    cocos2d::ui::Text *nameText;
    cocostudio::Armature *bossAramture;
    cocos2d::ui::Text *descriptText;
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *expText;
    cocos2d::ui::Text *gemText;
    cocos2d::ui::ImageView *gemIcon;
    cocos2d::ui::ImageView *rightBg;
    std::map<int, cocos2d::ui::ImageView*> stageBgMap;
    
    cocos2d::LayerColor *sweepLayer;
//    cocos2d::ui::Button *closeButton;
    cocos2d::ui::Button *startButton;
    cocos2d::ui::ImageView *costImg;
    cocos2d::ui::Text *costText;
    cocos2d::ui::ImageView *sweepCDImg;
    cocos2d::ui::Text *sweepCDText;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *sweepAction;
    
    cocos2d::EventListenerCustom *showSweepListener;
    cocos2d::EventListenerCustom *resetTowerListener;
    cocos2d::EventListenerCustom *startSweepListener;
    cocos2d::EventListenerCustom *completeSweepListener;
    cocos2d::EventListenerCustom *takeRewardListener;
    cocos2d::EventListenerCustom *speedupSweepListener;
    cocos2d::EventListenerCustom *updateSweepCDListener;
    
    int sweepState;
    float sweepSec;
    
};

#endif /* HallTowerLayer_h */
