//
// DressUpScene.h
// ANIM3203
//
// Created by dengcheng on 15/9/25.
//
//

#ifndef __ANIM3203__DressUpScene__
#define __ANIM3203__DressUpScene__

#include "cocos2d.h"
#include "BaseScene.h"
#include "AnimalModel.h"
#include "XCScrolledList1.h"

USING_NS_CC;

class DressUpScene : public BaseScene
{
public:
    DressUpScene();
    ~DressUpScene();

    static DressUpScene* create();
    virtual bool init();

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    AnimalModel *m_pAnimal;
    
    XCScrolledList1 *m_pCategoryList;
    
    LayerColor *m_pObjectsLayer;
    
    XCScrolledList1 *m_pObjectsList;
    
    Widget *m_pPopUp;
    
    EventListenerCustom *m_pCustom;
    
private://method
    
    void onButtonCallback(Ref *aButton, Widget::TouchEventType aType);
    
    void onChangeClothCallback(Node *aButton);
    
    void onCategoryCallback(Node *aSender);
    
    
};

#endif /* defined(__ANIM3203__DressUpScene__) */