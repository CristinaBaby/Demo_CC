//
// HomeScene.h
// ANIM3203
//
// Created by dengcheng on 15/9/10.
//
//

#ifndef __ANIM3203__HomeScene__
#define __ANIM3203__HomeScene__

#include "cocos2d.h"
#include "BaseScene.h"
#include "SwitchButton.h"

USING_NS_CC;

class HomeScene : public BaseScene
{
public:
    HomeScene();
    ~HomeScene();

    static HomeScene* create();
    virtual bool init();
    
    void showAds() {
        m_bCross = true;
    }

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    SwitchButton *m_pSound;
    
    bool m_bCross;
    
private://method
    
    void onButtonCallback(Ref *aButton, Widget::TouchEventType aType);
    
};

#endif /* defined(__ANIM3203__HomeScene__) */