//
//  CookieBake.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef CookieBake_hpp
#define CookieBake_hpp
#include "BaseStepLayer.h"
#include "TouchNodeComponent.h"
class CookieBake:public BaseStepLayer {
public:
    CREATE_SECENE(CookieBake);
    CookieBake();
protected:
    bool init();
    void onEnterTransitionDidFinish();
    virtual void touchEnd(ui::Widget* widget);
    void touchCookie(Node*,Component*, OperateListner* _lis);
protected:
    Node* cookie;
};


#endif /* CookieBake_hpp */
