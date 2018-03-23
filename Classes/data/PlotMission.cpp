//
//  PlotMission.cpp
//  life
//
//  Created by ff on 17/10/9.
//
//

#include "PlotMission.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> PlotMission::missionConfigIdTable;
std::map<int, int> PlotMission::npcConfigIdTable;

PlotMission::PlotMission():
coreStr(""),
plotId(0),
plotFileId(0)
{
    npcConfigIdTable.clear();
    npcConfigMap.clear();
    missionConfigIdTable.clear();
    missionConfigMap.clear();
    missionMap.clear();
    activeMission.clear();
}

PlotMission::~PlotMission()
{
    
}

void PlotMission::readConfig()
{
    auto doc = GameReader::getDocInstance(PLOT_MISSION_CONFIG_FILE);
    if (missionConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", missionConfigIdTable);
    }
    
    missionConfigMap.clear();
    for (int i = 0; i < missionConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PLOT_MISSION_CONFIG config;
        config.missionid = DICTOOL->getIntValue_json(dic, "id");
        config.title = DICTOOL->getStringValue_json(dic, "title");
        config.descript = DICTOOL->getStringValue_json(dic, "descript");
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.display = DICTOOL->getIntValue_json(dic, "display");
        config.open = DICTOOL->getIntValue_json(dic, "open");
        config.triggerCount = DICTOOL->getIntValue_json(dic, "trigger_count");
        for (int i = 0; i < 3; i++) {
            config.triggerType[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("trigger%d_type", i+1).c_str());
            config.flowMissionId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_mission_id", i+1).c_str());
            config.flowPlotId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_plot_id", i+1).c_str());
            config.flowPlotFileId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_plot_file", i+1).c_str());
            config.flowThresType[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_thres_type", i+1).c_str());
            config.flowThresId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_thres_id", i+1).c_str());
            config.flowThresVal[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("flow%d_thres_val", i+1).c_str());
            for (int j = 0; j < 5; j++) {
                config.triggerParam[i][j] = DICTOOL->getIntValue_json(dic, GameUtils::format("trigger%d_param%d", i+1, j+1).c_str());
            }
        }
        
        
        config.plotId = DICTOOL->getIntValue_json(dic, "plot_id");
        config.plotFileId = DICTOOL->getIntValue_json(dic, "plot_bin");
        config.preMissionCount = DICTOOL->getIntValue_json(dic, "pre_mission_count");
        config.flowType = DICTOOL->getIntValue_json(dic, "flow_type");
        config.flowCount = DICTOOL->getIntValue_json(dic, "flow_count");
        config.saveMissionId = DICTOOL->getIntValue_json(dic, "save_mission_id");
        missionConfigMap[config.missionid] = config;
    }
    return;
}

void PlotMission::readNPCConfig()
{
    auto doc = GameReader::getDocInstance(NPC_CONFIG_FILE);
    if (npcConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", npcConfigIdTable);
    }
    
    npcConfigMap.clear();
    for (int i = 0; i < npcConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PLOT_NPC_CONFIG config;
        config.npcId = DICTOOL->getIntValue_json(dic, "id");
        config.armaFile = DICTOOL->getStringValue_json(dic, "csb");
        npcConfigMap[config.npcId] = config;
    }
}

void PlotMission::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "plot_mission_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void PlotMission::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "plot_mission_info.json";
    
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

bool PlotMission::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "plot_mission_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool PlotMission::parseJson()
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
//    log("%s", tempStr.c_str());
    activeMission.clear();
    int saveId = 0;
    for(int i = 0; i < _doc.Size(); i++)
    {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        
        PLOT_MISSION_DATA data;
        data.missionId = DICTOOL->getIntValue_json(dic, "mission_id");
        data.state = DICTOOL->getIntValue_json(dic, "state");
        data.preMissionCount = DICTOOL->getIntValue_json(dic, "pre_mission_count");
        data.completeTriggerCount = DICTOOL->getIntValue_json(dic, "complete_trigger_count");
        data.flowIdx = DICTOOL->getIntValue_json(dic, "flow_idx");
        for (int i = 0; i < 3; i++) {
            data.triggerState[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("trigger%d_state", i+1).c_str());
            data.selFlow[i] =  DICTOOL->getIntValue_json(dic, GameUtils::format("sel%d_flow", i+1).c_str());
        }
        missionMap[data.missionId] = data;
        if (data.state != 0) {
            missionMap[data.missionId].state = 0;
            _doc[missionConfigIdTable[data.missionId]]["state"] = 0;
            saveId = missionConfigMap[data.missionId].saveMissionId;
            activeMission.push_back(saveId);
        }
    }
    
    for (auto missionId : activeMission) {
        missionMap[missionId].state = 1;
        _doc[missionConfigIdTable[missionId]]["state"] = 1;
    }
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
//    log("%s", s.c_str());
    return true;
}

