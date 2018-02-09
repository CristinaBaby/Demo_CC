//
//  PopcornMakeScene.h
//  shake004
//
//  Created by liji on 16/10/24.
//
//

#ifndef PopcornMakeScene_h
#define PopcornMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
#include "BowlNode.h"

class PopcornMakeScene : public ESMakeBaseScene
{
public:
    
    PopcornMakeScene();
    ~PopcornMakeScene();
    
    MY_SCENE(PopcornMakeScene);
    CREATE_FUNC(PopcornMakeScene);
    bool init();
    
protected:
    void showCorn();
    void initCorn();
    void initTouchMakeCorn();
    void makeCorn(Node *cornNode);
    
    void showManualMachine();
    void manualMachineShowOil();
    void manualMachineShowCorn();
    void manualMachineShowSalt();
    void manualMachineFinishMake();
    void manualMachineZoomIn();

    
protected:
    BowlNode *_bowl;
    Vector<Node*> _cornNodesVector;
    float _cornHeightInBowl;
    Node *_currentMakingCornNode;
    bool _once = false;
    Node *_manualMachineNode;
    
    int _scoopTimes = 1;

    
    
    
};

#endif /* PopcornMakeScene_h */
