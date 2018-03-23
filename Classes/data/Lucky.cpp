//
//  Lucky.cpp
//  life
//
//  Created by ff on 17/11/6.
//
//

#include "Lucky.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Lucky::configIdTable;
std::map<int, int> Lucky::normalConfigIdTable;
std::map<int, int> Lucky::specialConfigIdTable;

Lucky::Lucky():
coreStr(""),
specialStr(""),
normalStr(""),
tip(false)
{
    normalMap.clear();
    specialMap.clear();
    normalThresMap.clear();
    specialThresMap.clear();
}

Lucky::~Lucky()
{
    
}

void Lucky::readLuckyConfig()
{
    auto doc = GameReader::getDocInstance(LUCKY_CONFIG_FILE);
    if (configIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", configIdTable);
    }

    normalMap.clear();
    specialMap.clear();
    normalThresMap.clear();
    specialThresMap.clear();
    normalTotalThres = 0;
    specialTotalThres = 0;
    for (int i = 0; i < configIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);

        LUCKY_CONFIG config;
        config.luckyId = DICTOOL->getIntValue_json(dic, "id");
        config.luckyType = DICTOOL->getIntValue_json(dic, "type");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        config.rate = DICTOOL->getIntValue_json(dic, "rate");
        if (config.luckyType == 1) {
            normalMap[config.luckyId] = config;
            normalThresMap[config.luckyId] = config.rate;
            normalTotalThres += config.rate;
        }else if (config.luckyType == 2) {
            specialMap[config.luckyId] = config;
            specialThresMap[config.luckyId] = config.rate;
            specialTotalThres += config.rate;
        }
    }
}

