//
//  DragFall.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DragFall.h"
#include "MoveHideShade.h"
const std::string DragFall::fallingAnimation = "pouring";

IMPLEMENT_COCOS2DX_CLASS(DragFall);

DragFall::DragFall():
_inbow(nullptr)
,_inbowAction(nullptr)
,_frameStatus(nullptr){
    
}

DragFall::~DragFall() {

    CC_SAFE_RELEASE_NULL(_inbow);
    CC_SAFE_RELEASE_NULL(_inbowAction);
    CC_SAFE_RELEASE_NULL(_frameStatus);
}

bool DragFall::init(){
    if(LQComponent::init()){
        auto _move = MoveHideShade::create();
        _move->setMoveType(MoveComponentType::kTypeBack);
        addComponent(_move);
        
        _move->addListners(ComponentTouchMove,[this,_move](Node* _node,Component*,OperateListner*){
            if(_move->checkInlimit()){
                _move->setEnabled(false);
                _node->stopAllActions();
                _node->runAction(Sequence::create(_frameStatus->createAction(0.4, fallingAnimation) ,CallFunc::create(CC_CALLBACK_0(DragFall::pouring, this)), nullptr));
            }
            
        });
        
        return true;
    }
    return false;
}

void DragFall::setTargetRect(const Rect& _rect) {
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setTarget(LQRect::create(_rect));
}
void DragFall::setCheckPoint(const Vec2& _pos){
    dynamic_cast<MoveHideShade*> (this->getComponent("MoveHideShade"))->setCheckPoint(_pos);
}

void DragFall::setInbow(Node* _node,ActionInterval* _action){
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

void DragFall::onAdd(){
    LQComponent::onAdd();
    if(_frameStatus == nullptr)
        setFrameStatus(FrameStatus::create(_owner->getName()));
    _owner->setOnExitCallback([this](){
        if(_frameStatus != nullptr)
            _frameStatus->remove();
    });
}


void  DragFall::fallEnd(){
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& _particle->isActive()){
        _particle->stopSystem();
    }
    stopSound();
    this->dispatcherEvent(FallThingfallEnd);
}

void DragFall::showPouring() {
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
void DragFall::hidePouring(){
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

void DragFall::pouring(){
    showPouring();
    auto innerNode = _owner->getChildByName("inner");
    if(innerNode != nullptr)
        innerNode->runAction(ScaleTo::create(1, 0.7,0.5));
    if(_inbowAction != nullptr) {
        auto _action = Sequence::create(CallFunc::create([this](){_inbow->setVisible(true);}), _inbowAction,CallFunc::create(CC_CALLBACK_0(DragFall::fallEnd,this)),nullptr);
        _inbow->runAction(_action);
        this->playSound();
        auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
        if(_particle != nullptr&& !_particle->isActive()){
            _particle->resetSystem();
        }
        this->dispatcherEvent(FallThingstartFall);
    }
}