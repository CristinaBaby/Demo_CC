//
//  MakeConeStep02.hpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#ifndef MakeConeStep02_hpp
#define MakeConeStep02_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"

USING_NS_CC;
class MakeConeStep02 : public BaseLayer, public ToolSpriteDelegate{
public:
    MakeConeStep02();
    ~MakeConeStep02();
    
    static Scene* scene();
    CREATE_FUNC(MakeConeStep02);
    virtual bool init() override;
    
protected:
    bool initData();
    bool initLayer();
    
    void bakeCake(Sprite* pBatterInMachine);
    void addProgress();
    void runHandTipAnimation();
    
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    virtual void onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    
    virtual void onNextCallback() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    Node* m_pBg;
    Node* m_pMoveArea;
    Sprite* m_pTable1;
    Sprite* m_pTable2;
    
    Sprite* m_pMachine;
    Sprite* m_pCover;
    
    Sprite* m_pBowl;
    Sprite* m_pBatter;
    
    Sprite* m_pBatterInSpoon;
    
    bool m_bRollAllowClick;
    
    Sprite* m_pBigLight;
    
    int m_iMachineWorkSoundId;
    
    ToolSprite* m_pHandTip;
};

#endif /* MakeConeStep02_hpp */
