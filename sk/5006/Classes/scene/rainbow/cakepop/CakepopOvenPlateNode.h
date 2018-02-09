//
//  CakepopOvenPlateNode.h
//  shake5008
//
//  Created by liji on 17/1/4.
//
//

#ifndef CakepopOvenPlateNode_h
#define CakepopOvenPlateNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
class BowlNode;
class CakepopOvenPlateNode : public MakeBaseNode
{
public:
    CREATE_FUNC(CakepopOvenPlateNode);
    bool init();
public:
    CakepopOvenPlateNode():
    _plate(nullptr)
    {
        _posVec =
        {
            Vec2(-137,57-13),
            Vec2(138,57-12),
            Vec2(-141,-43-13),
            Vec2(145,-41-13),
        };
    
    };
    virtual ~CakepopOvenPlateNode(){};
    
    void showMixtureInPlate(int mouldIndex, int colorIndex);
    void showAllMixture();
    void showAllMixtureBaked();
    void bakeMixture(float duration);
protected:
    BowlNode* _plate;
    vector<Vec2> _posVec;
    vector<Sprite*> _mixtureVec;

    
};
#endif /* CakepopOvenPlateNode_h */
