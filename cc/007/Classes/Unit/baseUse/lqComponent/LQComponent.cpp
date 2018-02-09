//
//  AccleComplent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#include "LQComponent.h"

IMPLEMENT_COCOS2DX_CLASS(LQComponent);



 void LQComponent::dispatcherEventWithData(string _eventName,const DynAny& _any){
     if(_enabled&& nullptr != _listners){
         auto temp = *_listners;
         auto _itor = temp.begin();
         while ( _itor!= temp.end()) {
             if((*_itor)->getEnable() && (*_itor)->getKey().compare(_eventName) == 0){
                 (*_itor)->setData(_any);
                 auto _callBack = (*_itor)->getCallBack();
                 if(_callBack != nullptr)
                     _callBack(this,(*_itor));
             }
             _itor++;
         }
     }

}

 void LQComponent::dispatcherEvent(string _eventName){
     if(_enabled&&nullptr != _listners){
         auto temp = *_listners;
         auto _itor = temp.begin();
         while ( _itor!= temp.end()) {
             if((*_itor)->getEnable() && (*_itor)->getKey().compare(_eventName) == 0){
                 auto _callBack = (*_itor)->getCallBack();
                 if(_callBack != nullptr)
                     _callBack(this,(*_itor));
             }
             _itor++;
         }
     }
}

EventListener* LQComponent::createLis(){
    return nullptr;
}

LQComponent::LQComponent():
_componentLis(nullptr)
,_lisNode(nullptr)
,_childCompoent(nullptr)
,isRunning(false)
,_listners(nullptr)
,_sound(nullptr)
,isSoundPlaying(false),
_soundId(-1){
	_dispatcher = Director::getInstance()->getEventDispatcher();
    _dispatcher->retain();
    _scheduler = Director::getInstance()->getScheduler();
    _scheduler->retain();
   
   
}
LQComponent::~LQComponent(){
 
    CC_SAFE_RELEASE_NULL(_componentLis);
    CC_SAFE_RELEASE_NULL(_dispatcher);
    CC_SAFE_RELEASE_NULL(_scheduler);
    CC_SAFE_RELEASE_NULL(_lisNode);
    CC_SAFE_DELETE(_childCompoent);
    CC_SAFE_DELETE(_listners);
}

 bool LQComponent::init(){
     _lisNode = Node::create();
     _lisNode->retain();
     _lisNode->setonEnterTransitionDidFinishCallback(CC_CALLBACK_0(LQComponent::onResume, this));
     _lisNode->setonExitTransitionDidStartCallback(CC_CALLBACK_0(LQComponent::onPause, this));
     _name = getClassInfo()->getClassName();
     return true;
}

void LQComponent::onAdd(){
    Component::onAdd();
    auto _soundNode = ui::Helper::seekNodeByName(_owner, "Audio");
    if(_soundNode != nullptr)
         _sound = dynamic_cast<ComAudio*>(_soundNode->getComponent("Audio"));
    if(_lisNode->getParent() == nullptr){
        _owner->addChild(_lisNode);
    }
    if(_componentLis == nullptr){
        _componentLis =createLis();
        if(_componentLis != nullptr){
        _componentLis->setEnabled(_enabled);
        _componentLis->retain();
        }
    }
    if(_childCompoent != nullptr)
        for(auto _itore = _childCompoent->begin();_itore != _childCompoent->end();_itore++){
            if(_itore->second->getOwner() == nullptr)
                _owner->addComponent(_itore->second);
        }
}
void LQComponent::onRemove(){
    CC_SAFE_DELETE(_listners);
    if(_sound != nullptr&& _soundId !=-1){
        _sound->stopEffect(_soundId);
        _soundId = -1;
    }
    Component::onRemove();
    if(_lisNode != nullptr)
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_lisNode);
    _lisNode->removeFromParent();
    if(_childCompoent != nullptr)
        for(auto _itore = _childCompoent->begin();_itore != _childCompoent->end();_itore++)
            if(_itore->second->getOwner() != nullptr)
                _itore->second->getOwner()->removeComponent(_itore->second);
  
}

void LQComponent::onResume(){
    isRunning = true;
    _scheduler->resumeTarget(this);
}
void LQComponent::onPause(){
    isRunning = false;
    _scheduler->pauseTarget(this);
}

void LQComponent::setEnabled(bool b){
    Component::setEnabled(b);
    if(_componentLis != nullptr)
        _componentLis->setEnabled(_enabled);
    if(_childCompoent != nullptr)
        for(auto _itore = _childCompoent->begin();_itore != _childCompoent->end();_itore++)
            _itore->second->setEnabled(_enabled);
}

