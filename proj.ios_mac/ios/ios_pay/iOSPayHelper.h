//
//  iOSPayHelper.h
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#ifndef __iOSPayHelper__
#define __iOSPayHelper__

#include "cocos2d.h"

class iOSPayHelper
{
public:
    void pushPid(int pid, std::string idenStr);
    void pay(int idx);
    
    void paySuccess(int idx);
    
    void payError(int idx);
    
    static iOSPayHelper *getInstance();
private:
    static iOSPayHelper *instance;
    std::map<int, std::string> pidMap;
};

#endif /* defined(__diamond__iOSHelper__) */
