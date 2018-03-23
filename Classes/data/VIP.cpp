//
//  VIP.cpp
//  life
//
//  Created by ff on 17/11/13.
//
//

#include "VIP.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> VIP::configIdTable;
std::map<int, int> VIP::packageIdTable;

VIP::VIP():
tip(false)
{
    
}

VIP::~VIP()
{
    
}

void VIP::readVIPConfig()
{
    auto doc = GameReader::getDocInstance(VIP_CONFIG_FILE);
    if (configIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", configIdTable);
    }
    
    vipConfigMap.clear();
    
    for (int i = 0; i < configIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        VIP_CONFIG config;
        config.vipId = DICTOOL->getIntValue_json(dic, "id");
        config.uplevelRecharge = DICTOOL->getIntValue_json(dic, "uplevel_recharge");
        config.uplevelDay = DICTOOL->getIntValue_json(dic, "uplevel_day");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        config.descript[0] = DICTOOL->getStringValue_json(dic, "descript1");
        config.descript[1] = DICTOOL->getStringValue_json(dic, "descript2");
        config.descript[2] = DICTOOL->getStringValue_json(dic, "descript3");
        config.descript[3] = DICTOOL->getStringValue_json(dic, "descript4");
        config.descript[4] = DICTOOL->getStringValue_json(dic, "descript5");
        config.descript[5] = DICTOOL->getStringValue_json(dic, "descript6");
        config.descript[6] = DICTOOL->getStringValue_json(dic, "descript7");
        config.descript[7] = DICTOOL->getStringValue_json(dic, "descript8");
        config.descript[8] = DICTOOL->getStringValue_json(dic, "descript9");
        config.descript[9] = DICTOOL->getStringValue_json(dic, "descript10");
        
        vipConfigMap[config.vipId] = config;
    }
    return;
}

void VIP::readVIPPackageConfig()
{
    auto doc = GameReader::getDocInstance(VIP_PACKAGE_CONFIG_FILE);
    if (packageIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", packageIdTable);
    }
    
    packageConfigMap.clear();
    
    for (int i = 0; i < packageIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        VIP_PACKAGE_CONFIG config;
        config.itemId = DICTOOL->getIntValue_json(dic, "id");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        
        packageConfigMap[config.itemId] = config;
    }
    return;
}

void VIP::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "vip_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void VIP::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "vip_info.json";
    
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

void VIP::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("vip_id", 1, allocator);
    _doc.AddMember("login_day", 0, allocator);
//    _doc.AddMember("last_login_sec", 0, allocator);
    _doc.AddMember("bound_state", 1, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

bool VIP::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "vip_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool VIP::parseJson()
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
    
    
    coreData.vipId = DICTOOL->getIntValue_json(_doc, "vip_id");
    coreData.loginDay = DICTOOL->getIntValue_json(_doc, "login_day");
//    coreData.lastLoginSec = DICTOOL->getUint64Value_json(_doc, "last_login_sec");
    coreData.boundState = DICTOOL->getIntValue_json(_doc, "bound_state");
    tip = (coreData.boundState == 1);
    return true;
}

void VIP::addVIPDay()
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

    coreData.vipId = DICTOOL->getIntValue_json(_doc, "vip_id");
    coreData.loginDay = DICTOOL->getIntValue_json(_doc, "login_day");
    if (coreData.vipId > 1 && coreData.vipId < 4) {
    coreData.loginDay++;
        if (coreData.loginDay >= vipConfigMap[coreData.vipId].uplevelDay) {
            coreData.vipId++;
            coreData.loginDay = 1;
        }
    }
    _doc["vip_id"] = coreData.vipId;
    _doc["login_day"] = coreData.loginDay;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

void VIP::clearVIPDay()
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

    coreData.loginDay = 1;
    _doc["login_day"] = coreData.loginDay;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

void VIP::uplevelByRecharge(int totalRecharge)
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

    coreData.vipId = DICTOOL->getIntValue_json(_doc, "vip_id");
    
    for (int i = coreData.vipId; i < vipConfigMap.size(); i++) {
        if (totalRecharge >= vipConfigMap[i].uplevelRecharge) {
            coreData.loginDay = 1;
            coreData.boundState = 1;
            coreData.vipId = i+1;
        }
    }
    
    _doc["vip_id"] = coreData.vipId;
    _doc["login_day"] = coreData.loginDay;
    _doc["bound_state"] = coreData.boundState;
    tip = (coreData.boundState == 1);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

void VIP::enableBound()
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

    coreData.boundState = 1;
    _doc["bound_state"] = coreData.boundState;
    tip = (coreData.boundState == 1);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

int VIP::takeBound()
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
    coreData.boundState = DICTOOL->getIntValue_json(_doc, "bound_state");
    if (coreData.boundState != 1) {
        return 2;
    }
    coreData.boundState = 0;
    _doc["bound_state"] = coreData.boundState;
    tip = (coreData.boundState == 1);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
    return 0;
}

void VIP::getPackageById(int packageId, std::vector<int> &packageIdVec)
{
    packageIdVec.clear();
    for (auto config : packageConfigMap) {
        if (config.second.packageId == packageId) {
            packageIdVec.push_back(config.first);
        }
    }
}

//void VIP::setVIPLoginSec(uint64_t sec)
//{
//    if (coreStr == "") {
//        return;
//    }
//    
//    rapidjson::Document _doc;
//    std::string tempStr = decode(coreStr.c_str());
//    _doc.Parse < 0 > (tempStr.c_str());
//    if(_doc.HasParseError())
//    {
//        return;
//    }
//    
//    if(!_doc.IsObject())
//    {
//        return;
//    }
//
//    coreData.lastLoginSec = sec;
//    _doc["last_login_sec"] = coreData.lastLoginSec;
//    
//    StringBuffer buff;
//    rapidjson::Writer<StringBuffer> writer(buff);
//    _doc.Accept(writer);
//    std::string s = StringUtils::format("%s", buff.GetString());
//    //    log("========================\n");
//    //    log("%s\n", s.c_str());
//    coreStr = encode(s);
//    save2Json();
//}
