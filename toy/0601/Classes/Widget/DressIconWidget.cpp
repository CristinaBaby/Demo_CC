//
//  DressIconWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "DressIconWidget.h"
#include "../Model/AppManager.h"

USING_NS_CC;
using namespace ui;

#define RES_BUTTON_PAINT             "ui04_btn_paint.png"
#define RES_BUTTON_ADD               "ui04_btn_add.png"

DressIconWidget* DressIconWidget::create(Res* res)
{
    DressIconWidget *container = new DressIconWidget;
    if (container && container->init(res)) {
        container->autorelease();
        return container;
    }
    CC_SAFE_DELETE(container);
    return nullptr;
}

bool DressIconWidget::init(Res* res)
{
    if (!BaseIconWidget::init(res)) {
        return false;
    }
    
    bool rcode = false;
    do {
        if (res != nullptr)
        {
            //icon button
            Director::getInstance()->getTextureCache()->removeTextureForKey(_res->resPicturePath);
            Sprite *iconButton = Sprite::create();
            if (FileUtils::getInstance()->isFileExist(_res->resPicturePath)) {
                iconButton->setTexture(_res->resPicturePath);
            }
            iconButton->setScale(0.27);
            float offsetY = 0.0;
            if (_res->type == ResType::ResType_Shoes) {
                offsetY = 40.0;
            }
            iconButton->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + offsetY));
            
            addChild(iconButton);
        }

        //paint button
        _paintButton = Sprite::create(RES_BUTTON_PAINT);
        _paintButton->setPosition(Vec2(getContentSize().width/2, 30));
        addChild(_paintButton, 1);
        
        //add button
        _addButton = Sprite::create(RES_BUTTON_ADD);
        _addButton->setPosition(Vec2(getContentSize().width/2, 30));
        _addButton->setVisible(false);
        addChild(_addButton, 1);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

