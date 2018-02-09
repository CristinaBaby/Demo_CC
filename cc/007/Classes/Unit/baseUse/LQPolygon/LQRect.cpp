//
//  LQRect.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#include "LQRect.h"

LQRect::LQRect():
origin(Vec2::ZERO)
,size(Size::ZERO)
{
    
}

 LQRect *LQRect::create(Rect r){
     auto _rect = new LQRect;
     if(nullptr != _rect && _rect->initData(r)){
         _rect->autorelease();
         return _rect;
     }
     CC_SAFE_RELEASE_NULL(_rect);
     return nullptr;
}

bool LQRect::initData(Rect _r){
    polygonPoits.clear();
    polygonPoits.push_back(_r.origin);
    polygonPoits.push_back(_r.origin+Vec2(_r.size.width,0));
    polygonPoits.push_back(_r.origin+_r.size);
    polygonPoits.push_back(_r.origin+Vec2(0,_r.size.height));
    origin = _r.origin;
    size =  _r.size;
    _midPos = Vec2(_r.getMinX(),_r.getMidY());
    _judgeAngle = Vec2::ANCHOR_BOTTOM_RIGHT.getAngle(_r.size*.5);
    return true;
}


float LQRect::distanceToPoint(Vec2 _pos){
    
    //求当前点到矩形的距离
    auto _curenVe2 =  _pos- _midPos;
    float _currentAngle = Vec2::ANCHOR_BOTTOM_RIGHT.getAngle(_curenVe2);
    float _destanceToRect;
    
    auto _distance= _destanceToRect = _curenVe2.getLength();
    if(_currentAngle>-_judgeAngle && _currentAngle<=_judgeAngle){//与(maxX,0)相交
        _destanceToRect = _distance*(_pos.x- (origin.x+size.width))/(_pos.x - _midPos.x);
    } else if(_currentAngle>_judgeAngle&&_currentAngle<=(M_PI-_judgeAngle)) {//与(0,maxY)相交
        _destanceToRect = _distance*(_pos.y- (origin.y+size.height))/  (_pos.y - _midPos.y);
    } else if(_currentAngle<=-_currentAngle&&_currentAngle>=(-M_PI + _judgeAngle)) {//与(0,minY)相交
        _destanceToRect = _distance*(_pos.y- origin.y)/  (_pos.y - _midPos.y);
    } else {//与(minx,0)相交
        _destanceToRect = _distance*(_pos.x- origin.x)/  (_pos.x - _midPos.x);
    }
    return _destanceToRect;
}
Vec2  LQRect::getMidPos(){
    return origin + size*.5;
}