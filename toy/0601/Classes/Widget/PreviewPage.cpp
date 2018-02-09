//
//  PreviewPage.cpp
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#include "PreviewPage.h"
#include "../Model/STVisibleRect.h"
#include "../Model/AppConfigs.h"

USING_NS_CC;
using namespace ui;

bool PreviewPage::init()
{
    if (!TableViewCell::init())
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

void PreviewPage::touchEnd(cocos2d::Touch *touch)
{
    
}

void PreviewPage::setBackground(const std::string& fileName)
{
    if (!_background)
    {
        _background = Sprite::create();
        _background->setTexture(fileName);
        _background->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        _background->setTextureRect(Rect(namespaceST::STVisibleRect::getDesignOffset().x, namespaceST::STVisibleRect::getDesignOffset().y, getContentSize().width, getContentSize().height));
        addChild(_background);
    }
    else
    {
        Director::getInstance()->getTextureCache()->removeTextureForKey(_fileName);
        _background->setTexture(fileName);
        _background->setTextureRect(Rect(namespaceST::STVisibleRect::getDesignOffset().x, namespaceST::STVisibleRect::getDesignOffset().y, getContentSize().width, getContentSize().height));
        
        _fileName = fileName;
    }
}

