//
//  ComponentMultiLayer.h
//  BHUG1062
//
//  Created by maxiang on 3/23/16.
//
//

#ifndef ComponentMultiLayer_h
#define ComponentMultiLayer_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

/** @brief 此组件用于食物从下到上分层叠加, 如分层的冰淇淋, 分层的果汁等
 */
class ComponentMultiLayer : public ComponentBase
{
public:
    static ComponentMultiLayer *create(Sprite *maskSprite);
    
public:
    
    /* @brief 是否动画显示, 默认为true */
    CC_SYNTHESIZE(bool, _enableAnimation, EnableAnimation);
    
    /* @brief 动画显示速度, 默认为每秒移动50个像素 */
    CC_SYNTHESIZE(float, _animationSpeed, AnimationSpeed);
    
    /* @brief 设置clipping node的zorder, 默认为0 */
    void setClippingZorder(const int zorder);
    
    /* @brief 设置clipping node的position, 默认为0, 0 */
    void setPosition(const Vec2& position);
    
    /* @brief 添加一层内容
     @param layerSprite 需要添加的精灵
     @param zOrder 精灵的zOrder
     @param positionOffset 基于已经计算好的精灵位置，再做偏移
     @param height 需要显示的精灵高度
     @param finishCallback 显示完一层后, 回调
     */
    void appendLayer(Sprite *layerSprite,
                     const int zOrder,
                     const float height,
                     const std::function<void()>& finishCallback = nullptr,
                     const Vec2& positionOffset = Vec2::ZERO);
    
public:
    ComponentMultiLayer();
    virtual ~ComponentMultiLayer();
    
    bool init(Sprite *maskSprite);
    
protected:
    
    virtual void onAdd() override;
    
protected:
    Sprite          *_layerMaskSprite;
    ClippingNode    *_layerClippingNode;
    float _currentLayerHeight;
    
    bool _isAppending;
};

#endif /* ComponentMultiLayer_h */
