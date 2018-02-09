//
//  TesetLayer.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 12/29/14.
//
//

#include "EventLayer.h"

bool EventLayer::init() {
    clicke = nullptr;
    initWithColor(Color4B(0,0,0,125));
    mpListerner = nullptr;
    _enable = true;
    isSw = true;
    return  true;
}

void EventLayer::setTouchEnable(bool flag){
    _enable = flag;
    if(nullptr!=mpListerner)
        mpListerner->setEnabled(flag);
}


 void EventLayer::onEnterTransitionDidFinish() {
    LayerColor::onEnterTransitionDidFinish();
     if(mpListerner == nullptr){
         mpListerner = EventListenerTouchOneByOne::create();
         mpListerner->onTouchBegan = CC_CALLBACK_2(EventLayer::touchBegine,this);
         mpListerner->onTouchEnded = CC_CALLBACK_2(EventLayer::touchEnd,this);
         mpListerner->onTouchCancelled = CC_CALLBACK_2(EventLayer::touchCancle,this);
         mpListerner->setSwallowTouches(isSw);
         mpListerner->setEnabled(_enable);
         _eventDispatcher->addEventListenerWithSceneGraphPriority(mpListerner, this);
     }
}
void EventLayer::setSwallowTouche(bool flag){
    isSw = flag;
    if(mpListerner != nullptr)
        mpListerner->setSwallowTouches(isSw);
}
void EventLayer::onExitTransitionDidStart(){
    if(mpListerner != nullptr){
        _eventDispatcher->removeEventListener(mpListerner);
        mpListerner = nullptr;
    }
    LayerColor::onExitTransitionDidStart();
    
}

bool EventLayer::touchBegine(Touch * pTouch, Event*pEvent){
    auto _point = convertToNodeSpace(pTouch->getLocation());
    Rect _rect;
    _rect.size = getContentSize();
    return isVisible()&& _rect.containsPoint(_point);
}
void EventLayer::touchEnd(Touch*pTouch,Event*pEvent){
    if(nullptr != clicke)
        clicke(this,_tag,pTouch);
}
