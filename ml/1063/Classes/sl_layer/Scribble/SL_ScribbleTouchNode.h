//
//  SL_ScribbleTouchNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#ifndef __SL_ScribbleTouchNode__
#define __SL_ScribbleTouchNode__

#include <stdio.h>
#include "ScribbleNode.h"

//修改，增加getPecent()方法获取百分比
class SL_ScribbleTouchDelegate;

class SL_ScribbleTouchNode : public ScribbleNode {
public:
    enum class TouchType{
        eTouchBegan = 0x1,
        eTouchMoved = 0x2,
        eTouchEnded = 0x4
    };
    static SL_ScribbleTouchNode *create(Size pCanvasSize);
    
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
    bool init(Size pCanvasSize);
    SL_ScribbleTouchNode();
    ~SL_ScribbleTouchNode();
    
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
    void setDelegate(SL_ScribbleTouchDelegate* delegate);
    float getPecent();
    //像素点
    long restorePixes;
    inline  RenderTexture * getRender(){return _canvas;}
    SL_ScribbleTouchDelegate* m_pDelegate;
//    int m_iType;
//    bool m_bSound;
    int m_iCount;
};

//擦除的代理
class SL_ScribbleTouchDelegate {
    
public:
    virtual void onScribbleTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){};
    virtual void onScribbleTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){};
    virtual void onScribbleTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){};
    virtual void onScribbleTouchOver(){};
    
};
#endif /* defined(__SL_ScribbleTouchNode__) */
