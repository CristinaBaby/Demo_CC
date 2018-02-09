#include "STVisibleRect.h"

USING_NS_CC;

static Size s_FrameSize;
static Size s_DesignSize;
static Size s_RealDesignSize;
static Vec2 s_TopLeftOffset;
static Vec2 s_DesignOffset;
//屏幕边与showall区域的偏移
static Vec2 s_FrameAndDesignOffset;
static float s_RealDesignScale = 1.0f;

static int s_RetainOffsetLeft = 0;
static int s_RetainOffsetTop = 0;
static int s_RetainOffsetRight = 0;
static int s_RetainOffsetBottom = 0;

void STVisibleRect::setupVisibleRect(float width, float height)
{
    s_DesignSize = Size(width, height);
    
    GLView* glView = Director::getInstance()->getOpenGLView();
    
    Size frameSize = glView->getFrameSize();
    
    s_FrameSize = frameSize;
    
    float scaleX = frameSize.width / s_DesignSize.width;
    float scaleY = frameSize.height / s_DesignSize.height;
    float minScale = MIN(scaleX, scaleY);
    
    s_RealDesignScale = MAX(scaleX, scaleY) / MIN(scaleX, scaleY);
    s_RealDesignSize = Size(frameSize.width / minScale, frameSize.height / minScale);
    
    s_DesignOffset = (s_RealDesignSize - s_DesignSize) * 0.5;
    
    glView->setDesignResolutionSize(frameSize.width / minScale, frameSize.height / minScale, ResolutionPolicy::NO_BORDER);
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

void STVisibleRect::setPositionAdapted(Node* node, float x, float y, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    node->setPosition(getPositionAdapted(Vec2(x, y), borderX, borderY));
}

void STVisibleRect::setPositionAdapted(Node* node, Vec2 position, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    node->setPosition(getPositionAdapted(position, borderX, borderY));
}

std::vector<Vec2> STVisibleRect::getPositionAdapted(std::vector<Vec2> &vPosition, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : vPosition)
    {
        i = getPositionAdapted(i, borderX, borderY);
    }
    return vPosition;
}

Vec2 STVisibleRect::getPositionAdapted(Vec2 position, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    float posX = position.x + s_DesignOffset.x;
    float posY = position.y + s_DesignOffset.y;
    
    if (borderX & kBorderLeft)
    {
        posX -= s_DesignOffset.x;
    }
    else if (borderX & kBorderRight)
    {
        posX += s_DesignOffset.x;
    }
    
    if (borderY & kBorderTop)
    {
        posY += s_DesignOffset.y;
    }
    else if (borderY & kBorderBottom)
    {
        posY -= s_DesignOffset.y;
    }
    
    return Vec2(posX, posY);
}

Rect STVisibleRect::getRectAdapted(float x, float y, float width, float height, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    Vec2 offsetPoint = getPositionAdapted(Vec2(x, y), borderX, borderY);
    return Rect(offsetPoint.x, offsetPoint.y, width, height);
}

Rect STVisibleRect::getRectAdapted(cocos2d::Rect rect, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    return getRectAdapted(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, borderX, borderY);
}

std::vector<Rect> STVisibleRect::getRectAdapted(std::vector<Rect> &vRect, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : vRect)
    {
        i = getRectAdapted(i, borderX, borderY);
    }
    return vRect;
}

Size& STVisibleRect::getRealDesignSize()
{
	return s_RealDesignSize;
}

Size& STVisibleRect::getDesignSize()
{
	return s_DesignSize;
}

Vec2 STVisibleRect::getDesignCenter()
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

Vec2 STVisibleRect::getDesignOffset()
{
	return s_DesignOffset;
}

Vec2 STVisibleRect::getCenterOfScene()
{
    return Vec2(getPointOfSceneLeftBottom().x + Director::getInstance()->getVisibleSize().width * .5f, getPointOfSceneLeftBottom().y + Director::getInstance()->getVisibleSize().height* .5f);
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

Vec2 STVisibleRect::getPointOfSceneUp()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return Vec2(getPointOfSceneLeftBottom().x + visibleSize.width * 0.5, getPointOfSceneLeftBottom().y + visibleSize.height);
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

#pragma mark - 原来的接口，以后应当废弃的
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

std::vector<Vec2> STVisibleRect::getRealPositionForDesignPosition(std::vector<Vec2>& aPositiones, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    for (auto &i : aPositiones)
    {
        i = getRealPositionForDesignPosition(i, bTop, bBottom, bLeft, bRight);
    }
    return aPositiones;
}

Rect STVisibleRect::getRealRect(Rect pRect, bool pTop, bool pBottom, bool pLeft, bool pRight)
{
    float offsetX = pRect.origin.x + s_DesignOffset.x;
    float offsetY = pRect.origin.y + s_DesignOffset.y;
    
    if (pTop)
        offsetY += s_DesignOffset.y;
    else if (pBottom)
        offsetY -= s_DesignOffset.y;
    if (pLeft)
        offsetX -= s_DesignOffset.x;
    else if (pRight)
        offsetX += s_DesignOffset.x;
    
    return Rect(offsetX, offsetY, pRect.size.width, pRect.size.height);
}

Rect STVisibleRect::getRealRect(float x, float y, float width, float height, bool pTop, bool pBottom, bool pLeft, bool pRight)
{
    float offsetX = x + s_DesignOffset.x;
    float offsetY = y + s_DesignOffset.y;
    
    if (pTop)
        offsetY += s_DesignOffset.y;
    else if (pBottom)
        offsetY -= s_DesignOffset.y;
    if (pLeft)
        offsetX -= s_DesignOffset.x;
    else if (pRight)
        offsetX += s_DesignOffset.x;
    
    return Rect(offsetX, offsetY, width, height);
}

std::vector<Rect> STVisibleRect::getRealRects(std::vector<Rect> &aPositiones, bool pTop, bool pBottom, bool pLeft, bool pRight)
{
    std::vector<Rect> pRet = aPositiones;
    for (auto &i : pRet)
    {
        i = getRealRect(i, pTop, pBottom, pLeft, pRight);
    }
    return pRet;
}
