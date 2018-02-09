//
//  LQRect.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef LQRect_hpp
#define LQRect_hpp

#include "LQPolygon.h"
class LQRect: public LQPolygon{
public:
    CREATE_FUNC(LQRect);
    static LQRect *create(Rect);
    LQRect();
    virtual bool initData(Rect);
    float distanceToPoint(Vec2 _pos);
    Vec2 getMidPos();
public:
    Vec2 origin;
    Size size;
private:
    Vec2 _midPos;
    float _judgeAngle;
};

#endif /* LQRect_hpp */
