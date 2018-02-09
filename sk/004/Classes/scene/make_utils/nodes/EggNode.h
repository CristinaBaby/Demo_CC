//
//  EggNode.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef EggNode_h
#define EggNode_h

#include "MakeBaseNode.h"

class EggNode : public MakeBaseNode
{
public:
    static EggNode* create(const std::string& file);
    bool init(const std::string& file);
    
    //拖拽到哪里倾倒, 显示倾倒到碗里的物体回调
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    
public:
    EggNode();
    virtual ~EggNode();
    
protected:
    
protected:
    Sprite *_egg;
    Vec2 _originalPosition;
    std::function<void()> _showContentCallback;
};

#endif /* EggNode_h */
