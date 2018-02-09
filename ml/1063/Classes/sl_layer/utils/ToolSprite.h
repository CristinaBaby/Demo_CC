//
//  ToolSprite.h
//  PetRescue_tangbowen
//
//  Created by wangbosheng1 on 14-8-15.
//
//

#ifndef __ToolSprite__H__
#define __ToolSprite__H__

#include <iostream>
#include <vector>
#include "cocos2d.h"


using namespace std;
USING_NS_CC;

class ToolSpriteDelegate;

class ToolSprite : public Sprite {
    
public:
    ToolSprite();
    virtual ~ToolSprite();
    
public:
    static ToolSprite* createWithTexture(Texture2D *texture);
    static ToolSprite* create(const char* dragFileName, const char* iconFileName = NULL);
    virtual bool init(const char* dragFileName, const char* iconFileName);
    bool initData();
    
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    void setTextureByFileName(const char* dragFileName, const char* iconFileName = NULL);
    void removeRectByID(int id);
    void addRect(const Rect& rect,Node* root = NULL);
    void addRectByNode(Node*, Node* root = NULL);
    bool isContain(const Rect& rect);
    int  isInRect(const Point& pos);
    void setResponsePoint(const Point& pos);
    void setDelegate(ToolSpriteDelegate* delegate);
    Point getJudgePosition();
    void setMoveRectByNode(Node* node);
    void setMoveRect(const Rect& rect);
    void setNoBackRectByNode(Node* node);
    
    void setIsMove(bool b);
    
    void clearRectVector();
private:
    ToolSpriteDelegate* m_pDelegate;
    
public:
    Vec2            m_oResponseDelta;
    Vec2            m_oResponsePoint;
    Vec2            m_oOrginOffset;
    vector<Rect>    m_vRects;
    Rect            m_vMoveRect;
    Rect            m_rNoBackRect;
    
public:
    bool m_bCanDragBack;
    bool m_bCancelEvent;
    bool m_bCanDrag;
    bool m_bIsMove;
    Texture2D* m_pDragTexture2D;
    Texture2D* m_pIconTexture2D;
    Rect m_oDragRect;
    Rect m_oIconRect;
    Point m_oOriginPoint;
    
public:
    void moveOver(Ref* pSender);
    void moveBack();
    void setDragBack(const bool canDragBack);
    void cancelEvent(const bool cancelEvent);
    void setDrag(const bool canDrag);
    void setOriginalPoint(Vec2 vec);
    
public:
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void TouchCancelled(Touch *pTouch, Event *pEvent);
    
};

class ToolSpriteDelegate {
    
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index){};
    virtual void onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index){};
    virtual void onTouchUpOutRect(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchCancel(ToolSprite* toolSprite,Touch *pTouch){};


};

#endif