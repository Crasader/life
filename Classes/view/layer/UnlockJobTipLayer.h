//
//  UnlockJobTipLayer.h
//  life-mobile
//
//  Created by ff on 2018/5/31.
//

#ifndef UnlockJobTipLayer_h
#define UnlockJobTipLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h" 

class UnlockJobTipLayer : public cocos2d::LayerColor {
public:
    bool init();

    void removeOff();
    
    CREATE_FUNC(UnlockJobTipLayer);
};

#endif /* UnlockJobTipLayer_h */
