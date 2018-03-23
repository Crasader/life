//
//  HallLuckyLayer.h
//  life
//
//  Created by ff on 17/11/6.
//
//

#ifndef HallLuckyLayer_h
#define HallLuckyLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallLuckyLayer : public cocos2d::Layer {
public:
    HallLuckyLayer();
    ~HallLuckyLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void updateNormalDraw(cocos2d::EventCustom *event);
    void updateSpecialDrawOne(cocos2d::EventCustom *event);
    void updateSpecialDrawTen(cocos2d::EventCustom *event);
    void updateFreeNormal(cocos2d::EventCustom *event);
    void updateFreeSpecial(cocos2d::EventCustom *event);
    void updateLuckyCD(cocos2d::EventCustom *event);
    void updateSpecialTicket(cocos2d::EventCustom *event);
    
    void showBuyLayer(int count);
    
    void clickClose();
    void clickNormalButton();
    void clickSpecialButton();
    void clickSpecialTenButton();
    void clickNormal();
    void clickSpecial();
    void clickAddSpecialTicket();
    void clickBuyClose();
    void clickBuyOk();
    void clickBuyPlus();
    void clickBuyReduce();
    void clickBounds1Close();
    void clickBounds2Close();
    
    void removeOff();
    void endIn();
    void endRewardIn1();
    void endRewardIn2();
    void removeBuyOff();
    void removeBounds1();
    void removeBounds2();
    bool touchReward1(cocos2d::Touch * ,cocos2d::Event *);
    bool touchReward2(cocos2d::Touch * ,cocos2d::Event *);
    void removeReward1();
    void removeReward2();
    
    void update(float dt);
    CREATE_FUNC(HallLuckyLayer);
private:
    cocos2d::EventListenerCustom *normalDrawListener;
    cocos2d::EventListenerCustom *specialDrawListener;
    cocos2d::EventListenerCustom *tenDrawListener;
    cocos2d::EventListenerCustom *freeNormalListener;
    cocos2d::EventListenerCustom *freeSpecialListener;
    cocos2d::EventListenerCustom *updateLuckyCDListener;
    cocos2d::EventListenerCustom *updateSpecialTicketListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::Text *normalCountText;
    cocos2d::ui::Text *specialCountText;
    cocos2d::ui::Text *specialTenCountText;
    cocos2d::ui::Text *normalCDText;
    cocos2d::ui::Text *specialCDText;
    cocos2d::ui::Button *normalButton;
    cocos2d::ui::Button *specialButton;
    cocos2d::ui::Button *specialTenButton;
    cocos2d::ui::ImageView *normalTip;
    cocos2d::ui::ImageView *specialTip;
    
    cocostudio::timeline::ActionTimeline *buyAction;
    cocos2d::ui::Text *countText;
    cocos2d::ui::Text *priceText;
    cocos2d::Layer *buyLayer;
    cocos2d::Layer *boundsLayer1;
    cocos2d::Layer *boundsLayer2;
    cocos2d::Layer *rewardLayer1;
    cocos2d::Layer *rewardLayer2;
    cocostudio::timeline::ActionTimeline *bounds1Action;
    cocostudio::timeline::ActionTimeline *bounds2Action;
    cocostudio::timeline::ActionTimeline *reward1Action;
    cocostudio::timeline::ActionTimeline *reward2Action;
    
    float freeNormalCD;
    float freeSpecialCD;
    int buyCount;
};

#endif /* HallLuckyLayer_h */
