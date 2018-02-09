//
//  CBQuitGameLayer.h
//  ColorBook
//
//  Created by maxiang on 6/3/15.
//
//

#ifndef __ColorBook__CBQuitGameLayer__
#define __ColorBook__CBQuitGameLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "CBDownloadFaildLayer.h"



class QuitGameLayer : public DownloadFaildLayer
{
    public:
    CREATE_FUNC(QuitGameLayer);
    
    virtual bool init();
};



#endif /* defined(__ColorBook__CBQuitGameLayer__) */
