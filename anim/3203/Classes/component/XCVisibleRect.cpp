//
//  XCVisibleRect.cpp
//  WOLEGEQU
//
//  Created by dengcheng on 27/5/14.
//
//

#include "XCVisibleRect.h"

USING_NS_CC;

static Size kDesignSize;
static Size kFrameSize;
static Size kRealDesignSize;
static Point kDesignOffsetPoint;

void XCVisibleRect::setVisibleRect(const cocos2d::Size &frameSize, const cocos2d::Size &designSize)
{
    kDesignSize = designSize;
	kFrameSize = frameSize;
    float scaleX = frameSize.width / designSize.width;
    float scaleY = frameSize.height / designSize.height;
    float scale = MIN(scaleX, scaleY);
    kRealDesignSize = Size(frameSize.width / scale, frameSize.height / scale);
    XCVisibleRect::calculateOffset();
}

const Size &XCVisibleRect::getRealDesignSize() {
    return kRealDesignSize;
}

Point XCVisibleRect::getPosition(float positionX, float positionY,
                                   HorizontalBorder xType, VerticalBorder yType)
{
    float x = 0;
    float y = 0;
    x = XCVisibleRect::getPositionX(positionX, xType);
    y = XCVisibleRect::getPositionY(positionY, yType);
    return Point(x, y);
}

Point XCVisibleRect::getPosition(cocos2d::Point position,
                                   HorizontalBorder xType, VerticalBorder yType) {
    float x = 0;
    float y = 0;
    x = XCVisibleRect::getPositionX(position.x, xType);
    y = XCVisibleRect::getPositionY(position.y, yType);
    return Point(x, y);
}

float XCVisibleRect::getPositionX(float positionX, HorizontalBorder xType)
{
    float x = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    switch (xType) {
        case kLeft:
        {
            x = positionX;
        }
            break;
        case kRight:
        {
            x = visibleSize.width - positionX;
        }
            break;
        default:
        {
            x = kDesignOffsetPoint.x + positionX;
        }
            break;
    }
    return x;
}

float XCVisibleRect::getPositionY(float positionY, VerticalBorder yType)
{
    float y = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    switch (yType) {
        case kTop:
        {
            y = visibleSize.height - positionY;
        }
            break;
        case kBottom:
        {
            y = positionY;
        }
            break;
            
        default:
        {
            y = kDesignOffsetPoint.y + positionY;
        }
            break;
    }
    return y;
}

Point XCVisibleRect::getMidPoint() {
    return kFrameSize / 2;
}

void XCVisibleRect::calculateOffset()
{
    kDesignOffsetPoint = Point((kRealDesignSize.width - kDesignSize.width) / 2.0f,
                         (kRealDesignSize.height - kDesignSize.height) / 2.0f);
}