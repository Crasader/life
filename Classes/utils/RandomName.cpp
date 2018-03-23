//
//  RandomName.cpp
//  life
//
//  Created by ff on 17/9/18.
//
//

#include "RandomName.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> RandomName::part1IdTable;
std::map<int, int> RandomName::part2IdTable;

RandomName::RandomName()
{
    part1IdTable.clear();
    part1Map.clear();
    part2IdTable.clear();
    part2Map.clear();
}

RandomName::~RandomName()
{
    part1IdTable.clear();
    part1Map.clear();
    part2IdTable.clear();
    part2Map.clear();
}

void RandomName::readPart1Config()
{
    auto doc = GameReader::getDocInstance(NAME_PART1_FILE);
    if (part1IdTable.empty()) {
        GameReader::initIdTable(*doc, "id", part1IdTable);
    }
    
    part1Map.clear();
    
    for (int i = 0; i < part1IdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        int idx = DICTOOL->getIntValue_json(dic, "id");
        std::string name = DICTOOL->getStringValue_json(dic, "name");
        part1Map[idx] = name;
    }
    log("RandomName::readPart1config() finished!");
}

void RandomName::readPart2Config()
{
    auto doc = GameReader::getDocInstance(NAME_PART2_FILE);
    if (part2IdTable.empty()) {
        GameReader::initIdTable(*doc, "id", part2IdTable);
    }
    
    part2Map.clear();
    
    for (int i = 0; i < part2IdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        int idx = DICTOOL->getIntValue_json(dic, "id");
        std::string name = DICTOOL->getStringValue_json(dic, "name");
        part2Map[idx] = name;
    }
    log("RandomName::readPart2Config() finished!");
}

std::string RandomName::getRandomName()
{
    std::string name("");
    int part1Size = (int)part1Map.size();
//    int part2Size = (int)part2Map.size();
    
    int part1Idx = abs(rand())%part1Size;
//    int part2Idx = abs(rand())%part2Size;
    name += part1Map[part1Idx+1];
//    name += part2Map[part2Idx+1];
    return name;
}
