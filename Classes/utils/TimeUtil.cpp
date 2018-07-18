#include "TimeUtil.h"

double TimeUtil::serverTime = 0;

void TimeUtil::setServerTime(std::string timeStr)
{
    if (timeStr == "") {
        serverTime = 0;
        return;
    }
    tm tm_;
    time_t t_;
    strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &tm_);
    tm_.tm_isdst = -1;
    t_  = mktime(&tm_); //将tm时间转换为秒时间
    
    if (t_ <= 0) { //如解析错误，设为0
        serverTime = 0;
        return;
    }
    serverTime = t_;
}

////切割字符串
//void TimeUtil::splitString(std::vector<std::string> &contentVector,std::string content, std::string pattern)
//{
//    std::string::size_type pos;
//    //std::vector<std::string> result;
//    content += pattern;//扩展字符串以方便操作
//    int size = content.size();
//    
//    for (int i = 0; i<size; i++)
//    {
//        pos = content.find(pattern, i);
//        if (pos<size)
//        {
//            std::string s = content.substr(i, pos - i);
//            // result.push_back(s);
//            contentVector.push_back(s);
//            i = pos + pattern.size() - 1;
//        }
//    }
//}

void TimeUtil::updateServerTime(float dt)
{
    if (serverTime == 0) {
        return;
    }
    
    serverTime += dt;
}

//int64_t TimeUtil::currentTimeMillis() {
//
//    struct timeval now;
//    gettimeofday(&now, NULL); 
//    int64_t when = now.tv_sec * 1000LL + now.tv_usec / 1000;
//    return when;
//}
//int TimeUtil::currentTimeMillis(int64_t time){
//    return (int)TimeUtil::currentTimeMillis()-time;
//}
std::string TimeUtil::timeFormatToYYMMDD(long sec){
    char time[20];
    struct tm *local;
    time_t t =sec;
    local = localtime(&t);
    strftime(time, 20, "%Y-%m-%d %H:%M:%S", local);
    return time;
}

std::string TimeUtil::timeFormatToYMD(long sec)
{
    char time[20];
    struct tm *local;
    time_t t =sec;
    local = localtime(&t);
    strftime(time, 20, "%Y%m%d", local);
    return time;
}

std::string TimeUtil::timeFormatToMS(long sec)
{
    int m = sec/60;
    int s = sec%60;
    char time[20];
    sprintf(time,"%.2d:%.2d",m,s);
    return time;
}

std::string TimeUtil::timeFormatToHMS(long sec)
{
    int h = sec/3600;
    int m = (sec - h * 3600)/60;
    int s = (sec - h * 3600)%60;
    char time[128];
    sprintf(time,"%.2d:%.2d:%.2d", h, m, s);
    return time;
}

//long TimeUtil::get_system_tick()
//{
//    struct timeval now;
//    gettimeofday(&now, 0);
//    return (now.tv_sec * 1000 + now.tv_usec / 1000);
//}

long TimeUtil::get_system_tick_s()
{
    if (serverTime != 0) {
        return serverTime;
    }
    struct timeval now;
	gettimeofday(&now, 0);
	return (now.tv_sec + now.tv_usec / 1000000);
}

//bool TimeUtil::currentTimeInToday(int64_t time){
//    int64_t daytime = 60*60*24*1000;
//    auto d = currentTimeMillis(time);
//    if (d > daytime) {
//        return true;
//    }
//    return false;
//}

int TimeUtil::timeFormatToDay(long sce){
    return sce/24/60/60;
}

long TimeUtil::getTimeWithTomorrowHour(int deltaDay, int hour)
{
    struct tm *local;
    time_t t = get_system_tick_s();
    local = localtime(&t);

    local->tm_mday += deltaDay;
    local->tm_hour = hour;
    local->tm_min = 0;
    local->tm_sec = 0;
    long result = mktime(local);
    return result;
}

bool TimeUtil::isGreaterHour(int hour)
{
    struct tm *local;
    time_t t = get_system_tick_s();
    local = localtime(&t);
    return local->tm_hour >= hour;
}

long TimeUtil::leftSec(int hour)
{
    struct tm *local;
    time_t t = get_system_tick_s();
    local = localtime(&t);
    
    local->tm_hour = hour;
    local->tm_min = 0;
    local->tm_sec = 0;
    time_t result = mktime(local);
    
    return result - t;
}
