//
//  Shop.cpp
//  life
//
//  Created by ff on 17/11/8.
//
//

#include "Shop.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"
#include "utils/TimeUtil.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Shop::shopIdTable;
std::map<int, int> Shop::packageIdTable;

Shop::Shop():
coreStr("")
{
    
}

Shop::~Shop()
{
    
}

void Shop::readShopConfig()
{
    auto doc = GameReader::getDocInstance(SHOP_CONFIG_FILE);
    if (shopIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", shopIdTable);
    }
    
    shopConfigMap.clear();
    
    for (int i = 0; i < shopIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        SHOP_CONFIG config;
        config.shopId = DICTOOL->getIntValue_json(dic, "id");
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.packageImg = DICTOOL->getStringValue_json(dic, "package_img");
        config.packageTitle = DICTOOL->getStringValue_json(dic, "title_img");
        config.priceType = DICTOOL->getIntValue_json(dic, "price_type");
        config.price = DICTOOL->getFloatValue_json(dic, "price");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        config.payId = DICTOOL->getStringValue_json(dic, "pay_id");
        
        shopConfigMap[config.shopId] = config;
    }
    return;
}

void Shop::readPackageConfig()
{
    auto doc = GameReader::getDocInstance(SHOP_PACKAGE_CONFIG_FILE);
    if (packageIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", packageIdTable);
    }
    
    packageConfigMap.clear();
    
    for (int i = 0; i < packageIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        SHOP_PACKAGE_CONFIG config;
        config.itemId = DICTOOL->getIntValue_json(dic, "id");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        
        packageConfigMap[config.itemId] = config;
    }
    return;
}

void Shop::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "shop_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Shop::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "shop_info.json";
    
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

void Shop::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("first_recharge", 0, allocator);
    _doc.AddMember("novice_recharge", 0, allocator);
    _doc.AddMember("total_recharge", 0, allocator);
    _doc.AddMember("discount_state", 0, allocator);
    _doc.AddMember("discount_package", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

bool Shop::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "shop_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Shop::parseJson()
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
    
    
    coreData.firstRecharge = DICTOOL->getIntValue_json(_doc, "first_recharge");
    coreData.noviceRecharge = DICTOOL->getIntValue_json(_doc, "novice_recharge");
    coreData.totalRecharge = DICTOOL->getFloatValue_json(_doc, "total_recharge");
    coreData.discountState = DICTOOL->getIntValue_json(_doc, "discount_state");
    coreData.discountPackage = DICTOOL->getIntValue_json(_doc, "discount_package");
    log("parse Discount state:%d", coreData.discountState);
    return true;
}

void Shop::recharge(int shopId)
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

    
    if (shopConfigMap[shopId].type == 3) {
        coreData.noviceRecharge = DICTOOL->getIntValue_json(_doc, "novice_recharge");
        coreData.noviceRecharge = 1;
        _doc["novice_recharge"] = coreData.noviceRecharge;
    }
    if (shopConfigMap[shopId].type == 4) {
        coreData.firstRecharge = DICTOOL->getIntValue_json(_doc, "first_recharge");
        coreData.firstRecharge = 1;
        _doc["first_recharge"] = coreData.firstRecharge;
    }
//    if (shopConfigMap[shopId].type == 6) {
//        coreData.discountState = DICTOOL->getIntValue_json(_doc, "discount_state");
//        coreData.discountState = 2;
//        _doc["discount_state"] = coreData.discountState;
//    }

    
    if (shopConfigMap[shopId].priceType == 1) {
        coreData.totalRecharge = DICTOOL->getFloatValue_json(_doc, "total_recharge");
        coreData.totalRecharge += shopConfigMap[shopId].price;
        _doc["total_recharge"] = coreData.totalRecharge;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
void Shop::disableDiscount()
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
//    coreData.discountState = DICTOOL->getIntValue_json(_doc, "discount_state");
//    if (coreData.discountState == 1) {
//        return;
//    }
    coreData.discountState = 0;
    _doc["discount_state"] = coreData.discountState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
void Shop::enableDiscount()
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
    
//    coreData.discountState = DICTOOL->getIntValue_json(_doc, "discount_state");
//    if (coreData.discountState == 2) {
//        return;
//    }
    coreData.discountState = 1;
    _doc["discount_state"] = coreData.discountState;

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
    
    selDiscountPackage();
}

void Shop::selDiscountPackage()
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
    
    std::vector<int> shopIdVec;
    getShopByType(6, shopIdVec);
    srand((unsigned)TimeUtil::get_system_tick_s());
    int index = abs(rand())%shopIdVec.size();
    coreData.discountPackage = shopIdVec[index];
    _doc["discount_package"] = coreData.discountPackage;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
//void Shop::closeDiscount()
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
//    coreData.discountState = DICTOOL->getIntValue_json(_doc, "discount_state");
//    coreData.discountState = 2;
//    _doc["discount_state"] = coreData.discountState;
//    
//    StringBuffer buff;
//    rapidjson::Writer<StringBuffer> writer(buff);
//    _doc.Accept(writer);
//    std::string s = StringUtils::format("%s", buff.GetString());
//    //    log("========================\n");
//    //    log("%s\n", s.c_str());
//    coreStr = encode(s);
//    save2Json();
//
//}

void Shop::getShopByType(int shopType, std::vector<int> &shopIdVec)
{
    shopIdVec.clear();
    for (auto config : shopConfigMap) {
        if (config.second.type == shopType) {
            shopIdVec.push_back(config.first);
        }
    }
}

void Shop::getPackageById(int packageId, std::vector<int> &packageIdVec)
{
    packageIdVec.clear();
    for (auto config : packageConfigMap) {
        if (config.second.packageId == packageId) {
            packageIdVec.push_back(config.first);
        }
    }
}

void Shop::takeFirstRechargeBound()
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

    coreData.firstRecharge = DICTOOL->getIntValue_json(_doc, "first_recharge");
    if (coreData.firstRecharge != 1) {
        return;
    }
    coreData.firstRecharge = 2;
    _doc["first_recharge"] = coreData.firstRecharge;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

}
