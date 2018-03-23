#include "YNCamera.h"


USING_NS_CC;

bool YNCamera::bShake = false;
int YNCamera::shakeDuration = 0;
float YNCamera::shakeRange = 0;
int YNCamera::flashCount = 0;
YNCamera *YNCamera::inc;

bool YNCamera::init()
{
    if (!Node::init()) {
        return false;
    }
    inc = this;
    
    lockIndex = 0;
    lockCount = 0;
    locking = false;
    
    return true;
}

YNCamera *YNCamera::getInstance()
{
    return inc;
}
void YNCamera::focusPos(cocos2d::Vec2 pos)
{
    auto winSize = Director::getInstance()->getWinSize();
    float posX = pos.x;
    posX = MIN(mapAreaRight-winSize.width/2, posX);
    posX = MAX(mapAreaLeft+winSize.width/2, posX);
    float posY = pos.y-300;
    posY = MIN(740-winSize.height, posY);
    posY = MAX(0, posY);
    setPosition(Point(posX, posY));
}
void YNCamera::setCameraCenter(cocos2d::Point pos, float moveSpeed, bool follow)
{
    auto winSize = Director::getInstance()->getWinSize();
    float currX = getPositionX();
    float currY = getPositionY();
    float posY = pos.y-300;
	float posX = pos.x;
    float distanceX = abs(currX- posX);
    float distanceY = abs(currY- posY);
    if (!follow) {// 如果不跟随而是指定速度移动镜头
        if (distanceY <= moveSpeed) {
            currY = posY;
        }else{
            currY += (currY>posY?(-moveSpeed):moveSpeed);
        }
        if (distanceX <= moveSpeed) {
            currX = posX;
        }else{
            currX += (currX>posX?(-moveSpeed):moveSpeed);
        }
    }else{// 一般的跟随镜头
        if (distanceX <= 3) {
            currX = posX;
        }else{
            //按距离分级速度
            float speed = 3;
            if (distanceX >= 300) {
                speed = 16;
            }else if (distanceX >= 200){
                speed = 8;
//            }else {
//                speed = 3;
            }
            currX += (currX>posX?(-speed):(speed));
        }
        if (distanceX <= 3) {
            currY = posY;
        }else{
            //按距离分级速度
            float speed = 3;
            if (distanceY >= 30) {
                speed = 10;
            }else if (distanceX >= 20){
                speed = 5;
                //            }else {
                //                speed = 3;
            }
            currY += (currY>posY?(-speed):(speed));
        }
        
    }
    
    if (currX < posX) {// 向右移动时，只检查右方，避免因判断左方引发跳动
        currX = MIN(mapAreaRight-winSize.width/2, currX);
    }else if (currX > posX) {//同上
        currX = MAX(mapAreaLeft+winSize.width/2, currX);
    }
    posY = MIN(740-winSize.height, posY);
    posY = MAX(0, posY);

	
    //未锁屏时判断有锁屏数据，且下一锁屏ID在锁屏总数下
    if (!locking && lockCount >0 && lockIndex < lockCount) {
        //如镜头跟随的点位置在下一锁屏点右，启动锁屏
        if (posX > viewLocks[lockIndex].x) {
            startLock(lockIndex);
        }
    }
    
//    float posY = pos.y-300;
//    posY = MIN(740-winSize.height/2, posY);
//    posY = MAX(0, posY);

	setPosition(Point(currX, posY));
}

float YNCamera::vShake()
{
    if (shakeDuration >= 6) {
        stopShake();
        return 0;
    }
    shakeDuration++;
    if(shakeDuration%2 == 1)
    {
        return shakeRange;
    } else {
        shakeRange /= 2;
        return 0;
    }
}

void YNCamera::stopShake()
{
    bShake = false;
    shakeDuration = -1;
}

bool YNCamera::isOutWindow(Point p)
{
    if(p.x == mapAreaRight){
        log("catch it");
        return true;
    }
    if (p.x == mapAreaLeft) {
        return true;
    }
    auto winSize = Director::getInstance()->getWinSize();
    if (getPositionX() <= mapAreaLeft + winSize.width/2) {
        if (p.x<getPositionX()) {
            return p.x <= mapAreaLeft;
        } else {
            return p.x > winSize.width;
        }
    } else if (getPositionX() >= mapAreaRight - winSize.width/2) {
        if (p.x > getPositionX()) {
            return p.x >= mapAreaRight;
        } else {
            return p.x < mapAreaRight - winSize.width;
        }
    } else {
        return abs(p.x-getPositionX()) > winSize.width/2;
    }
}

//void YNCamera::scheduleShake()
//{
//    if (shakeDuration > 0 || !bShake) {
//        return;
//    }
//}
/**
 *  加入锁屏数据
 *
 **/
void YNCamera::addLock(int index, LOCK_AREA_DATA data)
{
    viewLocks[index] = data;
    lockCount++;
}

/**
 *  解锁屏
 *
 **/
void YNCamera::unlock()
{
    if (!locking) {
        return;
    }
    mapAreaLeft = 0;
    mapAreaRight = mapAreaWidth;
    locking = false;

}

/**
 *  激活锁屏
 *  @param int lockId 激活的锁屏区ID
 *
 **/
void YNCamera::startLock(int lockId)
{
//    if (lockId < lockIndex) {
    lockIndex++;
    mapAreaLeft = viewLocks[lockId].left;
    mapAreaRight = viewLocks[lockId].right;
    locking = true;
//    }
}
