//
//  ComponentDrink.h
//  BHUG1062
//
//  Created by maxiang on 3/23/16.
//
//

#ifndef ComponentDrink_h
#define ComponentDrink_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

/** @brief 此组件用于喝果汁, 可乐等
 */
class ComponentDrink : public ComponentBase
{
public:
    /* @brief 创建一个Drink组件
       @param drinkMask drink内容的遮罩
     */
    static ComponentDrink *create(Sprite *drinkMask);
    
public:
    
    /* @brief 喝的速度, 默认为每帧移动一个像素1.0 */
    CC_SYNTHESIZE(float, _drinkSpeed, DrinkSpeed);
    
    /* @brief 点击触发喝的区域, 世界坐标系为准, 默认为全屏幕*/
    CC_SYNTHESIZE(Rect, _triggerDrinkArea, TriggerDrinkArea);
    
    /* @brief 长按多少时间开始喝, 默认为0.2 */
    CC_SYNTHESIZE(float, _triggerDrinkTime, TriggerDrinkTime);
    
    /* @brief 检测喝完的高度标准, 默认为遮罩高度 */
    CC_SYNTHESIZE(float, _finishDrinkHeight, FinishDrinkHeight);
    
    /* @brief 开始喝回调 */
    std::function<void()> onStartDrinkingCallBack = nullptr;
    
    /* @brief 停止喝回调 */
    std::function<void()> onStopDrinkingCallBack = nullptr;
    
    /* @brief 喝完回调 */
    std::function<void()> onFinishDrinkCallBack = nullptr;
    
    /* @brief 添加一个喝的精灵
       @param drinkSprite drink内容
     */
    void setDrinkSprite(Sprite *drinkSprite);
    
    /* @brief 设置是否为自动喝, 自动喝此时touch无效, 默认为false */
    void setEnableAutoDrink(bool enable);
    
    /* @brief 重置
     */
    void reset();
    
    /* @brief 开始喝, 如果是自动喝, 则开始自动喝; 如果是触摸喝, 只是初始化,
     需要手指touch触发
     */
    virtual void start() override;
    
    /* @brief 停止喝, 如果要继续喝, 必须再次调用start
     */
    virtual void stop() override;
    
public:
    ComponentDrink();
    virtual ~ComponentDrink();
    
    bool init(Sprite *drinkMask);
    
protected:
    
    virtual void onAdd() override;
    
    void drink();
    
    void finishDrink();
    
    void drinkTimer(float time);
    
    void addTouchEventListener();
    void removeTouchEventListener();
    
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchMoved(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
protected:
    Sprite          *_drinkMask;
    Sprite          *_drinkSprite;
    ClippingNode    *_drinkClippingNode;
    EventListenerTouchOneByOne      *_eventListenerTouchOneByOne;
    Vec2 _drinkSpriteOriPosition;
    
    float _touchingTime;
    
    bool _isDrinking;
    bool _isTouching;
    bool _isDrinkOver;
    bool _isAutoDrink;
};

#endif /* ComponentDrink_h */
