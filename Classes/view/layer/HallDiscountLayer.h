//
//  HallDiscountLayer.h
//  life
//
//  Created by ff on 18/1/15.
//
//

#ifndef HallDiscountLayer_h
#define HallDiscountLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallDiscountLayer : public cocos2d::Layer {
public:
    HallDiscountLayer();
    ~HallDiscountLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    void setupView(cocos2d::EventCustom *event);
    void updateState(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickOk();
    
    void endIn();
    
    void removeOff();
    
    CREATE_FUNC(HallDiscountLayer);
private:
    int packageId;
    float openSec;
    cocos2d::EventListenerCustom *updateStateListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ListView *itemList;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::Text *doublePriceText;
    cocos2d::ui::ImageView *titleImg;
    cocos2d::ui::Text *cdText;
    
    cocos2d::ui::Button *okButton;
};

#endif /* HallDiscountLayer_h */
