//
//  MakeConfig.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-4.
//
//

#include "MakeConfig.h"

#define kTypeName "TypeName"
#define kTypeIconName "IconImageName"


static MakeConfig* instance= NULL ;

MakeConfig* MakeConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new MakeConfig();
    }
    return instance;
}


#pragma mark TypeSelConfig ______
void MakeConfig::loadTypeSelConfig(const char* path)
{
    typeArray_ = __Array::create();
    auto dic = Dictionary::createWithContentsOfFile(path);
    typeArray_ = dynamic_cast<__Array *>(dic -> objectForKey("ALLTypes"));
    typeArray_ -> retain();
    
    //获取tableview长度
    typeCount_ = (int)typeArray_ -> count();

}

void MakeConfig::loadTypeAtIdx(ssize_t idx)
{
    typeIdx_ = (int)idx;
    typeDicAtIdx_ = dynamic_cast<__Dictionary *>(typeArray_ -> getObjectAtIndex(idx));
    typeIconName_ = dynamic_cast<__String *>(typeDicAtIdx_ -> objectForKey("IconImageName"));
}

__String*  MakeConfig::setTypeName(int idx)
{
    auto dic = dynamic_cast<__Dictionary *>(typeArray_ -> getObjectAtIndex(idx));
    typeName_ = dynamic_cast<__String *>(dic -> objectForKey("TypeName"));
    return typeName_;
}

#pragma mark UnitSelConfig ______

void MakeConfig::loadUnitSelConfig(const char* path)
{
    unitDic_ = __Dictionary::createWithContentsOfFile(path);
    unitArray_ = unitDic_ -> allKeys();
    unitArray_ ->retain();
    unitDic_ ->retain();
    log("%zd", unitArray_ -> count());
    Ref* obj;
    CCARRAY_FOREACH(unitArray_, obj)
    {
        auto str =  dynamic_cast<__String* >(obj);
        if (!strcmp(str -> getCString(), typeName_->getCString()))
        {
            unitArrayAtIdx_ = dynamic_cast<__Array *>(unitDic_ -> objectForKey(str -> getCString()));
        }
    }
    unitCount_ = (int)unitArrayAtIdx_ -> count();

}

__Array* MakeConfig::getUnitKeyArray()
{
    return unitArray_;
}

__Dictionary* MakeConfig::getUnitDic()
{
    return unitDic_;
}

void MakeConfig::setUnit(ssize_t idx)
{
    
    if (idx > unitArrayAtIdx_ -> count()-1 )
    {
        unitIconName_ = nullptr;
        return;
    }
    
    auto dic = dynamic_cast<__Dictionary*>(unitArrayAtIdx_ ->getObjectAtIndex(idx));
    unitIconName_ = dynamic_cast<__String *>(dic -> objectForKey("IconName"));
    unitName_ = dynamic_cast<__String *>(dic -> objectForKey("ImageName"));
    isFree_ = dynamic_cast<__String *>(dic -> objectForKey("IsFree")) -> boolValue();
}
