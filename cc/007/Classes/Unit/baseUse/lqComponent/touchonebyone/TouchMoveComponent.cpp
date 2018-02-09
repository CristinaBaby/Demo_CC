//
//  TouchMoveComponent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#include "TouchMoveComponent.h"

const int TouchMoveComponent::backActionTag = 999;
IMPLEMENT_COCOS2DX_CLASS(TouchMoveComponent);
TouchMoveComponent::TouchMoveComponent():
_brintTop(true)
,_oldZOrder(nullptr)
,oragnalPos(nullptr)
,_moveType(MoveComponentType::kTypeNull)
,isTouch(false)
, _checkPoint(Vec2::ANCHOR_MIDDLE)
,_backTime(0.0f)
,_backDistance(Vec2::ZERO)
,_checkPolygon(nullptr)
{
}

TouchMoveComponent::~TouchMoveComponent(){
    CC_SAFE_DELETE(_oldZOrder);
    CC_SAFE_DELETE(oragnalPos);
    CC_SAFE_RELEASE_NULL(_checkPolygon);
}


void TouchMoveComponent::onAdd(){
    TouchComponent::onAdd();
    
}

bool TouchMoveComponent::touchBeganLis(Touch* _touch,Event*){
    bool flag = isCotain(_touch);
    if(flag){
        _backTime = 0.0f;
        isTouch = true;
     
        dispatcherEvent(ComponentTouchBegan);
    }
    return flag;
}
void TouchMoveComponent::touchMoveLis(Touch* _touch,Event*){
    if(_brintTop){
        if(_oldZOrder == nullptr){
            _oldZOrder = new int;
            *_oldZOrder = _owner->getLocalZOrder();
            _owner->getParent()->reorderChild(_owner, 500);
        }
    }
    if(oragnalPos == nullptr){
        oragnalPos = new Vec2;
        *oragnalPos =_owner->getPosition();
    }
    
    auto _newPoint = _owner->getPosition()+_touch->getDelta();
    
    switch (_moveType) {
        case MoveComponentType::kTypeInRect:{
            _newPoint = _owner->getParent()->convertToNodeSpace(_touch->getLocation());
            int _index = -1;
            if(_checkPolygon != nullptr){
                 auto dis = _owner->convertToNodeSpace(_touch->getLocation()) - _owner->getContentSize()*.5;
              //  auto dis = _touch->getDelta();
                vector<Vec2> temps (_checkPolygon->getPolygonPoits());
                for(int i=0;i<temps.size();i++)
                    temps[i] = _owner->convertToWorldSpace(temps[i] + dis);
                
                for(int i=0;i<_limits.size();i++){
                    auto p = _limits.at(i);
                    if(PolygonHelp::isPolygonInPolygon(temps,p->getPolygonPoits())){
                        _index =i;
                        break;
                    }
                }
                
            }else
                _index = getTargetIndex(getCheckPointInPoints(_newPoint));
            if(_index<0){
                _newPoint = _owner->getPosition();
                if(_owner != nullptr)
                    _owner->setPosition(_newPoint);
            }
            else{
                if(_owner != nullptr)
                    _owner->setPosition(_newPoint);
                dispatcherEventWithData(ComponentTouchMove,_touch->getDelta());
            }
            
            dispatcherEventWithData(ComponentTouchMoveALL,_touch->getDelta());
        }
            break;
        default:
            if(_owner != nullptr){
                if(_brintTop&&_owner->getZOrder() != 500)
                    _owner->getParent()->reorderChild(_owner, 500);
                _owner->setPosition(_newPoint);
            }
            dispatcherEventWithData(ComponentTouchMove,_touch->getDelta());
            break;
    }
}
void TouchMoveComponent::touchEndLis(Touch* t,Event*){
    
    int _index = getTargetIndex(getCheckPointInPoints(_owner->getPosition()));
    switch (_moveType) {
        case MoveComponentType::kTypeBack:{
            if(_index<0)
                backToStart();
            else {
                CC_SAFE_DELETE(_oldZOrder);
                dispatcherEventWithData(ComponentTouchEnd,_index);
            }
        }
            break;
        default:
            CC_SAFE_DELETE(_oldZOrder);
             dispatcherEventWithData(ComponentTouchEnd,_index);
            break;
    }
    isTouch = false;
}
void TouchMoveComponent::touchCanceLis(Touch* t,Event*){
    switch (_moveType) {
        case MoveComponentType::kTypeBack:
            backToStart();
            break;
        default:
            if(_oldZOrder != nullptr){
                _owner->getParent()->reorderChild(_owner, *_oldZOrder);
                CC_SAFE_DELETE(_oldZOrder);
            }
            dispatcherEvent(ComponentTouchCancle);
            break;
    }
    isTouch = false;
}

