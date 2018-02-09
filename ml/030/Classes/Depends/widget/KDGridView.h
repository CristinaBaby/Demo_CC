//
//  STGridView.h
//  SweetDiscovery
//
//  Created by zhangguangzong1 on 8/21/13.
//
//

#ifndef _ST_GRID_VIEW_H
#define _ST_GRID_VIEW_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "MyAdapter.h"

//#define ROW         "row"
//#define COL         "col"
//#define TOP         "top"
//#define LEFT        "left"
//#define BOTTOM      "bottom"
//#define RIGHT       "right"
//#define HSPACE      "hSpace"
//#define VSPACE      "vSpace"
//#define DIRECTION   "direction"
//#define HORIZONTAL  "hor"
//#define VERTICAL    "ver"

USING_NS_CC;
USING_NS_CC_EXT;

class KDGridViewScrollCallback;
class KDGridView :public Layer,public ScrollViewDelegate{
    
public:
    KDGridView();
//    ScrollView* pp;
    virtual ~KDGridView();
    static KDGridView* create(const Size &contentSize);
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
    
    void loadLayoutConfig(const char* csv,int row = 1);
    
    void refresh();
    void movePreStep();
    void moveNextStep();
    
    void setAdjustStep(float f);
    
    void setAdapter(KDAdapter* adpter);
    void setScrollCallback(KDGridViewScrollCallback* callback);
    
    KDAdapter* getAdapter();
    KDGridViewScrollCallback* getScrollCallback();
    
    void setScrollEnable(bool enable);
    
    void setEnable(bool enable);
    
    void setBackground(Node* bg);
    
    void setItemClickCall(const std::function<void(int,MenuItem*,int)>& fn)
    {
        m_fnClickCall = fn;
    }
public:
    bool isNearToLeftTopSide();
    bool isNearToRightBottomSide();
    bool isInTouchLoop;
private:
    //scrollview滚动的时候会调用
    void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    //scrollview缩放的时候会调用
    void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
    
    void layoutView();
    
    //根据手势滑动的距离和方向滚动图层
    void adjustScrollView(float offset);
    /*根据下标计算元素所在二维数组的行数*/
    int calculateRow(int index);
    /*根据下标计算元素所在二维数组的列数*/
    int calculateColumn(int index);
    
    void unlock();
    
private:
    std::function<void(int,MenuItem*,int)>  m_fnClickCall;
    KDGridViewScrollCallback*       m_ScrollCallback;
    bool m_bScroll;
    bool m_bLock;
    bool m_bTouchedMenu;
    bool m_bScrollNeedAdjust;
    bool m_bIsEnable;
protected:
    KDAdapter* m_Adapter;
public:
    Layer*      m_pMask;
    cocos2d::extension::ScrollView* m_pScrollView;
    Layer*      m_Container;
    
    Vec2        m_touchPoint;
    Menu*       m_pMenu;
    float       m_fBeginOffset;
    Vector<MenuItem*> m_pMenuItemsVector;
public:
    float m_fMarginTop;
    float m_fMarginLeft;
    float m_fMarginRight;
    float m_fMarginBottom;
    float m_fHSpace;
    float m_fVSpace;
    
    int m_iCol;
    int m_iRow;
    
    float m_fAdjustStep;
    float m_fLowLevelOffsetY;
    float m_fLowLevelOffsetX;
    
    bool m_bAutoGetAdjust;
    bool m_bNeedAdjust;
    
};

class KDGridViewScrollCallback {
public:
    virtual void onScrolling(float offsetX,float offsetY){};
    virtual void onScrollEnd(float offsetX,float offsetY){};
    virtual void onScrollDidZoom(cocos2d::extension::ScrollView* view){};
};
#endif
