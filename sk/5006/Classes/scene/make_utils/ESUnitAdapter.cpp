//
//  ESUnitAdapter.cpp
//  Make
//
//  Created by liji on 15/9/22.
//
//

#include "ESUnitAdapter.h"


ESUnitAdapter::ESUnitAdapter()
{
    unitTouchEnded = nullptr;
};

int ESUnitAdapter::getUnitCount()
{
    return _endIndex-_startIndex+1;
}

void ESUnitAdapter::registTouchEvent(ui::Button* obj)
{
    obj->addTouchEventListener(CC_CALLBACK_2(ESUnitAdapter::touchEvent, this));
}


ui::Widget* ESUnitAdapter::getUnit(int idx)
{
    ui::Widget* widget= ui::Widget::create();
    widget->setColor(Color3B(255, 255, 255));
    ui::Button* btn = ui::Button::create(StringUtils::format(_pathFormat.c_str(),idx));
    btn->loadTexturePressed(StringUtils::format(_pathFormat.c_str(),idx));
    widget->setContentSize(btn->getContentSize());
    btn->setTag(idx);
    widget->setTag(idx);
    widget->setContentSize(btn->getContentSize());
    btn->setPosition(widget->getContentSize()/2);
    widget->addChild(btn);
    registTouchEvent(btn);

    return widget;
}

void ESUnitAdapter::touchEvent(Ref* obj,ui::Widget::TouchEventType type)
{
    auto btn = dynamic_cast<ui::Button*>(obj);
    
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        log("%d btn",btn->getTag());
        
        auto image = dynamic_cast<ui::Widget*>(btn->getParent());
        
        if (unitTouchEnded)
            unitTouchEnded(image);
    }
}

Size ESUnitAdapter::getItemSize(int idx)
{
    auto sp = Sprite::create(StringUtils::format(_pathFormat.c_str(),idx+1));
    
    Size s = sp->getContentSize();

    return s;

}

