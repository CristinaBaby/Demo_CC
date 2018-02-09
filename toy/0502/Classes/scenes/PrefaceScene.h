//
//  PrefaceScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"

class PrefaceScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PrefaceScene);
    
protected:
    
private:
    void onButton(Ref* sender, Widget::TouchEventType type);
};

