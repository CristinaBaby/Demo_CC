//
//  ComponentCycleScroll.h
//
//
//  Created by maxiang on 3/30/16.
//
//

#ifndef ComponentCycleScroll_h
#define ComponentCycleScroll_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "ui/CocosGUI.h"
using namespace ui;

/* 忽略一次Touch事件，用于覆盖在此组件上的Button响应的时候屏蔽组件响应 */
#define CSIgnoreTouchOnceNotification       "CSIgnoreTouchOnceNotification"

/* 忽略所有Touch事件 */
#define CSIgnoreAllTouchNotification        "CSIgnoreAllTouchNotification"

/* 取消忽略所有Touch事件 */
#define CSCancelIgnoreAllTouchNotification  "CSCancelIgnoreAllTouchNotification"

/* @brief 此组件用于首尾相接，循环滑动的scroll */
class ComponentCycleScroll : public ComponentBase
{
public:
    CREATE_FUNC(ComponentCycleScroll);
    
    /* @brief 每个item的间隔, 默认为150 */
    CC_SYNTHESIZE(float, _margin, Margin);
    
    /* @brief 是否开启手指拖动item功能, 默认为true */
    CC_SYNTHESIZE(bool, _enableMoveItem, EnableMoveItem);
    
    /* @brief Touch事件优先级, 此类使用的是FixedPriority, 默认为-1 */
    CC_SYNTHESIZE(int, _fixedPriority, FixedPriority)
    
    /* @brief 忽略所有Touch事件 */
    CC_SYNTHESIZE(bool, _ignoreAllTouch, IgnoreAllTouch)
    
    CC_SYNTHESIZE(bool, _scrollSoundEable, ScrollSoundEable);
    
    
    /* @brief 所有设置完成后调用
     */
    virtual void start() override;
    
    /* @brief 使用完如果不清除，则强烈建议调用stop停止使用，因为次类里面注册的Touch事件优先级较高，
     如果不stop，在场景中可能导致无法意料的Touch屏蔽
     */
    virtual void stop() override;
    
    /* @brief 设置scroll的大小
     @param size scroll的大小，注意，是显示在屏幕上的区域，不是内部container大小
     */
    void setCycleScrollSize(const Size& size);
    
    /* @brief 设置scroll的滑动方向, 默认为水平方向
     @param dir scroll的滑动方向
     */
    void setCycleScrollDirection(ui::ScrollView::Direction dir);
    
    /* @brief item可以拖动的方向 */
    enum
    {
        DIRECTION_TOP = (0x1 << 0),
        DIRECTION_BOTTOM = (0x1 << 1),
        DIRECTION_LEFT = (0x1 << 2),
        DIRECTION_RIGHT = (0x1 << 3),
        
        DIRECTION_HORIZONTAL = (DIRECTION_LEFT | DIRECTION_RIGHT),
        DIRECTION_VERTICAL = (DIRECTION_TOP | DIRECTION_BOTTOM)
    };
    /* @brief 设置item可以拖动的方向, 默认为向上拖动, 不可以用错误的组合, 如水平的scroll
     不能设置为左右拖动, 同一方向可以组合, 比如可以同时上下拖动
     @param dir item可以拖动的方向
     */
    void setItemMoveDirection(uint32_t directions);
    
    /* @brief 添加一个item
     @param item 需要添加的item
     */
    void pushCustomItem(Widget *item);
    
    /* @brief 获得scroll上的所有item
     */
    std::vector<Widget*> getItems() const {return _itemsVector;};
    
    /* @brief 是否可以滑动, 默认为true */
    void setScrollEnable(bool enable);
    bool isScrollEnable() const {return _scrollEnable;};
    
    /* @brief ScrollView是否正在滑动中 */
    bool isScrolling() const {return _isScrollMoving;};
    
    /* @brief 滑动scroll，多用在启动时展示用
     @param duration 时间
     @param offset 滑动距离
     */
    void scrollBy(const float duration,
                  const Vec2& offset,
                  const std::function<void()>& callback);
    
    /* @brief 当一个item被判定为拖动时回调, 返回false表示不处理这次拖动, 后面的touchMove事件不会再调用 */
    std::function<bool(Widget *item, Touch *touch)> itemWillMoveCallback = nullptr;
    
    /* @brief 拖动一个item回调 */
    std::function<void(Widget *item, Touch *touch)> itemTouchMoveCallback = nullptr;
    
    /* @brief 拖动完一个item回调 */
    std::function<void(Widget *item, Touch *touch)> itemTouchEndCallback = nullptr;
    
    /* @brief 点击一个item回调 */
    std::function<void(Widget *item, Touch *touch)> itemClickCallback = nullptr;
    
    /* @brief scroll滑动的时候回调, 播放音效等 */
    std::function<void()> scrollMoveCallback = nullptr;
    
public:
    virtual bool init() override;
    virtual void onAdd() override;
    ComponentCycleScroll();
    virtual ~ComponentCycleScroll();
    
protected:
    void resetLink();
    
    void scrollMove();
    
    void timerUpdate(float time);
    
    void setInnerContainerSize(const Size& size);
    
    void horizontalScrollMove();
    void verticalScrollMove();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void addTouchEventListener();
    void removeTouchEventListener();
    
protected:
    EventListenerTouchOneByOne *_eventListenerTouchOneByOne;
    
    ui::ScrollView *_scrollView;
    std::vector<Widget*> _itemsVector;
    
    typedef struct _Link
    {
    public:
        Widget *item;
        int index;
        _Link *previous;
        _Link *next;
    }Link;
    std::vector<Link*> _linksVector;
    
    /* 是否已经添加了Touch屏蔽事件监听，整个运行期间保持，不释放 */
    static bool _hasAddListener;
    
    /* 存放所有创建的cycle scroll */
    static std::vector<ComponentCycleScroll*>* _cycleScrollsVector;
    
    /* scroll container的总长度 */
    float _maxContainerLenght;
    
    /* scroll是否正在滑动 */
    bool _isScrollMoving;
    
    /* scroll是否可以滑动 */
    bool _scrollEnable;
    
    /* 忽略一次Touch事件 */
    bool _ignoreTouchOnce;
    
    /* 按住的item */
    Widget *_touchedItem;
    
    /* 是否正在拖动选中的item */
    bool _isMovingItem;
    
    /* scroll的setContentSize方法会调用eventCallback, 导致innerContainer每次都会设置为
     起始点
     */
    bool _hasSetInnerPosition;
    
    uint32_t _directionsFlag;
    
    bool _moveScrollContainer;
    
    float _scrollStopMovingTime;
    
    bool _isReady;
    
    /* 忽略后续的TouchMove事件回调 */
    bool _ignoreTouchMove;
    
    float _timeInterval;
    
    Vec2 _innercontainerPos;
};

#endif /* ComponentCycleScroll_h */
