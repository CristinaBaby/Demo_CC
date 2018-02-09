//
//  CBServerPackSprite.h
//  ColorBook
//
//  Created by maxiang on 5/6/15.
//
//

#ifndef __ColorBook__CBServerPackSprite__
#define __ColorBook__CBServerPackSprite__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class ServerPackSprite : public cocos2d::Sprite
{
public:
    static ServerPackSprite* create(const std::string& filename);
    
    void setupUI();
    
    void changeToNotDownloadStyle();
    void changeToDownloadingStyle();
    void changeToHasDownloadStyle();

    cocos2d::ProgressTimer* getProgressTimer(){return _progressTimer;};
    
protected:
    cocos2d::Sprite *_downloadButton;
    cocos2d::Sprite *_availableLogo;
    cocos2d::Sprite *_downloadProgressBg;
    cocos2d::ProgressTimer *_progressTimer;
    
    //6+ logo
    cocos2d::Sprite *_topRightLogo;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBServerPackSprite__) */
