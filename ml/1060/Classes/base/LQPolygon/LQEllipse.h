//
//  LQEllipse.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/20.
//
//

#ifndef LQEllipse_hpp
#define LQEllipse_hpp

#include "LQPolygon.h"
/**
 椭圆 a和b一样就是圆了
 */
class LQEllipse : public LQPolygon{
public:
    CREATE_FUNC(LQEllipse);
   static LQEllipse *create(Vec2 _center,float _a,float _b);
    bool initData(Vec2 _center,float _a,float _b);
    virtual bool containsPoint(Vec2);
public:
    Vec2 _center;//中心点
    float _a;//x轴
    float _b;//y轴
};

#endif /* LQEllipse_hpp */
