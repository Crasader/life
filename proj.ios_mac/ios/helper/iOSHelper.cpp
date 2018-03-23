//
//  iOSHelper.cpp
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#include "iOSHelper.h"
#include "CSProtocol.h"
#include "logic/ClientLogic.h"


std::string iOSHelper::uuidStr = "";
bool iOSHelper::isConnected = false;


iOSHelper *iOSHelper::instance = nullptr;

iOSHelper *iOSHelper::getInstance()
{
    if (instance == nullptr) {
        instance = new iOSHelper;
    }
    
    return instance;
}

