//
//  EatLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef EatLayer_hpp
#define EatLayer_hpp
#include "EatSprite.h"
#include "BaseStepLayer.h"

class EatLayer: public BaseStepLayer{
    DECLARE_DYNCLASS(EatLayer);
public:
    ~EatLayer();
    CREATE_FUNC(EatLayer);
    bool init();
    virtual void onEnterTransitionDidFinish();
     virtual void onExitTransitionDidStart();
protected:
    void showEnd();
    void touchEnd(ui::Widget* widge);
    string _imagePath;
    Vec2 _eatAgainPos;
    EatSprite *_eatSprite;
protected:
    void checkEnd();
    void saveAbumle();
    void saveMyMenu();
    void homeClick();
    static const string _eventName;
};

#endif /* EatLayer_hpp */
