//
//  XXXLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__XXXLayer__
#define __Make__XXXLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class XXXLayer :public BaseLayer
{
public:
    static Scene* scene();
    CREATE_FUNC(XXXLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual void nextBtnClicked();

};
#endif /* defined(__Make__XXXLayer__) */
