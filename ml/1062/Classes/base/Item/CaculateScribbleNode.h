//
//  CaculateScribbleNode.h
//  HappyParty
//
//  Created by luotianqiang1 on 15/9/28.
//
//

#ifndef __HappyParty__CaculateScribbleNode__
#define __HappyParty__CaculateScribbleNode__

#include "cocos2d.h"
USING_NS_CC;
class CaculateScribbleNode: public Node{
public:
    CREATE_FUNC(CaculateScribbleNode);
    CC_SYNTHESIZE(Color4F, _drawColor, DrawColor);
    CC_SYNTHESIZE(float, _drawRadius,DrawRadius);
    void setEnable(bool flag);
    ~CaculateScribbleNode();
     void paint(Vec2 ,Vec2 );
    void clear();
protected:
    CaculateScribbleNode();
    bool init();
    DrawNode *_drawNode;
    EventListenerTouchOneByOne* _lis;
    bool isEnbale;
    virtual void onEnterTransitionDidFinish();
    void   onExitTransitionDidStart();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch *pTouch, Event *pEvent);
   
public:
    std::function<void(CaculateScribbleNode*,Vec2 _pos)> _moveCallback;
};

#endif /* defined(__HappyParty__CaculateScribbleNode__) */
