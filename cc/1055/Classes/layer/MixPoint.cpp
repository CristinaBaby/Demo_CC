//
//  MixPoint.cpp
//  candyfactory
//
//  Created by luotianqiang1 on 16/4/25.
//
//

#include "MixPoint.h"


void MixPoint::setPointPath(const string& s){
    pointPath = s;
}

void MixPoint::run(float time) {
   // if(!getActionByTag(acTionTag)||abs(delayTime-time)>0.7) {
//        auto acton = Sequence::create(DelayTime::create(time),CallFunc::create(CC_CALLBACK_0(MixPoint::crateFunc, this)),nullptr);
//        acton->setTag(acTionTag);
//        this->runAction(acton);
   // }
    delayTime = time;
    if(!isScheduled(schedule_selector(MixPoint::updateTime)))
        this->schedule(schedule_selector(MixPoint::updateTime));
}
void MixPoint::stop(){
    stopAllActions();
    unschedule(schedule_selector(MixPoint::updateTime));
}

void MixPoint::updateTime(float t){
    currentTime+=t;
    if(currentTime>delayTime){
        currentTime = 0;
        crateFunc();
    }
}


void MixPoint::crateFunc(){
    auto angle = 0;
    int num = 10;
    for(int i=0;i<num;i++){
        auto s = Sprite::create(pointPath);
        if(s != nullptr ) {
            s->setRotation(rand()%360);
            s->setScale(0.5+rand()%5*0.1);
            angle = angle+360/(num*1.0);
            auto pos = Vec2(0,distanceToCenter-10+rand()%20+getContentSize().height*.5);
            pos.rotate(getContentSize()*.5, angle);
            s->setPosition(pos);
            addChild(s);
            float time = delayTime*0.5; // 0.5;//delayTime*(0.4+rand()%4*0.1);
            if(time>0.5)
                time = 0.5;
            else if(time<0.2)
                time = 0.2;
          
            auto src = Vec2(0,25);
            //auto direction = Vec2(pos - getContentSize()*.5);
            //auto moveByPos =  (src.dot(direction)/direction.getLengthSq())*Vec2(std::abs(direction.x),std::abs(direction.y));
            src.rotate(Vec2::ZERO, angle);
            s->runAction(Sequence::create(MoveBy::create(time, src),RemoveSelf::create(),nullptr));
            s->runAction(FadeTo::create(time, 0));
        }
        
    }
     CCLOG("delayTime====%f",delayTime);
//    auto acton = Sequence::create(DelayTime::create(delayTime),CallFunc::create(CC_CALLBACK_0(MixPoint::crateFunc, this)),nullptr);
//    acton->setTag(acTionTag);
//    this->runAction(acton);
}

MixPoint::MixPoint():pointPath("")
,distanceToCenter(150)
,delayTime(0.0)
,acTionTag(10)
,currentTime(0){
    
}

bool MixPoint::init() {
    if(Node::init()) {
        setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return true;
    }
    return false;
}