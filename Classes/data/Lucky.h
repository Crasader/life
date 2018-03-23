//
//  Lucky.h
//  life
//
//  Created by ff on 17/11/6.
//
//

#ifndef Lucky_h
#define Lucky_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __LUCKY_CONFIG
{
    int luckyId;
    int luckyType;
    int boundType;
    int boundId;
    int boundCount;
    int rate;
}LUCKY_CONFIG;

class Lucky : public Singleton<Lucky> {
public:
    Lucky();
    ~Lucky();
    
    void readLuckyConfig();
    
    int drawNormalForFree();
    int drawSpecialForFree();
    int drawNormal();
    int drawSpecial();
    int drawSpecialTen(int cardId[10], int count[10]);
    
    void resetNormalThres();
    void resetSpecialThres();
    
    void enableNormalDraw();
    void enableSpecialDraw();
    
    void addNormalTicket(int val);
    void addSpecialTicket(int val);
    
    void deleteJson();
    void save2Json();
    void save2EmptyJson();
    bool parseJson();
    bool readJson();
    
    void deleteThresJson();
    void save2ThresJson();
    void save2EmptyNormal();
    void save2EmptySpecial();
    void readThres();
    
    void checkTip();
public:
    static std::map<int, int> configIdTable;
    static std::map<int, int> normalConfigIdTable;
    static std::map<int, int> specialConfigIdTable;
    
    std::map<int, LUCKY_CONFIG> normalMap;
    std::map<int, LUCKY_CONFIG> specialMap;
private:
    std::map<int, int> normalThresMap;
    std::map<int, int> specialThresMap;
    
    CC_SYNTHESIZE_READONLY(int, normalState, NormalState);
    CC_SYNTHESIZE_READONLY(int, specialState, SpecialState);
    CC_SYNTHESIZE_READONLY(int, normalTotalThres, NormalTotalThres);
    CC_SYNTHESIZE_READONLY(int, specialTotalThres, SpecialTotalThres);
    CC_SYNTHESIZE_READONLY(int, normalTicket, NormalTicket);
    CC_SYNTHESIZE_READONLY(int, specialTicket, SpecialTicket);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, normalStr, NormalStr);
    CC_SYNTHESIZE_READONLY(std::string, specialStr, SpecialStr);
    
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
};
#endif /* Lucky_h */
