#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Service/AndroidHelper.h"
#endif
#include "GameDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSHelper.h"
#endif
#include "utils/libs.h"
#include "time/TimeManager.h"
#include "data/Campaign.h"


USING_NS_CC;
using namespace CocosDenshion;

//static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Lippi", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Lippi");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);
    

    // 固定高度的适配策略
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);

    register_all_packages();

#ifdef FOR_HUA
    //android的SD卡直读
    std::vector<std::string> searchPath;
    std::string sdDir;
    #if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        sdDir = AndroidHelper::getWorkingDir();
        log("sd path: %s", sdDir.c_str());
        searchPath.push_back(sdDir);
    #endif
    
    FileUtils::getInstance()->setSearchPaths(searchPath);
#endif
    
    //设置资源文件的加密秘钥和签名
    CryptXXTEA::getInstance()->setXXTEAKeyAndSign(xxteaKeyStr.c_str(), (int)strlen(xxteaKeyStr.c_str()), xxteaSignStr.c_str(), (int)strlen(xxteaSignStr.c_str()));
    
    //设置存档json的加密秘钥
    std::string uuStr("mmcjccc");
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        uuStr += iOSHelper::uuidStr;
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        uuStr += AndroidHelper::getDeviceID().c_str();
    #endif
    coAesSetKey(uuStr.substr(0,32).c_str());
    
    GameUtils::winSize = Director::getInstance()->getWinSize();
    
    //从登陆状态开启游戏
    ClientLogic::instance()->ChangeState(GAME_STATE_LOGO);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    TimeManager::instance()->saveData();
    Campaign::instance()->saveDiamond();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
