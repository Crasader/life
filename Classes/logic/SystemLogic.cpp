//
//  SystemLogic.cpp
//  life
//
//  Created by ff on 17/7/21.
//
//

#include "SystemLogic.h"
#include "ClientLogic.h"
#include "TimeManager.h"
#include "GameCore.h"
#include "ChapterLogic.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSPayHelper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

USING_NS_CC;
using namespace cocostudio;

SystemLogic::SystemLogic():
currEquipPos(0),
currPetId(0),
currSwordId(0),
currArmsId(0),
everydayShow(0),
toHall(true),
popLayerId(0)
{
    
}

SystemLogic::~SystemLogic()
{
    
}

void SystemLogic::logic(float dt)
{
    if (campaign->addDiamond(dt)) {
        campaign->enableFreeDiamond();
        
        L2E_UPDATE_FREE_DIAMOND info;
        info.eProtocol = l2e_update_free_diamond;
        info.errNo = 0;
        info.freeDiamondState = campaign->getCoreData().freeDiamondState;
        info.onlineDuration = campaign->getCoreData().onlineDuration;
        ClientLogic::instance()->pass2Engine(&info);
    }
}

void SystemLogic::showLogo()
{
    L2E_COMMON info;
    info.eProtocol = l2e_show_login;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::checkNet()
{
    C2S_CHECK_CONNECT info;
    info.eProtocol = c2s_check_connect;
    ClientLogic::instance()->pass2Service(&info);
}

void SystemLogic::responseNetCheck(S2C_CHECK_CONNECT info)
{
    L2E_UPDATE_ONE_VALUE infoOut;
    infoOut.eProtocol = l2e_show_connected;
    if (info.result) {
        infoOut.value = 1;
        C2S_COMMON infoTime;
        infoTime.eProtocol = c2s_server_time_req;
        ClientLogic::instance()->pass2Service(&infoTime);
    }else{
        infoOut.value = 0;
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::arrangeLoad()
{
    account = Account::instance();
    equip = Equip::instance();
    vit = VIT::instance();
    pet = Pet::instance();
    petOffer = PetOffer::instance();
    sword = HolySword::instance();
    arms = Arms::instance();
    prov = Proving::instance();
    abyss = Abyss::instance();
    tower = Tower::instance();
    randomName = RandomName::instance();
    arena = Arena::instance();
    plotMission = PlotMission::instance();
    plot = Plot::instance();
    dialog = Dialog::instance();
    campaign = Campaign::instance();
    lucky = Lucky::instance();
    shop = Shop::instance();
    vip = VIP::instance();
    
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadVIP));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadShop));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadLucky));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadAccount));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadEquip));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadVIT));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadPet));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadPetOffer));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadHolySword));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadArms));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadProving));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadAbyss));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadTower));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadRandomNames));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadArena));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadPlotMission));
    addLoadFunc(Load::LoadFunc(&SystemLogic::loadCampaign));
    L2E_COMMON info;
    info.eProtocol = l2e_show_system_load;
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::loadAccount()
{
    account->readUnlockConfigFile();
    account->readJobConfigFile();
    account->readUplevelConfigFile();
    
//    account->deleteJson();
    if(!account->readJson())
    {
        account->save2EmptyCoreStr();
        account->parseJson();
    }
    
//    account->deleteUnlockJson();
    if (!account->readUnlockJson()) {
        account->save2EmptyUnlockStr();
        account->parseUnlockJson();
    }
    
//    account->deleteJobJson();
    if (!account->readJobJson()) {
        account->save2EmptyJobStr();
        account->parseJobJson();
    }
}

void SystemLogic::loadEquip()
{
    equip->readEquipItemConfigFile();
    equip->readEquipLevelConfigFile();
    equip->readEquipConfigFile();
    
//    equip->deleteItemJson();
    if (!equip->readItemJson()) {
        equip->save2EmptyItemStr();
        equip->parseItemJson();
    }
//    equip->deleteJson();
    if(!equip->readJson())
    {
        equip->save2EmptyCoreStr();
        equip->parseJson();
        account->changeHp(equip->getTotalHp());
        account->changeAttack(equip->getTotalAttack());
        account->changeDefence(equip->getTotalDefence());
    }
    
    equip->checkTip(account->getData().level);
}

void SystemLogic::loadVIT()
{
//    vit->deleteJson();
    if(!vit->readJson())
    {
        vit->save2EmptyCoreStr();
        vit->parseJson();
    }
}

void SystemLogic::loadPet()
{
    pet->readPetConfigFile();
    pet->readLevelConfigFile();
    
//    pet->deletePetJson();
    if (!pet->readPetJson()) {
        pet->save2EmptyPetStr();
        pet->parsePetJson();
    }
    
//    pet->deleteJson();
    if (!pet->readJson()) {
        pet->save2EmptyCoreStr();
        pet->parseJson();
        account->changeHp(pet->getInfo().hp);
        account->changeAttack(pet->getInfo().attack);
        account->changeDefence(pet->getInfo().defence);
    }
    
    pet->checkTip();
}

void SystemLogic::loadPetOffer()
{
    petOffer->readConfig();
    
//    petOffer->deleteBoardJson();
    if (!petOffer->readBoardJson()) {
        petOffer->save2EmptyBoardStr();
        petOffer->parseBoardJson();
    }
    
//    petOffer->deleteWorkJson();
    if (!petOffer->readWorkJson()) {
        petOffer->save2EmptyWorkStr();
        petOffer->parseWorkJson();
    }
    
    if(!petOffer->readDataJson()) {
        petOffer->save2EmptyDataStr();
        petOffer->parseDataJson();
    }
    
    petOffer->checkTip();
}
void SystemLogic::loadHolySword()
{
    sword->readSwordConfigFile();
    sword->readLevelConfigFile();
    
//    sword->deleteInfoJson();
    if (!sword->readInfoJson()) {
        sword->save2EmptyCoreStr();
        sword->parseInfoJson();
    }
    
//    sword->deleteSwordsJson();
    if (!sword->readSwordsJson()) {
        sword->save2EmptySwordsStr();
        sword->parseSwordsJson();
        sword->unlockAndSave(1);
        sword->setFightSword(1);
        account->changeHp(sword->getInfo().hp);
        account->changeAttack(sword->getInfo().attack);
        account->changeDefence(sword->getInfo().defence);
    }
    
    sword->checkTip(account->getData().gold);
}
void SystemLogic::loadArms()
{
    arms->readArmsConfigFile();
    arms->readLevelConfigFile();
    arms->readGemConfigFile();
    
//    arms->deleteGemJson();
    if (!arms->readGemJson()) {
        arms->save2EmptyGemStr();
        arms->parseGemJson();
    }
    
//    arms->deleteCoreJson();
    if (!arms->readCoreJson()) {
        arms->save2EmptyCoreStr();
        arms->parseCoreJson();
        account->changeHp(arms->getTotalHp());
        account->changeAttack(arms->getTotalAttack());
        account->changeDefence(arms->getTotalDefence());
        account->changeCrit(arms->getTotalCrit());
        account->changeDeCrit(arms->getTotalDecrit());
    }
    
    arms->checkTip(account->getData().fame);
}

void SystemLogic::loadProving()
{
    prov->readProvConfigFile();
    
//    prov->deleteCoreJson();
    if (!prov->readCoreJson()) {
        prov->save2EmptyCoreStr();
        prov->parseCoreJson();
    }
    
//    prov->deleteFighterJson();
    if (!prov->readFighterJson()) {
        prov->save2EmptyFighterStr();
        prov->parseFighterJson();
    }
    prov->fighterData.clear();
}
void SystemLogic::loadAbyss()
{
    abyss->readAbyssConfigFile();
    Chapter::instance()->readStageConfig(100);
    
//    abyss->deleteJson();
    if(!abyss->readJson()) {
        abyss->save2EmptyCoreStr();
        abyss->parseJson();
    }
}
void SystemLogic::loadTower()
{
    tower->readTowerConfigFile();
    Chapter::instance()->readStageConfig(200);
//    tower->deleteJson();
    if (!tower->readJson()) {
        tower->save2EmptyCoreStr();
        tower->parseJson();
    }
}

void SystemLogic::loadRandomNames()
{
    randomName->readPart1Config();
//    randomName->readPart2Config();
}

void SystemLogic::loadArena()
{
    arena->readRobotConfig();
    arena->readDivisionConfig();
    
//    arena->deleteRobotJson();
    if (!arena->readRobotJson()) {
        initArenaTopRobot();
        arena->save2EmptyRobotStr();
        arena->parseRobotJson();
    }
    
//    arena->deleteJson();
    if (!arena->readJson()) {
        arena->save2EmptyCoreStr();
        arena->parseJson();
    }
}

void SystemLogic::loadPlotMission()
{
    plotMission->readConfig();
    plotMission->readNPCConfig();
    
//    plotMission->deleteJson();
    if (!plotMission->readJson()) {
        plotMission->save2EmptyCoreStr();
        plotMission->parseJson();
    }
}

void SystemLogic::loadCampaign()
{
    campaign->readEverydayConfig();
    campaign->readActiveMissionConfig();
    campaign->readActiveBoxConfig();
    campaign->readBoundMissionConfig();
    campaign->readBoundPackageConfig();
    campaign->readMonthAssignConfig();
    
//    campaign->deleteCampaignJson();
    if (!campaign->readCampaignJson()) {
        campaign->save2EmptyCampaignJson();
        campaign->parseCampaignJson();
    }
    
//    campaign->deleteActiveJson();
    if (!campaign->readActiveJson()) {
        campaign->save2EmptyActiveJson();
        campaign->parseActiveJson();
    }
    
//    campaign->deleteMissionJson();
    if (!campaign->readMissionJson()) {
        campaign->save2EmptyMissionJson();
        campaign->parseMissionJson();
//    }else{
//        campaign->cleanBoundStr();
    }
    
    campaign->checkTip();
}

void SystemLogic::loadLucky()
{
    lucky->readLuckyConfig();
    
//    lucky->deleteJson();
    if (!lucky->readJson()) {
        lucky->save2EmptyJson();
        lucky->parseJson();
    }
    
//    lucky->deleteThresJson();
    lucky->readThres();
    
    lucky->checkTip();
}

