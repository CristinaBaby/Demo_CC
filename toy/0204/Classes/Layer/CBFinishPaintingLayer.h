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
#include "cocosgui.h"

CB_BEGIN_NAMESPACE

enum buttonTag{
    saveTag = 111,
    quitTag,
    backTag
};

class FinishPaintingLayer : public cocos2d::LayerColor
{
public:
    static FinishPaintingLayer* create(cocos2d::Image *image);

    virtual bool init(cocos2d::Image *image);
    virtual ~FinishPaintingLayer();
    
    void setQuitCallback(const std::function<void()>& callback);
    void setSaveCallback(const std::function<void()>& callback);
    
    void onButton(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
    
protected:
    cocos2d::Sprite *_bgSprite;
    cocos2d::ui::Button *_backButton;
    cocos2d::ui::Button *_quitButton;
    cocos2d::ui::Button *_saveButton;
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
