//
//  BaseLayer.h
//  Make
//
//  Created by QAMAC01 on 15-1-19.
//
//

#ifndef __Make__BaseLayer__
#define __Make__BaseLayer__

#include <stdio.h>
#include "Dialog.h"
#include "DialogCallback.h"
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "AdsLoadingLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


class BaseLayer :public Layer,public DialogCallback
{
public:
    CREATE_FUNC(BaseLayer);
    bool init();
    void setBackground(std::string pzFileName);
    void setHint(std::string pzFileName);
    virtual void nextBtnClicked(){};
    virtual void resetBtnClicked(){};
    virtual void resetGameBtnClicked(){};
    virtual void backBtnClicked(){};
    virtual void preBtnClicked(){};
    virtual void favBtnClicked(){};
    virtual void newOneBtnClicked(){};
    virtual void reeatClicked(){};
    
    ui::Button* nextBtn_;
    ui::Button* resetBtn_;
    ui::Button* resetGameBtn_;
    ui::Button* backBtn_;
    ui::Button* favBtn_;
    ui::Button* newOneBtn_;
    ui::Button* moreBtnInShare_;
    
    virtual void onEnter();
    virtual void onExit();
    Sprite* hintSprite_;
    bool shouldShowCross_;

    EventListenerTouchOneByOne* listener_;
    void initListener();
    void touchEvent(Ref *obj ,ui::Widget::TouchEventType type);

public:
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    

private:
    
    void initBtn();
    void refreshBtn();
    
    Sprite* bg_;
    


};

#endif /* defined(__Make__BaseLayer__) */
