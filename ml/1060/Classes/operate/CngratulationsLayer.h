//
//  CngratulationsLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#ifndef CngratulationsLayer_hpp
#define CngratulationsLayer_hpp

#include "BaseStepLayer.h"

class CngratulationsLayer: public BaseStepLayer{
    DECLARE_DYNCLASS(CngratulationsLayer);
public:
    CngratulationsLayer();
    CREATE_FUNC(CngratulationsLayer);
protected:;
    bool init();
 
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    virtual void touchEnd(ui::Widget* widget);
    
    
};

#endif /* CngratulationsLayer_hpp */
