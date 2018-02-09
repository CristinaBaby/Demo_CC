//
//  KeypadDispatchCenter.h
//  CodeBase
//
//  Created by tanshoumei on 14-7-17.
//
//

#ifndef CODEBASE_COMPONENT_LAYEREXT_KEYPADDISPATCHCENTER_H_
#define CODEBASE_COMPONENT_LAYEREXT_KEYPADDISPATCHCENTER_H_
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class KeypadDelegate
{
public:
    // The back key clicked
    virtual bool onBackKeyDown() { return false;};
};

class KeypadDispatchCenter: public Ref
{
public:
    static KeypadDispatchCenter* getInstance();
    
    
    void addDelegate(Ref* apDelegate);
    void removeDelegate(Ref* apDelegate);
    
    // The back key clicked
    virtual void keyBackClicked();
    
    // The menu key clicked. only available on wophone & android
    virtual void keyMenuClicked() {};
protected:
    KeypadDispatchCenter();
    ~KeypadDispatchCenter();
    
    Vector<Ref*> _delegates;
    EventListenerKeyboard* _keyBoardListener;
};

#endif /* defined(CODEBASE_COMPONENT_LAYEREXT_KEYPADDISPATCHCENTER_H_) */