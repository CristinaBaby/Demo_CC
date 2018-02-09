//
//  MyMakeConfig.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-4.
//
//

#ifndef __MakeSnowMan__MyMakeConfig__
#define __MakeSnowMan__MyMakeConfig__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;


class MyMakeConfig
{
public:
    static MyMakeConfig* getInstance();
    
    // type
    void loadTypeSelConfig(const char* path);
    void loadTypeAtIdx(ssize_t idx);
//    void setTypeName(int idx);

    void setUnitConfig();
    Vector<__String*> vec_UnitName;
    Vector<__String*> vec_UnitIconName;
    int getUnitCount();
    __String* getUnitIconName(int idx,bool picFormatPng);
    
    int getH(int idx);
    int getS(int idx);
    int getV(int idx);
    
    void setTypeName(std::string str);
    void setTypeNameByIdx(int idx);

    
    
    std::string typeName_;
    
    
    //从X开始锁住iap
    int iapLockedFrom();
    
    //icon图片大小，出图不规则时使用
    Size getUnitIconSize();

    
private:
    __String* typeIconName_;
    __Array* typeArray_;
    Vector<__String*> vec_TypeIconName;
    int typeCount_;
    
    __String* unitName_;
    __String* unitIconName_;
    
    __String*  getUnitIconPath();
    __String*  getUnitPath();
    //0.png  1.jpg
    __String*  getUnitIconFullPath(int idx,bool picFormatPng);
    __String*  getUnitFullPath(int idx,bool picFormatPng);
    
    //设置unitcount
    void getAllUnitPath(int count);
    void getAllUnitIconPath(int count);
    
};
#endif /* defined(__MakeSnowMan__MyMakeConfig__) */
