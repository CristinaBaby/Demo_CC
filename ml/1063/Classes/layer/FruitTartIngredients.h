//
//  FruitTartIngredients.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/25.
//
//

#ifndef FruitTartIngredients_hpp
#define FruitTartIngredients_hpp

#include "BaseMakeLayer.h"
class FruitTartIngredients:public BaseMakeLayer {
public:
    CREATE_SECENE(FruitTartIngredients);
    FruitTartIngredients();
protected:
    bool init();
    void fallEnd(Node*, Component*, OperateListner*);
    void onEnterTransitionDidFinish();
    void showThing(const string& name,float delayTime);
    void mixEnd(Node*node,Component*, OperateListner* _lis);
    void stopPrompt();
};

#endif /* FruitTartIngredients_hpp */