int Lucky::drawNormal()
{
    if (coreStr == "") {
        return -1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return -1;
    }
    
    if(!_doc.IsObject())
    {
        return -1;
    }
    if (normalStr == "") {
        return -1;
    }
    
    rapidjson::Document _normalDoc;
    std::string tempStr1 = decode(normalStr.c_str());
    _normalDoc.Parse < 0 > (tempStr1.c_str());
    if(_normalDoc.HasParseError())
    {
        return -1;
    }
    
    if(!_normalDoc.IsArray())
    {
        return -1;
    }
    normalTicket = DICTOOL->getIntValue_json(_doc, "normal_ticket");
    if (normalTicket <= 0) {
        return 0;
    }
    normalTicket--;
    _doc["normal_ticket"] = normalTicket;
    
    int rate = abs(rand())%normalTotalThres;
    int tempRate = 0;
    for (auto config : normalThresMap) {
        tempRate += config.second;
        if (rate < tempRate) {
            int normalThres = normalThresMap[config.first];
            normalThres = MIN(100, normalThres);
            normalThresMap[config.first] -= normalThres;
            normalThresMap[config.first] = MAX(0, normalThresMap[config.first]);
            _normalDoc[normalConfigIdTable[config.first]]["rate"] = normalThresMap[config.first];
            normalTotalThres -= normalThres;
            if (normalTotalThres <= 5000) {
                resetNormalThres();
            }
            _doc["normal_total_thres"] = normalTotalThres;
            
            StringBuffer buff;
            rapidjson::Writer<StringBuffer> writer(buff);
            _doc.Accept(writer);
            std::string s = StringUtils::format("%s", buff.GetString());
            coreStr = encode(s);
            save2Json();
            
            StringBuffer buff1;
            rapidjson::Writer<StringBuffer> writer1(buff1);
            _normalDoc.Accept(writer1);
            std::string s1 = StringUtils::format("%s", buff1.GetString());
            normalStr = encode(s1);
            save2ThresJson();
            return config.first;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
    
    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _normalDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    normalStr = encode(s1);
    save2ThresJson();

    return 0;
}

int Lucky::drawSpecial()
{
    if (coreStr == "") {
        return 0;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 0;
    }
    
    if(!_doc.IsObject())
    {
        return 0;
    }
    if (specialStr == "") {
        return 0;
    }
    
    rapidjson::Document _specialDoc;
    std::string tempStr1 = decode(specialStr.c_str());
    _specialDoc.Parse < 0 > (tempStr1.c_str());
    if(_specialDoc.HasParseError())
    {
        return 0;
    }
    
    if(!_specialDoc.IsArray())
    {
        return 0;
    }
    specialTicket = DICTOOL->getIntValue_json(_doc, "special_ticket");
    if (specialTicket <= 0) {
        return 0;
    }
    specialTicket--;
    _doc["special_ticket"] = specialTicket;
    
    int rate = abs(rand())%specialTotalThres;
    int tempRate = 0;
    for (auto config : specialThresMap) {
        tempRate += config.second;
        if (rate < tempRate) {
            int speicalThres = config.second;
            speicalThres = MIN(100, speicalThres);
            specialThresMap[config.first] -= speicalThres;
            specialThresMap[config.first] = MAX(0, specialThresMap[config.first]);
            _specialDoc[specialConfigIdTable[config.first]]["rate"] = specialThresMap[config.first];
            specialTotalThres -= speicalThres;
            if (specialTotalThres <= 5000) {
                resetSpecialThres();
            }
            _doc["special_total_thres"] = specialTotalThres;
            
            StringBuffer buff;
            rapidjson::Writer<StringBuffer> writer(buff);
            _doc.Accept(writer);
            std::string s = StringUtils::format("%s", buff.GetString());
            coreStr = encode(s);
            save2Json();
            
            StringBuffer buff1;
            rapidjson::Writer<StringBuffer> writer1(buff1);
            _specialDoc.Accept(writer1);
            std::string s1 = StringUtils::format("%s", buff1.GetString());
            specialStr = encode(s1);
            save2ThresJson();
            return config.first;
        }
    }
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
    
    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _specialDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    specialStr = encode(s1);
    save2ThresJson();

    return 0;
}

int Lucky::drawNormalForFree()
{
    if (coreStr == "") {
        return -1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return -1;
    }
    
    if(!_doc.IsObject())
    {
        return -1;
    }
    if (normalStr == "") {
        return -1;
    }
    
    rapidjson::Document _normalDoc;
    std::string tempStr1 = decode(normalStr.c_str());
    _normalDoc.Parse < 0 > (tempStr1.c_str());
    if(_normalDoc.HasParseError())
    {
        return -1;
    }
    
    if(!_normalDoc.IsArray())
    {
        return -1;
    }
    normalState = DICTOOL->getIntValue_json(_doc, "normal_state");
    if (normalState != 1) {
        return 0;
    }
    normalState = 0;
    _doc["normal_state"] = normalState;
    
    int rate = abs(rand())%normalTotalThres;
    int tempRate = 0;
    for (auto config : normalThresMap) {
        tempRate += config.second;
        if (rate < tempRate) {
            int normalThres = config.second;
            normalThres = MIN(100, normalThres);
            normalThresMap[config.first] -= normalThres;
            normalThresMap[config.first] = MAX(0, normalThresMap[config.first]);
            _normalDoc[normalConfigIdTable[config.first]]["rate"] = normalThresMap[config.first];
            normalTotalThres -= normalThres;
            if (normalTotalThres <= 5000) {
                resetNormalThres();
            }
            _doc["normal_total_thres"] = normalTotalThres;
            
            StringBuffer buff;
            rapidjson::Writer<StringBuffer> writer(buff);
            _doc.Accept(writer);
            std::string s = StringUtils::format("%s", buff.GetString());
            coreStr = encode(s);
            save2Json();
            
            StringBuffer buff1;
            rapidjson::Writer<StringBuffer> writer1(buff1);
            _normalDoc.Accept(writer1);
            std::string s1 = StringUtils::format("%s", buff1.GetString());
            normalStr = encode(s1);
            save2ThresJson();

            return config.first;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
    
    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _normalDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    normalStr = encode(s1);
    save2ThresJson();

    return 0;
}

int Lucky::drawSpecialForFree()
{
    if (coreStr == "") {
        return 0;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 0;
    }
    
    if(!_doc.IsObject())
    {
        return 0;
    }
    if (specialStr == "") {
        return 0;
    }
    
    rapidjson::Document _specialDoc;
    std::string tempStr1 = decode(specialStr.c_str());
    _specialDoc.Parse < 0 > (tempStr1.c_str());
    if(_specialDoc.HasParseError())
    {
        return 0;
    }
    
    if(!_specialDoc.IsArray())
    {
        return 0;
    }
    specialState = DICTOOL->getIntValue_json(_doc, "special_state");
    if (specialState != 1) {
        return 0;
    }
    specialState = 0;
    _doc["special_state"] = specialState;
    
    int rate = abs(rand())%specialTotalThres;
    int tempRate = 0;
    for (auto config : specialThresMap) {
        tempRate += config.second;
        if (rate < tempRate) {
            int speicalThres = config.second;
            speicalThres = MIN(100, speicalThres);
            specialThresMap[config.first] -= speicalThres;
            specialThresMap[config.first] = MAX(0, specialThresMap[config.first]);
            _specialDoc[specialConfigIdTable[config.first]]["rate"] = specialThresMap[config.first];
            specialTotalThres -= speicalThres;
            if (specialTotalThres <= 5000) {
                resetSpecialThres();
            }
            _doc["special_total_thres"] = specialTotalThres;
            
            StringBuffer buff;
            rapidjson::Writer<StringBuffer> writer(buff);
            _doc.Accept(writer);
            std::string s = StringUtils::format("%s", buff.GetString());
            coreStr = encode(s);
            save2Json();
            
            StringBuffer buff1;
            rapidjson::Writer<StringBuffer> writer1(buff1);
            _specialDoc.Accept(writer1);
            std::string s1 = StringUtils::format("%s", buff1.GetString());
            specialStr = encode(s1);
            save2ThresJson();

            return config.first;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
    
    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _specialDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    specialStr = encode(s1);
    save2ThresJson();

    return 0;
}

int Lucky::drawSpecialTen(int cardId[10], int count[10])
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    if (specialStr == "") {
        return 1;
    }
    
    rapidjson::Document _specialDoc;
    std::string tempStr1 = decode(specialStr.c_str());
    _specialDoc.Parse < 0 > (tempStr1.c_str());
    if(_specialDoc.HasParseError())
    {
        return 1;
    }
    
    if(!_specialDoc.IsArray())
    {
        return 1;
    }

    specialTicket = DICTOOL->getIntValue_json(_doc, "special_ticket");
    if (specialTicket < 10) {
        return 2;
    }
    specialTicket -= 10;
    _doc["special_ticket"] = specialTicket;
    
    memset(cardId, 0, sizeof(int)*10);
    memset(count, 0, sizeof(int)*10);
    for (int i = 0; i < 10; i++) {
        int rate = abs(rand())%specialTotalThres;
        int tempRate = 0;
        for (auto config : specialThresMap) {
            tempRate += config.second;
            if (rate < tempRate) {
                int speicalThres = config.second;
                speicalThres = MIN(100, speicalThres);
                specialThresMap[config.first] -= speicalThres;
                specialThresMap[config.first] = MAX(0, specialThresMap[config.first]);
                _specialDoc[specialConfigIdTable[config.first]]["rate"] = specialThresMap[config.first];
                specialTotalThres -= speicalThres;
                if (specialTotalThres <= 5000) {
                    resetSpecialThres();
                }
                _doc["special_total_thres"] = specialTotalThres;
                cardId[i] = config.first;
                if (specialMap[config.first].boundCount == 1) {
                    count[i] = 1;
                }else{
                    count[i] = abs(rand())%specialMap[config.first].boundCount+1;
                }
                break;
            }
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();

    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _specialDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    specialStr = encode(s1);
    save2ThresJson();
    

    return 0;
}

void Lucky::resetNormalThres()
{
    normalTotalThres = 0;
    for (auto config : normalMap) {
        normalThresMap[config.second.luckyId] = config.second.rate;
        normalTotalThres += config.second.rate;
    }
}

void Lucky::resetSpecialThres()
{
    specialTotalThres = 0;
    for (auto config : specialMap) {
        specialThresMap[config.second.luckyId] = config.second.rate;
        specialTotalThres += config.second.rate;
    }
}

void Lucky::enableNormalDraw()
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }

    normalState = 1;
    _doc["normal_state"] = normalState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
}

void Lucky::enableSpecialDraw()
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }

    specialState = 1;
    _doc["special_state"] = specialState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
}

void Lucky::addNormalTicket(int val)
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }
    
    normalTicket = DICTOOL->getIntValue_json(_doc, "normal_ticket");
    normalTicket += val;
    _doc["normal_ticket"] = normalTicket;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
}

void Lucky::addSpecialTicket(int val)
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }
    
    specialTicket = DICTOOL->getIntValue_json(_doc, "special_ticket");
    specialTicket += val;
    _doc["special_ticket"] = specialTicket;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
}

