//
//  ComponentEat.h
//  BHUG1062
//
//  Created by maxiang on 4/22/16.
//
//

#ifndef ComponentEat_h
#define ComponentEat_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

/** @brief 此组件用于吃各种食物
 */
class ComponentEat : public ComponentBase
{
public:
    CREATE_FUNC(ComponentEat);
    
    /* @brief 设置吃东西的嘴唇形状遮罩 */
    void setEatShape(Sprite *shape);
    void setEatShapeVec(vector<Sprite*> eatVec);

    /* @brief 重置, 重新开始吃 */
    void reset();

    /* @brief 吃完一个食物回调 */
    std::function<void()> finishEatCallback = nullptr;
    
    /* @brief 点击吃回调, 播放声音, 显示粒子效果等 */
    std::function<void(Touch* touch)> eatingCallback = nullptr;
    
    /* @brief 设置判定吃完的不透明像素比率, 值越大不透明像素越多, 说明吃得越少, 
       默认为0.005*/
    CC_SYNTHESIZE(float, _eatRate, EatRate);
    
    /* @brief 设置判定为不透明像素的阀值, 用于处理那些边缘带有点透明但不是完全
       透明的食物, 默认为0 */
    CC_SYNTHESIZE(int, _alphaThreshold, AlphaThreshold);

    /* @brief 是否已经吃完 */
    CC_SYNTHESIZE_READONLY(bool, _isFinishEat, IsFinishEat);
    
    /* @brief 所有设置完成后调用
     */
    virtual void start() override;
    
public:
    ComponentEat():
    _eventListenerTouchOneByOne(nullptr),
    _eatShapeSprite(nullptr),
    _stencilNode(nullptr),
    _clippingNode(nullptr),
    _isFinishEat(false),
    _eatRate(0.005),
    _alphaThreshold(0.0),
    _isMultiShape(false)
    {};
    virtual ~ComponentEat();
    
    virtual bool init() override;
    virtual void onAdd() override;

protected:
    float checkEatFinshed();

    void addTouchEventListener();
    void removeTouchEventListener();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
protected:
    EventListenerTouchOneByOne      *_eventListenerTouchOneByOne;
    ClippingNode   *_clippingNode;
    Node           *_stencilNode;
    Sprite         *_eatShapeSprite;
    vector<Sprite*> _eatShapeVec;
    bool _isMultiShape;
};

#endif /* ComponentEat_h */
