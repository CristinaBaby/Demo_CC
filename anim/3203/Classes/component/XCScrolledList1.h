/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：XCScrolledList
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 8/4/15.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __PizzaHouseStory__XCScrolledList1__
#define __PizzaHouseStory__XCScrolledList1__

#include "cocos2d.h"
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include "XCScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 *  操作本类须知:
        1、设置列表滚动方向(不支持多向)
        2、在载入元素之前,必须设置单个Item大小
        3、在载入元素之前,纵向列表必须设置最大元素个数和单个Item大小
        4、多次压入元素比一次性压入元素费时
 */
class XCScrolledList1 : public XCScrollView
{
    
public:
    
    XCScrolledList1();
    ~XCScrolledList1();
    
    virtual bool initWithXCScrolledList(const Size &aContentSize);
    
    /**
     *  创建滑动列表类
     *
     *  @param aContentSize 显示和滑动区域的大小
     *
     *  @return 滑动列表
     */
    static XCScrolledList1 *create(const Size &aContentSize);
    
    /**
     *  (必须设置调用的函数,不然列表是混乱的)设置列表元素大小,最好把元素与元素之间的间距包含在其中.不然则是元素之间就是无缝的
     *
     *  @param aSize 元素大小
     *
     *  @return
     */
    inline void setItemSize(const Size &aSize) {
        m_nMoveStepX = aSize.width;
        m_nMoveStepY = aSize.height;
    }
    
    /**
     *  获取所有元素
     *
     *  @return 元素集合
     */
    inline const Vector<Node *> & getAllItems()
    {
        return _container->getChildren();
    }
    
    /**
     *  设置滑动回调(避免XCScrollViewDelegate继承的麻烦)
     *
     *  @param  绑定回调函数
     *
     *  @return void
     */
    inline void setScrollViewDragCallback(std::function<void(Node *)> aFunc)
    {
        m_fDragCallback = aFunc;
    }
    
    /**
     *  设置最大列元素(如果是横向列表则无需设置, 纵向列表必须设置)
     *
     *  @param aColumn 列个数
     */
    inline void setMaxColumn(int aColumn) {
        m_nMaxColumn = aColumn;
    }
    
    /**
     *  恢复滚动(和暂停滚动成对)
     */
    inline void resumeScroll() {
        m_bStopTouch = false;
    }
    
    /**
     *  暂停滚动(用于拖拽元素出来，防止滚动)
     */
    inline void pauseScroll() {
        m_bStopTouch = true;
    }
    
    /**
     *  移动滑动列表
     *
     *  @param aNumber 移动几格列表
     */
    void moveList(int aNumber);
    
    /**
     *  在列表添加多个元素
     *
     *  @param aItems 元素
     */
    void pushItems(std::vector<Node *> &aItems);
    
    /**
     *  在列表添加一个元素
     *
     *  @param aItem 元素
     */
    void pushItem(Node *aItem);
    
    /**
     *  在列表移除一个元素
     *
     *  @param aItem 元素名称
     */
    void removeItem(const std::string &aName);
    
    /**
     *  在列表移除一个元素
     *
     *  @param aItem 元素
     */
    void removeItem(Node *aItem);
    
    /**
     *  清除滚动层元素
     */
    void cleanupScrollList();
    
    /**
     *  获取在显示在矩形里的元素
     *
     *  @return 元素
     */
    std::vector<Node *> getShowInContentRectItems();
    
    void setAutoHiddenBar(bool aHidden) { m_bAutoHiddenBar = aHidden; }
    
    /**
     *  设置滚动条
     *
     *  @param aBg    滚动条背景图
     *  @param aImage 滚动条
     */
    void setBarImage(Scale9Sprite *aBg, Scale9Sprite *aImage);
    
    
    void setContentOffset(Vec2 offset, bool animated) override;
    
    virtual void addChild(Node* child, int localZOrder, const std::string &name) override;
    
    virtual void addChild(Node * child, int zOrder, int tag) override;
    
protected:
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    
private://Variables
    
    int m_nMaxColumn;
    
    bool m_bStopTouch;
    
    bool m_bDragDone;
    
    float m_fContainerSize;
    
    Node *m_pTouchNode;
    
    std::function<void(Node *)> m_fDragCallback;
    
    Scale9Sprite *m_pBarBg;
    
    Scale9Sprite *m_pImage;
    
    bool m_bAutoHiddenBar;
    
private://method
    
    /**
     *  更新容器大小
     */
    void updateContainerSize();
    
    /**
     *  更新所有元素的坐标
     */
    void updateItemsPosition();
    
    /**
     *  更新滚动条坐标
     */
    void updateBarPosition();
    
    void deaccelerateScrolling(float dt) override;
    
};

#endif