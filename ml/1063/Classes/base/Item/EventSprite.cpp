//
//  EventSprite.cpp
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/6/15.
//
//

#include "EventSprite.h"
USING_NS_KD;
EventSprite::EventSprite(){
    
    touchSpriteBegan = nullptr;
    touchSpriteMove = nullptr;
    touchSpriteEnded = nullptr;
    touchSpriteCancle = nullptr;
    isSwallowTouches = false;
    enableFalg = true;
    touchListener = nullptr;
    isLockMove = false;
    backEndToOrn = nullptr;
}

EventSprite::~EventSprite() {
    reAddListner();
    CC_SAFE_RELEASE_NULL(touchListener);
}

void EventSprite::setLockMove(bool f) {
    isLockMove = f;
}

void EventSprite::setTouchProtity(int i){
    if(touchListener!=nullptr){
        Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener,i);
    }
}

void EventSprite::onEnterTransitionDidFinish(){
    Sprite::onEnterTransitionDidFinish();
    if(nullptr == touchListener) {
        touchListener = createListner();
        setSwallowTouches(isSwallowTouches);
        touchListener->setEnabled(enableFalg);
        touchListener->retain();
    }
    reAddListner();
}

EventListener* EventSprite::createListner() {
    auto  touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(EventSprite::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(EventSprite::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(EventSprite::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(EventSprite::onTouchCancle, this);
    return touchListener;
}

void EventSprite::onExitTransitionDidStart(){
    removeListener();
    Sprite::onExitTransitionDidStart();
}

void EventSprite::removeListener() {
    _eventDispatcher->removeEventListener(touchListener);
}

void EventSprite::reAddListner() {
    if(nullptr != touchListener) {
        _eventDispatcher->removeEventListener(touchListener);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }
}

bool EventSprite::initWithNoCache(const std::string& filename){
    Image * image = nullptr;
    Texture2D* tex = nullptr;
    bool flag = false;
    do {
        image = new Image();
        CC_BREAK_IF(nullptr == image);
        bool bImage = image->initWithImageFile(filename);
        CC_BREAK_IF(!bImage);
        tex = new Texture2D();
        CC_BREAK_IF(nullptr == tex);
        bool bTex = tex->initWithImage(image);
        CC_BREAK_IF(!bTex);
        bool bSprite = initWithTexture(tex);
        CC_BREAK_IF(!bSprite);
        flag = true;
    }while (0);
    CC_SAFE_RELEASE_NULL(image);
    CC_SAFE_RELEASE_NULL(tex);
    return flag;
}

EventSprite* EventSprite::createWithTex(Texture2D* tex){
    EventSprite *sprite = new (std::nothrow) EventSprite();
    if (sprite && sprite->initWithTexture(tex))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

EventSprite* EventSprite::create(const std::string& filename){
    auto *sprite = new (std::nothrow) EventSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}

 EventSprite* EventSprite::createWithSpriteFrame(SpriteFrame* sp) {
     auto *sprite = new (std::nothrow) EventSprite();
     if (sprite && sprite->initWithSpriteFrame(sp))
     {
         sprite->autorelease();
         return sprite;
     }
     CC_SAFE_DELETE(sprite);
     return nullptr;

}

EventSprite* EventSprite::createWithNoCache(const std::string &filename){
    auto *sprite = new (std::nothrow) EventSprite();
    if (sprite && sprite->initWithNoCache(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void EventSprite::setSwallowTouches(bool flag){
    isSwallowTouches = flag;
    auto touchListener = dynamic_cast<EventListenerTouchOneByOne*>(this->touchListener);
    if(touchListener!=nullptr)
        touchListener->setSwallowTouches(flag);
}

bool EventSprite::onTouchBegan(Touch *pTouch, Event *pEvent) {
    if(!isContain(pTouch)||!isVisible())
        return false;
    if(touchSpriteBegan != nullptr){
        bool b =  touchSpriteBegan(this,_tag,pTouch);
        return b;
    }
    return true;
}
void EventSprite::onTouchMoved(Touch *pTouch, Event *pEvent) {
    if(isContain(pTouch)&& touchSpriteMove != nullptr&&!isLockMove)
        touchSpriteMove(this,_tag,pTouch);
}

void EventSprite::onTouchEnded(Touch *pTouch, Event *pEvent) {
    isLockMove = false;
    if(touchSpriteEnded != nullptr&&isContain(pTouch))
       touchSpriteEnded(this,_tag,pTouch);
    else if(touchSpriteCancle != nullptr&&!isContain(pTouch))
        touchSpriteCancle(this,this->getTag(),pTouch);
}

void EventSprite::onTouchCancle(Touch *pTouch, Event *pEvent){
    isLockMove = false;
    if(touchSpriteCancle != nullptr)
        touchSpriteCancle(this,this->getTag(),pTouch);
}


void EventSprite::setEnable(bool flag){
    enableFalg = flag;
    if(touchListener!=nullptr)
        touchListener->setEnabled(flag);
}

bool EventSprite::isEnable(){
    if(touchListener!=nullptr)
        return touchListener->isEnabled();
    return false;
}

 bool EventSprite::isRequsetMove(){
    return false;
}

bool EventSprite::isContain(Touch *pTouch) {
    Rect limit = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    if(limit.containsPoint(this->convertToNodeSpace(pTouch->getLocation())))
        return true;
    return false;
}

void EventSprite::savePosition() {
    
}