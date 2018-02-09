#pragma once

#include "cocos2d.h"

namespace cocos2d {
    namespace ui {
        class RelativeLayoutParameter;
        class Margin;
    }
}

using namespace cocos2d;
using ui::RelativeLayoutParameter;
using ui::Margin;

Size quickWinSize();
float quickWinWidth();
float quickWinHeight();

EventListenerCustom * quickAddAfterVisit(Node * scene, const std::function<void(EventCustom*)>& callback);

RelativeLayoutParameter * quickLayoutTop(float top);
RelativeLayoutParameter * quickLayoutBottom(float bottom);
RelativeLayoutParameter * quickLayoutLeft(float left);
RelativeLayoutParameter * quickLayoutRight(float right);
RelativeLayoutParameter * quickLayoutTopLeft(float top, float left);
RelativeLayoutParameter * quickLayoutTopRight(float top, float right);
RelativeLayoutParameter * quickLayoutBottomLeft(float bottom, float left);
RelativeLayoutParameter * quickLayoutBottomRight(float bottom, float right);
RelativeLayoutParameter * quickLayoutCenter();