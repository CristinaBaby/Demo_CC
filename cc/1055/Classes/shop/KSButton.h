//
//  KSButton.h
//  GameFramework
//
//  Created by tangbowen on 14-12-5.
//
//

#ifndef __GameFramework__KSButton__
#define __GameFramework__KSButton__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace std;

class KSButton : public Node {
    
public:
    enum class ButtonState{
        StateNormal         =           0,
        StateSelected
    };
    
    enum class TouchEventType
    {
        BEGAN,
        MOVED,
        ENDED,
        CANCELED
    };
    typedef function<void(KSButton*, TouchEventType)> KSButtonCallBack;
    
public:
    KSButton();
    virtual ~KSButton();
    
public:
    static KSButton* create(const string& normalImage, const string& selectedImage = "");
    static KSButton* create(Sprite* normalSprite, Sprite* selectedSprite = nullptr);
    bool initWithSprite(Sprite* normalSprite, Sprite* selectedSprite);
    
public:
    void setButtonState(ButtonState state);
    inline void setEnabled(bool enable){
        m_bEnable = enable;
    }
    inline void setEventListener(KSButtonCallBack callBack){
        m_pCallBack = callBack;
    }
    inline void setClickListener(function<void(KSButton*)> clickCallBack){
        m_fnClickCallBack = clickCallBack;
    }
    
private:
    bool    m_bEnable;
    Sprite* m_pNormalSprite;
    Sprite* m_pSelectedSprite;
    
    KSButtonCallBack    m_pCallBack;
    function<void(KSButton*)> m_fnClickCallBack;
    
private:
    void callFunction(TouchEventType type);
    bool isAncestorsVisible(Node* node);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
public:
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__GameFramework__KSButton__) */
