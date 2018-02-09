//
//  REScrollView.h
//  CSnowman
//
//  Created by QAMAC01 on 14-11-17.
//
//

#ifndef __CSnowman__REScrollView__
#define __CSnowman__REScrollView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class REScrollViewDelegate: ScrollViewDelegate
{
public:
	virtual void scrollViewTouchEnd(Vec2 worldPoint){};
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};

};


class REScrollView : public cocos2d::extension::ScrollView
{
public:
    static REScrollView* create(Size size);
    void setDelegate(REScrollViewDelegate* pDelegate) { m_pREScrollViewDelegate = pDelegate; }

    
    
protected:
    virtual bool onTouchBegan(Touch *touch, Event *event) override;
    virtual void onTouchMoved(Touch *touch, Event *event) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;
    REScrollViewDelegate *m_pREScrollViewDelegate;

    
};

#endif /* defined(__CSnowman__REScrollView__) */
