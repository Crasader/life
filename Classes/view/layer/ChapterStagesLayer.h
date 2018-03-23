//
//  ChapterStagesLayer.h
//  life
//
//  Created by ff on 17/7/13.
//
//

#ifndef ChapterStagesLayer_h
#define ChapterStagesLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ChapterStagesLayer : public cocos2d::Layer {
public:
    ChapterStagesLayer();
    ~ChapterStagesLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickBox(cocos2d::Ref *pSender);
    void clickEnter(cocos2d::Ref *pSender);
    void clickCloseBounds();
    void removeOff();
    void removeOffBounds();
    void endIn();
    
    void openStarBox(cocos2d::EventCustom *event);
    void showStarBounds(cocos2d::EventCustom *event);
    void startPlot(cocos2d::EventCustom *event);
    
    CREATE_FUNC(ChapterStagesLayer);
private:
    cocos2d::Node *root;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ListView *mainScroll;
    cocos2d::ui::Button *boxButton[3];
    cocos2d::LayerColor *starBoundsLayer;
    cocostudio::timeline::ActionTimeline *starBoundsAction;
    
    cocos2d::EventListenerCustom *openStarBoxListener;
    cocos2d::EventListenerCustom *showStarBoundsListener;
    cocos2d::EventListenerCustom *startPlotListener;
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};
#endif /* ChapterStagesLayer_h */
