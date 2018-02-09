//
//  RainbowCupcakeNode.h
//  shake5008
//
//  Created by liji on 17/1/11.
//
//

#ifndef RainbowCupcakeNode_h
#define RainbowCupcakeNode_h

#include <stdio.h>
#include "FoodNode.h"
#define shareNameRainbowcupcake "share_rainbowcupcake.png"
class ScribbleNode;
class ComponentScribblePercent;
class RainbowCupcakeNode : public FoodNode
{
public:
    
    RainbowCupcakeNode():
    _scribbleNode(nullptr),
    _compScribbleComplete(nullptr),
    _topping(nullptr),
    _eatSprite(nullptr),
    _eatNode(nullptr)
    {};
    ~RainbowCupcakeNode(){};
    
    CREATE_FUNC(RainbowCupcakeNode);
    bool init();
    
    void scribbleTopping();
    void scribble(Vec2 pos);
    void checkScribble(const function<void()> &finishedCallback);
    void updateTopping();
    void addMarshmallow(int index);
    void addRainBow();
    void showDec();
    
    void addOther(const string &path);
    void addSprinkle(Node* s);
    void saveImage();
    void showShare(bool canEat);
protected:
    ScribbleNode *_scribbleNode;
    ComponentScribblePercent* _compScribbleComplete;
    Sprite* _topping;
    vector<Node*> _noneEatNode;
    vector<Node*> _foodVec;
    Sprite* _eatSprite;
    Node* _eatNode;
protected:
private:
    //    void onEnterTransitionDidFinish();
    
};


#endif /* RainbowCupcakeNode_h */
