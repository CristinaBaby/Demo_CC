//
//  LoadingLayer.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/14.
//
//

#ifndef __ColorBook0818__SettingLayer__
#define __ColorBook0818__SettingLayer__

#include "cocos2d.h"
#include "cocosgui.h"
#include "ArmatureManager.h"

USING_NS_CC;

class LoadingLayer : public Layer{
public:
    static LoadingLayer* create();
    virtual bool init();
    
    ActionInterval* setVisibleTrue(float time,Sprite * sp);
    ActionInterval* setVisibleFalse(float time);
    void setVisibleFalse();
    void setArmature(ArmatureManagerPtr armature) { _armature = armature; };
    void pageViewEvent(Ref *pSender, PageView::EventType type);
private:
    std::vector<Sprite*> _dots;
    Sprite*              _dot_1;
    Sprite*              _dot_2;
    Sprite*              _dot_3;
    Sprite*              _dot_4;
    RepeatForever*       _action;
    Sprite*              _loadingBg;
    ui::ImageView*       _prompt;
    Layout*              _root;
    ArmatureManagerPtr   _armature = nullptr;
    Sprite*              _dot;
};

#endif /* defined(__ColorBook0818__SettingLayer__) */
