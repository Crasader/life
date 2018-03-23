//
//  Plot.h
//  life
//
//  Created by ff on 17/10/12.
//
//

#ifndef Plot_h
#define Plot_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __PLOT_CONFIG
{
    int plotId;
    int segId;
    int type;
    int syn;
    int param[5];
}PLOT_CONFIG;

class Plot : public Singleton<Plot> {
public:
    Plot();
    ~Plot();
    
    void readPlotConfig(int fileId, int segId);
    bool getNextStep(std::vector<int> &plotIds);
    bool completSynPlot();
public:
    static std::map<int, int> configIdTable;
    std::map<int, PLOT_CONFIG> plotMap;
    std::vector<int> segPlotVec;
private:
    CC_SYNTHESIZE_READONLY(int, segId, SegId);
    CC_SYNTHESIZE_READONLY(int, fileId, FileId);
    CC_SYNTHESIZE_READONLY(int, currPlotId, CurrPlotId);
    CC_SYNTHESIZE_READONLY(int, synCount, SynCount);
};

#endif /* Plot_h */
