//
// SPAScene.h
// ANIM3203
//
// Created by dengcheng on 15/9/10.
//
//

#ifndef __ANIM3203__SPAScene__
#define __ANIM3203__SPAScene__

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class AnimalModel;
class ViewPager;
class XCPageView;

class SPAScene : public BaseScene
{
public:
    SPAScene();
    ~SPAScene();

    static SPAScene* create(AnimalModel *aAnimalModel);
    virtual bool init(AnimalModel *aAnimalModel);

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    typedef enum {
        kNormalMode,
        kZoomInMode
    }SPAMode;
    
    SPAMode m_eMode;
    
    Sprite *m_pNailclipper;
    
    Button *m_pPrevious;
    
    Button *m_pNext;
    
    AnimalModel *m_pAnimalModel;
    
    XCPageView *m_pToolsView;
    
    ViewPager *m_pViewPager;
    
    ImageView *m_pSelectedToolIcon;
    
    Sprite *m_pSelectedTool;
    
    Sprite *m_pShowerhead;
    
    std::string m_sBubblePath;
    
    int m_nShampooType;
    
    bool m_bAnimate;
    
    bool m_bScaleing;
    
    bool m_bSound;
    
    bool m_bScroll;
    
    bool m_bBeagan;
    
    std::vector<ImageView *> m_vTools;
    
    std::vector<ImageView *> m_vShampoos;
    
    EventListenerCustom *m_pCustom;
    
private://method
    
    void loadstaticWidget();
    
    void loadToolsBar();
    
    void changeSPAMode(SPAScene::SPAMode aMode);
    
    bool touchTool(Touch *aTouch);
    
    void changeTheShampoo(int aType);
    
    void excuteLogic();
    
    void completeUseOfTool();
    
    void onShampooCallback(Ref *aButton, Widget::TouchEventType aType);
    
    void onPageScrollDone(Ref *aRef, PageView::EventType aType);
    
    void onButtonCallback(Ref *aButton, Widget::TouchEventType aType);
    
    bool onTouchBegan(Touch*, Event*);
    void onTouchMoved(Touch*, Event*);
    void onTouchEnded(Touch*, Event*);
};

#endif /* defined(__ANIM3203__SPAScene__) */