//
//  FlowerLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin on 16/4/24.
//
//

#ifndef FlowerLayer_hpp
#define FlowerLayer_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

using namespace std;
using namespace ui;

class FlowerLayer : public Layer {
    FlowerLayer();
    ~FlowerLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(FlowerLayer);
    static void Loading(Node* node,std::function<void()> endFunc = nullptr);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    Sprite* m_pAnumation;
    
    std::function<void()> _endfunc;
    
};


#endif /* FlowerLayer_hpp */
