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
    static std::function<Scene*()> _changeLayer;
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    static bool isShow;
    int soundIndex;
  
};

#endif /* ChangeLayer_hpp */
