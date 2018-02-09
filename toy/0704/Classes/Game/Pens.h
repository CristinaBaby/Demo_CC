//
//  Pens.h
//  TOYBM0701
//
//  Created by huangwen on 15/11/10.
//
//

#ifndef Pens_hpp
#define Pens_hpp

#include "cocos2d.h"
#include "cocosgui.h"
USING_NS_CC;
class GameScene;

class Pens : public ui::Widget{
public:
    static Pens* create(int index, GameScene * game, bool net);
    
    bool initPens(int index, GameScene * game, bool net);
    
    void unLock();
private:
    int _sign = 0;
};

#endif /* Pens_hpp */
