
#ifndef __AnimationHelp__
#define __AnimationHelp__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

namespace AnimationHelp {
   Vector<SpriteFrame*> getAnimFramesCache(const std::string& preString, int startNum, int endNum,bool isPng = true);
    void removeFrames(const std::string& preString,int startNum, int endNum,bool isPng = true);
   Vector<SpriteFrame*> getAnimFrames(const std::string& preString,int startNum, int endNum,bool isPng = true);
    
   Animate* createAnimateCache(const std::string& frameName, int startNum, int endNum,bool returnOriginFrame = true,bool isPng = true, float delay = 0.2, unsigned int loops = 1);
   Animate* createAnimate(const std::string& frameName, int startNum, int endNum,bool returnOriginFrame = true,bool isPng = true, float delay = 0.2, unsigned int loops = 1);
    
};
#endif /* defined(__AnimationHelp__) */
