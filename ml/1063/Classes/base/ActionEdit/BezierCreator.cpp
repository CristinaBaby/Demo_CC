//
//  BezierCreator.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/17.
//
//

#include "BezierCreator.h"
USING_NS_CC;
using namespace std;

ActionInterval* BezierCreator::createAction(const vector<Vec2>&points,float time){
    ActionInterval* temp = nullptr;
    do{
        Vector<FiniteTimeAction*> actionList;
        CC_BREAK_IF(points.size()==0);
        actionList.pushBack(Place::create(points.at(0)));
        for(int i=1;i<points.size();i=i+3){
            CC_BREAK_IF(i+2>=points.size());
            ccBezierConfig _config;
            _config.endPosition = points.at(i+2);
            _config.controlPoint_1 = points.at(i);
            _config.controlPoint_2 = points.at(i+1);
            actionList.pushBack(BezierTo::create(time, _config));
        }
        temp = Sequence::create(actionList);
    }while (false);
    return temp;
}
BezierTo* BezierCreator::createParabola(float t, Vec2 startPoint, Vec2 endPoint, float height, float angle)
{
    // 把角度转换为弧度
    float radian = angle*3.14159/180.0;
    // 第一个控制点为抛物线左半弧的中点
    float q1x = startPoint.x+(endPoint.x - startPoint.x)/4.0;
    Vec2 q1 = Vec2(q1x, height + startPoint.y+cos(radian)*q1x);
    // 第二个控制点为整个抛物线的中点
    float q2x = startPoint.x + (endPoint.x - startPoint.x)/2.0;
    Vec2 q2 = Vec2(q2x, height + startPoint.y+cos(radian)*q2x);
    
    //曲线配置
    ccBezierConfig cfg;
    cfg.controlPoint_1 = q1;
    cfg.controlPoint_2 = q2;
    cfg.endPosition = endPoint;
    //使用CCEaseInOut让曲线运动有一个由慢到快的变化，显得更自然
    //    return CCEaseInOut::create(,0.5);
    return BezierTo::create(t,cfg);


}