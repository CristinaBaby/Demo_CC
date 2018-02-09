//
//  CakePopNode.h
//  BHUG1066
//
//  Created by maxiang on 7/5/16.
//
//

#ifndef CakePopNode_h
#define CakePopNode_h

#include "FoodNode.h"
#define shareNameCottonCandy "shareNameCakePop.png"
class CakePopNode : public FoodNode
{
public:
    CREATE_FUNC(CakePopNode);
    virtual bool init() override;
    
    Sprite *getStick(){return _stick;};
    ClippingNode *getSprinkleNode(){return _sprinkleNode;};
    
    void addOther(const string &path);
    void addSprinkle(Node* s);
    void addSprinkle(string sprinklePath, Vec2 worldPos);
    void updateStick(string stickPath);
    Rect getCottonRect();
    void updateFace(string facePath);
    void saveImage(const std::function<void(RenderTexture*, const std::string&)>& callback);
    void showShare(bool canEat);
    
public:
    CakePopNode():
    _stick(nullptr),
    _cottonCandy1(nullptr),
    _sprinkleNode(nullptr),
    _eatNode(nullptr),
    _eatSprite(nullptr),
    _face(nullptr)
    {};
    virtual ~CakePopNode();
    
protected:
    Sprite *_stick;
    ClippingNode *_sprinkleNode;
    Node *_eatNode; //截图用
    Sprite *_cottonCandy1;
    Sprite *_eatSprite;//分享时用于显示
    Sprite *_face;
};

#endif /* CakePopNode_h */
