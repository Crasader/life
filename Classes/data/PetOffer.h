//
//  PetOffer.h
//  life
//
//  Created by ff on 17/8/2.
//
//

#ifndef PetOffer_h
#define PetOffer_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __PET_OFFER_CONFIG
{
    int                     offerId;
    int                     type;
    int                     petCount;
    int                     costHours;
    int                     price;
    int                     boundLevel;
    int                     boundType[3];
    int                     boundId[3];
    int                     boundCount[3];
}PET_OFFER_CONFIG;

typedef struct __PET_OFFER_INFO
{
    int                     offerId;
    int                     petId[3];
    int                     state;
}PET_OFFER_INFO;

typedef struct _PET_OFFER_DATA
{
    int                     refreshCount;
}PET_OFFER_DATA;

class PetOffer : public Singleton<PetOffer> {
public:
    PetOffer();
    ~PetOffer();
    void readConfig();
    
    void deleteBoardJson();
    bool readBoardJson();
    bool parseBoardJson();
    void save2BoardJson();
    void save2EmptyBoardStr();
    
    void deleteDataJson();
    bool readDataJson();
    bool parseDataJson();
    void save2DataJson();
    void save2EmptyDataStr();
    
    void refreshBoardOffers();
    void refreshOptBoardOffers();
    void randOffers();
    void randOptOffers();
    
    void deleteWorkJson();
    bool readWorkJson();
    bool parseWorkJson();
    void save2WorkJson();
    void save2EmptyWorkStr();

    
    int takeOffer(int pos, int petId[3]);
    int overWork(int pos);
    int takeReward(int pos);
    int getWorkOfferCount();
    void getWorkPetsId(std::vector<int> &petsId);
    
    void resetRefreshCount();
    void checkTip();
public:
    static std::map<int, int> configIdTable;
    
    std::map<int, PET_OFFER_CONFIG> configMap;
    std::map<int, PET_OFFER_INFO> workOffers;
    std::map<int, PET_OFFER_INFO> boardOffer;
    
    PET_OFFER_DATA offerData;
private:
    std::vector<int> totalOfferVec;
    std::vector<int> optOfferVec;
    std::vector<int> optOfferId;
    
    CC_SYNTHESIZE_READONLY(std::string, dataStr, DataStr);
    CC_SYNTHESIZE_READONLY(std::string, boardStr, BoardStr);
    CC_SYNTHESIZE_READONLY(std::string, workStr, WorkStr);
    CC_SYNTHESIZE_READONLY(bool, tip, Tip);
    
    CC_SYNTHESIZE_READONLY(int, workPos, WorkPos);

};
#endif /* PetOffer_h */
