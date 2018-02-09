//
//  DecorationLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef FruitTartAddFruit_hpp
#define FruitTartAddFruit_hpp

#include "BaseDecorationLayer.h"
class FruitTartAddFruit: public BaseDecorationLayer{
public:
    CREATE_SECENE(FruitTartAddFruit);
    FruitTartAddFruit();
    ~FruitTartAddFruit();
    bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
protected:
    void touchEnd(ui::Widget* widge);
    virtual void freeClick(KDS::EventSprite*,size_t,KDS::MyScollView*);
    void reset();
    ClippingNode* clipping;
private:
    static const string _eventName;
    int chooseIndex;
};

#endif /* DecorationLayer_hpp */
