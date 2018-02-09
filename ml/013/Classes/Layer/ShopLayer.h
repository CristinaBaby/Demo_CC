//
//  ShopLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-4.
//
//

#ifndef __Make__ShopLayer__
#define __Make__ShopLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "TweenScrollView.h"
#include "BaseLayer.h"
#include "ShopSelView.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class ShopLayer : public BaseLayer /*public ShopSelViewDelegate,public DialogCallback*/
{
public:
    static Scene* scene();
    CREATE_FUNC(ShopLayer);
    bool init();
    
private:
    void initWithUI();
    
private:
//    virtual void onNegativeClick(void* type);
//    virtual void onPositiveClick(void* type);
//    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
//    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    //    virtual void onExit();
    //    virtual void onEnter();
    
    virtual void unitSelected(int idx) ;

private:
    ui::Button* buyBtn_;
    int curIdx_;
};

#endif /* defined(__Make__ShopLayer__) */
