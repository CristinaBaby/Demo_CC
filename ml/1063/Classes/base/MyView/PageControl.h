//
//  PageControl.h
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/20/15.
//
//

#ifndef __PizzaMaker__PageControl__
#define __PizzaMaker__PageControl__

#include "MyScollView.h"
NS_KD_BEGIN
class PageControl : public MyScollView {
private:
    enum class MoveDirection{
        left,
        right,
        no
    };
public:
    PageControl();
    static PageControl*create(float with,float height);
    void showPage(size_t i,bool flag = false);
    void showPage(size_t i,float time);
    void setMinScale(float _s){minScale = _s;}
     virtual void setAdapter(MyBaseAdapter* base);
    void scaleNode(float time=0);
private:
    void  onTouchEnded(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
   bool onTouchBegan(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    MoveDirection moveDirection;
    Vec2 startPo;
    Size itemSize;
  
    EventSprite* curentPage;
    float minScale;
    EventSprite* adjustCurentPage(Touch*);
protected:
      virtual void onExitTransitionDidStart();
     void touchEnd(EventSprite* btn,int tag, Touch *pTouch);
    void showPage(EventSprite* page,bool flag = false);
    virtual void onEnterTransitionDidFinish();
public:
    std::function<void()> pageChangeing;
    std::function<void(int,EventSprite*)> changeEnd;
};
NS_KD_END
#endif /* defined(__PizzaMaker__PageControl__) */
