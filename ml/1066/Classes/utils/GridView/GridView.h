//
//  GridView.h
//  Framework
//
//  Created by zhangguangzong1 on 15/6/9.
//
//

#ifndef __Framework__GridView__
#define __Framework__GridView__
#include "cocos2d.h"

USING_NS_CC;

/*抛投的衰减速率*/
#define SCROLL_RATE             0.90f
/*点击事件阈值,当move超过该值取消点击*/
#define CLICK_THRESHOLD         10

class Adapter;

class GridView : public Layer {
    
protected:
    GridView();
    ~GridView();
public:
    static GridView* create(const Size& size);
    
    virtual bool initWithSize(const Size &contentSize);
public:
    enum class Direction
    {
        HORIZONTAL ,
        VERTICAL
    };
public:
    /*覆写父类的方法*/
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    
    virtual void update(float) override;
public:
    virtual bool touchBegin(Touch* touch,Event* event);
    virtual void touchMove(Touch* touch,Event* event);
    virtual void touchEnd(Touch* touch,Event* event);
    virtual void touchCancel(Touch* touch,Event* event);
    
    virtual void handleMoveLogic(Touch *touch);
    virtual void handleReleaseLogic(Touch *touch);
public:
    /*对外可调用接口*/
    void setDirection(Direction dir,int row_or_col);
    
    void addGridChild(Node* child);
    /*添加一组Item*/
    void addGridChildren(const Vector<Node*>& vec);
    
    /*移除一个item*/
    void removeItem(Node*);
    
    void setTouchEnabled(bool enabled);
    bool isTouchEnabled() const;
    
    /*下标从1开始！*/
    Node* getItemNode(long index);
    
    /*刷新视图，一般在购买了IAP后会调用到此方法*/
    void refresh();
    
    void setAdapter(Adapter* adapter);
    
    /*设置gridview滚动*/
    void setContentOffset(float offset);
    /*设置自动矫正步长*/
    void setAdjustStep(float step)
    {
        m_fAdjustStep = step;
    }
    
    void setThrowEnable(bool is)
    {
        m_bThrowEnable = is;
    }
    void setAdjustEnable(bool is)
    {
        m_bAdjustEnable = is;
    }
    
    float getMaxOffset();
    
    /*滚动Gridview到顶部或底部*/
    void scrollToLeftAndTop(float duration,bool fromAnother = true);
    void scrollToRightAndBottom(float duration,bool fromAnother = true);
protected:
    /*事件点击控制*/
    Node* getTouchNode(Touch* touch);
    /*移动的距离,超过一定距离取消点击*/
    float m_fMove;
    /*当前选中的Item*/
    Node* m_pCurTouchNode;
    /*下标从1开始！*/
    std::function<void(GridView* grid,long index)>   m_fnClickCall;
protected:
    /*显示区域控制*/
    void beforeDraw();
    void onBeforeDraw();
    
    void afterDraw();
    void onAfterDraw();
    
    Rect getViewRect();
    
    CustomCommand m_oBeforeDrawCommand;
    CustomCommand m_oAfterDrawCommand;
    
    Rect m_oParentScissorRect;
    bool m_bSsissorRestored;
protected:
    /*滚动控制*/
    void moveContainer(const Vec2& move);

    /*减速滚动(抛投效果)*/
    void decelerateScrolling(float dt);
    
    /*是否超出最大或最小偏移量*/
    bool isOutOfOffset();
    
    /*计算需要自动调整的距离*/
    void countAdjustDistance();
    
    void autoScroll(float);
    
    bool    m_bAutoScrolling;
    /*需要调整的距离*/
    float   m_fAdjustDistance;
    /*需要调整的步长,默认一页宽度*/
    int   m_fAdjustStep;
    
    float   m_fSpeed;
    /*两次move之间的滚动距离*/
    Vec2    m_oScrollDistance;
    /*手指开始触摸时的偏移量*/
    Vec2    m_oBeginTouch;
    /*获取自动调整的距离,默认按页调整*/
    float getAdjustDistance();
    
    /*抛投与自动矫正是否开启*/
    bool    m_bThrowEnable;
    bool    m_bAdjustEnable;
    
