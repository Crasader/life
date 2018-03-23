//
//  Equip.cpp
//  life
//
//  Created by ff on 17/7/21.
//
//

#include "Equip.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Equip::equipItemConfigIdTable;
std::map<int, int> Equip::equipLevelConfigIdTable;
std::map<int, int> Equip::equipConfigIdTable;

Equip::Equip():
coreStr(""),
itemStr(""),
tip(false)
{
    equipItemConfigIdTable.clear();
    equipLevelConfigIdTable.clear();
    equipConfigIdTable.clear();
    equipConfigMap.clear();
    equipLevelConfigMap.clear();
    itemMap.clear();
}

Equip::~Equip()
{
    
}

void Equip::readEquipConfigFile()
{
    auto doc = GameReader::getDocInstance(EQUIP_CONFIG_FILE);
    if (equipConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", equipConfigIdTable);
    }
    
    for (int i = 0; i < equipConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        EQUIP_CONFIG config;
        config.equipId = DICTOOL->getIntValue_json(dic, "id");
        config.pos = DICTOOL->getIntValue_json(dic, "pos");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.maxStar = DICTOOL->getIntValue_json(dic, "max_star");
        config.nextEquipId = DICTOOL->getIntValue_json(dic, "next");
        config.deltaStarHp = DICTOOL->getIntValue_json(dic, "star_hp");
        config.deltaStarAttack = DICTOOL->getIntValue_json(dic, "star_attack");
        config.deltaStarDefence = DICTOOL->getIntValue_json(dic, "star_defence");
        for (int i = 0; i < 6; i++) {
            config.starItem[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("star_item%d", i+1).c_str());
            config.starItemCount[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("star_item%d_count", i+1).c_str());
        }
        equipConfigMap[config.equipId] = config;
    }
}

void Equip::readEquipLevelConfigFile()
{
    auto doc = GameReader::getDocInstance(EQUIP_LEVEL_CONFIG_FILE);
    if (equipLevelConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", equipLevelConfigIdTable);
    }
    
    memset(maxLevel, 0, sizeof(int)*6);
    for (int i = 0; i < equipLevelConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        EQUIP_LEVEL_CONFIG config;
        config.levelId = DICTOOL->getIntValue_json(dic, "id");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.pos = DICTOOL->getIntValue_json(dic, "pos");
        config.costGold = DICTOOL->getIntValue_json(dic, "cost_gold");
        config.deltaHp = DICTOOL->getIntValue_json(dic, "hp");
        config.deltaAttack = DICTOOL->getIntValue_json(dic, "attack");
        config.deltaDefence = DICTOOL->getIntValue_json(dic, "defence");

        equipLevelConfigMap[config.levelId] = config;
        if (maxLevel[config.pos-1] < config.level) {
            maxLevel[config.pos-1] = config.level;
        }
    }

}

void Equip::readEquipItemConfigFile()
{
    auto doc = GameReader::getDocInstance(EQUIP_ITEM_CONFIG_FILE);
    if (equipItemConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", equipItemConfigIdTable);
    }
    
    for (int i = 0; i < equipItemConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        EQUIP_ITEM_CONFIG config;
        config.itemId = DICTOOL->getIntValue_json(dic, "id");
        config.price = DICTOOL->getIntValue_json(dic, "price");
        config.dropRate = DICTOOL->getIntValue_json(dic, "sweep_rate");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.dropGold = DICTOOL->getIntValue_json(dic, "sweep_gold");
        config.dropExp = DICTOOL->getIntValue_json(dic, "sweep_exp");
        config.dropCount = DICTOOL->getIntValue_json(dic, "sweep_count");
        
        equipItemConfigMap[config.itemId] = config;
    }
    
}

void Equip::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "equip.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool Equip::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "equip.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

void Equip::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "equip.json";
    
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

bool Equip::parseJson()
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
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    for (int i = 0; i < 6; i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        WEAR_DATA data;
        data.equipId = DICTOOL->getIntValue_json(dic, "equip_id");
        data.pos = DICTOOL->getIntValue_json(dic, "pos");
        data.star = DICTOOL->getIntValue_json(dic, "star");
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.hp = DICTOOL->getIntValue_json(dic, "hp");
        data.attack = DICTOOL->getIntValue_json(dic, "attack");
        data.defence = DICTOOL->getIntValue_json(dic, "defence");
        wearMap[data.pos] = data;
    }
    return true;
}

