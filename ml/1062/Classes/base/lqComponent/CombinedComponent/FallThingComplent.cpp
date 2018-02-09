//
//  FallThingComplent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "FallThingComplent.h"


#include "ui/CocosGUI.h"
IMPLEMENT_COCOS2DX_CLASS(FallThingComplent);
FallThingComplent::FallThingComplent():
_moveEndPos(Vec2::ZERO)
,_moveEndPath("")
,_fallingPath(""){
    _moveType = MoveComponentType::kTypeBack;
}
FallThingComplent::~FallThingComplent(){
    
}

 void FallThingComplent::accleCallBack(Node* _node,Component*,OperateListner* _oper){

          getComponent("TouchAccleRoteComponent")->setEnabled(false);
         if(!_fallingPath.empty()){
    
           auto _image = dynamic_cast<ui::ImageView*>(_owner);
             if(_image != nullptr){
                 auto  _s = Sprite::create(_fallingPath);
                 _s->setPosition(_owner->getContentSize()*.5);
                 _owner->addChild(_s);
              
                 _image->getVirtualRenderer()->setOpacity(0);
             }
             else{
                 auto _sprite = dynamic_cast<Sprite*>(_owner);
                 if(_sprite != nullptr)
                     _sprite->setSpriteFrame(Sprite::create(_fallingPath)->getSpriteFrame());
             }
         }
         _node->runAction(Sequence::create(RotateTo::create(0.2, _fallingRote),CallFunc::create([&](){
             
              dispatcherEvent(FallThingstartFall);
         }), nullptr));
     
}

 bool FallThingComplent::init(){
     if(TouchMoveComponent::init()){
         auto _accleComoplent = TouchAccleRoteComponent::create();
         _accleComoplent->setEnabled(false);
         _accleComoplent->addListners(ACCLE_ROTE_END, CC_CALLBACK_3(FallThingComplent::accleCallBack,this));
         addComponent(_accleComoplent);
         return true;
     }
     return false;
}

void FallThingComplent::dispatcherEvent(string _eventName){
    if(_eventName == ComponentTouchEnd){
        setEnabled(false);
        if(!_moveEndPath.empty()){
       
       
             auto _image = dynamic_cast<ui::ImageView*>(_owner);
            if(_image != nullptr){
                auto  _s = Sprite::create(_moveEndPath);
                _s->setPosition(_owner->getContentSize()*.5);
                _owner->addChild(_s);
                _image->getVirtualRenderer()->setOpacity(0);
            }
            else{
                auto _sprite = dynamic_cast<Sprite*>(_owner);
                if(_sprite != nullptr)
                    _sprite->setSpriteFrame(Sprite::create(_moveEndPath)->getSpriteFrame());
            }
        }
        _owner->runAction(Sequence::create(MoveTo::create(0.2, _moveEndPos), CallFunc::create([&](){
            dispatcherEvent(FallThingMoveEnd);
            this->getComponent("TouchAccleRoteComponent")->setEnabled(true);
        }), nullptr));
    }
    TouchMoveComponent::dispatcherEvent(_eventName);
}

