//
//  Plot.cpp
//  life
//
//  Created by ff on 17/10/12.
//
//

#include "Plot.h"
#include "utils/GameReader.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Plot::configIdTable;

Plot::Plot():
fileId(0),
segId(0),
currPlotId(0),
synCount(0)
{
    configIdTable.clear();
    plotMap.clear();
    segPlotVec.clear();
}

Plot::~Plot()
{
    
}

void Plot::readPlotConfig(int fileId, int segId)
{
    if (fileId != this->fileId) {
        configIdTable.clear();
    }
    if (fileId != this->fileId || segId != this->segId) {
        plotMap.clear();
        segPlotVec.clear();
        this->fileId = fileId;
        this->segId = segId;
        currPlotId = 0;
        synCount = 0;
    }else{
        return;
    }
    
    auto doc = GameReader::getDocInstance(GameUtils::format(PLOT_CONFIG_FILE, fileId));
    if (configIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", configIdTable);
    }
    
    for (int i = 0; i < configIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        
        PLOT_CONFIG config;
        config.plotId = DICTOOL->getIntValue_json(dic, "id");
        config.segId = DICTOOL->getIntValue_json(dic, "seg");
        if (config.segId != segId) {
            continue;
        }
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.syn = DICTOOL->getIntValue_json(dic, "syn");
        for (int i = 0; i < 5; i++) {
            config.param[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("param%d", i+1).c_str());
        }
        plotMap[config.plotId] = config;
        segPlotVec.push_back(config.plotId);
    }
}

bool Plot::getNextStep(std::vector<int> &plotIds)
{
    plotIds.clear();
    synCount = 0;
    if (currPlotId >= segPlotVec.size())
    {
        return false;
    }
    
    for (; currPlotId < segPlotVec.size(); ) {
        plotIds.push_back(segPlotVec[currPlotId]);
        synCount++;
        currPlotId++;
        if (plotMap[segPlotVec[currPlotId-1]].syn == 0) {
            break;
        }
    }
    return true;
}

bool Plot::completSynPlot()
{
    synCount--;
    if (synCount <= 0) {
        synCount = 0;
        return true;
    }
    
    return false;
}
