//
//  ContentSpoonNode.h
//  BHUG1064
//
//  Created by maxiang on 5/25/16.
//
//

#ifndef PowderSpoonNode_h
#define PowderSpoonNode_h

#include "MakeBaseNode.h"

class ComponentTouchMoveToTarget;
class ContentSpoonNode : public MakeBaseNode
{
public:
    static ContentSpoonNode* create(const std::string& spoon, const std::string& file);
    bool init(const std::string& spoon, const std::string& file);
    
    void setContent(const std::string& file);
    void setContent(Sprite *content);
    Sprite* getContent();
    void setTapCall(const std::function<void()>& showCallback);

    //拖拽到哪里倾倒, 显示倾倒到碗里的物体回调
    void startDragToWorldPositonPour(const Vec2& worldPoint,
                                     const std::string& particleFile = "\0",
                                     const std::function<void()>& showCallback = nullptr);
    
    //勺子倾倒的角度，默认-30
    CC_SYNTHESIZE(float, _rotateAngel, RotateAngel);
    
    //选中的时候放大, 默认为false
    CC_SYNTHESIZE(bool, _enableTouchZoom, EnableTouchZoom);
    
    //粒子位置，以此节点坐标为准
    CC_SYNTHESIZE(Vec2, _particlePosition, ParticlePosition);
    
    //勺子倒完回到原始位置回调
    std::function<void()> backToOriginalPositionCallback = nullptr;
    
    //是否可以拖动
    void setEnableMove(bool enable);

public:
    ContentSpoonNode():
    _rotateAngel(-30.0),
    _enableTouchZoom(false),
    _compMove(nullptr)
    {};
    virtual ~ContentSpoonNode();
    
protected:
    Sprite *_spoon;
    Sprite *_content;
    
    Vec2 _originalPosition;
    
    ComponentTouchMoveToTarget *_compMove;
};

#endif /* PowderSpoonNode_h */
