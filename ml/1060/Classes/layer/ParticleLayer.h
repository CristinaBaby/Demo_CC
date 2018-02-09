//
//  ParticleLayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/31.
//
//

#ifndef ParticleLayer_hpp
#define ParticleLayer_hpp

#include "cocos2d.h"
USING_NS_CC;
class ParticleLayer: public Layer{
public:
    CREATE_FUNC(ParticleLayer);
protected:
    virtual void onEnterTransitionDidFinish();
};

#endif /* ParticleLayer_hpp */
