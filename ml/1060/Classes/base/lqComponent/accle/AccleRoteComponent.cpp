//
//  AccleRoteComponent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#include "AccleRoteComponent.h"
IMPLEMENT_COCOS2DX_CLASS(AccleRoteComponent);

AccleRoteComponent::AccleRoteComponent(){
    _magnification = 90;
    _originalRote = -10000000000;
    _rotedDirection = ACCLE_ROTE_EVENT::ACCLE_ROTE_CW;
    _callbackType = ACCLE_ROTE_BACK;
    _endRote = -55;
}



 void AccleRoteComponent::dispatcherRote(float _rote){
     float _newRote = _rote;
     switch (_rotedDirection) {
         case ACCLE_ROTE_EVENT::ACCLE_ROTE_CW:{
             if(_newRote<_originalRote)
                 _newRote = _originalRote;
             else if(_newRote>=_endRote){
                 _newRote = _endRote;
                 if(_callbackType != ACCLE_ROTE_END){
                     _callbackType = ACCLE_ROTE_END;
                     dispatcherEvent(ACCLE_ROTE_END);
                 } else
                     dispatcherEvent(ACCLE_ROTE_NULL);
             } else if(abs(_newRote-_endRote)<=5&& _callbackType != ACCLE_ROTE_BACK){
                 _callbackType = ACCLE_ROTE_BACK;
                  dispatcherEvent(ACCLE_ROTE_BACK);
             } else
                  dispatcherEvent(ACCLE_ROTE_NULL);
             break;
         }
         case ACCLE_ROTE_EVENT::ACCLE_ROTE_CCW: {
             if(_newRote>_originalRote)
                 _newRote = _originalRote;
             else if(_newRote<=_endRote){
                 _newRote = _endRote;
                 if(_callbackType != ACCLE_ROTE_END){
                     _callbackType = ACCLE_ROTE_END;
                      dispatcherEvent(ACCLE_ROTE_END);
                 } else
                      dispatcherEvent(ACCLE_ROTE_NULL);
             } else if(abs(_newRote-_endRote)<=5&& _callbackType !=ACCLE_ROTE_BACK){
                 _callbackType = ACCLE_ROTE_BACK;
                  dispatcherEvent(ACCLE_ROTE_BACK);
             }else
                 dispatcherEvent(ACCLE_ROTE_NULL);
             break;
         }
         default:
             break;
     }
     if(_owner != nullptr)
     _owner->setRotation(_newRote);
     
}

void  AccleRoteComponent::dispatcherAccelEvent(Acceleration * _acceleration, Event * _event){
    if(_enabled&& _owner != nullptr) {
        if(_originalRote == -10000000000)
            _originalRote = _owner->getRotation();
        if(_originalRote>_endRote)
            _rotedDirection = ACCLE_ROTE_EVENT::ACCLE_ROTE_CCW;
        else
            _rotedDirection = ACCLE_ROTE_EVENT::ACCLE_ROTE_CW;
        dispatcherRote(_originalRote+_acceleration->x*_magnification);
    }
}