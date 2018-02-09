
#ifndef __PriceCart__
#define __PriceCart__


#include "Cart.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PriceCart : public Cart
{
public:
    PriceCart();
    ~PriceCart();
    
    CREATE_FUNC(PriceCart);
    
    virtual bool init();
protected:
    
    Sprite* m_pShelfLeftBack;
    Sprite* m_pShelfLeftFront;
    Sprite* m_pShelfRightBack;
    Sprite* m_pShelfRightFront;
};


#endif