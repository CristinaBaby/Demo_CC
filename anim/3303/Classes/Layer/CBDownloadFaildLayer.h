//
//  CBDownloadFaildLayer.h
//  ColorBook
//
//  Created by maxiang on 5/11/15.
//
//

#ifndef __ColorBook__CBDownloadFaildLayer__
#define __ColorBook__CBDownloadFaildLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class DownloadFaildLayer : public cocos2d::LayerColor
{
public:
    
    CREATE_FUNC(DownloadFaildLayer);
    virtual bool init();
    
    void yesAction();
    void noAction();
    
    void setYesActionCallback(const std::function<void()>& callback);
    void setNoActionCallback(const std::function<void()>& callback);
    
protected:
    std::function<void()> _yesCallback;
    std::function<void()> _noCallback;
    cocos2d::Label *_textLabel;
};

CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBDownloadFaildLayer__) */
