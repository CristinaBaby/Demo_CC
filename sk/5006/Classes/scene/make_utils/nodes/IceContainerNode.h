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
class ComponetFade;
class IceContainerNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceContainerNode);
    virtual bool init() override;
    
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    //拖动到指定位置回调, 显示提示旋转倾倒
    std::function<void()> onMoveToTargetCallback = nullptr;
    
    void pourWaterIn(int index);
    
    void showRotateCallback(float rotate, const std::function<void()>& callback);
    void showIce();
    
    vector<Vec2> _icePosVec;
    void freeze(float duration);
    void resetFreeze();
    
    string getIcePath();
public:
    IceContainerNode()
    {};
    virtual ~IceContainerNode();
protected:
    void accelerateRotateDone();
 
protected:
    std::function<void()> _showContentCallback;
    std::vector<Sprite *> _iceVector;
    Vec2 _originalPosition;
    vector<ComponetFade*> _fadeVec;
};

#endif /* IceContainerNode_h */
