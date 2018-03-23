//
//  TipsNode.h
//  life
//
//  Created by ff on 17/7/25.
//
//

#ifndef TipsNode_h
#define TipsNode_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class TipsNode : public cocos2d::Node {
public:
    bool init();
    
    void removeOff();
    
    void setupText(std::string val);
    
    CREATE_FUNC(TipsNode);
private:
    cocos2d::ui::Text *tipText;
};
#endif /* TipsNode_h */
