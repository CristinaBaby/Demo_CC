
#include "MixPoint.h"


void MixPoint::setPointPath(const string& s){
    pointPath = s;
}

void MixPoint::run(float time) {
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

void MixPoint::countMove(float dt)
{    
    if(!isScheduled(schedule_selector(MixPoint::updateCount))){
        this->schedule(schedule_selector(MixPoint::updateCount), dt);        
    }
}

void MixPoint::stopCountMove()
{
    unschedule(schedule_selector(MixPoint::updateCount));
}
void MixPoint::updateCount(float)
{
    if (countMoveCallback) {
        countMoveCallback();
    }
}
void MixPoint::crateFunc(){
    this->setRotation(this->getRotation()+10);
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
            
            src.rotate(Vec2::ZERO, angle);
            s->runAction(Sequence::create(MoveBy::create(time, src),RemoveSelf::create(),nullptr));
            s->runAction(FadeTo::create(time, 0));
        }
        
    }
}

MixPoint::MixPoint():pointPath("")
,distanceToCenter(150)
,delayTime(0.0)
,acTionTag(10)
,currentTime(0){
    countMoveCallback = nullptr;
}

bool MixPoint::init() {
    if(Node::init()) {
        setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return true;
    }
    return false;
}