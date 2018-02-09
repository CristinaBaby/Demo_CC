//
//  KeypadDispatchCenter.cpp
//  HUGS108-IcePrincess Spa
//
//  Created by tanshoumei on 14-7-17.
//
//

#include "depends/component/layerext/KeypadDispatchCenter.h"

static KeypadDispatchCenter* g_sKeypadDispatchCenter = nullptr;

KeypadDispatchCenter* KeypadDispatchCenter::getInstance()
{
    if(!g_sKeypadDispatchCenter)
        g_sKeypadDispatchCenter = new KeypadDispatchCenter;
    return g_sKeypadDispatchCenter;
}

KeypadDispatchCenter::KeypadDispatchCenter()
{
    CCLOG("%s", __FUNCTION__);
    _keyBoardListener = EventListenerKeyboard::create();
    _keyBoardListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event*){
        CCLOG("%s %d onKeyPressed", __FUNCTION__, code);
        switch (code)
        {
            case EventKeyboard::KeyCode::KEY_BACK:
//                keyBackClicked();
                break;
                
            default:
                break;
        }
    };
    _keyBoardListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event*){
        CCLOG("%s %d onKeyReleased", __FUNCTION__, code);
        switch (code)
        {
            case EventKeyboard::KeyCode::KEY_BACK:
                keyBackClicked();
                break;
                
            default:
                break;
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_keyBoardListener, 1);
}

KeypadDispatchCenter::~KeypadDispatchCenter()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_keyBoardListener);
}

void KeypadDispatchCenter::addDelegate(Ref *apDelegate)
{
    _delegates.pushBack(apDelegate);
}

void KeypadDispatchCenter::removeDelegate(cocos2d::Ref *apDelegate)
{
    _delegates.eraseObject(apDelegate);
}

void KeypadDispatchCenter::keyBackClicked()
{
//    CCLOG("%s size:%d", __FUNCTION__, _delegates.size());
    for (int i = _delegates.size() - 1; i >= 0; --i) {
        KeypadDelegate* lpDelegate = dynamic_cast<KeypadDelegate*>(_delegates.at(i));
        if(lpDelegate)
        {
            if(lpDelegate->onBackKeyDown())
                break;
        }
    }
}