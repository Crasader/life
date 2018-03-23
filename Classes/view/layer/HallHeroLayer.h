//
//  HallHeroLayer.h
//  life
//
//  Created by ff on 17/7/24.
//
//

#ifndef HallHeroLayer_h
#define HallHeroLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallHeroLayer : public cocos2d::Layer {
public:
    HallHeroLayer();
    ~HallHeroLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void updateEquipDetail(cocos2d::EventCustom *event);
    void updateEquipLevel(cocos2d::EventCustom *event);
    void updateEquipStar(cocos2d::EventCustom *event);
    void updateTotalGold(cocos2d::EventCustom *event);
    void showItemWay(cocos2d::EventCustom *event);
    void showItemSweep(cocos2d::EventCustom *event);
    void updateItemCount(cocos2d::EventCustom *event);
    void updateExp(cocos2d::EventCustom *event);
    void updateTip(cocos2d::EventCustom *event);
    
    void clickClose();
    void removeOff();
    void endIn();
    
    void clickPosButton(cocos2d::Ref *pSender);
    void clickUpLevelButton();
    void clickUpGradeButton();
    void clickItemButton(cocos2d::Ref *pSender);
    
    void startPlot(cocos2d::EventCustom *event);
    
    CREATE_FUNC(HallHeroLayer);
private:
    cocos2d::EventListenerCustom *updateEquipDetailListener;
    cocos2d::EventListenerCustom *updateEquipLevelListener;
    cocos2d::EventListenerCustom *updateEquipStarListener;
    cocos2d::EventListenerCustom *updateGoldListener;
    cocos2d::EventListenerCustom *showItemWayListener;
    cocos2d::EventListenerCustom *updateItemCountListener;
    cocos2d::EventListenerCustom *showItemSweepListener;
    cocos2d::EventListenerCustom *updateExpListener;
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *updateTipListener;
    
    cocos2d::Node *root;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ImageView *leftBg;
    cocos2d::ui::ImageView *armatureBg;
    cocos2d::ui::TextAtlas *levelAtlas;
    cocos2d::ui::LoadingBar *expBar;
    cocos2d::ui::Text *forceText;
    cocos2d::ui::ImageView *valueBg;
    cocos2d::ui::Text *hpText;
    cocos2d::ui::Text *attackText;
    cocos2d::ui::Text *defenceText;
    cocos2d::ui::Text *critText;
    cocos2d::ui::Text *decritText;
    
    cocos2d::ui::ImageView *rightBg;
    cocos2d::ui::Text *totalGoldText;
    cocos2d::ui::ImageView *equipIcon;
    cocos2d::ui::Text *equipLevel;
    cocos2d::ui::Text *equipNameText;
    cocos2d::ui::Text *equipHpText;
    cocos2d::ui::Text *equipAttackText;
    cocos2d::ui::Text *equipDefenceText;
    cocos2d::ui::ImageView *uplevelText;
    cocos2d::ui::Text *plusHpText;
    cocos2d::ui::Text *plusAttackText;
    cocos2d::ui::Text *plusDefenceText;
    cocos2d::ui::Text *costGoldText;
    
    cocos2d::ui::ImageView *upGradeBg;
    cocos2d::ui::Button *uplevelButton;
    cocos2d::ui::Button *closeButton;
    cocos2d::ui::Button *upGradeButton;
    
    CC_SYNTHESIZE_READONLY(int, focusPos, FocusPos);
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* HallHeroLayer_h */
