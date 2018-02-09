//
//  TexasPizzaNode.h
//  shake004
//
//  Created by liji on 16/10/27.
//
//

#ifndef TexasPizzaNode_h
#define TexasPizzaNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
#include "ESDataManager.h"
#define screenNameJuice "paint_juice.png"
class TexasPizzaNode : public MakeBaseNode
{
public:
    CREATE_FUNC(TexasPizzaNode);
    bool init();
    
    TexasPizzaNode():
    _pizza(nullptr),
    _bowtie(nullptr),
    _sticker(nullptr),
    _box(nullptr),
    _boxBtm(nullptr)
    {};
    ~TexasPizzaNode(){};
    
    void updateBox(int index);
    void updateBowtie(const string& filePath,bool canMove = false);
    void updateSticker(const string& filePath,bool canMove = false);
    
    void showEat();
    void showServe();
    Sprite* getEatSprite();
    void reset();
private:
    Sprite* _pizza;
    Sprite* _box;
    Sprite* _boxBtm;
    Sprite* _bowtie;
    Sprite* _sticker;
    
};
#endif /* TexasPizzaNode_h */
