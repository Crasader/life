//
//  RandomName.h
//  life
//
//  Created by ff on 17/9/18.
//
//

#ifndef RandomName_h
#define RandomName_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

class RandomName : public Singleton<RandomName> {
public:
    RandomName();
    ~RandomName();
    void readPart1Config();
    void readPart2Config();
    
    std::string getRandomName();
public:
    static std::map<int, int> part1IdTable;
    static std::map<int, int> part2IdTable;
private:
    std::map<int, std::string> part1Map;
    std::map<int, std::string> part2Map;
};

#endif /* RandomName_h */
