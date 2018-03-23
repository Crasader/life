#pragma once

#include "cocos2d.h"
typedef struct __LOCK_AREA_DATA
{
    int lockId;
    float x;
    float left;
    float right;
}LOCK_AREA_DATA;

class YNCamera : public cocos2d::Node
{
public:

    static YNCamera *getInstance();
    
    bool init();
    
	CREATE_FUNC(YNCamera);

    void focusPos(cocos2d::Vec2 pos);
	void setCameraCenter(cocos2d::Point pos, float moveSpeed = 0, bool follow = true);
    
    float vShake();
    
    void addLock(int, LOCK_AREA_DATA);
    
    void startLock(int lockIndex);
    void unlock();
    
    static bool bShake;
    static int shakeDuration;
    static float shakeRange;
    static int flashCount;
    
//    void scheduleShake();
    
    void stopShake();
    
    bool isOutWindow(cocos2d::Point p);
    
private:
    static YNCamera *inc;
    std::map<int, LOCK_AREA_DATA> viewLocks;
    
    CC_SYNTHESIZE_READONLY(bool, locking, Locking);
    CC_SYNTHESIZE(int, lockIndex, LockIndex);
    CC_SYNTHESIZE_READONLY(int, lockCount, LockCount);
    CC_SYNTHESIZE(float, mapAreaRight, MapAreaRight);
    CC_SYNTHESIZE(float, mapAreaLeft, MapAreaLeft);
    CC_SYNTHESIZE(float, mapAreaWidth, MapAreaWidth);
};
