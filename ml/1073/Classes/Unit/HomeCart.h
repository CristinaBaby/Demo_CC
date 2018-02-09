
#ifndef HomeCart_h
#define HomeCart_h

#include "cocos2d.h"
#include <stdio.h>
#include "cocostudio/CCArmature.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

using namespace cocostudio;

USING_NS_CC;

class HomeCart:public Node {
    
    
public:
    HomeCart();
    ~HomeCart();
    CREATE_FUNC(HomeCart);
    bool init();
    
    void loadCartType(int type,bool left = true);
    void runCart();
    void stopRunCart();
    
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    std::function<void(HomeCart*)> onCartClickCallback;
private:
    Sprite* m_pBody;
    Vector<Sprite*> m_pWheelVector;
    bool m_bIsLeft;
};
#endif /* HomeCart_h */
