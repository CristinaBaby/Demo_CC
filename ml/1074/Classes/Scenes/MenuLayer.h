
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
        ePackageCupcake = 0,
        ePackageShortcake,
        ePackageDonut,
    };
    enum{
        eCupcake_sugar = 0,
        eCupcake_butter,
        eCupcake_egg,
        eCupcake_flour,
        eCupcake_milk,
        eCupcake_salt,
        eCupcake_powder,
        eCupcake_coloring,
        eCupcake_marshmallow,
        eCupcake_rainbow,
    };
    enum{
        eDonut_yeast = 0,
        eDonut_salt,
        eDonut_butter,
        eDonut_egg,
        eDonut_vanilla,
        eDonut_sugar,
        eDonut_milk,
        eDonut_flour,
        eDonut_jam,
        eDonut_syrup,
        eDonut_icing_sugar,
        eDonut_coloring,
    };
    enum{
        eShortcake_milk = 0,
        eShortcake_salt,
        eShortcake_yeast,
        eShortcake_butter,
        eShortcake_flour,
        eShortcake_sugar,
        eShortcake_egg,
        eShortcake_coloring,
        eShortcake_cream,
        eShortcake_icing_sugar,
        eShortcake_vanilla,
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
