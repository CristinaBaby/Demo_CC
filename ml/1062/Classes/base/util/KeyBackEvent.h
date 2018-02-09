//
//  KeyBackEvent.h
//  MyCppGame
//
//  Created by luotianqiang1 on 12/26/14.
//
//

#ifndef __MyCppGame__KeyBackEvent__
#define __MyCppGame__KeyBackEvent__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class KeyBackEvent : public Ref{
public:
    KeyBackEvent();
    ~KeyBackEvent();
    static KeyBackEvent* getInstance() {
        static KeyBackEvent backEvent;
        return &backEvent;
    }
    void addBackEvent(std::function<bool()> callBack, Node* node);
    void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent);
    void removeEvent(Node* node);
private:
    static bool onBackFalag;
    EventListenerKeyboard *backCallBack;
};

#endif /* defined(__MyCppGame__KeyBackEvent__) */
