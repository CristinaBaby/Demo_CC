//
//  BasePenWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "BasePenWidget.h"

USING_NS_CC;

#define RES_SELECT_BG                "ui03_color_a.png"

bool BasePenWidget::init()
{
    if (!LayerColor::initWithColor(Color4B(255,0,0,0))) {
        return false;
    }
    
    bool rcode = false;
    do {
        //select bg
        _selectBg = Sprite::create(RES_SELECT_BG);
        setContentSize(_selectBg->getContentSize());
        addChild(_selectBg, 2);
        
        _selectBg->setVisible(false);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


