//
//  MakeConfig.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-4.
//
//

#ifndef __MakeSnowMan__MakeConfig__
#define __MakeSnowMan__MakeConfig__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;


class MakeConfig
{
public:
    static MakeConfig* getInstance();
    // type
    
    void loadTypeSelConfig(const char* path);
    void loadTypeAtIdx(ssize_t idx);
    __String*  setTypeName(int idx);

    CC_SYNTHESIZE(int, typeCount_, TypeCount);
    CC_SYNTHESIZE(int, typeIdx_, TpyeIdx);
    CC_SYNTHESIZE(__Array*, typeArray_, TypeArray);
    CC_SYNTHESIZE(__Dictionary*, typeDicAtIdx_, TypeDicAtIdx);
    CC_SYNTHESIZE(__String*, typeIconName_, TypeIconName);
    
    // unit
    void loadUnitSelConfig(const char* path);
    void initUnit(int eTypeID);
    __Array* getUnitKeyArray();
    __Dictionary* getUnitDic();
   
    CC_SYNTHESIZE(__Array*, unitArrayAtIdx_, UnitArrayAtIdx);
    CC_SYNTHESIZE(__String*, typeName_, TypeName);
    void setUnit(ssize_t idx);
    CC_SYNTHESIZE(int, hue_, Hue);
    CC_SYNTHESIZE(int, saturation_, Saturation);
    CC_SYNTHESIZE(int, value, Value);
    CC_SYNTHESIZE(__String*, unitIconName_, UnitIconName);
    CC_SYNTHESIZE(__String*, unitName_, UnitName);
    CC_SYNTHESIZE(int, unitCount_, UnitCount);
    CC_SYNTHESIZE(bool, isFree_, IsFree);


private:
    // type
    
    //unit
    __Array *unitArray_;
    __Dictionary* unitDic_;
//    __String *unitIconName_;
    __String *currentTypeName_;



};
#endif /* defined(__MakeSnowMan__MakeConfig__) */
