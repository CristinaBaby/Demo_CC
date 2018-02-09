//
//  ScollView.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/4/14.
//
//

#ifndef __MyCppGame__MyScollView__
#define __MyCppGame__MyScollView__

#include <iostream>
#include"MyBaseAdapter.h"
#include"EventSprite.h"
#include "cocos-ext.h"
NS_KD_BEGIN
#define discardMove 10
class MyScollView : public extension::ScrollView {
public:
    /**visible width and heght**/
    static MyScollView * create(float w, float h);
    virtual void setAdapter(MyBaseAdapter* base);
    MyBaseAdapter *getAdapter();
    MyScollView();
	virtual ~MyScollView(){CC_SAFE_RELEASE_NULL(currAdpter);}
   void setSpace(float space);
    void setMarginLeft(float left);
    void setMarginRight(float right);
    void setMarginTop(float top);
    void setMarginBottom(float bottom);
    void changeAdapter(MyBaseAdapter* base);
    MyBaseAdapter* getCurrAdapter(){return currAdpter;}
    /**items**/
    Vector<EventSprite*> nodeCache;
   void updateButton();
protected:
    bool isMove;
    float space;
    float marginLeft,marginRight,marginTop,marginBottom;
    MyBaseAdapter *currAdpter;
	virtual void touchEnd(EventSprite* btn,int tag, Touch *pTouch);
    virtual bool touchDown(EventSprite* btn,int tag, Touch *pTouch);
	EventSprite* curentItem;
    virtual void onTouchMoved(Touch* touch, Event* event);
   virtual bool onTouchBegan(Touch *touch, Event *event);
   virtual void onTouchEnded(Touch *touch, Event *event);
   virtual void onTouchCancelled(Touch *touch, Event *event);
   virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void addChild(Node * child, int zOrder, const std::string &name);
    Layer* _unClipp;
   
};
NS_KD_END
#endif /* defined(__MyScollView__ScollView__) */
