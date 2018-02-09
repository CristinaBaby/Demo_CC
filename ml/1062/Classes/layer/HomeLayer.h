//
//  HomeLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#ifndef HomeLayer_hpp
#define HomeLayer_hpp

#include "BaseHomeLayer.h"

class HomeLayer: public BaseHomeLayer{
     DECLARE_DYNCLASS(HomeLayer);
public:
    CREATE_SECENE(HomeLayer);
protected:
    bool init();
     void showAdsCallBack();
    virtual void onEnterTransitionDidFinish();
     virtual void touchEnd(ui::Widget* widget);
    void playClick();
};

#endif /* HomeLayer_hpp */
