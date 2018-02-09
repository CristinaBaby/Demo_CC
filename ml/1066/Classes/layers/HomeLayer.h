//
//  HomeLayer.h
//  FastFood
//
//  Created by huxixiang on 15/8/3.
//
//

#ifndef __FastFood__HomeLayer__
#define __FastFood__HomeLayer__

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"
#include "ToolSprite.h"
#include "Dialog.h"
#include "SoundPlayer.h"
#include "LockScreenLayer.h"
#include "GameData.h"
#include "AdapterScreen.h"
#include "SceneManager.h"

USING_NS_CC;

class HomeLayer : public Layer, public ToolSpriteDelegate, public DialogCallback{
    
public:
    HomeLayer();
    ~HomeLayer();
    
    static Scene* scene();
    virtual bool init() override;
    CREATE_FUNC(HomeLayer);
    
    bool initData();
    bool initLayer();

protected:
    void addAnimation();
    void logoAnimation(float ft);
    
public:
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;

    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchUp(ToolSprite* toolSprite, Touch *pTouch) override;
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch) override;
    
public:
    // 监听Android返回键事件
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event) override;
    virtual void onNegativeClick(void* type) override;
    virtual void onPositiveClick(void* type) override;

private:
    Sprite* m_pBg;

    bool isReturn;
    
    bool m_bIsStartKey;
    
    int m_iCount;
    
    bool m_bIsFlag;
    
    Sprite* m_pWater1;
    Sprite* m_pWater2;
    
    Sprite* m_pTarget;
};

#endif /* defined(__FastFood__HomeLayer__) */
