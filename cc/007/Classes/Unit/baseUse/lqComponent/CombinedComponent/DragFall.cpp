//
//  DragFall.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DragFall.h"
#include "MoveHideShade.h"
#include "TouchAccleRoteComponent.h"
using namespace cocostudio;
using namespace timeline;
const std::string DragFall::fallingAnimation = "pouring";

IMPLEMENT_COCOS2DX_CLASS(DragFall);

DragFall::DragFall():
_needGravity(false),
_frameStatus(nullptr)
,_moveToTargetTime(0.4)
,_autoRemoveFrameStatus(true){
    
}

DragFall::~DragFall() {
    CC_SAFE_RELEASE_NULL(_frameStatus);
}

bool DragFall::init(){
    if(LQComponent::init()){
        auto _move = MoveHideShade::create();
        _move->setMoveType(MoveComponentType::kTypeBack);
        addComponent(_move);
        
        addListners(FallThingfallEnd, [this](LQComponent*,OperateListner*){fallEnd();});
        
        _move->addListners(ComponentTouchMove,[this,_move](LQComponent*,OperateListner* lis){
            if(_move->checkInlimit()){
                _move->setEnabled(false);
               // lis->setEnable(false);
                _move->getOwner()->stopAllActions();
                Vector<FiniteTimeAction*> arrays;
                if(_needGravity){
                    
                    auto frames = _frameStatus->searchFrame(fallingAnimation);
                    float fRotate = _move->getOwner()->getRotation();
                    
                    for(auto pFrame:frames){
                        auto pos = dynamic_cast<PositionFrame*>(pFrame);
                        if(pos != nullptr) {
                            arrays.pushBack(MoveTo::create(_moveToTargetTime, pos->getPosition()));
                        } else {
                            auto rotate = dynamic_cast<RotationFrame*>(pFrame);
                            if(rotate != nullptr){
                                fRotate = rotate->getRotation();
                            } else {
                                auto scale = dynamic_cast<ScaleFrame*>(pFrame);
                                if(scale != nullptr){
                                    arrays.pushBack(ScaleTo::create(_moveToTargetTime, scale->getScaleX(), scale->getScaleY()));
                                } else {
                                    auto sckew = dynamic_cast<SkewFrame*>(pFrame);
                                    if(sckew != nullptr) {
                                        if(abs(sckew->getSkewX() - sckew->getSkewY())<1){
                                            fRotate = sckew->getSkewX();
                                        }
                                        else
                                            arrays.pushBack(SkewTo::create(_moveToTargetTime, sckew->getSkewX(), sckew->getSkewY()));
                                    }
                                }
                            }
                        }
                    }
                    fRotate = (int)fRotate%360;
                    if(fRotate>180)
                        fRotate = fRotate-360;
                    else if(fRotate<-180)
                        fRotate = fRotate+360;
                    auto touchAccel = TouchAccleRoteComponent::create();
                    if(fRotate>0)
                        touchAccel->setEndRote(std::min(fRotate, 55.0f));
                    else
                        touchAccel->setEndRote(std::max(fRotate, -55.0f));
                    addComponent(touchAccel);
                    touchAccel->setEnabled(false);
                    arrays.pushBack(CallFunc::create([=](){
                        touchAccel->setEnabled(true);
                        touchAccel->addListners(ACCLE_ROTE_END, [=](LQComponent*,OperateListner* lis){
                            lis->setEnable(false);
                            touchAccel->setEnabled(false);
                            touchAccel->setEndRote(false);
                            auto action = _frameStatus->createAction(0.6, "pourEnd");
                            if( action!= nullptr)
                                _owner->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(DragFall::pouring, this)),_frameStatus->createAction(0.6, "pourEnd"),Spawn::create(Sequence::create(MoveBy::create(0.6, Vec2(-30, 0)), MoveBy::create(0.6, Vec2(40, 0)), nullptr), RotateBy::create(1.2, -10), nullptr), nullptr));
                            
                            else
                                _owner->runAction(Sequence::create(RotateTo::create(0.3, fRotate),CallFunc::create(CC_CALLBACK_0(DragFall::pouring, this)), nullptr));
                        });
                    }));
                    _move->getOwner()->runAction(Sequence::create(arrays));
                }else {
                    auto falling = _frameStatus->createAction(_moveToTargetTime, fallingAnimation);
                    if(falling != nullptr)
                        arrays.pushBack(falling);
                    arrays.pushBack(CallFunc::create(CC_CALLBACK_0(DragFall::pouring, this)));
                    _move->getOwner()->runAction(Sequence::create(arrays));
                }
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


void DragFall::onAdd(){
    LQComponent::onAdd();
    if(_frameStatus == nullptr)
        setFrameStatus(FrameStatus::create(_owner->getName()));
    _owner->setOnExitCallback([this](){
        if(_autoRemoveFrameStatus&&_frameStatus != nullptr)
            _frameStatus->remove();
    });
}


void  DragFall::fallEnd(){
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& _particle->isActive()){
        _particle->stopSystem();
    }
    stopSound();
}

void DragFall::showPouring() {
    if(_owner != nullptr){
        auto _pour = _owner->getChildByName("pouring");
        if(_pour != nullptr){
            auto _shade = _owner->getChildByName("noshade");
            auto moving = _owner->getChildByName("moving");
            if(_shade != nullptr){
                _shade->setOpacity(0);
            }else if(moving != nullptr){
                moving->setOpacity(0);
            } else{
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
            auto moving = _owner->getChildByName("moving");
            if(_shade != nullptr){
                _shade->setOpacity(255);
            }else  if(moving != nullptr){
                moving->setOpacity(255);
            } else{
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
    this->playSound();
    auto _particle = dynamic_cast<ParticleSystemQuad*>(_owner->getChildByName("particle"));
    if(_particle != nullptr&& !_particle->isActive()){
        _particle->resetSystem();
    }
    this->dispatcherEvent(FallThingstartFall);
    
}