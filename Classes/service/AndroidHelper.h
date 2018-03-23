//
//  AndroidHelper.h
//  Lippi
//
//  Created by ff on 16/12/7.
//
//

#ifndef AndroidHelper_h
#define AndroidHelper_h

#include <stdio.h>
#include "cocos2d.h"

class AndroidHelper {
public:
    static std::string getWorkingDir();
    /// 得到设备的唯一标示
    static std::string getDeviceID();
    
    static bool checkConnect();
    
    static void exitGame();
    
    static void pay(int pid, std::string orderId, int price, std::string name);
};

#endif /* AndroidHelper_h */
