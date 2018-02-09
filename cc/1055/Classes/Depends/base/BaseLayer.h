//
//  BaseLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#ifndef BaseLayer_hpp
#define BaseLayer_hpp

#include <stdio.h>
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
#include "FlowerLayer.h"
#include "AudioHelp.h"
#include "PopAcgLayer.h"
#include "AdsManager.h"
#include "AdsLoadingLayer.h"
#include "IAPManager.h"
#include "Global.h"
#include "MixPoint.h"
#include "SSCFileUtility.h"
USING_NS_CC;

using namespace std;
using namespace ui;

class BaseLayer : public Layer , public DialogCallback{
public:
    BaseLayer();
    ~BaseLayer();

public:
    virtual bool init();
    CREATE_FUNC(BaseLayer);
public:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    // 监听Android返回键事件
    void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    void onNegativeClick(void* type);
    void onPositiveClick(void* type);
public:
    //显示返回框
    bool m_bIsShowReturn;
    
    //设置弹出对话框类型
    DialogFuncType _type;
    void setAndroidKeyType(DialogFuncType type);
    
    StudioLayer* _studioLayer;
protected:
    //接收事件
    virtual void touchEnd(ui::Widget* widget){};
};


#endif /* BaseLayer_hpp */
