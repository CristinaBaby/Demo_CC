//
//  MixSpoonNode.h
//  BHUG1064
//
//  Created by maxiang on 5/27/16.
//
//

#ifndef SpoonNode_h
#define SpoonNode_h

#include "MakeBaseNode.h"

class MixSpoonNode : public MakeBaseNode
{
public:
    static MixSpoonNode* create(const std::string& file);
    bool init(const std::string& file);
    
    std::function<void(float percent)> mixPercentCallback = nullptr;
    std::function<void()> mixPaused = nullptr;
    
    CC_SYNTHESIZE(bool, _enableRotate, EnableRotate);
    CC_SYNTHESIZE(Vec2, _spoonCenter, SpoonCenter);
    CC_SYNTHESIZE(bool, _enableAnimation, EnableAnimation);//放大缩小动画
    
    Sprite* setMixAnimate(const std::string& format,
                          int start,
                          int end);
    void startMix(const Rect& worldArea,
                  Sprite *mixSprite,
                  const std::string& mixFileFormat,
                  int start,
                  int end,
                  bool needRotate = false);
    
public:
    MixSpoonNode():
    _spoon(nullptr),
    _mixAnimationSprite(nullptr),
    _enableRotate(false),
    _mixContentSprite(nullptr),
    _animation(nullptr),
    _enableAnimation(true)
    {};
    virtual ~MixSpoonNode();
    
protected:
    
protected:
    Sprite *_spoon;
    Sprite *_mixContentSprite;
    Sprite *_mixAnimationSprite;
    Vec2 _originalPosition;
    std::function<void()> _showContentCallback;
    ActionInterval *_animation;
};

#endif /* SpoonNode_h */