void Lucky::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Lucky::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_info.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(coreStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

void Lucky::save2EmptyJson()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }

    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("normal_state", 1, allocator);
    _doc.AddMember("special_state", 1, allocator);
    _doc.AddMember("normal_ticket", 1, allocator);
    _doc.AddMember("special_ticket", 1, allocator);
    _doc.AddMember("normal_total_thres", 10000, allocator);
    _doc.AddMember("special_total_thres", 10000, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    coreStr = encode(s);
    save2Json();
}

bool Lucky::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Lucky::parseJson()
{
    if (coreStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsObject())
    {
        return false;
    }
    
    normalState = DICTOOL->getIntValue_json(_doc, "normal_state");
    specialState = DICTOOL->getIntValue_json(_doc, "special_state");
    normalTicket = DICTOOL->getIntValue_json(_doc, "normal_ticket");
    specialTicket = DICTOOL->getIntValue_json(_doc, "special_ticket");
    normalTotalThres = DICTOOL->getIntValue_json(_doc, "normal_total_thres");
    specialTotalThres = DICTOOL->getIntValue_json(_doc, "special_total_thres");

    return true;
}

void Lucky::deleteThresJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_normal_thres.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
    
    path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_special_thres.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Lucky::save2ThresJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_normal_thres.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(normalStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
    
    path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_special_thres.json";
    
    FILE* pFile1 = fopen(path.c_str(),"wb");
    if (pFile1)
    {
        fputs(specialStr.c_str(),pFile1);
        fclose(pFile1);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

void Lucky::readThres()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_normal_thres.json";
    bool change = false;
    if(!FileUtils::getInstance()->isFileExist(path)) {
        save2EmptyNormal();
        change = true;
    }else{
        normalStr = FileUtils::getInstance()->getStringFromFile(path);
    }
    normalConfigIdTable.clear();
    rapidjson::Document _normalDoc;
    std::string tempStr = decode(normalStr.c_str());
    _normalDoc.Parse < 0 > (tempStr.c_str());
    if(_normalDoc.HasParseError())
    {
        return;
    }
    
    if(!_normalDoc.IsArray())
    {
        return;
    }
    GameReader::initIdTable(_normalDoc, "lucky_id", normalConfigIdTable);
    
    path = FileUtils::getInstance()->getWritablePath();
    path += "lucky_special_thres.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        save2EmptySpecial();
        change = true;
    }else{
        specialStr = FileUtils::getInstance()->getStringFromFile(path);
    }
    specialConfigIdTable.clear();
    rapidjson::Document _specialDoc;
    std::string tempStr1 = decode(specialStr.c_str());
    _specialDoc.Parse < 0 > (tempStr1.c_str());
    if(_specialDoc.HasParseError())
    {
        return;
    }
    
    if(!_specialDoc.IsArray())
    {
        return;
    }
    GameReader::initIdTable(_specialDoc, "lucky_id", specialConfigIdTable);

    if (change) {
        save2ThresJson();
    }
}

void Lucky::save2EmptyNormal()
{
    resetNormalThres();
    rapidjson::Document _doc;
    if (normalStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : normalThresMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("lucky_id", config.first, allocator);
        object.AddMember("rate", config.second, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    normalStr = encode(s);
}

void Lucky::save2EmptySpecial()
{
    resetSpecialThres();
    rapidjson::Document _doc;
    if (specialStr != "") {
        return;
    }else{
        _doc.SetArray();
    }

    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : specialThresMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("lucky_id", config.first, allocator);
        object.AddMember("rate", config.second, allocator);
        _doc.PushBack(object, allocator);
    }

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    specialStr = encode(s);

}

void Lucky::checkTip()
{
    tip = false;
    if (normalState == 1) {
        tip = true;
    }
    if (specialState == 1) {
        tip = true;
    }
}
