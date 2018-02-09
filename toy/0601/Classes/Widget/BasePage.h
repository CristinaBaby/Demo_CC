//
//  BasePage.h
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#ifndef __PaintDress__BasePage__
#define __PaintDress__BasePage__

#include "cocos2d.h"
#include "../Model/AppConfigs.h"
#include "cocos-ext.h"

class BasePage : public cocos2d::extension::TableViewCell
{
public:
    virtual void touchEnd(cocos2d::Touch *touch) = 0;
};

#endif /* defined(__PaintDress__BasePage__) */
