/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：SwitchButton
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

#ifndef __PizzaHouseStory__SwitchButton__
#define __PizzaHouseStory__SwitchButton__

#include "cocos2d.h"

class TaggedMenu;

USING_NS_CC;

class SwitchButton : public Sprite
{
public:
    typedef enum {
        STATE_NORMAL,
        STATE_SELECTED,
        STATE_DISABLE
    }ButtonStatus;
    
public:
    
    SwitchButton();
    ~SwitchButton();
    
    static SwitchButton *create(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable = "");
    
    bool initWithSwitchButton(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable = "");
    
    void setStatusChangeCallback(std::function<void(ButtonStatus)> aFunc)
    {
        m_fStatusChange = aFunc;
    }
    
    inline ButtonStatus getButtonStatus()
    {
        return m_eStatus;
    }
    
    inline const Size &getButtonSize()
    {
        switch (m_eStatus) {
            case STATE_NORMAL:
            {
                return m_pNormal->getContentSize();
            }
                break;
            case STATE_SELECTED:
                return m_pSelected->getContentSize();
                break;
            case STATE_DISABLE:
                return m_pDisable->getContentSize();
                break;
            default:
                break;
        }
    }
    
    void changeStatus(ButtonStatus aStatus);
    
private://Variables
    
    friend TaggedMenu;
    
    Sprite *m_pNormal;
    
    Sprite *m_pSelected;
    
    Sprite *m_pDisable;
    
    ButtonStatus m_eStatus;
    
    std::function<void(SwitchButton *)> m_fTouchCallback;
    
    std::function<void(ButtonStatus)> m_fStatusChange;
    
private://method
    
    void updateImagesVisibility();
    
    bool onTouchBegan(Touch*, Event*);
    void onTouchMoved(Touch*, Event*);
    void onTouchEnded(Touch*, Event*);
};

#endif