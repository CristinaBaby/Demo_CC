//
//  DragFryCommonent.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#include "DragFryCommonent.h"


IMPLEMENT_COCOS2DX_CLASS(DragFryCommonent);

DragFryCommonent::DragFryCommonent():
fryLimit(nullptr),
endLimit(nullptr)
,time(7.0f)
,currentTime(0)
,count(-1)
,isOk(false)
,bubble(nullptr)
,_progress(nullptr){
    
}
DragFryCommonent::~DragFryCommonent() {
    CC_SAFE_RELEASE_NULL(fryLimit);
    CC_SAFE_RELEASE_NULL(endLimit);
    CC_SAFE_RELEASE_NULL(_progress);
}
bool DragFryCommonent::init(){
    if(LQComponent::init()) {
        auto pMove = TouchMoveComponent::create();
        pMove->setMoveType(MoveComponentType::kTypeBack);
        pMove->addListners(ComponentTouchEnd, CC_CALLBACK_2(DragFryCommonent::touchOperate,this));
        pMove->addListners(ComponentTouchCancle, CC_CALLBACK_2(DragFryCommonent::touchOperate,this));
        pMove->addListners(ComponentTouchMove, CC_CALLBACK_2(DragFryCommonent::touchOperate,this));
        addComponent(pMove);
        return true;
    }
    return false;
}

void DragFryCommonent::onAdd(){
    LQComponent::onAdd();
    _owner->setCascadeOpacityEnabled(false);
    dynamic_cast<TouchMoveComponent*>(_owner->getComponent("TouchMoveComponent"))->setTarget(fryLimit);
    for(auto i=0;i<fryNodes.size();i++){
        auto _node = fryNodes.at(i);
        if(_node->getParent() == nullptr){
            _node->setPosition(_owner->getContentSize()*.5);
            _owner->addChild(_node,-i);
        }
    }
    if(_progress != nullptr&&_progress->getParent() == nullptr)
        _owner->addChild(_progress,30);
}

void DragFryCommonent::onRemove(){
    if(isSchedule(schedule_selector(DragFryCommonent::fryUpdate)))
        this->unSchedule(schedule_selector(DragFryCommonent::fryUpdate));
    LQComponent::onRemove();
}

void DragFryCommonent::setFryLimit(LQPolygon *pL){
    pL->retain();
    CC_SAFE_RELEASE_NULL(fryLimit);
    fryLimit  = pL;
    TouchMoveComponent* _operat = nullptr;
    if(_owner != nullptr)
        _operat = dynamic_cast<TouchMoveComponent*>(_owner->getComponent("TouchMoveComponent"));
    if(_operat != nullptr)
        _operat->setTarget(fryLimit);
}
void DragFryCommonent::setEndlimit(LQPolygon *pL) {
    pL->retain();
    CC_SAFE_RELEASE_NULL(endLimit);
    endLimit  = pL;
}

void DragFryCommonent::setFryPath(const vector<string>& fryPath){
    for(auto i=0;i<fryNodes.size();i++){
        auto _node = fryNodes.at(i);
        if(_node->getParent() != nullptr)
            _node->removeFromParent();
        
    }
    fryNodes.clear();
    for(auto i=0;i<fryPath.size();i++){
        auto _node = Sprite::create(fryPath.at(i));
        _node->setOpacity(0);
        fryNodes.pushBack(_node);
        if(_owner != nullptr){
            _node->setPosition(_owner->getContentSize()*.5);
            _owner->addChild(_node,-i-1);
        }
    }
}

void DragFryCommonent::setLoadingBar( ui::LoadingBar* _progress){
    _progress->retain();
    _progress->setVisible(false);
    if(_progress != nullptr)
        _progress->removeFromParent();
    CC_SAFE_RELEASE_NULL(this->_progress);
    this->_progress = _progress;
    if(_owner!=nullptr&& this->_progress->getParent() == nullptr)
        _owner->addChild(this->_progress,30);
}

void DragFryCommonent::setFryTime(float _time){
    this->time = _time;
}

void DragFryCommonent::setBubblePath(const vector<string>& Path){
    bubblePath = Path;
}

