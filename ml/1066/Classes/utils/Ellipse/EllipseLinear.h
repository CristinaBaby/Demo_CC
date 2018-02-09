//
//  EllipseLinearAction.h
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#ifndef __ActionTest_x__EllipseLinearAction__
#define __ActionTest_x__EllipseLinearAction__

#include "Ellipse.h"
/**
 *  沿着椭圆轨迹绕椭圆中心做匀速（线速度）运动
 */
class EllipseLinear : public Ellipse {
public:
    /**
     *  a: 椭圆长轴 b: 椭圆短轴 center: 椭圆中心点
     */
    static EllipseLinear *create(float dt, float a, float b, Point center);
    virtual EllipseLinear* reverse()const override;
    virtual EllipseLinear *clone()const override;
    
protected:
    EllipseLinear(float a, float b, Point center);
    virtual void update(float t);
   
    
};

#endif /* defined(__ActionTest_x__EllipseLinearAction__) */
