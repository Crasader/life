//
//  Time.cpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#include "GameTime.h"

#include "Equip.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"


USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

GameTime::GameTime()
{
    
}

GameTime::~GameTime()
{
    
}

void GameTime::deleteLoginJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "login_time.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool GameTime::readLoginJson()
{
    loginTime = 0;
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "login_time.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    std::string loginStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseLoginJson(loginStr)) {
        return false;
    }
    
    return true;
}

void GameTime::save2LoginJson()
{
    rapidjson::Document _doc;
    _doc.SetArray();
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("login_time", loginTime, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    std::string loginStr = encode(s);

    auto path = FileUtils::getInstance()->getWritablePath();
    path += "login_time.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(loginStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool GameTime::parseLoginJson(std::string loginStr)
{
    if (loginStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(loginStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsObject())
    {
        return false;
    }
    
    loginTime = (uint64_t)DICTOOL->getUint64Value_json(_doc, "login_time");
    
    return true;
}

void GameTime::deleteFlagJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "flag_time.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool GameTime::readFlagJson()
{
    eventMap.clear();
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "flag_time.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    std::string flagStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseFlagJson(flagStr)) {
        return false;
    }
    
    return true;
}

void GameTime::save2FlagJson()
{
    rapidjson::Document _doc;
    _doc.SetArray();

    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto data : eventMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        int event = data.second.eProtocol;
        object.AddMember("event", event, allocator);
        object.AddMember("time", data.second.timeFlag, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    std::string flagStr = encode(s);

    auto path = FileUtils::getInstance()->getWritablePath();
    path += "flag_time.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(flagStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool GameTime::parseFlagJson(std::string flagStr)
{
    if (flagStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(flagStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    eventMap.clear();
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        TIME_EVENT event;
        event.eProtocol = (T2LProtocol)(DICTOOL->getIntValue_json(dic, "event"));
        event.timeFlag = DICTOOL->getUint64Value_json(dic, "time");
        event.alive = true;
        eventMap[event.eProtocol] = event;
    }
    
    return true;
}


void GameTime::addEvent(T2LProtocol eProtocol, uint64_t time)
{
    TIME_EVENT data;
    data.eProtocol = eProtocol;
    data.timeFlag = time;
    data.alive = true;
    eventMap[eProtocol] = data;
}

bool GameTime::removeDeadEvent()
{
    bool changed = false;
    for (auto iter = eventMap.begin(); iter != eventMap.end(); ) {
        if ((*iter).second.alive == false) {
            changed = true;
            iter = eventMap.erase(iter);
        }else{
            iter++;
        }
    }
    return changed;
}

void GameTime::invalidEvent(T2LProtocol eProtocol)
{
    eventMap[eProtocol].alive = false;
}
