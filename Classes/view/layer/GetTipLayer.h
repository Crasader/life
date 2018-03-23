//
//  GetTipLayer.h
//  life
//
//  Created by ff on 17/8/31.
//
//

#ifndef GetTipLayer_h
#define GetTipLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h" 

typedef struct __TIP_DATA
{
    std::string         icon[20];
    bool                frag[20];
    int                 count[20];
    std::string         name[20];
    int                 validCount;
    int                 totalCount;
}TIP_DATA;
class GetTipLayer : public cocos2d::Layer {
public:
    bool init();
    void onEnter();
    void onExit();
    void addTipPack(cocos2d::EventCustom *event);
    
    void startWaitTipPack();
    void endTipIn();
    void endOneTip();
    
    CREATE_FUNC(GetTipLayer);
private:
    std::vector<TIP_DATA> waitTips;
    TIP_DATA currPack;
    int top;
    int topIndex;
    int bottom;
    int bottomIndex;
    cocostudio::timeline::ActionTimeline *tipAction[20];
    cocos2d::EventListenerCustom *addTipListener;
    
};

#endif /* GetTipLayer_h */
