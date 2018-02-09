//
//  ScribbleSVGUtil.hpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/11/26.
//
//

#ifndef ScribbleSVGUtil_hpp
#define ScribbleSVGUtil_hpp

#include <stdio.h>
#include <cocos2d.h>
using namespace cocos2d;
#include "FillSVGNode.h"
class ScribbleSVGUtil{
public:
    static Image *capture(ScribbleSVGLineFrameNode *pNode, uint pSampleNumber = 4);
    static RenderTexture *capture(Node *pNode);
};
#endif /* ScribbleSVGUtil_hpp */
