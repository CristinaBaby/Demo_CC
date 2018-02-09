//
//  ShapeButton.h
//  DUM2204_CuteSnowman
//
//  Created by huanghanzhi on 14-11-13.
//
//

#ifndef __DUM2204_CuteSnowman__ShapeButton__
#define __DUM2204_CuteSnowman__ShapeButton__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/**
 @brief     为了选模特那个界面写的。该类用于创建一个不规则的按钮，根据图片的Alpha值来精确判断用户是否点中按钮
 @version   3.1
 @bug       1. 该按钮的触摸优先级是按照zOrder来判断的，还没有写设置优先级函数，后添加的必然覆盖先添加的
 2. 这个Button最好只用于点击，不能像CCMenu中的CCMenuItem一样从一个转移到另一个
 */

class ShapeButton : public Sprite
{
protected:
    ShapeButton();
    ~ShapeButton();
public:
    static ShapeButton* create(const string& fileName);
    
    void initButton();
    
    // touchEvent
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
public:
    void setClickCall(std::function<void(ShapeButton*)> click)
    {
        m_fnClickCall = click;
    }
private:
    Image*          m_image;
    
    bool            m_isTouched;
    
    Color4B         m_color4B;
protected:
    std::function<void(ShapeButton*)>   m_fnClickCall;
};

#endif /* defined(__DUM2204_CuteSnowman__ShapeButton__) */
