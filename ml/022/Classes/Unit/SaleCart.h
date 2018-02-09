
#ifndef __SaleCart__
#define __SaleCart__


#include "Cart.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SaleCart : public Cart
{
public:
    SaleCart();
    ~SaleCart();
    
    CREATE_FUNC(SaleCart);
    
    virtual bool init();
protected:
    
    Sprite* m_pShelfLeftFront;
    Sprite* m_pShelfRightFront;
};


#endif