void PlotMission::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : missionConfigMap) {
        
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("mission_id", config.second.missionid, allocator);
        object.AddMember("state", config.second.open, allocator);
        object.AddMember("pre_mission_count", config.second.preMissionCount, allocator);
        object.AddMember("trigger1_state", 0, allocator);
        object.AddMember("trigger2_state", 0, allocator);
        object.AddMember("trigger3_state", 0, allocator);
        object.AddMember("complete_trigger_count", 0, allocator);
        object.AddMember("flow_idx", -1, allocator);
        object.AddMember("sel1_flow", -1, allocator);
        object.AddMember("sel2_flow", -1, allocator);
        object.AddMember("sel3_flow", -1, allocator);

        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

bool PlotMission::completeTrigger(int missionId, int triggerId, bool save)
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

    if (missionMap.find(missionId) == missionMap.end()) {
        return false;
    }
    if (triggerId <= 0 || triggerId > 3) {
        return false;
    }
    

    //避免重复触发
    if (missionMap[missionId].triggerState[triggerId-1] == 1) {
        return false;
    }
    
    if (missionMap[missionId].state != 1) {
        return false;
    }
    
    missionMap[missionId].triggerState[triggerId-1] = 1;
    if (save) {
        _doc[missionConfigIdTable[missionId]][GameUtils::format("trigger%d_state", triggerId).c_str()] = missionMap[missionId].triggerState[triggerId-1];
    }
    
    
    //统计触发条件完成数
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (missionMap[missionId].triggerState[i] == 1) {
            count++;
        }
    }
    
    if (count != missionMap[missionId].completeTriggerCount) {
        missionMap[missionId].completeTriggerCount = count;
        _doc[missionConfigIdTable[missionId]]["complete_trigger_count"] = missionMap[missionId].completeTriggerCount;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    
    //前继任务未全部完成，不可完成这个任务
    if (missionMap[missionId].preMissionCount > 0) {
        return false;
    }
    if (missionMap[missionId].completeTriggerCount >= missionConfigMap[missionId].triggerCount) {
        return completeMission(missionId);
    }
    
    return false;
}

bool PlotMission::completeMission(int missionId)
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

    //告诉后续任务已完成此任务，以便后续任务完成
    for (int i = 0; i < 3; i++) {
        int nextMissionId = missionConfigMap[missionId].flowMissionId[i];
        if (missionMap.find(nextMissionId) == missionMap.end()) {
            continue;
        }
        missionMap[nextMissionId].preMissionCount--;
        missionMap[nextMissionId].preMissionCount = MAX(0, missionMap[nextMissionId].preMissionCount);
    }
    
    plotFileId = 0;
    if (missionConfigMap[missionId].plotFileId != 0) {
        //如果有触发后的剧情，先进行剧情
        missionMap[missionId].state = 2;
        plotFileId = missionConfigMap[missionId].plotFileId;
        plotId = missionConfigMap[missionId].plotId;
    } else {
        //流向类型为直接流向，判断开启任务剧情，开启所有后续任务。否则，由外部综合判断应打开什么任务。
        if (missionConfigMap[missionId].flowType == 1) {
            //找到开启任务剧情
            int plotFileIdx = -1;
            for (int i = 0; i < 3; i++) {
                if (missionConfigMap[missionId].flowPlotFileId[2-i] != 0 && missionConfigMap[missionId].flowMissionId[2-i] != 0) {
                    plotFileIdx = 2-i;
                    break;
                }
            }
            if (plotFileIdx != -1) {
                //进行开启任务剧情
                missionMap[missionId].state = 4;
                plotFileId = missionConfigMap[missionId].flowPlotFileId[plotFileIdx];
                plotId = missionConfigMap[missionId].flowPlotId[plotFileIdx];
            }else{
                //开启后续任务
                for (int i = 0; i < 3; i++) {
                    if (missionConfigMap[missionId].flowMissionId[i] != 0) {
                        openMission(missionConfigMap[missionId].flowMissionId[i], _doc);
                    }
                }
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
        }else{
            //由外部综合判断应打开什么任务
            missionMap[missionId].state = 3;
        }
    }
    
    _doc[missionConfigIdTable[missionId]]["state"] = missionMap[missionId].state;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
//    log("%s", s.c_str());
    return true;
}

