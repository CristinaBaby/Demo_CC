//
//  SnowConeNode.h
//  BHUG1066
//
//  Created by maxiang on 6/27/16.
//
//

#ifndef SnowConeNode_h
#define SnowConeNode_h

#include "MakeBaseNode.h"

class SnowConeNode : public MakeBaseNode
{
public:
    static SnowConeNode* create(int index);
    virtual bool init(int index);
    
    //初始化倒入的冰块裁剪节点
    void setIceClipping(const std::string& maskFile, const Vec2& position);
    
    //初始化冰块, 将会被裁剪
    void setIce(const std::string& iceFile, const Vec2& position);
    
    //移动冰块，被裁剪
    void showIce(const Vec2& position, bool animation = true);
    Node *getIce(){return _iceClippingNode;};

    //设置刨冰形状
    void setShape(int index);
    void setShapeOpacity(int opacity);
    void showPartAnimation(int partindex);
    void showJuiceShape(const std::string& file);
    
    //根据果汁类型，淋到某一部分上
    void showPart(int partindex,
                  const std::string& juiceFile,
                  bool animation = true);
    Node *getIceShapeNode(){return _iceShapeNode;};
    
    //显示隐藏cup
    void setCupVisible(bool visible);

public:
    SnowConeNode():
    _iceClippingNode(nullptr),
    _iceShapeNode(nullptr),
    _juiceShapeSprite(nullptr)
    {};
    virtual ~SnowConeNode();
protected:
    
protected:
    std::string _cupPath = "res/snow_cone/7-9/";
    std::string _shapePath = "res/snow_cone/10-14/";

    ClippingNode *_iceClippingNode;
    Node *_iceShapeNode;
    
    Sprite *_juiceShapeSprite;

    Vec2 _originalPosition;
    Sprite *_ice;
};

#endif /* SnowConeNode_h */
