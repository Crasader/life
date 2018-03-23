//
//  ReviveLayer.h
//  life
//
//  Created by ff on 17/12/15.
//
//

#ifndef ReviveLayer_h
#define ReviveLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ReviveLayer : public cocos2d::Layer {
public:
    ReviveLayer();
    ~ReviveLayer();
    
    bool init();
    void endIn();
    
    void clickClose();
    void clickOk();
    
    void removeOff();
    
    CREATE_FUNC(ReviveLayer);
private:
    cocostudio::timeline::ActionTimeline *action;
    
};

#endif /* ReviveLayer_h */
