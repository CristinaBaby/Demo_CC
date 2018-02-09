//
//  ColorWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "ColorWidget.h"
#include "../Model/AppManager.h"

USING_NS_CC;

ColorWidget* ColorWidget::create(const Pen& pen)
{
    ColorWidget *color = new ColorWidget;
    if (color && color->init(pen)) {
        color->autorelease();
        return color;
    }
    CC_SAFE_DELETE(color);
    return nullptr;
}

bool ColorWidget::init(const Pen& pen)
{
    if (!BasePenWidget::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //store pen
        _pen = pen;

        //color logo
        auto logo = Sprite::create(_pen.penLogo);
        addChild(logo, 1);

        rcode = true;
    } while (0);
    
    return rcode;
}



