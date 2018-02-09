//
//  MoveLisenter.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#ifndef MoveLisenter_hpp
#define MoveLisenter_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

using namespace std;

class MoveLisenter : public  EventListenerTouchOneByOne{
    MoveLisenter();
    ~MoveLisenter();
public:
    static MoveLisenter* create();
    bool init();
private:
    bool touchBean(Touch* touch, Event* event);
    void touchMove(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancelled(Touch* touch, Event* event);
public:
    
    //回调方法
    std::function<void(Node* node,MoveLisenter* lisenter)> onTouchDown;
    std::function<void(Node* node,MoveLisenter* lisenter,int index)> onTouchMoveInRect;
    std::function<void(Node* node,MoveLisenter* lisenter)> onTouchMoveOutRect;
    std::function<void(Node* node,MoveLisenter* lisenter)> onTouchUp;
    std::function<void(Node* node,MoveLisenter* lisenter,Touch* touch)> onTouchMove;
    
    std::function<void(Node* node,MoveLisenter* lisenter,Touch* touch)> onTouchMix;
    
    void setOrginOffset();
    
    //响应区域
    void addRect(const Rect& rect,Node* root = NULL);
    void addRectByNode(Node*, Node* root = NULL);
    void clearRectVector();
    int isInRect(const Vec2& pos);
public:
    vector<Rect>    m_vRects;
    Rect            m_vMoveRect;
    Vec2            m_oOrginOffset;
    
    Vec2            _oldPos;
    int             _zOrder;
    
    bool            m_bIsMoveBack;
    bool            m_bIsMove;
};


#endif /* MoveLisenter_hpp */
