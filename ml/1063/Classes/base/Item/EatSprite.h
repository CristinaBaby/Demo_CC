//
//  EatSprite.h
//  JuiceMaker
//
//  Created by luotianqiang1 on 4/2/15.
//
//

#ifndef __JuiceMaker__EatSprite__
#define __JuiceMaker__EatSprite__

#include <iostream>
#include"cocos2d.h"
USING_NS_CC;
class EatSprite : public Node {
public:
    EatSprite();
   virtual ~EatSprite();
    CREATE_FUNC(EatSprite);
   virtual void addSprite(Sprite* s);
    virtual void addDownSprite(Sprite* s);
    float getEatPercentage();
    virtual void checkEatStatus(float);
    virtual void reset();
    CC_SYNTHESIZE(bool, _enable, Enable);
    CC_SYNTHESIZE(bool,checkEnable,CheckEnable);
    std::string _eatMaskPath,_particlePath,_soundPath;
protected:
   virtual bool init();
    Layer* maksLayer,*_downMaskLayer;
    Vector<Sprite*> eatSprites;
    EventListenerTouchOneByOne *touchListener;
    size_t _pixcelBegin;
    size_t _pixcelEnd;
    Image *_image;
    ClippingNode *_clippingNode;
    ClippingNode *_downClippingNode;
protected:
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    virtual void onTouchEnded(Touch *touch,Event* event);
    virtual bool onTouchBegin(Touch *touch,Event* event);
    size_t calcute(Image *pImage);
public:
    std::function<void(float)> eatPercentageCallback;
    std::function<void()> _touchCallBack;
    void CloneSprite();
};
#endif /* defined(__JuiceMaker__EatSprite__) */
