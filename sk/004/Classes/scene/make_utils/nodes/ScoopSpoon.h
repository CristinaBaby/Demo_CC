//
//  ScoopSpoon.h
//  bhug1071
//
//  Created by maxiang on 9/7/16.
//
//

#ifndef ScoopSpoon_h
#define ScoopSpoon_h

#include "MakeBaseNode.h"

class ComponentTouchMoveToTarget;
class ScoopSpoon : public MakeBaseNode
{
public:
    static ScoopSpoon* create(const std::string& spoon);
    bool init(const std::string& spoon);
    
    Sprite* setContent(const std::string& content, const Vec2& position = Vec2::ZERO);
    
    std::function<void()> onScroopedCallback = nullptr;
    std::function<void()> onFinishPourCallback = nullptr;
    std::function<void()> backToOriginalPositionCallback = nullptr;

    //拖拽到哪里倾倒, 显示倾倒到碗里的物体回调
    void startDragToWorldPositon(const Vec2& worldPoint);
    
    //倾倒的时候勺子里的食物移动，模拟倒出来的效果
    void setMoveContentOffset(const Vec2& offset);
    
    //舀的时候勺子倾斜动画开关
    CC_SYNTHESIZE(bool, _enableScoopAnimation, EnableScoopAnimation);

    //舀完倒的动画开关
    CC_SYNTHESIZE(bool, _enablePourAnimation, EnablePourAnimation);
    
    //倾倒的时候勺子里面食物移动动画开关
    CC_SYNTHESIZE(bool, _enableContentMoveAnimation, EnableContentMoveAnimation);

    //勺子倾倒的角度，默认-30
    CC_SYNTHESIZE(float, _rotateAngel, RotateAngel);
    
    //选中的时候放大, 默认为false
    CC_SYNTHESIZE(bool, _enableTouchZoom, EnableTouchZoom);
    
    //粒子位置，以此节点坐标为准
    CC_SYNTHESIZE(Vec2, _particlePosition, ParticlePosition);

    CC_SYNTHESIZE(Vec2, _backPosition, BackPosition);

    //是否可以拖动
    void setEnableMove(bool enable);
    
public:
    ScoopSpoon():
    _rotateAngel(-30.0),
    _enableTouchZoom(false),
    _compMove(nullptr),
    _isMoveContent(false),
    _enableScoopAnimation(true),
    _enablePourAnimation(true),
    _enableContentMoveAnimation(true)
    {};
    virtual ~ScoopSpoon();
    
protected:
    Sprite *_spoon;
    Sprite *_content;
    
    Vec2 _originalPosition;
    
    bool _isMoveContent;
    Vec2 _contentMoveOffset;
    
    ComponentTouchMoveToTarget *_compMove;
};


#endif /* ScoopSpoon_h */
