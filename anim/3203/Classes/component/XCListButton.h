//
// XCListButton.h
// SLUSHY3016
//
// Created by dengcheng on 15/7/23.
//
//

#ifndef __SLUSHY3016__XCListButton__
#define __SLUSHY3016__XCListButton__

#include "cocos2d.h"

USING_NS_CC;

class XCListButton : public Sprite
{
public:
    XCListButton();
    ~XCListButton();

    static XCListButton *create(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable = "");
    
    bool initWithXCListButton(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable = "");
    
    void setTouchCallback(std::function<void(Node *)> aFunc) {
        m_fTouch = aFunc;
    }

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    Sprite *m_pNormal;
    
    Sprite *m_pSelected;
    
    Sprite *m_pDisable;
    
    typedef enum {
        STATE_NORMAL,
        STATE_SELECTED,
        STATE_DISABLE
    }ButtonStatus;
    
    ButtonStatus m_eStatus;
    
    std::function<void(Node *)> m_fTouch;
    
    bool m_bClick;

private://method
    void changeStatus(ButtonStatus aStatus);
    
    void updateImagesVisibility();
    
    bool onTouchBegan(Touch*, Event*);
    void onTouchMoved(Touch*, Event*);
    void onTouchEnded(Touch*, Event*);
};

#endif /* defined(__SLUSHY3016__XCListButton__) */