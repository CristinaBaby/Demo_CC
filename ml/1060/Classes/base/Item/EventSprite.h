//
//  EventSprite.h
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/6/15.
//
//

#ifndef __PizzaMaker__EventSprite__
#define __PizzaMaker__EventSprite__

#include <iostream>
#include"cocos2d.h"
#include "KDheader.h"
USING_NS_CC;
NS_KD_BEGIN
class EventSprite : public Sprite{
public:
    EventSprite();
   virtual ~EventSprite();
    static EventSprite* create(const std::string& filename);
    static EventSprite* createWithNoCache(const std::string& filename);
    static EventSprite* createWithTex(Texture2D* tex);
    static EventSprite* createWithSpriteFrame(SpriteFrame*);
    virtual void setEnable(bool flag);
    bool isEnable();
    void setSwallowTouches(bool flag);
    void setTouchProtity(int i);
    void removeListener();
    void reAddListner();
    void setLockMove(bool);
    virtual bool isRequsetMove();
    virtual  void savePosition();
    
    std::function<bool(EventSprite*,int tag,Touch*)> touchSpriteBegan;
    std::function<void(EventSprite*,int tag,Touch*)> touchSpriteMove;
    std::function<void(EventSprite*,int tag,Touch*)> touchSpriteEnded;
    std::function<void(EventSprite*,int tag,Touch*)> touchSpriteCancle;
    
    std::function<void(EventSprite*)> backEndToOrn;
protected:
    EventListener *touchListener;
   virtual bool isContain(Touch *pTouch);
    bool initWithNoCache(const std::string& filename);
    bool isSwallowTouches,enableFalg;
    bool isLockMove;
public:
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch *pTouch, Event *pEvent);
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    virtual EventListener* createListner();
};
NS_KD_END
#endif /* defined(__PizzaMaker__EventSprite__) */