void TouchMoveComponent::backToStart(){
    if(oragnalPos != nullptr && _owner != nullptr) {
        _backDistance =  (*oragnalPos)- _owner->getPosition();
        _owner->stopActionByTag(backActionTag);
        auto _backAciton = Sequence::create(EaseElasticOut::create(MoveTo::create(0.2, (*oragnalPos))),CallFunc::create([&](){
            if(_oldZOrder != nullptr){;
                _owner->getParent()->reorderChild(_owner, *_oldZOrder);
                Director::getInstance()->getRenderer()->render();
                CC_SAFE_DELETE(_oldZOrder);
            }
            _owner->setPosition(*oragnalPos);
            dispatcherEvent(ComponentTouchCancle);
        }), nullptr);
        _backAciton->setTag(backActionTag);
        _owner->runAction(_backAciton);
    }
    //    if(!_scheduler->isScheduled(schedule_selector(TouchMoveComponent::moveBackSchedle), this))
    //        _scheduler->schedule(schedule_selector(TouchMoveComponent::moveBackSchedle) , this, 0 , CC_REPEAT_FOREVER, 0, false);
}

void TouchMoveComponent::setTargets(const Vector<LQPolygon*>& rects) {
    _limits = rects;
}
void TouchMoveComponent::removeTargets(size_t index){
    if(index<_limits.size())
        _limits.erase(_limits.begin()+index);
}

void TouchMoveComponent::removeTargets(LQPolygon* _temp){
    auto _itor = std::find(_limits.begin(), _limits.end(), _temp);
    if(_itor != _limits.end())
        _limits.erase(_itor);
}

void TouchMoveComponent::setTarget(LQPolygon* lq){
    _limits.clear();
    _limits.pushBack(lq);
}

void TouchMoveComponent::moveBackSchedle(float _time){
    if(isTouch){
        _scheduler->unschedule(schedule_selector(TouchMoveComponent::moveBackSchedle), this);
        return;
    }
    _backTime +=_time;
    auto _moveBy = ((*oragnalPos)-  _owner->getPosition())*(_backTime/0.2);
    auto _newPos = _owner->getPosition() +_moveBy;
    if(_newPos.distance((*oragnalPos)) <1){
        _scheduler->unschedule(schedule_selector(TouchMoveComponent::moveBackSchedle), this);
        if(_oldZOrder != nullptr){;
            _owner->getParent()->reorderChild(_owner, *_oldZOrder);
            Director::getInstance()->getRenderer()->render();
            CC_SAFE_DELETE(_oldZOrder);
        }
        _owner->setPosition(*oragnalPos);
        dispatcherEvent(ComponentTouchCancle);
        
    } else
        _owner->setPosition(_newPos);
    
}

bool TouchMoveComponent::isCotain(Touch* pTouch){
    return isPointInNode(_owner, pTouch->getLocation());
}

int TouchMoveComponent::getTargetIndex(Vec2 pos){
    int _index = -1;
    for(auto i=0;i<_limits.size();i++){
        if(_limits.at(i)->containsPoint(pos)){
            _index = i;
            break;
        }
    }
    return _index;
}

Vec2 TouchMoveComponent::getCheckPointInPoints(Vec2 _point){
    Size s = _owner-> getContentSize();
    Vec2 distanP = Vec2(s.width*_checkPoint.x, s.height*_checkPoint.y);
    return _owner->convertToWorldSpace(distanP) + _point-_owner->getPosition();
}

void TouchMoveComponent::setOrnagnalPos(Vec2 _vec){
    if(oragnalPos == nullptr)
        oragnalPos = new Vec2;
    *oragnalPos = _vec;
}

const Vec2 TouchMoveComponent::getOrnagnalPos(){
    Vec2 pos;
    if(oragnalPos != nullptr)
        pos = *oragnalPos;
    return pos;
}

bool TouchMoveComponent::checkInlimit(){
    
    return  getTargetIndex(getCheckPointInPoints(_owner->getPosition()))>=0;
}

void TouchMoveComponent::onResume(){
    TouchComponent::onResume();
}


void TouchMoveComponent::onRemove(){
    TouchComponent::onRemove();
    
}

int TouchMoveComponent::getOldZorder(){
    if(_oldZOrder != nullptr)
        return *_oldZOrder;
    else
        return 0;
}
