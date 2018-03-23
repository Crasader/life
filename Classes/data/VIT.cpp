//
//  VIT.cpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#include "VIT.h"
#include "utils/libs.h"
#include "utils/GameReader.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;


VIT::VIT():
coreStr("")
{
    maxBaseVIT = originVIT;
    baseVIT = originVIT;
    plusVIT = 0;
}

VIT::~VIT()
{
    
}

void VIT::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "VIT.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);

}
void VIT::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "VIT.json";
    
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

bool VIT::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "VIT.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool VIT::parseJson()
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
    
    maxBaseVIT = DICTOOL->getIntValue_json(_doc, "max_base");
    baseVIT = DICTOOL->getIntValue_json(_doc, "base");
    plusVIT = DICTOOL->getIntValue_json(_doc, "plus");
    buyCount = DICTOOL->getIntValue_json(_doc, "buy_count");
    maxBuyCount = DICTOOL->getIntValue_json(_doc, "max_buy_count");
    return true;
}

void VIT::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("max_base", originVIT, allocator);
    _doc.AddMember("base", originVIT, allocator);
    _doc.AddMember("plus", 0, allocator);
    _doc.AddMember("buy_count", 2, allocator);
    _doc.AddMember("max_buy_count", 2, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

bool VIT::restoreVIT(int val)
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
    
    maxBaseVIT = DICTOOL->getIntValue_json(_doc, "max_base");
    baseVIT = DICTOOL->getIntValue_json(_doc, "base");
    baseVIT += val;
    baseVIT = MIN(maxBaseVIT, baseVIT);
    _doc["base"] = baseVIT;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return true;
}

void VIT::addPlusVIT(int val)
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
    
    plusVIT = DICTOOL->getIntValue_json(_doc, "plus");
    plusVIT += val;
    _doc["plus"] = plusVIT;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

void VIT::addMaxBaseVIT(int val)
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
    
    maxBaseVIT = DICTOOL->getIntValue_json(_doc, "max_base");
    maxBaseVIT += val;
    _doc["max_base"] = maxBaseVIT;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

bool VIT::useVIT(int val)
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
    
    baseVIT = DICTOOL->getIntValue_json(_doc, "base");
    plusVIT = DICTOOL->getIntValue_json(_doc, "plus");
    
    if (val > baseVIT + plusVIT) {
        return false;
    }
    if (val > baseVIT) {
        int leftVal = val-baseVIT;
        baseVIT = 0;
        plusVIT -= leftVal;
    }else{
        baseVIT -= val;
    }
    
    _doc["base"] = baseVIT;
    _doc["plus"] = plusVIT;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return true;
}

int VIT::buyVIT()
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
    
    buyCount = DICTOOL->getIntValue_json(_doc, "buy_count");
    if (buyCount <= 0) {
        return 2;
    }
    plusVIT = DICTOOL->getIntValue_json(_doc, "plus");
    
    plusVIT += 50;
    buyCount--;
    
    _doc["plus"] = plusVIT;
    _doc["buy_count"] = buyCount;
    
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

void VIT::resetBuyCount()
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

    buyCount = DICTOOL->getIntValue_json(_doc, "buy_count");
    maxBuyCount = DICTOOL->getIntValue_json(_doc, "max_buy_count");
    
    buyCount = maxBuyCount;
    
    _doc["buy_count"] = buyCount;

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

}

void VIT::addMaxBuyCount(int val)
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
    
    maxBuyCount = DICTOOL->getIntValue_json(_doc, "max_buy_count");
    buyCount = DICTOOL->getIntValue_json(_doc, "buy_count");
    
    maxBuyCount += val;
    maxBuyCount = MAX(0, maxBuyCount);
    _doc["max_buy_count"] = maxBuyCount;
    buyCount += val;
    buyCount = MAX(0, buyCount);
    _doc["buy_count"] = buyCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

}

int VIT::getTotalVIT()
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

    baseVIT = DICTOOL->getIntValue_json(_doc, "base");
    plusVIT = DICTOOL->getIntValue_json(_doc, "plus");

    return baseVIT+plusVIT;
}
