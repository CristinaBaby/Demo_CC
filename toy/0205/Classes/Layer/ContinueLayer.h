//
//  ContinueLayer.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#ifndef __ColorBook0818__ContinueLayer__
#define __ColorBook0818__ContinueLayer__

#include "cocos2d.h"
#include "cocosgui.h"

USING_NS_CC;

class ContinueLayer : public Layer {
public:
    static ContinueLayer* create(int packIndex, int pictureIndex);
    
    virtual bool init(int packIndex, int pictureIndex);
    
    void TouchDown(Ref* pSender, ui::Widget::TouchEventType type);
    
    void resetAction();
    
    void continueAction();
private:
    int _pictureIndex;
    
    int _packIndex;
};

#endif /* defined(__ColorBook0818__ContinueLayer__) */
