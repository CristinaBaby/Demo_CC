//
//  ShopScene.h
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#ifndef ShopScene_h
#define ShopScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "depends/component/layerext/StudioLayer.h"
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "scene/shop/CCyclePageView.h"
#include "SSCIAPManager.h"
#include "Charactor.h"
#include "ComponentCycleScroll.h"
using namespace cocos2d;
using namespace std;

#define IAP_FullVersion                    "com.shakeitmediainc.rainbowdesserts.unlockall"
#define IAP_AllContent                     "com.shakeitmediainc.rainbowdesserts.decoration"
#define IAP_AllContent_A                   "com.kidsfoodinc.amazon_rainbowdesserts.decoration"
#define IAP_AllContent_G                   "com.kidsfoodinc.android_rainbowdesserts.decoration"
#define IAP_AllFood                        "com.shakeitmediainc.rainbowdesserts.foodpack"
#define IAP_AllFood_A                      "com.kidsfoodinc.amazon_rainbowdesserts.foodpack"
#define IAP_AllFood_G                      "com.kidsfoodinc.android_rainbowdesserts.foodpack"

class ShopScene : public StudioLayer
{
public:
    
    CREATE_FUNC(ShopScene);
    bool init();
    
    ShopScene();
    ~ShopScene();
    
    std::function<void()> onCloseCallback = nullptr;
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual void onButtonClick(Ref* ref,Widget::TouchEventType type);
    virtual void onCheckBoxClick(Ref*,CheckBox::EventType);

    static string getAllContentString();
    static string getAllFoodString();
    
    
private:
    void initScrollView();
    
protected:
    int _curIapIndex = 0;
};

#endif /* ShopScene_h */
