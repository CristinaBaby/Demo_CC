//
//  LiquidNode.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef LiquidNode_h
#define LiquidNode_h

#include "MakeBaseNode.h"

class LiquidNode : public MakeBaseNode
{
public:
    static LiquidNode* create(const std::string& file,
                              const std::string& cover  = "",
                              const std::string& mask = "",
                              const std::string& inLiquid = "",
                              const std::string& shadow = "",
                              const std::string& lid = "");
    
    bool init(const std::string& file,
              const std::string& cover = "",
              const std::string& mask = "",
              const std::string& inLiquid = "",
              const std::string& shadow = "",
              const std::string& lid = "");
    
    //拖拽到哪里倾倒, 显示倾倒到碗里的物体回调
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    void backToOriginal(const std::function<void()>& callback = nullptr);
    
    //拖动到指定位置回调, 显示提示旋转倾倒
    std::function<void()> onMoveToTargetCallback = nullptr;
    std::function<void()> onTouchBegan = nullptr;
    void setLiquidPos(const Vec2 &Pos);
    void openLid();
    
    Sprite* getInLiquid(){return _inLiquid;};
    Sprite* getBottm(){return _liquid;};
public:
    LiquidNode();
    virtual ~LiquidNode();
    
protected:
    void accelerateRotateDone();

protected:
    Sprite *_liquid;
    Sprite *_cover;
    Sprite *_mask;
    Sprite *_inLiquid;
    Sprite *_lid;
    Sprite *_shadow;
    Vec2 _originalPosition;
    std::function<void()> _showContentCallback;
    CC_SYNTHESIZE(float, _rotateDegree, RotateDegree);
    CC_SYNTHESIZE(float, _duration, Duration);
};

#endif /* LiquidNode_h */
