//
//  PatternWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "PatternWidget.h"
#include "../Model/AppManager.h"

USING_NS_CC;

PatternWidget* PatternWidget::create(const Pattern& pattern)
{
    PatternWidget *color = new PatternWidget;
    if (color && color->init(pattern)) {
        color->autorelease();
        return color;
    }
    CC_SAFE_DELETE(color);
    return nullptr;
}

bool PatternWidget::init(const Pattern& pattern)
{
    if (!BasePenWidget::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //store pen
        _pattern = pattern;
        
        //color logo
        auto logo = Sprite::create(_pattern.patternLogo);
        addChild(logo, 1);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


