//
//  DragFallComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DragFallComponent.h"
#include "DistanceToTarget.h"
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
        
        auto _rote = DistanceToTarget::create();
        addComponent(_rote);
        
        _move->addListners(ComponentTouchMove,[this](Node* _node,Component*,OperateListner*){
            dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->startUpdate();
            
        });
        
        _move->addListners(ComponentTouchEnd,[this](Node* _node,Component* _com,OperateListner*){
            auto _particle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("particle"));
            if(_particle != nullptr&& _particle->isActive()){
                _particle->stopSystem();
            }
            dynamic_cast<MoveHideShade*>(_com)->backToStart();
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
            if(_percent >=1){
                if(!isPouring){
                    isPouring = true;
                    if(innerNode == nullptr){
                        innerNode = _node->getChildByName("inner");
                        if(innerNode != nullptr)
                            innerNode->runAction(ScaleTo::create(1, 0.7,0.5));
                    }else
                        Director::getInstance()->getActionManager()->resumeTarget(innerNode);
               
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
                    
                    auto _particle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("particle"));
                    if(_particle != nullptr&& !_particle->isActive()){
                        _particle->resetSystem();
                        showPouring();
                    }
                     //_owner->getParent()->reorderChild(_owner, 5);
                         this->dispatcherEvent(FallThingResume);
                }
            }else if(_percent<1){
                if(isPouring){
                    isPouring = false;
                    Director::getInstance()->getActionManager()->pauseTarget(_inbow);
                    if(innerNode != nullptr)
                         Director::getInstance()->getActionManager()->pauseTarget(innerNode);
                    auto _particle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("particle"));
                    if(_particle != nullptr&& _particle->isActive()){
                        _particle->stopSystem();
                        hidePouring();
                    }
                    // _owner->getParent()->reorderChild(_owner, 500);
                   this->dispatcherEvent(FallThingPause);
                }
            }
        });
        return true;
    }
    return false;
}

void DragFallComponent::setTargetRect(const Rect& _rect) {
    dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->setTargetRect(_rect);
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setTarget(LQRect::create(_rect));
}
void DragFallComponent::setCheckPoint(const Vec2& _pos){
    dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->setCheckPoint(_pos);
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
     //_owner->getParent()->reorderChild(_owner, 500);
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