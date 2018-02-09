//
//  SaltNode.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef SaltNode_h
#define SaltNode_h

#include "MakeBaseNode.h"

class SaltNode : public MakeBaseNode
{
public:
    static SaltNode* create(const std::string& file);
    bool init(const std::string& file);
    
    void addContent(Node* content);
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback ,const std::function<void()>& backCallback);
    
public:
    SaltNode();
    virtual ~SaltNode();
    
protected:
    Sprite *_salt;
    
    Vec2 _originalPosition;
};

#endif /* SaltNode_h */
