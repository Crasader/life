//
//  EffectNode.h
//  life
//
//  Created by ff on 17/12/4.
//
//

#ifndef EffectNode_h
#define EffectNode_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class EffectNode : public cocos2d::Node {
public:
    bool init();
    
    void removeOff();
    
    bool initWithRes(std::string res);
    
    bool initUplevel(int level);
    
    CREATE_FUNC(EffectNode);
};

#endif /* EffectNode_h */
