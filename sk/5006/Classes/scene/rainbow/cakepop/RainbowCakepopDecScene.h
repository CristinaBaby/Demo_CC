//
//  RainbowCakepopDecScene.h
//  shake5008
//
//  Created by liji on 17/1/13.
//
//

#ifndef RainbowCakepopDecScene_h
#define RainbowCakepopDecScene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "CakePopNode.h"
#include "AddSprinkle.h"

using namespace std;
class ComponentTouchMove;
class AddSprinkle;

class RainbowCakepopDecScene : public DecorationBaseLayer
{
public:
    MY_SCENE(RainbowCakepopDecScene);
    CREATE_FUNC(RainbowCakepopDecScene);
    virtual bool init() override;
    RainbowCakepopDecScene();
    ~RainbowCakepopDecScene(){};
    
protected:
    CakePopNode* _food;
    bool _firstSticker = true;
    bool _firstCup = false;
    bool _firstStraw = true;
    bool _firstBg = true;
    AddSprinkle* _currentSprinkle;
    
private:
    void showScrollView(int index);
};

#endif /* RainbowCakepopDecScene_h */
