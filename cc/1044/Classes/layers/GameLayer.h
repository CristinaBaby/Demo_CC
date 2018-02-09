//
//  GameLayer.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#ifndef __OreoMaker__GameLayer__
#define __OreoMaker__GameLayer__
#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseLayer.h"
#include "StepLayer.h"

USING_NS_CC;
using namespace std;

class StepLayer;
class DecorationLayer;
class GameLayer : public BaseLayer{
    
public:
    GameLayer();
    ~GameLayer();
    
public:
    CREATE_FUNC(GameLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void showBanner(Ref* r);
    void hideBanner(Ref* r);
    
    void initUI();
public:
    void onBtnCallBack(Ref* sender);
    void changeBackGround(string bgPath);
    void setNextButtonEnable(bool isVisible);
    void showNextButton(bool isVisible);
    void doFirstStep(StepLayer* firstStep);
    void doNextStep(StepLayer* nextStep);
    void OperateEndToDecoration(BaseLayer* dec);
    void addToBGSprite(Node* node,int zOrder = 0);
    
    void popDecorationLayer();
  
    void addNextSpritePosY(float y);
    
    void nextButtonClicked(StepLayer* nextStep);
    
    void setEatCreamSceneNum(int num);
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
public:
//    Menu* m_pMenu;
//    MenuItemSprite* m_pNextItemSprite;
    ui::Button* m_pNextItemSprite;
    Sprite* m_pBgSprite;
    StepLayer* m_pNextStepLayer;
    StepLayer* m_pCurrentStepLayer;
    
    BaseLayer* m_pDecorationLayer;
    
    bool isOperateEnd;
    bool isDecorateEnd;
    
    float m_fAddPosY;
    
    //进入不同的Scene
    int m_iEatSceneNum;
    bool m_bNextAction;
    
    bool m_bNextButtonIsShow;
};



#endif /* defined(__OreoMaker__GameLayer__) */
