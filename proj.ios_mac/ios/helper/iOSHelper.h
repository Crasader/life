//
//  iOSHelper.h
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#ifndef __diamond__iOSHelper__
#define __diamond__iOSHelper__

#include "cocos2d.h"
//#include "MakeJson.h"

class iOSHelper
{
public:
    static iOSHelper *getInstance();
    
    static std::string uuidStr;
    
    //设备是否联网
    static bool isConnected;
private:
    static iOSHelper *instance;
};

#endif /* defined(__diamond__iOSHelper__) */
