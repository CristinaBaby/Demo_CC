
#ifndef MenuLayer_h
#define MenuLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>
#include "LockItem.h"
#include "AudioHelp.h"

USING_NS_CC;
class MenuLayer : public Layer
{
public:
    MenuLayer();
    ~MenuLayer();
    
    CREATE_FUNC(MenuLayer);
    
    virtual bool init();
    enum{
        ePackageCottoncandy = 0,
        ePackageFunnelcake,
        ePackageSnowcone,
    };
    enum{
        eSnowcone_machine = 0,
        eSnowcone_ice,
        eSnowcone_cone,
        eSnowcone_fruit,
        eSnowcone_candy,
        eSnowcone_syrup,
        eSnowcone_spoon,
    };
    enum{
        eCottoncandy_sugar = 0,
        eCottoncandy_machine,
        eCottoncandy_candy,
        eCottoncandy_fruit,
        eCottoncandy_stick,
    };
    enum{
        eFunnelcake_flour = 0,
        eFunnelcake_oil,
        eFunnelcake_egg,
        eFunnelcake_milk,
        eFunnelcake_sugar,
        eFunnelcake_powder,
        eFunnelcake_salt,
    };
    enum{
        eFries_potato = 0,
        eFries_oil,
        eFries_salt,
        eFries_ketchup,
    };
    
    void showMenu(int index,bool canTouch = false);
    void hideMene();
    
    static void showAddIngredientAnimation(Node* btn)
    {
        if(!btn) {
            return;
        }
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bling.plist");
        btn->addChild(pParticle);
        pParticle->setPosition(btn->getContentSize()*0.5);
        pParticle->setLocalZOrder(10);
        btn->runAction(JumpBy::create(1, Vec2::ZERO, 10, 3));
        btn->runAction(Sequence::create(RotateBy::create(0.2, -15),
                                        RotateBy::create(0.4, 30),
                                        RotateBy::create(0.4, -30),
                                        RotateBy::create(0.2, 15), NULL));
        AudioHelp::getInstance()->playEffect("magic.mp3");
        
    }
    static void addIngredient(int package,int index,bool add = true)
    {
        sIngredientVector[package][index] = add;
    }
    
    static void reset(){
        std::memset(sIngredientVector,0, sizeof(sIngredientVector));
    }
protected:
    Sprite* m_pFrame;
    
    Sprite* m_pMenu;
    
    bool m_bCanHide;
    bool m_bCanTouch;
    static bool sIngredientVector[5][10];
};
#endif /* MenuLayer_h */
