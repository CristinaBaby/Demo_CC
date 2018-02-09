//
//  CircleBy.cpp
//  CrazyCalculate
//
//  Created by tangbowen on 14-12-26.
//
//

#include "CircleBy.h"

CircleBy::CircleBy():_circleCenter(Vec2::ZERO),_radians(0),_radiansValue(0),_targetRandians(0),_maxRadians(0),_times(0)
{
}

CircleBy::~CircleBy()
{
}

CircleBy * CircleBy::create(float tm, Vec2 circleCenter,float initRandians, float targetRandians)
{
    CircleBy * _circle = new CircleBy();
    _circle->init(tm, circleCenter, initRandians, targetRandians);
    _circle->autorelease();
    return _circle;
}
//initRandians
bool CircleBy::init(float tm, Vec2 circleCenter, float initRandians, float targetRandians)
{
    //动作执行时间
    if(!initWithDuration(tm))
    {
        return false;
    }
    //初始化圆心和半径
    this->_circleCenter = circleCenter;
    _radius = sqrt(pow(_circleCenter.x, 2) + pow(_circleCenter.y, 2));
    //每帧需要转过的弧度
    this->_radians = (Director::getInstance()->getAnimationInterval()) * 2 * M_PI / tm;
    //如果第三个参数小于零，代表做的是逆时针的圆周运动
    if(initRandians < 0)
        _radians = -_radians;
    //刷新次数
    this->_times = 1;
    //初始弧度数
    this->_radiansValue = initRandians / 360 * 2 * M_PI;
    //旋转的角度，默认为360
    this->_maxRadians = 2 * M_PI;
    
    _targetRandians = targetRandians;
    this->setRadians(_targetRandians);
    
    return true;
}

void CircleBy::setRadians(float radians)
{
    auto val = radians / 360 * 2 * M_PI;
    this->_radians = _radians / _radians * (Director::getInstance()->getAnimationInterval()) * val / _duration;
    //如果第三个参数小于零，代表做的是逆时针的圆周运动
    if(_radiansValue < 0){
        _radians = -_radians;
    }
}

void CircleBy::startWithTarget(Node * target)
{
    ActionInterval::startWithTarget(target);
    //根据执行动作精灵的坐标重新初始化圆心，repeat的时候会多次调用该函数，所以这里判断是否是第一次
    if(_times == 1) {
        _circleCenter = _circleCenter + target->getPosition();
    }
}

//动作管理器调用update函数，每帧刷新坐标
void CircleBy::update(float)
{
    // _radiansValue 初始的角度
    auto radians = _radiansValue;
    //如果radians小于零，需要做逆时针的圆周运动
    if(radians < 0){
        radians = -radians;
    }
    radians = _radians * _times + radians;
    
    auto x = _radius*sin(radians);
    auto y = _radius*cos(radians);
    
    _target->setPosition(Vec2(x + _circleCenter.x, y + _circleCenter.y));
    
    _times++;
    
//    /*以下的代码将做圆周运动的轨迹绘制了出来，必要的时候可以删除掉*/
//    
//    //创建一个drawNode用来绘图
//    auto draw = DrawNode::create();
//    //将DrawNode添加到父节点中，这样绘制的位置就和执行动作的sprite在同一个节点坐标系中
//    _target->getParent()->addChild(draw);
//    //画点
//    draw->drawDot(_target->getPosition(), 1, Color4F(1, 1, 1, 1));
}

//反动作，逆时针方向转动
CircleBy* CircleBy::reverse() const
{
    return CircleBy::create(_duration, _circleCenter, -_radiansValue * 360 / (2 * M_PI), _targetRandians);
}

//克隆一个相同的动作，init传入的时候转化为角度
CircleBy* CircleBy::clone() const
{
    auto a = new CircleBy();
    a->init(_duration, _circleCenter, _radiansValue * 360 / (2 * M_PI), _targetRandians);
    a->autorelease();
    return a;
}