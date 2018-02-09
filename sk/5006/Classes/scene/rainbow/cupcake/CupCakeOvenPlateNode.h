//
//  CupCakeOvenPlateNode.h
//  shake5008
//
//  Created by liji on 17/1/4.
//
//

#ifndef CupCakeOvenPlateNode_h
#define CupCakeOvenPlateNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
class BowlNode;
class CupCakeOvenPlateNode : public MakeBaseNode
{
public:
    CREATE_FUNC(CupCakeOvenPlateNode);
    bool init();
public:
    CupCakeOvenPlateNode():
    _plate(nullptr)
    {
    
    };
    virtual ~CupCakeOvenPlateNode(){};
    
    void showMixtureInPlate(int mouldIndex, int colorIndex);
    void showAllMixture();
    void showAllMixtureBaked();
    void bakeMixture(float duration);
protected:
    BowlNode* _plate;
    Sprite* _mix;

    
};
#endif /* CupCakeOvenPlateNode_h */
