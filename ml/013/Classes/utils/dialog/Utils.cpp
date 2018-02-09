#include "Utils.h"
#define M_PI_VALUE 3.14159f
float Utils::getAngle(Point beginPoint,Point endPoint){
    float len_y = endPoint.y - beginPoint.y;
    float len_x = endPoint.x - beginPoint.x;
    
    
    if(len_y == 0.0f)
    {
        if(len_x < 0)
        {
            return 270;
        }
        else if(len_x > 0)
        {
            return 90;
        }
        return 0;
    }
    
    if(len_x == 0.0f)
    {
        if(len_y >= 0)
        {
            return 0;
        }
        else if(len_y < 0)
        {
            return 180;
        }
    }
    
    
    return atan2f(len_x,len_y)*180/M_PI_VALUE;
}

Point Utils::createDirection(){
	Point change=ccpNormalize(ccp(rand()%200-100,rand()%200-100));
	return ccpMult(change,0.2);
    
}

//圆形碰撞函数
bool Utils::isCollision(Point p1,float r1,Point p2,float r2)
{
    //用三角型定理来计算圆心距,然后与半径和对比
    if(sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2))>r1+r2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Utils::isExistFile(const char *pszFileName)
{
	FILE *fp = fopen(pszFileName, "rb");
    if(fp == NULL)
        return false;
    fclose(fp);
    return true;
}

const char* Utils::getLanguageType()
{
	ccLanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();
    //	char *path;
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
            return "en";
            
    }
}

Sprite* Utils::getUISprite(const char* fileName)
{
	Sprite* sprite=NULL;
	
	sprite=Sprite::create(getUIFilePath(fileName).c_str());
    
	return sprite;
}



Sprite* Utils::getContentSprite( const char* fileName )
{
	Sprite* sprite=NULL;
	
    
	sprite=CCSprite::create(getContentFilePath(fileName).c_str());
    
	return sprite;
}
Texture2D* Utils::getContentTexture( const char* fileName )
{
	return CCTextureCache::sharedTextureCache()->addImage(getContentFilePath(fileName).c_str());
}
Texture2D* Utils::getUITexture( const char* fileName )
{
	return TextureCache::sharedTextureCache()->addImage(getUIFilePath(fileName).c_str());
}
std::string Utils::getContentFilePath( const char* fileName )
{
	const char* language=getLanguageType();
	string path="Content";
	string path_resolution="Content";
	string path_lang="Content";
	if(resourceType==SF_RESOURCES_2048_1536)
	{
		path.append("-HD");
		path_resolution.append("-HD");
        
        
	}
    
	if(language!=0)
	{
		path.append("-");
		path.append(language);
		path_lang.append("-");
		path_lang.append(language);
	}
	path.append("/");
	path.append(fileName);
	path_lang.append("/");
	path_lang.append(fileName);
	path_resolution.append("/");
	path_resolution.append(fileName);
    
	if(!FileUtils::sharedFileUtils()->isFileExist(path.c_str()))
	{
        
        bool lRes = FileUtils::sharedFileUtils()->isFileExist(path_resolution);
		if(!lRes)
		{
            
			if(!FileUtils::getInstance()->isFileExist(path_lang.c_str()))
			{
				path="Content/";
				path.append(fileName);
			}else
			{
				path=path_lang;
			}
		}else
		{
			path=path_resolution;
		}
	}
    
	return path;
}
std::string Utils::getUIFilePath( const char* fileName )
{
	const char* language=getLanguageType();
	string path="UI";
	string path_resolution="UI";
	string path_lang="UI";
	if(resourceType==SF_RESOURCES_2048_1536)
	{
		path.append("-HD");
		path_resolution.append("-HD");
        
        
	}
    
	if(language!=0)
	{
		path.append("-");
		path.append(language);
		path_lang.append("-");
		path_lang.append(language);
	}
	path.append("/");
	path.append(fileName);
	path_lang.append("/");
	path_lang.append(fileName);
	path_resolution.append("/");
	path_resolution.append(fileName);
    
	if(!FileUtils::getInstance()->isFileExist(path.c_str()))
	{
        
        bool lRes = FileUtils::getInstance()->isFileExist(path_resolution);
		if(!lRes)
		{
            
			if(!FileUtils::getInstance()->isFileExist(path_lang.c_str()))
			{
				path="";
				path.append(fileName);
			}else
			{
				path=path_lang;
			}
		}else
		{
			path=path_resolution;
		}
	}
    
	return path;
}

long Utils::getCurrentTime()     
{      
	struct timeval tv;      
	gettimeofday(&tv,NULL);      
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;      
}

Rect Utils::convertToWorldRect(Node* node){
    Rect rect;
    
    Point worldPosi=node->getParent()->convertToWorldSpace(node->getBoundingBox().origin );
    rect=Rect(worldPosi.x,worldPosi.y,node->getBoundingBox().size.width,node->getBoundingBox().size.height );
    return rect;
}

bool Utils::isPurchsed(string iapKey){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return true;
#endif
    
    int count = UserDefault::getInstance()->getIntegerForKey(iapKey.c_str(), 0);
    if(count>0)
        return true;
    else
        return false;
}
void Utils::saveIAPData(string iapKey){
    UserDefault::getInstance()->setIntegerForKey(iapKey.c_str(), 1);
    
    int allCount=UserDefault::getInstance()->getIntegerForKey(IAP_ALL.c_str(), 0);
    
    if(iapKey==IAP_ALL){
        if(allCount>0)
        {
            UserDefault::getInstance()->setIntegerForKey(IAP_CLOTHES.c_str(), 1);
            UserDefault::getInstance()->setIntegerForKey(IAP_VEHICLES.c_str(), 1);
            closeAds=true;
        }
    }else{

        int clothesCount=UserDefault::getInstance()->getIntegerForKey(IAP_CLOTHES.c_str(), 0);
        int VehiclesCount=UserDefault::getInstance()->getIntegerForKey(IAP_VEHICLES.c_str(), 0);
        if(clothesCount>0&&VehiclesCount>0)
        {
            UserDefault::getInstance()->setIntegerForKey(IAP_ALL.c_str(), 1);
            closeAds=true;
        }
    }
    
}