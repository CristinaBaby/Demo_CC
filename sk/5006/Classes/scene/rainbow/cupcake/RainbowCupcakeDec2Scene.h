//
//  RainbowCupcakeDec2Scene.h
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#ifndef RainbowCupcakeDec2Scene_h
#define RainbowCupcakeDec2Scene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "RainbowCupcakeNode.h"
#include "AddSprinkle.h"

using namespace std;
class ComponentTouchMove;
class AddSprinkle;

class RainbowCupcakeDec2Scene : public DecorationBaseLayer
{
public:
    MY_SCENE(RainbowCupcakeDec2Scene);
    CREATE_FUNC(RainbowCupcakeDec2Scene);
    virtual bool init() override;
    RainbowCupcakeDec2Scene();
    ~RainbowCupcakeDec2Scene(){};
    
protected:
    RainbowCupcakeNode* _food;
    bool _firstSticker = true;
    bool _firstCup = false;
    bool _firstStraw = true;
    bool _firstBg = true;
    bool _firstCandy = false;
    AddSprinkle* _currentSprinkle;

private:
    void showScrollView(int index);
};
#endif /* RainbowCupcakeDec2Scene_h */
