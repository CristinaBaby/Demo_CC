//
//  DistanceToTarget.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/10.
//
//

#include "DistanceToTarget.h"
#include "TouchMoveComponent.h"
#include "LQMath.h"
IMPLEMENT_COCOS2DX_CLASS(DistanceToTarget);


DistanceToTarget::DistanceToTarget():
_startLen(-1)
,_checkPos(Vec2::ANCHOR_MIDDLE)
,targetNode(nullptr){
    
}

void DistanceToTarget::setTargetRect(const Rect& _rect){
    _endRect = _rect;
    MidRect = Vec2(_rect.getMidX(),_rect.getMidY());
    _judgeAngle = Vec2::ANCHOR_BOTTOM_RIGHT.getAngle(_endRect.size*.5);
}

void DistanceToTarget::setCheckPoint(const Vec2& _pos){
    _checkPos = _pos;
}

void  DistanceToTarget::startUpdate(){
    if(!this->isSchedule(schedule_selector(DistanceToTarget::updatePos))) {
         updatePos(0);
        this->schedule(schedule_selector(DistanceToTarget::updatePos),0.0f);
    }
   
}
void  DistanceToTarget::pauseUpdate(){
    if(this->isSchedule(schedule_selector(DistanceToTarget::updatePos)))
        this->unSchedule(schedule_selector(DistanceToTarget::updatePos));
    
}

 void  DistanceToTarget::updatePos(float _time){
     if(_enabled && _owner != nullptr){
         //求当前点到矩形的距离
         if(targetNode != nullptr){
             _endRect.origin = targetNode->convertToWorldSpace(Point::ZERO);
             _endRect.size = targetNode->convertToWorldSpace(targetNode->getContentSize()) - _endRect.origin;
         }
         MidRect = Vec2(_endRect.getMidX(),_endRect.getMidY());
         _judgeAngle = Vec2::ANCHOR_BOTTOM_RIGHT.getAngle(_endRect.size*.5);
		 auto _checkPos = getTargetPos();
         auto _curenVe2 =  _checkPos- MidRect;
         float _currentAngle = Vec2::ANCHOR_BOTTOM_RIGHT.getAngle(_curenVe2);
         float _destanceToRect;
         
         auto _distance= _destanceToRect = _curenVe2.getLength();
         if(_currentAngle>-_judgeAngle && _currentAngle<=_judgeAngle){//与(maxX,0)相交
             _destanceToRect = _distance*(_checkPos.x- _endRect.getMaxX())/  (_checkPos.x - MidRect.x);
         } else if(_currentAngle>_judgeAngle&&_currentAngle<=(M_PI-_judgeAngle)) {//与(0,maxY)相交
              _destanceToRect = _distance*(_checkPos.y- _endRect.getMaxY())/  (_checkPos.y - MidRect.y);
         } else if(_currentAngle<=-_currentAngle&&_currentAngle>=(-M_PI + _judgeAngle)) {//与(0,minY)相交
              _destanceToRect = _distance*(_checkPos.y- _endRect.getMinY())/  (_checkPos.y - MidRect.y);
         } else {//与(minx,0)相交
              _destanceToRect = _distance*(_checkPos.x- _endRect.getMinX())/  (_checkPos.x - MidRect.x);
         }
		 if(_destanceToRect<0)
			 _destanceToRect = 0;
         if(_startLen<0)
             _startLen = _destanceToRect;
         auto _percent = _destanceToRect/_startLen;
         //if(_percent>1.0)
           //  _percent = 1.0;
         dispatcherEventWithData(MoveDistancePercent, 1-_percent);
     }
}


Vec2 DistanceToTarget::getTargetPos(){
    Vec2 _pos;
    if(_owner != nullptr){
        Size s = _owner-> getContentSize();
        Vec2 distanP = Vec2(s.width*_checkPos.x, s.height*_checkPos.y);
        _pos =_owner->convertToWorldSpace(distanP);
    }
    return _pos;
}

void DistanceToTarget::setTargetNode(Node* node){
    targetNode = node;
}

 void DistanceToTarget::onRemove(){
    if(this->isSchedule(schedule_selector(DistanceToTarget::updatePos)))
        this->unSchedule(schedule_selector(DistanceToTarget::updatePos));
     LQComponent::onRemove();
}