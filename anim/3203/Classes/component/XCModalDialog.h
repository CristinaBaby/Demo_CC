/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：XCModalDialog
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 31/3/15.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __PizzaHouseStory__XCModalDialog__
#define __PizzaHouseStory__XCModalDialog__

#include "cocos2d.h"
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace ui;

class XCModalDialog : public LayerColor
{
    
public:
    
    typedef enum {
        BUTTON_1,
        BUTTON_2
    }ButtonTag;
    
public:
    
    /**
     *  显示模态窗口
     */
    static void show(const std::string &aContent, std::function<void(ButtonTag)> aFunc);

private://Variables
    
    static XCModalDialog *g_sDialog;
    
    //背景色深度
    float m_fBackgroundColorDepth;
    
    Sprite *m_pBg;
    
    Label *m_pContent;
    
    std::function<void(ButtonTag)> m_Func;
    
private://method
    
    XCModalDialog();
    ~XCModalDialog();
    
    bool initWithXCModalDialog(const std::string &aBoxFile, std::function<void(ButtonTag)> aFunc);
    
    void onButtonCallback(Ref*, Widget::TouchEventType);
};

#endif