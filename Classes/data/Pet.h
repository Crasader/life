//
//  Pet.h
//  life
//
//  Created by ff on 17/7/28.
//
//

#ifndef Pet_h
#define Pet_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __PET_CONFIG
{
    int                 petId;
    std::string         name;
    std::string         icon;
    std::string         image;
    std::string         pvpImg;
    std::string         skillName;
    int                 fighterId;
    int                 skillId;
    int                 relation[2];
    int                 quality;
}PET_CONFIG;

typedef struct __PET_LEVEL_CONFIG
{
    int                 levelId;
    int                 qualityId;
    int                 level;
    int                 costFragment;
    int                 deltaHp;
    int                 deltaAttack;
    int                 deltaDefence;
    int                 provHp;
    int                 provCritRate;
    int                 provCirtPercent;
}PET_LEVEL_CONFIG;

typedef struct __PET_DATA
{
    int                 petId;
    int                 level;
    int                 fragmentCount;
}PET_DATA;

typedef struct __PET_INFO
{
    int                 hp;
    int                 attack;
    int                 defence;
    int                 fightPet[3];
}PET_INFO;

class Pet : public Singleton<Pet> {
public:
    Pet();
    ~Pet();
    
    void readPetConfigFile();
    void readLevelConfigFile();
    
    void deleteJson();
    bool readJson();
    bool parseJson();
    void save2Json();
    void save2EmptyCoreStr();
    
    void deletePetJson();
    bool readPetJson();
    bool parsePetJson();
    void save2PetJson();
    void save2EmptyPetStr();
    
    int getFightPetCount();
    int addFightPet(int petId);
    bool removeFightPet(int pos);
    
    int uplevel(int petId);
    bool isMaxLevel(int petId);
    int getLevelId(int quality, int petLevel);
    int checkFragment(int petId);
    int getNextLevelId(int petId);
    bool addFragment(int petId, int count);
    
    void getRelations(int petId, std::vector<int> &relation);
    void getPetsWithSameRelation(int relationId, std::vector<int> &petsId);
    void getPetsWithQuality(int quality, std::vector<int> &petsId);
    
    void checkTip();
public:
    static std::map<int, int> petConfigIdTable;
    static std::map<int, int> levelConfigIdTable;
    
    std::map<int, PET_CONFIG> petConfigMap;
    std::map<int, PET_LEVEL_CONFIG> levelConfigMap;
    
    std::map<int, PET_DATA> petsMap;
    std::map<int, int> maxLevel;
    
    std::vector<bool> uplevelTip;
private:
    CC_SYNTHESIZE_READONLY(PET_INFO, info, Info);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
    CC_SYNTHESIZE_READONLY(std::string, petsStr, PetsStr);
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
};
#endif /* Pet_h */
