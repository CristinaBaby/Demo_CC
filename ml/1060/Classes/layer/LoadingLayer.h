//
//  LoadingLayer.h
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/17/15.
//
//

#ifndef __PizzaMaker__LoadingLayer__
#define __PizzaMaker__LoadingLayer__

#include "BaseStepLayer.h"
class LoadingLayer : public BaseStepLayer {
public:
    CREATE_FUNC(LoadingLayer);
    bool init();
    virtual  void onEnterTransitionDidFinish();
    void enCallBack();
    ~LoadingLayer();
};

#endif /* defined(__PizzaMaker__LoadingLayer__) */
