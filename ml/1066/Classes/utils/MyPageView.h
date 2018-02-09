//
//  MyPageView.h
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/28.
//
//

#ifndef __LunchFoodMaker__MyPageView__
#define __LunchFoodMaker__MyPageView__

#include <stdio.h>

#include <iostream>
#include "cocos2d.h"
#include "ToolSprite.h"
#include "BaseLayer.h"

USING_NS_CC;

class MyPageViewDelegate;
class MyPageView : public PageView{
    
public:
    MyPageView();
    ~MyPageView();
    
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(MyPageView);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent);
    
public:
    void setMyPageViewDelegate(MyPageViewDelegate* delegate);
    
private:
    MyPageViewDelegate* m_pDelegate;
    
    bool m_bIsFlag;
};

class MyPageViewDelegate{

public:
    virtual void isCancelTouch(bool isCancel){};
    virtual void touchBegin(){};
    virtual void touchEnd(){};
};
#endif /* defined(__LunchFoodMaker__MyPageView__) */
