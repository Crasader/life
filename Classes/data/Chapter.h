//
//  Chapter.h
//  life
//
//  Created by ff on 17/7/11.
//
//

#ifndef Chapter_h
#define Chapter_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __CHAPTER_CONFIG
{
    int                 chapterId;
    std::string         name;
    std::string         img;
    int                 nextId;
    int                 boxBoundType[9];
    int                 boxBoundCount[9];
}CHAPTER_CONFIG;

typedef struct __CHAPTER_SAVE_DATA
{
    int                 chapterId;
    int                 boxState[3];
    bool                open;
}CHAPTER_SAVE_DATA;

typedef struct __STAGE_CONFIG
{
    int                 stageId;
    std::string         name;
    std::string         imgFile;
    std::string         iconFile;
    std::string         musicFile;
    int                 chapterId;
    int                 boundsGold;
    int                 boundsExp;
    int                 firstGold;
    int                 firstExp;
    int                 starSecond;
    int                 starHpPer;
    int                 starUseBottle;
    int                 costVIT;
}STAGE_CONFIG;

typedef struct __STAGE_SAVE_DATA
{
    int                 stageId;
    int                 star[3];
    bool                open;
}STAGE_SAVE_DATA;

class Chapter : public Singleton<Chapter>
{
public:
    Chapter();
    ~Chapter();
    
    void deleteJson();
    void readConfigFile();
    bool readJson();
    void save2Json();
    bool parseJson();
    void save2EmptyCoreStr();
    
    void openChapter(int chapterId);
    int openStarBox(int chapterId, int boxId);
    void enableStarBox(int chapterId,int boxId);
    int getChapterStarCount(int chapterId);
    
    void readStageConfig(int chapterId);
    bool readStageJson(int chapterId);
    bool parseStageJson(int chapterId);
    void saveStageJson(int chapterId);
    void save2EmptyStageStr(int chapterId);
    
    bool enterChapter(int chapterId);
    void boundsStar(int chapterId, int stageId, int star[3]);
    void updateStageInfo(int chapterId, int stageId);
    
    std::string getChapterImg(int chapterId);
    int getMaxChapterId();

public:
    static std::map<int, int>configIdTable;
    
    static std::map<int, CHAPTER_CONFIG> configMap;
    static std::map<int, CHAPTER_SAVE_DATA> saveMap;
    
    static std::map<int, int>stageConfigIdTable;
    std::map<int, STAGE_CONFIG> stageConfigMap;
    std::map<int, STAGE_SAVE_DATA> stageSaveMap;
    
private:
    std::map<int, std::string> stageStrMap;
    std::map<int, int> stageStrIdTable;
    
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE(int, currChapter, CurrChapter);
};
#endif /* Chapter_h */
