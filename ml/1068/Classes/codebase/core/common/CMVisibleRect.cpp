#include "CMVisibleRect.h"

USING_NS_CC;

static Size s_FrameSize;
static Size s_DesignSize;
static Size s_RealDesignSize;
static Vec2 s_TopLeftOffset;
static Vec2 s_DesignOffset;
static float s_RealDesignScale = 1.0f;

void CMVisibleRect::setupVisibleRect(float width, float height)
{
    s_DesignSize = Size(width, height);
    
    GLView* glView = Director::getInstance()->getOpenGLView();
    
    Size frameSize = glView->getFrameSize();
    
    float scaleX = frameSize.width / s_DesignSize.width;
    float scaleY = frameSize.height / s_DesignSize.height;
    float minScale = MIN(scaleX, scaleY);
    
    s_RealDesignScale = 1.0 / minScale;
    s_RealDesignSize = Size(frameSize.width / minScale, frameSize.height / minScale);
    
    s_DesignOffset = (s_RealDesignSize - s_DesignSize) * 0.5;
    
    glView->setDesignResolutionSize(frameSize.width / minScale, frameSize.height / minScale, ResolutionPolicy::NO_BORDER);
}

void CMVisibleRect::setPosition(Node* node, float x, float y,
                                BorderType borderX,
                                BorderType borderY)
{
    Size vSize = Director::getInstance()->getVisibleSize();
    Size nodeSize = node->getContentSize();
    Vec2 anchor = node->getAnchorPoint();
    if (borderX & kBorderNone) {
        x = s_DesignOffset.x + x;
    }else if (borderX & kBorderLeft){
        x =  x + nodeSize.width * anchor.x;
    }else if (borderX & kBorderRight){
        x = vSize.width - x - nodeSize.width * (1 - anchor.x);
    }
    
    if (borderY & kBorderNone) {
        y = s_DesignOffset.y + y;
    }else if (borderY & kBorderBottom){
        y = y + nodeSize.height * anchor.y;
    }else if (borderY & kBorderTop){
        y = vSize.height - y - nodeSize.height * (1 - anchor.y);
    }
    node->setPosition(x, y);
}

void CMVisibleRect::setPosition(Node* node, Vec2 position,
                                BorderType borderX,
                                BorderType borderY)
{
    setPosition(node,position.x,position.y,borderX,borderY);
}

Vec2 CMVisibleRect::getPosition(float x, float y,
                                BorderType borderX,
                                BorderType borderY)
{
    Size vSize = Director::getInstance()->getVisibleSize();
    if (borderX & kBorderNone) {
        x = s_DesignOffset.x + x;
    }else if (borderX & kBorderRight){
        x = vSize.width - x;
    }
    
    if (borderY & kBorderNone) {
        y = s_DesignOffset.y + y;
    }else if (borderY & kBorderTop){
        y = vSize.height - y;
    }
    return Vec2(x,y);
}

void CMVisibleRect::getPosition(std::vector<Vec2>& vPosition,
                                BorderType borderX,
                                BorderType borderY)
{
    for (auto &i : vPosition)
    {
        i = getPosition(i.x,i.y, borderX, borderY);
    }
}

void CMVisibleRect::setPositionAdapted(Node* node, float x, float y,
                                       BorderType borderX/*=kBorderNone*/,
                                       BorderType borderY/*=kBorderNone*/)
{
    node->setPosition(getPositionAdapted(Vec2(x, y), borderX, borderY));
}

void CMVisibleRect::setPositionAdapted(Node* node, Vec2 position,
                                       BorderType borderX/*=kBorderNone*/,
                                       BorderType borderY/*=kBorderNone*/)
{
    node->setPosition(getPositionAdapted(position, borderX, borderY));
}

void CMVisibleRect::getPositionAdapted(std::vector<Vec2> &vPosition,
                                                    BorderType borderX/*=kBorderNone*/,
                                                    BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : vPosition)
    {
        i = getPositionAdapted(i, borderX, borderY);
    }
}

Vec2 CMVisibleRect::getPositionAdapted(Vec2 position, BorderType borderX/*=kBorderNone*/, BorderType borderY/*=kBorderNone*/)
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