void SystemLogic::loadShop()
{
    shop->readShopConfig();
    shop->readPackageConfig();
    
//    shop->deleteJson();
    if (!shop->readJson()) {
        shop->save2EmptyCoreStr();
        shop->parseJson();
        openDiscount();
    }
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    for (auto config :shop->shopConfigMap) {
        std::string idStr(config.second.payId);
        if(idStr == "" || idStr == "null")
        {
            continue;
        }
        
        iOSPayHelper::getInstance()->pushPid(config.second.shopId, idStr);
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
#endif
    
}

void SystemLogic::loadVIP()
{
    vip->readVIPConfig();
    vip->readVIPPackageConfig();
    
//    vip->deleteJson();
    if (!vip->readJson()) {
        vip->save2EmptyCoreStr();
        vip->parseJson();
    }
}

void SystemLogic::load()
{
    L2E_UPDATE_LOAD info;
    info.eProtocol = l2e_update_system_load;
    info.isOver = false;
    info.step = 0;
    info.count = (int)m_vecLoadFunc.size();
    int result = Load::load();
    if (result != -1) {
        info.step = result;
        //更新加载界面
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        onLoadFinish();
    }
}

void SystemLogic::onLoadFinish()
{
    everydayShow = 1;
    Load::onLoadFinish();
    ClientLogic::instance()->ChangeState(GAME_STATE_HALL);
}

void SystemLogic::showHall()
{
    L2E_SHOW_HALL info;
    info.eProtocol = l2e_show_hall_scene;
    info.autoPopLayerId = popLayerId;
    
    info.vipLevel = account->getData().vipLevel;
    info.totalForce = account->getForce();
    info.headIcon = account->jobConfigMap[account->getData().currJob].headIcon;
    info.level = account->getData().level;
    info.abyssLock = false;
    info.towerLock = false;
    info.pvpLock = false;
    info.provLock = false;
    info.gold = account->getData().gold;
    info.diamond = account->getData().diamond;
    info.VIT = vit->getTotalVIT();
    info.maxVIT = vit->getMaxBaseVIT();
    info.name = account->getData().name;
//    info.onlineCountDown = 1256;
    
    info.vipTip = vip->getTip();
    info.rewardTip = false;
    info.soulTip = false;
    info.monsterTip = false;
    info.skillTip = false;
    info.heroTip = true;
    info.armsTip = true;
    if (shop->getCoreData().discountState == 0) {
        info.discountTip = false;
//        info.discountSec = 0;
    }else{
        info.discountTip = true;
//        info.discountSec = 0;
    }
    info.tip[0] = vip->getTip();
    info.tip[1] = equip->getTip();
    info.tip[2] = pet->getTip();
    info.tip[3] = sword->getTip();
    info.tip[4] = arms->getTip();
    info.tip[5] = petOffer->getTip();
    info.tip[6] = lucky->getTip();
    info.tip[7] = campaign->getTip();
    for (int i = 0; i < 11; i++) {
        info.lockType[i] = account->unlockConfigMap[i+1].lockType;
        info.lock[i] = account->unlockMap[i+1].lock;
//#ifdef FOR_HUA
//        info.lock[i] = 0;
//#endif
//        info.lock[i] = 0;
    }
    info.freeDiamondState = campaign->getCoreData().freeDiamondState;
    info.onlineDuration = campaign->getCoreData().onlineDuration;
    ClientLogic::instance()->pass2Engine(&info);
    
    if (vit->getBaseVIT() < vit->getMaxBaseVIT()) {
        L2T_RESTORE_VIT info;
        info.eProtocol = l2t_start_restore_vit;
        info.restoreSec = vitRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }
    if (arena->getCoreData().ticketCount < arena->getCoreData().maxTicketCount) {
        L2T_RESTORE_ARENA_TICKET info;
        info.eProtocol = l2t_restore_arena_ticket;
        info.restoreSec = arenaTicketRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }
}

void SystemLogic::showHallHero()
{
    L2E_SHOW_HALL_HERO info;
    info.eProtocol = l2e_show_hall_hero;
    info.level = account->getData().level;
    if (account->isMaxLevel()) {
        info.exp = 100;
        info.maxExp = 100;
    }else{
        int baseExp = 0;
        if (info.level > 1) {
            baseExp = account->uplevelConfigMap[info.level-1].expThres;
        }
        int nextExp = account->uplevelConfigMap[info.level].expThres;
        info.exp = account->getData().exp - baseExp;
        info.maxExp = nextExp-baseExp;
    }
    
    info.totalGold = account->getData().gold;
    info.totalHp = account->getData().hp;
    info.totalAttack = account->getData().attack;
    info.totalDefence = account->getData().defence;
    info.totalCrit = account->getData().crit;
    info.totalDeCrit = account->getData().deCrit;
    info.force = account->getForce();
    info.armature = account->jobConfigMap[account->getData().currJob].armature;
    if (currEquipPos == 0) {
        currEquipPos = 1;
    }
    
    memset(info.equipId, 0, sizeof(int)*6);
    memset(info.equipLevel, 0, sizeof(int)*6);
    memset(info.equipStar, 0, sizeof(int)*6);
    for (int i = 0; i < 6; i++) {
        info.equipId[i] = equip->wearMap[i+1].equipId;
        info.equipStar[i] = equip->wearMap[i+1].star;
        info.equipLevel[i] = equip->wearMap[i+1].level;
        info.equipIcon[i] = equip->equipConfigMap[info.equipId[i]].icon;
    }
    info.currPos = currEquipPos;
    info.currEquipLevel = info.equipLevel[currEquipPos-1];
    info.currEquipName = equip->equipConfigMap[info.equipId[currEquipPos-1]].name;
    info.currEquipIcon = equip->equipConfigMap[info.equipId[currEquipPos-1]].icon;
    info.currEquipStar = equip->wearMap[currEquipPos].star;
    info.currEquipHp = equip->wearMap[currEquipPos].hp;
    info.currEquipAttack = equip->wearMap[currEquipPos].attack;
    info.currEquipDefence = equip->wearMap[currEquipPos].defence;
    info.maxLevel = equip->isMaxLevel(currEquipPos);
    if(!info.maxLevel){
        info.plusEquipHp = equip->equipLevelConfigMap[equip->wearMap[currEquipPos].level+1].deltaHp;
        info.plusEquipAttack = equip->equipLevelConfigMap[equip->wearMap[currEquipPos].level+1].deltaAttack;
        info.plusEquipDefence = equip->equipLevelConfigMap[equip->wearMap[currEquipPos].level+1].deltaDefence;
        info.costGold = equip->equipLevelConfigMap[equip->wearMap[currEquipPos].level].costGold;
    }else{
        info.plusEquipHp = 0;
        info.plusEquipAttack = 0;
        info.plusEquipDefence = 0;
        info.costGold = 0;
    }
    
    info.maxGrade = equip->isMaxGrade(currEquipPos);
    int equipId = equip->wearMap[currEquipPos].equipId;
    for (int i = 0; i < 6; i++) {
//        info.starItemId[i] = equip->equipConfigMap[info.equipId[currEquipPos]].starItem[i];
//        int currEquipId = info.equipId[currEquipPos-1];
        int itemId = equip->equipConfigMap[equipId].starItem[i];
        if (itemId <= 0) {
            info.starItemIcon[i] = "";
            info.starItemRequire[i] = 0;
            info.starItemCount[i] = 0;
        }else{
            info.starItemIcon[i] = equip->equipItemConfigMap[itemId].icon;
            info.starItemRequire[i] = equip->equipConfigMap[equipId].starItemCount[i];
            info.starItemCount[i] = equip->itemMap[itemId].count;
        }
    }

    info.posTip.clear();
    for (auto tip : equip->posTip) {
        info.posTip.push_back(tip);
    }
    info.posUplevelTip.clear();
    for (auto tip : equip->posUplevelTip) {
        info.posUplevelTip.push_back(tip);
    }
    info.posUpgradeTip.clear();
    for (auto tip : equip->posUpgradeTip) {
        info.posUpgradeTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::showSweepItem(E2L_UPDATE_ONE_VALUE info)
{
    int equipId = equip->wearMap[currEquipPos].equipId;
    int itemId = equip->equipConfigMap[equipId].starItem[info.value-1];
    if (itemId <= 0) {
        return;
    }
    int requireCount = equip->equipConfigMap[equipId].starItemCount[info.value-1];
    int count = equip->itemMap[itemId].count;
    
    requireCount -= count;
    requireCount = MAX(0, requireCount);

    L2E_SHOW_ITEM_SWEEP infoOut;
    infoOut.eProtocol = l2e_show_item_sweep;
    infoOut.currVIT = vit->getTotalVIT();
    infoOut.maxVIT = vit->getMaxBaseVIT();
    infoOut.itemTag = info.value;
    infoOut.requireCount = requireCount;
    infoOut.dropGold = equip->equipItemConfigMap[itemId].dropGold;
    infoOut.dropExp = equip->equipItemConfigMap[itemId].dropExp;
    infoOut.icon = equip->equipItemConfigMap[itemId].icon;
    infoOut.dropItemCount = equip->equipItemConfigMap[itemId].dropCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
}
void SystemLogic::startSweepItem(E2L_START_ITEM_SWEEP info)
{
    int equipId = equip->wearMap[currEquipPos].equipId;
    int itemId = equip->equipConfigMap[equipId].starItem[info.itemTag-1];
    if (itemId <= 0) {
        return;
    }
    int requireCount = equip->equipConfigMap[equipId].starItemCount[info.itemTag-1];
    int count = equip->itemMap[itemId].count;
    int vitValue = vit->getTotalVIT();
    int dropItemCount = equip->equipItemConfigMap[itemId].dropCount;
    requireCount -= count;
    requireCount = MAX(0, requireCount);
    int sweepCount = requireCount;
    if (dropItemCount > 1 && sweepCount > 0) {
        sweepCount = requireCount/dropItemCount;
        if (requireCount%dropItemCount > 0) {
            sweepCount++;
        }
    }

    L2E_ITEM_SWEEP infoOut;
    infoOut.eProtocol = l2e_item_sweep;
    int totalCount = 1;
    if (sweepCount == 0) {
        if (vitValue < info.count*5) {
            infoOut.errNo = 1;
            ClientLogic::instance()->pass2Engine(&infoOut);
            return;
        }
        totalCount = info.count;
    }else{
        if (vitValue < info.count*5) {
            totalCount = vitValue/5;
        }else{
            totalCount = info.count;
        }
    }
    if (totalCount <= 0) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    infoOut.errNo = 0;
    for (int i = 0; i < 10; i++) {
        infoOut.getItem[i] = false;
    }
    std::vector<int> randResult;
    GameUtils::getRandArray(randResult, 20);
    
    int thres = equip->equipItemConfigMap[itemId].dropRate*20/100.0;
    int successCount = 0;
    
    infoOut.count = 0;
    for (int i = 0; i < totalCount; i++) {
        infoOut.count++;
        campaign->activeMission(2, 1);
        if (randResult[i] <= thres) {
            infoOut.getItem[i] = true;
            successCount++;
            if (sweepCount > 0) {
                if (successCount >= sweepCount) {
                    break;
                }
            }
        }
    }
    if (infoOut.count > 0) {
        campaign->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = campaign->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }
    

    
    vit->useVIT(infoOut.count*5);
    L2E_UPDATE_VIT infoVIT;
    infoVIT.eProtocol = l2e_update_vit;
    infoVIT.currVIT = vit->getTotalVIT();
    infoVIT.maxVIT = vit->getMaxBaseVIT();
    ClientLogic::instance()->pass2Engine(&infoVIT);
    getVITCD();
    if (vit->getBaseVIT() < vit->getMaxBaseVIT()) {
        L2T_RESTORE_VIT info;
        info.eProtocol = l2t_start_restore_vit;
        info.restoreSec = vitRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }

    
    account->changeGold(equip->equipItemConfigMap[itemId].dropGold*infoOut.count);
    L2E_UPDATE_ONE_VALUE infoGold;
    infoGold.eProtocol = l2e_update_gold;
    infoGold.value = account->getData().gold;
    ClientLogic::instance()->pass2Engine(&infoGold);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "get_gold";
    infoUmeng.count = equip->equipItemConfigMap[itemId].dropGold*infoOut.count;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);
    
    int originForce = account->getForce();
    int originLevel = account->getData().level;
    account->addExp(equip->equipItemConfigMap[itemId].dropExp*infoOut.count);
    L2E_UPDATE_EXP infoExp;
    infoExp.eProtocol = l2e_update_exp;
    infoExp.currExp = account->getData().exp;
    infoExp.level = account->getData().level;
    infoExp.upLevel = (infoExp.level>originLevel);
    if (infoExp.upLevel) {
        equip->checkTip(infoExp.level);
        
        if (infoExp.level>=15 && originLevel < 15) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(15);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }
        
        if (infoExp.level>=10 && originLevel < 10) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(10);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }
        if (infoExp.level>=5 && originLevel < 5) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(5);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }
    }
    if (account->isMaxLevel()) {
        infoExp.currExp = 100;
        infoExp.maxExp = 100;
    }else{
        int baseExp = 0;
        if (infoExp.level > 1) {
            baseExp = account->uplevelConfigMap[infoExp.level-1].expThres;
        }
        int nextExp = account->uplevelConfigMap[infoExp.level].expThres;
        infoExp.currExp = account->getData().exp - baseExp;
        infoExp.maxExp = nextExp-baseExp;
//        infoExp.currExp = account->getData().exp;
//        infoExp.maxExp = account->uplevelConfigMap[infoExp.level].expThres;
    }
    
    infoExp.hp = account->getData().hp;
    infoExp.attack = account->getData().attack;
    infoExp.defence = account->getData().defence;
    infoExp.crit = account->getData().crit;
    infoExp.deCrit = account->getData().deCrit;
    infoExp.force = account->getForce();
    ClientLogic::instance()->pass2Engine(&infoExp);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal-originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);


    if (successCount > 0) {
        equip->addItem(itemId, successCount*dropItemCount);
        L2E_UPDATE_ITEM_COUNT infoItem;
        int requireCount = equip->equipConfigMap[equipId].starItemCount[info.itemTag-1];
        infoItem.eProtocol = l2e_update_item_count;
        infoItem.errNo = 0;
        infoItem.itemTag = info.itemTag;
        infoItem.count = equip->itemMap[itemId].count;
        infoItem.requireCount = requireCount;
        infoItem.totalDiamond = account->getData().diamond;
        ClientLogic::instance()->pass2Engine(&infoItem);

        equip->checkTip(account->getData().level);
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 1;
        infoTip.tipValue = equip->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
        
        L2E_UPDATE_EQUIP_TIP infoEquipTip;
        infoEquipTip.eProtocol = l2e_update_equip_tip;
        infoEquipTip.posTip.clear();
        for (auto tip : equip->posTip) {
            infoEquipTip.posTip.push_back(tip);
        }
        infoEquipTip.posUplevelTip.clear();
        for (auto tip : equip->posUplevelTip) {
            infoEquipTip.posUplevelTip.push_back(tip);
        }
        infoEquipTip.posUpgradeTip.clear();
        for (auto tip : equip->posUpgradeTip) {
            infoEquipTip.posUpgradeTip.push_back(tip);
        }
        ClientLogic::instance()->pass2Engine(&infoEquipTip);
    }
    
    ClientLogic::instance()->pass2Engine(&infoOut);
}
void SystemLogic::buyEquipItem(E2L_BUY_EQUIP_ITEM info)
{
    int equipId = equip->wearMap[currEquipPos].equipId;
    int itemId = equip->equipConfigMap[equipId].starItem[info.itemTag-1];
    if (itemId <= 0) {
        return;
    }
    int requireCount = equip->equipConfigMap[equipId].starItemCount[info.itemTag-1];
    int price = equip->equipItemConfigMap[itemId].price;
    
    L2E_UPDATE_ITEM_COUNT infoOut;
    infoOut.eProtocol = l2e_update_item_count;
    if (account->getData().diamond < price*info.count) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    if (!equip->addItem(itemId, info.count)) {
        infoOut.errNo = 2;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    account->changeDiamond(-price*info.count);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
    
    infoOut.errNo = 0;
    infoOut.itemTag = info.itemTag;
    infoOut.count = equip->itemMap[itemId].count;
    infoOut.requireCount = requireCount;
    infoOut.totalDiamond = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    equip->checkTip(account->getData().level);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 1;
    infoTip.tipValue = equip->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_EQUIP_TIP infoEquipTip;
    infoEquipTip.eProtocol = l2e_update_equip_tip;
    infoEquipTip.posTip.clear();
    for (auto tip : equip->posTip) {
        infoEquipTip.posTip.push_back(tip);
    }
    infoEquipTip.posUplevelTip.clear();
    for (auto tip : equip->posUplevelTip) {
        infoEquipTip.posUplevelTip.push_back(tip);
    }
    infoEquipTip.posUpgradeTip.clear();
    for (auto tip : equip->posUpgradeTip) {
        infoEquipTip.posUpgradeTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoEquipTip);
}
void SystemLogic::selEquipItem(E2L_UPDATE_ONE_VALUE info)
{
    int equipId = equip->wearMap[currEquipPos].equipId;
    int itemId = equip->equipConfigMap[equipId].starItem[info.value-1];
    if (itemId <= 0) {
        return;
    }
    int requireCount = equip->equipConfigMap[equipId].starItemCount[info.value-1];
    int count = equip->itemMap[itemId].count;
    
    count = requireCount - count;
    count = MAX(1, count);
    
    L2E_SHOW_ITEM_WAY infoOut;
    infoOut.eProtocol = l2e_show_item_way;
    infoOut.itemTag = info.value;
    infoOut.count = count;
    infoOut.price = equip->equipItemConfigMap[itemId].price;
    infoOut.totalDiamond = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::selEquipPos(E2L_SEL_EQUIP_POS info)
{
    currEquipPos = info.pos;
    int equipId = equip->wearMap[currEquipPos].equipId;
    int equipLevel = equip->wearMap[currEquipPos].level;
    
    L2E_SEL_EQUIP_POS infoOut;
    infoOut.eProtocol = l2e_sel_equip_pos;
    infoOut.currPos = currEquipPos;
    infoOut.currEquipLevel = equipLevel;
    infoOut.currEquipName = equip->equipConfigMap[equipId].name;
    infoOut.currEquipIcon = equip->equipConfigMap[equipId].icon;
    infoOut.currEquipStar = equip->wearMap[currEquipPos].star;
    infoOut.currEquipHp = equip->wearMap[currEquipPos].hp;
    infoOut.currEquipAttack = equip->wearMap[currEquipPos].attack;
    infoOut.currEquipDefence = equip->wearMap[currEquipPos].defence;
    infoOut.maxLevel = equip->isMaxLevel(currEquipPos);
    if(!infoOut.maxLevel){
        infoOut.plusEquipHp = equip->equipLevelConfigMap[equipLevel+1].deltaHp;
        infoOut.plusEquipAttack = equip->equipLevelConfigMap[equipLevel+1].deltaAttack;
        infoOut.plusEquipDefence = equip->equipLevelConfigMap[equipLevel+1].deltaDefence;
        infoOut.costGold = equip->equipLevelConfigMap[equipLevel].costGold;
    }else{
        infoOut.plusEquipHp = 0;
        infoOut.plusEquipAttack = 0;
        infoOut.plusEquipDefence = 0;
        infoOut.costGold = 0;
    }
    
    infoOut.maxGrade = equip->isMaxGrade(currEquipPos);
    for (int i = 0; i < 6; i++) {
        //        info.starItemId[i] = equip->equipConfigMap[info.equipId[currEquipPos]].starItem[i];
        int itemId = equip->equipConfigMap[equipId].starItem[i];
        if (itemId <= 0) {
            infoOut.starItemIcon[i] = "";
            infoOut.starItemRequire[i] = 0;
            infoOut.starItemCount[i] = 0;
        }else{
            infoOut.starItemIcon[i] = equip->equipItemConfigMap[itemId].icon;
            infoOut.starItemRequire[i] = equip->equipConfigMap[equipId].starItemCount[i];
            infoOut.starItemCount[i] = equip->itemMap[itemId].count;
        }
    }

    infoOut.posUplevelTip.clear();
    for (auto tip : equip->posUplevelTip) {
        infoOut.posUplevelTip.push_back(tip);
    }
    infoOut.posUpgradeTip.clear();
    for (auto tip : equip->posUpgradeTip) {
        infoOut.posUpgradeTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::uplevelCurrEquip()
{
    int equipLevel = equip->wearMap[currEquipPos].level;
    int accountLevel = account->getData().level;
    
    L2E_UPDATE_EQUIP_LEVEL info;
    info.eProtocol = l2e_update_equip_level;
    if (equip->isMaxLevel(currEquipPos)) {
        info.errNo = 1;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    if (equipLevel >= accountLevel) {
        info.errNo = 2;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    int costGold = equip->equipLevelConfigMap[equipLevel].costGold;
    if (costGold > account->getData().gold) {
        info.errNo = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    int tempHp = equip->wearMap[currEquipPos].hp;
    int tempAttack = equip->wearMap[currEquipPos].attack;
    int tempDefence = equip->wearMap[currEquipPos].defence;
    account->changeGold(-costGold);
    L2E_UPDATE_ONE_VALUE goldInfo;
    goldInfo.eProtocol = l2e_update_gold;
    goldInfo.value = account->getData().gold;
    ClientLogic::instance()->pass2Engine(&goldInfo);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "use_gold";
    infoUmeng.count = costGold;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);

    equip->uplevel(currEquipPos);
    equipLevel = equip->wearMap[currEquipPos].level;
    
    info.errNo = 0;
    info.currPos = currEquipPos;
    info.currEquipLevel = equipLevel;
    info.currEquipHp = equip->wearMap[currEquipPos].hp;
    info.currEquipAttack = equip->wearMap[currEquipPos].attack;
    info.currEquipDefence = equip->wearMap[currEquipPos].defence;
    
    info.maxLevel = equip->isMaxLevel(currEquipPos);
    if(!info.maxLevel){
        info.plusEquipHp = equip->equipLevelConfigMap[equipLevel+1].deltaHp;
        info.plusEquipAttack = equip->equipLevelConfigMap[equipLevel+1].deltaAttack;
        info.plusEquipDefence = equip->equipLevelConfigMap[equipLevel+1].deltaDefence;
        info.costGold = equip->equipLevelConfigMap[equipLevel].costGold;
    }else{
        info.plusEquipHp = 0;
        info.plusEquipAttack = 0;
        info.plusEquipDefence = 0;
        info.costGold = 0;
    }
    int originForce = account->getForce();
    account->changeHp(info.currEquipHp-tempHp);
    account->changeAttack(info.currEquipAttack-tempAttack);
    account->changeDefence(info.currEquipDefence-tempDefence);
    info.force = account->getForce();
    info.totalHp = account->getData().hp;
    info.totalAttack = account->getData().attack;
    info.totalDefence = account->getData().defence;
    ClientLogic::instance()->pass2Engine(&info);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);
    
    equip->checkTip(account->getData().level);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 1;
    infoTip.tipValue = equip->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_EQUIP_TIP infoEquipTip;
    infoEquipTip.eProtocol = l2e_update_equip_tip;
    infoEquipTip.posTip.clear();
    for (auto tip : equip->posTip) {
        infoEquipTip.posTip.push_back(tip);
    }
    infoEquipTip.posUplevelTip.clear();
    for (auto tip : equip->posUplevelTip) {
        infoEquipTip.posUplevelTip.push_back(tip);
    }
    infoEquipTip.posUpgradeTip.clear();
    for (auto tip : equip->posUpgradeTip) {
        infoEquipTip.posUpgradeTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoEquipTip);
}

void SystemLogic::upgradeCurrEquip()
{
    L2E_UPDATE_EQUIP_STAR info;
    info.eProtocol = l2e_update_equip_star;
    
    if (equip->isMaxGrade(currEquipPos)) {
        info.errNo = 1;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    if (!equip->checkUpgradeItem(currEquipPos)) {
        info.errNo = 2;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    int tempHp = equip->wearMap[currEquipPos].hp;
    int tempAttack = equip->wearMap[currEquipPos].attack;
    int tempDefence = equip->wearMap[currEquipPos].defence;
    if (!equip->upgrade(currEquipPos)) {
        info.errNo = 3;
        ClientLogic::instance()->ProcessUIRequest(&info);
        return;
    }
    
    int equipId = equip->wearMap[currEquipPos].equipId;
    info.errNo = 0;
    info.currPos = currEquipPos;
    info.currEquipIcon = equip->equipConfigMap[equipId].icon;
    info.currEquipName = equip->equipConfigMap[equipId].name;
    info.currEquipStar = equip->wearMap[currEquipPos].star;
    info.currEquipHp = equip->wearMap[currEquipPos].hp;
    info.currEquipAttack = equip->wearMap[currEquipPos].attack;
    info.currEquipDefence = equip->wearMap[currEquipPos].defence;
    
    int originForce = account->getForce();
    account->changeHp(info.currEquipHp-tempHp);
    account->changeAttack(info.currEquipAttack-tempAttack);
    account->changeDefence(info.currEquipDefence-tempDefence);
    info.force = account->getForce();
    info.totalHp = account->getData().hp;
    info.totalAttack = account->getData().attack;
    info.totalDefence = account->getData().defence;

    info.maxGrade = equip->isMaxGrade(currEquipPos);
    for (int i = 0; i < 6; i++) {
        //        info.starItemId[i] = equip->equipConfigMap[info.equipId[currEquipPos]].starItem[i];
        int itemId = equip->equipConfigMap[equipId].starItem[i];
        if (itemId <= 0) {
            info.starItemIcon[i] = "";
            info.starItemRequire[i] = 0;
            info.starItemCount[i] = 0;
        }else{
            info.starItemIcon[i] = equip->equipItemConfigMap[itemId].icon;
            info.starItemRequire[i] = equip->equipConfigMap[equipId].starItemCount[i];
            info.starItemCount[i] = equip->itemMap[itemId].count;
        }
    }
    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeBoundMission(1, currEquipPos, equip->wearMap[currEquipPos].star);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);
    
    equip->checkTip(account->getData().level);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 1;
    infoTip.tipValue = equip->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_EQUIP_TIP infoEquipTip;
    infoEquipTip.eProtocol = l2e_update_equip_tip;
    infoEquipTip.posTip.clear();
    for (auto tip : equip->posTip) {
        infoEquipTip.posTip.push_back(tip);
    }
    infoEquipTip.posUplevelTip.clear();
    for (auto tip : equip->posUplevelTip) {
        infoEquipTip.posUplevelTip.push_back(tip);
    }
    infoEquipTip.posUpgradeTip.clear();
    for (auto tip : equip->posUpgradeTip) {
        infoEquipTip.posUpgradeTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoEquipTip);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}


void SystemLogic::restoreVIT(T2L_COMMON info)
{
    if (info.passSec > 0) {
        int plusCount = info.passSec/vitRestoreSec;
        vit->restoreVIT(1+plusCount);
    }else{
        vit->restoreVIT(1);
    }
    
    if (vit->getBaseVIT() < vit->getMaxBaseVIT()) {
        L2T_RESTORE_VIT info;
        info.eProtocol = l2t_start_restore_vit;
        info.restoreSec = vitRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }
    L2E_UPDATE_VIT infoOut;
    infoOut.eProtocol = l2e_update_vit;
    infoOut.currVIT = vit->getTotalVIT();
    infoOut.maxVIT = vit->getMaxBaseVIT();
    ClientLogic::instance()->pass2Engine(&infoOut);
    getVITCD();
}

void SystemLogic::restoreArenaTicket(T2L_COMMON info)
{
    if (info.passSec > 0) {
        int plusCount = info.passSec/arenaTicketRestoreSec;
        arena->addTicket(1+plusCount);
    }
    
    if (arena->getCoreData().ticketCount < arena->getCoreData().maxTicketCount) {
        L2T_RESTORE_ARENA_TICKET info;
        info.eProtocol = l2t_restore_arena_ticket;
        info.restoreSec = arenaTicketRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }
    L2E_UPDATE_ARENA_TICKET infoOut;
    infoOut.eProtocol = l2e_update_arena_ticket;
    infoOut.currCount = arena->getCoreData().ticketCount;
    infoOut.maxCount = arena->getCoreData().maxTicketCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::showHallPet()
{
    L2E_SHOW_HALL_PET info;
    info.eProtocol = l2e_show_hall_pet;
    memset(info.petId, 0, sizeof(int)*10);
    int index=0;
    for (auto data : pet->petsMap) {
        info.petId[index] = data.first;
        info.petLevel[index] = pet->petsMap[data.first].level;
        index++;
    }
    for (int i = 0; i < 10; i++) {
        int petId = info.petId[i];
        if (petId != 0) {
            info.icon[i] = pet->petConfigMap[petId].icon;
            info.name[i] = pet->petConfigMap[petId].name;
        }else{
            info.icon[i] = "";
        }
    }
    
    memcpy(info.fightPetId, pet->getInfo().fightPet, sizeof(int)*3);
//    if (currPetId == 0) {
        currPetId = info.petId[0];
//    }
    
    info.currPetId = currPetId;
    info.currPetImg = pet->petConfigMap[currPetId].image;
    info.currPetSkillName = pet->petConfigMap[currPetId].skillName;
    info.currPetLevel = pet->petsMap[currPetId].level;
    info.currPetFragCount = pet->petsMap[currPetId].fragmentCount;
    info.currPetMaxLevel = pet->isMaxLevel(currPetId);
    if (!info.currPetMaxLevel) {
        int nextLevel = pet->getNextLevelId(currPetId);
        info.currPetMaxFragCount = pet->levelConfigMap[nextLevel].costFragment;
        info.currPetLevelhp = pet->levelConfigMap[nextLevel].deltaHp;
        info.currPetLevelAttack = pet->levelConfigMap[nextLevel].deltaAttack;
        info.currPetLevelDefence = pet->levelConfigMap[nextLevel].deltaDefence;
    }
    
    info.totalHp = pet->getInfo().hp;
    info.totalAttack = pet->getInfo().attack;
    info.totalDefence = pet->getInfo().defence;
    
    info.uplevelTip.clear();
    for (auto tip : pet->uplevelTip) {
        info.uplevelTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::selPetPos(E2L_UPDATE_ONE_VALUE info)
{
    currPetId = info.value;
    
    L2E_SEL_PET_POS infoOut;
    infoOut.eProtocol = l2e_sel_pet_pos;
    infoOut.currPetId = currPetId;
    infoOut.currPetImg = pet->petConfigMap[currPetId].image;
    infoOut.currPetSkillName = pet->petConfigMap[currPetId].skillName;
    infoOut.currPetLevel = pet->petsMap[currPetId].level;
    infoOut.currPetFragCount = pet->petsMap[currPetId].fragmentCount;
    infoOut.currPetMaxLevel = pet->isMaxLevel(currPetId);
    if (!infoOut.currPetMaxLevel) {
        int nextLevel = pet->getNextLevelId(currPetId);
        infoOut.currPetMaxFragCount = pet->levelConfigMap[nextLevel].costFragment;
        infoOut.currPetLevelhp = pet->levelConfigMap[nextLevel].deltaHp;
        infoOut.currPetLevelAttack = pet->levelConfigMap[nextLevel].deltaAttack;
        infoOut.currPetLevelDefence = pet->levelConfigMap[nextLevel].deltaDefence;
    }
    
    infoOut.uplevelTip.clear();
    for (auto tip : pet->uplevelTip) {
        infoOut.uplevelTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::uplevelCurrPet()
{
    int tempHp = pet->getInfo().hp;
    int tempAttack = pet->getInfo().attack;
    int tempDefence = pet->getInfo().defence;

    int errNo = pet->uplevel(currPetId);
    L2E_UPDATE_PET_LEVEL info;
    info.eProtocol = l2e_update_pet_level;
    info.errNo = errNo;
    if (errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.currPetId = currPetId;
    info.currPetImg = pet->petConfigMap[currPetId].image;
    info.currPetSkillName = pet->petConfigMap[currPetId].skillName;
    info.currPetLevel = pet->petsMap[currPetId].level;
    info.currPetFragCount = pet->petsMap[currPetId].fragmentCount;
    info.currPetMaxLevel = pet->isMaxLevel(currPetId);
    if (!info.currPetMaxLevel) {
        int nextLevel = pet->getNextLevelId(currPetId);
        info.currPetMaxFragCount = pet->levelConfigMap[nextLevel].costFragment;
        info.currPetLevelhp = pet->levelConfigMap[nextLevel].deltaHp;
        info.currPetLevelAttack = pet->levelConfigMap[nextLevel].deltaAttack;
        info.currPetLevelDefence = pet->levelConfigMap[nextLevel].deltaDefence;
    }
    
    info.totalHp = pet->getInfo().hp;
    info.totalAttack = pet->getInfo().attack;
    info.totalDefence = pet->getInfo().defence;
    int originForce = account->getForce();
    account->changeHp(info.totalHp-tempHp);
    account->changeAttack(info.totalAttack-tempAttack);
    account->changeDefence(info.totalDefence-tempDefence);


    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeBoundMission(2, 0, pet->petsMap[currPetId].level);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);

    pet->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 2;
    infoTip.tipValue = pet->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_PET_TIP infoPetTip;
    infoPetTip.eProtocol = l2e_update_pet_tip;
    infoPetTip.uplevelTip.clear();
    for (auto tip : pet->uplevelTip) {
        infoPetTip.uplevelTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoPetTip);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::addFightPet(E2L_UPDATE_ONE_VALUE info)
{
    int petId = info.value;
    
    L2E_UPDATE_FIGHT_PET infoOut;
    infoOut.eProtocol = l2e_update_fight_pet;
    int errNo = pet->addFightPet(petId);
    infoOut.errNo = errNo;
    if (errNo != 0) {
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    memcpy(infoOut.fightPetId, pet->getInfo().fightPet, sizeof(int)*3);
    for (int i = 0; i < 3; i++) {
        int petId = infoOut.fightPetId[i];
        if (petId != 0) {
            infoOut.icon[i] = pet->petConfigMap[petId].icon;
        }else{
            infoOut.icon[i] = "";
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::removeFightPet(E2L_UPDATE_ONE_VALUE info)
{
    int petPos = info.value;
    pet->removeFightPet(petPos);
    
    L2E_UPDATE_FIGHT_PET infoOut;
    infoOut.eProtocol = l2e_update_fight_pet;
    infoOut.errNo = 0;
    
    memcpy(infoOut.fightPetId, pet->getInfo().fightPet, sizeof(int)*3);
    for (int i = 0; i < 3; i++) {
        int petId = infoOut.fightPetId[i];
        if (petId != 0) {
            infoOut.icon[i] = pet->petConfigMap[petId].icon;
        }else{
            infoOut.icon[i] = "";
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);

}

void SystemLogic::showHallPetOffer()
{
    L2E_SHOW_HALL_PET_OFFER info;
    info.eProtocol = l2e_show_hall_pet_offer;
    for (int i = 0; i < 3; i++) {
        int offerId = petOffer->boardOffer[i+1].offerId;
        info.boardOfferState[i] = petOffer->boardOffer[i+1].state;
        info.boardOfferType[i] = petOffer->configMap[offerId].type;
        info.boardOfferCostTime[i] = petOffer->configMap[offerId].costHours;
//        info.boundCount[i]
        memcpy(info.boundCount[i], petOffer->configMap[offerId].boundCount, sizeof(int)*3);
        memcpy(info.boundType[i], petOffer->configMap[offerId].boundType, sizeof(int)*3);
        for (int j = 0; j < 3; j++) {
            int boundType = petOffer->configMap[offerId].boundType[j];
            int boundId = petOffer->configMap[offerId].boundId[j];
            switch (boundType) {
                case 1:
                {
                    info.boundIcons[i][j] = "drop-16.png";
                }
                    break;
                case 2:
                {
                    info.boundIcons[i][j] = pet->petConfigMap[boundId].icon;
                }
                    break;
                case 3:
                {
                    info.boundIcons[i][j] = arms->gemsConfigMap[boundId].icon;
                }
                    break;
                default:
                {
                    info.boundIcons[i][j] = "";
                }
                    break;
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        int offerId = petOffer->workOffers[i+1].offerId;
        info.workOfferType[i] = petOffer->configMap[offerId].type;
        info.workOfferState[i] = petOffer->workOffers[i+1].state;
    }
    info.refreshBoardPrice = refreshOfferPrice[petOffer->offerData.refreshCount];
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::getAllOfferCD()
{
    L2T_COMMON info;
    info.eProtocol = l2t_get_offer_all_cd;
    ClientLogic::instance()->pass2Time(&info);
}

void SystemLogic::updateAllOfferCD(T2L_UPDATE_OFFER_ALL_CD info)
{
    L2E_UPDATE_ALL_OFFER_CD infoOut;
    infoOut.eProtocol = l2e_update_offer_all_cd;
    infoOut.refreshCD = info.refreshLeftSec;
    infoOut.workOfferCD[0] = info.work1LeftSec;
    infoOut.workOfferCD[1] = info.work2LeftSec;
    infoOut.workOfferCD[2] = info.work3LeftSec;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::refreshBoardOffer()
{
    petOffer->refreshBoardOffers();
    L2T_RESET_OFFER_BOARD info;
    info.eProtocol = l2t_reset_offer_board;
    info.refreshSec = offerBoardRefreshSec;
    ClientLogic::instance()->pass2Time(&info);
    
    L2E_UPDATE_BOARD_OFFER infoOut;
    infoOut.eProtocol = l2e_update_board_offer;
    infoOut.errNo = 0;
    infoOut.refreshPrice = refreshOfferPrice[petOffer->offerData.refreshCount];
    for (int i = 0; i < 3; i++) {
        int offerId = petOffer->boardOffer[i+1].offerId;
        infoOut.boardOfferState[i] = petOffer->boardOffer[i+1].state;
        infoOut.boardOfferType[i] = petOffer->configMap[offerId].type;
        infoOut.boardOfferCostTime[i] = petOffer->configMap[offerId].costHours;
        //        info.boundCount[i]
        memcpy(infoOut.boundCount[i], petOffer->configMap[offerId].boundCount, sizeof(int)*3);
        memcpy(infoOut.boundType[i], petOffer->configMap[offerId].boundType, sizeof(int)*3);
        for (int j = 0; j < 3; j++) {
            int boundType = petOffer->configMap[offerId].boundType[j];
            int boundId = petOffer->configMap[offerId].boundId[j];
            switch (boundType) {
                case 1:
                {
                    infoOut.boundIcons[i][j] = "drop-16.png";
                }
                    break;
                case 2:
                {
                    infoOut.boundIcons[i][j] = pet->petConfigMap[boundId].icon;
                }
                    break;
                case 3:
                {
                    infoOut.boundIcons[i][j] = arms->gemsConfigMap[boundId].icon;
                }
                    break;
                default:
                {
                    infoOut.boundIcons[i][j] = "";
                }
                    break;
            }
        }
    }
    infoOut.refreshCD = offerBoardRefreshSec;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::refreshOptBoardOffer()
{
    L2E_UPDATE_BOARD_OFFER infoOut;
    infoOut.eProtocol = l2e_update_board_offer;
    infoOut.errNo = 0;

    if (account->getData().diamond < refreshOfferPrice[petOffer->offerData.refreshCount]) {
//        if (account->getData().diamond < offerBoardRefreshPrice) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    account->changeDiamond(-refreshOfferPrice[petOffer->offerData.refreshCount]);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
    
    petOffer->refreshOptBoardOffers();
    L2T_RESET_OFFER_BOARD info;
    info.eProtocol = l2t_reset_offer_board;
    info.refreshSec = offerBoardRefreshSec;
    ClientLogic::instance()->pass2Time(&info);

    for (int i = 0; i < 3; i++) {
        int offerId = petOffer->boardOffer[i+1].offerId;
        infoOut.boardOfferState[i] = petOffer->boardOffer[i+1].state;
        infoOut.boardOfferType[i] = petOffer->configMap[offerId].type;
        infoOut.boardOfferCostTime[i] = petOffer->configMap[offerId].costHours;
        //        info.boundCount[i]
        memcpy(infoOut.boundCount[i], petOffer->configMap[offerId].boundCount, sizeof(int)*3);
        memcpy(infoOut.boundType[i], petOffer->configMap[offerId].boundType, sizeof(int)*3);
        for (int j = 0; j < 3; j++) {
            int boundType = petOffer->configMap[offerId].boundType[j];
            int boundId = petOffer->configMap[offerId].boundId[j];
            switch (boundType) {
                case 1:
                {
                    infoOut.boundIcons[i][j] = "drop-16.png";
                }
                    break;
                case 2:
                {
                    infoOut.boundIcons[i][j] = pet->petConfigMap[boundId].icon;
                }
                    break;
                case 3:
                {
                    infoOut.boundIcons[i][j] = arms->gemsConfigMap[boundId].icon;
                }
                    break;
                default:
                {
                    infoOut.boundIcons[i][j] = "";
                }
                    break;
            }
        }
    }
    infoOut.refreshPrice = refreshOfferPrice[petOffer->offerData.refreshCount];
    infoOut.refreshCD = offerBoardRefreshSec;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::openBoardOffer(E2L_UPDATE_ONE_VALUE info)
{
    L2E_OPEN_BOARD_OFFER infoOut;
    infoOut.eProtocol = l2e_open_board_offer;
    infoOut.errNo = 0;
    if (petOffer->getWorkOfferCount() >= 3) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    if (petOffer->boardOffer[info.value].state != 0) {
        infoOut.errNo = 2;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    petOffer->getWorkPetsId(infoOut.workPet);
    
    int offerId = petOffer->boardOffer[info.value].offerId;
    infoOut.offerPos = info.value;
    infoOut.offerId = offerId;
    infoOut.needCount = petOffer->configMap[offerId].petCount;
    infoOut.offerType = petOffer->configMap[offerId].type;
    int index = 0;
    for (auto config : pet->petConfigMap) {
        infoOut.petsIcon[index] = config.second.icon;
        infoOut.unlock[index] = (pet->petsMap[config.first].level>0);
        index++;
        if (index >= 10) {
            break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::addOfferPet(E2L_ADD_OFFER_PET info)
{
    int offerId = info.offerId;
    int petId = info.petId;
    int offerType = petOffer->configMap[offerId].type;
    
    std::vector<int> limitRelation;
    limitRelation.clear();
    int readyCount = 0;
    std::map<int, int> relationMap;
    for (int i = 0; i < 3; i++) {
        if (info.readyPets[i] == 0) {
            continue;
        }
        readyCount++;
        std::vector<int> relation;
        pet->getRelations(info.readyPets[i], relation);
        for (auto data : relation) {
            if (relationMap.find(data) != relationMap.end()) {
                relationMap[data]++;
            }else{
                relationMap[data] = 1;
            }
        }
    }
    for (auto relation : relationMap) {
        if (relation.second == readyCount) {
            limitRelation.push_back(relation.first);
        }
    }
    
    
    L2E_ADD_OFFER_PET infoOut;
    infoOut.eProtocol = l2e_add_offer_pet;
    infoOut.errNo = 0;
    infoOut.petId = petId;
    infoOut.petIcon = pet->petConfigMap[petId].icon;
    infoOut.relationPets.clear();
    if (pet->petsMap[petId].level == 0) {
        infoOut.errNo = 4;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    std::vector<int> workPets;
    petOffer->getWorkPetsId(workPets);
    for (auto pet : workPets) {
        if (pet == petId) {
            infoOut.errNo = 1;
            ClientLogic::instance()->pass2Engine(&infoOut);
            return;
        }
    }
    if (offerType == 4) {
        std::vector<int> petRelationIds;
        pet->getRelations(petId, petRelationIds);
        if (petRelationIds.empty()) {
            infoOut.errNo = 2;
            ClientLogic::instance()->pass2Engine(&infoOut);
            return;
        }
        if (!limitRelation.empty()) {
            bool match = false;
            int matchId = 0;
            for (auto limit : limitRelation) {
                for (auto relation : petRelationIds) {
                    if (limit == relation) {
                        matchId = limit;
                        match = true;
                        break;
                    }
                }
                if (match) {
                    break;
                }
            }
            if (!match) {
                infoOut.errNo = 3;
                ClientLogic::instance()->pass2Engine(&infoOut);
                return;
            }
            
            pet->getPetsWithSameRelation(matchId, infoOut.relationPets);
            ClientLogic::instance()->pass2Engine(&infoOut);
            return;
        }
        for (auto relationId : petRelationIds) {
            std::vector<int> petsId;
            pet->getPetsWithSameRelation(relationId, petsId);
            for (auto petId : petsId) {
                infoOut.relationPets.push_back(petId);
            }
        }
        
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::removeOfferPet(E2L_REMOVE_OFFER_PET info)
{
    int offerId = info.offerId;
    int offerType = petOffer->configMap[offerId].type;
    if (offerType != 4) {
        return;
    }
//    int readyPetCount = 0;
//    std::map<int, int> sameRelationCount;
//    sameRelationCount.clear();
//    for (int i = 0; i < 3; i++) {
//        int petId = info.readyPets[i];
//        if (petId == 0) {
//            continue;
//        }
//        readyPetCount++;
//        std::vector<int> limitRelation;
//        limitRelation.clear();
//        pet->getRelations(info.readyPets[i], limitRelation);
//        for (auto relation : limitRelation) {
//            if (sameRelationCount.find(relation) == sameRelationCount.end()) {
//                sameRelationCount[relation] = 1;
//            }else{
//                sameRelationCount[relation]++;
//            }
//        }
//    }
    std::vector<int> limitRelation;
    limitRelation.clear();
    int readyCount = 0;
    std::map<int, int> relationMap;
    for (int i = 0; i < 3; i++) {
        if (info.readyPets[i] == 0) {
            continue;
        }
        readyCount++;
        std::vector<int> relation;
        pet->getRelations(info.readyPets[i], relation);
        for (auto data : relation) {
            if (relationMap.find(data) != relationMap.end()) {
                relationMap[data]++;
            }else{
                relationMap[data] = 1;
            }
        }
    }
    for (auto relation : relationMap) {
        if (relation.second == readyCount) {
            limitRelation.push_back(relation.first);
        }
    }
//    if (readyPetCount == 0) {
//        return;
//    }
//    
//    if (sameRelationCount.size() == 0) {
//        return;
//    }
//    
//    for (auto relation : sameRelationCount) {
//        if (relation.second < readyPetCount) {
//            sameRelationCount.erase(relation.first);
//        }
//    }
//    if (sameRelationCount.empty()) {
//        return;
//    }
//    L2E_REMOVE_OFFER_PET infoOut;
//    infoOut.eProtocol = l2e_remove_offer_pet;
//    infoOut.relationPets.clear();
//    for (auto relationId : sameRelationCount) {
//        std::vector<int> petsId;
//        pet->getPetsWithSameRelation(relationId.first, petsId);
//        for (auto petId : petsId) {
//            infoOut.relationPets.push_back(petId);
//        }
//    }
//    ClientLogic::instance()->pass2Engine(&infoOut);
    L2E_REMOVE_OFFER_PET infoOut;
    infoOut.eProtocol = l2e_remove_offer_pet;
    infoOut.relationPets.clear();
    for (auto relationId : limitRelation) {
        std::vector<int> petsId;
        pet->getPetsWithSameRelation(relationId, petsId);
        for (auto petId : petsId) {
            infoOut.relationPets.push_back(petId);
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::takeBoardOffer(E2L_TAKE_BOARD_OFFER info)
{
    int offerId = petOffer->boardOffer[info.offerPos].offerId;
    int petCount = 0;
    for (int i = 0; i < 3; i++) {
        if (info.petsId[i] != 0) {
            petCount++;
        }
    }
    L2E_TAKE_BOARD_OFFER infoOut;
    infoOut.eProtocol = l2e_take_board_offer;
    infoOut.errNo = 0;
    if(petCount < petOffer->configMap[offerId].petCount) {
        infoOut.errNo = 4;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    int err = petOffer->takeOffer(info.offerPos, info.petsId);
    if (err != 0) {
        infoOut.errNo = err;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    infoOut.boardOfferPos = info.offerPos;
    infoOut.workOfferPos = petOffer->getWorkPos();
    infoOut.workOfferType = petOffer->configMap[offerId].type;
    infoOut.workOfferCD = petOffer->configMap[offerId].costHours*3600;//
    
    L2T_START_WORK_OFFER infoTime;
    infoTime.eProtocol = l2t_start_work_offer;
    infoTime.pos = infoOut.workOfferPos;
    infoTime.cd = infoOut.workOfferCD;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::workOfferOver(int pos)
{
    petOffer->overWork(pos);
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_work_offer_over;
    info.value = pos;
    ClientLogic::instance()->pass2Engine(&info);
    
    petOffer->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 5;
    infoTip.tipValue = petOffer->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
}

void SystemLogic::takeOfferReward(E2L_UPDATE_ONE_VALUE info)
{
    int offerId = petOffer->workOffers[info.value].offerId;
    L2E_TAKE_OFFER_REWARD infoOut;
    infoOut.eProtocol = l2e_take_offer_reward;
    infoOut.errNo = 0;
    if (petOffer->workOffers[info.value].state == 1) {
//        infoOut.errNo = 1;
//        ClientLogic::instance()->pass2Engine(&infoOut);
        
        L2E_SHOW_BUY_OFFER infoBuy;
        infoBuy.eProtocol = l2e_show_buy_offer;
        infoBuy.workOfferPos = info.value;
        infoBuy.workOfferType = petOffer->configMap[offerId].type;
        infoBuy.price = petOffer->configMap[offerId].price;
        memcpy(infoBuy.boundCount, petOffer->configMap[offerId].boundCount, sizeof(int)*3);
        memcpy(infoBuy.boundType, petOffer->configMap[offerId].boundType, sizeof(int)*3);
        for (int i = 0; i < 3; i++) {
            int petId = petOffer->workOffers[info.value].petId[i];
            if (petId == 0) {
                infoBuy.petIcons[i] = "";
            }else{
                infoBuy.petIcons[i] = pet->petConfigMap[petId].icon;
            }
            
            int boundType = petOffer->configMap[offerId].boundType[i];
            int boundId = petOffer->configMap[offerId].boundId[i];
            switch (boundType) {
                case 1:
                {
                    infoBuy.boundIcons[i] = "drop-16.png";
                }
                    break;
                case 2:
                {
                    infoBuy.boundIcons[i] = pet->petConfigMap[boundId].icon;
                }
                    break;
                case 3:
                {
                    infoBuy.boundIcons[i] = arms->gemsConfigMap[boundId].icon;
                }
                    break;
                default:
                {
                    infoBuy.boundIcons[i] = "";
                }
                    break;
            }
        }
        ClientLogic::instance()->pass2Engine(&infoBuy);
        return;
    }
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    for (int i = 0; i < 3; i++) {
        int boundType = petOffer->configMap[offerId].boundType[i];
        int boundId = petOffer->configMap[offerId].boundId[i];
        infoGet.count[i] = petOffer->configMap[offerId].boundCount[i];
        infoGet.frag[i] = false;
        switch (boundType) {
            case 1:
            {
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                infoGet.name[i] = StringData::shared()->stringFromKey("gold");
                account->changeGold(petOffer->configMap[offerId].boundCount[i]);
                
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = petOffer->configMap[offerId].boundCount[i];
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);
            }
                break;
            case 2:
            {
                infoGet.frag[i] = true;
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                infoGet.name[i] = StringData::shared()->stringFromKey("pet_fragment");
                pet->addFragment(boundId, petOffer->configMap[offerId].boundCount[i]);
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
            break;
            case 3:
            {
                arms->addGem(boundId, petOffer->configMap[offerId].boundCount[i]);
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                infoGet.name[i] = StringData::shared()->stringFromKey("gem");
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
            break;
            default:
                break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoGet);
    
    petOffer->takeReward(info.value);
    infoOut.workOfferPos = info.value;
    petOffer->getWorkPetsId(infoOut.workPet);
    ClientLogic::instance()->pass2Engine(&infoOut);
    campaign->activeMission(7, 1);
    campaign->activeBoundMission(6, petOffer->configMap[offerId].type, 0);
    
    petOffer->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 5;
    infoTip.tipValue = petOffer->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::buyOffer(E2L_UPDATE_ONE_VALUE info)
{
    int pos = info.value;
    int offerId = petOffer->workOffers[pos].offerId;
    
    L2E_BUY_OFFER infoOut;
    infoOut.eProtocol = l2e_buy_offer;
    infoOut.errNo = 0;
    if (offerId <= 0) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    int price = petOffer->configMap[offerId].price;
    if (account->getData().diamond < price) {
        infoOut.errNo = 2;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    account->changeDiamond(-price);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);

    L2T_STOP_WORK_OFFER infoTime;
    infoTime.eProtocol = l2t_stop_work_offer;
    infoTime.pos = pos;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    for (int i = 0; i < 3; i++) {
        int boundType = petOffer->configMap[offerId].boundType[i];
        int boundId = petOffer->configMap[offerId].boundId[i];
        infoGet.count[i] = petOffer->configMap[offerId].boundCount[i];
        infoGet.frag[i] = false;
        switch (boundType) {
            case 1:
            {
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                infoGet.name[i] = StringData::shared()->stringFromKey("gold");
                account->changeGold(petOffer->configMap[offerId].boundCount[i]);
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = petOffer->configMap[offerId].boundCount[i];
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);
            }
                break;
            case 2:
            {
                infoGet.frag[i] = true;
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                infoGet.name[i] = StringData::shared()->stringFromKey("pet_fragment");
                pet->addFragment(boundId, petOffer->configMap[offerId].boundCount[i]);
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 3:
            {
                arms->addGem(boundId, petOffer->configMap[offerId].boundCount[i]);
                infoGet.icon[i] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                infoGet.name[i] = StringData::shared()->stringFromKey("gem");
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            default:
                break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoGet);

    petOffer->takeReward(info.value);
    infoOut.workOfferPos = info.value;
    petOffer->getWorkPetsId(infoOut.workPet);
    ClientLogic::instance()->pass2Engine(&infoOut);
    campaign->activeMission(7, 1);
    campaign->activeBoundMission(6, petOffer->configMap[offerId].type, 0);
    

    
    petOffer->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 5;
    infoTip.tipValue = petOffer->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::resetOfferCount()
{
    petOffer->resetRefreshCount();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_reset_petoffer_count;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_reset_offer_refresh_count;
    info.value = refreshOfferPrice[petOffer->offerData.refreshCount];
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::showHallHolySword()
{
    L2E_SHOW_HOLY_SWORD info;
    info.eProtocol = l2e_show_hall_sword;
    int index = 0;
    for (auto swordId : sword->swordConfigIdTable) {
        info.swordName[index] = sword->swordConfigMap[swordId.first].name;
        info.swordLock[index] = (sword->swordsMap[swordId.first].lock == 1);
        index++;
        if (index >= 5) {
            break;
        }
    }
    info.fightSwordId = sword->getInfo().fightSwordId;
    if (currSwordId == 0) {
        currSwordId = sword->getInfo().fightSwordId;
    }
    info.currSwrodId = currSwordId;
    info.currSwordImg = sword->swordConfigMap[currSwordId].image;
    info.currSwordLevel = sword->swordsMap[currSwordId].level;
    info.nextSwordLockLevel = sword->swordConfigMap[currSwordId].lockLevel;
    info.totalHp = sword->getInfo().hp;
    info.totalAttack = sword->getInfo().attack;
    info.totalDefence = sword->getInfo().defence;
    info.totalGold = account->getData().gold;
    info.totalStone = sword->getInfo().stoneCount;
    
    info.maxLevel = sword->isMaxLevel(currSwordId);
    if(!info.maxLevel){
        int nextLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level+1);
        info.nextHp = sword->levelConfigMap[nextLevel].deltaHp;
        info.nextAttack = sword->levelConfigMap[nextLevel].deltaAttack;
        info.nextDefence = sword->levelConfigMap[nextLevel].deltaDefence;
//        int currLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level);
        info.costStone = sword->levelConfigMap[nextLevel].costStone;
        info.costGold = sword->levelConfigMap[nextLevel].costGold;
    }else{
        info.nextHp = 0;
        info.nextAttack = 0;
        info.nextAttack = 0;
        info.costGold = 0;
        info.costStone = 0;
    }
    if (!sword->getTip()) {
        info.tipIndex = 0;
    }else{
        info.tipIndex = sword->getUplevelTip();
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::selSword(E2L_UPDATE_ONE_VALUE info)
{
    if (currSwordId == info.value) {
        return;
    }
    
    L2E_UPDATE_SWORD infoOut;
    infoOut.eProtocol = l2e_update_sword;
    infoOut.errNo = 0;
    if (sword->swordsMap[info.value].lock == 1) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    currSwordId = info.value;
    infoOut.currSwordId = currSwordId;
    infoOut.currSwordImg = sword->swordConfigMap[currSwordId].image;
    infoOut.currSwordLevel = sword->swordsMap[currSwordId].level;
    infoOut.nextSwordLockLevel = sword->swordConfigMap[currSwordId].lockLevel;
    
    infoOut.maxLevel = sword->isMaxLevel(currSwordId);
    if(!infoOut.maxLevel){
        int nextLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level+1);
        infoOut.nextHp = sword->levelConfigMap[nextLevel].deltaHp;
        infoOut.nextAttack = sword->levelConfigMap[nextLevel].deltaAttack;
        infoOut.nextDefence = sword->levelConfigMap[nextLevel].deltaDefence;
//        int currLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level);
        infoOut.costStone = sword->levelConfigMap[nextLevel].costStone;
        infoOut.costGold = sword->levelConfigMap[nextLevel].costGold;
    }else{
        infoOut.nextHp = 0;
        infoOut.nextAttack = 0;
        infoOut.nextAttack = 0;
        infoOut.costGold = 0;
        infoOut.costStone = 0;
    }
    if (!sword->getTip()) {
        infoOut.tipIndex = 0;
    }else{
        infoOut.tipIndex = sword->getUplevelTip();
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::setFightSword()
{
    L2E_UPDATE_FIGHT_SWORD info;
    info.eProtocol = l2e_update_fight_sword;
    info.errNo = 0;
    if (sword->swordsMap[currSwordId].lock == 1) {
        info.errNo = 1;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    sword->setFightSword(currSwordId);
    info.fightSwordId = currSwordId;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::uplevelSword()
{
    int tempHp = sword->getInfo().hp;
    int tempAttack = sword->getInfo().attack;
    int tempDefence = sword->getInfo().defence;
    
    L2E_UPDATE_SWORD_LEVEL info;
    info.eProtocol = l2e_update_sword_level;

    bool maxLevel = sword->isMaxLevel(currSwordId);
    int costGold = 0;
    if(!maxLevel){
        
        int nextLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level+1);
        costGold = sword->levelConfigMap[nextLevel].costGold;
        if (costGold > account->getData().gold) {
            info.errNo = 5;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    }


    info.errNo = sword->uplevel(currSwordId);
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    account->changeGold(-costGold);
    L2E_UPDATE_ONE_VALUE infoGold;
    infoGold.eProtocol = l2e_update_gold;
    infoGold.value = account->getData().gold;
    ClientLogic::instance()->pass2Engine(&infoGold);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "use_gold";
    infoUmeng.count = costGold;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);

    int index = 0;
    for (auto swordId : sword->swordConfigIdTable) {
        info.swordLock[index] = (sword->swordsMap[swordId.first].lock == 1);
        index++;
        if (index >= 5) {
            break;
        }
    }

    info.currSwordLevel = sword->swordsMap[currSwordId].level;
    info.nextSwordLockLevel = sword->swordConfigMap[currSwordId].lockLevel;
    info.totalHp = sword->getInfo().hp;
    info.totalAttack = sword->getInfo().attack;
    info.totalDefence = sword->getInfo().defence;
    info.totalStone = sword->getInfo().stoneCount;
    int originForce = account->getForce();
    account->changeHp(info.totalHp-tempHp);
    account->changeAttack(info.totalAttack-tempAttack);
    account->changeDefence(info.totalDefence-tempDefence);
    
    info.maxLevel = sword->isMaxLevel(currSwordId);
    if(!info.maxLevel){
        int nextLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level+1);
        info.nextHp = sword->levelConfigMap[nextLevel].deltaHp;
        info.nextAttack = sword->levelConfigMap[nextLevel].deltaAttack;
        info.nextDefence = sword->levelConfigMap[nextLevel].deltaDefence;
//        int currLevel = sword->getLevelId(currSwordId, sword->swordsMap[currSwordId].level);
        info.costStone = sword->levelConfigMap[nextLevel].costStone;
        info.costGold = sword->levelConfigMap[nextLevel].costGold;
    }else{
        info.nextHp = 0;
        info.nextAttack = 0;
        info.nextAttack = 0;
        info.costGold = 0;
        info.costStone = 0;
    }
    sword->checkTip(account->getData().gold);
    if (!sword->getTip()) {
        info.tipIndex = 0;
    }else{
        info.tipIndex = sword->getUplevelTip();
    }
    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeBoundMission(3, currSwordId, sword->swordsMap[currSwordId].level);
    
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 3;
    infoTip.tipValue = sword->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);

    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::showHallArms()
{
    L2E_SHOW_HALL_ARMS info;
    info.eProtocol = l2e_show_hall_arms;
    int index = 0;
    for (auto config : arms->armsConfigMap) {
        info.armsNames[index] = config.second.name;
        index++;
        if (index >= 5) {
            break;
        }
    }
    info.totalHp = arms->getTotalHp();
    info.totalAttack = arms->getTotalAttack();
    info.totalDefence = arms->getTotalDefence();
    info.totalCrit = arms->getTotalCrit();
    info.totalDecrit = arms->getTotalDecrit();
    
    if (currArmsId == 0) {
        currArmsId = 1;
    }
    info.currArmsId = currArmsId;
    info.armsIcon = arms->armsConfigMap[currArmsId].icon;
    info.armsType = arms->armsConfigMap[currArmsId].type;
    info.level = arms->armsMap[currArmsId].level;
    switch (info.armsType) {
        case 1:
        {
            info.armsValue = arms->armsMap[currArmsId].hp;
        }
            break;
        case 2:
        {
            info.armsValue = arms->armsMap[currArmsId].attack;
        }
            break;
        case 3:
        {
            info.armsValue = arms->armsMap[currArmsId].defence;
        }
            break;
        case 4:
        {
            info.armsValue = arms->armsMap[currArmsId].crit;
        }
            break;
        case 5:
        {
            info.armsValue = arms->armsMap[currArmsId].decrit;
        }
            break;
            
        default:
            break;
    }
    
    info.currFame = account->getData().fame;
    info.maxLevel = arms->isMaxLevel(currArmsId);
    if (!info.maxLevel) {
        int levelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level);
        info.costFame = arms->levelConfigMap[levelId].costFame;
        int nextLevelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level+1);
        switch (info.armsType) {
            case 1:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaHp;
            }
                break;
            case 2:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaAttack;
            }
                break;
            case 3:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDefence;
            }
                break;
            case 4:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaCrit;
            }
                break;
            case 5:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDecrit;
            }
                break;
                
            default:
                break;
        }

    }else{
        info.costFame = 0;
        info.nextLevelValue = 0;
    }
    
    memcpy(info.lockPos, arms->armsMap[currArmsId].lockPos, sizeof(int)*6);
    memcpy(info.unlockLevel, arms->armsConfigMap[currArmsId].unlockLevel, sizeof(int)*6);
    for (int i = 0; i < 6; i++) {
        int gemId = arms->armsMap[currArmsId].gemsId[i];
        if (gemId == 0) {
            info.posGemIcon[i] = "";
        }else{
            info.posGemIcon[i] = arms->gemsConfigMap[gemId].icon;
        }
    }
    
    std::map<int, int> gemIds;
    arms->filterGems(info.armsType, gemIds);
    int idx = 0;
    for (auto data : gemIds) {
        int gemId = data.second;
        info.gemsIcon[idx] = arms->gemsConfigMap[gemId].icon;
        info.gemCount[idx] = arms->gemCountMap[gemId];
        idx++;
        if (idx >= 6) {
            break;
        }
    }
    
    
    info.armsTip.clear();
    for (auto tip : arms->armsTip) {
        info.armsTip.push_back(tip);
    }
    info.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        info.armsFameTip.push_back(tip);
    }
    info.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        info.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::selArms(E2L_UPDATE_ONE_VALUE info)
{
    int selArms = info.value;
    L2E_UPDATE_ARMS infoOut;
    infoOut.eProtocol = l2e_update_arms;
    infoOut.errNo = 0;
    if (arms->armsConfigMap.find(selArms) == arms->armsConfigMap.end()) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    currArmsId = selArms;
    infoOut.currArmsId = currArmsId;
    infoOut.armsIcon = arms->armsConfigMap[currArmsId].icon;
    infoOut.armsType = arms->armsConfigMap[currArmsId].type;
    infoOut.level = arms->armsMap[currArmsId].level;
    switch (infoOut.armsType) {
        case 1:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].hp;
        }
        break;
        case 2:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].attack;
        }
        break;
        case 3:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].defence;
        }
        break;
        case 4:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].crit;
        }
        break;
        case 5:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].decrit;
        }
        break;
        
        default:
        break;
    }
    
    infoOut.currFame = account->getData().fame;
    infoOut.maxLevel = arms->isMaxLevel(currArmsId);
    if (!infoOut.maxLevel) {
        int levelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level);
        infoOut.costFame = arms->levelConfigMap[levelId].costFame;
        int nextLevelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level+1);
        switch (infoOut.armsType) {
            case 1:
            {
                infoOut.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaHp;
            }
            break;
            case 2:
            {
                infoOut.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaAttack;
            }
            break;
            case 3:
            {
                infoOut.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDefence;
            }
            break;
            case 4:
            {
                infoOut.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaCrit;
            }
            break;
            case 5:
            {
                infoOut.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDecrit;
            }
            break;
            
            default:
            break;
        }
        
    }else{
        infoOut.costFame = 0;
        infoOut.nextLevelValue = 0;
    }
    
    memcpy(infoOut.lockPos, arms->armsMap[currArmsId].lockPos, sizeof(int)*6);
    memcpy(infoOut.unlockLevel, arms->armsConfigMap[currArmsId].unlockLevel, sizeof(int)*6);
    for (int i = 0; i < 6; i++) {
        int gemId = arms->armsMap[currArmsId].gemsId[i];
        if (gemId == 0) {
            infoOut.posGemIcon[i] = "";
        }else{
            infoOut.posGemIcon[i] = arms->gemsConfigMap[gemId].icon;
        }
    }
    std::map<int, int> gemIds;
    arms->filterGems(infoOut.armsType, gemIds);
    int idx = 0;
    for (auto data : gemIds) {
        int gemId = data.second;
        infoOut.gemsIcon[idx] = arms->gemsConfigMap[gemId].icon;
        infoOut.gemCount[idx] = arms->gemCountMap[gemId];
        idx++;
        if (idx >= 6) {
            break;
        }
    }

    infoOut.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        infoOut.armsFameTip.push_back(tip);
    }
    infoOut.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        infoOut.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::uplevelArms()
{
    int tempHp = arms->getTotalHp();
    int tempAttack = arms->getTotalAttack();
    int tempDefence = arms->getTotalDefence();
    int tempCrit = arms->getTotalCrit();
    int tempDecrit = arms->getTotalDecrit();

    L2E_UPDATE_ARMS_LEVEL info;
    info.eProtocol = l2e_update_arms_level;
    info.errNo = 0;
    int levelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level);
    int costFame = arms->levelConfigMap[levelId].costFame;
    if (account->getData().fame < costFame) {
        info.errNo = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    info.errNo = arms->uplevelArms(currArmsId);
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    int originForce = account->getForce();
    account->changeFame(-costFame);
    info.currFame = account->getData().fame;
    account->changeHp(arms->getTotalHp()-tempHp);
    account->changeAttack(arms->getTotalAttack()-tempAttack);
    account->changeDefence(arms->getTotalDefence()-tempDefence);
    account->changeCrit(arms->getTotalCrit()-tempCrit);
    account->changeDeCrit(arms->getTotalDecrit()-tempDecrit);
    
    info.totalHp = arms->getTotalHp();
    info.totalAttack = arms->getTotalAttack();
    info.totalDefence = arms->getTotalDefence();
    info.totalCrit = arms->getTotalCrit();
    info.totalDecrit = arms->getTotalDecrit();
    info.level = arms->armsMap[currArmsId].level;
    switch (arms->armsConfigMap[currArmsId].type) {
        case 1:
        {
            info.armsValue = arms->armsMap[currArmsId].hp;
        }
        break;
        case 2:
        {
            info.armsValue = arms->armsMap[currArmsId].attack;
        }
        break;
        case 3:
        {
            info.armsValue = arms->armsMap[currArmsId].defence;
        }
        break;
        case 4:
        {
            info.armsValue = arms->armsMap[currArmsId].crit;
        }
        break;
        case 5:
        {
            info.armsValue = arms->armsMap[currArmsId].decrit;
        }
        break;
        
        default:
        break;
    }
    info.maxLevel = arms->isMaxLevel(currArmsId);
    if (!info.maxLevel) {
        int levelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level);
        info.costFame = arms->levelConfigMap[levelId].costFame;
        int nextLevelId = arms->getLevelId(currArmsId, arms->armsMap[currArmsId].level+1);
        switch (arms->armsConfigMap[currArmsId].type) {
            case 1:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaHp;
            }
            break;
            case 2:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaAttack;
            }
            break;
            case 3:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDefence;
            }
            break;
            case 4:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaCrit;
            }
            break;
            case 5:
            {
                info.nextLevelValue = arms->levelConfigMap[nextLevelId].deltaDecrit;
            }
            break;
            
            default:
            break;
        }
        
    }else{
        info.costFame = 0;
        info.nextLevelValue = 0;
    }
    memcpy(info.lockPos, arms->armsMap[currArmsId].lockPos, sizeof(int)*6);
    memcpy(info.unlockLevel, arms->armsConfigMap[currArmsId].unlockLevel, sizeof(int)*6);
    for (int i = 0; i < 6; i++) {
        int gemId = arms->armsMap[currArmsId].gemsId[i];
        if (gemId == 0) {
            info.posGemIcon[i] = "";
        }else{
            info.posGemIcon[i] = arms->gemsConfigMap[gemId].icon;
        }
    }

    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeBoundMission(4, currArmsId, arms->armsMap[currArmsId].level);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);
    
    arms->checkTip(account->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = arms->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_ARMS_TIP infoArmsTip;
    infoArmsTip.eProtocol = l2e_update_arms_tip;
    infoArmsTip.armsTip.clear();
    for (auto tip : arms->armsTip) {
        infoArmsTip.armsTip.push_back(tip);
    }
    infoArmsTip.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        infoArmsTip.armsFameTip.push_back(tip);
    }
    infoArmsTip.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        infoArmsTip.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoArmsTip);
    
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::assembleGem(E2L_UPDATE_ONE_VALUE info)
{
    int gemPos = info.value;
    std::map<int, int> gemIds;
    arms->filterGems(arms->armsConfigMap[currArmsId].type, gemIds);
    
    L2E_UPDATE_GEM_COUNT infoOut;
    infoOut.eProtocol = l2e_update_gems_count;
    infoOut.errNo = 0;
    infoOut.nextGemPos = 0;
    if (gemPos <= 0 || gemPos >= 6) {
        infoOut.errNo = 5;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    infoOut.nextGemPos = gemPos+1;
    int currGemId = gemIds[gemPos];
    infoOut.errNo = arms->assembleGem(currGemId);
    if (infoOut.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    int idx = 0;
    for (auto data : gemIds) {
        int gemId = data.second;
        infoOut.gemCount[idx] = arms->gemCountMap[gemId];
        idx++;
        if (idx >= 6) {
            break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    arms->checkTip(account->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = arms->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_ARMS_TIP infoArmsTip;
    infoArmsTip.eProtocol = l2e_update_arms_tip;
    infoArmsTip.armsTip.clear();
    for (auto tip : arms->armsTip) {
        infoArmsTip.armsTip.push_back(tip);
    }
    infoArmsTip.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        infoArmsTip.armsFameTip.push_back(tip);
    }
    infoArmsTip.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        infoArmsTip.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoArmsTip);
    
}
void SystemLogic::showFillGemBoard(E2L_UPDATE_ONE_VALUE info)
{
    int gemPos = info.value;
    std::map<int, int> gemIds;
    arms->filterGems(arms->armsConfigMap[currArmsId].type, gemIds);
    int currGemId = gemIds[gemPos];
    L2E_SHOW_FILL_GEM infoOut;
    infoOut.eProtocol = l2e_show_fill_board;
    infoOut.gemType = arms->armsConfigMap[currArmsId].type;
    infoOut.gemIcon = arms->gemsConfigMap[currGemId].icon;
    infoOut.gemCount = arms->gemCountMap[currGemId];
    infoOut.gemValue = arms->gemsConfigMap[currGemId].value;
    ClientLogic::instance()->pass2Engine(&infoOut);
}
void SystemLogic::fillGem(E2L_UPDATE_ONE_VALUE info)
{
    int tempHp = arms->getTotalHp();
    int tempAttack = arms->getTotalAttack();
    int tempDefence = arms->getTotalDefence();
    int tempCrit = arms->getTotalCrit();
    int tempDecrit = arms->getTotalDecrit();

    int gemPos = info.value;
    std::map<int, int> gemIds;
    arms->filterGems(arms->armsConfigMap[currArmsId].type, gemIds);
    int currGemId = gemIds[gemPos];
    
    L2E_UPDATE_ARMS_GEM infoOut;
    infoOut.eProtocol = l2e_update_arms_gem;
    infoOut.errNo = arms->fillGem(currArmsId, currGemId);
    int originForce = account->getForce();
    account->changeHp(arms->getTotalHp()-tempHp);
    account->changeAttack(arms->getTotalAttack()-tempAttack);
    account->changeDefence(arms->getTotalDefence()-tempDefence);
    account->changeCrit(arms->getTotalCrit()-tempCrit);
    account->changeDeCrit(arms->getTotalDecrit()-tempDecrit);
    
    infoOut.totalHp = arms->getTotalHp();
    infoOut.totalAttack = arms->getTotalAttack();
    infoOut.totalDefence = arms->getTotalDefence();
    infoOut.totalCrit = arms->getTotalCrit();
    infoOut.totalDecrit = arms->getTotalDecrit();

    switch (arms->armsConfigMap[currArmsId].type) {
        case 1:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].hp;
        }
        break;
        case 2:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].attack;
        }
        break;
        case 3:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].defence;
        }
        break;
        case 4:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].crit;
        }
        break;
        case 5:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].decrit;
        }
        break;
        
        default:
        break;
    }

    for (int i = 0; i < 6; i++) {
        int gemId = arms->armsMap[currArmsId].gemsId[i];
        if (gemId == 0) {
            infoOut.posGemIcon[i] = "";
        }else{
            infoOut.posGemIcon[i] = arms->gemsConfigMap[gemId].icon;
        }
    }

    int idx = 0;
    for (auto data : gemIds) {
        int gemId = data.second;
        infoOut.gemCount[idx] = arms->gemCountMap[gemId];
        idx++;
        if (idx >= 6) {
            break;
        }
    }
    memcpy(infoOut.lockPos, arms->armsMap[currArmsId].lockPos, sizeof(int)*6);
    memcpy(infoOut.unlockLevel, arms->armsConfigMap[currArmsId].unlockLevel, sizeof(int)*6);

    ClientLogic::instance()->pass2Engine(&infoOut);
    campaign->activeBoundMission(5, 0, arms->gemsConfigMap[currGemId].level);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);
    
    arms->checkTip(account->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = arms->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_ARMS_TIP infoArmsTip;
    infoArmsTip.eProtocol = l2e_update_arms_tip;
    infoArmsTip.armsTip.clear();
    for (auto tip : arms->armsTip) {
        infoArmsTip.armsTip.push_back(tip);
    }
    infoArmsTip.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        infoArmsTip.armsFameTip.push_back(tip);
    }
    infoArmsTip.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        infoArmsTip.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoArmsTip);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::removeGem(E2L_UPDATE_ONE_VALUE info)
{
    int tempHp = arms->getTotalHp();
    int tempAttack = arms->getTotalAttack();
    int tempDefence = arms->getTotalDefence();
    int tempCrit = arms->getTotalCrit();
    int tempDecrit = arms->getTotalDecrit();
    
    int gemPos = info.value;
//    int currGemId = arms->armsMap[currArmsId].gemsId[gemPos-1];
    
    L2E_UPDATE_ARMS_GEM infoOut;
    infoOut.eProtocol = l2e_update_arms_gem;
    infoOut.errNo = arms->removeGem(currArmsId, gemPos);
    int originForce = account->getForce();
    account->changeHp(arms->getTotalHp()-tempHp);
    account->changeAttack(arms->getTotalAttack()-tempAttack);
    account->changeDefence(arms->getTotalDefence()-tempDefence);
    account->changeCrit(arms->getTotalCrit()-tempCrit);
    account->changeDeCrit(arms->getTotalDecrit()-tempDecrit);
    
    infoOut.totalHp = arms->getTotalHp();
    infoOut.totalAttack = arms->getTotalAttack();
    infoOut.totalDefence = arms->getTotalDefence();
    infoOut.totalCrit = arms->getTotalCrit();
    infoOut.totalDecrit = arms->getTotalDecrit();
    
    switch (arms->armsConfigMap[currArmsId].type) {
        case 1:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].hp;
        }
        break;
        case 2:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].attack;
        }
        break;
        case 3:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].defence;
        }
        break;
        case 4:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].crit;
        }
        break;
        case 5:
        {
            infoOut.armsValue = arms->armsMap[currArmsId].decrit;
        }
        break;
        
        default:
        break;
    }
    
    for (int i = 0; i < 6; i++) {
        int gemId = arms->armsMap[currArmsId].gemsId[i];
        if (gemId == 0) {
            infoOut.posGemIcon[i] = "";
        }else{
            infoOut.posGemIcon[i] = arms->gemsConfigMap[gemId].icon;
        }
    }
    
    std::map<int, int> gemIds;
    arms->filterGems(arms->armsConfigMap[currArmsId].type, gemIds);
    int idx = 0;
    for (auto data : gemIds) {
        int gemId = data.second;
        infoOut.gemCount[idx] = arms->gemCountMap[gemId];
        idx++;
        if (idx >= 6) {
            break;
        }
    }
    memcpy(infoOut.lockPos, arms->armsMap[currArmsId].lockPos, sizeof(int)*6);
    memcpy(infoOut.unlockLevel, arms->armsConfigMap[currArmsId].unlockLevel, sizeof(int)*6);

    ClientLogic::instance()->pass2Engine(&infoOut);
    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal - originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);

    arms->checkTip(account->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = arms->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_ARMS_TIP infoArmsTip;
    infoArmsTip.eProtocol = l2e_update_arms_tip;
    infoArmsTip.armsTip.clear();
    for (auto tip : arms->armsTip) {
        infoArmsTip.armsTip.push_back(tip);
    }
    infoArmsTip.armsFameTip.clear();
    for (auto tip : arms->armsFameTip) {
        infoArmsTip.armsFameTip.push_back(tip);
    }
    infoArmsTip.gemTip.clear();
    for (auto tip : arms->armsGemIdTip) {
        infoArmsTip.gemTip.push_back(tip);
    }
    ClientLogic::instance()->pass2Engine(&infoArmsTip);
}

void SystemLogic::showHallProv()
{
    for (auto data : pet->petsMap) {
        int petId = data.second.petId;
        int petLevel = data.second.level;
        int levelId = pet->getLevelId(pet->petConfigMap[petId].quality, petLevel);
        int provHp = pet->levelConfigMap[levelId].provHp;
        int provCritRate = pet->levelConfigMap[levelId].provCritRate;
        int provCirtPer = pet->levelConfigMap[levelId].provCirtPercent;
        prov->pushPet(petId, petLevel, provHp, provCritRate, provCirtPer);
    }

    L2E_SHOW_HALL_PROV info;
    info.eProtocol = l2e_show_hall_prov;
    info.ticketCount = prov->getProvData().sweepTicketCount;
    info.resetCount = prov->getProvData().resetCount;
    
    info.stage = prov->getProvData().provingStage;
    info.petIcon = prov->provConfigMap[info.stage].petIcon;
    info.petMaxHp = prov->provConfigMap[info.stage].hp;
    info.petLevel = prov->provConfigMap[info.stage].level;
    info.boundGold = prov->provConfigMap[info.stage].dropGold;
    info.boundStone = prov->provConfigMap[info.stage].dropStone;
    info.petHp = prov->getProvData().petHp;
    
    int index = 0;
    for (auto data : prov->fighterData) {
        int petId = data.second.petId;
        int petLevel = data.second.petLevel;
        info.fighterId[index] = petId;
        info.fighterLock[index] = (petLevel>0?0:1);
        info.fighterAlive[index] = (data.second.petHp>0?1:0);
        info.fighterIcon[index] = pet->petConfigMap[petId].icon;
        index++;
        if (index >= 10) {
            break;
        }
    }
    info.stageIcon.clear();
    for (auto config : prov->provConfigMap) {
        info.stageIcon.push_back(config.second.stageIcon);
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_prov";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::confirmProvStage()
{
    L2E_PROV_CONFIRM_STAGE info;
    info.eProtocol = l2e_prov_confirm_stage;
    info.errNo = prov->confirmStage();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    info.stage = prov->getProvData().provingStage;
    info.petIcon = prov->provConfigMap[info.stage].petIcon;
    info.petMaxHp = prov->provConfigMap[info.stage].hp;
    info.petLevel = prov->provConfigMap[info.stage].level;
    info.boundGold = prov->provConfigMap[info.stage].dropGold;
    info.boundStone = prov->provConfigMap[info.stage].dropStone;
    info.petHp = prov->getProvData().petHp;
    
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::confirmProvFighter()
{
    L2E_PROV_CONFIRM_FIGHTER info;
    info.eProtocol = l2e_prov_confirm_fighter;
    info.errNo = prov->confirmFighter();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    info.fighterId = prov->getFighterId();
    if (info.fighterId < 0) {
        return;
    }
    info.fighterHp = prov->fighterData[info.fighterId].petHp;
    info.fighterMaxHp = prov->fighterData[info.fighterId].maxHp;
    info.fighterLevel = prov->fighterData[info.fighterId].petLevel;
    info.fighterIcon = pet->petConfigMap[info.fighterId].icon;
    
    info.ticketCount = prov->getProvData().sweepTicketCount;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::provFight()
{
    L2E_PROV_FIGHT info;
    info.eProtocol = l2e_prov_fight;
    prov->fight();
    info.attackDirect = prov->getProvData().attackDirect;
    if (info.attackDirect == 0) {
        int fightId = prov->getFighterId();
        info.defenceHp = prov->fighterData[fightId].petHp;
        info.defenceMaxHp = prov->fighterData[fightId].maxHp;
    }else{
        info.defenceHp = prov->getProvData().petHp;
        info.defenceMaxHp = prov->getProvData().petMaxHp;
    }
    
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::provEndFight()
{
    int attackDirect = prov->getProvData().attackDirect;
    if (attackDirect == 0) {
        int fightId = prov->getFighterId();
        int defenceHp = prov->fighterData[fightId].petHp;
        if (defenceHp <= 0) {
            L2E_PROV_LOSE info;
            info.eProtocol = l2e_prov_lose;
            info.fighterId = fightId;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    }else{
        int defenceHp = prov->getProvData().petHp;
        if (defenceHp <= 0) {
            L2E_PROV_WIN info;
            info.eProtocol = l2e_prov_win;
            info.boundGold = prov->provConfigMap[prov->getProvData().provingStage].dropGold;
            info.boundStone = prov->provConfigMap[prov->getProvData().provingStage].dropStone;
            ClientLogic::instance()->pass2Engine(&info);
            int stage = prov->getProvData().provingStage;
            account->changeGold(prov->provConfigMap[stage].dropGold);
            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = prov->provConfigMap[stage].dropGold;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);

            
            sword->addStone(prov->provConfigMap[stage].dropStone);
            campaign->activeMission(5, prov->provConfigMap[stage].provingId);
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
            
            campaign->checkTip();
            L2E_UPDATE_HALL_TIP infoTip1;
            infoTip1.eProtocol = l2e_update_hall_tip;
            infoTip1.tipIndex = 7;
            infoTip1.tipValue = campaign->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip1);
            return;
        }
    }
    provFight();
    
}

void SystemLogic::provEndWin()
{
    confirmProvStage();
}

void SystemLogic::provEndLose()
{
    confirmProvFighter();
}

void SystemLogic::provReset()
{
    L2E_PROV_RESET info;
    info.eProtocol = l2e_prov_reset;
    info.errNo = prov->resetProv();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.resetCount = prov->getProvData().resetCount;
    
    info.stage = prov->getProvData().provingStage;
    info.petIcon = prov->provConfigMap[info.stage].petIcon;
    info.petMaxHp = prov->provConfigMap[info.stage].hp;
    info.petLevel = prov->provConfigMap[info.stage].level;
    info.boundGold = prov->provConfigMap[info.stage].dropGold;
    info.boundStone = prov->provConfigMap[info.stage].dropStone;
    info.petHp = prov->getProvData().petHp;
    
    int index = 0;
    for (auto data : prov->fighterData) {
        int petLevel = data.second.petLevel;
        info.fighterLock[index] = (petLevel>0?0:1);
        info.fighterAlive[index] = (data.second.petHp>0?1:0);
        index++;
        if (index >= 10) {
            break;
        }
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::provBuyTicket(E2L_UPDATE_ONE_VALUE info)
{
    int count = info.value;
    L2E_PROV_ADD_TICKET infoOut;
    infoOut.eProtocol = l2e_prov_add_ticket;
    infoOut.errNo = 0;
    if (account->getData().diamond < provTicketPrice*count) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    account->changeDiamond(-provTicketPrice*count);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);

    prov->addTicket(count);
    infoOut.ticketCount = prov->getProvData().sweepTicketCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::provRefreshReset()
{
    prov->refreshProv();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_refresh_prov_reset;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_prov_refresh_reset;
    info.value = prov->getProvData().resetCount;
    ClientLogic::instance()->pass2Engine(&info);
    
}

void SystemLogic::showBuyVIT()
{
    L2E_SHOW_BUY_VIT info;
    info.eProtocol = l2e_show_buy_vit;
    info.price = vitPrice;
    info.buyCount = vit->getBuyCount();
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::showHallBuyVIT()
{
    L2E_SHOW_BUY_VIT info;
    info.eProtocol = l2e_show_hall_buy_vit;
    info.price = vitPrice;
    info.buyCount = vit->getBuyCount();
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::buyVIT()
{
    L2E_BUY_VIT info;
    info.eProtocol = l2e_buy_vit;
    if (account->getData().diamond < vitPrice) {
        info.errNo = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    info.errNo = vit->buyVIT();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.buyCount = vit->getBuyCount();
    ClientLogic::instance()->pass2Engine(&info);
    
    account->changeDiamond(-vitPrice);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
    
    L2E_UPDATE_VIT infoVIT;
    infoVIT.eProtocol = l2e_update_vit;
    infoVIT.currVIT = vit->getTotalVIT();
    infoVIT.maxVIT = vit->getMaxBaseVIT();
    ClientLogic::instance()->pass2Engine(&infoVIT);
}

void SystemLogic::resetVITBuyCount()
{
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_reset_vit_buy_count;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_update_vit_buy_count;
    info.value = vit->getBuyCount();
    ClientLogic::instance()->pass2Engine(&info);

}

void SystemLogic::showHallAbyss()
{
    L2E_SHOW_HALL_ABYSS info;
    info.eProtocol = l2e_show_hall_abyss;
    int index = 0;
    Chapter::instance()->readStageConfig(100);
    for (auto config : abyss->abyssConfigMap) {
        info.abyssId[index] = config.second.abyssId;
        info.abyssAnimation[index] = config.second.animationFile;
        int stageId = config.second.stageId;
        int fragId = config.second.petFragId;
        info.abyssNameImg[index] = Chapter::instance()->stageConfigMap[stageId].iconFile;
        info.boundGold[index] = Chapter::instance()->stageConfigMap[stageId].boundsGold;
        info.boundExp[index] = Chapter::instance()->stageConfigMap[stageId].boundsExp;
        info.petFragIcon[index] = pet->petConfigMap[fragId].icon;
        info.petFragCount[index] = config.second.petFragCount;
        info.fightCount[index] = abyss->getAbyssData().fightCount[index];
        index++;
        if (index >= 10) {
            break;
        }
    }
    info.focus = ChapterLogic::instance()->getCurrAbyss();
    info.openAbyssCount = abyss->getAbyssData().openStageCount;
    info.currVIT = vit->getTotalVIT();
    info.maxVIT = vit->getMaxBaseVIT();
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::initArenaTopRobot()
{
    for(auto config : arena->robotConfigMap)
    {
        // 等级
        int hp = 720;
        int attack = 24;
        int defence = 16;
        int crit = 0;
        int decrit = 0;
        for (int i = 2; i <= config.second.level; i++) {
            attack += account->uplevelConfigMap[i].deltaAttack;
            defence += account->uplevelConfigMap[i].deltaDefence;
            crit += account->uplevelConfigMap[i].deltaCrit;
            decrit += account->uplevelConfigMap[i].deltaDeCrit;
            hp += account->uplevelConfigMap[i].deltaHp;
        }
        
        // 神兵，10级为最大，满10级才会解锁下一个神兵
        int swordLevel = config.second.holySwordLevel;
        int swordCount = swordLevel/10+1;
        swordCount = MIN((int)sword->swordConfigMap.size(), swordCount);
        // 加神兵级别
        for (int i = 0; i < swordLevel; i++) {
            int levelId = sword->getLevelId(i/10+1, i%10+1);
            hp += sword->levelConfigMap[levelId].deltaHp;
            attack += sword->levelConfigMap[levelId].deltaAttack;
            defence += sword->levelConfigMap[levelId].deltaDefence;
        }
        
        // 使魔
        std::map<int, int> totalPet;
        for (int i = 0; i < 5; i++)
        {
            // 获得同品质使魔的所有id
            std::vector<int> petId;
            pet->getPetsWithQuality(i+1, petId);
            // 随机拥有指定个数的使魔
            int qualityPetCount = (int)petId.size();
            int count = config.second.petCount[i];
            int start = abs(rand())%qualityPetCount;
//            for (int j = 0; j < count; j++) {
//                totalPet.push_back(petId[(j+start)%qualityPetCount]);
//            }
            
            // 根据等级，加属性
            for (int j = 0; j < count; j++) {
                int level = config.second.petLevel[i][j];
                totalPet[petId[(j+start)%qualityPetCount]] = level;
                for (int k = 0; k < level; k++) {
                    int levelId = pet->getLevelId(i+1, k+1);
                    hp += pet->levelConfigMap[levelId].deltaHp;
                    attack += pet->levelConfigMap[levelId].deltaAttack;
                    defence += pet->levelConfigMap[levelId].deltaDefence;
                }
            }
        }
        
        std::string name = randomName->getRandomName();
        arena->addRobot(config.second.robotId, swordCount, totalPet, name, abs(rand())%account->jobConfigMap.size()+1, hp, attack, defence, crit, decrit);
    }
}

void SystemLogic::matchArenaOpponent()
{
    if (arena->getCoreData().place <= 20) {
        // 上榜玩家匹配top30里的前5后10机器人
        arena->matchRobotInTop30(robot);
    }else{
        // 未上榜玩家由积分档次匹配随机机器人
        int division = arena->getCoreData().divisionId;
        int maxSowrdId = arena->divistionConfigMap[division].maxSwordId;
        int swordId = abs(rand())%maxSowrdId;
        swordId = MAX(1, swordId);
        // 使魔
        std::vector<int> totalPet;
        for (int i = 0; i < 5; i++)
        {
            int count = arena->divistionConfigMap[division].petCount[i];
            if (count <= 0) {
                continue;
            }
            // 获得同品质使魔的所有id
            std::vector<int> petId;
            pet->getPetsWithQuality(i+1, petId);
            // 随机拥有指定个数的使魔
            int qualityPetCount = (int)petId.size();
            int start = abs(rand())%qualityPetCount;
            
            // 根据等级，加属性
            for (int j = 0; j < count; j++) {
                totalPet.push_back(petId[(j+start)%qualityPetCount]);
            }
        }
        
        std::string name = randomName->getRandomName();
        int jobId = abs(rand())%account->jobConfigMap.size()+1;
//        int jobId = abs(rand())%2+1;
        
        arena->matchRobotByDivision(division, jobId, name, swordId, totalPet, robot);
    }
    
    arena->matchRobot(robot);
    log("SystemLogic::matchArenaOpponent finish!");
}

void SystemLogic::updateArenaRank()
{
    int currSword = sword->getInfo().fightSwordId;
    int petLevel[3];
    memset(petLevel, 0, sizeof(int)*3);
    int index = 0;
    for (auto petId : pet->getInfo().fightPet) {
        if (petId <= 0) {
            index++;
            continue;
        }
        petLevel[index] = pet->petsMap[petId].level;
        index++;
    }
    arena->refreshSelfRobot(account->getData().currJob, account->getData().name,
                            account->getData().level, currSword,
                            pet->getInfo().fightPet, petLevel,
                            account->getData().hp, account->getData().attack,
                            account->getData().defence, account->getData().crit, account->getData().deCrit);
}

void SystemLogic::showHallArena()
{
    updateArenaRank();
    L2E_SHOW_HALL_ARENA info;
    info.eProtocol = l2e_show_hall_arena;
    info.place = arena->getCoreData().place;
    info.score = arena->getCoreData().score;
    info.ticketCount = arena->getCoreData().ticketCount;
    info.maxTicketCount = arena->getCoreData().maxTicketCount;
    info.divisionIcon1 = arena->divistionConfigMap[arena->getCoreData().divisionId].icon1;
    info.divisionIcon2 = arena->divistionConfigMap[arena->getCoreData().divisionId].icon2;
    info.force = account->getForce();
    
    int swordId = sword->getInfo().fightSwordId;
    info.swordIcon = sword->swordConfigMap[swordId].icon;
    for (int i = 0; i < 3; i++) {
        int petId = pet->getInfo().fightPet[i];
        if (petId > 0) {
            info.petIcon[i] = pet->petConfigMap[petId].icon;
            info.petLevel[i] = pet->petsMap[petId].level;
        }else{
            info.petIcon[i] = "";
            info.petLevel[i] = 0;
        }
    }
    info.cd = 0;
    
    for (int i = 0; i < 20; i++) {
        info.robotPlace[i] = arena->robotMap[i].place;
        info.robotLevel[i] = arena->robotMap[i].level;
        info.robotScore[i] = arena->robotMap[i].score;
        info.robotName[i] = arena->robotMap[i].name;
        info.robotJobIcon[i] = account->jobConfigMap[arena->robotMap[i].jobId].headIcon;
        info.robotForce[i] = account->calForce(arena->robotMap[i].hp,
                                               arena->robotMap[i].attack,
                                               arena->robotMap[i].defence,
                                               arena->robotMap[i].crit,
                                               arena->robotMap[i].decrit);
        int swordId = arena->robotMap[i].holySwordId;
        info.robotSwordIcon[i] = sword->swordConfigMap[swordId].icon;
        memcpy(info.robotPetLevel[i], arena->robotMap[i].petLevel, sizeof(int)*3);
        for (int j = 0; j < 3; j++) {
            int petId = arena->robotMap[i].petId[j];
            if (petId > 0) {
                info.robotPetIcon[i][j] = pet->petConfigMap[petId].icon;
            }else{
                info.robotPetIcon[i][j] = "";
            }
            
        }
    }
    info.rewardState = arena->getCoreData().rewardState;
    info.rewardGold = arena->divistionConfigMap[arena->getCoreData().divisionId].rewardGold;
    info.rewardFame = arena->divistionConfigMap[arena->getCoreData().divisionId].rewardFame;
    
    ClientLogic::instance()->pass2Engine(&info);
    
//    L2T_COMMON infoTime;
//    infoTime.eProtocol = l2t_get_arena_ticket_cd;
//    ClientLogic::instance()->pass2Time(&infoTime);
}

void SystemLogic::showArenaDivision()
{
    L2E_SHOW_ARENA_DIVISION info;
    info.eProtocol = l2e_show_arena_division;
    for (int i = 0; i < 30; i++) {
        info.icon1[i] = arena->divistionConfigMap[i+1].icon1;
        info.icon2[i] = arena->divistionConfigMap[i+1].icon2;
        info.goldCount[i] = arena->divistionConfigMap[i+1].rewardGold;
        info.fameCount[i] = arena->divistionConfigMap[i+1].rewardFame;
        info.minScore[i] = arena->divistionConfigMap[i+1].minScore;
        info.maxScore[i] = arena->divistionConfigMap[i+1].maxScore;
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::getArenaCD()
{
    L2T_COMMON info;
    info.eProtocol = l2t_get_arena_ticket_cd;
    ClientLogic::instance()->pass2Time(&info);
}

void SystemLogic::startMatchOpp()
{
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_arena_start_match;
    info.value = arena->startFight();
    ClientLogic::instance()->pass2Engine(&info);
    
    if (arena->getCoreData().ticketCount < arena->getCoreData().maxTicketCount) {
        L2T_RESTORE_ARENA_TICKET info;
        info.eProtocol = l2t_restore_arena_ticket;
        info.restoreSec = arenaTicketRestoreSec;
        ClientLogic::instance()->pass2Time(&info);
    }
}

void SystemLogic::arenaMatchRobot()
{
    matchArenaOpponent();
    L2E_ARENA_SHOW_ROBOT info;
    info.eProtocol = l2e_arena_show_robot;
    info.heroJobIcon = account->jobConfigMap[account->getData().currJob].pvpImg;
    info.heroName = account->getData().name;
    info.heroForce = account->getForce();
    if (sword->getInfo().fightSwordId > 0) {
        info.heroSwordImg = sword->swordConfigMap[sword->getInfo().fightSwordId].pvpImg;
        info.heroSwordName = sword->swordConfigMap[sword->getInfo().fightSwordId].name;
    }else{
        info.heroSwordImg = "";
        info.heroSwordName = "";
    }
    
    for (int i = 0; i < 3; i++) {
        int petId = pet->getInfo().fightPet[i];
        if (petId > 0) {
            info.heroPetImg[i] = pet->petConfigMap[petId].pvpImg;
            info.heroPetName[i] = pet->petConfigMap[petId].name;
        }else{
            info.heroPetImg[i] = "";
            info.heroPetName[i] = "";
        }
    }
    
    info.oppJobIcon = account->jobConfigMap[robot.jobId].pvpImg;
    info.oppName = robot.name;
    info.oppForce = account->calForce(robot.hp, robot.attack, robot.defence, robot.crit, robot.decrit);
    if (robot.holySwordId > 0) {
        info.oppSwordImg = sword->swordConfigMap[robot.holySwordId].pvpImg;
        info.oppSwordName = sword->swordConfigMap[robot.holySwordId].name;
    }else{
        info.oppSwordImg = "";
        info.oppSwordName = "";
    }
    
    for (int i = 0; i < 3; i++) {
        int petId = robot.petId[i];
        if (petId > 0) {
            info.oppPetImg[i] = pet->petConfigMap[petId].pvpImg;
            info.oppPetName[i] = pet->petConfigMap[petId].name;
        }else{
            info.oppPetImg[i] = "";
            info.oppPetName[i] = "";
        }
    }
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::buyArenaTicket()
{
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_buy_arena_ticket;
    info.value = 0;
    if (account->getData().diamond < arenaTicketPrice) {
        info.value = 1;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    arena->addTicket(1);
    if (arena->getCoreData().ticketCount >= arena->getCoreData().maxTicketCount) {
        L2T_COMMON info;
        info.eProtocol = l2t_stop_arena_ticket_restore;
        ClientLogic::instance()->pass2Time(&info);
    }
    
    L2E_UPDATE_ARENA_TICKET infoOut;
    infoOut.eProtocol = l2e_update_arena_ticket;
    infoOut.currCount = arena->getCoreData().ticketCount;
    infoOut.maxCount = arena->getCoreData().maxTicketCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    account->changeDiamond(-arenaTicketPrice);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
}

void SystemLogic::updateArenaTicketCD(T2L_COMMON info)
{
    L2E_UPDATE_ONE_VALUE infoOut;
    infoOut.eProtocol = l2e_update_arena_ticket_cd;
    infoOut.value = (int)info.passSec;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::refreshArenaReward()
{
    arena->reward();
    
    int divisionId = arena->getCoreData().divisionId;
    L2E_ARENA_REWARD info;
    info.eProtocol = l2e_arena_reward;
    info.gold = arena->divistionConfigMap[divisionId].rewardGold;
    info.fame = arena->divistionConfigMap[divisionId].rewardFame;
    info.divisionIcon1 = arena->divistionConfigMap[divisionId].icon1;
    info.divisionIcon2 = arena->divistionConfigMap[divisionId].icon2;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::takeArenaReward()
{
    int divisionId = arena->getCoreData().divisionId;
    arena->takeReward();
    account->changeGold(arena->divistionConfigMap[divisionId].rewardGold);
    L2E_UPDATE_ONE_VALUE infoGold;
    infoGold.eProtocol = l2e_update_gold;
    infoGold.value = account->getData().gold;
    ClientLogic::instance()->pass2Engine(&infoGold);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "get_gold";
    infoUmeng.count = arena->divistionConfigMap[divisionId].rewardGold;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);

    
    account->changeFame(arena->divistionConfigMap[divisionId].rewardFame);
//    L2E_UPDATE_ONE_VALUE infoFame;
//    infoGold.eProtocol = l2e_update_fame;
//    infoGold.value = account->getData().fame;
//    ClientLogic::instance()->pass2Engine(&infoFame);
    
    arms->checkTip(account->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = arms->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);

}
void SystemLogic::triggerUnlockFunc(E2L_UPDATE_ONE_VALUE info)
{
    account->unlock(info.value);
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 10) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.value) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}
void SystemLogic::triggerClickButton(E2L_TRIGGER_CLICK_BUTTON info)
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 2) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.index &&
                    plotMission->missionConfigMap[missionId].triggerParam[i][1] == info.buttonId) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}
void SystemLogic::triggerOpenLayer(E2L_TRIGGER_OPEN_LAYER info)
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 1) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.index) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}

void SystemLogic::triggerSetupHero()
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 3) {
                continue;
            }
            if(plotMission->completeTrigger(missionId, i+1, false)) {
                completeMission = missionId;
            }
            break;
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}

void SystemLogic::triggerPassNpc(E2L_TRIGGER_PASS_NPC info)
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 7) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.npcId) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}

void SystemLogic::triggerCreateMonster(E2L_TRIGGER_CREATE_MONSTER info)
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 4) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.monsterId) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}

bool SystemLogic::triggerKillMonster(E2L_TRIGGER_KILL_MONSTER info)
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 6) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == info.monsterId) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
                break;
            }
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
    return (completeMission != 0);
}

