//
//  PhotoPage.cpp
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#include "PhotoPage.h"

USING_NS_CC;
using namespace ui;

#define RES_FRAME "ui06_photobox1.png"
#define OFFSET_Y 110

PhotoPage::PhotoPage() : _image(nullptr)
{
}

bool PhotoPage::init()
{
    if (!BasePage::init())
    {
        return false;
    }
    
    bool rcode = false;
    
    do {
 
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Action methods

void PhotoPage::touchEnd(cocos2d::Touch *touch)
{
 
}

void PhotoPage::setImage(const std::string& fileName)
{
    if (!_image)
    {
        _image = Sprite::create();
        addChild(_image);
        
        _frame = extension::Scale9Sprite::create(RES_FRAME, Rect(0, 0, 135, 139), Rect(25, 25,135 - 50, 139 - 50));
        _frame->setPreferredSize(getContentSize() * PHOTO_DISPLAY_SCALE);
        _frame->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + OFFSET_Y));
        addChild(_frame);
    }

    _image->setTexture(fileName);
    _image->setScale(PHOTO_DISPLAY_SCALE - 0.03);
    _image->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + OFFSET_Y));
}

