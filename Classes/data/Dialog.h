//
//  Dialog.h
//  life
//
//  Created by ff on 17/10/16.
//
//

#ifndef Dialog_h
#define Dialog_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __DIALOG_GROUP_CONFIG
{
    int groupId;
    int segCount;
    int segStartId;
}DIALOG_GROUP_CONFIG;

typedef struct __DIALOG_SEG_CONFIG
{
    int segId;
    std::string faceIcon;
    int direct;
    std::string dialogName;
    int startLine;
    int lineCount;
}DIALOG_SEG_CONFIG;

typedef struct __DIALOG_LINE_CONFIG
{
    int lineId;
    std::string content;
}DIALOG_LINE_CONFIG;

class Dialog : public Singleton<Dialog> {
public:
    Dialog();
    ~Dialog();
    
    void readDialogConfig(int fileId, int groupId);
    bool getNextLine();
public:
    static std::map<int, int> lineConfigIdTable;
    static std::map<int, int> segConfigIdTable;
    static std::map<int, int> groupConfigIdTable;
    
    std::map<int, DIALOG_LINE_CONFIG> lineMap;
    std::map<int, DIALOG_SEG_CONFIG> segMap;
    std::map<int, DIALOG_GROUP_CONFIG> groupMap;
private:
    CC_SYNTHESIZE_READONLY(int, lineId, LineId);
    CC_SYNTHESIZE_READONLY(int, segId, SegId);
    CC_SYNTHESIZE_READONLY(int, groupId, GroupId);
    CC_SYNTHESIZE_READONLY(int, fileId, FileId);
    
    CC_SYNTHESIZE(int, missionId, MissionId);
};
#endif /* Dialog_h */
