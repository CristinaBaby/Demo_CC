//
//  MixToolOperate.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/21.
//
//

#include "MixToolOperate.h"
#include "DistanceToTarget.h"

 const string MixToolOperate::operatemixing = "operatemixing";
  const string MixToolOperate::operatemixpause = "operatemixpause";

IMPLEMENT_COCOS2DX_CLASS(MixToolOperate);

void MixToolOperate::start(){
    if(mixLayer != nullptr){
          _owner->getParent()->reorderChild(_owner, 5);
        dispatcherEvent(operatemixing);
      
        playSound();
        auto _timerChange = dynamic_cast<TimerChangeComponent*>(mixLayer->getComponent("TimerChangeComponent"));
        if(_timerChange != nullptr)
            _timerChange->start();
        auto _operateNode = _owner->getChildByName("operating");
        if(_operateNode != nullptr){
            auto _shade = _owner->getChildByName("noshade");
            if(_shade!=nullptr){
                _shade->setOpacity(0);
            }else{
                _owner->setCascadeOpacityEnabled(false);
                _owner->setOpacity(0);
            }
            _operateNode->setOpacity(255);
            if(operating != nullptr){
                if(operating->getTarget() != _operateNode)
                    _operateNode->runAction(operating);
                else
                    Director::getInstance()->getActionManager()->resumeTarget(_operateNode);
            }
        }
    }

}
void MixToolOperate::stop(){
    if(mixLayer != nullptr){
        pauseSound();
         _owner->getParent()->reorderChild(_owner,500);
         dispatcherEvent(operatemixpause);
        auto _timerChange = dynamic_cast<TimerChangeComponent*>(mixLayer->getComponent("TimerChangeComponent"));
        if(_timerChange != nullptr)
            _timerChange->stop();
    }
    auto _operateNode = _owner->getChildByName("operating");
    if(_operateNode != nullptr){
        auto _shade = _owner->getChildByName("noshade");
        if(_shade!=nullptr){
            _shade->setOpacity(255);
        }else{
            _owner->setOpacity(255);
        }
        _operateNode->setOpacity(0);
        if(operating != nullptr)
            Director::getInstance()->getActionManager()->pauseTarget(_operateNode);
        
    }
}

MixToolOperate::MixToolOperate(){
    _startRote = -10000;
    _endRote = 0;
    _startScale = -10000;
    _endScale = 1;
    mixLayer = nullptr;
    operating = nullptr;
    isPlaying = false;
}

MixToolOperate::~MixToolOperate(){
    CC_SAFE_RELEASE_NULL(operating);
}

bool MixToolOperate::MixToolOperate::init(){
    if(LQComponent::init()){
        auto _move = MoveHideShade::create();
        _move->setMoveType(MoveComponentType::kTypeBack);
        addComponent(_move);
        
        auto _rote = DistanceToTarget::create();
        addComponent(_rote);
        
        _move->addListners(ComponentTouchMove,[this,_move](Node* _node,Component*,OperateListner*){
            dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->startUpdate();
                if(dynamic_cast<MoveHideShade*>(_move)->checkInlimit())
                    start();
                else
                    stop();
        });
        
        _move->addListners(ComponentTouchEnd,[this](Node* _node,Component* _com,OperateListner*){
            dynamic_cast<MoveHideShade*>(_com)->backToStart();
            stop();
        });
        _move->addListners(ComponentTouchCancle,[this](Node* _node,Component*,OperateListner*){
            dynamic_cast<DistanceToTarget*> (this->getComponent("DistanceToTarget"))->pauseUpdate();
            if(_startRote != -10000)
                _node->setRotation(_startRote);
            else if(_startScale != -10000)
                _node->setScale(_startScale);
              stop();
        });
        
        _rote->addListners(MoveDistancePercent,[this](Node* _node,Component*,OperateListner* _oper){
            float _percent = dynAny_cast<float>(_oper->getData());
            if(_percent<0)
                _percent =0;
            else if(_percent>1)
                _percent = 1;
            if(_startRote == -10000)
                _startRote = _node->getRotation();
            if(_startScale == -10000)
                _startScale = _node->getScale();
            _node->setRotation(_endRote*_percent);
             _node->setScale(_startScale+ (_endScale-_startScale)*_percent);
        });
        return true;
    }
    return false;
}

void MixToolOperate::setEndRote(float f){
    _endRote =f;
}
void MixToolOperate::setEndScale(float f){
    _endScale = f;
}
void MixToolOperate::setMixLayer(Node* _node){
    mixLayer = _node;
}

void MixToolOperate::setTarget(const Rect& _rect){
    dynamic_cast<MoveHideShade*>(getComponent("MoveHideShade"))->setTarget(LQRect::create(_rect));
    dynamic_cast<DistanceToTarget*>(getComponent("DistanceToTarget"))->setTargetRect(_rect);
}
void MixToolOperate::setCheckPoint(Vec2 v){
    dynamic_cast<MoveHideShade*>(getComponent("MoveHideShade"))->setCheckPoint(v);
    dynamic_cast<DistanceToTarget*>(getComponent("DistanceToTarget"))->setCheckPoint(v);
}

void MixToolOperate::setOperating(ActionInterval* _a){
    CC_SAFE_RETAIN(_a);
    CC_SAFE_RELEASE_NULL(operating);
    operating = _a;
}
