//
//  HomeScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"
#include "../layer/ParentGate.h"

class HomeScene : public Layer, public ParentGateDelegate
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HomeScene);
    
    virtual void parentGateBack(std::string &key_word);
    void onDialog(const string& name);
    
protected:
    virtual void onExit() override;
    
private:
    void onButton(Ref* sender, Widget::TouchEventType type);
    void onQuitCallback(const string& name);
};

