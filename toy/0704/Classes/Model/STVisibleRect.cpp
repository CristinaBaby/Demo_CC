#include "STVisibleRect.h"

USING_NS_CC;
USING_NS_ST;

static Size s_FrameSize;
static Size s_DesignSize;
static Size s_RealDesignSize;
static Vec2 s_TopLeftOffset;
static Vec2 s_DesignOffset;
//屏幕边与showall区域的偏移
static Vec2 s_FrameAndDesignOffset;
static float s_RealDesignScale = 1.0f;
static float _relativeScale;

static int s_RetainOffsetLeft = 0;
static int s_RetainOffsetTop = 0;
static int s_RetainOffsetRight = 0;
static int s_RetainOffsetBottom = 0;

void STVisibleRect::setupVisibleRect(Size frameSize, Size designSize)
{
	s_DesignSize = designSize;
	s_FrameSize = frameSize;

    float scaleX=frameSize.width/designSize.width;
    float scaleY=frameSize.height/designSize.height;
    
    float minScale=scaleX>scaleY? scaleY: scaleX;

    frameSize = Size(frameSize.width/minScale, frameSize.height/minScale);

	s_RealDesignScale = 1.0 / minScale;
	s_RealDesignSize = frameSize;
    s_RealDesignSize.width = roundf(s_RealDesignSize.width);
    s_RealDesignSize.height = roundf(s_RealDesignSize.height);

	s_DesignOffset = Vec2((s_RealDesignSize.width - s_DesignSize.width) / 2.0f,
		(s_RealDesignSize.height - s_DesignSize.height) / 2.0f);
    
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(s_RealDesignSize.width, s_RealDesignSize.height, ResolutionPolicy::SHOW_ALL);
    
    //add by luoxp, 得到实际的s_RealDesignSize的scale
    scaleX = s_RealDesignSize.width/designSize.width;
    scaleY = s_RealDesignSize.height/designSize.height;
    
    if (scaleX > scaleY) {
        _relativeScale = scaleX;
    }
    else
    {
        _relativeScale = scaleY;
    }
    
    //relativeScale 实际和下列代码一样
//    auto scaleX = _winSize.width / DESIGN_WIDTH;
//    auto scaleY = _winSize.height / DESIGN_HEIGHT;
//    _relativeScale = scaleX > scaleY ? scaleX : scaleY;
}

void STVisibleRect::initRetainOffset(int left, int top, int right, int bottom)
{
	s_RetainOffsetLeft = left * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetTop = top * s_RealDesignSize.height / s_FrameSize.height;
	s_RetainOffsetRight = right * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetBottom = bottom * s_RealDesignSize.height / s_FrameSize.height;
    NotificationCenter::getInstance()->postNotification("NotifyRetainOffset");
}

int STVisibleRect::getRetainOffsetTop()
{
    return s_RetainOffsetTop;
}

void STVisibleRect::setPosition(Node* node, float x, float y, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
	node->setPosition(getRealPositionForDesignPosition(Vec2(x, y), bTop, bBottom, bLeft, bRight));
}

void STVisibleRect::setPosition(cocos2d::Node *node, cocos2d::Vec2 pt, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    node->setPosition(getRealPositionForDesignPosition(pt, bTop, bBottom, bLeft, bRight));
}

Vec2 STVisibleRect::getRealPositionForDesignPosition(cocos2d::Vec2 aPosition, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    float posX = aPosition.x + s_DesignOffset.x;
	float posY = aPosition.y + s_DesignOffset.y;
    
	if(bTop)
		posY += s_DesignOffset.y;
	else if(bBottom)
		posY -= s_DesignOffset.y;
    if(bLeft)
        posX -= s_DesignOffset.x;
    else if(bRight)
        posX += s_DesignOffset.x;
    return Vec2(posX, posY);
}


Size& STVisibleRect::getRealDesignSize()
{
	return s_RealDesignSize;
}

Size& STVisibleRect::getDesignSize()
{
	return s_DesignSize;
}

Vec2 STVisibleRect::getDesignCeneter()
{
    return Vec2(s_DesignSize.width * .5f, s_DesignSize.height * .5f);
}

Size& STVisibleRect::getFrameSize()
{
	return s_FrameSize;
}

float STVisibleRect::getRealDesignScale()
{
	return s_RealDesignScale;
}

float STVisibleRect::getRelativeScale()
{
    return _relativeScale;
}

Vec2 STVisibleRect::getDesignOffset()
{
	return s_DesignOffset;
}

Vec2 STVisibleRect::getCenterOfScene()
{
    return Vec2(getPointOfSceneLeftBottom().x + s_FrameSize.width * .5f, getPointOfSceneLeftBottom().y + s_FrameSize.height* .5f);
}

Vec2 STVisibleRect::getPointOfSceneLeftBottom()
{
    return Director::getInstance()->getVisibleOrigin();
}

Vec2 STVisibleRect::getPointOfSceneRightUp()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return Vec2(getPointOfSceneLeftBottom().x + visibleSize.width, getPointOfSceneLeftBottom().y + visibleSize.height);
}

Rect STVisibleRect::getBottomMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
                - apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                visibleSize.width + + apNode->getContentSize().width,
                apNode->getContentSize().height);
    return rect;
}

Rect STVisibleRect::getTopMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x, visibleSize.height - apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                visibleSize.width, apNode->getContentSize().height);
    return rect;
}

Rect STVisibleRect::getLeftMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
                getPointOfSceneLeftBottom().y,
                apNode->getContentSize().width, visibleSize.height);
    return rect;
}

Rect STVisibleRect::getRightMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneRightUp().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
               getPointOfSceneLeftBottom().y- apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                apNode->getContentSize().width, visibleSize.height);
    return rect;
}