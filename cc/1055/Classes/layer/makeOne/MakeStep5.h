//
//  MakeStep55.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/5.
//
//

#ifndef MakeStep55_hpp
#define MakeStep55_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

using namespace std;

//make步骤的模板
class MakeStep5 : public BaseLayer  {
public:
    MakeStep5();
    ~MakeStep5();
public:
    bool init();
    CREATE_FUNC(MakeStep5);
    CREATE_SCENE(MakeStep5);
public:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    void onTouchMove(Node* node,MoveLisenter* lisenter,Touch* touch);
    
    int checkIsOver();
    int checkIsPushOver();
    void overShow();
    
    bool onPushTouch(Touch *pTouch, Event *pEvent);
    void onPushMove(Touch *pTouch, Event *pEvent);
    
private:
    
    Sprite* m_pScooper;
    Sprite* m_pBowl;
    Sprite* m_pPlate;
    
    Vector<Node*> m_vPlateRect;
    Vector<Sprite*> m_vPlateCookie;
    Vec2    _spoonPos;
    Sprite* m_pPushTips;
    Sprite* m_pMoveTips;
    Sprite* m_pHand;
    Sprite* m_pTitle;
    bool m_bIsOver;
};

#endif /* MakeStep55_hpp */
