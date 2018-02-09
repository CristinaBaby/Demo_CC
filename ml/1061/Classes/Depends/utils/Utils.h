#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Utils{
    
public:
	//check the file exists
	static bool isExistFile(const char *pszFileName);
    //获取一个UI sprite，此方法只会到resource的UI目录下寻找资源
	static Sprite* getUISprite(const char* fileName);
    static Texture2D* getUITexture(const char* fileName);
    
     //获取一个content sprite，此方法只会到resource的Content目录下寻找资源
	static Sprite* getContentSprite(const char* fileName);
	static Texture2D* getContentTexture(const char* fileName);
    
    //获取一个UI menuItmesprite，此方法只会到resource的UI目录下寻找资源
	static MenuItemSprite* getMenuItemSprite(const char* normalName,const char* selectedName,Node* parent,Ref* target,SEL_MenuHandler sel);
    static MenuItemSprite* getMenuItemSpriteWithFrameName(const char* normalName,const char* selectedName,Node* parent,Ref* target,SEL_MenuHandler sel);
    static string getContentFilePath(const char* fileName);
    
    //获取当前系统时间
    static long getCurrentTime();

//    //判断当前ComponentInfo是否有IAP，如果有IAP并且已经被购买，将返回false
//	static bool isIAP(ComponentInfo*);
//	
//    //判断当前Category是否有IAP，如果有IAP并且已经被购买，将返回false
//    static bool isIAP(Category*);
	//替换字符串中的内容
	static string replaceStr(std::string &str, const char *string_to_replace, const char *new_string);
};

#endif