//
//  KDIYScrollView.h
//  CookiesMaker
//
//  Created by wangbosheng1 on 14-12-1.
//
//

#ifndef __CookiesMaker__KDIYScrollView__
#define __CookiesMaker__KDIYScrollView__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "KDAdapter.h"
USING_NS_CC;
USING_NS_CC_EXT;
class KDIYScrollView :public Layer,public ScrollViewDelegate{
    
public:
    KDIYScrollView();
    //    ScrollView* pp;
    virtual ~KDIYScrollView();
    static KDIYScrollView* create(const Size &contentSize);
    virtual bool initWithSize(const Size &contentSize);
    
    void menuCallback(Ref* pSender);
public:
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool TouchBegan(Touch *pTouch, Event *pEvent);
    virtual void TouchMoved(Touch *pTouch, Event *pEvent);
    virtual void TouchEnded(Touch *pTouch, Event *pEvent);
    virtual void TouchCancelled(Touch *pTouch, Event *pEvent);
public:
    void setDirection(cocos2d::extension::ScrollView::Direction eDirection);
    void setScrollEnable(bool enable);
    void refresh();
    void setBackground(Node* bg);
    void setAdapter(KDAdapter* adpter);
    KDAdapter* getAdapter();
    void setItemClickCall(const std::function<void(int,MenuItem*,int)>& fn)
    {
        m_fnClickCall = fn;
    }
private:
    //scrollview滚动的时候会调用
    void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    //scrollview缩放的时候会调用
    void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    
    void layoutView();
public:
    Layer*      m_pMask;
    cocos2d::extension::ScrollView* m_pScrollView;
    Layer*      m_Container;
    
    Vec2        m_touchPoint;
    Menu*       m_pMenu;
    float       m_fBeginOffset;
    Vector<MenuItem*> m_pMenuItemsVector;
private:
    std::function<void(int,MenuItem*,int)>  m_fnClickCall;
    bool m_bScroll;
    bool m_bLock;
    bool m_bTouchedMenu;
    bool m_bScrollNeedAdjust;
protected:
    KDAdapter* m_Adapter;
};
#endif /* defined(__CookiesMaker__KDIYScrollView__) */
