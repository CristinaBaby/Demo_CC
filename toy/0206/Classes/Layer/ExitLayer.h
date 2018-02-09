//
//  ExitLayer.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#ifndef __ColorBook0818__ExitLayer__
#define __ColorBook0818__ExitLayer__

#include "cocos2d.h"
#include "cocosgui.h"

USING_NS_CC;

class ExitLayer : public Layer {
public:
    CREATE_FUNC(ExitLayer);
    virtual bool init();
    
    void TouchDown(Ref* pSender, ui::Widget::TouchEventType type);
};

#endif /* defined(__ColorBook0818__ExitLayer__) */