Rect CMVisibleRect::getRectAdapted(float x, float y, float width, float height,
                                   BorderType borderX/*=kBorderNone*/,
                                   BorderType borderY/*=kBorderNone*/)
{
    Vec2 offsetPoint = getPositionAdapted(Vec2(x, y), borderX, borderY);
    return Rect(offsetPoint.x, offsetPoint.y, width, height);
}

Rect CMVisibleRect::getRectAdapted(cocos2d::Rect rect,
                                   BorderType borderX/*=kBorderNone*/,
                                   BorderType borderY/*=kBorderNone*/)
{
    return getRectAdapted(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, borderX, borderY);
}

void CMVisibleRect::getRectAdapted(std::vector<Rect> &vRect,
                                   BorderType borderX/*=kBorderNone*/,
                                   BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : vRect)
    {
        i = getRectAdapted(i, borderX, borderY);
    }
}

Size& CMVisibleRect::getRealDesignSize()
{
	return s_RealDesignSize;
}

Size& CMVisibleRect::getDesignSize()
{
	return s_DesignSize;
}

Vec2 CMVisibleRect::getDesignCenter()
{
    return Vec2(s_DesignSize.width * .5f, s_DesignSize.height * .5f);
}

Size& CMVisibleRect::getFrameSize()
{
	return s_FrameSize;
}

float CMVisibleRect::getRealDesignScale()
{
	return s_RealDesignScale;
}

Vec2 CMVisibleRect::getDesignOffset()
{
	return s_DesignOffset;
}

Vec2 CMVisibleRect::getCenterOfScene()
{
    Size vSize = Director::getInstance()->getVisibleSize();
    return Vec2(vSize.width * 0.5f, vSize.height* 0.5f);
}

Rect CMVisibleRect::getMoveRect(Node* apNode)
{
    Size vSize = Director::getInstance()->getVisibleSize();
    Rect rect;
    Size nodeSize = apNode->getContentSize();
    Vec2 anchor = apNode->getAnchorPoint();
    rect.origin = Vec2(nodeSize.width * anchor.x,nodeSize.height * anchor.y);
    rect.size.width = vSize.width - nodeSize.width * (1 - anchor.x);
    rect.size.height = vSize.height - nodeSize.height * (1 - anchor.y);
    return rect;
}

#pragma mark - 原来的接口，以后应当废弃的
void CMVisibleRect::setPositionST(Node* node, float x, float y, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    node->setPosition(getRealPositionForDesignPosition(Vec2(x, y), bTop, bBottom, bLeft, bRight));
}

void CMVisibleRect::setPositionST(cocos2d::Node *node, cocos2d::Vec2 pt, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    node->setPosition(getRealPositionForDesignPosition(pt, bTop, bBottom, bLeft, bRight));
}

Vec2 CMVisibleRect::getRealPositionForDesignPosition(cocos2d::Vec2 aPosition, bool bTop, bool bBottom, bool bLeft, bool bRight)
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

std::vector<Vec2> CMVisibleRect::getRealPositionForDesignPosition(std::vector<Vec2>& aPositiones, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    for (auto &i : aPositiones)
    {
        i = getRealPositionForDesignPosition(i, bTop, bBottom, bLeft, bRight);
    }
    return aPositiones;
}

Vec2 CMVisibleRect::getRealPoint(float x, float y,  bool bTop , bool bBottom , bool bLeft, bool bRight)
{
    float posX = x + s_DesignOffset.x;
    float posY = y + s_DesignOffset.y;
    
    if(bTop)
        posY += s_TopLeftOffset.y;
    else if(bBottom)
        posY -= s_TopLeftOffset.y;
    
    if(bLeft)
        posX -= s_TopLeftOffset.x;
    else if(bRight)
        posX += s_TopLeftOffset.x;
    
    return Point(posX, posY);
}

Rect CMVisibleRect::getRealRect(Rect pRect, bool pTop, bool pBottom, bool pLeft, bool pRight)
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

Rect CMVisibleRect::getRealRect(float x, float y, float width, float height, bool pTop, bool pBottom, bool pLeft, bool pRight)
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

std::vector<Rect> CMVisibleRect::getRealRects(std::vector<Rect> &aPositiones, bool pTop, bool pBottom, bool pLeft, bool pRight)
{
    std::vector<Rect> pRet = aPositiones;
    for (auto &i : pRet)
    {
        i = getRealRect(i, pTop, pBottom, pLeft, pRight);
    }
    return pRet;
}
