//
//  ScribbleTouchNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#ifndef __ScribbleTouchNode__
#define __ScribbleTouchNode__

#include <stdio.h>
#include "ScribbleNode.h"

class ScribbleTouchNode : public ScribbleNode {
public:
    static ScribbleTouchNode *create(Size pCanvasSize);
    
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void enable(){_enabled = true;}
    void disable(){_enabled = false;}
    bool isEnabled(){return _enabled;}
    
protected:
    bool init(Size pCanvasSize);
    
private:
    bool _enabled;
};

#endif /* defined(__ScribbleTouchNode__) */