void DragFryCommonent::startFry(){
    if(!isSchedule(schedule_selector(DragFryCommonent::fryUpdate))){
           playSound();
        this->schedule(schedule_selector(DragFryCommonent::fryUpdate),0.0f);
        if(bubblePath.size()>0&&bubble == nullptr){
            bubble = Sprite::create(bubblePath.at(0));
            bubble->setPosition(_owner->getContentSize()*.5);
            _owner->addChild(bubble,-10);
            Animation* _anim = Animation::create();
            _anim->setDelayPerUnit(0.15);
            _anim->setLoops(-1);
            for(auto _itor = bubblePath.begin();_itor != bubblePath.end();_itor++){
                _anim->addSpriteFrameWithFile(*_itor);
            }
            bubble->runAction(Animate::create(_anim));
        }
        if(bubble != nullptr){
            bubble->setVisible(true);
            Director::getInstance()->getActionManager()->resumeTarget(bubble);
        }
        if(_progress != nullptr)
            _progress->setVisible(true);
        dispatcherEvent(FryOperateStart);
    }
}
void DragFryCommonent::stopFry(){
    if(isSchedule(schedule_selector(DragFryCommonent::fryUpdate))){
        pauseSound();
        this->unSchedule(schedule_selector(DragFryCommonent::fryUpdate));
        if(bubble != nullptr){
            Director::getInstance()->getActionManager()->pauseTarget(bubble);
            bubble->setVisible(false);
        }
        if(_progress != nullptr)
            _progress->setVisible(false);
        dispatcherEvent(FryOperatePause);
    }
}

void DragFryCommonent::fryUpdate(float f){
    currentTime+=f;
    float _percent = currentTime/time;
    if(_percent>1.0f)
        _percent =1.0f;
    if(_progress != nullptr)
        _progress->setPercent(_percent*100);
    dispatcherEventWithData(fryOperateing, _percent);
    if(_percent<=0.75){
        auto  tcount = size_t(_percent/(0.75/(fryNodes.size()-1)));
        if(tcount != count && count<fryNodes.size()-1){
            ++count;
            fryNodes.at(count)->setOpacity(255);
            _owner->setCascadeOpacityEnabled(false);
            if(count == 0){
                _owner->runAction(FadeTo::create(0.4, 0));
            }else{
                fryNodes.at(count-1)->runAction(FadeTo::create(0.4, 0));
            
            }
            
            if(count == (fryNodes.size()-2)){
                isOk = true;
                auto pMove = dynamic_cast<TouchMoveComponent*>(_owner->getComponent("TouchMoveComponent"));
                pMove->setTarget(endLimit);
                pMove->setEnabled(true);
                dispatcherEvent(fryOperateOk);
            }
        }
    } else if(_percent >= 1.0f) {
           ++count;
        fryNodes.at(count)->setOpacity(255);
           _owner->getComponent("TouchMoveComponent")->setEnabled(false);
         stopFry();
        fryNodes.at(count-1)->runAction(Sequence::create( FadeTo::create(0.4, 0),CallFunc::create([this](){
        //    auto _s = dynamic_cast<Sprite*>(_owner);
//            if(_s != nullptr){
//                for (auto _child: fryNodes)
//                    _child->removeFromParent();
//                _s->setSpriteFrame(fryNodes.at(fryNodes.size()-1)->getSpriteFrame());
//                _owner->setOpacity(255);
//                fryNodes.clear();
//                
//            }
            this->stopSound();
            this->dispatcherEvent(fryOperateFailed);
        }),nullptr));
     
       
    }
}

void DragFryCommonent::touchOperate(LQComponent* _move,OperateListner* _lis){
    if(_lis->getKey() == ComponentTouchEnd){
        _move->setEnabled(false);
        if(!isOk) {
            auto _node = _move->getOwner();
            dynamic_cast<TouchMoveComponent*>(_move)->setOrnagnalPos(_node->getPosition());
            startFry();
        } else {
              this->stopSound();
            dispatcherEvent(FryOperateEnd);
        }
    } else if(_lis->getKey() == ComponentTouchMove) {
        if(isOk)
            stopFry();
    } else if(_lis->getKey() == ComponentTouchCancle){
        if(isOk)
            startFry();
    }
}

