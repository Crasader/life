//
//  HallCastleLayer.h
//  life
//
//  Created by ff on 17/11/1.
//
//

#ifndef HallCastleLayer_h
#define HallCastleLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallCastleLayer : public cocos2d::Layer {
public:
    HallCastleLayer();
    ~HallCastleLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void changeName(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickJob(cocos2d::Ref *pSender);
    void clickConfirm();
    void clickBuyJob();
    void clickCloseBuyJob();
    void clickChangeName();
    
    void removeOff();
    void removeBuyJob();
    void endIn();
    
    void startPlot(cocos2d::EventCustom *event);
    void showJob(cocos2d::EventCustom *event);
    void activeJob(cocos2d::EventCustom *event);
    
    void activeAllJob(cocos2d::EventCustom *event);
    
    CREATE_FUNC(HallCastleLayer);
private:
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *showJobListener;
    cocos2d::EventListenerCustom *activeListener;
    cocos2d::EventListenerCustom *changeNameListener;
    cocos2d::EventListenerCustom *activeAllListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ImageView *jobImg1;
    cocos2d::ui::ImageView *jobImg2;
    cocos2d::ui::Text *jobDescript;
    cocos2d::ui::Button *jobButton[4];
    cocos2d::ui::ImageView *jobLock[4];
    cocos2d::ui::TextField *nameTextField;
    cocos2d::ui::Button *nameButton;
    
    cocostudio::timeline::ActionTimeline *buyJobAction;
    cocos2d::Node *buyJobLayer;
    int showJobId;
    int buyJobId;
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* HallCastleLayer_h */
