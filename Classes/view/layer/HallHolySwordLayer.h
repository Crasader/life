//
//  HallHolySwordLayer.h
//  life
//
//  Created by ff on 17/8/7.
//
//

#ifndef HallHolySwordLayer_h
#define HallHolySwordLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallHolySwordLayer : public cocos2d::Layer {
public:
    HallHolySwordLayer();
    ~HallHolySwordLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void updateFight(cocos2d::EventCustom *event);
    void updateSword(cocos2d::EventCustom *event);
    void updateLevel(cocos2d::EventCustom *event);
    void updateGold(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickUplevel();
    void clickFight();
    void clickSword(cocos2d::Ref *pSender);
    
    void removeOff();
    void endIn();
    void startPlot(cocos2d::EventCustom *event);
    
    CREATE_FUNC(HallHolySwordLayer);
private:
    cocos2d::EventListenerCustom *updateFightListener;
    cocos2d::EventListenerCustom *updateSwordListener;
    cocos2d::EventListenerCustom *updateGoldListener;
    cocos2d::EventListenerCustom *updateLevelListener;
    cocos2d::EventListenerCustom *startPlotListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *stoneText;
    cocos2d::ui::Text *totalHpText;
    cocos2d::ui::Text *totalAttackText;
    cocos2d::ui::Text *totalDefenceText;
    cocos2d::ui::ImageView *nextLevelText;
    cocos2d::ui::Text *plusHpText;
    cocos2d::ui::Text *plusAttackText;
    cocos2d::ui::Text *plusDefenceText;
    cocos2d::ui::Text *costGoldText;
    cocos2d::ui::Text *costStoneText;
    cocos2d::Sprite *swordImg;
    cocos2d::ui::TextAtlas *levelAtlas;
    cocos2d::ui::Text *lockText;
    cocos2d::ui::Button* swordButton[5];
    cocos2d::Sprite *fightFlag[5];
    cocos2d::ui::ImageView *lockFlag[5];
    cocos2d::ui::Button *uplevelButton;
    cocos2d::ui::Button *closeButton;
    
    int currSwordId;
    int fightSwordId;
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* HallHolySwordLayer_h */
