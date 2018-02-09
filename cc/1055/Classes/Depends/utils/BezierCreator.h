//
//  BezierCreator.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/17.
//
//

#ifndef BezierCreator_hpp
#define BezierCreator_hpp

#include "cocos2d.h"

USING_NS_CC;

class BezierCreator{
public:
  static  cocos2d::ActionInterval* createAction(const std::vector<cocos2d::Vec2>& points,float time);
    /*创建一个抛物线动作
     参数：
     t                   时间
     startPoint      开始点
     endPoint        结束点
     height          高度（影响抛物线的高度）
     angle           角度（贝塞尔曲线两个控制点与y轴的夹角，直接影响精灵的抛出角度）
     */
    static cocos2d::BezierTo* createParabola(float t, Vec2 startPoint, Vec2 endPoint, float height = 0, float angle = 60);
};
#endif /* BezierCreator_hpp */
