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
class ComponentTouchMove;
class MixSpoonNode : public MakeBaseNode
{
public:
    static MixSpoonNode* create(const std::string& file);
    bool init(const std::string& file);
    
    std::function<void(float percent)> mixPercentCallback = nullptr;
    std::function<void(int tag)> mixerTouchBegan = nullptr;
    std::function<void()> mixPaused = nullptr;
    std::function<void()> mixChanged = nullptr;
    
    CC_SYNTHESIZE(bool, _enableRotate, EnableRotate);
    CC_SYNTHESIZE(ComponentTouchMove*, _cTouch, CTouch);
    CC_SYNTHESIZE(Vec2, _spoonCenter, SpoonCenter);
    CC_SYNTHESIZE(int, _mixLength, MixLength);
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
    void setMixMoveRect(Rect r);
    void setAutoConfig();
    void startDrill();
    void pauseDrill();
    void addlock();
    
public:
    MixSpoonNode():
    _spoon(nullptr),
    _mixAnimationSprite(nullptr),
    _enableRotate(false),
    _mixContentSprite(nullptr),
    _animation(nullptr),
    _enableAnimation(true),
    _drillAnimation(nullptr),
    _first(true),
    _drill(nullptr),
    _cTouch(nullptr),
    _mixLength(90000)
    {};
    virtual ~MixSpoonNode();
    
protected:
    void onExit();
    
protected:
    Sprite *_spoon;
    Sprite *_drill;
    Sprite *_mixContentSprite;
    Sprite *_mixAnimationSprite;
    Vec2 _originalPosition;
    std::function<void()> _showContentCallback;
    ActionInterval *_animation;
    ActionInterval *_drillAnimation;
    Rect _moveRect;
    bool _first;
    
    
};

#endif /* SpoonNode_h */
