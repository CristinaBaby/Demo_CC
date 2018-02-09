//
//  CBFinishPaintingLayer.h
//  ColorBook
//
//  Created by maxiang on 4/27/15.
//
//

#ifndef __ColorBook__CBFinishPaintingLayer__
#define __ColorBook__CBFinishPaintingLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class FinishPaintingLayer : public cocos2d::LayerColor
{
public:
    static FinishPaintingLayer* create(const std::string& pictureName);
    static FinishPaintingLayer* create(cocos2d::Image *image);

    virtual bool init(const std::string& pictureName);
    virtual bool init(cocos2d::Image *image);
    virtual ~FinishPaintingLayer();

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void setQuitCallback(const std::function<void()>& callback);
    void setSaveCallback(const std::function<void()>& callback);
    
protected:
    cocos2d::Sprite *_bgSprite;
    cocos2d::Sprite *_backButton;
    cocos2d::Sprite *_quitButton;
    cocos2d::Sprite *_saveButton;
    cocos2d::Sprite *_saveBg;

    std::string _pictureName;

    void quitAction();
    void saveAction();
    void backAction();

    std::function<void()> _quitCallback;
    std::function<void()> _saveCallback;
};


CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBFinishPaintingLayer__) */
