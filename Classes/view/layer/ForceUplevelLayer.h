//
//  ForceUplevelLayer.h
//  life
//
//  Created by ff on 17/12/4.
//
//

#ifndef ForceUplevelLayer_h
#define ForceUplevelLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ForceUplevelLayer : public cocos2d::Layer {
public:
    bool init();
    
    void showLayer(cocos2d::EventCustom *event);
    CREATE_FUNC(ForceUplevelLayer);
private:
    cocos2d::ui::Text *totalForceText;
    cocos2d::ui::Text *addForceText;
    cocostudio::timeline::ActionTimeline *rootAction;
};

#endif /* ForceUplevelLayer_h */
