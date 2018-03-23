//
//  PlotMission.h
//  life
//
//  Created by ff on 17/10/9.
//
//

#ifndef PlotMission_h
#define PlotMission_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __PLOT_MISSION_CONFIG
{
    int missionid;
    std::string title;
    std::string descript;
    int type;
    int display;
    int open;
    int triggerCount;
    int triggerType[3];
    int triggerParam[3][5];
    int plotId;
    int plotFileId;
    int preMissionCount;
    int flowType;
    int flowCount;
    int flowMissionId[3];
    int flowPlotId[3];
    int flowPlotFileId[3];
    int flowThresType[3];
    int flowThresId[3];
    int flowThresVal[3];
    int saveMissionId;
}PLOT_MISSION_CONFIG;

typedef struct __PLOT_MISSION_DATA
{
    int missionId;
    int preMissionCount;
    int state;
    int triggerState[3];
    int completeTriggerCount;
    int flowIdx;
    int selFlow[3];
}PLOT_MISSION_DATA;

typedef struct __PLOT_NPC_CONFIG
{
    int npcId;
    std::string armaFile;
}PLOT_NPC_CONFIG;

class PlotMission : public Singleton<PlotMission> {
public:
    PlotMission();
    ~PlotMission();
    
    void readConfig();
    void readNPCConfig();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();

    bool completeTrigger(int missionId, int triggerId, bool save);
    bool completeMission(int missionId);
    void completePlot(int missionId);
    void confirmNextMission(int missionId, int selFlow[3]);
    void openMission(int missionId, rapidjson::Document &_doc);
public:
    static std::map<int, int> missionConfigIdTable;
    static std::map<int, int> npcConfigIdTable;
    std::map<int, PLOT_NPC_CONFIG> npcConfigMap;
    std::map<int, PLOT_MISSION_CONFIG> missionConfigMap;
    std::map<int, PLOT_MISSION_DATA> missionMap;
    std::vector<int> activeMission;
private:
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(int, plotId, PlotId);
    CC_SYNTHESIZE_READONLY(int, plotFileId, PlotFileId);
};

#endif /* PlotMission_h */
