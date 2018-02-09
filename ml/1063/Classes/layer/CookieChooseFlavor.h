//
//  CookieChooseFlavor.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef CookieChooseFlavor_hpp
#define CookieChooseFlavor_hpp
#include "BaseStepLayer.h"
class CookieChooseFlavor:public BaseStepLayer {
public:
    CREATE_SECENE(CookieChooseFlavor);
    CookieChooseFlavor();
    ~CookieChooseFlavor();
protected:
    bool init();
    void onEnter();
    void onEnterTransitionDidFinish();
    virtual void touchEnd(ui::Widget* widget);
protected:
    float moveDistance;
    float posY;
    ParticleSystemQuad* chooseParticle;
};


#endif /* CookieChooseFlavor_hpp */
