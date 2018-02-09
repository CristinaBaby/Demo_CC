//
//
//  Created by tanshoumei on 1/21/16.
//
//

#pragma once

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class ComponentBase: public Component
{
public:
    virtual void start(){}
    
    virtual void pause(){}

    virtual void resume(){}
    
    virtual void stop(){}
};

