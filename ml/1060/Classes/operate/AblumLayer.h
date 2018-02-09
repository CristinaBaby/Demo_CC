//
//  AblumLayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#ifndef AblumLayer_hpp
#define AblumLayer_hpp

#include "BaseStepLayer.h"
class AblumLayer: public BaseStepLayer{
    DECLARE_DYNCLASS(AblumLayer);
public:
    CREATE_FUNC(AblumLayer);
    AblumLayer();
    ~AblumLayer();
    void setRender(RenderTexture* _render);
protected:
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    Image* _image;
    string _shareFilePath;
      virtual void touchEnd(ui::Widget* widget);
};

#endif /* AblumLayer_hpp */
