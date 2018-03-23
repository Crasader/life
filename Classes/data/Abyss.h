//
//  Abyss.h
//  life
//
//  Created by ff on 17/9/7.
//
//

#ifndef Abyss_h
#define Abyss_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __ABYSS_CONFIG
{
    int abyssId;
    std::string animationFile;
    int petFragId;
    int petFragCount;
    int stageId;
}ABYSS_CONFIG;

typedef struct __ABYSS_DATA
{
    int openStageCount;
    int maxResetCount;
    int resetCount[10];
    int fightCount[10];
}ABYSS_DATA;

class Abyss : public Singleton<Abyss> {
public:
    Abyss();
    ~Abyss();
    
    void readAbyssConfigFile();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    int refreshAbyss();
    int resetAbyss(int abyssId);
    void addMaxResetCount(int val);
    int checkStart(int abyssId);
    int start(int abyssId);
    void win(int abyssId);
public:
    static std::map<int, int> abyssConfigIdTable;
    std::map<int, ABYSS_CONFIG> abyssConfigMap;
private:
    CC_SYNTHESIZE_READONLY(ABYSS_DATA, abyssData, AbyssData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
};
#endif /* Abyss_h */
