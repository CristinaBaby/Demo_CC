//
//  MakeOtherStep5.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#ifndef MakeOtherStep5_hpp
#define MakeOtherStep5_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeOtherStep5 : public BaseLayer  {
public:
    MakeOtherStep5();
    ~MakeOtherStep5();
public:
    bool init();
    CREATE_FUNC(MakeOtherStep5);
    CREATE_SCENE(MakeOtherStep5);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void startOven(float dt);
    void showHand(float dt);
private:
    Sprite* m_pCookie;
    Sprite* m_pRectNode;
    Sprite* m_pOvenDoorOpen;
    Sprite* m_pOvenDoorClose;
    Sprite* m_pOven_cookie;
    int _zorder;
    bool m_bIsTouchBtn;
    bool m_bIsOvenStart;
};


#endif /* MakeOtherStep5_hpp */
