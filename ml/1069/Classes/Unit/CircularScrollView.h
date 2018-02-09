//
//  CircularScrollView.h
//  HHZ001_TEST
//
//  Created by huanghanzhi on 24/3/16.
//
//

#ifndef CircularScrollView_h
#define CircularScrollView_h

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class CircularAdapter;
class CircularScrollView : public LayerColor
{
protected:
    CircularScrollView(){};
    virtual ~CircularScrollView(){};
    
public:
    static CircularScrollView* create(const Size &viewSize);
    virtual bool initWithSize(const Size &viewSize);
    
public:
    // 设置间隔
    void setSpace(float space)
    {
        _space = space;
        _doLayoutDirty = true;
    }
    
    // 设置可视区域
    void setViewSize(const Size &viewSize)
    {
        _viewSize = viewSize;
        _doLayoutDirty = true;
    }
    
    // 设置Adapter
    void setAdapter(CircularAdapter* adapter);
    
    // 设置Item点击回调
    void setItemClickBack(function<void(CircularScrollView* circular, long index)> fn)
    {
        _fnClickCall = fn;
    }
    
    void setAdjustCallBack(function<void(Node*)> fn)
    {
        _adjustCall = fn;
    }
    
    // 设置滚动回调
    void setItemMoveCall(function<void()> fn)
    {
        _fnScrollCall = fn;
    }
    
    // 设置是否开启抛投效果
    void setEnableThrow(bool enable)
    {
        _throwEnable = enable;
    }
    
    // 设置是否开启自动校正效果，自动校正的位置
    void setEnableAdjust(bool enable, float adjustPosition)
    {
        _adjustEnable = enable;
        _flagPosition = adjustPosition;
    }
    
    
public:
    // 布局控制
    void doLayout();
    // 更新所有元素位置
    void updateAllItemsPosition();
    // 重新计算ViewSize
    void countNewViewSize();
    // 横向排列
    void horizontalLayout();
    void verticalLayout();
    // 计算边界
    void countBounds();
    
    /**** 滚动控制 ****/
    void moveItems(const Vec2 &delta);
    // 减速滚动（抛投效果）
    void decelerateScrolling(float dt);
    // 自动校正
    void autoAdjustPosition(float dt);
    // 计算需要校正的距离
    Vec2 countAdjustDelta();
    // 滚动到指定item，index从1开始
    void scrollToItem(float index, float duration);
    void scrollToFirst(float duration);
    void scrollToLast(float duration);
    // 自动滚动
    void autoScroll(float dt);
    
public:
    // 添加元素
    void addItem(Node* child);
    void addItem(Vector<Node*> vChildren);
    
    // 移除元素
    void removeItem(Node* node);
    
    // 获取所有元素
    Vector<Node*> getAllItems()
    {
        return _vItems;
    }
    
public:     // Touch事件
    void setTouchEnable(bool isTouchEnable);
    virtual bool onTouchBegan(Touch *touch, Event *event) override;
    virtual void onTouchMoved(Touch *touch, Event *event) override;
    virtual void onTouchEnded(Touch *touch, Event *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
    
    virtual void handleMoveLogic(Touch* touch);
    virtual void handleReleaseLogic(Touch* touch);
    
    Node* getTouchNode(Touch* touch);
    
    // 检测是否应该循环
    void checkItemCirculation(Node* node);
    
    void setDirectClip(bool var){
        _directClip = var;
    }
protected:  // 显示区域
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void beforeDraw();
    void onBeforeDraw();
    void afterDraw();
    void onAfterDraw();
    
    Rect getViewRect();
    
    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;
    
    Rect _parentScissorRect;
    bool _isScissorRestored;
    
protected:
    Size    _viewSize;      // 可视区域大小
    Size    _cellSize;      // 单元格大小(item大小 + 2 * 间隔大小)
    float   _space = 0;     // 间隔大小
    float   _totalWidth;    // 总宽度(单元格 * 元素个数)
    float   _totalHeight;    // 总高度(单元格 * 元素个数)
    bool    _doLayoutDirty = true;
    
    bool    _isLocked = false;
    
    bool    _throwEnable = true;    // 抛投效果
    bool    _adjustEnable = false;  // 校正位置
    Node*   _flagNode = nullptr;    // 标志位置的Node
    float   _flagPosition;          // 校正标志位置
    Vec2    _adjustDelta;           // 校正变化量
    
    bool    _touchIsOut = true;     // 用户touch点已经移出区域
    Vec2    _scrollDistance;        // 滚动距离
    float   _move = 0;
    
    float   _distanceToSpecified;       // flag与指定Item之间的距离
    Node*   _specificNode = nullptr;    // 指定Node
    float   _autoScrollSpeed;           // 自动滚动速率
    
    Node*   _currentTouchNode = nullptr;    // 当前选中的Item
    CircularAdapter*    _adapter = nullptr;
    
    bool _directClip; // false 如果是vertical 圆心在左边 反之在右边
    
    // 边界控制
    float   _boundLeftOrUp;
    float   _boundRightOrBottom;
    
    Vector<Node*>   _vItems;
    EventListenerTouchOneByOne* _touchListener = nullptr;
    
    // 点击回调
    function<void(CircularScrollView*, long)> _fnClickCall = nullptr;
    
    function<void(Node*)> _adjustCall = nullptr;
    
    // 滚动回调
    function<void()> _fnScrollCall = nullptr;
};

class CircularAdapter : public Ref
{
public:
    static CircularAdapter* create(CircularScrollView* circularScrollView);
    
public:
    virtual void setDatas(const Vector<Node*> &vDatas) final;
    virtual void addDatas(const Vector<Node*> &vDatas) final;
    virtual void addData(Node* data) final;
    virtual void refresh() final;
    
    virtual long getCount();
    virtual bool isEmpty();
    
public:
    // item被按下时
    virtual void itemDown(Node* item);
    // item抬手时
    virtual void itemUp(Node* item);
    // 取消时
    virtual void itemCancel(Node* item);
    // 下标从1开始
    virtual void itemView(Node* item, long index);
    virtual void itemRefresh(Node* item, long index);
    
protected:
    CircularAdapter(CircularScrollView* circularScrollView)
    : _circularScrollView(circularScrollView)
    {
        _circularScrollView->setAdapter(this);
    }
    
    CircularScrollView* _circularScrollView = nullptr;
    Vector<Node*>       _vDatas;
};

#endif /* CircularScrollView_h */
