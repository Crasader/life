//
//  VIT.h
//  life
//
//  Created by ff on 17/7/27.
//
//

#ifndef VIT_h
#define VIT_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

class VIT : public Singleton<VIT> {
public:
    VIT();
    ~VIT();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    bool restoreVIT(int val);
    void addPlusVIT(int val);
    bool useVIT(int val);
    void addMaxBaseVIT(int val);
    int buyVIT();
    void addMaxBuyCount(int val);
    void resetBuyCount();
    
    int getTotalVIT();
    
private:
    std::string coreStr;
    
    int plusVIT;
    CC_SYNTHESIZE_READONLY(int, baseVIT, BaseVIT);
    CC_SYNTHESIZE_READONLY(int, maxBaseVIT, MaxBaseVIT);
    CC_SYNTHESIZE_READONLY(int, buyCount, BuyCount);
    CC_SYNTHESIZE_READONLY(int, maxBuyCount, maxBuyCount);
};

#endif /* VIT_h */
