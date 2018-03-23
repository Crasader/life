//
//  Dialog.cpp
//  life
//
//  Created by ff on 17/10/16.
//
//

#include "Dialog.h"
#include "utils/GameReader.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Dialog::lineConfigIdTable;
std::map<int, int> Dialog::segConfigIdTable;
std::map<int, int> Dialog::groupConfigIdTable;

Dialog::Dialog():
lineId(-1),
segId(-1),
groupId(-1),
fileId(-1)
{
    lineConfigIdTable.clear();
    segConfigIdTable.clear();
    groupConfigIdTable.clear();
}

Dialog::~Dialog()
{
    
}

void Dialog::readDialogConfig(int fileId, int groupId)
{
    if (fileId != this->fileId) {
        groupConfigIdTable.clear();
        segConfigIdTable.clear();
        lineConfigIdTable.clear();
        this->fileId = fileId;
    }else if (groupId != this->groupId) {
        segConfigIdTable.clear();
        lineConfigIdTable.clear();
    }else{
        return;
    }
    
    if (groupConfigIdTable.empty()) {
        auto doc = GameReader::getDocInstance(GameUtils::format(DIALOG_GROUP_CONFIG_FILE, fileId));
        GameReader::initIdTable(*doc, "dialogId", groupConfigIdTable);
        for (int i = 0; i < groupConfigIdTable.size(); i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
            
            DIALOG_GROUP_CONFIG config;
            config.groupId = DICTOOL->getIntValue_json(dic, "dialogId");
            config.segCount = DICTOOL->getIntValue_json(dic, "segCount");
            config.segStartId = DICTOOL->getIntValue_json(dic, "startSeg");
            groupMap[config.groupId] = config;
        }
    }
    
    this->groupId = groupId;
    segId = groupMap[groupId].segStartId;
    int segCount = groupMap[groupId].segCount;
    if (segConfigIdTable.empty()) {
        auto doc = GameReader::getDocInstance(GameUtils::format(DIALOG_SEG_CONFIG_FILE, fileId));
        GameReader::initIdTable(*doc, "segId", segConfigIdTable);
        
        for (int i = 0; i < segConfigIdTable.size(); i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
            
            DIALOG_SEG_CONFIG config;
            config.segId = DICTOOL->getIntValue_json(dic, "segId");
            config.faceIcon = DICTOOL->getStringValue_json(dic, "face");
            std::string dir = DICTOOL->getStringValue_json(dic, "direct");
            config.direct = (dir=="1"?1:0);
            config.dialogName = DICTOOL->getStringValue_json(dic, "dialogName");
            config.startLine = DICTOOL->getIntValue_json(dic, "startLine");
            config.lineCount = DICTOOL->getIntValue_json(dic, "lineCount");
            segMap[config.segId] = config;
        }
    }
    
    lineId = segMap[segId].startLine;
    if (lineConfigIdTable.empty()) {
        auto doc = GameReader::getDocInstance(GameUtils::format(DIALOG_LINE_CONFIG_FILE, fileId));
        GameReader::initIdTable(*doc, "lineId", lineConfigIdTable);
        
        for (int i = 0; i < lineConfigIdTable.size(); i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
            int lineId = DICTOOL->getIntValue_json(dic, "lineId");
            if (lineId < segMap[segId].startLine || lineId >= segMap[segId+segCount-1].startLine+segMap[segId+segCount-1].lineCount) {
                continue;
            }
            
            DIALOG_LINE_CONFIG config;
            config.lineId = DICTOOL->getIntValue_json(dic, "lineId");
            config.content = DICTOOL->getStringValue_json(dic, "content");
            lineMap[config.lineId] = config;
        }
    }
}

bool Dialog::getNextLine()
{
    if (lineId == segMap[segId].startLine+segMap[segId].lineCount-1) {
        if (segId == groupMap[groupId].segStartId+groupMap[groupId].segCount-1) {
            return false;
        }else{
            segId++;
            lineId = segMap[segId].startLine;
            return true;
        }
    }else{
        lineId++;
    }
    
    
    return true;
}
