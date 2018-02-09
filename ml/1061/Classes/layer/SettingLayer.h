//
//  SettingLayer.h
//  FMAKER033
//
//  Created by wuguiling on 9/11/14.
//
//

#ifndef __FMAKER033__SettingLayer__
#define __FMAKER033__SettingLayer__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "BaseLayer.h"
#include "HomeLayer.h"
using namespace cocos2d::extension;
USING_NS_CC;

class SettingLayer:public BaseLayer{
    
    
public:
    CREATE_FUNC(SettingLayer);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    void showInParent(HomeLayer* parentNode);
    
    
private:
    
    //从屏幕上方掉落下来
    void _easeIn();
    //往屏幕下方出去
    void _easeOut();
    void _easeOutDone();
    void _onCloseClickedCallBack(Ref* ref, Control::EventType controlEvent);
    void _onRestoreClickedCallBack(Ref* ref, Control::EventType controlEvent);
    void _onSettingCallback(Ref* ref, Control::EventType controlEvent);
    void musicCheckBoxCallback(Ref* ref,CheckBox::EventType checkEvent);
    void soundCheckBoxCallback(Ref* ref,CheckBox::EventType checkEvent);

private:
    Sprite* m_pBgSprite;
    CheckBox* m_pSoundCheckBox;
    CheckBox* m_pMusicCheckBox;

    ControlSlider* m_pSoundSlider;
    ControlSlider* m_pMusicSlider;
    ControlButton* m_pRestore;
    HomeLayer* m_Parent;
};

#endif /* defined(__FMAKER033__SettingLayer__) */
