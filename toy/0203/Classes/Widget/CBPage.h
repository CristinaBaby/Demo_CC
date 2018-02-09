//
//  CBPage.h
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#ifndef __ColorBook__CBPage__
#define __ColorBook__CBPage__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class Page : public cocos2d::extension::TableViewCell
{
public:
    virtual void touchEnd(cocos2d::Touch *touch) = 0;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPage__) */
