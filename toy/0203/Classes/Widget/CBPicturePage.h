//
//  CBPicturePage.h
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#ifndef __ColorBook__CBPicturePage__
#define __ColorBook__CBPicturePage__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "../Model/CBAppGlobal.h"
#include "../Widget/CBPage.h"

CB_BEGIN_NAMESPACE

class PicturePage : public Page
{
public:
    
    CREATE_FUNC(PicturePage);
    
    virtual bool init();
    
    virtual void reset();
    
    void setPackIndex(const int packIndex);

    virtual void touchEnd(cocos2d::Touch *touch);
    
    void reload(const int picturePageIndex);
protected:
    /* avoid user tap too many times */
    bool _hasChoosed;
    int _packIndex;
    int _picturePageIndex;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPicturePage__) */
