//
//  MakeOtherStep4.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#ifndef MakeOtherStep4_hpp
#define MakeOtherStep4_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "MoldChooseLayer.h"
//make步骤的模板
class MakeOtherStep4 : public BaseLayer  {
public:
    MakeOtherStep4();
    ~MakeOtherStep4();
public:
    bool init();
    CREATE_FUNC(MakeOtherStep4);
    CREATE_SCENE(MakeOtherStep4);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    bool isOver();
    void showHand();
private:
    Sprite* m_pMold;
    Vector<Sprite*> m_vRect;
    Vec2 _moldPos;
    
    string m_sMold;
    string m_sFlavor;
    Sprite* m_pFlavor;
    MoldChooseLayer* _moldChooseLayer;
    
    Sprite* m_pHand;
    Vector<Sprite*> m_vSprite;
    Sprite* m_pTitle;
public:
    std::function<void()> nextStep = nullptr;
};

#endif /* MakeOtherStep4_hpp */
