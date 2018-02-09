//
//  JPDifficultyScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"

class JPDifficultyScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(JPDifficultyScene);
    
protected:
    virtual void onExit() override;
    
private:
    void onButton(Ref* sender, Widget::TouchEventType type);
};

