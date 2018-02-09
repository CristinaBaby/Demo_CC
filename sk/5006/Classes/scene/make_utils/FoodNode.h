//
//  FoodNode.h
//  ccats024
//
//  Created by liji on 16/12/5.
//
//

#ifndef FoodNode_h
#define FoodNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
#include "ComponentEat.h"
#include "SoundConfig.h"
class FoodNode : public MakeBaseNode
{
public:
    
    FoodNode(){};
    ~FoodNode(){};
    
    CREATE_FUNC(FoodNode);
    bool init();
    Rect getBoundingRect();
    
    std::function<void()> _eatOver = nullptr;
    vector<ComponentEat*> _eatVec;

    
    void registEat(Node *node);
    void resetEat();

protected:
    
private:
//    void onEnterTransitionDidFinish(){};
    
};


#endif /* FoodNode_h */
