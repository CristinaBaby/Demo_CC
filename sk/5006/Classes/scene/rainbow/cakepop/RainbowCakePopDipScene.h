//
//  RainbowCakePopDipScene.h
//  shake5008
//
//  Created by liji on 17/1/10.
//
//

#ifndef RainbowCakePopDipScene_h
#define RainbowCakePopDipScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class BowlNode;
class RainbowCakePopDipScene : public ESMakeBaseScene
{
public:
    
    RainbowCakePopDipScene();
    ~RainbowCakePopDipScene();
    
    MY_SCENE(RainbowCakePopDipScene);
    CREATE_FUNC(RainbowCakePopDipScene);
    bool init();
    
protected:
    int _tipIndex;
    Sprite* _pot;
    BowlNode* _bowl;
    Sprite* _light;
private:
    void showPot();
    void showChocolate();
    void showMix();
    void showDip();
    
};

#endif /* RainbowCakePopDipScene_h */
