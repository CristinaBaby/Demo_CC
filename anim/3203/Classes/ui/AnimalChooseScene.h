//
// AnimalChooseScene.h
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#ifndef __ANIM3203__AnimalChooseScene__
#define __ANIM3203__AnimalChooseScene__

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class ViewPager;

class AnimalChooseScene : public BaseScene
{
public:
    AnimalChooseScene();
    ~AnimalChooseScene();

    static AnimalChooseScene* create();
    virtual bool init();
    
    void showAds() {
        m_bCross = true;
    }

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    PageView *m_pPageView;
    
    ViewPager *m_pViewPager;
    
    ImageView *m_pItem;
    
    EventListenerCustom *m_pCustom;
    
    bool m_bFirstIn;
    
    bool m_bDrag;
    
    bool m_bCross;
    
private://method
    
    void onButtonCallback(Ref *aButton, Widget::TouchEventType aType);
    
    void onPageScrollDone(Ref *aRef, PageView::EventType aType);
    
    void onAdsShowDone();
    
    bool onTouchBegan(Touch*, Event*);
    void onTouchMoved(Touch*, Event*);
    void onTouchEnded(Touch*, Event*);
};

#endif /* defined(__ANIM3203__AnimalChooseScene__) */