#ifndef __SNEAKY_JOYSTICK_H__
#define __SNEAKY_JOYSTICK_H__

#include "cocos2d.h"

class SneakyJoystick : public cocos2d::Node 
{
protected:
	float joystickRadiusSq;
    float touchRadiusSq;
	float thumbRadiusSq;
	float deadRadiusSq;

    CC_SYNTHESIZE_READONLY(bool, bigStick, BigStick);
	CC_SYNTHESIZE_READONLY(cocos2d::Point, stickPosition, StickPosition);
	CC_SYNTHESIZE_READONLY(float, degrees, Degrees);
	CC_SYNTHESIZE_READONLY(cocos2d::Point, velocity, Velocity);
	CC_SYNTHESIZE(bool, autoCenter, AutoCenter);
	CC_SYNTHESIZE_READONLY(bool, isDPad, IsDPad);
	CC_SYNTHESIZE(bool, hasDeadzone, HasDeadzone);
	CC_SYNTHESIZE(int, numberOfDirections, NumberOfDirections);

	CC_SYNTHESIZE_READONLY(float, joystickRadius, JoystickRadius);
    CC_SYNTHESIZE_READONLY(float, touchRadius, TouchRadius);
	CC_SYNTHESIZE_READONLY(float, thumbRadius, ThumbRadius);
	CC_SYNTHESIZE_READONLY(float, deadRadius, DeadRadius);

	//add press flag
	CC_SYNTHESIZE_READONLY(bool, press, Press);
	CC_SYNTHESIZE_READONLY(bool, touchIn, TouchIn);

	virtual ~SneakyJoystick();

	bool initWithRect(cocos2d::Rect rect, bool bigStick);
	virtual bool init();
	virtual void onExit();
	void setIsDPad(bool b);
	void setJoystickRadius(float r);
    void setTouchRadius(float r);
	void setThumbRadius(float r);
	void setDeadRadius(float r);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

	void touchDelegateRelease();
	void touchDelegateRetain();

	CREATE_FUNC(SneakyJoystick);
private:
	void updateVelocity(cocos2d::Point point);
	void setTouchRadius();
	float round(float r);
};
#endif
