//
//  BulletManager.h
//  life
//
//  Created by ff on 17/6/8.
//
//

#ifndef BulletManager_h
#define BulletManager_h

#include "cocos2d.h"
#include "utils/GameReader.h"

typedef struct __CREATE_BULLET_CMD
{
    int bulletConfigId;
    std::string res;
    float scale;
    float speed;
    int bulletType;
    int configCount;
    float configInterval;
    int bulletSkillId;
    bool copyReverse;
    bool through;
    int distDistance;
    int distRange;
    float bulletWidth;
    float bulletHeight;
    bool calAngel;
    float baseAttack;
    float baseCritRatio;
    
    int ownerType;
    int count;
    float interval;
    float gunX;
    float gunY;
    float ownerX;
    float ownerY;
    bool flipX;
    int comboFactor;
}CREATE_BULLET_CMD;

class BulletManager : public cocos2d::Node {
public:
    BulletManager();
    ~BulletManager();
    
    bool init();
    void onEnter();
    
    void update(float dt);
    
    void createBullets(cocos2d::EventCustom *event);
    
    void readFile(int idx, std::string filename, CREATE_BULLET_CMD& cmd);
    
    bool initData(int idx, rapidjson::Document& _doc, CREATE_BULLET_CMD &cmd);
    
    void fire(CREATE_BULLET_CMD& cmd);
    
    void loose(CREATE_BULLET_CMD& cmd, bool copyReverse);
    CREATE_FUNC(BulletManager);
public:
    static std::map<int, int> idTable;
private:
    std::vector<CREATE_BULLET_CMD> cmdQueue;
    cocos2d::EventListenerCustom *createGunListner;
};

#endif /* BulletManager_h */
