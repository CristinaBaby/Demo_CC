//
//  VoteLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef VoteLayer_hpp
#define VoteLayer_hpp
#include "BaseStepLayer.h"
class VoteLayer: public BaseStepLayer{
    DECLARE_DYNCLASS(VoteLayer);
public:
    ~VoteLayer();
    CREATE_FUNC(VoteLayer);
    bool init();
    virtual void onEnterTransitionDidFinish();
protected:
    void touchEnd(ui::Widget* widge);
};

#endif /* VoteLayer_hpp */
