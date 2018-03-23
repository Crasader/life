//
//  CampaignLayer.h
//  life
//
//  Created by ff on 17/11/2.
//
//

#ifndef CampaignLayer_h
#define CampaignLayer_h


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CampaignLayer : public cocos2d::Layer {
public:
    CampaignLayer();
    ~CampaignLayer();
    
    bool init();
    void onEnter();
    void updateEveryday(cocos2d::EventCustom *event);
    void updateFirstRecharge(cocos2d::EventCustom *event);
    void startPlot(cocos2d::EventCustom *event);
    void hide(cocos2d::EventCustom *event);
    void showActive(cocos2d::EventCustom *event);
    void updateActive(cocos2d::EventCustom *event);
    void updateActiveBound(cocos2d::EventCustom *event);
    void updateActiveBox(cocos2d::EventCustom *event);
    void showBoundMission(cocos2d::EventCustom *event);
    void updateTip(cocos2d::EventCustom *event);
    void jumpLocked(cocos2d::EventCustom *event);
    
    void clickBoxClose();
    void clickClose();
    void clickEveryday();
    void clickRecharge();
    void clickMission();
    void clickBound();
    
    void clickTakeEveryday();
    void clickTakeRecharge();
    void clickActiveBox(cocos2d::Ref *pSender);
    void clickMissionJump(cocos2d::Ref *pSender);
    void clickMissionBound(cocos2d::Ref *pSender);
    void clickBoundMissionJump(cocos2d::Ref *pSender);
    void clickTakeBoundMissionReward(cocos2d::Ref *pSender);
    
    void removeOff();
    void endIn();
    void removeBoxOff();
    
    CREATE_FUNC(CampaignLayer);
private:
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *updateEverydayListener;
    cocos2d::EventListenerCustom *showEverydayListener;
    cocos2d::EventListenerCustom *updateFirstRechargeListener;
    cocos2d::EventListenerCustom *hideListener;
    cocos2d::EventListenerCustom *showActiveListener;
    cocos2d::EventListenerCustom *updateActiveBoundListener;
    cocos2d::EventListenerCustom *updateActiveBoxListener;
    cocos2d::EventListenerCustom *refreshActiveListener;
    cocos2d::EventListenerCustom *showBoundListener;
    cocos2d::EventListenerCustom *updateTipListener;
    cocos2d::EventListenerCustom *jumpLockedListener;

    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ImageView *everydayBg;
    cocos2d::ui::ImageView *missionBg;
    cocos2d::ui::ImageView *firstRechargeBg;
    cocos2d::ui::ImageView *boundMissionBg;
    cocos2d::ui::Button *boundButton;
    cocos2d::ui::Button *rechargeButton;
    cocos2d::ui::Button *missionButton;
    cocos2d::ui::Button *everydayButton;
    cocos2d::ui::ImageView *missionTip;
    cocos2d::ui::ImageView *boundTip;
    
    cocos2d::ui::Button *takeFirstRecharge;
    
    cocos2d::ui::LoadingBar *activeBar;
    cocos2d::ui::ListView *missionList;
    cocos2d::ui::Text *totalActiveText;
    cocos2d::ui::Button *boxButton[4];
    cocos2d::ParticleSystemQuad *boxParticle[4];
    cocostudio::timeline::ActionTimeline *boxAction;
    cocos2d::ui::ListView *boundMissionList;
    
    cocos2d::Layer *boxLayer;
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* CampaignLayer_h */
