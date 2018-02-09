//
//  ButterNode.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef ButterNode_h
#define ButterNode_h

#include "MakeBaseNode.h"

class ButterNode : public MakeBaseNode
{
public:
    static ButterNode* create(const std::string& file);
    bool init(const std::string& file);
    
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    void setClickCall(const std::function<void()>& showCallback);
    
    static ButterNode* createPicChange(const std::string& fileFormat, Vec2 range);
    bool initPicChange(const std::string& fileFormat, Vec2 range);
    
public:
    ButterNode();
    virtual ~ButterNode();
    
protected:
    Sprite *_butter;
    Vec2 _originalPosition;
    Vec2 _formatRange = Vec2::ZERO;
    std::string _formatStr;
};

#endif /* ButterNode_h */
