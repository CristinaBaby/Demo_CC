//
//  KeyBackEvent.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 12/26/14.
//
//

#include "KeyBackEvent.h"
bool KeyBackEvent:: onBackFalag = false;

void KeyBackEvent::addBackEvent(std::function<bool()> callBack, Node * node){
    //num++;
    auto backEvent = EventListenerCustom::create("onBack",[callBack](EventCustom *t){
        if(!onBackFalag)
            onBackFalag = callBack();
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backEvent, node);
}

KeyBackEvent::KeyBackEvent(){
     backCallBack = EventListenerKeyboard::create();
    backCallBack->onKeyReleased = CC_CALLBACK_2(KeyBackEvent::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backCallBack, 1);
}

void KeyBackEvent::removeEvent(Node* node) {
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
}

KeyBackEvent::~KeyBackEvent(){
    Director::getInstance()->getEventDispatcher()->removeEventListener(backCallBack);
}

void KeyBackEvent::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
        onBackFalag = false;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("onBack");
    }
}