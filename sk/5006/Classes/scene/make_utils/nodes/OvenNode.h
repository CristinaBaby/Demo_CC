//
//  OvenNode.h
//  ccats024
//
//  Created by liji on 16/11/30.
//
//

#ifndef OvenNode_h
#define OvenNode_h

#include <stdio.h>
#include "MakeBaseNode.h"

class OvenNode : public MakeBaseNode
{
public:
    CREATE_FUNC(OvenNode);
    bool init();
public:
    OvenNode():
    _ovenDoor(nullptr){};
    virtual ~OvenNode(){};
    
    void showButtonTurnNode(std::function<void()> callback);
    void onEnter();
    void rotateButton(float duration);
    void showOvenDoor(bool visible);
protected:
    vector<Sprite*> _btnVec;
    int _rotateCount = 2;
    Sprite* _ovenDoor;

};

#endif /* OvenNode_h */
