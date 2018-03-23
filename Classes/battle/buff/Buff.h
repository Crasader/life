//
//  Buff.h
//  WuhunLua
//
//  Created by ff on 15-3-19.
//
//

#ifndef __WuhunLua__Buff__
#define __WuhunLua__Buff__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Buff : public cocos2d::Node
{
public:
    bool init();
    void onEnter();
    void onExit();
    
    bool initData(int idx, rapidjson::Document& _doc);
    
    void beginActive();
    
    void onActive(float dt);
    
    void onRemove(float dt);
    
    void initHandle(std::function<void (Buff*)> handle, std::function<void(Buff*)> remove);
    
    void pauseFight();
    void resumeFight();
    CREATE_FUNC(Buff);

private:
    cocos2d::EventListenerCustom *pauseListener;
    cocos2d::EventListenerCustom *resumeListener;

    CC_SYNTHESIZE_READONLY(std::string, descript, Descript);    //描述
    CC_SYNTHESIZE_READONLY(std::string, iconStr, IconStr);      //图标资源名
    CC_SYNTHESIZE_READONLY(std::string, efctStr, EffectStr);    //效果资源名
    CC_SYNTHESIZE_READONLY(int, buffId, BuffId);                //配置表中ID
    CC_SYNTHESIZE_READONLY(bool, debuff, DeBuff);               //是否减益buff
    CC_SYNTHESIZE_READONLY(float, interval, Interval);          //间隔时间单位是秒，小数点后为毫秒
    CC_SYNTHESIZE_READONLY(float, duration, Duration);          //有效时间，过了有效时间就删除此buff
    CC_SYNTHESIZE_READONLY(int, type, Type);                    //buff类型，类型列表见类型枚举
    CC_SYNTHESIZE_READONLY(int, valueType, ValueType);          //用于改值型buff，类型见值类型枚举
    CC_SYNTHESIZE_READONLY(float, value, Value);                //改值型buff的值
    CC_SYNTHESIZE_READONLY(int, targetType, TargetType);        //目标对象类型：1:自己；2:敌方；3:所有友方(包括自己)
    
    CC_SYNTHESIZE_READONLY(bool, valid, Valid);                 //buff有效标签，无效的buff由BuffManager去掉
    
    std::function<void(Buff* )> handleFunc;     //起效时执行的函数
    std::function<void(Buff* )> removeFunc;     //移除时执行的函数
};

#endif /* defined(__WuhunLua__Buff__) */
