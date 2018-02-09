//
//  TexasPizzaMakeScene.h
//  shake004
//
//  Created by liji on 16/10/17.
//
//

#ifndef TexasPizzaMakeScene_h
#define TexasPizzaMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class BowlNode;
class ScribbleNode;
class ComponentScribblePercent;
class TexasPizzaMakeScene : public ESMakeBaseScene
{
public:
    
    TexasPizzaMakeScene();
    ~TexasPizzaMakeScene();
    
    MY_SCENE(TexasPizzaMakeScene);
    CREATE_FUNC(TexasPizzaMakeScene);
    bool init();
    
protected:
    BowlNode* _plate;
    Sprite* _tempBread;
    int _breadCount;
    Sprite* _brush;
    Sprite* _oven;
    Sprite* _door;
    Sprite* _rotater;
    vector<Sprite*> _breadInPlateVec;
    vector<Sprite*> _cheeseOnBreadVec;
    vector<Sprite*> _otherOnBreadVec;
    vector<ScribbleNode*> _scribbleVec;
    Vector<ComponentScribblePercent*> _percentVec;
    ProgressTimer* _progressTimer;
    Sprite* _progressBg;

private:
    void addBread();
    void addBreadInPlate();
    
    void addScribble();
    void scribbleSauce();
    
    void addCheese();
    
    void addOther();
    void disableOther();
    
    void showOven();
    void putInOven();
    void setTemperature();
    void showRotate();
    
    void bake();
    void bakeOther();
    void bakeOver();
    
    void chooseBread();
};

#endif /* TexasPizzaMakeScene_h */
