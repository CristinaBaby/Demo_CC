//
//  MoregameBtn.h
//  JapaneseFood
//
//  Created by youjinchao1 on 15-3-5.
//
//

#ifndef __JapaneseFood__MoregameBtn__
#define __JapaneseFood__MoregameBtn__

#include <iostream>
#include "cocos2d.h"
#include "MoreGameLoader.h"
using namespace std;
USING_NS_CC;

class MoregameBtn : public Layer{
public:
    enum MoregameIconDirection{
        VERTICAL = 0,
        HORIZONTAL
    };
    enum MoreGameModel{
        HomeView = 1,
        GameView,
    };
public:
    
    MoregameBtn();
    ~MoregameBtn();
    virtual bool init();
    CREATE_FUNC(MoregameBtn);
    CC_SYNTHESIZE(MoregameIconDirection, _direction, Direction);
    CC_SYNTHESIZE(MoreGameModel, _viewModel, ViewModel);
private:
    Node* container;
    Vec2 originalPoint;
public:
    virtual void onEnter();
    virtual void onExit();
    void addtoParentLayer(Layer* layer);
    void startLoading();
protected:
    void onLoadmoregameIconSucceed(Ref* pRef);
    void onBannerDidload(Ref* pRef);
    void moregameClicked(Ref* pObj);
    float getBannerHeight();
};


#endif /* defined(__JapaneseFood__MoregameBtn__) */
