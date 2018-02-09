//
//  MakeIceCreamStep01.hpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#ifndef MakeIceCreamStep01_hpp
#define MakeIceCreamStep01_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"

USING_NS_CC;
class MakeIceCreamStep01 : public BaseLayer, public ToolSpriteDelegate{
public:
    MakeIceCreamStep01();
    ~MakeIceCreamStep01();
    
    static Scene* scene();
    CREATE_FUNC(MakeIceCreamStep01);
    virtual bool init() override;
    
protected:
    bool initData();
    bool initLayer();
    
    inline void addMilk();
    inline void addSuger();
    inline void addEggs();
    inline void addBlender();

    inline void beatEgg1(ToolSprite* toolSprite);
    inline void beatEgg2(ToolSprite* toolSprite);
    
    Action* getStickMixAnimation();
    Action* getMachineShakeAnimation();
    ActionInterval* getIngredientAnimation();
    
    virtual void onNextCallback() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    virtual void onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    virtual void onMoveBackCallback(ToolSprite* toolSprite) override;
    
private:
    Node* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowl_up;

    Layer* m_pMixLayer;
    
    Sprite* m_pMilk;
    Sprite* m_pSuger;
    Sprite* m_pEgg1;
    Sprite* m_pEgg2;
    
    bool m_bEgg1AllowClick;
    bool m_bEgg2AllowClick;
    int m_iAddEggCount;
    
    Sprite* m_pStick;
    bool m_bIsMix;
    int m_iMixCount;
    
    // 搅拌时，碗里的材料
    Sprite* m_pIngredient;
    // 下面加一层，使搅拌时隐现，效果更真实
    Sprite* m_pIngredient_down;
    
    int m_iMilkSoundId;
    int m_iSugerSoundId;
    int m_iMixSoundId;
};
#endif /* MakeIceCreamStep01_hpp */
