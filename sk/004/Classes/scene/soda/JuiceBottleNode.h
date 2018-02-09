//
//  JuiceBottleNode.h
//  ccats1061
//
//  Created by liji on 16/8/22.
//
//

#ifndef JuiceBottleNode_h
#define JuiceBottleNode_h

#include <stdio.h>
#include "BowlNode.h"
#include "MakeBaseNode.h"
#include "ESDataManager.h"
#define shareNameJuice "share_juice.png"
class JuiceBottleNode : public MakeBaseNode
{
public:
    CREATE_FUNC(JuiceBottleNode);
    bool init();
    
    JuiceBottleNode();
    ~JuiceBottleNode(){};
    
    void updateBottle(int index);
    void updateCream(int index);
    void updateBottleLid(int index);
    void updateStraw(int index);
    void addSprinkle(string sprinklePath, Vec2 worldPos);
    void addFood(string foodPath);
    void addIce();
    
    void saveJuiceImage(const std::function<void(RenderTexture*, const std::string&)>& callback);
    void showDrink();
    
    void showLid(bool animated = true);
    void enableDrink(const function<void()> &startDrinkingCallback , const function<void()> &finishedDrinkingCallback);
    void resetDrink();
    void reset();
private:
    BowlNode* _bottle;
    Sprite* _lid;
    Sprite* _lidBtm;
    Sprite* _straw;
    Sprite* _strawShadow;
    Sprite* _juiceInBottle;
    Sprite* _cream;
    bool isFirst;

};



#endif /* JuiceBottleNode_h */
