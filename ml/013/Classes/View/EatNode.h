//
//  EatNode.h
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#ifndef __Make__EatNode__
#define __Make__EatNode__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class EatNode: public ClippingNode
{
public:
    CREATE_FUNC(EatNode);
    bool init();
    void allowEat();
    void forbidEat();
    void reset();
private:
    Node* stencil_;
    EventListenerTouchOneByOne* listener;
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);

};

#endif /* defined(__Make__EatNode__) */
