//
//  HallAbyssLayer.h
//  life
//
//  Created by ff on 17/9/8.
//
//

#ifndef HallAbyssLayer_h
#define HallAbyssLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallAbyssLayer : public cocos2d::Layer {
public:
    HallAbyssLayer();
    ~HallAbyssLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    void setupView(cocos2d::EventCustom *event);
    void readyStart(cocos2d::EventCustom *event);
    void reset(cocos2d::EventCustom *event);
    void showBuyVIT(cocos2d::EventCustom *event);
    void buyVIT(cocos2d::EventCustom *event);
    void updateVITBuyCount(cocos2d::EventCustom *event);
    void updateVIT(cocos2d::EventCustom *event);
    void showAbyssReset(cocos2d::EventCustom *event);
    void updateVITCD(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickStart(cocos2d::Ref *pSender);
    void clickReset(cocos2d::Ref *pSender);
    void clickAddVIT();
    void clickCloseBuyVIT();
    void clickBuyVIT();
    void clickCloseBuyAbyss();
    void clickBuyAbyss();
    
    
    void endStart();
    void removeOff();
    void removeBuy();
    void removeReset();
    void endIn();
    
    CREATE_FUNC(HallAbyssLayer);
private:
    cocos2d::ui::Text *currVITText;
    cocos2d::ui::Text *maxVITText;
    cocos2d::ui::Text *buyCountText;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::Text *VITCDTipText;
    cocos2d::ui::Text *VITCDText;

    
    cocos2d::ui::ListView *stageList;
    cocos2d::LayerColor *buyLayer;
    cocos2d::LayerColor *resetLayer;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *buyAction;
    cocostudio::timeline::ActionTimeline *buyResetAction;
    
    cocos2d::EventListenerCustom *readyStartListener;
    cocos2d::EventListenerCustom *resetListener;
    cocos2d::EventListenerCustom *showBuyVITListener;
    cocos2d::EventListenerCustom *buyVITListener;
    cocos2d::EventListenerCustom *updateVITBuyCountListener;
    cocos2d::EventListenerCustom *updateVITListener;
    cocos2d::EventListenerCustom *updateVITCDListener;
    cocos2d::EventListenerCustom *showAbyssResetListener;

    
    bool start;
    int resetAbyssId;
    float vitCD;
};

#endif /* HallAbyssLayer_h */
