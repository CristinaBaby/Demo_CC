//
//  EllipseConfig.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/17.
//
//

#ifndef MermaidSeaKingdom_EllipseConfig_h
#define MermaidSeaKingdom_EllipseConfig_h
#include "cocos2d.h"
USING_NS_CC;

class EllipseConfig {
public:
    Vec2 _center;
    float _a;
    float _b;
//    bool checkPoint(Vec2&_point){//判断点是否在椭圆上，阈值为30(切线的垂线)
//        
//        //椭圆的斜率为-b*b*(x-_center.x)/(a*a*(y-_center.y))
//        if(_point.y==_center.y)
//            return false;
//        float _temX = _point.x-_center.x;
//        float _temY = _point.y-_center.y;
//        //
//        
//        //点的斜率(假设中心点在原点，最后再根据中心点做平移)
//        float k = -_b*_b*_temX/(_a*_a*_temY);
//        //求这个斜率在椭圆上的点
//    
//    }
    
    bool checkPoint(Vec2&_point){//判断点是否在椭圆上，阈值为30(直接求point到中心点的直线与椭圆的焦点)
        auto _ornPoint = _point;
        float cos0 = (_point.x-_center.x)/_point.distance(_center);
        float sin02,cos02,absY,absX;
        cos02 = cos0*cos0;
        sin02 = 1-cos02;
        
        if(cos02 == 0)
            absX=_a;
        else
            absX = sqrt(1/((sin02/cos02)/(_b*_b) + 1/(_a*_a)));
        if(_ornPoint.x>_center.x)
            _ornPoint.x = absX;
        else
            _ornPoint.x = -absX;
        
        absY =sqrt(1-_ornPoint.x*_ornPoint.x/(_a*_a))*_b;
        if(_ornPoint.y>_center.y)
            _ornPoint.y = absY;
        else
            _ornPoint.y = -absY;
        _ornPoint.x = _ornPoint.x+_center.x;
        _ornPoint.y = _ornPoint.y+ _center.y;
        if(_point.distance(_ornPoint)<=30) {
            _point = _ornPoint;
            return true;
        } else
            return false;
        
    }
};
#endif
