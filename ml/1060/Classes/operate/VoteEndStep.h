//
//  VoteEndStep.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#ifndef VoteEndStep_hpp
#define VoteEndStep_hpp

#include "BaseStepLayer.h"
#include "ImageLabel.h"
class VoteEndStep: public BaseStepLayer{
    DECLARE_DYNCLASS(VoteEndStep);
    CC_SYNTHESIZE(int, playIndex, PlayIndex);
public:
    CREATE_FUNC(VoteEndStep);
    VoteEndStep();
       virtual void touchEnd(ui::Widget* widget);
protected:
    virtual bool init();
      virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
    ParticleSystemQuad* starParticle;
    void showImage(int index);
    ImageLabel* _timeLabel;
    int startIndex;
    static const string _eventName;
};

#endif /* VoteEndStep_hpp */
