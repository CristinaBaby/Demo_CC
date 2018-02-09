//
//  CartView.h
//  Make
//
//  Created by liji on 15/6/2.
//
//

#ifndef __Make__CartView__
#define __Make__CartView__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;


class CartView : public Layer
{
public:
    CREATE_FUNC(CartView);
    bool init();
    void addItemToCart(int itemIdx);
    
    
public:
    Rect cartCollisionRect_;

private:
    Node* itemNode_;

};
#endif /* defined(__Make__CartView__) */
