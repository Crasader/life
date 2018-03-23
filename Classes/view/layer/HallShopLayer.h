//
//  HallShopLayer.h
//  life
//
//  Created by ff on 17/11/8.
//
//

#ifndef HallShopLayer_h
#define HallShopLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallShopLayer : public cocos2d::Layer {
public:
    HallShopLayer();
    ~HallShopLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void updateShop(cocos2d::EventCustom *event);
    void updateBuyResult(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickShopButton(cocos2d::Ref *pSender);
    void clickPackageOk(cocos2d::Ref *pSender);
    void clickPackageDetail(cocos2d::Ref *pSender);
    
    void removeOff();
    void endIn();
    
    CREATE_FUNC(HallShopLayer);
private:
    cocos2d::EventListenerCustom *updateResultListener;
    cocos2d::EventListenerCustom *updateShopListener;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ListView *itemList;
    cocos2d::ui::ImageView *buttonBg;
    cocos2d::ui::Button *typeButton[3];
    
    int shopType;
};

#endif /* HallShopLayer_h */