void LQComponent::setTouchPorit(int _por){
    if(_componentLis != nullptr){
        _dispatcher->removeEventListener(_componentLis);
        _dispatcher->addEventListenerWithFixedPriority(_componentLis, _por);
    }
}

void LQComponent::setDispatche(EventDispatcher* _dis){
    if(_dis!=nullptr&& _dis != _dispatcher){
        CC_SAFE_RETAIN(_dis);
        CC_SAFE_RELEASE_NULL(_dispatcher);
        _dispatcher = _dis;
    }
}

void LQComponent::addComponent(Component* _component){
    auto _mkC = dynamic_cast<LQComponent*>(_component);
    if(_mkC != nullptr){
        if(_childCompoent == nullptr)
            _childCompoent = new  Map<string,LQComponent*>;
        auto _itor = _childCompoent->find(_component->getName());
        if(_itor != _childCompoent->end())
            CCLOG("err =======%s已存在=====",_component->getName().c_str());
        else {
            _childCompoent->insert(_component->getName(), _mkC);
            if(_owner  != nullptr)
                _owner->addComponent(_mkC);
        }
        
    } else {
         CCLOG("err =======%s not LQComponent=====",_component->getName().c_str());
    }
}

LQComponent* LQComponent::getComponent(string _name){
    if(_childCompoent != nullptr){
        auto _itor = _childCompoent->find(_name);
        if(_itor != _childCompoent->end())
            return _itor->second;
    }
    return nullptr;
}

 void LQComponent::addListners(string name,ComponentCallFunc _callBack){
     auto _lis = OperateListner::create();
     _lis->setKey(name);
     _lis->setCallBack(_callBack);
     if(_listners == nullptr)
         _listners = new Vector<OperateListner*>();
     _listners->pushBack(_lis);
}

 void  LQComponent::addListners(OperateListner* _lis){
     if(_lis != nullptr) {
         if(_listners == nullptr)
             _listners = new Vector<OperateListner*>();
         _listners->pushBack(_lis);
     }
}

 void LQComponent::removeListner(OperateListner* _oper,bool flag){
     if(_listners != nullptr)
    _listners->eraseObject(_oper,flag);
     if(_listners->size() == 0)
         CC_SAFE_DELETE(_listners);
}

 void LQComponent::removeListner(string _eventName,bool removeAll ){
     if(_listners != nullptr){
         if (removeAll)
         {
             for (auto iter = _listners->begin(); iter != _listners->end();)
             {
                 if ((*iter)->getKey() == _eventName)
                 {
                     iter = _listners->erase(iter);
                 }
                 else
                 {
                     ++iter;
                 }
             }
         }
         else
         {
             auto iter = std::find_if(_listners->begin(), _listners->end(), [_eventName](OperateListner* _oper){
                 return _oper->getKey()== _eventName;
             });
             if (iter != _listners->end())
             {
                 _listners->erase(iter);
             }
         }
     }
     if(_listners->begin() == _listners->end())
         CC_SAFE_DELETE(_listners);
}

void LQComponent::scheduleOnce(SEL_SCHEDULE selector, float delay)
{
    this->schedule(selector, 0.0f, 0, delay);
}
void LQComponent::schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
{
    CCASSERT( selector, "Argument must be non-nil");
    CCASSERT( interval >=0, "Argument must be positive");
    
    _scheduler->schedule(selector, this, interval , repeat, delay, !isRunning);
}

void LQComponent::schedule(SEL_SCHEDULE selector, float interval)
{
    this->schedule(selector, interval, CC_REPEAT_FOREVER, 0.0f);
}

void LQComponent::unSchedule(SEL_SCHEDULE _s){
    _scheduler->unschedule(_s,this);
}
bool LQComponent::isSchedule(SEL_SCHEDULE _s){
    return _scheduler->isScheduled(_s, this);
}

void LQComponent::playSound(){
    if(_sound != nullptr){
        if(!isSoundPlaying){
            if(_sound->isLoop()){
                 isSoundPlaying = true;
            if(_soundId == -1){
                _soundId = _sound->playEffect();
            }else
                _sound->resumeEffect(_soundId);
            }else
                _soundId = _sound->playEffect();
    }
    }
}

void LQComponent::pauseSound(){
    if(_sound != nullptr&&isSoundPlaying&&_soundId != -1){
        isSoundPlaying = false;
        _sound->pauseEffect(_soundId);
    }
}
void LQComponent::stopSound(){
    if(_sound != nullptr){
        isSoundPlaying = false;
         _sound->stopEffect(_soundId);
        _soundId = -1;
       
    }
}

void LQComponent::changeSound(const string& _path){
    if(_sound != nullptr){
        _sound->setFile(_path.c_str());
    }
}