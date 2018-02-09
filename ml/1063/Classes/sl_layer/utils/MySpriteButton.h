//
//  MyMySpriteButton.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#ifndef MyMySpriteButton_hpp
#define MyMySpriteButton_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class MySpriteButton : public Sprite
{
public:
    MySpriteButton();
    ~MySpriteButton();
    /**
     *  create MySpriteButton
     *
     *  @param pImage                   Image of Buttom
     *  @param std::function<voidRef*   callback of button
     * @param selectedStastus           if the Button have selectedStastus
     *
     *  @return a MySpriteButton object
     */
    static MySpriteButton* create(const char* pImage , std::function<void(Ref*)> selector ,bool SwallowTouch = true , bool selectedStastus = true);
     static MySpriteButton* createTexture(Texture2D* texture , std::function<void(Ref*)> selector ,bool SwallowTouch = true , bool selectedStastus = true);
    virtual bool init(const char* pImage , std::function<void(Ref*)> selector , bool SwallowTouch = true ,bool selectedStastus = true);
    virtual bool initTexture(Texture2D* texture , std::function<void(Ref*)> selector , bool SwallowTouch = true ,bool selectedStastus = true);
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
    
    void setSelectorTouchBegan(std::function<void(Ref*)> selector);
    
private:
    bool onBtnTouchBegan(Touch *touch , Event *unusedEvent);
    void onBtnTouchMove(Touch *touch , Event *unusedEvent);
    void onBtnTouchEnd(Touch *touch , Event *unusedEvent);
    void onBtnTouchCancel(Touch *touch , Event *unusedEvent);
    
    virtual void onExit();
private:
    std::function<void(Ref*)> __pSelectorTouchBegan;
    std::function<void(Ref*)> __pSelectorTouchEnd;
    EventListenerTouchOneByOne *__touchListener;
    Point __startPos;
    bool __selected;//是否可选中
    bool __enable;
};
#endif /* MyMySpriteButton_hpp */
