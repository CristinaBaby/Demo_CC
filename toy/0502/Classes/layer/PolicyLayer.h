//
//  PolicyLayer.h
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#ifndef __MyGame_Project__PolicyLayer__
#define __MyGame_Project__PolicyLayer__

#include <iostream>
#include "cocos2d.h"
#include "CovorLayer.h"
#include "../utilities/UITools.h"


class PolicyLayer : public Layer{
    
public:
    static PolicyLayer* create();
        
    bool init();
    
    virtual ~PolicyLayer();
    
    void onButton(Ref* sender, Widget::TouchEventType type);
    
    void backCallback(Ref* sender );
    
public:
    
    cocos2d::Sprite* boxNode;
};
#endif /* defined(__MyGame_Project__PolicyLayer__) */
