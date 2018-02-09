#include "ui/UILayoutParameter.h"
#include "LayoutTools.h"


Size quickWinSize()
{
    Size size = Director::getInstance()->getWinSize();
    return std::move(size);
}

float quickWinWidth()
{
    Size size = Director::getInstance()->getWinSize();
    return size.width;
}

float quickWinHeight()
{
    Size size = Director::getInstance()->getWinSize();
    return size.height;
}

EventListenerCustom * quickAddAfterVisit(Node * scene, const std::function<void(EventCustom*)>& callback)
{
    auto listener = EventListenerCustom::create("director_after_visit", callback);
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scene);
    return listener;
}

RelativeLayoutParameter * quickLayoutTop(float top)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    parameter->setMargin(Margin(0, top, 0, 0));
    return parameter;
}

RelativeLayoutParameter * quickLayoutBottom(float bottom)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    parameter->setMargin(Margin(0, 0, 0, bottom));
    return parameter;
}

RelativeLayoutParameter * quickLayoutLeft(float left)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    parameter->setMargin(Margin(left, 0, 0, 0));
    return parameter;
}

RelativeLayoutParameter * quickLayoutRight(float right)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    parameter->setMargin(Margin(0, 0, right, 0));
    return parameter;
}

RelativeLayoutParameter * quickLayoutTopLeft(float top, float left)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    parameter->setMargin(Margin(left, top, 0, 0));
    return parameter;
}

RelativeLayoutParameter * quickLayoutTopRight(float top, float right)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    parameter->setMargin(Margin(0, top, right, 0));
    return parameter;
}

RelativeLayoutParameter * quickLayoutBottomLeft(float bottom, float left)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    parameter->setMargin(Margin(left, 0, 0, bottom));
    return parameter;
}

RelativeLayoutParameter * quickLayoutBottomRight(float bottom, float right)
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    parameter->setMargin(Margin(0, 0, right, bottom));
    return parameter;
}

RelativeLayoutParameter * quickLayoutCenter()
{
    RelativeLayoutParameter* parameter = RelativeLayoutParameter::create();
    parameter->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    return parameter;
}

