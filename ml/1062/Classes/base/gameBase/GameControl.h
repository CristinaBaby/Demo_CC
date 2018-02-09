//
//  GameControl.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/3.
//
//

#ifndef GameControl_h
#define GameControl_h

#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class GameControl {
public:
    static void changeLayer(Layer*);
    static void pushBackLayer(Layer*);
    static void changeScene(Scene*);
    static void pushBackScene(Scene*);
    static void goToPreLayer(Layer*);
    static void goToPreScene();
};

#endif /* GameControl_hpp */
