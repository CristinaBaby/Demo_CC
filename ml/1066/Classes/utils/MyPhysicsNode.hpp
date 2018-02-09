//
//  MyPhysicsNode.hpp
//  MyTest
//
//  Created by huxixiang on 16/5/4.
//
//

#ifndef MyPhysicsNode_hpp
#define MyPhysicsNode_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MyPhysicsNode : public Node {
public:
    MyPhysicsNode();
    ~MyPhysicsNode();
    
    static MyPhysicsNode* create(const std::string& filename);
    virtual bool init(const std::string& filename);
    bool initData();
    bool initLayer(const std::string& filename);
    
public:
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void TouchCancelled(Touch *pTouch, Event *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    Sprite* getNode();
    
    CC_SYNTHESIZE(Vec2, m_oTouchPoint, TouchPoint);
    
private:
    std::unordered_map<int, Node*> _mouses;

    Sprite* m_pNode;
    

};

#endif /* MyPhysicsNode_hpp */
