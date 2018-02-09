#include "TouchComponent.h"

IMPLEMENT_COCOS2DX_CLASS(TouchComponent);
TouchComponent::TouchComponent(void):
isSwallowTouches(true)
{
   
}


TouchComponent::~TouchComponent(void)
{
}

 void TouchComponent::onAdd(){
     LQComponent::onAdd();
  
}

EventListener* TouchComponent::createLis(){
    auto _lis = EventListenerTouchOneByOne::create();
    _lis->onTouchBegan = std::bind(&TouchComponent::touchBeganLis,this,std::placeholders::_1,std::placeholders::_2);
    _lis->onTouchMoved = std::bind(&TouchComponent::touchMoveLis,this,std::placeholders::_1,std::placeholders::_2);
    _lis->onTouchEnded = std::bind(&TouchComponent::touchEndLis,this,std::placeholders::_1,std::placeholders::_2);
    _lis->onTouchCancelled = std::bind(&TouchComponent::touchCanceLis,this,std::placeholders::_1,std::placeholders::_2);
    _lis->setSwallowTouches(isSwallowTouches);
    _dispatcher->addEventListenerWithSceneGraphPriority(_lis, _lisNode);
    return _lis;
}

 bool TouchComponent::isCotain(Touch* pTouch){
     return true;
}

bool TouchComponent::isPointInNode(Node* _node,Vec2 _vec){
    bool flag = false;
    if(_node != nullptr){
        Rect _limit;
        _limit.size = _node->getContentSize();
        flag = _limit.containsPoint(_node->convertToNodeSpace(_vec));
    }
    return flag;
}

bool TouchComponent::touchBeganLis(Touch*pTouch,Event*){
    return isCotain(pTouch);
}
void TouchComponent::touchMoveLis(Touch*,Event*){
    
}
void TouchComponent::touchEndLis(Touch*,Event*){
    
}
void TouchComponent::touchCanceLis(Touch*,Event*){
    
}



void TouchComponent::setSwallowTouches(bool _flag){
    isSwallowTouches = _flag;
    auto _lis = dynamic_cast<EventListenerTouchOneByOne*>(_componentLis);
    if(_lis != nullptr)
        _lis->setSwallowTouches(isSwallowTouches);
}