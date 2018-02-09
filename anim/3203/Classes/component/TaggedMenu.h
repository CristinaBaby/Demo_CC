/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：TaggedMenu
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 11/4/15.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __PizzaHouseStory__TaggedMenu__
#define __PizzaHouseStory__TaggedMenu__

#include "cocos2d.h"
#include "SwitchButton.h"

USING_NS_CC;

class TaggedMenu : public Layer
{
public:
    TaggedMenu();
    ~TaggedMenu();
    
    virtual bool initWithTaggedMenu();
    
    static TaggedMenu *create();
    
    static TaggedMenu *create(SwitchButton *aButton, ...);
    
    static TaggedMenu *createWithItems(SwitchButton *firstButton, va_list args);
    
    static TaggedMenu *createWithArray(const Vector<SwitchButton*>& arrayOfItems);
    
    void setDefaultSelected(SwitchButton *aButton);
    
    void setTaggedChangeCallback(std::function<void(int)> aFunc)
    {
        m_fTaggedChangeCallback = aFunc;
    }
    
private://Variables
    
    std::vector<SwitchButton *> m_vButtons;
    
    std::function<void(int)> m_fTaggedChangeCallback;
    
    SwitchButton *m_pCurrent;
    
private://method
    
    void setCurrentTagged(SwitchButton *aButton);
    
    bool initWithArray(const Vector<SwitchButton*>& arrayOfItems);
    
    void onButtonCallback(SwitchButton *pSender);
};

#endif