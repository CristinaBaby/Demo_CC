
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
        ePackageHotdog = 0,
        ePackageNachos,
        ePackagePizza,
        ePackageBurger,
        ePackageFries,
    };
    enum{
        eBurger_bread = 0,
        eBurger_beaf,
        eBurger_milk,
        eBurger_egg,
        eBurger_sauce,
        eBurger_onion,
        eBurger_garlic,
        eBurger_salt,
        eBurger_pepper,
    };
    enum{
        eHotdog_onion = 0,
        eHotdog_oil,
        eHotdog_sugar,
        eHotdog_mustard,
        eHotdog_paprika,
        eHotdog_chili,
        eHotdog_olive_oil,
        eHotdog_garlic,
    };
    enum{
        ePizza_flour = 0,
        ePizza_yeast,
        ePizza_salt,
        ePizza_oil,
        ePizza_water,
    };
    enum{
        eFries_potato = 0,
        eFries_oil,
        eFries_salt,
        eFries_ketchup,
    };
    enum{
        eNachos_chip = 0,
        eNachos_bean,
        eNachos_cheese,
        eNachos_tomato,
        eNachos_salsa,
        eNachos_onion,
        eNachos_pepper,
        eNachos_cream,
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
