//
//  ChangeLayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/23.
//
//

#ifndef ChangeLayer_hpp
#define ChangeLayer_hpp

#include "BaseStepLayer.h"
class ChangeLayer:public BaseStepLayer{
public:
    CREATE_FUNC(ChangeLayer);
    static void loading();
    static std::function<Layer*()> _changeLayer;
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    void endFunc();
    static bool isShow;
    int soundIndex;
    virtual bool onBack(){return true;}
  
};

#endif /* ChangeLayer_hpp */
