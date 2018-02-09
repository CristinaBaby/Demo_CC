#include "Utils.h"
#include <string>


using namespace std;
bool Utils::isExistFile(const char *pszFileName)
{
	FILE *fp = fopen(pszFileName, "rb");
    if(fp == NULL)
        return false;
    fclose(fp);
    return true;
}

Sprite* Utils::getUISprite(const char* fileName)
{
	Sprite* sprite=NULL;
	
	sprite=Sprite::create(fileName);

	return sprite;
}

MenuItemSprite *Utils::getMenuItemSprite(const char *normalName, const char *selectedName,Node* parent,Ref* target,SEL_MenuHandler sel)
{
    Sprite* normalSprite = getUISprite(normalName);
    Sprite* selectedSprite = getUISprite(selectedName);
    MenuItemSprite* menueItemSprite = MenuItemSprite::create(normalSprite, selectedSprite, target, sel);
    parent->addChild(menueItemSprite);
    return menueItemSprite;
}

MenuItemSprite* Utils::getMenuItemSpriteWithFrameName(const char *normalName, const char *selectedName, Node *parent, cocos2d::Ref *target, SEL_MenuHandler sel)
{
    Sprite* normalSprite = Sprite::createWithSpriteFrameName(normalName);
    Sprite* selectedSprite = Sprite::createWithSpriteFrameName(selectedName);
    MenuItemSprite* menueItemSprite = MenuItemSprite::create(normalSprite, selectedSprite, target, sel);
    parent->addChild(menueItemSprite);
    return menueItemSprite;
    
}


Sprite* Utils::getContentSprite( const char* fileName )
{
	Sprite* sprite=NULL;
	
 
	sprite=Sprite::create(fileName);

	return sprite;
}
Texture2D* Utils::getContentTexture( const char* fileName )
{
	return TextureCache::getInstance()->addImage(fileName);
}
Texture2D* Utils::getUITexture( const char* fileName )
{
	return TextureCache::getInstance()->addImage(fileName);
}

long Utils::getCurrentTime()     
{      
	struct timeval tv;      
	gettimeofday(&tv,NULL);      
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;      
}    

std::string Utils::replaceStr(std::string &str, const char *string_to_replace, const char *new_string)
{
	int index = str.find(string_to_replace);
	while(index != std::string::npos)
	{
		str.replace(index, strlen(string_to_replace), new_string);
		index = str.find(string_to_replace, index + strlen(new_string));
	}
	return str;
}
