//
//  ItemSweepLayer.h
//  life
//
//  Created by ff on 17/7/26.
//
//

#ifndef ItemSweepLayer_h
#define ItemSweepLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ItemSweepLayer : public cocos2d::Layer {
public:
    ItemSweepLayer();
    ~ItemSweepLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    void setupView(cocos2d::EventCustom *event);
    void updateVIT(cocos2d::EventCustom *event);
    void sweepItem(cocos2d::EventCustom *event);
    void showBuyVIT(cocos2d::EventCustom *event);
    void updateVITBuyCount(cocos2d::EventCustom *event);
    void buyVIT(cocos2d::EventCustom *event);
    void updateVITCD(cocos2d::EventCustom *event);
    
    void pushResult2List();
    
    void clickClose();
    void clickSweep();
    void clickMulti();
    void clickAddVIT();
    void clickCloseBuy();
    void clickBuy();
    
    void removeOff();
    void removeBuy();
    void endIn();
    CREATE_FUNC(ItemSweepLayer);
private:
    int itemTag;
    int requireCount;
    int sweepTotalCount;
    int showCount;
    int dropGold;
    int dropExp;
    int dropItemCount;
    std::string itemIcon;
    bool multi;
    bool showing;
    bool valid[10];
    float vitCD;
    
    cocos2d::ui::Text *goldText;
    cocos2d::ui::Text *expText;
    cocos2d::ui::ImageView *itemImg;
    cocos2d::ui::Text *itemCountText;
    cocos2d::ui::ImageView *icon;
    cocos2d::ui::Text *countText;
    cocos2d::ui::Text *otherText;
    cocos2d::ui::Text *sweepCountText;
    cocos2d::ui::CheckBox *multiCheck;
    cocos2d::ui::ListView *sweepList;
    cocos2d::ui::Button *sweepButton;
    
    cocos2d::ui::Text *VITText;
    cocos2d::ui::Button *addVITButton;
    cocos2d::LayerColor *buyLayer;
    cocos2d::ui::Text *vitBuyCountText;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::Text *VITCDTipText;
    cocos2d::ui::Text *VITCDText;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *buyAction;
    
    cocos2d::EventListenerCustom *updateVITListener;
    cocos2d::EventListenerCustom *updateVITCDListener;
    cocos2d::EventListenerCustom *sweepItemListener;
    cocos2d::EventListenerCustom *showBuyVITListener;
    cocos2d::EventListenerCustom *updateVITBuyCountListener;
    cocos2d::EventListenerCustom *buyVITListener;
};

#endif /* ItemSweepLayer_h */
