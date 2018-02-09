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
    HomeLayer();
    CREATE_FUNC(HomeLayer);
    CC_SYNTHESIZE(bool, autoWriteLogo, AutoWriteLogo)
protected:
     virtual void playClick();
    bool init();
     void showAdsCallBack();
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
     virtual void touchEnd(ui::Widget* widget);
 
    
};

#endif /* HomeLayer_hpp */
