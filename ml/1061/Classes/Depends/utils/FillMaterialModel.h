//
//  FillMaterialModel.h
//  FMAKER3042_Taco
//
//  Created by jianghai on 15-4-23.
//
//

#ifndef __FMAKER3042_Taco__FillMaterialModel__
#define __FMAKER3042_Taco__FillMaterialModel__

#include <iostream>
#include "cocos2d.h"
#include "SpriteButton.h"
USING_NS_CC;

typedef enum {
    Meat,
    Vegetable,
    Sauce,
} fillSaveModelType;

typedef enum {
    NONE,
    MOVE,
    SCALE,
    ROTATE,
} EventType;

class FillMaterialModel : public Sprite
{
public:
    FillMaterialModel();
    ~FillMaterialModel();
    static FillMaterialModel* create(const char* ImageName);
    virtual bool init(const char* ImageName);
    inline const char* getResourceName()
    {
        return _resourceName.c_str();
    }
    inline void setEnable(bool isEnable)
    {
        _enable = isEnable;
    }
    inline void hiddenTipsFrame()
    {
        _close->setVisible(false);
        _magnify->setVisible(false);
        this->setSpriteFrame(Sprite::create("controlTip_bg_Clear.png")->getSpriteFrame());
        _isTipsFrameShow = false;
    }
    inline fillSaveModelType getType()
    {
        return _type;
    }
    inline void setType(fillSaveModelType type)
    {
        _type = type;
    }
    inline void setMoveRect(Rect rect)
    {
        _moveRect = rect;
    }
    void stopTipsFrameStatus();
    void setNotDeleteSelf();
private:
    void deleteSelf(Ref *sender);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    virtual void onEnter();
    virtual void onExit();
    
    virtual void updateTipsFrameStatus(float);
    
private:
    fillSaveModelType _type;
    SpriteButton *_close;
    Sprite *_magnify;
    Sprite *_item;
    EventListenerTouchOneByOne *_listener;
    EventType _eventType;
    float _orignalRotate;
    bool _isTipsFrameShow;
    std::string _resourceName;
    bool _enable;
    //移动区域
    Rect _moveRect;
};

#endif /* defined(__FMAKER3042_Taco__FillMaterialModel__) */
