//
//  SpriteButton.h
//  FMAKER3042_Taco
//
//  Created by jianghai on 14-8-29.
//
//

#ifndef __FMAKER3042_Taco__SpriteButton__
#define __FMAKER3042_Taco__SpriteButton__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class SpriteButton : public Sprite
{
public:
    SpriteButton();
    ~SpriteButton();
    /**
     *  create SpriteButton
     *
     *  @param pImage                   Image of Buttom
     *  @param std::function<voidRef*   callback of button
     * @param selectedStastus           if the Button have selectedStastus
     *
     *  @return a SpriteButton object
     */
    static SpriteButton* create(const char* pImage , std::function<void(Ref*)> selector ,bool SwallowTouch = true , bool selectedStastus = true);
    virtual bool init(const char* pImage , std::function<void(Ref*)> selector , bool SwallowTouch = true ,bool selectedStastus = true);
    inline void setEnable(bool enable){
        __enable = enable;
    }
    
    inline void setEnableWithSelectStatus(bool enable){
        __enable = enable;
        if (__enable)
            this->setColor(Color3B::WHITE);
        else
            this->setColor(Color3B::GRAY);
    }
    
private:
    bool onBtnTouchBegan(Touch *touch , Event *unusedEvent);
    void onBtnTouchMove(Touch *touch , Event *unusedEvent);
    void onBtnTouchEnd(Touch *touch , Event *unusedEvent);
    void onBtnTouchCancel(Touch *touch , Event *unusedEvent);
    
    virtual void onExit();
private:
    std::function<void(Ref*)> __pSelector;
    EventListenerTouchOneByOne *__touchListener;
    Point __startPos;
    bool __selected;//是否可选中
    bool __enable;
};
#endif /* defined(__FMAKER3042_Taco__SpriteButton__) */
