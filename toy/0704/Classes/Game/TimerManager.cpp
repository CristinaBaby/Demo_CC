//
//  TimerManager.cpp
//  TOYBM0701
//
//  Created by huangwen on 15/11/24.
//
//

#include "TimerManager.hpp"
#include "AppConfigs.h"
#include "PensManager.h"
#include "AppConfigs.h"
#include "PackManager.h"
#define TWODAY      172800

TimerManager* TimerManager::_timer = nullptr;

TimerManager* TimerManager::getTimer(){
    if (!_timer) {
        _timer = new TimerManager();
    }
    return _timer;
}


void TimerManager::setPicturePackTime(std::string time, int index){
    _user->setStringForKey((PICTUREPACK_TIME_LOCK + std::to_string(index)).c_str(), time);
    _user->flush();
}

void TimerManager::setPensPackTime(std::string time, int index){
    _user->setStringForKey((PENSPACK_TIME_LOCK + std::to_string(index)).c_str(), time);
    _user->flush();
}

void TimerManager::setPackTime(PACK_TYPE type, std::string time, int index){
    if (PACK_TYPE::PICTURE_PACK == type) {
        setPicturePackTime(time, index);
    }
    else if (PACK_TYPE::PENS_PACK == type)
    {
        setPensPackTime(time, index);
    }
}

std::string TimerManager::getPicturePackTime(int index){
    return _user->getStringForKey((PICTUREPACK_TIME_LOCK + std::to_string(index)).c_str(), "0000-00-00 00-00-00");
}

std::string TimerManager::getPensPackTime(int index){
    return _user->getStringForKey((PENSPACK_TIME_LOCK + std::to_string(index)).c_str(), "0000-00-00 00-00-00");
}

bool TimerManager::isLocked(PACK_TYPE type, int index){
    if (PACK_TYPE::PICTURE_PACK == type) {
        return offsetTime(getPicturePackTime(index));
    }
    else if (PACK_TYPE::PENS_PACK == type)
    {
        return offsetTime(getPensPackTime(index));
    }
}

std::string TimerManager::getCurrTime(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    struct timeval now;
    struct tm* time;
    
    gettimeofday(&now, NULL);
    
    
    time = localtime(&now.tv_sec);
    int year = time->tm_year + 1900;
    log("year = %d",year);
    
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d %02d-%02d-%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
    log("%s",date);
    return StringUtils::format("%s",date);
    
#endif
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    
    struct tm* tm;
    time_t timep;
    time(timep);
    
    tm = localtime(&timep);
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d %02d-%02d-%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
    log("%s",date);
    return StringUtils::format("%s",date);
    
#endif
}

bool TimerManager::offsetTime(std::string time){
    struct tm now;
    struct tm late;
    std::string now_time = getCurrTime();
    now.tm_year = stod(now_time.substr(0,4));
    now.tm_mon = stod(now_time.substr(5,2));
    now.tm_mday = stod(now_time.substr(8,2));
    now.tm_hour = stod(now_time.substr(11,2));
    now.tm_min = stod(now_time.substr(14,2));
    now.tm_sec = stod(now_time.substr(17,2));
    
    late.tm_year = stod(time.substr(0,4));
    late.tm_mon = stod(time.substr(5,2));
    late.tm_mday = stod(time.substr(8,2));
    late.tm_hour = stod(time.substr(11,2));
    late.tm_min = stod(time.substr(14,2));
    late.tm_sec = stod(time.substr(17,2));
    //返回真为超过时间  因该加锁
    if (now.tm_year > late.tm_year) {
        return true;
    }
    else if(now.tm_mon > late.tm_mon)
    {
        return true;
    }
    else if(now.tm_mday - late.tm_mday > 2)
    {
        return true;
    }
    else
    {
        if ((now.tm_mday * 86400 + now.tm_hour * 3600 + now.tm_min * 60 + now.tm_sec) - (late.tm_mday * 86400 + late.tm_hour * 3600 + late.tm_min * 60 + late.tm_sec) > TWODAY) {
            return true;
        }
    }
    return false;
}



