//
//  CBSavingLayer.h
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#ifndef __ColorBook__CBSavingLayer__
#define __ColorBook__CBSavingLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class SavingLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(SavingLayer);
    virtual bool init();
    virtual ~SavingLayer();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void finishSave(float time);
    void setFinishCallback(const std::function<void()>& callback);
    
protected:
    std::function<void()> _finishCallback;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBSavingLayer__) */
