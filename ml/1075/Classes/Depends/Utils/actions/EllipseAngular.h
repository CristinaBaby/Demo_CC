//
//  EllipseAngularAction.h
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#ifndef __ActionTest_x__EllipseAngularAction__
#define __ActionTest_x__EllipseAngularAction__

#include "Ellipse.h"

/**
 *  沿着椭圆轨迹绕椭圆中心做匀速（角速度）运动
 */
class EllipseAngular : public Ellipse {
public:
    /**
     *  a: 椭圆长轴 b: 椭圆短轴 center: 椭圆中心点
     */
    static EllipseAngular *create(float dt, float a, float b, CCPoint center);

protected:
    EllipseAngular(float a, float b, CCPoint center);
    virtual void update(float t);
    
};

#endif /* defined(__ActionTest_x__EllipseAngularAction__) */
