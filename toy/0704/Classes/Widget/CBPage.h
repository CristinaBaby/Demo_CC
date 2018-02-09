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
#include "cocos-ext.h"
#include "../Model/CBAppGlobal.h"


class Page : public cocos2d::extension::TableViewCell
{
public:
    virtual void touchEnd(cocos2d::Touch *touch) = 0;
};



#endif /* defined(__ColorBook__CBPage__) */
