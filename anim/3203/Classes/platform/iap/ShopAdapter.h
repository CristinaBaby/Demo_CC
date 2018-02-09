//
//  ShopAdapter.h
//  Doctor
//
//  Created by tanshoumei on 13-8-20.
//
//

#ifndef __Doctor__ShopAdapter__
#define __Doctor__ShopAdapter__

#include <iostream>
#include "SingleTon.h"

class ShopAdapter: public SingleTon<ShopAdapter>
{
    friend class SingleTon<ShopAdapter>;
public:
    void requestBuyItem(std::string key);
    void requestRestore();
    void buyItemSuccess(std::string key);
};
#endif /* defined(__Doctor__ShopAdapter__) */
