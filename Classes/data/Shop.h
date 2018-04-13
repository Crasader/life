//
//  Shop.h
//  life
//
//  Created by ff on 17/11/8.
//
//

#ifndef Shop_h
#define Shop_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

typedef struct __SHOP_CONFIG
{
    int shopId;
    int type;
    std::string name;
    std::string icon;
    std::string packageImg;
    std::string packageTitle;
    int priceType;
    float price;
    int boundType;
    int boundId;
    int boundCount;
    std::string payId;
    std::string showRes;
}SHOP_CONFIG;

typedef struct __SHOP_PACKAGE_CONFIG
{
    int itemId;
    int packageId;
    int boundType;
    int boundId;
    int boundCount;
}SHOP_PACKAGE_CONFIG;

typedef struct __SHOP_DATA
{
    int firstRecharge;
    int noviceRecharge;
    int discountState;
    int discountPackage;
    float totalRecharge;
}SHOP_DATA;

class Shop : public Singleton<Shop> {
public:
    Shop();
    ~Shop();
    
    void readShopConfig();
    void readPackageConfig();
    
    void deleteJson();
    void save2Json();
    void save2EmptyCoreStr();
    bool readJson();
    bool parseJson();
    
    void recharge(int shopId);
    
    void getShopByType(int shopType, std::vector<int> &shopIdVec);
    void getPackageById(int packageId, std::vector<int> &packageIdVec);
    void takeFirstRechargeBound();
    void disableDiscount();
    void enableDiscount();
    void selDiscountPackage();
    
//    void closeDiscount();
public:
    static std::map<int, int> shopIdTable;
    static std::map<int, int> packageIdTable;
    
    std::map<int, SHOP_CONFIG> shopConfigMap;
    std::map<int, SHOP_PACKAGE_CONFIG> packageConfigMap;
    
private:
    CC_SYNTHESIZE_READONLY(SHOP_DATA, coreData, CoreData);
    CC_SYNTHESIZE_READONLY(std::string, coreStr, CoreStr);
};

#endif /* Shop_h */
