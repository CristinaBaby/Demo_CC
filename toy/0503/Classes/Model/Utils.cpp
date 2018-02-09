//
//  Utils.cpp
//  BedtimeStory
//
//  Created by maxiang on 9/15/15.
//
//

#include "Utils.h"

Button* Utils::addEventToButton(Widget *ui,
                                const string& buttonname,
                                Widget::TouchEventType type,
                                const std::function<void()>& callback)
{
    auto button = dynamic_cast<Button*>(Helper::seekWidgetByName(ui, buttonname));
    if (!button) {
        return nullptr;
    }
    
    button->addTouchEventListener([=](Ref* ref, Widget::TouchEventType eventType){
        
        if (eventType == type)
        {
            callback();
        }
    });
    
    return button;
}

bool Utils::hitTest(Node *node, const Vec2& point)
{
    Rect boudingbox;
    boudingbox.size = node->getContentSize();
    if (boudingbox.containsPoint(point))
    {
        return true;
    }
    return false;
}

LayerColor* Utils::createMaskLayer(const float alpha)
{
    auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, alpha));
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    
    return layer;
}

Sprite* Utils::createSpriteWithNoCache(const std::string& file)
{
    auto image = new Image();
    if (!image || !image->initWithImageFile(file))
    {
        delete image;
        return nullptr;
    }
    
    auto texture = new Texture2D();
    if (!texture || !texture->initWithImage(image))
    {
        delete texture;
        delete image;
        return nullptr;
    }
    
    auto sprite = Sprite::createWithTexture(texture);
    if (!sprite)
    {
        delete texture;
        delete image;
        return nullptr;
    }
    else
    {
        CC_SAFE_RELEASE(image);
        CC_SAFE_RELEASE(texture);
        
        return sprite;
    }
}


