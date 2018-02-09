//
//  FruitTartBake.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef FruitTartBake_hpp
#define FruitTartBake_hpp
#include "BaseStepLayer.h"
#include "TouchNodeComponent.h"
class FruitTartBake:public BaseStepLayer {
public:
    CREATE_SECENE(FruitTartBake);
    FruitTartBake();
protected:
    bool init();
    void onEnterTransitionDidFinish();
    virtual void touchEnd(ui::Widget* widget);
protected:
    Node* fruitTart;
};


#endif /* FruitTartBake_hpp */
