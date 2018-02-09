
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
        ePackageFriedEgg = 0,
        ePackageBacon,
        ePackageCorn,
        ePackageJuice,
        ePackageOmelette,
    };
    enum{
        eFriedEgg_egg = 0,
        eFriedEgg_salt,
        eFriedEgg_pepper,
        eFriedEgg_butter,
    };
    enum{
        eBacon_ham = 0,
        eBacon_bacon,
        eBacon_butter,
    };
    enum{
        eCorn_milk = 0,
        eCorn_sugar,
        eCorn_water,
        eCorn_corn,
    };
    enum{
        eOmelette_egg = 0,
        eOmelette_salt,
        eOmelette_pepper,
        eOmelette_butter,
        eOmelette_oil,
    };
    enum{
        eJuice_water = 0,
        eJuice_fruit,
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
