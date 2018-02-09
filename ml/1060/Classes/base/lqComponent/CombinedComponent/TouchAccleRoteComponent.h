//
//  TouchAccleRoteComponent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#ifndef TouchAccleRoteComponent_hpp
#define TouchAccleRoteComponent_hpp

#include "AccleRoteComponent.h"
#include "TouchNodeComponent.h"

class TouchAccleRoteComponent : public AccleRoteComponent {
      DECLARE_COMPONENT_DYNCLASS(TouchAccleRoteComponent);
public:
    TouchAccleRoteComponent();
    virtual bool init();
};

#endif /* TouchAccleRoteComponent_hpp */
