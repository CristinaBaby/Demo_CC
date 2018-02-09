//
//  MyMakeConfig.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-4.
//
//

#include "MyMakeConfig.h"
#include "GameMaster.h"
#define kTypeName "TypeName"
#define kTypeIconName "IconImageName"
#define rootPath "decoration"


int h[15] = {-129,-154,160,-72,-139,15, 180,-29,166,89,50,-50,0,125,120};
int s[15] = {-7,  -27, 25, -16,27, 18,  25, -20,22,-19,0,0,-25,0};
int v[15] = {33,  -7,  2,  1,  -8,  2,  34, 0, 38, 17,0,0,0,0,0};




static MyMakeConfig* instance= NULL ;

MyMakeConfig* MyMakeConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new MyMakeConfig();
    }
    return instance;
}


#pragma mark TypeSelConfig ______

void MyMakeConfig::loadTypeSelConfig(const char* path)
{
    typeArray_ = __Array::create();
    auto dic = Dictionary::createWithContentsOfFile(path);
    typeArray_ = dynamic_cast<__Array *>(dic -> objectForKey("ALLTypes"));
    typeArray_ -> retain();
    
    //获取tableview长度
    typeCount_ = (int)typeArray_ -> count();
}

void MyMakeConfig::loadTypeAtIdx(ssize_t idx)
{
    auto typeDicAtIdx = dynamic_cast<__Dictionary *>(typeArray_ -> getObjectAtIndex(idx));
    typeIconName_ = dynamic_cast<__String *>(typeDicAtIdx -> objectForKey("IconImageName"));
}

void MyMakeConfig::setTypeName(std::string str)
{
    typeName_ = str;
    this -> setUnitConfig();
}


#pragma mark UnitConfig______

void MyMakeConfig::setUnitConfig()
{
    this -> getAllUnitIconPath(getUnitCount());
    this -> getAllUnitPath(getUnitCount());
}

__String*  MyMakeConfig::getUnitIconPath()
{
    return  __String::createWithFormat("%s/icon_%s/",rootPath,typeName_.c_str());;
}

__String*  MyMakeConfig::getUnitPath()
{
    return  __String::createWithFormat("%s/%s/",rootPath,typeName_.c_str());;
}

__String*  MyMakeConfig::getUnitIconFullPath(int idx,bool picFormatPng)
{
    const char* str;
    str = picFormatPng ? "png" : "png";
    return  __String::createWithFormat("%s/icon_%s/icon_%s%d.%s",rootPath,typeName_.c_str(),typeName_.c_str(),idx,str);;
}

__String*  MyMakeConfig::getUnitIconName(int idx,bool picFormatPng)
{
    const char* str;
    str = picFormatPng ? "png" : "png";
    return  __String::createWithFormat("icon_%s%d.%s",typeName_.c_str(),idx,str);
}


__String* MyMakeConfig:: getUnitFullPath(int idx,bool picFormatPng)
{
    const char* str;
    str = picFormatPng ? "png" : "png";

    return  __String::createWithFormat("%s/%s/%s%d.%s",rootPath,typeName_.c_str(),typeName_.c_str(),idx,str);;
}

void MyMakeConfig::getAllUnitPath(int count)
{
    //前一组全部清除
    vec_UnitName.clear();

    bool picFormat = strcmp("background", typeName_.c_str());
    
    for (int i=0; i<=count; i++)
    {
        if (FileUtils::getInstance() -> isFileExist(getUnitFullPath(i,picFormat) -> getCString()))
        {
            vec_UnitName.pushBack(getUnitFullPath(i,picFormat));
        }
        else
        {
            log("UnitNotExist    %s   " , getUnitFullPath(i,picFormat)-> getCString() );
        }
    }
}

void MyMakeConfig::getAllUnitIconPath(int count)
{
    
    vec_UnitIconName.clear();
    
    bool picFormat = true;
    
    for (int i=0; i<=count; i++)
    {
        if (FileUtils::getInstance() -> isFileExist(getUnitIconFullPath(i,picFormat) -> getCString()))
        {
            vec_UnitIconName.pushBack(getUnitIconFullPath(i,picFormat));
        }
        else
        {
            log("UnitIconNotExist   %s" , getUnitIconFullPath(i,picFormat)-> getCString() );
        }
            
    }
}

void MyMakeConfig::setTypeNameByIdx(int idx)
{
    std::string str;
    
    switch (idx)
    {
        case 0:
            str = "candy";
            break;
        case 1:
            str = "syrup";
            break;
        case 2:
            str = "fruit";
            break;
        case 3:
            str = "cups";
            break;
        case 4:
            str = "spoons";
            break;
        case 5:
            str = "extras";
            break;
        case 6:
            str = "background";
            break;

        default:
            break;
    }
    
    this -> setTypeName(str);
}



int MyMakeConfig::getUnitCount()
{
    auto str = typeName_.c_str();
    if (!strcmp(str, "background"))
        return 32;
    else if (!strcmp(str, "candy"))
        return 51;
    else if (!strcmp(str, "syrup"))
        return 10;
    else if (!strcmp(str, "fruit"))
        return 47;
    else if (!strcmp(str, "cups"))
        return 60;
    else if (!strcmp(str, "spoons"))
        return 55;
    else if (!strcmp(str, "extras"))
        return 48;
    else if (!strcmp(str, "shape"))
        return 8;
    else
        return 15;
    
}

int MyMakeConfig::iapLockedFrom()
{
    auto str = typeName_.c_str();
    if (!strcmp(str, "background"))
        return 4;
    else if (!strcmp(str, "candy"))
        return 4;
    else if (!strcmp(str, "syrup"))
        return 4;
    else if (!strcmp(str, "fruit"))
        return 4;
    else if (!strcmp(str, "cups"))
        return 32;
    else if (!strcmp(str, "spoons"))
        return 42;
    else if (!strcmp(str, "extras"))
        return 6;
    else
        return 5;
}


Size MyMakeConfig::getUnitIconSize()
{
    auto str = typeName_.c_str();
    if (!strcmp(str, "cups"))
        return Size(160, 230);
    if (!strcmp(str, "shape"))
        return Size(234, 230);
//    else if (!strcmp(str, "juice"))
//        return Size(213, 185);
//    else if (!strcmp(str, "vegetable"))
//        return Size(213, 185);
//    else if (!strcmp(str, "fruit"))
//        return Size(213, 185);
    else
        return Size(110, 110);

}


int MyMakeConfig::getH(int idx)
{
    return h[idx];
}
int MyMakeConfig::getS(int idx)
{
    return s[idx];
}
int MyMakeConfig::getV(int idx)
{
    return v[idx];
}



