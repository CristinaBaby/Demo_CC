//
// ShopLayer.h
// ANIM3203
//
// Created by dengcheng on 15/10/8.
//
//

#ifndef __ANIM3203__ShopLayer__
#define __ANIM3203__ShopLayer__

#include "cocos2d.h"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class ShopLayer : public LayerColor
{
public:
    ShopLayer();
    ~ShopLayer();

    static ShopLayer* create();
    virtual bool init();

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    EventListenerCustom *m_pNOtifaction;
private://method

};

#endif /* defined(__ANIM3203__ShopLayer__) */