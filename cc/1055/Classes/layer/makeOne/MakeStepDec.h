//
//  MakeStepDecDec.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/13.
//
//

#ifndef MakeStepDecDec_hpp
#define MakeStepDecDec_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "ScribbleWithCallBack.h"

//make步骤的模板
class MakeStepDec : public BaseLayer  {
public:
    MakeStepDec();
    ~MakeStepDec();
public:
    bool init();
    CREATE_FUNC(MakeStepDec);
    CREATE_SCENE(MakeStepDec);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void showDecFruit();
    void showDecIcing();
    void showDecPlate();
    void showDecSprinkle();
    
    void selectedItemEvent(Ref *pSender, ListView::EventType type);
    void showListview(string path,bool isMold = false);
    
    bool onTouchAddDec(Touch* touch, Event* event);
    
    void onNegativeClick(void*);
    void onPositiveClick(void*);
    
    void showHand();
private:
    Sprite* m_pHand;
    Sprite* m_pBowl;
    Sprite* m_pDecBtn;
    Sprite* m_pDecBg;
    ListView* _listView;
    Layout* _decorate;
    int _decType;
    Sprite* m_pDecSelect;
    bool m_bIsTouch;
    Sprite* m_pFood;
    ScribbleWithCallBack* _icing;
    ScribbleWithCallBack* _icingSprinkle;
    int m_iIndex;
    ClippingNode* _cliping;
    ClippingNode* _clipingSprinkle;
    Sprite* m_pTitle;
    Vector<Sprite*> m_vLock;
    Vector<ScribbleWithCallBack*> m_vScribble;
    ParticleSystemQuad* m_pParticleSystemQuad;
    bool m_bIsShowHand;
    bool m_bIsShowHand1;
    bool m_bIsShowHand2;
};

#endif /* MakeStepDecDec_hpp */
