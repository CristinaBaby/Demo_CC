#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

USING_NS_CC;

#define stfontsize(size)    (IS_2048) ? (float)(size) * 2 : size

//#define ccp2(X,Y)(IS_2048 ? (Vec2((float)(X)*2, (float)(Y)*2)):(Vec2((float)(X), (float)(Y))))

#define CCSizeMake2(width, height)(IS_2048 ? Size((float)(width)*2, (float)(height)*2):Size((float)(width), (float)(height)))

/** Helper macro that creates a Rect with Vec2 and Size
 *  @return Rect
 */
#define st_ccrps(point,size)  CCRectMake(((Vec2)(point)).x,((Vec2)(point)).y, ((Size)(size)).width,((Size)(size)).height)
/**
 *  Helper macro that find a sprite's center point
 */
#define st_CenterPoint(size) Vec2(((Size)(size)).width / 2, ((Size)(size)).height / 2)
/**
 *  Helper macro that divided sprite's position by half.
 */
#define st_divided_by_half(X, Y) ccp2(X / 2, Y / 2)
/**
 *  Helper macro that divided sprite's position by half.
 */
#define st_divided_by_half_size(width, height) CCSizeMake2(width / 2, height / 2)
/**
 *
 */
//#define ccp_horizontal_center(Y) (IS_2048 ? (Vec2(1024, (float)(Y)*2)) : (Vec2(512, (float)(Y))))

/**
 *  help to user UserDefault
 */
//#define STSaveStringToUD        STUserDefault::sharedUserDefault()->setStringForKey
//#define STSaveIntegerToUD       STUserDefault::sharedUserDefault()->setIntegerForKey
//#define STSaveBooleanToUD       STUserDefault::sharedUserDefault()->setBoolForKey
//#define STSaveFloatToUD         STUserDefault::sharedUserDefault()->setFloatForKey
//#define STLoadStringFromUD      STUserDefault::sharedUserDefault()->getStringForKey
//#define STLoadIntegerFromUD     STUserDefault::sharedUserDefault()->getIntegerForKey
//#define STLoadBooleanFromUD     STUserDefault::sharedUserDefault()->getBoolForKey
//#define STLoadFloatFromUD       STUserDefault::sharedUserDefault()->getFloatForKey


enum STDesignResolution
{
    ST_DESIGN_RESOLUTION_480X320,
    ST_DESIGN_RESOLUTION_960X640,
    ST_DESIGN_RESOLUTION_1024X768,
    ST_DESIGN_RESOLUTION_2048X1536,
};

class Util
{
public:
	/**
     *
     */
	static bool isExistFile(const char *pszFileName);
    
    /**
     *
     */
	static Sprite* getUISprite(const char* fileName);
    
	/**
     *
     */
	static const char* getLanguageType();
    
    /**
     *
     */
	static void loadUITexture(const char* file, const char* language = getLanguageType());
    
    /**
     *
     */
    static void removeUITexture(const char* file, const char* language = getLanguageType());
    
    /**
     * @brief adjust array elments' sequence
     */
    static void stArrayShuffle(CCArray* src);
    
    /**
     * @brief get random number between [0, limit)
     */
    static unsigned stGetRandom(unsigned limit);
    
    /**
     * @return true is odd number ,other is false
     */
    inline static bool isOddNumber(int n)
    {
        return (n & 1) == 1;
    }
    
    static bool parseBoolean(const std::string &value);
    
    static std::string convertInt(int number);

    static std::string convertFloat(float number);
	/*
	//获取当前时间
	static tm* getTime();
	
	//保存这次运行日期
	static void saveTime2UD(int year,int month, int day, int hour);
	*/

	static long getRateUsTimestamp(const std::string &key);
    static void setRateUsTimestamp(const std::string &key, int value);
	static long getCurrentTimeSeconds();
    static long getHoursByTimeInterval(long before, long now);

    static std::string formatTime(long interval);
public:
    static STDesignResolution _STDesignResolution;
    
    static std::string getMapFilePath(int number);
    
    static Vec2 checkBorder(float positinX, float positionY , int moveX , int moveY);
    
    static std::string getParkShowName(std::string name);
    
//    static Sprite* getSpriteFromWriteablePath(const char* name);
};

#endif