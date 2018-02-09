#include "Utils.h"
#include <string>
#include "../AppGlobal.h"
using namespace std;

#define HOURS_TO_SECONDS    (60 * 60)

STDesignResolution Util::_STDesignResolution = ST_DESIGN_RESOLUTION_1024X768;

bool Util::isExistFile(const char *pszFileName)
{
	FILE *fp = fopen(pszFileName, "rb");
    if(fp == NULL)
        return false;
    
    fclose(fp);
    return true;
}

const char* Util::getLanguageType()
{
	ccLanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();

    switch (currentLanguageType)
    {
    case kLanguageChinese:
		return "zh";
    case kLanguageFrench:
		return "fr";
    case kLanguageGerman:
		return "de";
    case kLanguageItalian:
		return "it";
    case kLanguageRussian:
		return "ru";
    case kLanguageSpanish:
		return "es";
	case kLanguageKorean:
		return "ko";
	default:
		return 0;
    }
}

Sprite* Util::getUISprite(const char* fileName)
{
	const char* language = getLanguageType();
	string path = "UI";
    
	if(language != 0)
	{
		path.append("-");
		path.append(language);
	}
	path.append("/");
	path.append(fileName);

	if(!Util::isExistFile(path.c_str()))
	{
		path="UI/";
		path.append(fileName);
	}
	Sprite* sprite=Sprite::create(path.c_str());

	return sprite;
}

void Util::loadUITexture(const char* file, const char* language /*  = getLanguageType() */)
{
	string plist="UI";
    
	if(language!=0)
	{
		plist.append("-");
		plist.append(language);
	}
    
	plist.append("/");
	plist.append(file);
	plist.append(".plist");

	if(!Util::isExistFile(plist.c_str()))
	{
		plist="UI/";
		plist.append(file);
		plist.append(".plist");
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist.c_str());
}

void Util::removeUITexture(const char* file, const char *language /*  = getLanguageType() */)
{
    string plist="UI";

	if(language!=0)
	{
		plist.append("-");
		plist.append(language);
	}
    
	plist.append("/");
	plist.append(file);
	plist.append(".plist");
    
    if(!Util::isExistFile(plist.c_str()))
	{
		plist="UI/";
		plist.append(file);
		plist.append(".plist");
	}
    
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plist.c_str());
}

void Util::stArrayShuffle(CCArray* src)
{
    // ensure the result is not same as before
    srand(time(NULL));
    
    const int size = src->count();
    // create a random squence
    for(int i = 0; i < size; i++)
    {
        float r1 = CCRANDOM_0_1();
        float r2 = CCRANDOM_0_1();
        
        if (r1 == 1) // to prevent from accessing stAvailableLevels->count() * r1, out of range.
        {
            r1 = 0;
        }
        if (r2 == 1)
        {
            r2 = 0;
        }
        
        if (r1 != r2)
        {
            src->exchangeObjectAtIndex((int)(size * r1), (int)(size * r2));
        }
    }
}

unsigned Util::stGetRandom(unsigned limit)
{
    // ensure the result is not same as before
    srand(time(NULL));
    
    float r = CCRANDOM_0_1();
    if (r == 1) {
        r = 0;
    }
    
    return (unsigned)(limit * r);
}

bool Util::parseBoolean(const std::string &value)
{
    if (value.length() == 0)
    {
        return false;
    }
    
    if (0 == strcmp(value.c_str(), "0") || 0 == strcmp(value.c_str(), "false") || 0 == strcmp(value.c_str(), "FALSE"))
    {
        return false;
    }
    return true;
}

