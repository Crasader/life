//
//  StageLayer.h
//  life
//
//  Created by ff on 17/7/13.
//
//

#ifndef StageLayer_h
#define StageLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class StageLayer : public cocos2d::Layer {
public:
    StageLayer();
    ~StageLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickStart();
    void removeOff();
    void endIn();
    
    void startStage(cocos2d::EventCustom *event);
    void clickAddVIT();
    void clickCloseBuy();
    void clickBuy();
    void removeBuy();
    void buyVIT(cocos2d::EventCustom *event);
    void showBuyVIT(cocos2d::EventCustom *event);
    void updateVIT(cocos2d::EventCustom *event);
    void updateVITBuyCount(cocos2d::EventCustom *event);
    void startPlot(cocos2d::EventCustom *event);
    void updateVITCD(cocos2d::EventCustom *event);

    
    std::string getStarText(int type, int thres);
    
    CREATE_FUNC(StageLayer);
private:
    float vitCD;

    cocos2d::Node *root;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::Text *currVITText;
    cocos2d::ui::Text *maxVITText;
    cocos2d::ui::Text *boundGoldText;
    cocos2d::ui::Text *boundExpText;
    cocos2d::ui::Text *firstGoldText;
    cocos2d::ui::Text *firstExpText;
    cocos2d::ui::ImageView *firstPassBg;
    cocos2d::LayerColor *buyLayer;
    cocostudio::timeline::ActionTimeline *buyAction;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::Text *buyCountText;
    cocos2d::ui::Button *startButton;
    cocos2d::ui::Button *selButton;
    cocos2d::ui::Text *VITCDTipText;
    cocos2d::ui::Text *VITCDText;

    
    cocos2d::EventListenerCustom *updateVITListener;
    cocos2d::EventListenerCustom *updateVITCDListener;
    cocos2d::EventListenerCustom *startStageListener;
    cocos2d::EventListenerCustom *showBuyVITListener;
    cocos2d::EventListenerCustom *buyVITListener;
    cocos2d::EventListenerCustom *updateVITBuyCountListener;
    cocos2d::EventListenerCustom *startPlotListener;
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* StageLayer_h */
