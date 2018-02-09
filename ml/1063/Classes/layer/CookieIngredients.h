//
//  CookieIngredients.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/25.
//
//

#ifndef CookieIngredients_hpp
#define CookieIngredients_hpp

#include "BaseMakeLayer.h"
class CookieIngredients:public BaseMakeLayer {
public:
    CREATE_SECENE(CookieIngredients);
    CookieIngredients();
protected:
    bool init();
    void fallEnd(Node*, Component*, OperateListner*);
    void onEnterTransitionDidFinish();
    void showThing(const string& name,float delayTime);
    virtual void stopPrompt();
    StudioLayer* vanillaBottle;
    void mixEnd(Node*node,Component* c, OperateListner* _lis);
};

#endif /* CookieIngredients_hpp */
