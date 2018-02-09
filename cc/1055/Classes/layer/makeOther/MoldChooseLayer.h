//
//  MoldChooseLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/20.
//
//

#ifndef MoldChooseLayer_hpp
#define MoldChooseLayer_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Dialog.h"
#include "AdapterScreen.h"
#include "Configure.h"
#include "StudioLayer.h"
#include "ActionHelper.h"
#include "MoveLisenter.h"
#include "KitchenLayer.h"
#include "GameData.h"
#include "ActionUtils.h"
#include "RewardLayer.h"
#include "PopDialogLayer.h"
USING_NS_CC;

using namespace std;
using namespace ui;

//make步骤的模板
class MoldChooseLayer : public Layer  {
public:
    MoldChooseLayer();
    ~MoldChooseLayer();
public:
    bool init();
    CREATE_FUNC(MoldChooseLayer);
    CREATE_SCENE(MoldChooseLayer);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    void showBg();
    void hideBg();
    
    std::function<void()> chooseMold;
    std::function<void()> clickBtn;
    void moveBtn();
    
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    StudioLayer*  _studioLayer;
    Sprite* m_pBg;
    Button* btnChoose;
    bool m_bIsShow;
    bool m_bIsFirst;
    Vector<MoveLisenter*> m_vMoveLisenter;
};

#endif /* MoldChooseLayer_hpp */
