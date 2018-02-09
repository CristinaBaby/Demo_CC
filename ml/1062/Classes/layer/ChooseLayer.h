//
//  ChooseLayer.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/18.
//
//

#ifndef ChooseLayer_hpp
#define ChooseLayer_hpp

#include "BaseStepLayer.h"
#include "BookView.h"
class ChooseLayer : public BaseStepLayer{
     DECLARE_DYNCLASS(ChooseLayer);
public:
   CREATE_SECENE(ChooseLayer)
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    int currentPage;
    void pageControl();
    void update(float);
    BookView * bookView;
     virtual void touchEnd(ui::Widget* widget);
};
#endif /* ChooseLayer_hpp */
