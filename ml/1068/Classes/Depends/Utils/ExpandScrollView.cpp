
#include "ExpandScrollView.h"
#include "2d/CCDrawNode.h"

ExpandScrollView::ExpandScrollView()
{
    m_bIsScrolling = false;
    m_bSpeedUp = true;
    onExpandScrollViewCallback = nullptr;
}

ExpandScrollView::~ExpandScrollView()
{
    
}

ExpandScrollView* ExpandScrollView::create()
{
    ExpandScrollView* widget = new ExpandScrollView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool ExpandScrollView::init()
{
    if ( !ScrollView::init() )
    {
        return false;
    }
    return true;
}

bool ExpandScrollView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    m_bIsScrolling = false;
    if (onExpandScrollViewCallback) {
        onExpandScrollViewCallback(this,eBeginScroll);
    }
    poStart = touch->getLocation();
    return ScrollView::onTouchBegan(touch, unusedEvent);
}

void ExpandScrollView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    Vec2 poCur = touch->getLocation();
//    Vec2 offset = Vec2(_touchBeganPosition, _touchMovePosition);
    Vec2 offset = Vec2(poStart, poCur);
    switch (_direction)
    {
        case Direction::VERTICAL:
        {
            if (fabs(offset.y)>15.0f) {
                m_bIsScrolling = true;
            }
            break;
        }
        case Direction::HORIZONTAL:
        {
            if (fabs(offset.x)>15.0f) {
                m_bIsScrolling = true;
            }
            break;
        }
        case Direction::BOTH:
        {
            if (fabs(offset.y)>15.0f || fabs(offset.x)>15.0f) {
                m_bIsScrolling = true;
            }
            break;
        }
        default:
            break;
    }
    if (onExpandScrollViewCallback) {
        onExpandScrollViewCallback(this,eScrolling);
    }
    ScrollView::onTouchMoved(touch, unusedEvent);
}

void ExpandScrollView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    if (onExpandScrollViewCallback) {
        onExpandScrollViewCallback(this,eEndedScroll);
    }
    ScrollView::onTouchEnded(touch, unusedEvent);
    
}

void ExpandScrollView::onTouchCancelled(Touch* touch, Event* event)
{
    if (onExpandScrollViewCallback) {
        onExpandScrollViewCallback(this,eEndedScroll);
    }
    
    ScrollView::onTouchCancelled(touch, event);
}

void ExpandScrollView::handleReleaseLogic(Touch *touch)
{
    m_bIsScrolling = false;
    if (m_bSpeedUp) {
        ui::ScrollView::handleReleaseLogic(touch);
    }
}


//void ExpandScrollView::setStencilClippingSize(const Size& size)
//{
//    if (_clippingEnabled && _clippingType == ClippingType::STENCIL)
//    {
//        Vec2 rect[4];
//        rect[0] = Vec2::ZERO;
//        rect[1] = Vec2(size.width, 0);
//        rect[2] = Vec2(size.width, size.height);
//        rect[3] = Vec2(0, size.height);
//        Color4F green(0, 1, 0, 1);
//        _clippingStencil->clear();
//        _clippingStencil->drawPolygon(rect, 4, green, 0, green);
//    }
//}