    /*======Scroll ContentOffset相关======*/
    /*是否设置偏移量*/
    bool    m_bScrollOffset;
    /*update计数器,计算update调用次数*/
    int     m_iUpdateCounter;
    /*设置偏移滚动时间*/
    float   m_fScrollDuration;
    /*是否从另外一端滚动过来*/
    bool    m_bFromAnother;
    /*偏移滚动方向*/
    enum class OffsetDirection
    {
        LeftAndTop,
        RightAndBottom
    };
    
    OffsetDirection m_eOffsetDir;
    
    void scrollToLeftAndTop();
    void scrollToRightAndBottom();
    
    void scrollContentOffset();
protected:
    /*布局控制*/
    void doLayout();
    /*更新所有元素位置*/
    void updateAllItemsPosition();
    /*横行排布*/
    void horizontalLayout();
    /*纵向排布*/
    void verticalLayout();
    /*获取所有元素布局后的总宽度和总高度*/
    float getTotalWidth();
    float getTotalHeight();
    
    /*根据滚动方向计算行列数*/
    void countRowAndCol();
    
    bool    m_bdoLayoutDirty;
    
    long    m_iRow;//行
    long    m_iCol;//列
    
    /*最大偏移量*/
    float   m_fMaxOffset;
    /*最小偏移量*/
    float   m_fMinOffset;
    
    
    /*布局属性*/
    float m_fMarginTop;
    float m_fMarginLeft;
    float m_fMarginRight;
    float m_fMarginBottom;
    float m_fHSpace;
    float m_fVSpace;
protected:
    /*数据绑定相关*/
    Adapter*    m_pAdapter;
public:
    /*GridView的显示区域尺寸*/
    Size        m_oViewSize;
    /*GridView中的容器层，所有显示的item均放在上面*/
    Layer*      m_pContainer;
    /*GridView中的所有Item对象*/
    Vector<Node*>   m_vItems;
    /*点击事件监听listener*/
    EventListenerTouchOneByOne* m_pTouchListener;
    /*滚动方向*/
    Direction   m_eDirection;
    
//    /*Test LayerColor*/
//    LayerColor* m_pBgColor;
public:
    void setOnItemClick(std::function<void(GridView* grid,long index)> fn)
    {
        m_fnClickCall = fn;
    }
    
    void setMarginTop(float margin)
    {
        m_fMarginTop = margin;
        m_bdoLayoutDirty = true;
    }
    void setMarginBottom(float margin)
    {
        m_fMarginBottom = margin;
        m_bdoLayoutDirty = true;
    }
    void setMarginRight(float margin)
    {
        m_fMarginRight = margin;
        m_bdoLayoutDirty = true;
    }
    void setHSpace(float space)
    {
        m_fHSpace = space;
        m_bdoLayoutDirty = true;
    }
    void setVSpace(float space)
    {
        m_fVSpace = space;
        m_bdoLayoutDirty = true;
    }
};

class Adapter : public Ref
{
protected:
    Adapter(GridView* grid):m_pGrid(grid)
    {
        m_pGrid->setAdapter(this);
    }
public:
    static Adapter* create(GridView* grid)
    {
        Adapter* adapter = new Adapter(grid);
        if(adapter)
        {
            adapter->autorelease();
            return adapter;
        }
        CC_SAFE_RELEASE_NULL(adapter);
        return nullptr;
    }
public:
    virtual void setDatas(const Vector<Node*>& datas) final;
    virtual void addDatas(const Vector<Node*>& datas) final;
    virtual void addData(Node* data) final;
    virtual void refresh() final;
    
    virtual long getCount();
    virtual bool isEmpty();
public:
    /*item被按下时*/
    virtual void itemDown(Node*);
    /*抬手时*/
    virtual void itemUp(Node*);
    /*取消时*/
    virtual void itemCancel(Node*);
    /*下标从1开始*/
    virtual void itemView(Node* item,long index);
    /*下标从1开始*/
    virtual void itemRefresh(Node* item,long index);
    
protected:
    GridView*       m_pGrid;
    Vector<Node*>   m_vData;
};
#endif /* defined(__PuzzleTest__GridView__) */
