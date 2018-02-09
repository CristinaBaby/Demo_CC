//
//  FruitTartPour.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef FruitTartPour_hpp
#define FruitTartPour_hpp
#include "BaseStepLayer.h"
#include "TouchNodeComponent.h"
class FruitTartPour:public BaseStepLayer {
public:
    CREATE_SECENE(FruitTartPour);
    FruitTartPour();
protected:
    bool init();
    void onEnterTransitionDidFinish();
    virtual void touchEnd(ui::Widget* widget);
};


#endif /* FruitTartPour_hpp */
