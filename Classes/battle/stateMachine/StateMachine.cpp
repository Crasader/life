//
//  StateMachine.cpp
//  Dabao
//
//  Created by ff on 17/3/9.
//
//

#include "StateMachine.h"
#include "cocostudio/CocoStudio.h"
#include "json/document.h"
#include "utils/GameReader.h"
#include "utils/GameUtils.h"

using namespace cocostudio;




void StateMachine::loadFromFile(std::string filename)
{
    std::string eventListFile(filename);
    eventListFile += "_Event.json";
    loadEventList(eventListFile);
    
    std::string stateListFile(filename);
    stateListFile += "_State.json";
    loadStateList(stateListFile);
    
    std::string stateFlowFile(filename);
    stateFlowFile += "_Flow.json";
    loadStateFlowList(stateFlowFile);
    
    std::string stateDataFile(filename);
    stateDataFile += "_Data.json";
    loadFlowData(stateDataFile);
    
    std::string stateRateFile(filename);
    stateRateFile += "_Rate.json";
    loadRateData(stateRateFile);
    
}

void StateMachine::loadEventList(std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        int objId;
        int val;
        std::string name;
        std::string action;
        
        for (int i = 0; i<size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            objId =DICTOOL->getIntValue_json(dic, "ID");
            name = DICTOOL->getStringValue_json(dic, "eventLib");
            val = DICTOOL->getIntValue_json(dic, "value");
            action = DICTOOL->getStringValue_json(dic, "animName");
            eventMap[name] = val;
            eventName[val] = name;
            auto actNames = GameUtils::splitStr(action, ",");
            actionNames[val] = actNames;
        }
    }
}

void StateMachine::loadStateList(std::string filename)

{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        int objId;
        int val;
        std::string name;
        
        for (int i = 0; i<size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            objId =DICTOOL->getIntValue_json(dic, "ID");
            name = DICTOOL->getStringValue_json(dic, "stateLib");
            val = DICTOOL->getIntValue_json(dic, "value");
            
            stateMap[name] = val;
            stateName[val] = name;
        }
    }
    
}

void StateMachine::loadStateFlowList(std::string filename)

{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        int objId;
        std::string stateName;
        std::string eventName;
        int flowIndex;
        
        for (int i = 0; i<size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            objId =DICTOOL->getIntValue_json(dic, "ID");
            stateName = DICTOOL->getStringValue_json(dic, "state");
            eventName = DICTOOL->getStringValue_json(dic, "event");
            flowIndex = DICTOOL->getIntValue_json(dic, "index");
            
            int state = stateMap[stateName];
            int event = eventMap[eventName];
            
            auto flow = stateFlowMap.find(state);
            if (flow == stateFlowMap.end()) {
                std::map<int, int> eventFlowMap;
                eventFlowMap[event] = flowIndex;
                stateFlowMap[state] = eventFlowMap;
            } else {
                stateFlowMap[state][event] = flowIndex;
            }
        }
    }
    
}

void StateMachine::loadFlowData(std::string filename)

{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        int objId;
        std::string nextStateName;
        std::string nextActionName;
        int flag;
        
        for (int i = 0; i<size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            objId =DICTOOL->getIntValue_json(dic, "ID");
            nextStateName = DICTOOL->getStringValue_json(dic, "nextstate");
            nextActionName = DICTOOL->getStringValue_json(dic, "nextaction");
            flag = DICTOOL->getIntValue_json(dic, "flag");
            
            int state = stateMap[nextStateName];
            int action = eventMap[nextActionName];
            
            NextStateData nextData;
            nextData.nextTactics = state;
            nextData.nextAction = action;
            nextData.flag = flag;
            
            stateData.push_back(nextData);
        }
    }
}

