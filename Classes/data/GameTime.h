//
//  Time.hpp
//  life
//
//  Created by ff on 17/7/27.
//
//

#ifndef Time_h
#define Time_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "TLProtocol.h"

typedef struct __TIME_EVENT
{
    uint64_t timeFlag;
    T2LProtocol eProtocol;
    bool alive;
}TIME_EVENT;

class GameTime : public Singleton<GameTime> {
public:
    GameTime();
    ~GameTime();
    
    void deleteLoginJson();
    bool readLoginJson();
    bool parseLoginJson(std::string loginStr);
    void save2LoginJson();
    
    void deleteFlagJson();
    bool readFlagJson();
    bool parseFlagJson(std::string flagStr);
    void save2FlagJson();

    void addEvent(T2LProtocol eProtocol, uint64_t time);
    bool removeDeadEvent();
    void invalidEvent(T2LProtocol eProtocol);
    inline void setLoginTime(uint64_t val)
    {
        loginTime = val;
    };
public:
    std::map<T2LProtocol, TIME_EVENT> eventMap;
private:
    uint64_t loginTime;
};
#endif /* Time_h */
