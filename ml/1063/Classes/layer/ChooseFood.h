//
//  ChooseFood.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef ChooseFood_hpp
#define ChooseFood_hpp
#include "BaseStepLayer.h"
class ChooseFood:public BaseStepLayer {
public:
    CREATE_SECENE(ChooseFood);
    ChooseFood();
    ~ChooseFood();
protected:
    bool init();
    void onEnter();
    void onEnterTransitionDidFinish();
    virtual void touchEnd(ui::Widget* widget);
protected:
    float moveDistance;
    vector<float> posY;
    ParticleSystemQuad* chooseParticle;
    int chooseIndex;
};


#endif /* ChooseFood_hpp */
