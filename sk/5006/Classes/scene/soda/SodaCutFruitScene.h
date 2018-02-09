//
//  SodaCutFruitScene.h
//  CCATS022
//
//  Created by liji on 16/11/7.
//
//

#ifndef SodaCutFruitScene_h
#define SodaCutFruitScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
#include "ESDataManager.h"
#include "BowlNode.h"
class SodaCutFruitScene : public ESMakeBaseScene
{
public:
    
    SodaCutFruitScene();
    ~SodaCutFruitScene();
    
    MY_SCENE(SodaCutFruitScene);
    CREATE_FUNC(SodaCutFruitScene);
    bool init();
    
protected:
    int _partIndex;
    Sprite* _board;
    BowlNode* _bowl;
    vector<Sprite*> _pieceVec;
    

    
private:
    void initChooseFruit();
    void initCutFruit(int index);
    void cut(vector<Vec2>, Node *friesNode,ESDataManager::CutFood *cutFood);
    void pourInBowl();
    void removeBowl();

};

#endif /* SodaCutFruitScene_h */
