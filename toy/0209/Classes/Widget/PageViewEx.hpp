//
//  PageViewEx.hpp
//  TOYBM0205
//
//  Created by huangwen on 15/10/26.
//
//

#ifndef PageViewEx_hpp
#define PageViewEx_hpp

#include "cocos2d.h"
#include "cocosgui.h"

USING_NS_CC;

class PageViewEx : public cocos2d::ui::PageView{
public:
    static PageViewEx* create();
    
    virtual void handleReleaseLogic(Touch *touch) override;
};

#endif /* PageViewEx_hpp */
