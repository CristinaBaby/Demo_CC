//
//  BaseIconWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "BaseIconWidget.h"

USING_NS_CC;

#define RES_SCROLL_ICON_BG           "ui02_dress_box.png"
#define RES_SCROLL_ICON_BG_SELECT    "ui02_dress_box_a.png"
#define RES_NEW_LOGO                 "ui02_new.png"

bool BaseIconWidget::init(Res* res)
{
    if (!LayerColor::initWithColor(Color4B(255,0,0,0))) {
        return false;
    }
    
    bool rcode = false;
    do {
        //store res
        _res = res;
        
        //icon bg
        _iconBg = Sprite::create(RES_SCROLL_ICON_BG);
        setContentSize(_iconBg->getContentSize());
        _iconBg->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        addChild(_iconBg);
        
        //new logo
        _newLogo = Sprite::create(RES_NEW_LOGO);
        _newLogo->setPosition(Vec2(107, 111));
        _newLogo->setVisible(false);
        addChild(_newLogo, 1);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void BaseIconWidget::changeToSelectStyle()
{
    _iconBg->setTexture(RES_SCROLL_ICON_BG_SELECT);
}

void BaseIconWidget::changeToUnSelectStyle()
{
    _iconBg->setTexture(RES_SCROLL_ICON_BG);
}

