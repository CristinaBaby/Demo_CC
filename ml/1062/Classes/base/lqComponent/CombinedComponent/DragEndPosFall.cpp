//
//  DragEndPosFall.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DragEndPosFall.h"
#include "DistanceToTarget.h"
#include "MoveHideShade.h"
IMPLEMENT_COCOS2DX_CLASS(DragEndPosFall);

DragEndPosFall::DragEndPosFall():
_inbow(nullptr)
,_inbowAction(nullptr)
,_endRote(0)
,_startRote(-10000){
    
}

DragEndPosFall::~DragEndPosFall() {
    CC_SAFE_RELEASE_NULL(_inbow);
    CC_SAFE_RELEASE_NULL(_inbowAction);
}

bool DragEndPosFall::init(){
    if(LQComponent::init()){
        auto _move = MoveHideShade::create();
        _move->setMoveType(MoveComponentType::kTypeBack);
        addComponent(_move);
        
        auto _rote = DistanceToTarget::create();
        addComponent(_rote);
        
        _move->addListners(ComponentTouchMove,[this,_rote,_move](Node* _node,Component*,OperateListner*){
            _rote->startUpdate();
            if(_move->checkInlimit()){
                _move->setEnabled(false);
                _rote->pauseUpdate();
                _node->runAction(RotateTo::create(0.2, _endRote));
                _node->runAction(Sequence::create(MoveTo::create(0.2, endPos),CallFunc::create(CC_CALLBACK_0(DragEndPosFall::pouring, this)), nullptr));
            }
            
        });
        
        _move->addListners(ComponentTouchCancle,[this](Node* _node,Component*,OperateListner*){
            dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->pauseUpdate();
            _node->setRotation(_startRote);
        });
        
        _rote->addListners(MoveDistancePercent,[this](Node* _node,Component*,OperateListner* _oper){
            float _percent = dynAny_cast<float>(_oper->getData());
            if(_percent<0)
                _percent =0;
            else if(_percent>1)
                _percent = 1;
            if(_startRote == -10000)
                _startRote = _node->getRotation();
            _node->setRotation(_endRote*_percent);
        });
        return true;
    }
    return false;
}

void DragEndPosFall::setTargetRect(const Rect& _rect) {
    dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->setTargetRect(_rect);
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setTarget(LQRect::create(_rect));
}
void DragEndPosFall::setCheckPoint(const Vec2& _pos){
    dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->setCheckPoint(_pos);
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setCheckPoint(_pos);
}

void DragEndPosFall::setInbow(Node* _node,ActionInterval* _action){
    if(_node != nullptr){
        _node->retain();
        CC_SAFE_RELEASE_NULL(_inbow);
        _inbow = _node;
    }
    
    if(_action != nullptr){
        _action->retain();
        CC_SAFE_RELEASE_NULL(_inbowAction);
        _inbowAction = _action;
    }
}

void DragEndPosFall::setEndRote(float f){
    _endRote =f;
}

void DragEndPosFall::setEndPos(const Vec2& end){
    this->endPos = end;
}

void  DragEndPosFall::fallEnd(){
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& _particle->isActive()){
        _particle->stopSystem();
    }
      //dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->startUpdate();
    stopSound();
    this->dispatcherEvent(FallThingfallEnd);
}

void DragEndPosFall::showPouring() {
    if(_owner != nullptr){
        auto _pour = _owner->getChildByName("pouring");
        if(_pour != nullptr){
            auto _shade = _owner->getChildByName("noshade");
            if(_shade != nullptr){
                _shade->setOpacity(0);
            }else {
                _owner->setCascadeOpacityEnabled(false);
                _owner->setOpacity(0);
            }
            _pour->setOpacity(255);
        }
    }
}
void DragEndPosFall::hidePouring(){
    if(_owner != nullptr){
        auto _pour = _owner->getChildByName("pouring");
        if(_pour != nullptr){
            auto _shade = _owner->getChildByName("noshade");
            if(_shade != nullptr){
                _shade->setOpacity(255);
            }else {
                _owner->setOpacity(255);
            }
            _pour->setOpacity(0);
        }
    }
}

void DragEndPosFall::pouring(){
    showPouring();
    auto innerNode = _owner->getChildByName("inner");
    if(innerNode != nullptr)
        innerNode->runAction(ScaleTo::create(1, 0.7,0.5));
    if(_inbowAction != nullptr) {
        auto _action = Sequence::create(CallFunc::create([this](){_inbow->setVisible(true);}), _inbowAction,CallFunc::create(CC_CALLBACK_0(DragEndPosFall::fallEnd,this)),nullptr);
        _inbow->runAction(_action);
        this->playSound();
        auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
        if(_particle != nullptr&& !_particle->isActive()){
            _particle->resetSystem();
        }
          this->dispatcherEvent(FallThingstartFall);
    }
}