//
//  Ellipse.h
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#ifndef __ActionTest_x__Ellipse__
#define __ActionTest_x__Ellipse__

#include <iostream>
#include <cocos2d.h>
using namespace cocos2d;

/**
 *  椭圆轨迹动画基类 _a: 椭圆长轴 _b: 椭圆短轴 _center: 椭圆中心点
 */
class Ellipse : public CCActionInterval {
protected:
    float _a;
    float _b;
    CCPoint _center;
};

#endif /* defined(__ActionTest_x__Ellipse__) */
