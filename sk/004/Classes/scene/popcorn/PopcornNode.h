//
//  PopcornNode.h
//  shake004
//
//  Created by liji on 16/10/25.
//
//

#ifndef PopcornNode_h
#define PopcornNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
#include "ESDataManager.h"
#define shareNamePopCorn "share_popcorn.png"

class ComponentTouchMove;
class PopcornNode : public MakeBaseNode
{
public:
    PopcornNode():
    _boxBottom(nullptr),
    _boxTop(nullptr),
    _popcorn(nullptr),
    _maskNode(nullptr),
    _syrup(nullptr)
    {
    
    };
    virtual ~PopcornNode(){};
    
    static PopcornNode* createPopcorn(bool canEat = false);
    bool initPopcorn(bool canEat);
    Sprite* getEatSprite()
    {
        return _popcorn;
    };
    
//    void enableEat();
//    void resetEat();
//    void checkEatDone();
//    function<void()> _eatDoneCallBack = nullptr;
//    function<void(Touch* t)> _eatingCallBack = nullptr;
    void showPopcorn();
    void updateBox();
    
    void savePopcornImage(const std::function<void(RenderTexture*, const std::string&)>& callback);
    void showShare();

    
    void addSprinkle(string sprinklePath, Vec2 worldPos);
    void addFood(string foodPath);
    void addSyrup(string foodPath);

protected:
    Sprite* _boxBottom;
    Sprite* _boxTop;
    Sprite* _popcorn;
    Sprite* _syrup;
    ClippingNode* _maskNode;
private:
    
};


#endif /* PopcornNode_h */
