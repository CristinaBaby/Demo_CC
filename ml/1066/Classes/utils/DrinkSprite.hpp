//
//  DrinkSprite.hpp
//  MyTest
//
//  Created by huxixiang on 15/11/17.
//
//

#ifndef DrinkSprite_hpp
#define DrinkSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class DrinkSprite : public Sprite {
    
public:
    DrinkSprite();
    virtual ~DrinkSprite();
    
    /**
     *  @param      cupDownPath    杯子后半部分
     *  @param      maskPath       黑色遮罩
     *  @param      drinkPath      饮料
     *  @param      cupUpPath      杯子前半部分
     *  @param      tempPath       杯口空白精灵
     */
    static DrinkSprite* create(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath);
    virtual bool init(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath);
    
    bool initUI(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath);
    
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    void onStartBtnCallback(Ref *pSender, Widget::TouchEventType type);
    void drinkCallback();
    
public:
    std::function<void()>   m_fnDrinkBeginCallback;
    std::function<void()>   m_fnDrinkEndCallback;
    
private:
    Sprite* m_pDrink;
    
    Button* m_pStartBtn;
    
    bool m_bIsStartDrink;
    
    int m_iDrinkSoundId;
    bool m_bIsSoundPlay;
};

#endif /* DrinkSprite_hpp */
