//
//  LQCircle.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef LQCircle_hpp
#define LQCircle_hpp

#include "LQPolygon.h"
class LQCircle: public LQPolygon{
public:
    LQCircle();
    CREATE_FUNC(LQCircle);
    static LQCircle* create(Vec2 ,float);
    virtual bool containsPoint(Vec2);
    virtual bool initData(Vec2 ,float);
    float distanceToPoint(Vec2);
public:
    float _R;
    Vec2 _center;
};

#endif /* LQCircle_hpp */
