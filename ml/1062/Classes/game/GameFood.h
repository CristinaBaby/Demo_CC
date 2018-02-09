//
//  FoodRead.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#ifndef GameFood_hpp
#define GameFood_hpp

#include "SingleInstance.h"
#include "cocos2d.h"
USING_NS_CC;
class GameFood : public SingleInstance<GameFood>{
    CC_SYNTHESIZE(int, _foodIndex, FoodIndex);
public:
    GameFood();
    Node* createBeforeDip();
    Node* createBeforePowder();
    Node* createFryFood();
    Node* createFryEndInplate();
    const std::vector<Sprite*>& getFoods();
    Sprite* getDippedFood(int index);
    std::vector<std::string> getDipPath();
    std::string getFryPath();
    std::vector<std::string> getChoosePath();
    std::string getDecorationPath();
    const std::vector<Sprite*>& getInOilFood();
    std::string getFryFailedPath();
    std::vector<Vec2> getCoateEndPos();
protected:
    std::vector<Sprite*> foods;
    static int z_order;
    Vector<Sprite*> positions;
};
#endif /* GameFood_hpp */
