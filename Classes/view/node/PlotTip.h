//
//  PlotTip.h
//  life
//
//  Created by ff on 17/10/19.
//
//

#ifndef PlotTip_h
#define PlotTip_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class PlotTip : public cocos2d::Node {
public:
    bool init();
    
    void removeOff();
    
    void setupText(std::string val);
    
    CREATE_FUNC(PlotTip);
private:
    cocos2d::ui::Text *tipText;
};

#endif /* PlotTip_h */
