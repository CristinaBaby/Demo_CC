//
//  LQ_adLoadingLayer.h
//  Cupcake
//
//  Created by luotianqiang1 on 15/6/15.
//
//

#ifndef __Cupcake__LQ_adLoadingLayer__
#define __Cupcake__LQ_adLoadingLayer__

#include "AdLoadingLayerBase.h"
class LQ_adLoadingLayer : public AdLoadingLayerBase{
public:
    CREATE_FUNC(LQ_adLoadingLayer);
    ~LQ_adLoadingLayer(){}
    static bool isChangeLayer;
protected:
    bool init();
protected:
    virtual  void onEnterTransitionDidFinish();
};

#endif /* defined(__Cupcake__LQ_adLoadingLayer__) */
