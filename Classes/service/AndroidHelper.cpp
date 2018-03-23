//
//  AndroidHelper.cpp
//  Lippi
//
//  Created by ff on 16/12/7.
//
//

#include "AndroidHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#include "CSProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
extern "C"
{
    void Java_org_cocos2dx_cpp_Depend_buySuccess(JNIEnv *env, jobject thiz, int pid)
    {
        log("back to c++ buySuccess!");
        S2C_PAY info;
        info.eProtocol = s2c_pay;
        info.success = true;
        info.packageId = pid;
        ClientLogic::instance()->ProcessServiceResponse(&info);
        log("pass 2 sever");
        //        ClientLogic::instance()->ProcessServerResponse(&info);
    }
    
    void Java_org_cocos2dx_cpp_Depend_buyError(JNIEnv *env, jobject thiz, int pid)
    {
        log("back to c++ buyError!");
        S2C_PAY info;
        info.eProtocol = s2c_pay;
        info.success = false;
        info.packageId = pid;
        ClientLogic::instance()->ProcessServiceResponse(&info);
        log("pass 2 sever");
        //        ClientLogic::instance()->ProcessServerResponse(&info);
    }
}
#endif


std::string AndroidHelper::getWorkingDir()
{
    std::string ret = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo minfore;
    bool isHavere =JniHelper::getStaticMethodInfo(minfore,"org/cocos2dx/cpp/Depend", "getResSdPath", "()Ljava/lang/String;");
    if (isHavere)
    {
        jstring temppathre=(jstring)minfore.env -> CallStaticObjectMethod(minfore.classID,minfore.methodID);
        jboolean isCopyre;
        ret=std::string(minfore.env->GetStringUTFChars(temppathre,&isCopyre));
    }
    else
    {
        log("have no getWorkingDir");
    }
#endif
    return ret;
}

std::string AndroidHelper::getDeviceID()
{
    
    std::string ret = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo minfore;
    bool isHavere =JniHelper::getStaticMethodInfo(minfore,"org/cocos2dx/cpp/Depend", "getDeviceID", "()Ljava/lang/String;");
    if (isHavere)
    {
        jstring temppathre=(jstring)minfore.env -> CallStaticObjectMethod(minfore.classID,minfore.methodID);
        jboolean isCopyre;
        ret=std::string(minfore.env->GetStringUTFChars(temppathre,&isCopyre));
    }
    else
    {
        log("have no getDeviceID");
    }
#endif
    return ret;
}

bool AndroidHelper::checkConnect()
{
    bool ret = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo minfore;
    bool isHavere =JniHelper::getStaticMethodInfo(minfore,"org/cocos2dx/cpp/Depend", "checkConnect", "()Z");
    if (isHavere)
    {
        jboolean temppathre=(jboolean)minfore.env -> CallStaticBooleanMethod(minfore.classID,minfore.methodID);
        ret = temppathre;
    }
    else
    {
        log("have no checkConnect");
    }
#endif
    
    return ret;
}

void AndroidHelper::exitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo minfore;
    bool isHavere =JniHelper::getStaticMethodInfo(minfore,"org/cocos2dx/cpp/Depend", "exitGame", "()V");
    if (isHavere)
    {
        minfore.env -> CallStaticBooleanMethod(minfore.classID,minfore.methodID);
    }
    else
    {
        log("have no exitGame");
    }
#endif
}

void AndroidHelper::pay(int pid, std::string orderId, int price, std::string name)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo minfore;
    bool isHavere =JniHelper::getStaticMethodInfo(minfore,"org/cocos2dx/cpp/Depend", "pay", "(ILjava/lang/String;ILjava/lang/String;)V");
    if (isHavere)
    {
        jint jpid = pid;
        jstring order_jstr = minfore.env->NewStringUTF(orderId.c_str());
        jint jprice = price;
        jstring name_jstr = minfore.env->NewStringUTF(name.c_str());
        minfore.env->CallStaticVoidMethod(minfore.classID, minfore.methodID, jpid, order_jstr, jprice, name_jstr);
        minfore.env->DeleteLocalRef(order_jstr);
        minfore.env->DeleteLocalRef(name_jstr);
    }
    else
    {
        log("have no pay");
    }
#endif
}
