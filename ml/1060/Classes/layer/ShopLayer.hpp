//
//  ShopLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/30.
//
//

#ifndef ShopLayer_hpp
#define ShopLayer_hpp

#include "BaseShopLayer.h"
class ShopLayer : public BaseShopLayer{
    DECLARE_DYNCLASS(ShopLayer);
public:
    ShopLayer();
    CREATE_FUNC(ShopLayer);
protected:
    bool init();
};

#endif /* ShopLayer_hpp */
