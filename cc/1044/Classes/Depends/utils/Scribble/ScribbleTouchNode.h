//
//  ScribbleTouchNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#ifndef __ScribbleTouchNode__
#define __ScribbleTouchNode__

#include <stdio.h>
#include "ScribbleNode.h"

//修改，增加getPecent()方法获取百分比
class ScribbleTouchNode : public ScribbleNode {
public:
    enum class TouchType{
        eTouchBegan = 0x1,
        eTouchMoved = 0x2,
        eTouchEnded = 0x4
    };
    static ScribbleTouchNode *create(Size pCanvasSize,int type);
    
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void enable(){_enabled = true;}
    void disable(){_enabled = false;}
    bool isEnabled(){return _enabled;}
    void setEnable(bool pEnable){_enabled = pEnable;}
    void enableTouchBegan(bool pEnable){_enableTouchBegan = pEnable;}
    void enableTouchMoved(bool pEnable){_enableTouchMoved = pEnable;}
    void enableTouchEnded(bool pEnable){_enableTouchEnded = pEnable;}
    
    void setEatSceneType();
    
protected:
    bool init(Size pCanvasSize,int type);
    ScribbleTouchNode();
    ~ScribbleTouchNode();
    
protected:
    bool _enabled;
    bool _enableTouchBegan;
    bool _enableTouchMoved;
    bool _enableTouchEnded;
    EventListenerTouchOneByOne *_touchListener;
public:
    long calcute(Image *pImage);
    bool useTarget(string pTargetFile);
    bool useTarget(Sprite* sprite);
    float getPecent();
    //像素点
    long restorePixes;
    inline  RenderTexture * getRender(){return _canvas;}
    
    int m_iType;
    bool m_bSound;
    int m_iCount;
};

#endif /* defined(__ScribbleTouchNode__) */
