//
//  MoveForeverComponent.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#include "MoveForeverComponent.h"
#include "ui/CocosGUI.h"

PositionChange::PositionChange():
_inner(nullptr)
,_callBack(nullptr){
    
}

PositionChange::~PositionChange(){
    CC_SAFE_RELEASE_NULL(_inner);
}

PositionChange* PositionChange::create(ActionInterval* _inner){
    auto _action = new PositionChange();
    if(_action && _action->initWithAction(_inner)){
        _action->autorelease();
        return _action;
    }
    CC_SAFE_RELEASE_NULL(_action);
    return nullptr;
}
void PositionChange::startWithTarget(Node* _node){
    ActionInterval::startWithTarget(_node);
    _inner->startWithTarget(_node);
}

void PositionChange::stop(void)
{
    _inner->stop();
    ActionInterval::stop();
}

bool PositionChange::initWithAction(ActionInterval* action){
    if (ActionInterval::initWithDuration(action->getDuration()))
    {
        _inner = action;
        action->retain();
        return true;
    }
    
    return false;
}

void  PositionChange::update(float time){
    auto _prePos = _target->getPosition();
    _inner->update(time);
    if(_callBack != nullptr){
        auto _current = _target->getPosition();
        _callBack(_target,_prePos,_current);
    }
}

ActionInterval* PositionChange::getInnerAction()
{
    return _inner;
}

 PositionChange* PositionChange::clone() const
{
 
    return nullptr;
}

 PositionChange* PositionChange::reverse() const 
{
  
    return nullptr;
}


const  Vec2 MoveForeverComponent::leftToRigth = Vec2(1,0);
const  Vec2 MoveForeverComponent::rightToLeft = Vec2(-1,0);
const  Vec2 MoveForeverComponent::bottomToTop = Vec2(0,1);
const  Vec2 MoveForeverComponent::TopToBottom = Vec2(0,-1);


IMPLEMENT_COCOS2DX_CLASS(MoveForeverComponent);

MoveForeverComponent::MoveForeverComponent():
_speed(0)
,_direction(leftToRigth)
,_moveAction(nullptr)
,_temp1(nullptr)
,_temp2(nullptr)
,_playing(false){
    
}

MoveForeverComponent::~MoveForeverComponent(){
    
}

bool MoveForeverComponent::MoveForeverComponent::init(){
    if(LQComponent::init()){
        
        return true;
    }
    return false;
}

void MoveForeverComponent::onAdd(){
    LQComponent::onAdd();
    resumeAction();
}

void  MoveForeverComponent::setDirection(Vec2  _direction){
    if(!((_direction.x == 0)&&(_direction.y==0))){
        auto _legnth = _direction.getLength();
        this->_direction.x = _direction.x/_legnth;
        this->_direction.y = _direction.y/_legnth;
    }
}

void  MoveForeverComponent::resumeAction(){
    if( _owner != nullptr&&!((_direction.x == 0)&&(_direction.y==0))&& _speed !=0){
        if(!_playing){
            _playing = true;
            _owner->setCascadeOpacityEnabled(false);
            _owner->setOpacity(0);
            auto _owerSprite = dynamic_cast<Sprite*>(_owner);
            if(_owerSprite != nullptr){
                _temp1 = Sprite::createWithSpriteFrame(_owerSprite->getSpriteFrame());
                _temp2 = Sprite::createWithSpriteFrame(_owerSprite->getSpriteFrame());
            } else {
                auto _widget = dynamic_cast<ui::Widget*>(_owner);
                if(_widget != nullptr){
                    _temp1 = _widget->clone();
                    _temp2 = _widget->clone();
                }
            }
            
            auto _contentSize = _owner->getContentSize();
            if(abs(_direction.x)>abs(_direction.y)){
                _moveDeta.x = _contentSize.width*(_direction.x/abs(_direction.x));
                _moveDeta.y = (_direction.y/_direction.x)*_moveDeta.x;
            } else {
                _moveDeta.y = _contentSize.height*(_direction.y/abs(_direction.y));
                _moveDeta.x = (_direction.x/_direction.y)*_moveDeta.y;
            }
            auto _startPos = _owner->getPosition();
            
            
            _temp1->setAnchorPoint(_owner->getAnchorPoint());
            _temp1->setPosition(_owner->getAnchorPointInPoints());
            _owner->addChild(_temp1);
            
            _temp2->setAnchorPoint(_owner->getAnchorPoint());
            _temp2->setPosition(_owner->getAnchorPointInPoints()-_moveDeta);
            _owner->addChild(_temp2);
            
            auto _chan1 = PositionChange::create(MoveBy::create(_moveDeta.getLength()/_speed, _moveDeta));
            _chan1->setCallBack([this](Node* _node,Vec2 _pre,Vec2 _cru){
                 _temp2->setPosition(_temp1->getPosition()-_moveDeta);
                dispatcherEventWithData(MoveForeverCallBack, _cru-_pre);
            });
            
            auto _chan2 = PositionChange::create(MoveBy::create(_moveDeta.getLength()/_speed, _moveDeta));
            _chan2->setCallBack([this](Node* _node,Vec2 _pre,Vec2 _cru){
                _temp2->setPosition(_temp1->getPosition()+_moveDeta);
                dispatcherEventWithData(MoveForeverCallBack, _cru-_pre);
            });
            _temp1->runAction(RepeatForever::create(Sequence::create(_chan1,MoveTo::create(0, _owner->getAnchorPointInPoints()-_moveDeta), _chan2,nullptr)));
            
        } else{
            Director::getInstance()->getActionManager()->resumeTarget(_temp1);
            Director::getInstance()->getActionManager()->resumeTarget(_temp2);
        }
    }
}
void MoveForeverComponent:: pauseAction(){
    if(_owner != nullptr&&_temp1 != nullptr&& _temp2 != nullptr){
        Director::getInstance()->getActionManager()->pauseTarget(_temp1);
        Director::getInstance()->getActionManager()->pauseTarget(_temp2);
    }
}