void SystemLogic::triggerFightVictory()
{
    int completeMission = 0;
    for (auto missionId : plotMission->activeMission) {
        int count = plotMission->missionConfigMap[missionId].triggerCount;
        for (int i = 0; i < count; i++) {
            if (plotMission->missionConfigMap[missionId].triggerType[i] != 9) {
                continue;
            }
            if (plotMission->missionConfigMap[missionId].triggerParam[i][0] == GameCore::instance()->getStageId()) {
                if(plotMission->completeTrigger(missionId, i+1, false)) {
                    completeMission = missionId;
                }
            }
            break;
        }
    }
    switch(plotMission->missionMap[completeMission].state)
    {
        case 2:
        case 4:
        {
            int plotId = plotMission->getPlotId();
            int plotFileId = plotMission->getPlotFileId();
            plot->readPlotConfig(plotFileId, plotId);
            std::vector<int> plotIdVec;
            plot->getNextStep(plotIdVec);
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], completeMission);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(completeMission);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = completeMission;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = completeMission;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = completeMission;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);
            }
        }
            break;
        case 3:
        {
            
        }
            break;
    }
}

void SystemLogic::plotComplete(E2L_COMPLETE_PLOT info)
{
    int missionId = info.missionId;
    if (plot->completSynPlot()) {
        std::vector<int> plotIdVec;
        if (plot->getNextStep(plotIdVec)) {
            for (auto plotId : plotIdVec) {
                if (plot->plotMap[plotId].type == 12) {
                    GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], missionId);
                    continue;
                }else if (plot->plotMap[plotId].type == 1){
                    dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                             plot->plotMap[plotId].param[0]);
                    dialog->setMissionId(missionId);
                    
                    L2E_SHOW_DIALOG infoPlot;
                    infoPlot.eProtocol = l2e_show_dialog;
                    infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                    infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                    infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                    infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                    ClientLogic::instance()->pass2Engine(&infoPlot);
                    continue;
                }else if (plot->plotMap[plotId].type == 7){
                    L2E_ADD_NPC infoNpc;
                    infoNpc.eProtocol = l2e_add_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                    infoNpc.posX = plot->plotMap[plotId].param[1];
                    infoNpc.direct = plot->plotMap[plotId].param[2];
                    infoNpc.missionId = missionId;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if (plot->plotMap[plotId].type == 8){
                    L2E_REMOVE_NPC infoNpc;
                    infoNpc.eProtocol = l2e_remove_npc;
                    infoNpc.npcId = plot->plotMap[plotId].param[0];
                    infoNpc.missionId = missionId;
                    ClientLogic::instance()->pass2Engine(&infoNpc);
                    continue;
                }else if(plot->plotMap[plotId].type == 20){
                    L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                    infoPackage.eProtocol = l2e_show_custom_package;
                    std::vector<int> packageIdVec;
                    shop->getShopByType(9, packageIdVec);
                    if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                  plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                        infoPackage.shopId = plot->plotMap[plotId].param[0];
                        infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                        infoPackage.missionId = missionId;
                        ClientLogic::instance()->pass2Engine(&infoPackage);
                        continue;
                    }
                }
                
                L2E_START_PLOT infoPlot;
                infoPlot.eProtocol = l2e_start_plot;
                infoPlot.missionId = missionId;
                infoPlot.type = plot->plotMap[plotId].type;
                memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                ClientLogic::instance()->pass2Engine(&infoPlot);

            }
        }else{
            L2E_COMMON infoPlot;
            infoPlot.eProtocol = l2e_complete_plot;
            ClientLogic::instance()->pass2Engine(&infoPlot);
            
            plotMission->completePlot(missionId);
            switch(plotMission->missionMap[missionId].state)
            {
                case 2:
                case 4:
                {
                    int plotId = plotMission->getPlotId();
                    int plotFileId = plotMission->getPlotFileId();
                    plot->readPlotConfig(plotFileId, plotId);
                    std::vector<int> plotIdVec;
                    plot->getNextStep(plotIdVec);
                    for (auto plotId : plotIdVec) {
                        if (plot->plotMap[plotId].type == 12) {
                            GameCore::instance()->plotOpenSendMonster(plot->plotMap[plotId].param[0], missionId);
                            continue;
                        }else if (plot->plotMap[plotId].type == 1){
                            dialog->readDialogConfig(plot->plotMap[plotId].param[1],
                                                     plot->plotMap[plotId].param[0]);
                            dialog->setMissionId(missionId);
                            
                            L2E_SHOW_DIALOG infoPlot;
                            infoPlot.eProtocol = l2e_show_dialog;
                            infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
                            infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
                            infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
                            infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
                            ClientLogic::instance()->pass2Engine(&infoPlot);
                            continue;
                        }else if (plot->plotMap[plotId].type == 7){
                            L2E_ADD_NPC infoNpc;
                            infoNpc.eProtocol = l2e_add_npc;
                            infoNpc.npcId = plot->plotMap[plotId].param[0];
                            infoNpc.armaFile = plotMission->npcConfigMap[infoNpc.npcId].armaFile;
                            infoNpc.posX = plot->plotMap[plotId].param[1];
                            infoNpc.direct = plot->plotMap[plotId].param[2];
                            infoNpc.missionId = missionId;
                            ClientLogic::instance()->pass2Engine(&infoNpc);
                            continue;
                        }else if (plot->plotMap[plotId].type == 8){
                            L2E_REMOVE_NPC infoNpc;
                            infoNpc.eProtocol = l2e_remove_npc;
                            infoNpc.npcId = plot->plotMap[plotId].param[0];
                            infoNpc.missionId = missionId;
                            ClientLogic::instance()->pass2Engine(&infoNpc);
                            continue;
                        }else if(plot->plotMap[plotId].type == 20){
                            L2E_SHOW_CUSTOM_PACKAGE infoPackage;
                            infoPackage.eProtocol = l2e_show_custom_package;
                            std::vector<int> packageIdVec;
                            shop->getShopByType(9, packageIdVec);
                            if (std::find(packageIdVec.begin(), packageIdVec.end(),
                                          plot->plotMap[plotId].param[0]) != packageIdVec.end()) {
                                infoPackage.shopId = plot->plotMap[plotId].param[0];
                                infoPackage.showRes = shop->shopConfigMap[infoPackage.shopId].showRes;
                                infoPackage.missionId = missionId;
                                ClientLogic::instance()->pass2Engine(&infoPackage);
                                continue;
                            }
                        }
                        
                        L2E_START_PLOT infoPlot;
                        infoPlot.eProtocol = l2e_start_plot;
                        infoPlot.missionId = missionId;
                        infoPlot.type = plot->plotMap[plotId].type;
                        memcpy(infoPlot.param, plot->plotMap[plotId].param, sizeof(int)*5);
                        ClientLogic::instance()->pass2Engine(&infoPlot);
                    }
                }
                    break;
                case 3:
                {
                    
                }
                    break;
            }
        }
    }
    
}

