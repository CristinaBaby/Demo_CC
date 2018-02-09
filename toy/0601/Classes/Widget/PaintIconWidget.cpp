//
//  PaintIconWidget.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "PaintIconWidget.h"
#include "../Model/AppManager.h"

USING_NS_CC;
using namespace ui;

#define RES_PRICE_IMAGE              "ui02_btn_price.png"
#define RES_BUTTON_PRICE             "ui02_btn_price.png"
#define RES_BUTTON_BUY               "ui02_btn_buy.png"

#define FONT_NAME                    "Helvetica_Rounded_LT_Black.ttf"
#define FONT_SIZE                    17

PaintIconWidget* PaintIconWidget::create(Res* res)
{
    PaintIconWidget *container = new PaintIconWidget;
    if (container && container->init(res)) {
        container->autorelease();
        return container;
    }
    CC_SAFE_DELETE(container);
    return nullptr;
}

bool PaintIconWidget::init(Res* res)
{
    if (!BaseIconWidget::init(res)) {
        return false;
    }
    
    bool rcode = false;
    do {
        //icon button
        auto iconButton = Sprite::create(res->resIconPath);
        iconButton->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        
        addChild(iconButton);
        
        //price button
        _priceButton = Sprite::create(RES_BUTTON_PRICE);
        _priceButton->setPosition(Vec2(getContentSize().width/2, 30));
        
        addChild(_priceButton, 1);
        
        //price label
        _priceLabel = Label::createWithTTF("999", FONT_NAME, FONT_SIZE);
        _priceLabel->setPosition(Vec2(_priceButton->getContentSize().width/2 + 10, _priceButton->getContentSize().height/2 + 2));
        _priceLabel->setColor(Color3B(14, 164, 0));
        _priceButton->addChild(_priceLabel);
        
        std::string key = UserDefaultKey_HasPurchasedItem + res->resName;
        bool hasPurchased = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
        if (_res->price > 0 && !hasPurchased)
        {
            char priceString[10];
            sprintf(priceString, "%d", _res->price);
            _priceLabel->setString(priceString);
        }
        else
        {
            _priceButton->setVisible(false);
        }
        
        rcode = true;
    } while (0);
    
    
    return rcode;
}

void PaintIconWidget::changeToUnSelectStyle()
{
    BaseIconWidget::changeToUnSelectStyle();
    
    if (_isBuyStyle) {
        changeToPriceStyle();
    }
}

void PaintIconWidget::changeToBuyStyle()
{
    _isBuyStyle = true;
    _priceButton->setTexture(RES_BUTTON_BUY);
    _priceLabel->setVisible(false);
}

void PaintIconWidget::changeToPriceStyle()
{
    _isBuyStyle = false;
    _priceButton->setTexture(RES_BUTTON_PRICE);
    _priceLabel->setVisible(true);
}

