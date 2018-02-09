//
//  DragFallComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DragFallComponent.h"
#include "MoveHideShade.h"
IMPLEMENT_COCOS2DX_CLASS(DragFallComponent);

DragFallComponent::DragFallComponent():
_inbow(nullptr)
,_inbowAction(nullptr)
,_action(nullptr)
,_endRote(0)
,_startRote(-10000)
,isPouring(false)
,innerNode(nullptr){
    
}

DragFallComponent::~DragFallComponent() {
    CC_SAFE_RELEASE_NULL(_inbow);
    CC_SAFE_RELEASE_NULL(_inbowAction);
}

bool DragFallComponent::init(){
    if(LQComponent::init()){
        auto _move = MoveHideShade::create();
        _move->setMoveType(MoveComponentType::kTypeBack);
        addComponent(_move);
        
        _move->addListners(ComponentTouchMove,[_move,this](Node* _node,Component*,OperateListner*){
            if(_move->checkInlimit()){
                _move->setEnabled(false);
                
                _node->runAction(Sequence::create(RotateTo::create(0.2, _endRote) ,CallFunc::create(CC_CALLBACK_0(DragFallComponent::pouring, this)), nullptr));
            }
        });
        
        return true;
    }
    return false;
}

void DragFallComponent::pouring(){
    
    innerNode = _owner->getChildByName("inner");
    if(innerNode != nullptr)
        innerNode->runAction(ScaleTo::create(1, 0.7,0.5));
    if(_inbow != nullptr && _inbowAction != nullptr){
        if(_action == nullptr) {
            _action = Sequence::create(_inbowAction,CallFunc::create(CC_CALLBACK_0(DragFallComponent::fallEnd,this)),nullptr);
            _inbow->runAction(_action);
            this->playSound();
        } else
        {
            Director::getInstance()->getActionManager()->resumeTarget(_inbow);
        }
    }
    
    
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& !_particle->isActive()){
        _particle->resetSystem();
        showPouring();
    }
    this->dispatcherEvent(FallThingResume);
    
}

void DragFallComponent::setTargetRect(const Rect& _rect) {
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setTarget(LQRect::create(_rect));
}
void DragFallComponent::setCheckPoint(const Vec2& _pos){
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setCheckPoint(_pos);
}

void DragFallComponent::setInbow(Node* _node,ActionInterval* _action){
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

void DragFallComponent::setEndRote(float f){
    _endRote =f;
}

void  DragFallComponent::fallEnd(){
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& _particle->isActive()){
        _particle->stopSystem();
    }
    stopSound();
    this->dispatcherEvent(FallThingfallEnd);
}

void DragFallComponent::showPouring() {
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
void DragFallComponent::hidePouring(){
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