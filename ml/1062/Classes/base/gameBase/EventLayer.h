//
//  TesetLayer.h
//  MyCppGame
//
//  Created by luotianqiang1 on 12/29/14.
//
//

#ifndef __MyCppGame__TesetLayer__
#define __MyCppGame__TesetLayer__

#include"cocos2d.h"
USING_NS_CC;
class EventLayer : public LayerColor {
public:
    CREATE_FUNC(EventLayer);
    virtual bool init();
    void setTouchEnable(bool flag);
   virtual bool touchBegine(Touch * pTouch, Event*pEvent);
    virtual void touchEnd(Touch*pTouch,Event*pEvent);
    virtual void touchCancle(Touch*pTouch,Event*pEvent){}
    void setSwallowTouche(bool flag);
    std::function<void(EventLayer*, int,Touch*)> clicke;
protected:
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    EventListenerTouchOneByOne* mpListerner;
    bool _enable;
    bool isSw;
};

#endif /* defined(__MyCppGame__TesetLayer__) */
