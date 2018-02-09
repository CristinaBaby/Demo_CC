//
//  WriteTouchNode.h
//  WriteDemo_3_2
//
//  Created by renhong on 9/19/14.
//
//

#ifndef __WriteDemo_3_2__WriteTouchNode__
#define __WriteDemo_3_2__WriteTouchNode__

#include <stdio.h>
#include "WriteNode.h"

class WriteTouchNode : public WriteNode {
public:
    static WriteTouchNode *create(Size pCanvasSize);
    virtual bool init(Size pCanvasSize);
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

};
#endif /* defined(__WriteDemo_3_2__WriteTouchNode__) */
