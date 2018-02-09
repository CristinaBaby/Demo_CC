//
//  BowlNode.h
//  BHUG1064
//
//  Created by maxiang on 5/25/16.
//
//

#ifndef BowlNode_h
#define BowlNode_h

#include "MakeBaseNode.h"

class BowlNode : public MakeBaseNode
{
public:
    static BowlNode* create(const std::string& bottom, const std::string& top);
    bool init(const std::string& bottom, const std::string& top);
    
    Node* addContent(const std::string& file,
                    const std::string& name,
                    const int zorder,
                    const Vec2& position = Vec2::ZERO);
    void addContent(Node *content,
                    const std::string& name,
                    const int zorder,
                    const Vec2& position = Vec2::ZERO);
    void setContentVisible(const std::string& name,
                           const std::function<void()>& callback = nullptr,
                           bool visible = true,
                           bool animation = true);

    void initMaskNode(const std::string& file);
    
    void addContentToMask(const std::string& file,
                    const std::string& name,
                    const int zorder,
                    const Vec2& position = Vec2::ZERO);
    
    void addContentToMask(Node *content,
                    const std::string& name,
                    const int zorder,
                    const Vec2& position = Vec2::ZERO);
    void setContentInMaskVisible(const std::string& name,
                                 const std::function<void()>& callback = nullptr,
                                 bool visible = true,
                                 bool animation = true);
    
    void showContentInMask(const std::string& name);
    
    void updateBowlConfig(
                      const std::string &bottom,
                      const std::string &cover,
                      const std::string &mask = ""
                      );
    
    //有的碗可以倾倒东西
    void setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback);
    void beginPour(float rotateTo, const std::function<void()>& callback);
    void endPour();
    Rect getBowlRect();
    
public:
    BowlNode():
    _maskNode(nullptr),
    _bottom(nullptr),
    _top(nullptr),
    _mask(nullptr)
    {};
    virtual ~BowlNode();
    
protected:
    void accelerateRotateDone();

protected:
    Sprite *_bottom;
    Sprite *_top;
    Sprite *_mask;
    ClippingNode* _maskNode;
    std::function<void()> _pourCallback;
    float _rotateTo;
    Vec2 _originalPosition;

};

#endif /* BowlNode_h */