void Equip::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    int originEquipId[6] = {1, 6, 11, 16, 21, 26};
    int equipLevelId[6];
    memset(equipLevelId, 0, sizeof(int)*6);
    for (auto config : equipLevelConfigMap) {
        if(config.second.level != 1)
        {
            continue;
        }
        
        equipLevelId[config.second.pos-1] = config.second.levelId;
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (int i = 0; i < 6; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("equip_id", originEquipId[i], allocator);
        object.AddMember("pos", equipConfigMap[originEquipId[i]].pos, allocator);
        object.AddMember("star", 0, allocator);
        object.AddMember("level", 1, allocator);
        int levelId = equipLevelId[equipConfigMap[originEquipId[i]].pos-1];
        if (levelId == 0) {
            object.AddMember("hp", 0, allocator);
            object.AddMember("attack", 0, allocator);
            object.AddMember("defence", 0, allocator);
        }else{
            object.AddMember("hp", equipLevelConfigMap[levelId].deltaHp, allocator);
            object.AddMember("attack", equipLevelConfigMap[levelId].deltaAttack, allocator);
            object.AddMember("defence", equipLevelConfigMap[levelId].deltaDefence, allocator);
        }

        _doc.PushBack(object, allocator);
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
bool Equip::uplevel(int pos)
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
    
    if(!_doc.IsArray())
    {
        return false;
    }

    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        int equipPos = DICTOOL->getIntValue_json(dic, "pos");
        if (equipPos != pos) {
            continue;
        }
        int equipLevel = DICTOOL->getIntValue_json(dic, "level");
        int equipHp = DICTOOL->getIntValue_json(dic, "hp");
        int equipAttack = DICTOOL->getIntValue_json(dic, "attack");
        int equipDefence = DICTOOL->getIntValue_json(dic, "defence");
        
        
        int nextLevel = equipLevel+1;
        equipHp += equipLevelConfigMap[nextLevel].deltaHp;
        equipAttack += equipLevelConfigMap[nextLevel].deltaAttack;
        equipDefence += equipLevelConfigMap[nextLevel].deltaDefence;
        wearMap[pos].level = nextLevel;
        wearMap[pos].hp = equipHp;
        wearMap[pos].attack = equipAttack;
        wearMap[pos].defence = equipDefence;
        
        _doc[i]["level"] = nextLevel;
        _doc[i]["hp"] = equipHp;
        _doc[i]["attack"] = equipAttack;
        _doc[i]["defence"] = equipDefence;
        break;
    }
    
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

bool Equip::upgrade(int pos)
{
    if (itemStr == "") {
        return false;
    }
    
    rapidjson::Document _itemDoc;
    std::string tempItemStr = decode(itemStr.c_str());
    _itemDoc.Parse < 0 > (tempItemStr.c_str());
    if(_itemDoc.HasParseError())
    {
        return false;
    }
    
    if(!_itemDoc.IsArray())
    {
        return false;
    }
    
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
    
    if(!_doc.IsArray())
    {
        return false;
    }
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        int equipPos = DICTOOL->getIntValue_json(dic, "pos");
        if (equipPos != pos) {
            continue;
        }
        
        int equipId = DICTOOL->getIntValue_json(dic, "equip_id");
        int equipStar = DICTOOL->getIntValue_json(dic, "star");
        int equipHp = DICTOOL->getIntValue_json(dic, "hp");
        int equipAttack = DICTOOL->getIntValue_json(dic, "attack");
        int equipDefence = DICTOOL->getIntValue_json(dic, "defence");
        
        for (int i = 0; i < 6; i++) {
            int itemId = equipConfigMap[equipId].starItem[i];
            int requirCount = equipConfigMap[equipId].starItemCount[i];
            
            if (itemId <= 0) {
                continue;
            }
            itemMap[itemId].count -= requirCount;
            _itemDoc[equipItemConfigIdTable[itemId]]["count"] = itemMap[itemId].count;
        }

        equipHp += equipConfigMap[equipId].deltaStarHp;
        equipAttack += equipConfigMap[equipId].deltaStarAttack;
        equipDefence += equipConfigMap[equipId].deltaStarDefence;
        if (equipStar<3) {
            equipStar++;
        }else{
            equipStar = 0;
            equipId = equipConfigMap[equipId].nextEquipId;
        }
        
        wearMap[pos].equipId = equipId;
        wearMap[pos].star = equipStar;
        wearMap[pos].hp = equipHp;
        wearMap[pos].attack = equipAttack;
        wearMap[pos].defence = equipDefence;
        
        _doc[i]["equip_id"] = equipId;
        _doc[i]["star"] = equipStar;
        _doc[i]["hp"] = equipHp;
        _doc[i]["attack"] = equipAttack;
        _doc[i]["defence"] = equipDefence;
        break;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
    
    StringBuffer itemBuff;
    rapidjson::Writer<StringBuffer> writer1(itemBuff);
    _itemDoc.Accept(writer1);
    
    
    std::string item_str = StringUtils::format("%s", itemBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    itemStr = encode(item_str);
    save2ItemJson();

    return true;
}

bool Equip::addItem(int itemId, int value)
{
    if (itemStr == "") {
        return false;
    }
    
    rapidjson::Document _itemDoc;
    std::string tempItemStr = decode(itemStr.c_str());
    _itemDoc.Parse < 0 > (tempItemStr.c_str());
    if(_itemDoc.HasParseError())
    {
        return false;
    }
    
    if(!_itemDoc.IsArray())
    {
        return false;
    }
    int index = equipItemConfigIdTable[itemId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_itemDoc, index);
    itemMap[itemId].count = DICTOOL->getIntValue_json(dic, "count");
    itemMap[itemId].count += value;
    _itemDoc[equipItemConfigIdTable[itemId]]["count"] = itemMap[itemId].count;
    
    StringBuffer itemBuff;
    rapidjson::Writer<StringBuffer> writer1(itemBuff);
    _itemDoc.Accept(writer1);
    
    
    std::string item_str = StringUtils::format("%s", itemBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    itemStr = encode(item_str);
    save2ItemJson();

    return true;
}

int Equip::getTotalHp()
{
    int total = 0;
    for (auto wearEquip : wearMap) {
        total += wearEquip.second.hp;
    }
    
    return total;
}

int Equip::getTotalAttack()
{
    int total = 0;
    for (auto wearEquip : wearMap) {
        total += wearEquip.second.attack;
    }
    
    return total;
}

int Equip::getTotalDefence()
{
    int total = 0;
    for (auto wearEquip : wearMap) {
        total += wearEquip.second.defence;
    }
    
    return total;
}

bool Equip::isMaxLevel(int pos)
{
    
    if (wearMap[pos].level >= maxLevel[pos-1]) {
        return true;
    }
    
    return false;
}
bool Equip::isMaxGrade(int pos)
{
    if(wearMap[pos].star<3)
    {
        return false;
    }
    if (equipConfigMap[wearMap[pos].equipId].nextEquipId != 0) {
        return false;
    }
    return true;
}
bool Equip::checkUpgradeItem(int pos)
{
    int equipId = wearMap[pos].equipId;
    if(isMaxGrade(pos)){
        return false;
    }
    for (int i = 0; i < 6; i++) {
        int itemId = equipConfigMap[equipId].starItem[i];
        int requirCount = equipConfigMap[equipId].starItemCount[i];
        
        if (itemId <= 0) {
            continue;
        }
        if(itemMap[itemId].count < requirCount)
        {
            return false;
        }
    }
    
    return true;
}
void Equip::deleteItemJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "equip_item.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool Equip::readItemJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "equip_item.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    itemStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseItemJson()) {
        return false;
    }
    
    return true;
}

void Equip::save2ItemJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "equip_item.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(itemStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Equip::parseItemJson()
{
    if (itemStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(itemStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        EQUIP_ITEM_DATA data;
        data.itemId = DICTOOL->getIntValue_json(dic, "item_id");
        data.count = DICTOOL->getIntValue_json(dic, "count");
        itemMap[data.itemId] = data;
    }
    return true;
}

void Equip::save2EmptyItemStr()
{
    rapidjson::Document _doc;
    if (itemStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : equipItemConfigMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("item_id", config.second.itemId, allocator);
        object.AddMember("count", 0, allocator);
        
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    itemStr = encode(s);
    
    save2ItemJson();
}

void Equip::checkTip(int accountLevel)
{
    tip = false;
    posTip.clear();
    posUplevelTip.clear();
    posUpgradeTip.clear();
    for (int i = 0; i < 6; i++) {
        posTip.push_back(false);
        posUplevelTip.push_back(false);
        posUpgradeTip.push_back(false);
        if (wearMap[i+1].level < accountLevel) {
            tip = true;
            posTip[i] = true;
            posUplevelTip[i] = true;
        }
        if (checkUpgradeItem(i+1)) {
            tip = true;
            posTip[i] = true;
            posUpgradeTip[i] = true;
        }
    }
}
