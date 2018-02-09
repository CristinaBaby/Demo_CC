//
//  IceContainerNode.h
//  BHUG1066
//
//  Created by maxiang on 6/24/16.
//
//

#ifndef IceContainerNode_h
#define IceContainerNode_h

#include "MakeBaseNode.h"

#define CONTAINER "res/snow_cone/3-6/ice_box2.png"
#define ICE_PIECE "res/snow_cone/3-6/ice_box2_ice0.png"

class IceContainerNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceContainerNode);
    virtual bool init() override;
    
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    //拖动到指定位置回调, 显示提示旋转倾倒
    std::function<void()> onMoveToTargetCallback = nullptr;
    
public:
    IceContainerNode()
    {};
    virtual ~IceContainerNode();
protected:
    void accelerateRotateDone();
 
protected:
    Sprite *_salt;
    std::function<void()> _showContentCallback;
    std::vector<Sprite *> _iceVector;
    Vec2 _originalPosition;
};

#endif /* IceContainerNode_h */
