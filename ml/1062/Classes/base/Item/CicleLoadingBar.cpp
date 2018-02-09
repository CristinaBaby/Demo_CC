//
//  CicleLoadingBar.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/28.
//
//

#include "CicleLoadingBar.h"
CicleLoadingBar::CicleLoadingBar(){
    _starRote =0;
    endRote = 0;
}

CicleLoadingBar* CicleLoadingBar::create(string scr,string mask){
    auto _node = new CicleLoadingBar();
    _node->autorelease();
    auto _scr = Sprite::create(scr);
    Sprite* _mask;
    if(mask.empty()){
        _mask = Sprite::create(scr);
    }else
        _mask = Sprite::create(mask);
    _node->initSprite(_scr, _mask);
    _scr->setPosition(_scr->getContentSize()*.5);
    _node->setContentSize(_scr->getContentSize());
    return _node;
}

void CicleLoadingBar::setStartRote(float f){
    _starRote = f;
    maksSprite->setRotation(_starRote);
}

void CicleLoadingBar::setEndRote(float f){
    endRote = f;
}

void CicleLoadingBar::setPercent(float p){
    if(p>100)
        p = 100;
    else if(p<0)
        p =0;
    maksSprite->setRotation(_starRote+(endRote-_starRote)*p);
}

void CicleLoadingBar::resumAction() {
    if(isRunning||isEnd)
        return;
    srcSprite->stopActionByTag(actionTag);
    isRunning = true;
    auto time =  abs(endRote-maksSprite->getRotation())/speed;
    auto thisAction  = Sequence::create(RotateTo::create(time,0), CallFuncN::create(CC_CALLBACK_0(CicleLoadingBar::endAction,this)), nullptr);
    thisAction->setTag(actionTag);
    maksSprite->runAction(thisAction);
    
}