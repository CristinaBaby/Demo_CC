//
//  ComponentCutter.h
//
//
//  Created by maxiang on 3/17/16.
//
//

#ifndef ComponentCutter_h
#define ComponentCutter_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"

/** @brief 此组件用于切各种食物，一般添加到刀精灵上
 */
class ComponentCutter : public ComponentBase
{
public:
    CREATE_FUNC(ComponentCutter);

public:
    enum class CutMode
    {
        kCutModeAuto = 0,   /* 拖到食物上自动切, default */
        kCutModeDrag        /* 手动拖拽切 */
    };
    
    typedef struct _cutModeAutoConfig
    {
        /* @brief 自动切食物的时候, 刀子上下移动距离, 默认为50 */
        float cutterMoveDistance;
        
        /* @brief 自动切食物的时候, 刀子移动速度, 默认为0.3 */
        float cutterMoveDuration;
        
        /* @brief 自动切食物的时候, 上下切多少次, 默认为5 */
        int cutTimes;
        
        _cutModeAutoConfig(float _cutterMoveDistance = 50.0,
                           float _cutterMoveDuration = 0.3,
                           int _cutTimes = 5)
        :cutterMoveDistance(_cutterMoveDistance)
        ,cutterMoveDuration(_cutterMoveDuration)
        ,cutTimes(_cutTimes)
        {
            
        }
    }CutModeAutoConfig;
    
    typedef struct _cutModeDragConfig
    {
        /* @brief 手动切食物的时候, 食物需要切割的高度, 默认为100 */
        float cutSomethingsHeight;
        
        /* @brief 手动切食物的时候, 以+y轴为参考, 刀具的偏移角度, 默认为20 */
        float cutterAngel;
        
        /* @brief 手动切食物的时候, 刀具每次切割向下的偏移角度, 默认为10 */
        float cutterFrameAngel;
        
        /* @brief 手动切食物的时候, 刀具切割完成一次要移动的距离, 默认为10 */
        float cutterMoveDistance;
        
        /* @brief 手动切食物的时候, 刀具滑动的速度, 默认为0.5 */
        float cutterMoveSpeed;
        
        _cutModeDragConfig(float _cutSomethingsHeight = 100.0,
                           float _cutterAngel = 20.0,
                           float _cutterFrameAngel = 10.0,
                           float _cutterMoveDistance = 10.0,
                           float _cutterMoveSpeed = 0.5)
        :cutSomethingsHeight(_cutSomethingsHeight)
        ,cutterAngel(_cutterAngel)
        ,cutterFrameAngel(_cutterFrameAngel)
        ,cutterMoveDistance(_cutterMoveDistance)
        ,cutterMoveSpeed(_cutterMoveSpeed)
        {
            CCASSERT(_cutterAngel > 10.0 && _cutterAngel < 80.0, "道具偏移角度不在范围(10.0, 80.0)内!");
            CCASSERT(_cutterFrameAngel < _cutterAngel, "角度设置有误!");
        }
    }CutModeDragConfig;

    /* @brief 切食物的方式, 默认为kCutModeAuto */
    CC_SYNTHESIZE(CutMode, _cutMode, CutMode);
    
    /* @brief 手动切位移动画播放时，回调 */
    std::function<void()> _dragCutting = nullptr;
    /* @brief 手动切位移动画结束时，回调 */
    std::function<void()> _dragCuttingStop = nullptr;

    
    /* @brief 切好食物后，回调 */
    std::function<void()> cutFinishCallBack = nullptr;

    /* @brief 自动切食物参数配置, 有默认值 */
    CC_SYNTHESIZE(CutModeAutoConfig, _autoCutConfig, AutoCutConfig);
    
    /* @brief 手动切食物参数配置, 有默认值 */
    CC_SYNTHESIZE(CutModeDragConfig, _dragCutConfig, DragCutConfig);
    
    /* @brief 拖动到哪个范围开始切, 以世界坐标系为准
       @param worldPoint 拖动到哪个点触发切的动作
       @param radius 以此点为圆心，此半径内都是触发区域，默认为100
     */
    void setTargetWorldPoint(const Vec2& worldPoint,
                             const float radius = 100);
    
    /* @brief 拖动到指定位置，回调 */
    std::function<void()> onTargetWorldPointCallBack = nullptr;
    
    /* @brief 开始切, 可以直接调用
     */
    virtual void start() override;
    
public:
    ComponentCutter();
    virtual ~ComponentCutter();
    
    bool init() override;
    
protected:
    void doAutoCut();
    void doDragCut();
    
    void addTouchEventListener();
    void removeTouchEventListener();

    void dragCut(float time);
    
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchMoved(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
protected:
    ComponentTouchMoveToTarget      *_compMoveToTarget;
    EventListenerTouchOneByOne      *_eventListenerTouchOneByOne;
    
    Vec2 _cutterOriPosition;
    Vec2 _cutterPosition;
    
    bool _isFinish;
};

#endif /* ComponentCutter_h */
