//
//  AdapterScreen.h
//  TestCreate
//
//  Created by tangbowen on 14-9-11.
//
//

#ifndef __TestCreate__AdapterScreen__
#define __TestCreate__AdapterScreen__

#pragma once
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

#define         kAnchorLT           Vec2(0, 1)
#define         kAnchorRT           Vec2(1, 1)
#define         kAnchorLB           Vec2(0, 0)
#define         kAnchorRB           Vec2(1, 0)
#define         kAnchorDF           Vec2(0.5, 0.5)
#define         kAnchorLC           Vec2(0, 0.5)
#define         kAnchorTC           Vec2(0.5, 1)
#define         kAnchorRC           Vec2(1, 0.5)
#define         kAnchorBC           Vec2(0.5, 0)

#define         kAdapterScreen          AdapterScreen::getInstance()
#define         kNodeCenter(__node__)   AdapterScreen::getInstance()->getCenterFromNode(__node__)

enum BorderType{
    kBorderNone         =       1 << 0,
    kBorderLeft         =       1 << 1,
    kBorderRight        =       1 << 2,
    kBorderTop          =       1 << 3,
    kBorderBottom       =       1 << 4,
};

enum kRelativeLocationType{
    kRelativeLeft           =           1 << 0,
    kRelativeTop            =           1 << 1,
    kRelativeRight          =           1 << 2,
    kRelativeBottom         =           1 << 3
};

class AdapterScreen {
    
private:
    AdapterScreen();
    virtual ~AdapterScreen();
    
public:
    static AdapterScreen* m_pAdapterScreen;
    static AdapterScreen* getInstance();
    
public:
    Vec2 g_oOffset;
    
public:
    Vec2 getExactPostion(Vec2 point);
    void setUpScreenAdapter(float width, float height);
    
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
    void setExactPosition(Node* node, float x, float y, Vec2 anchorVec2 = kAnchorDF, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    
    Vec2 getRealPosition(float x, float y, bool bTop = false, bool bBottom = false, bool bLeft = false
                         , bool bRight = false);
    
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
    Vec2 setPositionByNode(Node* targetNode, Node* relativeNode, const float space, kRelativeLocationType locationType = kRelativeBottom);
    
    Vec2 getCenterFromNode(Node* node, int offsetX = 0, int offsetY = 0);
};

#endif /* defined(__TestCreate__AdapterScreen__) */
