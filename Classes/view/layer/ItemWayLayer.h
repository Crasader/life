//
//  ItemWayLayer.h
//  life
//
//  Created by ff on 17/7/25.
//
//

#ifndef ItemWayLayer_h
#define ItemWayLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ItemWayLayer : public cocos2d::Layer {
public:
    ItemWayLayer();
    ~ItemWayLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void endIn();
    
    void updateItemCount(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickReduce();
    void clickPlus();
    void clickBuy();
    void clickSweep();
    
    void startPlot(cocos2d::EventCustom *event);
    
    CREATE_FUNC(ItemWayLayer);
private:
    int requireCount;
    int totalDiamond;
    int count;
    int price;
    int itemTag;
    
    cocos2d::ui::Text *countText;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::Button *closeButton;
    cocostudio::timeline::ActionTimeline *rootAction;
    
    cocos2d::EventListenerCustom *updateItemCountListener;
    cocos2d::EventListenerCustom *startPlotListener;
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};
#endif /* ItemWayLayer_h */
