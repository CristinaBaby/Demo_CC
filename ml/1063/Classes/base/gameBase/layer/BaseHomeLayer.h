//
//  BaseHomeLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#ifndef BaseHomeLayer_hpp
#define BaseHomeLayer_hpp

#include "BaseStepLayer.h"
class BaseHomeLayer:public BaseStepLayer{
public:
    BaseHomeLayer();
protected:
    virtual bool onBack();
    virtual void onEnterTransitionDidFinish();
      virtual void touchEnd(ui::Widget* widget);
    virtual void showAdsCallBack() =0;
        virtual void playClick();
protected:
    string _shopClass,startClass,startEffect;
};

#endif /* BaseHomeLayer_hpp */
