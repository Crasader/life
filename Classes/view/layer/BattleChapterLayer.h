//
//  BattleChapterLayer.h
//  life
//
//  Created by ff on 17/7/12.
//
//

#ifndef BattleChapterLayer_h
#define BattleChapterLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class BattleChapterLayer : public cocos2d::Layer {
public:
    BattleChapterLayer();
    ~BattleChapterLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void clickChapterName(cocos2d::Ref *gateNode);
    void clickEnter(cocos2d::Ref *gateNode);
    void clickClose();
    void removeOff();
    void endIn();
    
    void showChapterName(cocos2d::EventCustom *event);
    void startPlot(cocos2d::EventCustom *event);
    
    CREATE_FUNC(BattleChapterLayer);
private:
    int focusChapter;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *chapterAction[8];
    cocos2d::ui::ScrollView *mainScroll;
    cocos2d::EventListenerCustom *showChapterNameListener;
    cocos2d::EventListenerCustom *startPlotListener;
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);
};

#endif /* BattleChapterLayer_h */
