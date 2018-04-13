//
//  CustomPackageLayer.h
//  life-mobile
//
//  Created by ff on 2018/4/13.
//

#ifndef CustomPackageLayer_h
#define CustomPackageLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CustomPackageLayer : public cocos2d::Layer {
public:
    CustomPackageLayer();
    ~CustomPackageLayer();
    
    bool init();
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickOk();
    
    void removeOff();
    
    void endIn();
    CREATE_FUNC(CustomPackageLayer);
private:
    int packageId;
    cocostudio::timeline::ActionTimeline *rootAction;
};

#endif /* CustomPackageLayer_h */