string Util::convertInt(int number)
{
    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

string Util::convertFloat(float number)
{
    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}


/*
tm* Util::getTime(){
	struct tm *tm; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
    //win32∆ΩÃ®
    time_t timep;  
    time(&timep);  
    tm = localtime(&timep); 
#else  
    //android°¢ios∆ΩÃ®
    struct cc_timeval now;  
    CCTime::gettimeofdayCocos2d(&now, NULL); 
    tm = localtime(&now.tv_sec); 
#endif   

	return tm;
}


void Util::saveTime2UD(int year,int month, int day, int hour) {
	STSaveIntegerToUD(last_running_time_year.c_str(),year);
	STSaveIntegerToUD(last_running_time_month.c_str(),month);
	STSaveIntegerToUD(last_running_time_day.c_str(),day);
	STSaveIntegerToUD(last_running_time_hour.c_str(),hour);
}
*/

long Util::getRateUsTimestamp(const std::string &key)
{
    // encode the key value
//    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
//    
//    string temp = CCUserDefault::sharedUserDefault()->getStringForKey(encodeKey.c_str());
//    
//    string tv = base64_decode(temp);
//    
//    if (!tv.empty())
//    {
//        return atol(tv.c_str());
//    }
//    else
//    {
//        return 0;
//    }
    
}

void Util::setRateUsTimestamp(const string &key, int value)
{
    // check key
    if (key.empty())
    {
        return;
    }
    
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), convertInt(value));
    
}

string Util::formatTime(long interval)
{
    CCString* minuteStr = NULL;
    CCString* secondStr = NULL;
    CCString*  hourStr = NULL;
    int hour = interval / 3600;
    int minute = interval / 60;
    int second = interval - minute * 60;
    hourStr   = CCString::createWithFormat("%02d" , hour);
    minuteStr = CCString::createWithFormat("%02d", minute);
    secondStr = CCString::createWithFormat("%02d", second);
    
    string result = "";
    result.append(hourStr->getCString()).append(":").append(minuteStr->getCString()).append(":").append(secondStr->getCString());
    
    return result;
}

string Util::getParkShowName(std::string sInput)
{
    int i = 0;
    stringstream stream;
    
    for (string::iterator it = sInput.begin(); it != sInput.end(); it++)
    {
        if ('A' <= *it && *it <= 'Z')
        {
            i++;
            if(i != 1){
                stream << " ";
            }
        }
        stream << (*it);
    }
    return stream.str();
};

long Util::getCurrentTimeSeconds()
{
    
    time_t  now;
    time(&now);
    return  now;
}

string Util::getMapFilePath(int cur_pieces_number)
{
    stringstream cur_pieces_number_str;
    cur_pieces_number_str << cur_pieces_number;
    string path = "map/ImageSplite";
    path.append(cur_pieces_number_str.str());
    path.append("x");
    path.append(cur_pieces_number_str.str());
    path.append("_s");
    path.append(".csv");
//    log("path=%s" , path.c_str());
    return path;
}

long Util::getHoursByTimeInterval(long before, long now)
{
    long interval = now - before;
    
    long during = interval / HOURS_TO_SECONDS;
    
    return during;
}

Vec2 Util::checkBorder(float positinX, float positionY , int moveX , int moveY)
{
    return Vec2::ZERO;
    
    log("TBD");     //后面再来写
    
    int x = 0;
    
    if((positinX<0))
    {
        x = 1;
        
    }else if(positinX > VISIBLE_RECT.getMaxX()){
        x = 2;
    }
    
//    CCLOG("VISIBLE_RECT.getMaxY()=%f", VISIBLE_RECT.getMaxY());
    int y = 0;
    if(positionY < 100){
        y = 1;
    }else if(positionY > VISIBLE_RECT.getMaxY()){
        y = 2;
    }
    
    if(positionY > VISIBLE_RECT.getMaxY()*0.93f && positinX > VISIBLE_RECT.getMaxX()*0.93){
        return Vec2(-moveX,-moveY);
    }
    
    if(x !=0 || y!=0){
        if(x == 1){
            if(y == 1)
                return Vec2(moveX,2*moveY);
            else if( y ==2)
                return Vec2(moveX,-moveY);
            else
                return Vec2(moveX,0);
        }else if(x == 2){
            if(y == 1)
                return Vec2(-moveX,2*moveY);
            else if( y ==2){
                return Vec2(-moveX,-moveY);
            }
            else{
                return Vec2(-moveX,0);
            }
            
        }else if( y == 1){
            return Vec2(0,2*moveY);
        }else if(y == 2){
            
            
            return Vec2(0,-moveY);
        }
        
    }
    return Vec2(0, 0);
}


