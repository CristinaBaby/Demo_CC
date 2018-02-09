//
//  AdapterScreen.cpp
//  TestCreate
//
//  Created by tangbowen on 14-9-11.
//
//

#include "AdapterScreen.h"

AdapterScreen* AdapterScreen::m_pAdapterScreen = nullptr;

AdapterScreen::AdapterScreen()
{
    g_oOffset = Vec2::ZERO;
}

AdapterScreen::~AdapterScreen()
{
    
}

AdapterScreen* AdapterScreen::getInstance()
{
    if (!m_pAdapterScreen) {
        m_pAdapterScreen = new AdapterScreen();
    }
    return m_pAdapterScreen;
}

void AdapterScreen::setUpScreenAdapter(float width, float height)
{
    Size designSize = Size(width, height);
    Director* director = Director::getInstance();
    GLView* glview = director->getOpenGLView();
    
    Size frameSize = glview->getFrameSize();
    
    float scaleX = frameSize.width / designSize.width;
    float scaleY = frameSize.height / designSize.height;
    float scale = MIN(scaleX, scaleY);
    
    glview->setDesignResolutionSize(frameSize.width / scale, frameSize.height / scale, ResolutionPolicy::NO_BORDER);
    
    // set the offset
    Size winSize = director->getWinSize();
    g_oOffset = Vec2((winSize.width - designSize.width) / 2, (winSize.height - designSize.height) / 2);
}

/**
 *  @Method     _G_SetPosition
 *  @author     tangbowen
 *  @date       2014-08-21 05:05:30
 *  @function   set the node position<br/>
 *  @param      node            want to set the node
 *  @param      x               the x position
 *  @param      y               the y position
 *  @param      anchorVec2      the anchor point of noe
 *  @param      borderX
 *  @param      borderY
 */
void AdapterScreen::setExactPosition(Node* node, float x, float y, Vec2 anchorPoint, BorderType borderX, BorderType borderY)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 originVec2 = Director::getInstance()->getVisibleOrigin();
    if (borderX & kBorderNone) {
        x = g_oOffset.x + x;
    }else if (borderX & kBorderLeft){
        x = originVec2.x + x;
        
    }else if (borderX & kBorderRight){
        x = originVec2.x + visibleSize.width - x;
    }
    
    if (borderY & kBorderNone) {
        y = g_oOffset.y + y;
    }else if (borderY & kBorderBottom){
        y = originVec2.y + y;
    }else if (borderY & kBorderTop){
        y = originVec2.y + visibleSize.height - y;
    }
    node->setAnchorPoint(anchorPoint);
    node->setPosition(x, y);
}

Vec2 AdapterScreen::getExactPostion(Vec2 point)
{
    return Vec2(point.x+g_oOffset.x, point.y+g_oOffset.y);
}

/**
 *  @Method     _G_SetPositionByNode
 *  @author     tangbowen
 *  @date       2014-08-21 05:05:30
 *  @function   make the target node to the relative node around
 *  @param      targetNode      the target node
 *  @param      relativeNode    relative node
 *  @param      space           the space
 *  @param      locationType    the direction of relative node
 *  @return     Vec2            the position of target node
 */
Vec2 AdapterScreen::setPositionByNode(Node* targetNode, Node* relativeNode, const float space, kRelativeLocationType locationType)
{
    Vec2 relativeAnchorVec2 = relativeNode->getAnchorPoint();
    Vec2 targetAnchorVec2 = relativeNode->getAnchorPoint();
    Vec2 newPoint = Vec2::ZERO;
    if (locationType & kRelativeLeft) {
        // 得到相对于那个的最左边x的值
        float relativeMinX = relativeNode->getPositionX() - (relativeAnchorVec2.x * relativeNode->getContentSize().width);
        float targetWidth = targetNode->getContentSize().width - (targetNode->getContentSize().width * targetAnchorVec2.x);
        newPoint.x = relativeMinX - targetWidth - space;
        newPoint.y = relativeNode->getPositionY();
    }else if (locationType & kRelativeTop)
    {
        float relativeMaxY = relativeNode->getPositionY() + ((1 - relativeAnchorVec2.y) * relativeNode->getContentSize().height);
        float targetHeight = targetNode->getContentSize().height * targetAnchorVec2.y;
        newPoint.y = relativeMaxY + targetHeight + space;
        newPoint.x = relativeNode->getPositionX();
    }else if (locationType & kRelativeRight)
    {
        float relativeMaxX = relativeNode->getPositionX() + ((1 - relativeAnchorVec2.x) * relativeNode->getContentSize().width);
        float targetWidth = targetNode->getContentSize().width * targetAnchorVec2.x;
        newPoint.x = relativeMaxX + targetWidth + space;
        newPoint.y = relativeNode->getPositionY();
        
    }else if (locationType & kRelativeBottom)
    {
        float relativeMinY = relativeNode->getPositionY() - (relativeAnchorVec2.y * relativeNode->getContentSize().height);
        float targetHeight = (1 - targetAnchorVec2.y) * targetNode->getContentSize().height;
        newPoint.x = relativeNode->getPositionX();
        newPoint.y = relativeMinY - targetHeight - space;
    }
    targetNode->setPosition(newPoint);
    return newPoint;
}

Vec2 AdapterScreen::getCenterFromNode(cocos2d::Node *node, int offsetX, int offsetY)
{
    return Vec2(node->getContentSize().width / 2 + offsetX, node->getContentSize().height / 2 + offsetY);
}