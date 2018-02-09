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


enum eDirection{
    eHORIZONTAL = 0,
    eVERTICAL,
    eNONE
};

class ToolSpriteDelegate;

class ToolSprite : public Sprite {
    
public:
    ToolSprite();
    virtual ~ToolSprite();
    
public:
    static ToolSprite* create(const char* dragFileName, const char* iconFileName = nullptr);
    
    virtual bool init(const char* dragFileName, const char* iconFileName);
    
    static ToolSprite* createWithTexture(Texture2D *texture);
    
    virtual bool initWithTexture(Texture2D* texture);
    
public:
    virtual void onEnter();
    virtual void onExit();    
public:
    void setTextureByFileName(const char* dragFileName, const char* iconFileName = nullptr);
    void removeRectByID(int id);
    
    void addRect(const Rect& rect);
    
    void addRectByNode(Node*, float originOffsetX = 0, float originOffsetY = 0, float widthOffset = 0, float heightOffset = 0, Node* root = nullptr);
    void clearRect();
    
    bool isContain(const Rect& rect);
    
    int isInRect(const Point& pos);
    
    void setResponsePoint(const Point& pos);
    
    void setDelegate(ToolSpriteDelegate* delegate);
    
    Point getJudgePosition();
    
    void setMoveRectByNode(Node* node, float originOffsetX = 0, float originOffsetY = 0, float widthOffset = 0, float heightOffset = 0, bool isFlag = false);
    
public:
    void moveOver(Ref* pSender);
    
    void moveBack();
    
    void setDragBack(const bool canDragBack);
    
    void cancelEvent(const bool cancelEvent);
    void setSpriteEnabled(const bool enable){ cancelEvent(!enable);};
    bool getSpriteEnabled(){ return !m_bCancelEvent;};
    
    void setDrag(const bool canDrag);
    
    void setOriginalPoint(Vec2 vec);
    
    void setScaleBtn(bool isScale);
    void setShadeBtn(bool isShade);
    
    void setSwallowTouch(bool isSwallow);
    
    void setNeedDragBack(bool isNeed);
    
public:
    void setLimitDirection(eDirection dir);
    
    void setFlag(int flag);
    int getFlag();
    
    void setAllowTouch(bool isAllow);
    
    void setOriginPoint(Point point);
    Point getOriginPoint();
    
    void setRegisterType(bool b);
    
    void setMovebackTime(float delay) {
        m_fDelay = delay;
    };

public:
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    
    void TouchMoved(Touch *pTouch, Event *pEvent);
    
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    void TouchCancelled(Touch *pTouch, Event *pEvent);
    
private:
    ToolSpriteDelegate* m_pDelegate;
    
    
public:
    Vec2            m_oResponseDelta;
    Vec2            m_oResponsePoint;
    Vec2            m_oOrginOffset;
    vector<Rect>    m_vRects;
    Rect            m_vMoveRect;
    
public:
    bool m_bCanDragBack;
    
    bool m_bCancelEvent;
    
    bool m_bCanDrag;
    
public:
    Texture2D* m_pDragTexture2D;
    
    Texture2D* m_pIconTexture2D;
    
    Rect m_oDragRect;
    
    Rect m_oIconRect;
    
    Vec2 m_oOriginPoint;
    
private:
    bool m_bIsScale;
    bool m_bIsShade;
    
    bool m_bIsSwallowTouch;
    
    bool m_bIsNeedDragBack;

    Size m_oToolSIze;
    
    eDirection m_eDir;
    
    int m_iFlag;
    
    bool m_bIsAllowTouch;
    
    bool m_bIsPriority;
    EventListenerTouchOneByOne* m_pListerner;
    
    float m_fDelay;
    
};

class ToolSpriteDelegate {
    
public:
    virtual void onTouchCancel(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index){};
    virtual void onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch){};
    virtual void onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index){};
    virtual void onTouchUpOutRect(ToolSprite* toolSprite,Touch *pTouch){};
    
    virtual void onMoveBackCallback(ToolSprite* toolSprite){};

};

#endif