//
//  MiniShopScene.h
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#ifndef MiniShopScene_h
#define MiniShopScene_h

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

class MiniShopScene : public StudioLayer
{
public:
    
    CREATE_FUNC(MiniShopScene);
    bool init();
    
    MiniShopScene();
    ~MiniShopScene();

    virtual void onEnter();
    virtual void onExit();
    virtual void onButtonClick(Ref* ref,Widget::TouchEventType type);
    virtual void onCheckBoxClick(Ref*,CheckBox::EventType);
    
    function<void()> _onRewardButtonClicked = nullptr;
    std::function<void()> onCloseCallback = nullptr;
    function<void()> _onShopButtonClicked = nullptr;
    void showDecorationButton();
    void showAllFoodButton();
    
    
private:
    void initScrollView();
    
protected:
    int _curIapIndex = 0;
};

#endif /* MiniShopScene_h */