void SystemLogic::nextDialog()
{
    if (dialog->getNextLine()) {
        L2E_SHOW_DIALOG infoPlot;
        infoPlot.eProtocol = l2e_update_dialog;
        infoPlot.icon = dialog->segMap[dialog->getSegId()].faceIcon;
        infoPlot.name = dialog->segMap[dialog->getSegId()].dialogName;
        infoPlot.content = dialog->lineMap[dialog->getLineId()].content;
        infoPlot.direct = dialog->segMap[dialog->getSegId()].direct;
        ClientLogic::instance()->pass2Engine(&infoPlot);
    }else{
        L2E_COMMON info;
        info.eProtocol = l2e_complete_dialog;
        ClientLogic::instance()->pass2Engine(&info);
        
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = dialog->getMissionId();
        infoPlot.value = 0;
        plotComplete(infoPlot);
    }
}

void SystemLogic::HallIn()
{
    int unlockId = 0;
    for (auto data : account->unlockMap) {
        if (data.second.lock == 2)
        {
            unlockId = data.first;
            break;
        }
    }
    if (unlockId == 0) {
        if (toHall) {
            if (everydayShow == 1) {
                if (plotMission->getPlotFileId() == 0) {
                    if(campaign->getCoreData().everydayState == 1)
                    {
                        everydayShow = 2;
                        showCampaignEveryday();
                    }
                }
                
            }
            
        }else{
            toHall = true;
        }
        return;
    }
    toHall = true;
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_unlock_func;
    info.value = unlockId;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::showCampaignEveryday()
{
    L2E_SHOW_EVERYDAY info;
    info.eProtocol = l2e_show_hall_everyday;
    info.everydayState = campaign->getCoreData().everydayState;
    info.everydayId = campaign->getCoreData().everydayId;
    for (int i = 0; i < 7; i++) {
        info.count[i] = campaign->everydayConfigMap[i+1].boundCount;
        switch (campaign->everydayConfigMap[i+1].boundType) {
            case 1:
            {
                info.icon[i] = "jinbi1.png";
                info.name[i] = StringData::shared()->stringFromKey("gold");
            }
                break;
            case 2:
            {
                info.icon[i] = "zuanshi1.png";
                info.name[i] = StringData::shared()->stringFromKey("diamond");
            }
                break;
            case 3:
            {
                info.icon[i] = "tilibiao.png";
                info.name[i] = StringData::shared()->stringFromKey("VIT");
            }
                break;
            case 4:
            {
                info.icon[i] = "hunshi.png";
                info.name[i] = StringData::shared()->stringFromKey("stone");
            }
                break;
            case 5:
            {
                info.icon[i] = "shengwang.png";
                info.name[i] = StringData::shared()->stringFromKey("fame");
            }
                break;
            case 6:
            {
                info.icon[i] = "saodangquan.png";
                info.name[i] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                info.icon[i] = arms->gemsConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("gem");
            }
                break;
            case 8:
            {
                info.icon[i] = equip->equipItemConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
            }
                break;
            case 9:
            {
                info.icon[i] = pet->petConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = pet->petConfigMap[campaign->everydayConfigMap[i+1].boundId].name;
            }
                break;
            case 10:
            {
                info.name[i] = StringData::shared()->stringFromKey("normal_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            }
                break;
            case 11:
            {
                info.name[i] = StringData::shared()->stringFromKey("special_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            }
                break;
                
            default:
                break;
        }
    }
    
    info.activeTip = campaign->getActiveTip();
    info.boundTip = campaign->getBoundTip();
    info.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::showEveryday()
{
    L2E_SHOW_EVERYDAY info;
    info.eProtocol = l2e_show_everyday;
    info.everydayState = campaign->getCoreData().everydayState;
    info.everydayId = campaign->getCoreData().everydayId;
    for (int i = 0; i < 7; i++) {
        info.count[i] = campaign->everydayConfigMap[i+1].boundCount;
        switch (campaign->everydayConfigMap[i+1].boundType) {
            case 1:
            {
                info.icon[i] = "jinbi1.png";
                info.name[i] = StringData::shared()->stringFromKey("gold");
            }
                break;
            case 2:
            {
                info.icon[i] = "zuanshi1.png";
                info.name[i] = StringData::shared()->stringFromKey("diamond");
            }
                break;
            case 3:
            {
                info.icon[i] = "tilibiao.png";
                info.name[i] = StringData::shared()->stringFromKey("VIT");
            }
                break;
            case 4:
            {
                info.icon[i] = "hunshi.png";
                info.name[i] = StringData::shared()->stringFromKey("stone");
            }
                break;
            case 5:
            {
                info.icon[i] = "shengwang.png";
                info.name[i] = StringData::shared()->stringFromKey("fame");
            }
                break;
            case 6:
            {
                info.icon[i] = "saodangquan.png";
                info.name[i] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                info.icon[i] = arms->gemsConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("gem");
            }
                break;
            case 8:
            {
                info.icon[i] = equip->equipItemConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
            }
                break;
            case 9:
            {
                info.icon[i] = pet->petConfigMap[campaign->everydayConfigMap[i+1].boundId].icon;
                info.name[i] = pet->petConfigMap[campaign->everydayConfigMap[i+1].boundId].name;
            }
                break;
            case 10:
            {
                info.name[i] = StringData::shared()->stringFromKey("normal_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            }
                break;
            case 11:
            {
                info.name[i] = StringData::shared()->stringFromKey("special_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            }
                break;
    
            default:
                break;
        }
    }
    
    info.activeTip = campaign->getActiveTip();
    info.boundTip = campaign->getBoundTip();
    info.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::takeEverydayBound()
{
    if (campaign->getCoreData().everydayState == 2) {
        return;
    }

    int everydayId = campaign->getCoreData().everydayId;
    int count = campaign->everydayConfigMap[everydayId].boundCount;
    int boundId = campaign->everydayConfigMap[everydayId].boundId;
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    infoGet.count[0] = count;
    infoGet.frag[0] = false;
    switch (campaign->everydayConfigMap[everydayId].boundType)
    {
        case 1:
        {
            account->changeGold(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-16.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("gold");

            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = count;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);

        }
            break;
        case 2:
        {
            account->changeDiamond(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
        }
            break;
        case 3:
        {
            vit->addPlusVIT(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "tilibiao.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("VIT");
            L2E_UPDATE_VIT infoVIT;
            infoVIT.eProtocol = l2e_update_vit;
            infoVIT.currVIT = vit->getTotalVIT();
            infoVIT.maxVIT = vit->getMaxBaseVIT();
            ClientLogic::instance()->pass2Engine(&infoVIT);
        }
            break;
        case 4:
        {
            sword->addStone(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "hunshi.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("stone");
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 5:
        {
            account->changeFame(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "shengwang.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("fame");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 6:
        {
            prov->addTicket(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "saodangquan.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("prov_ticket");
        }
            break;
        case 7:
        {
            arms->addGem(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("gem");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 8:
        {
            equip->addItem(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("equip_item_fragment");
            
            equip->checkTip(account->getData().level);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 1;
            infoTip.tipValue = equip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 9:
        {
            infoGet.frag[0] = true;
            pet->addFragment(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("pet_fragment");
            
            pet->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 2;
            infoTip.tipValue = pet->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 10:
        {
            lucky->addNormalTicket(count);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("normal_draw_ticket");
        }
            break;
        case 11:
        {
            lucky->addSpecialTicket(count);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("special_draw_ticket");
        }
            break;

    }
    ClientLogic::instance()->pass2Engine(&infoGet);

    campaign->takeEverydayBound();
    showEveryday();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_refresh_everyday;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "seven_day";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    umengInfo.attrMap["day"] = Convert2String(everydayId);
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::refreshEveryday()
{
    campaign->enableEveryday();
}

void SystemLogic::showMonthAssign()
{
    L2E_SHOW_MONTH_ASSIGN info;
    info.eProtocol = l2e_show_month_assign;
    info.monthAssignState = campaign->getCoreData().monthAssignState;
    info.monthAssignId = campaign->getCoreData().monthAssignId;
    info.configCount = (int)campaign->monthAssignConfigMap.size();
    memset(info.count, 0, sizeof(int)*40);
    memset(info.vipLimit, 0, sizeof(int)*40);
    for (int i = 0; i < info.configCount; i++) {
        info.count[i] = campaign->monthAssignConfigMap[i+1].boundCount;
        info.vipLimit[i] = campaign->monthAssignConfigMap[i+1].vipLimit;
        switch (campaign->monthAssignConfigMap[i+1].boundType) {
            case 1:
            {
                info.icon[i] = "jinbi1.png";
                info.name[i] = StringData::shared()->stringFromKey("gold");
            }
                break;
            case 2:
            {
                info.icon[i] = "zuanshi1.png";
                info.name[i] = StringData::shared()->stringFromKey("diamond");
            }
                break;
            case 3:
            {
                info.icon[i] = "tilibiao.png";
                info.name[i] = StringData::shared()->stringFromKey("VIT");
            }
                break;
            case 4:
            {
                info.icon[i] = "hunshi.png";
                info.name[i] = StringData::shared()->stringFromKey("stone");
            }
                break;
            case 5:
            {
                info.icon[i] = "shengwang.png";
                info.name[i] = StringData::shared()->stringFromKey("fame");
            }
                break;
            case 6:
            {
                info.icon[i] = "saodangquan.png";
                info.name[i] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                info.icon[i] = arms->gemsConfigMap[campaign->monthAssignConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("gem");
            }
                break;
            case 8:
            {
                info.icon[i] = equip->equipItemConfigMap[campaign->monthAssignConfigMap[i+1].boundId].icon;
                info.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
            }
                break;
            case 9:
            {
                info.icon[i] = pet->petConfigMap[campaign->monthAssignConfigMap[i+1].boundId].icon;
                info.name[i] = pet->petConfigMap[campaign->monthAssignConfigMap[i+1].boundId].name;
            }
                break;
            case 10:
            {
                info.name[i] = StringData::shared()->stringFromKey("normal_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            }
                break;
            case 11:
            {
                info.name[i] = StringData::shared()->stringFromKey("special_draw_ticket");
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            }
                break;
                
            default:
                break;
        }
    }
    
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::takeMonthAssignBound()
{
    if (campaign->getCoreData().monthAssignState == 2) {
        return;
    }
    
    int monthAssignId = campaign->getCoreData().monthAssignId;
    int count = campaign->monthAssignConfigMap[monthAssignId].boundCount;
    int boundId = campaign->monthAssignConfigMap[monthAssignId].boundId;
    int vipLimit = campaign->monthAssignConfigMap[monthAssignId].vipLimit;
    if (vip->getCoreData().vipId >= vipLimit) {
        count *= 2;
    }
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    infoGet.count[0] = count;
    infoGet.frag[0] = false;
    switch (campaign->monthAssignConfigMap[monthAssignId].boundType)
    {
        case 1:
        {
            account->changeGold(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-16.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("gold");
            
            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = count;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);
            
        }
            break;
        case 2:
        {
            account->changeDiamond(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
        }
            break;
        case 3:
        {
            vit->addPlusVIT(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "tilibiao.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("VIT");
            L2E_UPDATE_VIT infoVIT;
            infoVIT.eProtocol = l2e_update_vit;
            infoVIT.currVIT = vit->getTotalVIT();
            infoVIT.maxVIT = vit->getMaxBaseVIT();
            ClientLogic::instance()->pass2Engine(&infoVIT);
        }
            break;
        case 4:
        {
            sword->addStone(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "hunshi.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("stone");
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 5:
        {
            account->changeFame(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "shengwang.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("fame");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 6:
        {
            prov->addTicket(count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "saodangquan.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("prov_ticket");
        }
            break;
        case 7:
        {
            arms->addGem(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("gem");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 8:
        {
            equip->addItem(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("equip_item_fragment");
            
            equip->checkTip(account->getData().level);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 1;
            infoTip.tipValue = equip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 9:
        {
            infoGet.frag[0] = true;
            pet->addFragment(boundId, count);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("pet_fragment");
            
            pet->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 2;
            infoTip.tipValue = pet->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 10:
        {
            lucky->addNormalTicket(count);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("normal_draw_ticket");
        }
            break;
        case 11:
        {
            lucky->addSpecialTicket(count);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("special_draw_ticket");
        }
            break;
            
    }
    ClientLogic::instance()->pass2Engine(&infoGet);
    
    campaign->takeMonthAssignBound();
    showMonthAssign();
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_CAMPAIGN_TIP infoCampaignTip;
    infoCampaignTip.eProtocol = l2e_update_campaign_tip;
    infoCampaignTip.activeTip = campaign->getActiveTip();
    infoCampaignTip.boundTip = campaign->getBoundTip();
    infoCampaignTip.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&infoCampaignTip);
    
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_refresh_month_assign;
    ClientLogic::instance()->pass2Time(&infoTime);
}

void SystemLogic::refreshMonthAssign()
{
    campaign->enableMonthAssign();

}

void SystemLogic::showCampaignActive()
{
    L2E_SHOW_CAMPAIGN_ACTIVE info;
    info.eProtocol = l2e_show_campaign_active;
    info.totalActiveVal = campaign->getCoreData().totalActiveVal;
    memcpy(info.boxState, campaign->getCoreData().boxState, sizeof(int)*4);
    
    info.missionCount = (int)campaign->activeMissionMap.size();
    info.descript.clear();
    info.count.clear();
    info.totalCount.clear();
    info.activeVal.clear();
    info.boundCount.clear();
    info.boundState.clear();
    for (auto config : campaign->activeMissionMap) {
        info.missionId.push_back(config.second.missionId);
        info.descript.push_back(config.second.descript);
        info.totalCount.push_back(config.second.count);
        info.activeVal.push_back(config.second.activeVal);
        info.boundCount.push_back(config.second.boundCount);
    }
    for (auto data : campaign->activeDataMap) {
        info.count.push_back(data.second.count);
        info.boundState.push_back(data.second.boundState);
    }
    ClientLogic::instance()->pass2Engine(&info);
}
void SystemLogic::showHallCastle()
{
    L2E_SHOW_HALL_CASTLE info;
    info.eProtocol = l2e_show_hall_castle;
    info.currJob = account->getData().currJob;
    info.name = account->getData().name;
    for (int i = 0; i < 4; i++) {
        info.jobLock[i] = account->jobMap[i+1].lock;
        info.jobActive[i] = account->jobMap[i+1].active;
    }
    
    info.jobImg = account->jobConfigMap[info.currJob].pvpImg;
    info.jobDescript = account->jobConfigMap[info.currJob].descript;
    ClientLogic::instance()->pass2Engine(&info);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_castle";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::showJob(E2L_UPDATE_ONE_VALUE info)
{
    int jobId = info.value;
    account->jobMap[jobId];
    L2E_SHOW_JOB infoOut;
    infoOut.eProtocol = l2e_show_job;
    infoOut.errNo = 0;
    infoOut.currJob = jobId;
    infoOut.price = 0;
    if (account->jobMap[jobId].active == 0) {
        infoOut.errNo = 1;
        infoOut.price = account->jobConfigMap[jobId].price;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    infoOut.jobImg = account->jobConfigMap[jobId].pvpImg;
    infoOut.jobDescript = account->jobConfigMap[jobId].descript;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::changeJob(E2L_UPDATE_ONE_VALUE info)
{
    int jobId = info.value;
    if (jobId == account->getData().currJob) {
        return;
    }
    account->changeJob(jobId);
    L2E_CHANGE_JOB infoOut;
    infoOut.eProtocol = l2e_change_job;
    infoOut.jobIcon = account->jobConfigMap[account->getData().currJob].headIcon;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::activeJob(E2L_UPDATE_ONE_VALUE info)
{
    int jobId = info.value;
    L2E_SHOW_JOB infoOut;
    infoOut.eProtocol = l2e_active_job;
    infoOut.errNo = 0;
    int price = account->jobConfigMap[jobId].price;
    if (account->getData().diamond < price) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    account->changeDiamond(-price);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
    
    account->activeJob(jobId);
    infoOut.currJob = jobId;
    infoOut.jobImg = account->jobConfigMap[jobId].pvpImg;
    infoOut.jobDescript = account->jobConfigMap[jobId].descript;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::changeName(E2L_CHANGE_NAME info)
{
    account->changeName(info.name);
    L2E_UPDATE_NAME infoName;
    infoName.eProtocol = l2e_update_name;
    infoName.name = account->getData().name;
    ClientLogic::instance()->pass2Engine(&infoName);
}

void SystemLogic::showLucky()
{
    L2E_SHOW_LUCKY info;
    info.eProtocol = l2e_show_hall_lucky;
    info.normalTicket = lucky->getNormalTicket();
    info.specialTicket = lucky->getSpecialTicket();
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();
//    info.normalCD = 0;
//    info.specialCD = 0;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::drawNormal()
{
    L2E_DRAW_ONE info;
    info.eProtocol = l2e_draw_normal_one;
    info.errNo = 0;
    info.normalCD = 0;
    info.specialCD = 0;
    int cardId = 0;
    if (lucky->getNormalState() == 1) {
        cardId = lucky->drawNormalForFree();
        if (cardId == 0) {
            info.errNo = 2;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }else if (cardId == -1) {
            info.errNo = 1;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
        info.normalCD = normalDrawRestoreSec;
        L2T_RESTORE_DRAW_TICKET infoTime;
        infoTime.eProtocol = l2t_restore_draw_normal;
        infoTime.restoreSec = normalDrawRestoreSec;
        ClientLogic::instance()->pass2Time(&infoTime);
        
        lucky->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 6;
        infoTip.tipValue = lucky->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }else{
        cardId = lucky->drawNormal();
        if (cardId == 0) {
            info.errNo = 3;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }else if (cardId == -1) {
            info.errNo = 1;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    }
    info.normalTicket = lucky->getNormalTicket();
    info.specialTicket = lucky->getSpecialTicket();
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();
    
    int count = lucky->normalMap[cardId].boundCount;
    int boundId = lucky->normalMap[cardId].boundId;
    info.count = count;
    info.frag = false;
    switch (lucky->normalMap[cardId].boundType)
    {
        case 1:
        {
            account->changeGold(count);
            info.icon = GameUtils::format(COMMON_DIR, "drop-16.png");
            info.name = StringData::shared()->stringFromKey("gold");
            
            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = count;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);

        }
            break;
        case 2:
        {
            account->changeDiamond(count);
            info.icon = GameUtils::format(COMMON_DIR, "drop-17.png");
            info.name = StringData::shared()->stringFromKey("diamond");
            
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
        }
            break;
        case 3:
        {
            vit->addPlusVIT(count);
            info.icon = GameUtils::format(COMMON_DIR, "tilibiao.png");
            info.name = StringData::shared()->stringFromKey("VIT");
            
            L2E_UPDATE_VIT infoVIT;
            infoVIT.eProtocol = l2e_update_vit;
            infoVIT.currVIT = vit->getTotalVIT();
            infoVIT.maxVIT = vit->getMaxBaseVIT();
            ClientLogic::instance()->pass2Engine(&infoVIT);
        }
            break;
        case 4:
        {
            sword->addStone(count);
            info.icon = GameUtils::format(COMMON_DIR, "hunshi.png");
            info.name = StringData::shared()->stringFromKey("stone");
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 5:
        {
            account->changeFame(count);
            info.icon = GameUtils::format(COMMON_DIR, "shengwang.png");
            info.name = StringData::shared()->stringFromKey("fame");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 6:
        {
            prov->addTicket(count);
            info.icon = GameUtils::format(COMMON_DIR, "saodangquan.png");
            info.name = StringData::shared()->stringFromKey("prov_ticket");
        }
            break;
        case 7:
        {
            arms->addGem(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
            info.name = StringData::shared()->stringFromKey("gem");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 8:
        {
            equip->addItem(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
            info.name = StringData::shared()->stringFromKey("equip_item_fragment");
            
            equip->checkTip(account->getData().level);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 1;
            infoTip.tipValue = equip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 9:
        {
            pet->addFragment(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
            info.name = pet->petConfigMap[boundId].name;
            info.frag = true;
            
            pet->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 2;
            infoTip.tipValue = pet->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 10:
        {
            lucky->addNormalTicket(count);
            info.icon = GameUtils::format(LUCKY_DIR, "chouqu1.png");
        }
            break;
        case 11:
        {
            lucky->addSpecialTicket(count);
            info.icon = GameUtils::format(LUCKY_DIR, "chouqu2.png");
        }
            break;
    }

    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeMission(1, 0);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
}

void SystemLogic::drawSpecial()
{
    L2E_DRAW_ONE info;
    info.eProtocol = l2e_draw_special_one;
    info.errNo = 0;
    info.normalCD = 0;
    info.specialCD = 0;
    int cardId = 0;
    if (lucky->getSpecialState() == 1) {
        cardId = lucky->drawSpecialForFree();
        if (cardId == 0) {
            info.errNo = 2;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }else if (cardId == -1) {
            info.errNo = 1;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
        info.specialCD = specialDrawRestoreSec;
        L2T_RESTORE_DRAW_TICKET infoTime;
        infoTime.eProtocol = l2t_restore_draw_special;
        infoTime.restoreSec = specialDrawRestoreSec;
        ClientLogic::instance()->pass2Time(&infoTime);
        
        lucky->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 6;
        infoTip.tipValue = lucky->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }else{
        cardId = lucky->drawSpecial();
        if (cardId == 0) {
            info.errNo = 3;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }else if (cardId == -1) {
            info.errNo = 1;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    }
    info.normalTicket = lucky->getNormalTicket();
    info.specialTicket = lucky->getSpecialTicket();
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();
    
    int count = lucky->specialMap[cardId].boundCount;
    int boundId = lucky->specialMap[cardId].boundId;
    info.count = count;
    info.frag = false;
    switch (lucky->specialMap[cardId].boundType)
    {
        case 1:
        {
            account->changeGold(count);
            info.icon = GameUtils::format(COMMON_DIR, "drop-16.png");
            info.name = StringData::shared()->stringFromKey("gold");
            
            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = count;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);

        }
            break;
        case 2:
        {
            account->changeDiamond(count);
            info.icon = GameUtils::format(COMMON_DIR, "drop-17.png");
            info.name = StringData::shared()->stringFromKey("diamond");
            
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
        }
            break;
        case 3:
        {
            vit->addPlusVIT(count);
            info.icon = GameUtils::format(COMMON_DIR, "tilibiao.png");
            info.name = StringData::shared()->stringFromKey("VIT");
            
            L2E_UPDATE_VIT infoVIT;
            infoVIT.eProtocol = l2e_update_vit;
            infoVIT.currVIT = vit->getTotalVIT();
            infoVIT.maxVIT = vit->getMaxBaseVIT();
            ClientLogic::instance()->pass2Engine(&infoVIT);
        }
            break;
        case 4:
        {
            sword->addStone(count);
            info.icon = GameUtils::format(COMMON_DIR, "hunshi.png");
            info.name = StringData::shared()->stringFromKey("stone");
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 5:
        {
            account->changeFame(count);
            info.icon = GameUtils::format(COMMON_DIR, "shengwang.png");
            info.name = StringData::shared()->stringFromKey("fame");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 6:
        {
            prov->addTicket(count);
            info.icon = GameUtils::format(COMMON_DIR, "saodangquan.png");
            info.name = StringData::shared()->stringFromKey("prov_ticket");
        }
            break;
        case 7:
        {
            arms->addGem(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
            info.name = StringData::shared()->stringFromKey("gem");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 8:
        {
            equip->addItem(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
            info.name = StringData::shared()->stringFromKey("equip_item_fragment");
            
            equip->checkTip(account->getData().level);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 1;
            infoTip.tipValue = equip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 9:
        {
            pet->addFragment(boundId, count);
            info.icon = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
            info.name = pet->petConfigMap[boundId].name;
            info.frag = true;
            
            pet->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 2;
            infoTip.tipValue = pet->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 10:
        {
            lucky->addNormalTicket(count);
            info.icon = GameUtils::format(LUCKY_DIR, "chouqu1.png");
        }
            break;
        case 11:
        {
            lucky->addSpecialTicket(count);
            info.icon = GameUtils::format(LUCKY_DIR, "chouqu2.png");
        }
            break;
    }

    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeMission(1, 0);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
}

void SystemLogic::drawSpecialTen()
{
    L2E_DRAW_TEN info;
    info.eProtocol = l2e_draw_special_ten;
    info.errNo = 0;
    int cardId[10];
    int count[10];
    info.errNo = lucky->drawSpecialTen(cardId, count);
    if (info.errNo != 0) {
        info.specialTicket = lucky->getSpecialTicket();
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }

    for (int i = 0; i < 10; i++) {
        int boundId = lucky->specialMap[cardId[i]].boundId;
        info.count[i] = count[i];
        info.frag[i] = false;
        switch (lucky->specialMap[cardId[i]].boundType)
        {
            case 1:
            {
                account->changeGold(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                info.name[i] = StringData::shared()->stringFromKey("gold");
                
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = count[i];
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);

            }
                break;
            case 2:
            {
                account->changeDiamond(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "drop-17.png");
                info.name[i] = StringData::shared()->stringFromKey("diamond");
                
                L2E_UPDATE_ONE_VALUE infoDiamond;
                infoDiamond.eProtocol = l2e_update_diamond;
                infoDiamond.value = account->getData().diamond;
                ClientLogic::instance()->pass2Engine(&infoDiamond);
            }
                break;
            case 3:
            {
                vit->addPlusVIT(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                info.name[i] = StringData::shared()->stringFromKey("VIT");
                
                L2E_UPDATE_VIT infoVIT;
                infoVIT.eProtocol = l2e_update_vit;
                infoVIT.currVIT = vit->getTotalVIT();
                infoVIT.maxVIT = vit->getMaxBaseVIT();
                ClientLogic::instance()->pass2Engine(&infoVIT);
            }
                break;
            case 4:
            {
                sword->addStone(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "hunshi.png");
                info.name[i] = StringData::shared()->stringFromKey("stone");
                
                sword->checkTip(account->getData().gold);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 3;
                infoTip.tipValue = sword->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 5:
            {
                account->changeFame(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "shengwang.png");
                info.name[i] = StringData::shared()->stringFromKey("fame");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 6:
            {
                prov->addTicket(count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                info.name[i] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                arms->addGem(boundId, count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                info.name[i] = StringData::shared()->stringFromKey("gem");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 8:
            {
                equip->addItem(boundId, count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
                info.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
                
                equip->checkTip(account->getData().level);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 1;
                infoTip.tipValue = equip->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 9:
            {
                pet->addFragment(boundId, count[i]);
                info.icon[i] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                info.name[i] = pet->petConfigMap[boundId].name;
                info.frag[i] = true;
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 10:
            {
                lucky->addNormalTicket(count[i]);
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            }
                break;
            case 11:
            {
                lucky->addSpecialTicket(count[i]);
                info.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            }
                break;

        }

    }
    info.normalTicket = lucky->getNormalTicket();
    info.specialTicket = lucky->getSpecialTicket();
//    info.normalCD = 0;
//    info.specialCD = 0;
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();

    ClientLogic::instance()->pass2Engine(&info);
    campaign->activeMission(1, 0);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
}

void SystemLogic::restoreDrawNormal()
{
    lucky->enableNormalDraw();
    L2E_UPDATE_DRAW info;
    info.eProtocol = l2e_update_draw_normal;
//    info.normalCD = 0;
//    info.specialCD = 0;
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();
    ClientLogic::instance()->pass2Engine(&info);
    
    lucky->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 6;
    infoTip.tipValue = lucky->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);

}

void SystemLogic::restoreDrawSpecial()
{
    lucky->enableSpecialDraw();
    L2E_UPDATE_DRAW info;
    info.eProtocol = l2e_update_draw_special;
//    info.normalCD = 0;
//    info.specialCD = 0;
    info.normalState = lucky->getNormalState();
    info.specialState = lucky->getSpecialState();
    ClientLogic::instance()->pass2Engine(&info);
    
    lucky->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 6;
    infoTip.tipValue = lucky->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
}

void SystemLogic::getLuckyCD()
{
    L2T_COMMON info;
    info.eProtocol = l2t_get_lucky_cd;
    ClientLogic::instance()->pass2Time(&info);
}

void SystemLogic::updateLuckyCD(T2L_UPDATE_LUCKY_CD info)
{
    L2E_UPDATE_LUCKY_CD infoOut;
    infoOut.eProtocol = l2e_update_lucky_cd;
    infoOut.normalCD = info.normalLeftCD;
    infoOut.specialCD = info.specialLeftCD;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::buyLuckySpecialTicket(E2L_UPDATE_ONE_VALUE info)
{
    int count = info.value;
    L2E_UPDATE_SPECIAL_TICKET infoOut;
    infoOut.eProtocol = l2e_update_special_ticket;
    infoOut.errNo = 0;
    if (count*luckyTicketPrice > account->getData().diamond) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    account->changeDiamond(-count*luckyTicketPrice);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = account->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
    
    lucky->addSpecialTicket(count);
    infoOut.count = lucky->getSpecialTicket();
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::showHallShop(E2L_UPDATE_ONE_VALUE info)
{
    L2E_SHOW_HALL_SHOP infoOut;
    infoOut.eProtocol = l2e_show_hall_shop;
    infoOut.type = info.value;
    std::vector<int> shopIdVec;
    shop->getShopByType(infoOut.type, shopIdVec);
    infoOut.count = (int)shopIdVec.size();
    memset(infoOut.boundCount, 0, sizeof(int)*10);
    for (int i = 0; i < 10; i++) {
        if (i<infoOut.count) {
            infoOut.shopId[i] = shop->shopConfigMap[shopIdVec[i]].shopId;
            infoOut.name[i] = shop->shopConfigMap[shopIdVec[i]].name;
            infoOut.icon[i] = shop->shopConfigMap[shopIdVec[i]].icon;
            infoOut.price[i] = shop->shopConfigMap[shopIdVec[i]].price;
            infoOut.boundCount[i] = shop->shopConfigMap[shopIdVec[i]].boundCount;
        }else{
            infoOut.name[i] = "";
            infoOut.icon[i] = "";
            infoOut.price[i] = 0;
            infoOut.boundCount[i] = 0;
            infoOut.shopId[i] = 0;
        }
    }
    
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::showShop(E2L_UPDATE_ONE_VALUE info)
{
    int shopType = info.value;
    L2E_SHOW_HALL_SHOP infoOut;
    infoOut.eProtocol = l2e_show_shop;
    infoOut.type = shopType;
    std::vector<int> shopIdVec;
    shop->getShopByType(infoOut.type, shopIdVec);
    infoOut.count = (int)shopIdVec.size();
    memset(infoOut.boundCount, 0, sizeof(int)*10);
    for (int i = 0; i < 10; i++) {
        if (i<infoOut.count) {
            infoOut.shopId[i] = shop->shopConfigMap[shopIdVec[i]].shopId;
            infoOut.name[i] = shop->shopConfigMap[shopIdVec[i]].name;
            infoOut.icon[i] = shop->shopConfigMap[shopIdVec[i]].icon;
            infoOut.price[i] = shop->shopConfigMap[shopIdVec[i]].price;
            infoOut.boundCount[i] = shop->shopConfigMap[shopIdVec[i]].boundCount;
        }else{
            infoOut.name[i] = "";
            infoOut.icon[i] = "";
            infoOut.price[i] = 0;
            infoOut.boundCount[i] = 0;
            infoOut.shopId[i] = 0;
        }
    }
    
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::showShopPackageDetail(E2L_UPDATE_ONE_VALUE info)
{
    int packageId = info.value;
    L2E_SHOW_PACKAGE_DETAIL infoOut;
    infoOut.eProtocol = l2e_show_package_detail;
    infoOut.packageId = packageId;
    infoOut.packageImg = shop->shopConfigMap[packageId].packageImg;
    infoOut.packageTitle = shop->shopConfigMap[packageId].packageTitle;
    infoOut.price = shop->shopConfigMap[packageId].price;
    infoOut.noviceState = shop->getCoreData().noviceRecharge;
    memset(infoOut.count, 0, sizeof(int)*20);
    if (shop->shopConfigMap[packageId].boundType == 1) {
        infoOut.itemCount = 1;
        infoOut.count[0] = shop->shopConfigMap[packageId].boundCount;
        infoOut.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    std::vector<int> packageIdVec;
    shop->getPackageById(shop->shopConfigMap[packageId].boundId, packageIdVec);
    infoOut.itemCount = (int)packageIdVec.size();
    if (infoOut.itemCount > 0) {
        int index = 0;
        for (auto packageId : packageIdVec) {
            infoOut.count[index] = shop->packageConfigMap[packageId].boundCount;
            int boundType = shop->packageConfigMap[packageId].boundType;
            switch(boundType)
            {
                case 1:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                }
                    break;
                case 2:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                }
                    break;
                case 3:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");

                }
                    break;
                case 4:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                }
                    break;
                case 5:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                }
                    break;
                case 6:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                }
                    break;
                case 7:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                }
                    break;
                case 8:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                }
                    break;
                case 9:
                {
                    infoOut.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                }
                    break;
                case 10:
                {
                    infoOut.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                }
                    break;
                case 11:
                {
                    infoOut.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                }
                    break;
            }
            index++;
        }
        
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::buyRevive()
{
    C2S_PAY infoPay;
    infoPay.eProtocol = c2s_pay;
    std::vector<int> packageIdVec;
    shop->getShopByType(8, packageIdVec);
    infoPay.payId = packageIdVec[0];
    infoPay.orderId = shop->shopConfigMap[infoPay.payId].payId;
    infoPay.price = shop->shopConfigMap[infoPay.payId].price;
    infoPay.name = shop->shopConfigMap[infoPay.payId].name;
    ClientLogic::instance()->pass2Service(&infoPay);
    
    C2S_UMENG_EXCHANGE umengInfo;
    umengInfo.eProtocol = c2s_umeng_exchange;
    umengInfo.orderId = infoPay.orderId;
    umengInfo.count = 1;
    umengInfo.channel = 21;
    umengInfo.price = infoPay.price;
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::buyShopPackage(E2L_UPDATE_ONE_VALUE info)
{
    int value = info.value;
    if (shop->shopConfigMap[value].priceType == 1) {
        C2S_PAY infoPay;
        infoPay.eProtocol = c2s_pay;
        infoPay.payId = value;
        infoPay.orderId = shop->shopConfigMap[infoPay.payId].payId;
        infoPay.price = shop->shopConfigMap[infoPay.payId].price;
        infoPay.name = shop->shopConfigMap[infoPay.payId].name;
        
        C2S_UMENG_TRACK infoUmeng;
        infoUmeng.eProtocol = c2s_umeng_track;
        infoUmeng.count = 0;
        infoUmeng.attrMap.clear();
        switch(infoPay.payId)
        {
            case 1:
            {
                infoUmeng.eventName = "diamond_60";
            }
                break;
            case 2:
            {
                infoUmeng.eventName = "diamond_300";
            }
                break;
            case 3:
            {
                infoUmeng.eventName = "diamond_680";
            }
                break;
            case 4:
            {
                infoUmeng.eventName = "diamond_1280";
            }
                break;
            case 5:
            {
                infoUmeng.eventName = "diamond_3280";
            }
                break;
            case 6:
            {
                infoUmeng.eventName = "diamond_6480";
            }
                break;
            case 7:
            {
                infoUmeng.eventName = "gift_1";
            }
                break;
            case 8:
            {
                infoUmeng.eventName = "gift_2";
            }
                break;
            case 9:
            {
                infoUmeng.eventName = "gift_3";
            }
                break;
            case 10:
            {
                infoUmeng.eventName = "gift_4";
            }
                break;
        }
        ClientLogic::instance()->pass2Service(&infoUmeng);
        
        ClientLogic::instance()->pass2Service(&infoPay);
    }else{
        int price = shop->shopConfigMap[value].price;
        L2E_SHOW_PACKAGE_RESULT infoOut;
        infoOut.eProtocol = l2e_show_package_result;
        infoOut.errNo = 0;
        if (account->getData().diamond < price) {
            infoOut.errNo = 2;
            ClientLogic::instance()->pass2Engine(&infoOut);
            return;
        }
        
        account->changeDiamond(-price);
        L2E_UPDATE_ONE_VALUE infoDiamond;
        infoDiamond.eProtocol = l2e_update_diamond;
        infoDiamond.value = account->getData().diamond;
        ClientLogic::instance()->pass2Engine(&infoDiamond);
        
        takePackageBound(value);
        infoOut.noviceState = shop->getCoreData().noviceRecharge;
        ClientLogic::instance()->pass2Engine(&infoOut);
        
        
        C2S_UMENG_TRACK infoUmeng;
        infoUmeng.eProtocol = c2s_umeng_track;
        infoUmeng.count = 0;
        infoUmeng.attrMap.clear();
        switch(value)
        {
            case 13:
            {
                infoUmeng.eventName = "gold_pack_1";
            }
                break;
            case 14:
            {
                infoUmeng.eventName = "gold_pack_2";
            }
                break;
            case 15:
            {
                infoUmeng.eventName = "gold_pack_3";
            }
                break;
        }
        ClientLogic::instance()->pass2Service(&infoUmeng);

    }
    
    
}

void SystemLogic::takePackageBound(int packageId)
{
    bool firstRecharge = 0;
    if (shop->shopConfigMap[packageId].priceType == 1) {
        if (shop->getCoreData().totalRecharge+shop->shopConfigMap[packageId].price >= 6) {
            if (shop->getCoreData().firstRecharge == 0) {
                std::vector<int> packageIdVec;
                shop->getShopByType(4, packageIdVec);
                shop->recharge(packageIdVec[0]);
                firstRecharge = 1;
            }
        }
    }
    
    shop->recharge(packageId);
    int vipLevel = vip->getCoreData().vipId;
    vip->uplevelByRecharge(shop->getCoreData().totalRecharge);
    if (vipLevel != vip->getCoreData().vipId) {
        uplevelVip(vipLevel, vip->getCoreData().vipId);
        campaign->activeBoundMission(10, 0, vip->getCoreData().vipId);
        
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 0;
        infoTip.tipValue = vip->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
        
        campaign->checkTip();
        L2E_UPDATE_HALL_TIP infoTip1;
        infoTip1.eProtocol = l2e_update_hall_tip;
        infoTip1.tipIndex = 7;
        infoTip1.tipValue = campaign->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip1);
    }
//    if (shop->shopConfigMap[packageId].type == 8) {
//        return;
//    }
    
    bool unlockAllJob = false;
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    if (shop->shopConfigMap[packageId].boundType == 1) {
        infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
        infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
        infoGet.count[0] = shop->shopConfigMap[packageId].boundCount;
        infoGet.frag[0] = false;
        account->changeDiamond(infoGet.count[0]);
        L2E_UPDATE_ONE_VALUE infoDiamond;
        infoDiamond.eProtocol = l2e_update_diamond;
        infoDiamond.value = account->getData().diamond;
        ClientLogic::instance()->pass2Engine(&infoDiamond);
        
        ClientLogic::instance()->pass2Engine(&infoGet);
    }else if (shop->shopConfigMap[packageId].boundType == 3) {
        infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-16.png");
        infoGet.name[0] = StringData::shared()->stringFromKey("gold");
        infoGet.count[0] = shop->shopConfigMap[packageId].boundCount;
        infoGet.frag[0] = false;
        account->changeGold(infoGet.count[0]);
        L2E_UPDATE_ONE_VALUE infoGold;
        infoGold.eProtocol = l2e_update_gold;
        infoGold.value = account->getData().gold;
        ClientLogic::instance()->pass2Engine(&infoGold);
        
        C2S_UMENG_TRACK infoUmeng;
        infoUmeng.eProtocol = c2s_umeng_track;
        infoUmeng.eventName = "get_gold";
        infoUmeng.count = infoGet.count[0];
        infoUmeng.attrMap.clear();
        ClientLogic::instance()->pass2Service(&infoUmeng);

        
        ClientLogic::instance()->pass2Engine(&infoGet);
    }else if (shop->shopConfigMap[packageId].boundType == 2) {
        if (shop->shopConfigMap[packageId].boundId == 0) {
            return;
        }
        std::vector<int> packageIdVec;
        shop->getPackageById(shop->shopConfigMap[packageId].boundId, packageIdVec);
        int totalBoundCount = 0;
        for (int i = 0; i < 20; i++) {
            if (i >= packageIdVec.size()) {
                break;
            }
            int itemId = packageIdVec[i];
            infoGet.count[i] = shop->packageConfigMap[itemId].boundCount;
            infoGet.frag[i] = false;
            totalBoundCount++;
            switch(shop->packageConfigMap[itemId].boundType)
            {
                case 1:
                {
                    account->changeGold(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("gold");
                    
                    L2E_UPDATE_ONE_VALUE infoGold;
                    infoGold.eProtocol = l2e_update_gold;
                    infoGold.value = account->getData().gold;
                    ClientLogic::instance()->pass2Engine(&infoGold);
                    
                    C2S_UMENG_TRACK infoUmeng;
                    infoUmeng.eProtocol = c2s_umeng_track;
                    infoUmeng.eventName = "get_gold";
                    infoUmeng.count = infoGet.count[i];
                    infoUmeng.attrMap.clear();
                    ClientLogic::instance()->pass2Service(&infoUmeng);
                }
                    break;
                case 2:
                {
                    account->changeDiamond(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-17.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("diamond");
                    
                    L2E_UPDATE_ONE_VALUE infoDiamond;
                    infoDiamond.eProtocol = l2e_update_diamond;
                    infoDiamond.value = account->getData().diamond;
                    ClientLogic::instance()->pass2Engine(&infoDiamond);
                }
                    break;
                case 3:
                {
                    vit->addPlusVIT(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("VIT");
                    
                    L2E_UPDATE_VIT infoVIT;
                    infoVIT.eProtocol = l2e_update_vit;
                    infoVIT.currVIT = vit->getTotalVIT();
                    infoVIT.maxVIT = vit->getMaxBaseVIT();
                    ClientLogic::instance()->pass2Engine(&infoVIT);
                }
                    break;
                case 4:
                {
                    sword->addStone(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "hunshi.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("stone");
                    
                    sword->checkTip(account->getData().gold);
                    L2E_UPDATE_HALL_TIP infoTip;
                    infoTip.eProtocol = l2e_update_hall_tip;
                    infoTip.tipIndex = 3;
                    infoTip.tipValue = sword->getTip();
                    ClientLogic::instance()->pass2Engine(&infoTip);
                }
                    break;
                case 5:
                {
                    account->changeFame(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "shengwang.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("fame");
                    
                    arms->checkTip(account->getData().fame);
                    L2E_UPDATE_HALL_TIP infoTip;
                    infoTip.eProtocol = l2e_update_hall_tip;
                    infoTip.tipIndex = 4;
                    infoTip.tipValue = arms->getTip();
                    ClientLogic::instance()->pass2Engine(&infoTip);
                }
                    break;
                case 6:
                {
                    prov->addTicket(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("prov_ticket");
                }
                    break;
                case 7:
                {
                    arms->addGem(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                    infoGet.name[i] = StringData::shared()->stringFromKey("gem");
                    
                    arms->checkTip(account->getData().fame);
                    L2E_UPDATE_HALL_TIP infoTip;
                    infoTip.eProtocol = l2e_update_hall_tip;
                    infoTip.tipIndex = 4;
                    infoTip.tipValue = arms->getTip();
                    ClientLogic::instance()->pass2Engine(&infoTip);
                }
                    break;
                case 8:
                {
                    equip->addItem(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                    infoGet.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
                    
                    E2L_SEL_EQUIP_POS info;
                    info.eProtocol = e2l_sel_equip_pos;
                    info.pos = currEquipPos;
                    selEquipPos(info);
                    
                    equip->checkTip(account->getData().level);
                    L2E_UPDATE_HALL_TIP infoTip;
                    infoTip.eProtocol = l2e_update_hall_tip;
                    infoTip.tipIndex = 1;
                    infoTip.tipValue = equip->getTip();
                    ClientLogic::instance()->pass2Engine(&infoTip);
                    L2E_UPDATE_EQUIP_TIP infoEquipTip;
                    infoEquipTip.eProtocol = l2e_update_equip_tip;
                    infoEquipTip.posTip.clear();
                    for (auto tip : equip->posTip) {
                        infoEquipTip.posTip.push_back(tip);
                    }
                    infoEquipTip.posUplevelTip.clear();
                    for (auto tip : equip->posUplevelTip) {
                        infoEquipTip.posUplevelTip.push_back(tip);
                    }
                    infoEquipTip.posUpgradeTip.clear();
                    for (auto tip : equip->posUpgradeTip) {
                        infoEquipTip.posUpgradeTip.push_back(tip);
                    }
                    ClientLogic::instance()->pass2Engine(&infoEquipTip);
                }
                    break;
                case 9:
                {
                    pet->addFragment(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, pet->petConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                    infoGet.frag[i] = true;
                    infoGet.name[i] = StringData::shared()->stringFromKey("pet_fragment");
                    
                    pet->checkTip();
                    L2E_UPDATE_HALL_TIP infoTip;
                    infoTip.eProtocol = l2e_update_hall_tip;
                    infoTip.tipIndex = 2;
                    infoTip.tipValue = pet->getTip();
                    ClientLogic::instance()->pass2Engine(&infoTip);
                }
                    break;
                case 10:
                {
                    lucky->addNormalTicket(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("normal_draw_ticket");
                }
                    break;
                case 11:
                {
                    lucky->addSpecialTicket(infoGet.count[i]);
                    infoGet.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("special_draw_ticket");
                }
                    break;
                case 12:
                {
                    account->activeAllJob();
                    L2E_COMMON infoOut;
                    infoOut.eProtocol = l2e_active_all_job;
                    ClientLogic::instance()->pass2Engine(&infoOut);
                    
                    infoGet.count[i] = 0;
                    
                    unlockAllJob = true;
                    
                    totalBoundCount--;
                }
                    break;
                default:
                {
                    totalBoundCount--;
                }
                    break;
            }
        }
        if (totalBoundCount > 0) {
            ClientLogic::instance()->pass2Engine(&infoGet);
        }
        
        if (unlockAllJob) {
            L2E_COMMON info;
            info.eProtocol = l2e_show_unlock_job;
            ClientLogic::instance()->pass2Engine(&info);
        }
    }
}

void SystemLogic::responseBuy(S2C_PAY info)
{
    int packageId = info.packageId;
    
    if (shop->shopConfigMap[packageId].type == 8) {
        if (info.success) {
            GameCore::instance()->revive();
            takePackageBound(packageId);
        }else{
            GameCore::instance()->notRevive();
        }
        
        return;
    }
    
    L2E_SHOW_PACKAGE_RESULT infoOut;
    infoOut.eProtocol = l2e_show_package_result;
    infoOut.errNo = 0;
    if (!info.success) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    takePackageBound(packageId);
    
    infoOut.noviceState = shop->getCoreData().noviceRecharge;
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    if (shop->shopConfigMap[packageId].type == 6) {
        closeDiscount();
    }
}

void SystemLogic::showNovice()
{
    std::vector<int> shopIdVec;
    shop->getShopByType(3, shopIdVec);
    int packageId = shopIdVec[0];
    L2E_SHOW_PACKAGE_DETAIL infoOut;
    infoOut.eProtocol = l2e_show_hall_novice;
    infoOut.packageId = packageId;
    infoOut.price = shop->shopConfigMap[packageId].price;
    infoOut.noviceState = shop->getCoreData().noviceRecharge;
    memset(infoOut.count, 0, sizeof(int)*20);
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::showFirstRecharge()
{
    L2E_SHOW_FIRST_RECHARGE info;
    info.eProtocol = l2e_update_first_recharge;
    info.firstRechargeState = shop->getCoreData().firstRecharge;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::takeFirstRecharge()
{
    if (shop->getCoreData().firstRecharge == 0) {
        L2E_COMMON info;
        info.eProtocol = l2e_hide_campaign;
        ClientLogic::instance()->pass2Engine(&info);
        
        E2L_UPDATE_ONE_VALUE infoShop;
        infoShop.eProtocol = e2l_click_main_shop;
        infoShop.value = 1;
        showHallShop(infoShop);
    }else if (shop->getCoreData().firstRecharge == 1){
        shop->takeFirstRechargeBound();
        
        std::vector<int> shopIdVec;
        shop->getShopByType(4, shopIdVec);
        int packageId = shopIdVec[0];
        L2E_SHOW_GET infoGet;
        infoGet.eProtocol = l2e_show_get;
        memset(infoGet.count, 0, sizeof(int)*20);
        if (shop->shopConfigMap[packageId].boundType == 1) {
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
            infoGet.count[0] = shop->shopConfigMap[packageId].boundCount;
            infoGet.frag[0] = false;
            account->changeDiamond(infoGet.count[0]);
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);

            ClientLogic::instance()->pass2Engine(&infoGet);
        }else if (shop->shopConfigMap[packageId].boundType == 2) {
            std::vector<int> packageIdVec;
            shop->getPackageById(shop->shopConfigMap[packageId].boundId, packageIdVec);
            for (int i = 0; i < 20; i++) {
                if (i >= packageIdVec.size()) {
                    break;
                }
                int itemId = packageIdVec[i];
                infoGet.count[i] = shop->packageConfigMap[itemId].boundCount;
                infoGet.frag[i] = false;
                switch(shop->packageConfigMap[itemId].boundType)
                {
                    case 1:
                    {
                        account->changeGold(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("gold");

                        L2E_UPDATE_ONE_VALUE infoGold;
                        infoGold.eProtocol = l2e_update_gold;
                        infoGold.value = account->getData().gold;
                        ClientLogic::instance()->pass2Engine(&infoGold);
                        
                        
                        C2S_UMENG_TRACK infoUmeng;
                        infoUmeng.eProtocol = c2s_umeng_track;
                        infoUmeng.eventName = "get_gold";
                        infoUmeng.count = infoGet.count[i];
                        infoUmeng.attrMap.clear();
                        ClientLogic::instance()->pass2Service(&infoUmeng);
                    }
                        break;
                    case 2:
                    {
                        account->changeDiamond(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-17.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("diamond");

                        L2E_UPDATE_ONE_VALUE infoDiamond;
                        infoDiamond.eProtocol = l2e_update_diamond;
                        infoDiamond.value = account->getData().diamond;
                        ClientLogic::instance()->pass2Engine(&infoDiamond);
                    }
                        break;
                    case 3:
                    {
                        vit->addPlusVIT(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("VIT");

                        L2E_UPDATE_VIT infoVIT;
                        infoVIT.eProtocol = l2e_update_vit;
                        infoVIT.currVIT = vit->getTotalVIT();
                        infoVIT.maxVIT = vit->getMaxBaseVIT();
                        ClientLogic::instance()->pass2Engine(&infoVIT);
                    }
                        break;
                    case 4:
                    {
                        sword->addStone(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "hunshi.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("stone");
                        
                        sword->checkTip(account->getData().gold);
                        L2E_UPDATE_HALL_TIP infoTip;
                        infoTip.eProtocol = l2e_update_hall_tip;
                        infoTip.tipIndex = 3;
                        infoTip.tipValue = sword->getTip();
                        ClientLogic::instance()->pass2Engine(&infoTip);
                    }
                        break;
                    case 5:
                    {
                        account->changeFame(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "shengwang.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("fame");
                        
                        arms->checkTip(account->getData().fame);
                        L2E_UPDATE_HALL_TIP infoTip;
                        infoTip.eProtocol = l2e_update_hall_tip;
                        infoTip.tipIndex = 4;
                        infoTip.tipValue = arms->getTip();
                        ClientLogic::instance()->pass2Engine(&infoTip);
                    }
                        break;
                    case 6:
                    {
                        prov->addTicket(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("prov_ticket");
                    }
                        break;
                    case 7:
                    {
                        arms->addGem(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                        infoGet.name[i] = StringData::shared()->stringFromKey("gem");
                        
                        arms->checkTip(account->getData().fame);
                        L2E_UPDATE_HALL_TIP infoTip;
                        infoTip.eProtocol = l2e_update_hall_tip;
                        infoTip.tipIndex = 4;
                        infoTip.tipValue = arms->getTip();
                        ClientLogic::instance()->pass2Engine(&infoTip);
                    }
                        break;
                    case 8:
                    {
                        equip->addItem(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                        infoGet.name[i] = StringData::shared()->stringFromKey("equip_item_fragment");
                        
                        equip->checkTip(account->getData().level);
                        L2E_UPDATE_HALL_TIP infoTip;
                        infoTip.eProtocol = l2e_update_hall_tip;
                        infoTip.tipIndex = 1;
                        infoTip.tipValue = equip->getTip();
                        ClientLogic::instance()->pass2Engine(&infoTip);
                    }
                        break;
                    case 9:
                    {
                        pet->addFragment(shop->packageConfigMap[itemId].boundId, infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(COMMON_DIR, pet->petConfigMap[shop->packageConfigMap[itemId].boundId].icon.c_str());
                        infoGet.name[i] = StringData::shared()->stringFromKey("pet_fragment");
                        infoGet.frag[i] = true;
                        
                        pet->checkTip();
                        L2E_UPDATE_HALL_TIP infoTip;
                        infoTip.eProtocol = l2e_update_hall_tip;
                        infoTip.tipIndex = 2;
                        infoTip.tipValue = pet->getTip();
                        ClientLogic::instance()->pass2Engine(&infoTip);
                    }
                        break;
                    case 10:
                    {
                        lucky->addNormalTicket(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("normal_draw_ticket");
                    }
                        break;
                    case 11:
                    {
                        lucky->addSpecialTicket(infoGet.count[i]);
                        infoGet.icon[i] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                        infoGet.name[i] = StringData::shared()->stringFromKey("special_draw_ticket");
                    }
                        break;
                }
            }
            ClientLogic::instance()->pass2Engine(&infoGet);
            
            L2E_SHOW_FIRST_RECHARGE info;
            info.eProtocol = l2e_update_first_recharge;
            info.firstRechargeState = shop->getCoreData().firstRecharge;
            ClientLogic::instance()->pass2Engine(&info);
        }
    }
}

void SystemLogic::uplevelVip(int originVip, int currVip)
{
    for (int i = originVip+1; i <= currVip; i++) {
        int packageId = vip->vipConfigMap[i].packageId;
        std::vector<int> packageIdVec;
        vip->getPackageById(packageId, packageIdVec);
        for (auto itemId : packageIdVec) {
            int boundType = vip->packageConfigMap[itemId].boundType;
            int boundId = vip->packageConfigMap[itemId].boundId;
            int boundCount = vip->packageConfigMap[itemId].boundCount;
            switch (boundType) {
                case 12:
                {
                    int currMaxVIT = vit->getMaxBaseVIT();
                    if (currMaxVIT < boundCount) {
                        vit->addMaxBaseVIT(boundCount-currMaxVIT);
                        if (vit->getBaseVIT() < vit->getMaxBaseVIT()) {
                            L2T_RESTORE_VIT info;
                            info.eProtocol = l2t_start_restore_vit;
                            info.restoreSec = vitRestoreSec;
                            ClientLogic::instance()->pass2Time(&info);
                        }
                    }
                    L2E_UPDATE_VIT infovit;
                    infovit.eProtocol = l2e_update_vit;
                    infovit.currVIT = vit->getTotalVIT();
                    infovit.maxVIT = vit->getMaxBaseVIT();
                    ClientLogic::instance()->pass2Engine(&infovit);
                    getVITCD();
                }
                    break;
                case 13:
                {
                    int currBuyCount = vit->getmaxBuyCount();
                    if (currBuyCount < boundCount) {
                        vit->addMaxBuyCount(boundCount-currBuyCount);
                    }
                }
                    break;
                case 14:
                {
                    int currRestCount = abyss->getAbyssData().maxResetCount;
                    if (currRestCount < boundCount) {
                        abyss->addMaxResetCount(boundCount - currRestCount);
                    }
                }
                    break;
                case 15:
                {
                    int currResetCount = tower->getTowerData().maxResetCount;
                    if (currResetCount < boundCount) {
                        tower->addMaxResetCount(boundCount-currResetCount);
                    }
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void SystemLogic::addVipLoginDay()
{
//    if (vip->getCoreData().lastLoginSec == 0) {
//        vip->setVIPLoginSec(TimeUtil::get_system_tick_s());
//        vip->addVIPDay();
//        vip->enableBound();
//        L2E_UPDATE_HALL_TIP infoTip;
//        infoTip.eProtocol = l2e_update_hall_tip;
//        infoTip.tipIndex = 0;
//        infoTip.tipValue = vip->getTip();
//        ClientLogic::instance()->pass2Engine(&infoTip);
//    }else{
//        int currDay = TimeUtil::timeFormatToDay(TimeUtil::get_system_tick_s());
//        int lastDay = TimeUtil::timeFormatToDay(vip->getCoreData().lastLoginSec);
//        if (currDay != lastDay) {
//            vip->setVIPLoginSec(TimeUtil::get_system_tick_s());
            int vipLevel = vip->getCoreData().vipId;
            vip->addVIPDay();
            vip->enableBound();
            if (vipLevel != vip->getCoreData().vipId) {
                uplevelVip(vipLevel, vip->getCoreData().vipId);
//                campaign->activeBoundMission(10, 0, vip->getCoreData().vipId);
                
//                L2E_UPDATE_HALL_TIP infoTip;
//                infoTip.eProtocol = l2e_update_hall_tip;
//                infoTip.tipIndex = 0;
//                infoTip.tipValue = vip->getTip();
//                ClientLogic::instance()->pass2Engine(&infoTip);
//                
//                campaign->checkTip();
//                L2E_UPDATE_HALL_TIP infoTip1;
//                infoTip1.eProtocol = l2e_update_hall_tip;
//                infoTip1.tipIndex = 7;
//                infoTip1.tipValue = campaign->getTip();
//                ClientLogic::instance()->pass2Engine(&infoTip1);
            }
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 0;
            infoTip.tipValue = vip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
//        }
//    }
    campaign->activeBoundMission(10, 0, vip->getCoreData().vipId);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip1;
    infoTip1.eProtocol = l2e_update_hall_tip;
    infoTip1.tipIndex = 7;
    infoTip1.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip1);
}

void SystemLogic::takeVipBound()
{
    L2E_TAKE_VIP_BOUND info;
    info.eProtocol = l2e_take_vip_bound;
    info.errNo = vip->takeBound();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 0;
    infoTip.tipValue = vip->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    int vipId = vip->getCoreData().vipId;
    int packageId = vip->vipConfigMap[vipId].packageId;
    std::vector<int> packageIdVec;
    vip->getPackageById(packageId, packageIdVec);
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    int index = 0;
    for (auto itemId : packageIdVec) {
        if (vip->packageConfigMap[itemId].boundType <= 11){
            infoGet.count[index] = vip->packageConfigMap[itemId].boundCount;
        }
        infoGet.frag[index] = false;
        switch(vip->packageConfigMap[itemId].boundType)
        {
            case 1:
            {
                account->changeGold(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("gold");
                
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = infoGet.count[index];
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);
            }
                break;
            case 2:
            {
                account->changeDiamond(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("diamond");
                
                L2E_UPDATE_ONE_VALUE infoDiamond;
                infoDiamond.eProtocol = l2e_update_diamond;
                infoDiamond.value = account->getData().diamond;
                ClientLogic::instance()->pass2Engine(&infoDiamond);
            }
                break;
            case 3:
            {
                vit->addPlusVIT(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("VIT");
                
                L2E_UPDATE_VIT infoVIT;
                infoVIT.eProtocol = l2e_update_vit;
                infoVIT.currVIT = vit->getTotalVIT();
                infoVIT.maxVIT = vit->getMaxBaseVIT();
                ClientLogic::instance()->pass2Engine(&infoVIT);
            }
                break;
            case 4:
            {
                sword->addStone(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("stone");
                
                sword->checkTip(account->getData().gold);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 3;
                infoTip.tipValue = sword->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 5:
            {
                account->changeFame(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("fame");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 6:
            {
                prov->addTicket(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                arms->addGem(vip->packageConfigMap[itemId].boundId, infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[vip->packageConfigMap[itemId].boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("gem");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 8:
            {
                equip->addItem(vip->packageConfigMap[itemId].boundId, infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[vip->packageConfigMap[itemId].boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("equip_item_fragment");
                
                equip->checkTip(account->getData().level);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 1;
                infoTip.tipValue = equip->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 9:
            {
                int petId = abs(rand())%pet->petConfigMap.size()+1;
                pet->addFragment(petId, infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[petId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("pet_fragment");
                infoGet.frag[index] = true;
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 10:
            {
                lucky->addNormalTicket(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("normal_draw_ticket");
            }
                break;
            case 11:
            {
                lucky->addSpecialTicket(infoGet.count[index]);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("special_draw_ticket");
            }
                break;
            default:
            {
//                infoGet.count[i] = 0;
                index--;
            }
                break;
        }
        index++;
    }

    ClientLogic::instance()->pass2Engine(&infoGet);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "vip_bound";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    umengInfo.attrMap["vip_level"] = Convert2String(vipId);
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::showVIP()
{
    L2E_SHOW_VIP info;
    info.eProtocol = l2e_show_hall_vip;
    info.currVipLevel = vip->getCoreData().vipId;
    if (info.currVipLevel < 4) {
        int temp = vip->vipConfigMap[info.currVipLevel].uplevelRecharge - shop->getCoreData().totalRecharge;
        info.nextRechargeCount = temp;
    }else{
        info.nextRechargeCount = 0;
    }
    if (info.currVipLevel > 1 && info.currVipLevel < 4){
        info.totalLoginDay = vip->vipConfigMap[info.currVipLevel].uplevelDay;
        info.currLoginDay = vip->getCoreData().loginDay;
    }else{
        info.totalLoginDay = 1;
        info.currLoginDay = 0;
    }
    info.boundState = vip->getCoreData().boundState;
    for (int i = 0; i < 10; i++) {
        info.detailDescript[i] = vip->vipConfigMap[info.currVipLevel].descript[i];
    }
    memset(info.boundCount, 0, sizeof(int)*6);
    int vipId = vip->getCoreData().vipId;
    int packageId = vip->vipConfigMap[vipId].packageId;
    std::vector<int> packageIdVec;
    vip->getPackageById(packageId, packageIdVec);
    int index = 0;
    for (auto itemId : packageIdVec) {
        int boundType = vip->packageConfigMap[itemId].boundType;
        if (boundType<=11) {
            info.boundCount[index] = vip->packageConfigMap[itemId].boundCount;
        }
        
        switch (boundType) {
            case 1:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
            }
                break;
            case 2:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
            }
                break;
            case 3:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
            }
                break;
            case 4:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
            }
                break;
            case 5:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
            }
                break;
            case 6:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
            }
                break;
            case 7:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[vip->packageConfigMap[itemId].boundId].icon.c_str());
            }
                break;
            case 8:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[vip->packageConfigMap[itemId].boundId].icon.c_str());
            }
                break;
            case 9:
            {
                info.boundIcon[index] = GameUtils::format(COMMON_DIR, "shimo1.png");
            }
                break;
            case 10:
            {
                info.boundIcon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            }
                break;
            case 11:
            {
                info.boundIcon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            }
                break;
            default:
            {
                index--;
            }
                break;
        }
        index++;
    }

    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::showVIPDetail(E2L_UPDATE_ONE_VALUE info)
{
    L2E_SHOW_VIP_DETAIL infoOut;
    infoOut.eProtocol = l2e_show_vip_detail;
    infoOut.detailVipLevel = info.value;
    for (int i = 0; i < 10; i++) {
        infoOut.detailDescript[i] = vip->vipConfigMap[infoOut.detailVipLevel].descript[i];
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::vipJumpRecharge()
{
    L2E_COMMON info;
    info.eProtocol = l2e_hide_vip;
    ClientLogic::instance()->pass2Engine(&info);
    
    E2L_UPDATE_ONE_VALUE infoShop;
    infoShop.eProtocol = e2l_click_main_shop;
    infoShop.value = 1;
    showHallShop(infoShop);
}

void SystemLogic::takeCampaignActiveBound(E2L_UPDATE_ONE_VALUE info)
{
    int missionId = info.value;
    L2E_UPDATE_CAMPAIGN_ACTIVE_MISSION infoOut;
    infoOut.eProtocol = l2e_update_mission_bound;
    infoOut.errNo = campaign->takeActiveMissionBound(missionId);
    if (infoOut.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_CAMPAIGN_TIP infoCampaignTip;
    infoCampaignTip.eProtocol = l2e_update_campaign_tip;
    infoCampaignTip.activeTip = campaign->getActiveTip();
    infoCampaignTip.boundTip = campaign->getBoundTip();
    infoCampaignTip.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&infoCampaignTip);
    
    int boundType = campaign->activeMissionMap[missionId].boundType;
    int boundId = campaign->activeMissionMap[missionId].boundId;
    int boundCount = campaign->activeMissionMap[missionId].boundCount;
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    infoGet.count[0] = boundCount;
    infoGet.frag[0] = false;
    switch (boundType) {
        case 1:
        {
            account->changeGold(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-16.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("gold");
            
            L2E_UPDATE_ONE_VALUE infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.value = account->getData().gold;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = boundCount;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);
        }
            break;
        case 2:
        {
            account->changeDiamond(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
            
            L2E_UPDATE_ONE_VALUE infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.value = account->getData().diamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
        }
            break;
        case 3:
        {
            vit->addPlusVIT(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "tilibiao.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("VIT");
            
            L2E_UPDATE_VIT infoVIT;
            infoVIT.eProtocol = l2e_update_vit;
            infoVIT.currVIT = vit->getTotalVIT();
            infoVIT.maxVIT = vit->getMaxBaseVIT();
            ClientLogic::instance()->pass2Engine(&infoVIT);
        }
            break;
        case 4:
        {
            sword->addStone(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "hunshi.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("stone");
            
            sword->checkTip(account->getData().gold);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 3;
            infoTip.tipValue = sword->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 5:
        {
            account->changeFame(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "shengwang.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("fame");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 6:
        {
            prov->addTicket(boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, "saodangquan.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("prov_ticket");
        }
            break;
        case 7:
        {
            arms->addGem(boundId, boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("gem");
            
            arms->checkTip(account->getData().fame);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 4;
            infoTip.tipValue = arms->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 8:
        {
            equip->addItem(boundId, boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("equip_item_fragment");
            
            equip->checkTip(account->getData().level);
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 1;
            infoTip.tipValue = equip->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 9:
        {
//            int petId = abs(rand())%pet->petConfigMap.size()+1;
//            pet->addFragment(petId, infoGet.count[index]);
            pet->addFragment(pet->petConfigMap[boundId].petId, boundCount);
            infoGet.icon[0] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
            infoGet.name[0] = StringData::shared()->stringFromKey("pet_fragment");
            infoGet.frag[0] = true;
            
            pet->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 2;
            infoTip.tipValue = pet->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
        }
            break;
        case 10:
        {
            lucky->addNormalTicket(boundCount);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("normal_draw_ticket");
        }
            break;
        case 11:
        {
            lucky->addSpecialTicket(boundCount);
            infoGet.icon[0] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
            infoGet.name[0] = StringData::shared()->stringFromKey("special_draw_ticket");
        }
            break;
        default:
            break;
    }
    ClientLogic::instance()->pass2Engine(&infoGet);
    
    infoOut.missionId = missionId;
    infoOut.totalActiveVal = campaign->getCoreData().totalActiveVal;
    memcpy(infoOut.boxState, campaign->getCoreData().boxState, sizeof(int)*4);
    infoOut.boundState = campaign->activeDataMap[missionId].boundState;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void SystemLogic::takeCampaignActiveBoxBound(E2L_UPDATE_ONE_VALUE info)
{
    int boxId = info.value;
    L2E_UPDATE_CAMPAIGN_ACTIVE_BOX infoOut;
    infoOut.eProtocol = l2e_update_active_box;
    infoOut.errNo = campaign->takeBoxBound(boxId);
    if (infoOut.errNo != 0) {
        std::vector<int> packageIdVec;
        campaign->getPackageById(boxId, packageIdVec);
        infoOut.boundCount = (int)packageIdVec.size();
        for (auto itemId : packageIdVec) {
            int boundType = campaign->activeBoxMap[itemId].boundType;
            int boundId = campaign->activeBoxMap[itemId].boundId;
            int boundCount = campaign->activeBoxMap[itemId].boundCount;
            infoOut.count.push_back(boundCount);
            switch (boundType) {
                case 1:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "drop-16.png"));
                }
                    break;
                case 2:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "drop-17.png"));
                }
                    break;
                case 3:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "tilibiao.png"));
                }
                    break;
                case 4:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "hunshi.png"));
                }
                    break;
                case 5:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "shengwang.png"));
                }
                    break;
                case 6:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "saodangquan.png"));
                }
                    break;
                case 7:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str()));
                }
                    break;
                case 8:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str()));
                }
                    break;
                case 9:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str()));
                }
                    break;
                case 10:
                {
                    infoOut.icon.push_back(GameUtils::format(LUCKY_DIR, "chouqu1.png"));
                }
                    break;
                case 11:
                {
                    infoOut.icon.push_back(GameUtils::format(LUCKY_DIR, "chouqu2.png"));
                }
                    break;
                case 16:
                {
                    infoOut.icon.push_back(GameUtils::format(COMMON_DIR, "jingyan.png"));
                }
                    break;
                default:
                    break;
            }
        }
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    
    std::vector<int> packageIdVec;
    campaign->getPackageById(boxId, packageIdVec);
    infoOut.boundCount = (int)packageIdVec.size();
    int index = 0;
    for (auto itemId : packageIdVec) {
        int boundType = campaign->activeBoxMap[itemId].boundType;
        int boundId = campaign->activeBoxMap[itemId].boundId;
        int boundCount = campaign->activeBoxMap[itemId].boundCount;
        infoGet.count[index] = boundCount;
        infoGet.frag[index] = false;
        switch (boundType) {
            case 1:
            {
                account->changeGold(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("gold");
                
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = boundCount;
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);

            }
                break;
            case 2:
            {
                account->changeDiamond(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("diamond");
                
                L2E_UPDATE_ONE_VALUE infoDiamond;
                infoDiamond.eProtocol = l2e_update_diamond;
                infoDiamond.value = account->getData().diamond;
                ClientLogic::instance()->pass2Engine(&infoDiamond);
            }
                break;
            case 3:
            {
                vit->addPlusVIT(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("VIT");
                
                L2E_UPDATE_VIT infoVIT;
                infoVIT.eProtocol = l2e_update_vit;
                infoVIT.currVIT = vit->getTotalVIT();
                infoVIT.maxVIT = vit->getMaxBaseVIT();
                ClientLogic::instance()->pass2Engine(&infoVIT);
            }
                break;
            case 4:
            {
                sword->addStone(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("stone");
                
                sword->checkTip(account->getData().gold);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 3;
                infoTip.tipValue = sword->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 5:
            {
                account->changeFame(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("fame");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 6:
            {
                prov->addTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                arms->addGem(boundId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("gem");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 8:
            {
                equip->addItem(boundId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("equip_item_fragment");
                
                equip->checkTip(account->getData().level);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 1;
                infoTip.tipValue = equip->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 9:
            {
                //            int petId = abs(rand())%pet->petConfigMap.size()+1;
                //            pet->addFragment(petId, infoGet.count[index]);
                pet->addFragment(pet->petConfigMap[boundId].petId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("pet_fragment");
                infoGet.frag[index] = true;
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 10:
            {
                lucky->addNormalTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("normal_draw_ticket");
            }
                break;
            case 11:
            {
                lucky->addSpecialTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("special_draw_ticket");
            }
                break;
            case 16:
            {
                int originLevel = account->getData().level;
                account->addExp(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "jingyan.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("exp");
                
                L2E_UPDATE_EXP infoExp;
                infoExp.eProtocol = l2e_update_exp;
                infoExp.currExp = account->getData().exp;
                infoExp.level = account->getData().level;
                infoExp.upLevel = (infoExp.level>originLevel);
                if (infoExp.upLevel) {
                    equip->checkTip(infoExp.level);
                    
                    if (infoExp.level>=15 && originLevel < 15) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(15);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                    
                    if (infoExp.level>=10 && originLevel < 10) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(10);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                    if (infoExp.level>=5 && originLevel < 5) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(5);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                }
                if (account->isMaxLevel()) {
                    infoExp.currExp = 100;
                    infoExp.maxExp = 100;
                }else{
                    int baseExp = 0;
                    if (infoExp.level > 1) {
                        baseExp = account->uplevelConfigMap[infoExp.level-1].expThres;
                    }
                    int nextExp = account->uplevelConfigMap[infoExp.level].expThres;
                    infoExp.currExp = account->getData().exp - baseExp;
                    infoExp.maxExp = nextExp-baseExp;
//                    infoExp.currExp = account->getData().exp;
//                    infoExp.maxExp = account->uplevelConfigMap[infoExp.level].expThres;
                }
                
                infoExp.hp = account->getData().hp;
                infoExp.attack = account->getData().attack;
                infoExp.defence = account->getData().defence;
                infoExp.crit = account->getData().crit;
                infoExp.deCrit = account->getData().deCrit;
                infoExp.force = account->getForce();
                ClientLogic::instance()->pass2Engine(&infoExp);
            }
                break;
            default:
            {
                index--;
            }
                break;
        }
        index++;
    }

    ClientLogic::instance()->pass2Engine(&infoGet);
    
    memcpy(infoOut.boxState, campaign->getCoreData().boxState, sizeof(int)*4);
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "every_day_active";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    umengInfo.attrMap["boxId"] = Convert2String(boxId);
    ClientLogic::instance()->pass2Service(&umengInfo);
}

void SystemLogic::campaignActiveJump(E2L_UPDATE_ONE_VALUE info)
{
    int missionId = info.value;
    int type = campaign->activeMissionMap[missionId].activeType;
    bool locked = false;
    switch (type) {
        case 1:
        {
            locked = (account->unlockMap[10].lock == 1);
        }
            break;
        case 2:
        {
            locked = false;
        }
            break;
        case 3:
        {
            locked = (account->unlockMap[3].lock == 1);
        }
            break;
        case 4:
        {
            locked = (account->unlockMap[6].lock == 1);
        }
            break;
        case 5:
        {
            locked = (account->unlockMap[8].lock == 1);
        }
            break;
        case 6:
        {
            locked = (account->unlockMap[9].lock == 1);
        }
            break;
        case 7:
        {
            locked = (account->unlockMap[11].lock == 1);
        }
            break;
            
        default:
            break;
    }
    if (locked) {
        L2E_COMMON infoLock;
        infoLock.eProtocol = l2e_jump_locked;
        ClientLogic::instance()->pass2Engine(&infoLock);
        return;
    }
    L2E_COMMON infoCommon;
    infoCommon.eProtocol = l2e_hide_campaign;
    ClientLogic::instance()->pass2Engine(&infoCommon);
    switch (type) {
        case 1:
        {
            showLucky();
        }
            break;
        case 2:
        {
            ChapterLogic::instance()->showBattle();
        }
            break;
        case 3:
        {
            showHallAbyss();
        }
            break;
        case 4:
        {
            ChapterLogic::instance()->showTower();
        }
            break;
        case 5:
        {
            showHallProv();
        }
            break;
        case 6:
        {
            showHallArena();
        }
            break;
        case 7:
        {
            showHallPetOffer();
        }
            break;
        case 8:
        {
            
        }
            break;
            
        default:
            break;
    }
}

void SystemLogic::refreshCampaignActive()
{
    campaign->resetActive();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_refresh_campaign_active;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_SHOW_CAMPAIGN_ACTIVE info;
    info.eProtocol = l2e_refresh_campaign_active;
    info.totalActiveVal = campaign->getCoreData().totalActiveVal;
    memcpy(info.boxState, campaign->getCoreData().boxState, sizeof(int)*4);
    
    info.missionCount = (int)campaign->activeMissionMap.size();
    info.descript.clear();
    info.count.clear();
    info.totalCount.clear();
    info.activeVal.clear();
    info.boundCount.clear();
    info.boundState.clear();
    for (auto config : campaign->activeMissionMap) {
        info.missionId.push_back(config.second.missionId);
        info.descript.push_back(config.second.descript);
        info.totalCount.push_back(config.second.count);
        info.activeVal.push_back(config.second.activeVal);
        info.boundCount.push_back(config.second.boundCount);
    }
    for (auto data : campaign->activeDataMap) {
        info.count.push_back(data.second.count);
        info.boundState.push_back(data.second.boundState);
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    
    ClientLogic::instance()->pass2Engine(&infoTip);
    L2E_UPDATE_CAMPAIGN_TIP infoCampaignTip;
    infoCampaignTip.eProtocol = l2e_update_campaign_tip;
    infoCampaignTip.activeTip = campaign->getActiveTip();
    infoCampaignTip.boundTip = campaign->getBoundTip();
    infoCampaignTip.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&infoCampaignTip);
}

void SystemLogic::takeFreeDiamond()
{
    int count = freeDiamondCount[campaign->getCoreData().freeDiamondId-1];
    L2E_UPDATE_FREE_DIAMOND info;
    info.eProtocol = l2e_update_free_diamond;
    info.errNo = campaign->takeFreeDiamond();
    if (info.errNo == 0) {
        account->changeDiamond(count);
        L2E_UPDATE_ONE_VALUE infoDiamond;
        infoDiamond.eProtocol = l2e_update_diamond;
        infoDiamond.value = account->getData().diamond;
        ClientLogic::instance()->pass2Engine(&infoDiamond);
        campaign->activeMission(8, 1);
        
        L2E_SHOW_GET infoGet;
        infoGet.eProtocol = l2e_show_get;
        memset(infoGet.count, 0, sizeof(int)*20);
        infoGet.count[0] = count;
        infoGet.frag[0] = false;
        infoGet.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
        infoGet.name[0] = StringData::shared()->stringFromKey("diamond");
        ClientLogic::instance()->pass2Engine(&infoGet);
        
        campaign->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = campaign->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }
    info.freeDiamondState = campaign->getCoreData().freeDiamondState;
    info.onlineDuration = campaign->getCoreData().onlineDuration;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::resetFreeDiamond()
{
    campaign->resetFreeDiamond();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_reset_free_diamond;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    L2E_UPDATE_FREE_DIAMOND info;
    info.eProtocol = l2e_update_free_diamond;
    info.errNo = 0;
    info.freeDiamondState = campaign->getCoreData().freeDiamondState;
    info.onlineDuration = campaign->getCoreData().onlineDuration;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::showCampaignBoundMission()
{
    L2E_UPDATE_CAMPAIGN_BOUND_MISSION info;
    info.eProtocol = l2e_show_campaign_bound_mission;
    int index = 0;
    memset(info.boundCount, 0, sizeof(int)*200);
    std::vector<int> missionIdVec;
    campaign->currBoundMission(missionIdVec);
    info.missionCount = (int)missionIdVec.size();
    for (auto missionId : missionIdVec) {
        info.missionId.push_back(missionId);
        info.missionState.push_back(campaign->boundDataMap[missionId].boundState);
        info.missionIcon.push_back(campaign->boundMissionMap[missionId].icon);
        if (campaign->boundMissionMap[missionId].activeType == 8) {
            info.count.push_back(campaign->boundDataMap[missionId].maxThres);
            info.totalCount.push_back(campaign->boundMissionMap[missionId].activeThres);
        }else{
            info.count.push_back(campaign->boundDataMap[missionId].count);
            info.totalCount.push_back(campaign->boundMissionMap[missionId].count);
        }
        
        
        info.descript.push_back(campaign->boundMissionMap[missionId].descript);
        
        int packageId = campaign->boundMissionMap[missionId].packageId;
        std::vector<int> packageIdVec;
        campaign->getBoundPackageById(packageId, packageIdVec);
        info.totalBound.push_back((int)packageIdVec.size());

        int boundIndex = 0;
        for (auto itemId : packageIdVec) {
            if (boundIndex >= 4) {
                break;
            }
            info.boundCount[index][boundIndex] = campaign->boundPackageMap[itemId].boundCount;

            int boundType = campaign->boundPackageMap[itemId].boundType;
            int boundId = campaign->boundPackageMap[itemId].boundId;
            switch (boundType) {
                case 1:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "drop-16.png");
                }
                    break;
                case 2:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "drop-17.png");
                }
                    break;
                case 3:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                }
                    break;
                case 4:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "hunshi.png");
                }
                    break;
                case 5:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "shengwang.png");
                }
                    break;
                case 6:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                }
                    break;
                case 7:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                }
                    break;
                case 8:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
                }
                    break;
                case 9:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                }
                    break;
                case 10:
                {
                    info.icon[index][boundIndex] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                }
                    break;
                case 11:
                {
                    info.icon[index][boundIndex] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                }
                    break;
                case 16:
                {
                    info.icon[index][boundIndex] = GameUtils::format(COMMON_DIR, "jingyan.png");
                }
                    break;
                default:
                    break;
            }
            
            boundIndex++;
        }
        index++;
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::takeCampaignMissionReward(E2L_UPDATE_ONE_VALUE info)
{
    int missionId = info.value;
    int packageId = campaign->boundMissionMap[missionId].packageId;
    int errNo = campaign->takeBoundMissionReward(missionId);
    if (errNo != 0) {
        return;
    }
    
    showCampaignBoundMission();
    
    campaign->checkTip();
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 7;
    infoTip.tipValue = campaign->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_UPDATE_CAMPAIGN_TIP infoCampaignTip;
    infoCampaignTip.eProtocol = l2e_update_campaign_tip;
    infoCampaignTip.activeTip = campaign->getActiveTip();
    infoCampaignTip.boundTip = campaign->getBoundTip();
    infoCampaignTip.monthTip = campaign->getMonthTip();
    ClientLogic::instance()->pass2Engine(&infoCampaignTip);

    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    
    std::vector<int> packageIdVec;
    campaign->getBoundPackageById(packageId, packageIdVec);
    int index = 0;
    for (auto itemId : packageIdVec) {
        int boundType = campaign->boundPackageMap[itemId].boundType;
        int boundId = campaign->boundPackageMap[itemId].boundId;
        int boundCount = campaign->boundPackageMap[itemId].boundCount;
        infoGet.count[index] = boundCount;
        infoGet.frag[index] = false;
        switch (boundType) {
            case 1:
            {
                account->changeGold(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("gold");
                
                L2E_UPDATE_ONE_VALUE infoGold;
                infoGold.eProtocol = l2e_update_gold;
                infoGold.value = account->getData().gold;
                ClientLogic::instance()->pass2Engine(&infoGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = boundCount;
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);

            }
                break;
            case 2:
            {
                account->changeDiamond(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("diamond");
                
                L2E_UPDATE_ONE_VALUE infoDiamond;
                infoDiamond.eProtocol = l2e_update_diamond;
                infoDiamond.value = account->getData().diamond;
                ClientLogic::instance()->pass2Engine(&infoDiamond);
            }
                break;
            case 3:
            {
                vit->addPlusVIT(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("VIT");
                
                L2E_UPDATE_VIT infoVIT;
                infoVIT.eProtocol = l2e_update_vit;
                infoVIT.currVIT = vit->getTotalVIT();
                infoVIT.maxVIT = vit->getMaxBaseVIT();
                ClientLogic::instance()->pass2Engine(&infoVIT);
            }
                break;
            case 4:
            {
                sword->addStone(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("stone");
                
                sword->checkTip(account->getData().gold);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 3;
                infoTip.tipValue = sword->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 5:
            {
                account->changeFame(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("fame");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 6:
            {
                prov->addTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("prov_ticket");
            }
                break;
            case 7:
            {
                arms->addGem(boundId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("gem");
                
                arms->checkTip(account->getData().fame);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 4;
                infoTip.tipValue = arms->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 8:
            {
                equip->addItem(boundId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("equip_item_fragment");
                
                equip->checkTip(account->getData().level);
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 1;
                infoTip.tipValue = equip->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 9:
            {
                //            int petId = abs(rand())%pet->petConfigMap.size()+1;
                //            pet->addFragment(petId, infoGet.count[index]);
                pet->addFragment(pet->petConfigMap[boundId].petId, boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[boundId].icon.c_str());
                infoGet.name[index] = StringData::shared()->stringFromKey("pet_fragment");
                infoGet.frag[index] = true;
                
                pet->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 2;
                infoTip.tipValue = pet->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
            }
                break;
            case 10:
            {
                lucky->addNormalTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("normal_draw_ticket");
            }
                break;
            case 11:
            {
                lucky->addSpecialTicket(boundCount);
                infoGet.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("special_draw_ticket");
            }
                break;
            case 16:
            {
                int originLevel = account->getData().level;
                account->addExp(boundCount);
                infoGet.icon[index] = GameUtils::format(COMMON_DIR, "jingyan.png");
                infoGet.name[index] = StringData::shared()->stringFromKey("exp");
                
                L2E_UPDATE_EXP infoExp;
                infoExp.eProtocol = l2e_update_exp;
                infoExp.currExp = account->getData().exp;
                infoExp.level = account->getData().level;
                infoExp.upLevel = (infoExp.level>originLevel);
                if (infoExp.upLevel) {
                    equip->checkTip(infoExp.level);
                    
                    if (infoExp.level>=15 && originLevel < 15) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(15);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                    
                    if (infoExp.level>=10 && originLevel < 10) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(10);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                    if (infoExp.level>=5 && originLevel < 5) {
                        C2S_UMENG_TRACK umengLevelInfo;
                        umengLevelInfo.eProtocol = c2s_umeng_track;
                        umengLevelInfo.eventName = "user_up_level";
                        umengLevelInfo.count = 0;
                        umengLevelInfo.attrMap.clear();
                        umengLevelInfo.attrMap["level"] = Convert2String(5);
                        ClientLogic::instance()->pass2Service(&umengLevelInfo);
                    }
                }
                if (account->isMaxLevel()) {
                    infoExp.currExp = 100;
                    infoExp.maxExp = 100;
                }else{
                    int baseExp = 0;
                    if (infoExp.level > 1) {
                        baseExp = account->uplevelConfigMap[infoExp.level-1].expThres;
                    }
                    int nextExp = account->uplevelConfigMap[infoExp.level].expThres;
                    infoExp.currExp = account->getData().exp - baseExp;
                    infoExp.maxExp = nextExp-baseExp;
//                    infoExp.currExp = account->getData().exp;
//                    infoExp.maxExp = account->uplevelConfigMap[infoExp.level].expThres;
                }
                
                infoExp.hp = account->getData().hp;
                infoExp.attack = account->getData().attack;
                infoExp.defence = account->getData().defence;
                infoExp.crit = account->getData().crit;
                infoExp.deCrit = account->getData().deCrit;
                infoExp.force = account->getForce();
                ClientLogic::instance()->pass2Engine(&infoExp);
            }
                break;
            default:
            {
                index--;
            }
                break;
        }
        index++;
    }
    
    ClientLogic::instance()->pass2Engine(&infoGet);
}

void SystemLogic::campaignMissionJump(E2L_UPDATE_ONE_VALUE info)
{
    int missionId = info.value;
    int type = campaign->boundMissionMap[missionId].activeType;
    bool locked = false;
    switch (type) {
        case 1:
        {
            locked = (account->unlockMap[10].lock == 1);
        }
            break;
        case 2:
        {
            locked = false;
        }
            break;
        case 3:
        {
            locked = (account->unlockMap[3].lock == 1);
        }
            break;
        case 4:
        {
            locked = (account->unlockMap[6].lock == 1);
        }
            break;
        case 5:
        {
            locked = (account->unlockMap[8].lock == 1);
        }
            break;
        case 6:
        {
            locked = (account->unlockMap[9].lock == 1);
        }
            break;
        case 7:
        {
            locked = (account->unlockMap[11].lock == 1);
        }
            break;
            
        default:
            break;
    }
    if (locked) {
        L2E_COMMON infoLock;
        infoLock.eProtocol = l2e_jump_locked;
        ClientLogic::instance()->pass2Engine(&infoLock);
        return;
    }
    L2E_COMMON infoCommon;
    infoCommon.eProtocol = l2e_hide_campaign;
    ClientLogic::instance()->pass2Engine(&infoCommon);
    switch (type) {
        case 1:
        {
            showHallHero();
        }
            break;
        case 2:
        {
            showHallPet();
        }
            break;
        case 3:
        {
            showHallHolySword();
        }
            break;
        case 4:
        case 5:
        {
            showHallArms();
        }
            break;
        case 6:
        {
            showHallPetOffer();
        }
            break;
        case 7:
        case 11:
        {
            ChapterLogic::instance()->showBattle();
        }
            break;
        case 8:
        {
            showHallArena();
        }
            break;
        case 9:
        {
            showHallAbyss();
        }
            break;
        case 10:
        {
            E2L_UPDATE_ONE_VALUE infoShop;
            infoShop.eProtocol = e2l_click_main_shop;
            infoShop.value = 1;
            showHallShop(infoShop);
        }
            break;
            
        default:
            break;
    }
}

void SystemLogic::openDiscount()
{
    log("openDiscount--------------");
    int index = 0;
    for (; index < 2; index++) {
        if (!TimeUtil::isGreaterHour(discountStart[index])) {
            break;
        }
        if (!TimeUtil::isGreaterHour(discountEnd[index])) {
            shop->enableDiscount();
            log("open Discount>>>>>>>");
            break;
        }
    }
    log("open index:%d", index);
    
    switch (shop->getCoreData().discountState) {
        case 0:
        {
            log("can't open discount XXXXX");
            updateDiscountState();
            
            L2T_WAIT_DISCOUNT_OPEN info;
            info.eProtocol = l2t_wait_discount_open;
            
            if (index == 2) {
                log("wait tomorrow %d", discountStart[0]);
                info.openSec = TimeUtil::getTimeWithTomorrowHour(1, discountStart[0]);
            }else{
                log("wait today %d", discountStart[index]);
                info.openSec = TimeUtil::getTimeWithTomorrowHour(0, discountStart[index]);
            }
            ClientLogic::instance()->pass2Time(&info);
        }
            break;
        case 1:
        {
            long openSec = TimeUtil::leftSec(discountEnd[index]);
            openSec = MIN(1800, openSec);
            log("show close cd:%d",(int)openSec);
            updateDiscountState(openSec);
            L2T_WAIT_DISCOUNT_CLOSE info;
            info.eProtocol = l2t_wait_discount_close;
            info.closeSec = TimeUtil::get_system_tick_s()+openSec;
            ClientLogic::instance()->pass2Time(&info);
        }
            break;
        default:
            break;
    }
    
    log("end openDiscount===========");
}

void SystemLogic::closeDiscount()
{
    log("closeDiscount--------------");
    shop->disableDiscount();
    updateDiscountState();
    L2T_WAIT_DISCOUNT_OPEN info;
    info.eProtocol = l2t_wait_discount_open;
    log("wait tomorrow %d", discountStart[0]);
    info.openSec = TimeUtil::getTimeWithTomorrowHour(1, discountStart[0]);
    ClientLogic::instance()->pass2Time(&info);
    log("end closeDiscount===========");
}

void SystemLogic::getDiscountState()
{
    log("getDiscountState--------------");
    if (shop->getCoreData().discountState == 0) {
        log("state = close");
        updateDiscountState();
        log("end getDiscountState===========");
        return;
    }
    
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_get_discount_close_cd;
    ClientLogic::instance()->pass2Time(&infoTime);
    log("end getDiscountState===========");
}

void SystemLogic::updateDiscountState(long val)
{
    log("updateDiscountState--------------");
    L2E_UPDATE_DISCOUNT_STATE info;
    info.eProtocol = l2e_update_discount_state;
    info.state = shop->getCoreData().discountState;
    info.leftSec = val;
    log("cd sec:%d", (int)val);
    if (info.state == 1) {
        log("Discount state = open");
        int packageId = shop->getCoreData().discountPackage;
        
        info.packageId = packageId;
        info.packageImg = shop->shopConfigMap[packageId].packageImg;
        info.packageTitle = shop->shopConfigMap[packageId].packageTitle;
        info.price = shop->shopConfigMap[packageId].price;
        
        memset(info.count, 0, sizeof(int)*100);
        if (shop->shopConfigMap[packageId].boundType == 1) {
            info.itemCount = 1;
            info.count[0] = shop->shopConfigMap[packageId].boundCount;
            info.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            
        }else{
            std::vector<int> packageIdVec;
            shop->getPackageById(shop->shopConfigMap[packageId].boundId, packageIdVec);
            info.itemCount = (int)packageIdVec.size();
            if (info.itemCount > 0) {
                int index = 0;
                for (auto packageId : packageIdVec) {
                    info.count[index] = shop->packageConfigMap[packageId].boundCount;
                    int boundType = shop->packageConfigMap[packageId].boundType;
                    switch(boundType)
                    {
                        case 1:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                        }
                            break;
                        case 2:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                        }
                            break;
                        case 3:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                            
                        }
                            break;
                        case 4:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                        }
                            break;
                        case 5:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                        }
                            break;
                        case 6:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                        }
                            break;
                        case 7:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 8:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 9:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 10:
                        {
                            info.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                        }
                            break;
                        case 11:
                        {
                            info.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                        }
                            break;
                    }
                    index++;
                }
            }
        }
        
    }
    ClientLogic::instance()->pass2Engine(&info);
    log("end updateDiscountState===========");
}

void SystemLogic::showDiscount()
{
    log("showDiscount--------------");
    L2E_SHOW_DISCOUNT info;
    info.eProtocol = l2e_show_hall_discount;
    info.state = shop->getCoreData().discountState;
    if (info.state == 1) {
        log("Discount state = open");
        int packageId = shop->getCoreData().discountPackage;
        
        info.packageId = packageId;
        info.packageImg = shop->shopConfigMap[packageId].packageImg;
        info.packageTitle = shop->shopConfigMap[packageId].packageTitle;
        info.price = shop->shopConfigMap[packageId].price;
        
        memset(info.count, 0, sizeof(int)*100);
        if (shop->shopConfigMap[packageId].boundType == 1) {
            info.itemCount = 1;
            info.count[0] = shop->shopConfigMap[packageId].boundCount;
            info.icon[0] = GameUtils::format(COMMON_DIR, "drop-17.png");
            
        }else{
            std::vector<int> packageIdVec;
            shop->getPackageById(shop->shopConfigMap[packageId].boundId, packageIdVec);
            info.itemCount = (int)packageIdVec.size();
            if (info.itemCount > 0) {
                int index = 0;
                for (auto packageId : packageIdVec) {
                    info.count[index] = shop->packageConfigMap[packageId].boundCount;
                    int boundType = shop->packageConfigMap[packageId].boundType;
                    switch(boundType)
                    {
                        case 1:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "drop-16.png");
                        }
                            break;
                        case 2:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "drop-17.png");
                        }
                            break;
                        case 3:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                            
                        }
                            break;
                        case 4:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "hunshi.png");
                        }
                            break;
                        case 5:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "shengwang.png");
                        }
                            break;
                        case 6:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                        }
                            break;
                        case 7:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, arms->gemsConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 8:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, equip->equipItemConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 9:
                        {
                            info.icon[index] = GameUtils::format(COMMON_DIR, pet->petConfigMap[shop->packageConfigMap[packageId].boundId].icon.c_str());
                        }
                            break;
                        case 10:
                        {
                            info.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu1.png");
                        }
                            break;
                        case 11:
                        {
                            info.icon[index] = GameUtils::format(LUCKY_DIR, "chouqu2.png");
                        }
                            break;
                    }
                    index++;
                }
            }
        }
        
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_discount";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
    
    log("Discount state :%d",info.state);
    log("end showDiscount===========");
}

void SystemLogic::clickExit()
{
    TimeManager::instance()->saveData();
    Campaign::instance()->saveDiamond();
    
    C2S_EXIT_APP info;
    info.eProtocol = c2s_exit_app;
    ClientLogic::instance()->pass2Service(&info);
}

void SystemLogic::exitApp()
{
    TimeManager::instance()->saveData();
    Campaign::instance()->saveDiamond();
    Director::getInstance()->end();
//    exit(0);
}

void SystemLogic::getVITCD()
{
    if (vit->getBaseVIT()>=vit->getMaxBaseVIT()) {
        L2E_UPDATE_VIT_CD info;
        info.eProtocol = l2e_update_vit_cd;
        info.state = false;
        info.leftSec = 0;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    L2T_COMMON info;
    info.eProtocol = l2t_get_vit_restore_sec;
    ClientLogic::instance()->pass2Time(&info);
}

void SystemLogic::updateVITCD(T2L_COMMON infoTime)
{
    L2E_UPDATE_VIT_CD info;
    info.eProtocol = l2e_update_vit_cd;
    if (vit->getBaseVIT()>=vit->getMaxBaseVIT()) {
        info.state = false;
        info.leftSec = 0;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.state = true;
    info.leftSec = infoTime.passSec;
    ClientLogic::instance()->pass2Engine(&info);
}

void SystemLogic::initServerTime(S2C_SERVER_TIME info)
{
    TimeUtil::setServerTime(info.timeStr);
}
