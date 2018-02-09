//
//  Utils.cpp
//  Runrunrun
//
//  Created by liuhailong1 on 14-5-8.
//
//

#ifndef __UTILS_H__
#define __UTILS_H__
#include "cocos2d.h"
#include "Global.h"

USING_NS_CC;
using namespace std;
class Utils{
public:
	static float getAngle(cocos2d::Point begin,cocos2d::Point end);
	static cocos2d::Point createDirection();
	static bool isExistFile(const char *pszFileName);
	static Sprite* getUISprite(const char* fileName);
    static Texture2D* getUITexture(const char* fileName);
	static Sprite* getContentSprite(const char* fileName);
	static Texture2D* getContentTexture(const char* fileName);
	static std::string getContentFilePath(const char* fileName);
    static std::string getUIFilePath(const char* fileName);
	static const char* getLanguageType();
    static bool isCollision(cocos2d::Point p1,float r1,cocos2d::Point p2,float r2);
	static long getCurrentTime();
    static cocos2d::Rect convertToWorldRect(Node* node);
    static bool isPurchsed(string iapKey);
    static void saveIAPData(string iapKey);
};
#endif
