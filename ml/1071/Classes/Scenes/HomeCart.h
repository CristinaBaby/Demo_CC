//
//  HomeCart.h
//  Hotdog
//
//  Created by wuguiling on 16/12/2.
//
//

#ifndef HomeCart_h
#define HomeCart_h

#include "cocos2d.h"
#include "Heads.h"
#include "GameUILayoutLayer.h"
#include "GuideLayer.h"

USING_NS_CC;

class HomeCart:public Node {
    
    
public:
    CREATE_FUNC(HomeCart);
    bool init();
    
    void runCar();
    void stopRunCar();
    void stopCartAction();
    void openCar();
private:
    Armature* m_pCar;
};
#endif /* HomeCart_h */