void PlotMission::completePlot(int missionId)
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
    
    if(!_doc.IsArray())
    {
        return;
    }

    plotFileId = 0;
    if (missionMap[missionId].state == 2) {
        //流向类型为直接流向，判断开启任务剧情，开启所有后续任务。否则，由外部综合判断应打开什么任务。
        if (missionConfigMap[missionId].flowType == 1) {
            //找到开启任务剧情
            int plotFileIdx = 0;
            for (int i = 0; i < 3; i++) {
                if (missionConfigMap[missionId].flowPlotFileId[2-i] != 0) {
                    plotFileIdx = missionConfigMap[missionId].flowPlotFileId[2-i];
                }
            }
            
            if (plotFileIdx != 0) {
                //进行开启任务剧情
                missionMap[missionId].state = 4;
                plotFileId = missionConfigMap[missionId].flowPlotFileId[plotFileIdx];
                plotId = missionConfigMap[missionId].flowPlotId[plotFileIdx];
            }else{
                //开启后续任务
                for (int i = 0; i < 3; i++) {
                    if (missionConfigMap[missionId].flowMissionId[i] != 0) {
                        openMission(missionConfigMap[missionId].flowMissionId[i], _doc);
                    }
                }
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
        }else{
            missionMap[missionId].state = 3;
        }
    }else if (missionMap[missionId].state == 4){
        switch (missionConfigMap[missionId].flowType) {
            case 1:
            {
                //如果是完成的是开启任务剧情，则开启后续任务，关闭此任务。
                for (int i = 0; i < 3; i++) {
                    if (missionConfigMap[missionId].flowMissionId[i] != 0) {
                        openMission(missionConfigMap[missionId].flowMissionId[i], _doc);
                    }
                }
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
                break;
            case 2:
            {
                //开启后续任务
                openMission(missionConfigMap[missionId].flowMissionId[missionMap[missionId].flowIdx], _doc);
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
                break;
            case 3:
            case 4:
            {
                //开启后续任务
                for (int i = 0; i < 3; i++) {
                    if (missionConfigMap[missionId].flowMissionId[i] != 0 &&
                            missionMap[missionId].selFlow[i] != 0) {
                        openMission(missionConfigMap[missionId].flowMissionId[i], _doc);
                    }
                }
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
                break;
            default:
                break;
        }
        
    }
    
    _doc[missionConfigIdTable[missionId]]["state"] = missionMap[missionId].state;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
//    log("%s", s.c_str());
}

void PlotMission::openMission(int missionId, rapidjson::Document &_doc)
{
    if (missionMap[missionId].state == 0) {
        missionMap[missionId].state = 1;
        _doc[missionConfigIdTable[missionId]]["state"] = missionMap[missionId].state;
        activeMission.push_back(missionId);
    }
}

void PlotMission::confirmNextMission(int missionId, int selFlow[3])
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
    
    if(!_doc.IsArray())
    {
        return;
    }
    plotFileId = 0;
    switch (missionConfigMap[missionId].flowType) {
        case 2:
        {
            int flowIdx = -1;
            for (int i = 0; i < 3; i++) {
                if (selFlow[i] == 1) {
                    flowIdx = i;
                    break;
                }
            }
            missionMap[missionId].flowIdx = flowIdx;
            
            if (flowIdx == -1) {
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }else{
                if (missionConfigMap[missionId].flowPlotFileId[flowIdx] != 0) {
                    missionMap[missionId].state = 4;
                    plotId = missionConfigMap[missionId].flowPlotFileId[flowIdx];
                    plotFileId = missionConfigMap[missionId].flowPlotId[flowIdx];
                }else{
                    //开启后续任务
                    openMission(missionConfigMap[missionId].flowMissionId[flowIdx], _doc);
                    //关闭此任务
                    missionMap[missionId].state = 0;
                    for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                        if (*iter == missionId) {
                            iter = activeMission.erase(iter);
                        }else{
                            iter++;
                        }
                    }
                }
            }
        }
            break;
        case 3:
        case 4:
        {
            //找到开启任务剧情
            int plotFileIdx = 0;
            for (int i = 0; i < 3; i++) {
                if (missionConfigMap[missionId].flowPlotFileId[2-i] != 0 && selFlow[2-i] != 0) {
                    plotFileIdx = missionConfigMap[missionId].flowPlotFileId[2-i];
                    break;
                }
            }
            memcpy(missionMap[missionId].selFlow, selFlow, sizeof(int)*3);
            if (plotFileIdx != 0) {
                //进行开启任务剧情
                missionMap[missionId].state = 4;
                plotId = missionConfigMap[missionId].flowPlotFileId[plotFileIdx];
                plotFileId = missionConfigMap[missionId].flowPlotId[plotFileIdx];
            }else{
                //开启后续任务
                for (int i = 0; i < 3; i++) {
                    if (missionConfigMap[missionId].flowMissionId[i] != 0 && selFlow[i] != 0) {
                        openMission(missionConfigMap[missionId].flowMissionId[i], _doc);
                    }
                }
                //关闭此任务
                missionMap[missionId].state = 0;
                for (auto iter = activeMission.begin(); iter != activeMission.end(); ) {
                    if (*iter == missionId) {
                        iter = activeMission.erase(iter);
                    }else{
                        iter++;
                    }
                }
            }
        }
            break;
            
        default:
            break;
    }

    _doc[missionConfigIdTable[missionId]]["state"] = missionMap[missionId].state;
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
//    log("%s", s.c_str());
}
