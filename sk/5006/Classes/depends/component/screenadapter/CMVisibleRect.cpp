#include "depends/component/screenadapter/CMVisibleRect.h"

USING_NS_CC;

static Size g_sFrameSize;
static Size g_sDesignSize;
static Size g_sRealDesignSize;
static Vec2 g_sTopLeftOffset;
static Vec2 g_sDesignOffset;
static float g_sRealDesignScale = 1.0f;

void CMVisibleRect::setupVisibleRect(float width, float height)
{
    g_sDesignSize = Size(width, height);
    
    GLView* glView = Director::getInstance()->getOpenGLView();
    
    Size frameSize = glView->getFrameSize();
    
    float scaleX = frameSize.width / g_sDesignSize.width;
    float scaleY = frameSize.height / g_sDesignSize.height;
    float minScale = MIN(scaleX, scaleY);
    
    g_sRealDesignScale = 1.0 / minScale;
    g_sRealDesignSize = Size(frameSize.width / minScale, frameSize.height / minScale);
    
    g_sDesignOffset = (g_sRealDesignSize - g_sDesignSize) * 0.5;
    
    glView->setDesignResolutionSize(frameSize.width / minScale, frameSize.height / minScale, ResolutionPolicy::NO_BORDER);
}

void CMVisibleRect::setPosition(Node* node, float x, float y,
                                BorderType borderX/*=kBorderNone*/,
                                BorderType borderY/*=kBorderNone*/)
{
    Size vSize = Director::getInstance()->getVisibleSize();
    Size nodeSize = node->getContentSize();
    Vec2 anchor = node->getAnchorPoint();
    if (borderX & kBorderNone)
    {
        x = g_sDesignOffset.x + x;
    }
    else if (borderX & kBorderLeft)
    {
        x =  x + nodeSize.width * anchor.x;
    }
    else if (borderX & kBorderRight)
    {
        x = vSize.width - x - nodeSize.width * (1 - anchor.x);
    }
    
    if (borderY & kBorderNone)
    {
        y = g_sDesignOffset.y + y;
    }
    else if (borderY & kBorderBottom)
    {
        y = y + nodeSize.height * anchor.y;
    }
    else if (borderY & kBorderTop)
    {
        y = vSize.height - y - nodeSize.height * (1 - anchor.y);
    }
    node->setPosition(x, y);
}

void CMVisibleRect::setPosition(Node* node, Vec2 position,
                                BorderType borderX/*=kBorderNone*/,
                                BorderType borderY/*=kBorderNone*/)
{
    setPosition(node,position.x,position.y,borderX,borderY);
}

Vec2 CMVisibleRect::getPosition(float x, float y,
                                BorderType borderX,
                                BorderType borderY)
{
    Size vSize = Director::getInstance()->getVisibleSize();
    if (borderX & kBorderNone)
    {
        x = g_sDesignOffset.x + x;
    }
    else if (borderX & kBorderRight)
    {
        x = vSize.width - x;
    }
    
    if (borderY & kBorderNone)
    {
        y = g_sDesignOffset.y + y;
    }
    else if (borderY & kBorderTop)
    {
        y = vSize.height - y;
    }
    return Vec2(x,y);
}

void CMVisibleRect::getPosition(std::vector<Vec2>* vPosition,
                                BorderType borderX,
                                BorderType borderY)
{
    for (auto &i : (*vPosition))
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

void CMVisibleRect::getPositionAdapted(std::vector<Vec2>* vPosition,
                                       BorderType borderX/*=kBorderNone*/,
                                       BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : (*vPosition))
    {
        i = getPositionAdapted(i, borderX, borderY);
    }
}

Vec2 CMVisibleRect::getPositionAdapted(Vec2 position,
                                       BorderType borderX/*=kBorderNone*/,
                                       BorderType borderY/*=kBorderNone*/)
{
    float posX = position.x + g_sDesignOffset.x;
    float posY = position.y + g_sDesignOffset.y;
    
    if (borderX & kBorderLeft)
    {
        posX -= g_sDesignOffset.x;
    }
    else if (borderX & kBorderRight)
    {
        posX += g_sDesignOffset.x;
    }
    
    if (borderY & kBorderTop)
    {
        posY += g_sDesignOffset.y;
    }
    else if (borderY & kBorderBottom)
    {
        posY -= g_sDesignOffset.y;
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
    return getRectAdapted(rect.origin.x, rect.origin.y,
                          rect.size.width, rect.size.height,
                          borderX, borderY);
}

void CMVisibleRect::getRectAdapted(std::vector<Rect>* vRect,
                                   BorderType borderX/*=kBorderNone*/,
                                   BorderType borderY/*=kBorderNone*/)
{
    for (auto &i : (*vRect))
    {
        i = getRectAdapted(i, borderX, borderY);
    }
}

const Size& CMVisibleRect::getRealDesignSize()
{
	return g_sRealDesignSize;
}

const Size& CMVisibleRect::getDesignSize()
{
	return g_sDesignSize;
}

Vec2 CMVisibleRect::getDesignCenter()
{
    return Vec2(g_sDesignSize.width * .5f, g_sDesignSize.height * .5f);
}

const Size& CMVisibleRect::getFrameSize()
{
	return g_sFrameSize;
}

float CMVisibleRect::getRealDesignScale()
{
	return g_sRealDesignScale;
}

const Vec2& CMVisibleRect::getDesignOffset()
{
	return g_sDesignOffset;
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
