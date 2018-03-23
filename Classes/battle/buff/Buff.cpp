//
//  Buff.cpp
//  WuhunLua
//
//  Created by ff on 15-3-19.
//
//

#include "Buff.h"
#include "EventDefine.h"

USING_NS_CC;
using namespace cocostudio;
bool Buff::init()
{
    if (!Node::init()) {
        return false;
    }
    

    return true;
}

void Buff::onEnter()
{
    Node::onEnter();
    pauseListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(Buff::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseListener, -1);
    resumeListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(Buff::resumeFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeListener, -1);
    
}

void Buff::onExit()
{
    Node::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeListener);
}

bool Buff::initData(int index, rapidjson::Document& _doc) {
//    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    
    buffId = DICTOOL->getIntValue_json(dic, "ID");
    descript = DICTOOL->getStringValue_json(dic, "descript");
    iconStr = DICTOOL->getStringValue_json(dic, "icon");
    efctStr = DICTOOL->getStringValue_json(dic, "res");
    type = DICTOOL->getIntValue_json(dic, "type");
    debuff = (DICTOOL->getIntValue_json(dic, "debuff") == 1);
    interval = DICTOOL->getFloatValue_json(dic, "interval");
    duration = DICTOOL->getFloatValue_json(dic, "duration");
    valueType = DICTOOL->getIntValue_json(dic, "valueType");
    value = DICTOOL->getFloatValue_json(dic, "value");
    targetType = DICTOOL->getIntValue_json(dic, "target");
    return true;
}

void Buff::initHandle(std::function<void (Buff*)> handle, std::function<void(Buff*)> remove)
{
    handleFunc = handle;
    removeFunc = remove;
}

void Buff::beginActive()
{
    valid = true;
    handleFunc(this);
    if (interval > 0) {
        schedule(schedule_selector(Buff::onActive), interval);
    }
    
    if (duration > 0) {
        schedule(schedule_selector(Buff::onRemove), duration);
    }
}

void Buff::onActive(float dt)
{
    handleFunc(this);
}

void Buff::onRemove(float dt)
{
    unschedule(schedule_selector(Buff::onActive));
    unschedule(schedule_selector(Buff::onRemove));
    removeFunc(this);
    valid = false;

}

void Buff::pauseFight()
{
    pause();
}

void Buff::resumeFight()
{
    resume();
}
