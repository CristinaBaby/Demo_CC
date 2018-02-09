//
//  PopcornColorMakeScene.h
//  shake004
//
//  Created by liji on 16/10/25.
//
//

#ifndef PopcornColorMakeScene_h
#define PopcornColorMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class BowlNode;
class TapDropNode;

class PopcornColorMakeScene : public ESMakeBaseScene
{
public:
    
    PopcornColorMakeScene();
    ~PopcornColorMakeScene();
    
    MY_SCENE(PopcornColorMakeScene);
    CREATE_FUNC(PopcornColorMakeScene);
    bool init();
    
protected:
    BowlNode *_bowl;
    TapDropNode *_previousColor;
    int _butterTipIndex;
    Sprite* _scrollBG;
    BowlNode *_box;

    
private:
    void showBowl();
    void addWater();

    void showIngredients();
    void addButter();
    void addSugar();
    void addSyrup();
    
    void addColors();
    void colorChoosed(int index);
    
    void mixColors();
    
    void mixPopcorn();
    
    void chooseBox();
    void fillPopcorn();
    
};

#endif /* PopcornColorMakeScene_h */