void StateMachine::loadRateData(std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if (doc->IsArray()){
        int size = doc->Size();
        int objId;
        int rateIdx = -1;
        int rate = -1;
        int flowData = -1;
        
        for (int i = 0; i<size; i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json((*doc), i);
            objId =DICTOOL->getIntValue_json(dic, "ID");
            rateIdx = DICTOOL->getIntValue_json(dic, "index");
            rate = DICTOOL->getIntValue_json(dic, "rate");
            flowData = DICTOOL->getIntValue_json(dic, "flow");
            
            StateRate rateObj;
            rateObj.rate1 = rate;
            rateObj.flowIndex = flowData;
            auto rateVec = rateData.find(rateIdx);
            if (rateVec == rateData.end()) {
                std::vector<StateRate> stateRateVec;
                stateRateVec.push_back(rateObj);
                rateData[rateIdx] = stateRateVec;
            }else{
                rateData[rateIdx].push_back(rateObj);
            }
        }
    }
}

int StateMachine::findEventVal(std::string eventName)
{
    auto eventMap = getEventMap();
    auto eventVal = eventMap.find(eventName);
    if (eventVal == eventMap.end()) {
        return action_undefined;
    }
    return eventMap[eventName];
}

std::vector<std::string> StateMachine::findActionNames(int action)
{
    auto name = actionNames.find(action);
    if (name == actionNames.end()) {
        return *new std::vector<std::string>;
    }
    if (actionNames[action][0] == "null") {
        return *new std::vector<std::string>;
    }
    return actionNames[action];
}

void StateMachine::nextState()

{
    nextAction = action_undefined;
    
    getTacticsAction(currEvent, currTactics);
    
}


void StateMachine::getTacticsAction(int event, int state)

{
    //找不到状态，不处理
    auto eventFlowMap = stateFlowMap.find(state);
    if(eventFlowMap == stateFlowMap.end())
    {
        return;
    }
    
    //找不到事件，不处理
    auto flowMap = eventFlowMap->second;
    auto flowVal = flowMap.find(event);
    if (flowVal == flowMap.end()) {
        return;
    }
    
    //索引不对，不处理
    auto flowIndex = flowVal->second;
    int flag = flowIndex & StateAIFlag::getFlag;
    if (flag == 0) {
        if (flowIndex < action_undefined || flowIndex >= stateData.size()) {
            return;
        }
    }
    
    parseData(flowIndex);
    
}


void StateMachine::parseData(int flowData)

{
    int flag, data;
    
    if (flowData == action_undefined) {
        //需要将下个动作设为未定义时
        nextAction = action_undefined;
    }else{
        //正常流程
        flag = flowData & StateAIFlag::getFlag;
        data = flowData & StateAIFlag::getData;
        
        if (flag == StateAIFlag::rate) {
            //随机值判断
            getRateAction(data);            
        }else{
            //赋值过程
            auto nextData = stateData.at(data);
            nextTactics = nextData.nextTactics;
            nextAction = nextData.nextAction;
            nextFlag = nextData.flag;
        }
    }
}


void StateMachine::getRateAction(int randIndex)

{
    //找到随机集合
    auto rateVal = rateData.find(randIndex);
    if(rateVal == rateData.end())
    {
        return;
    }
    
    auto rateVec = rateVal->second;
    
    //初始化
    int total, gameRand;
    int vecIndex = 0;
    total = 0;
    
    gameRand = rand()%max_Rate;
    
    //在集合中匹配对应的随机值
    while (total < max_Rate) {
        if (vecIndex>=rateVec.size()) {
            //没有找到匹配的随机值
            break;
        }
        
        auto data = rateVec.at(vecIndex);
        total += data.rate1;
        //规则是临时随机值在匹配范围内
        if (gameRand < total) {
            parseData(data.flowIndex);
            break;
        }
        vecIndex++;
    }
}


void StateMachine::changeState(int state, int action)

{
    currTactics = state;
    currEvent = action;
}

std::map<std::string, int> StateMachine::getEventMap()
{
    return eventMap;
}

std::map<std::string, int> StateMachine::getStateMap()
{
    return stateMap;
}

