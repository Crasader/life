//
//  HallPetLayer.h
//  life
//
//  Created by ff on 17/7/31.
//
//

#ifndef HallPetLayer_h
#define HallPetLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallPetLayer : public cocos2d::Layer {
public:
    HallPetLayer();
    ~HallPetLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void updateFightPet(cocos2d::EventCustom *event);
    void updatePetLevel(cocos2d::EventCustom *event);
    void updatePetDetail(cocos2d::EventCustom *event);
    void updateTip(cocos2d::EventCustom *event);
    
    void clickClose();
    void removeOff();
    void clickFight(cocos2d::Ref *pSender);
    void clickRemoveFight(cocos2d::Ref *pSender);
    void clickUplevel();
    void clickPet(cocos2d::Ref *pSender);
    
    void startPlot(cocos2d::EventCustom *event);
    void endIn();
    CREATE_FUNC(HallPetLayer);
private:
    cocos2d::EventListenerCustom *updateFightPetListener;
    cocos2d::EventListenerCustom *updatePetLevelListener;
    cocos2d::EventListenerCustom *updatePetDetailListener;
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *updateTipListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *maskAction;
    cocos2d::ui::ImageView *petIcon;
    cocos2d::ui::ImageView *uplevelImg;
    cocos2d::ui::Text *skillNameText;
    cocos2d::ui::ImageView *maxFlagText;
    cocos2d::ui::LoadingBar *fragBar;
    cocos2d::ui::Text *fragCountText;
    cocos2d::ui::Text *plusHpText;
    cocos2d::ui::Text *plusAttackText;
    cocos2d::ui::Text *plusDefenceText;
    cocos2d::ui::Text *petHpText;
    cocos2d::ui::Text *petAttackText;
    cocos2d::ui::Text *petDefenceText;
    cocos2d::ui::ImageView *fighPetBg;
    cocos2d::ui::ListView *petList;
    cocos2d::ui::Button *uplevelButton;
    cocos2d::ui::Button *closeButton;
    cocos2d::Node* maskNode;
    
    cocos2d::ui::ImageView *petBg[10];
    
    int fightPetId[3];
    
    CC_SYNTHESIZE_READONLY(int, focusPos, FocusPos);
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);

};

#endif /* HallPetLayer_h */
