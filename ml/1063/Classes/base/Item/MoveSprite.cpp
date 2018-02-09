//
//  MoveSprite.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/10/14.
//
//

#include "MoveSprite.h"
USING_NS_KD;
int moveBackActionTag = 98765;

MoveSprite::MoveSprite(){
    moveType = kMoveEndTouchStop;
    isBrintFront = true;
    isFisrt = true;
    isGetTouch = true;
    targetAnPoint = Vec2(0.5,0.5);
    limitRect = nullptr;
}

MoveSprite* MoveSprite::create()
{
    MoveSprite *sprite = new (std::nothrow) MoveSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MoveSprite* MoveSprite::create(const std::string& filename){
    MoveSprite *sprite = new (std::nothrow) MoveSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MoveSprite* MoveSprite::createWithSpriteFrame(SpriteFrame* sp){
    auto *sprite = new (std::nothrow) MoveSprite();
    if (sprite && sprite->initWithSpriteFrame(sp))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void MoveSprite::setPositionWithTarget(Vec2 point) {
    auto p = getParent()->convertToNodeSpace(point);
    Size s = getContentSize();
    Vec2 an = getAnchorPoint()-targetAnPoint;
    Vec2 distanP = Vec2(s.width*an.x, s.height*an.y);
    setPosition(p+distanP);
}

MoveSprite* MoveSprite::createWithNoCache(const std::string &filename){
    auto *sprite = new (std::nothrow) MoveSprite();
    if (sprite && sprite->initWithNoCache(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool MoveSprite::isInLimitRect(){
    auto position = getCheckPointInPoints();
    if(limitRect!=nullptr&& limitRect->containsPoint(position))
       return true;
    return  false;
}

void MoveSprite::savePosition(){
    orgiginal = this->getPosition();
}

void MoveSprite::setOrnPoint(Vec2 point) {
    orgiginal = point;
}


void MoveSprite::setMoveType(MoveSpriteType moveType){
    this->moveType = moveType;
}

bool MoveSprite::onTouchBegan(Touch *pTouch, Event *pEvent){
    if(isGetTouch&&EventSprite::onTouchBegan(pTouch, pEvent)){
         oldPosition = getPosition();
        if(isBrintFront){
            if(isFisrt){
                isFisrt = false;
                oldZorder = getZOrder();
            }
        getParent()->reorderChild(this, 500);
        }
        return true;
    }
    return false;
}
void MoveSprite::onTouchMoved(Touch *pTouch, Event *pEvent){
    if(isLockMove)
        return;
    Vec2 p =  getParent()->convertToNodeSpace(pTouch->getLocation());
    setPosition(getPosition()+p-oldPosition);;
    switch (moveType) {
        case kMoveEndTouchStop:
            break;
        case KMoveLimitInRect:
            if(!isInLimitRect()){
                setPosition(oldPosition);
                    return;
            }
            break;
        case kMoveEndBackStart:
     
            break;
        default:
            break;
    }
    oldPosition = p;
    EventSprite::onTouchMoved(pTouch, pEvent);
}

bool  MoveSprite::isRequsetMove(){
    return !isLockMove;
}


void MoveSprite::setTargetAnPoint(Vec2 point){
    this->targetAnPoint = point;
}

Vec2 MoveSprite::getCheckPointInPoints(){
    Size s = getContentSize();
//    Vec2 an = getAnchorPoint()-targetAnPoint;
//    Vec2 distanP = Vec2(s.width*an.x, s.height*an.y);
//   // getPosition()-distanP
    Vec2 we = Vec2(targetAnPoint.x*s.width,targetAnPoint.y*s.height);
    return convertToWorldSpace(we);
}

Vec2 MoveSprite::getCheckPointInPoints(Vec2 point){
    Size s = getContentSize();
    Vec2 an = getAnchorPoint()-targetAnPoint;
    Vec2 distanP = Vec2(s.width*an.x, s.height*an.y);
    return getParent()->convertToWorldSpace(point-distanP);
}

void MoveSprite::moveBackSchedule(float time){
    _moveTime += time;
    auto _p = getPosition() + distance*(time/0.2);
    if(_moveTime>=0.2){
         unschedule(schedule_selector(MoveSprite::moveBackSchedule));
        _p = orgiginal;
        if(backEndToOrn != nullptr)
            backEndToOrn(this);
        backEnd();
    }
    setPosition(_p);
}

void MoveSprite::moveBack(){
   //unschedule(schedule_selector(MoveSprite::moveBackSchedule));
    distance = orgiginal - getPosition();
    _moveTime = 0;
    isGetTouch = false;
    if(!isScheduled(schedule_selector(MoveSprite::moveBackSchedule)))
    schedule(schedule_selector(MoveSprite::moveBackSchedule));
}

void MoveSprite::backEnd() {
    if(isBrintFront)
        getParent()->reorderChild(this, oldZorder);
    isGetTouch = true;
}

void MoveSprite::onTouchEnded(Touch *pTouch, Event *pEvent){
    switch (moveType){
        case kMoveEndBackStart:
            if(!isInLimitRect()){
                onTouchCancle(pTouch,pEvent);
                return;
            }
            break;
        default:
            break;
    }
    EventSprite::onTouchEnded(pTouch, pEvent);
}



void MoveSprite::setBrintToTop(bool flag){
    this->isBrintFront = flag;
}

void MoveSprite::onTouchCancle(Touch *pTouch, Event *pEvent){
    moveBack();
    EventSprite::onTouchCancle(pTouch, pEvent);
}