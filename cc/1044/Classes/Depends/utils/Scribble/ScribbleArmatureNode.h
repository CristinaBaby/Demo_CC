//
//  ScribbleArmatureNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 14-10-29.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleArmatureNode__
#define __ScribbleDemo_3_2__ScribbleArmatureNode__

#include <stdio.h>
#include "ScribbleTouchNode.h"

class ScribbleArmatureNode : public ScribbleTouchNode {
public:
    static ScribbleArmatureNode *create(Size pCanvasSize);
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
public:
    ScribbleArmatureNode();
    inline void setArmaturePosition(Point pPoint){_armaturePosition = pPoint;}
protected:
    Point _armaturePosition;
};
#endif /* defined(__ScribbleDemo_3_2__ScribbleArmatureNode__) */
