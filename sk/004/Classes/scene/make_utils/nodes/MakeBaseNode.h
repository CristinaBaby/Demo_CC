//
//  MakeBaseNode.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef MakeBaseNode_h
#define MakeBaseNode_h

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "ActionHelper.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class MakeBaseNode : public Node
{
public:
    virtual bool init() override;
    virtual const Size& getContentSize() const override;
    
    //拖动到目的地， 默认为250.0
    CC_SYNTHESIZE(float, _moveToTargetTriggerDistance, MoveToTargetTriggerDistance)
   
    //拖动到指定位置回调, 显示提示旋转倾倒
    std::function<void()> onMoveToTargetCallback = nullptr;
    
public:
    MakeBaseNode():
    _mainContent(nullptr),
    _moveToTargetTriggerDistance(250.0),
    _isAnimation(false)
    {};
    virtual ~MakeBaseNode();
    
protected:
    Sprite *_mainContent;
    bool _isAnimation;

};

#endif /* MakeBaseNode_h */
