//
//  MakeStep66.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/5.
//
//

#ifndef MakeStep66_hpp
#define MakeStep66_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeStep6 : public BaseLayer  {
public:
    MakeStep6();
    ~MakeStep6();
public:
    bool init();
    CREATE_FUNC(MakeStep6);
    CREATE_SCENE(MakeStep6);
    
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


#endif /* MakeStep66_hpp */
