//
//  TapDropNode.h
//  BHUG1066
//
//  Created by liji on 16/6/28.
//
//

#ifndef TapDropNode_h
#define TapDropNode_h

#include "MakeBaseNode.h"

class TapDropNode : public MakeBaseNode
{
public:
    static TapDropNode* create(const std::string& file);
    bool init(const std::string& file);
    
    void setPourWorldPoint(const Vec2& worldPoint);
    void backToOriginal(const std::function<void()>& callback);

    
    std::function<void()> dragBeginCall = nullptr;
    
    std::function<void()> dropShowCall = nullptr;
    
    std::function<void()> rotateDoneCall = nullptr;

    std::function<void()> dropDoneCall = nullptr;

public:
    TapDropNode();
    virtual ~TapDropNode();
    
protected:
    Sprite *_container;
    Vec2 _originalPosition;
    CC_SYNTHESIZE(int, _dropCountNeeded, DropStartedCount);
};

#endif /* TapDropNode_h */
