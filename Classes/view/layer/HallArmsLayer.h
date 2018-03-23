//
//  HallArmsLayer.h
//  life
//
//  Created by ff on 17/8/9.
//
//

#ifndef HallArmsLayer_h
#define HallArmsLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallArmsLayer : public cocos2d::Layer {
public:
    HallArmsLayer();
    ~HallArmsLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void updateArms(cocos2d::EventCustom *event);
    void updateLevel(cocos2d::EventCustom *event);
    void updateGemCount(cocos2d::EventCustom *event);
    void updatePosGem(cocos2d::EventCustom *event);
    void showFillBoard(cocos2d::EventCustom *event);
    void updateTip(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickUplevel();
    void clickArms(cocos2d::Ref *pSender);
    void clickAssemble(cocos2d::Ref *pSender);
    void clickRemove(cocos2d::Ref *pSender);
    void clickArmsGem(cocos2d::Ref *pSender);
    void clickGem(cocos2d::Ref *pSender);
    void clickFill();
    void clickFillBoardClose();
    
    void removeOff();
    void removeOffFillBoard();
    
    void startPlot(cocos2d::EventCustom *event);
    
    void endIn();
    void endFillIn();
    
    CREATE_FUNC(HallArmsLayer);
private:
    cocos2d::EventListenerCustom *updateArmsListener;
    cocos2d::EventListenerCustom *updateLevelListener;
    cocos2d::EventListenerCustom *updateGemCountListener;
    cocos2d::EventListenerCustom *updatePosGemListener;
    cocos2d::EventListenerCustom *showFillBoardListener;
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *updateTipListener;

    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *fillAction;
    cocos2d::Layer *fillGemLayer;
    cocos2d::ui::Button *armsButton[5];
    cocos2d::ui::Text *totalHpText;
    cocos2d::ui::Text *totalAttackText;
    cocos2d::ui::Text *totalDefenceText;
    cocos2d::ui::Text *totalCritText;
    cocos2d::ui::Text *totalDecritText;
    
    cocos2d::ui::ImageView *armsTypeImg;
    cocos2d::ui::Text *armsValueText;
    cocos2d::ui::ImageView *nextLevelText;
    cocos2d::ui::Text *plusValueText;
    
    cocos2d::ui::ImageView *gemsBg[6];
    
    cocos2d::ui::Button *posButton[6];
    cocos2d::ui::Text *lockPosText[6];
    cocos2d::Sprite *armsIcon;
    cocos2d::ui::Button *uplevelButton;
    cocos2d::ui::Text *fameCountText;
    cocos2d::ui::TextAtlas *levelAtlas;
    
    cocos2d::ui::Button *closeButton;
    cocos2d::ui::Button *fillButton;
    int currArmsId;
    int currGemTag;

    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
    CC_SYNTHESIZE_READONLY(int, plotFillButtonId, PlotFillButtonId);
    CC_SYNTHESIZE_READONLY(int, plotFillMissionId, PlotFillMissionId);

};

#endif /* HallArmsLayer_h */
