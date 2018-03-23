//
//  BuffManager.h
//  WuhunLua
//
//  Created by ff on 15-3-19.
//
//

#ifndef __WuhunLua__BuffManager__
#define __WuhunLua__BuffManager__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "cocostudio/CocoStudio.h"
#include "Buff.h"
#include "Buff_Common.h"

class BuffManager : public cocos2d::Node
{
public:
    bool init();
    
    
//    void onExit();
    
    int addBuff(int idx, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove);
    
    static void initIdTable(rapidjson::Document &_doc);
    
    void addBuff(int buffId, rapidjson::Document& _doc, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove);
    
    void replaceBuff(int buffId, rapidjson::Document& _doc, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove);
    
    void removeAllBuff();
    
    void removeBuff(int buffId);
    
    void update(float dt);
    
    void removeImmu(int buffId);
    
    static std::map<int, int> idTable;
    
    static std::map<int, int> targetTable;
    
    static std::map<int, int> typeTable;
    static std::map<int, bool> deBuffTable;
    
    CREATE_FUNC(BuffManager);
    
private:
    std::map<int, Buff*> buffs;
    std::map<int, int> immuTable;
};

#endif /* defined(__WuhunLua__BuffManager__) */
