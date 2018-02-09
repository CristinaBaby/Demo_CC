//
//  MXPageView.hpp
//  DonutMaker
//
//  Created by maxiang on 10/12/15.
//
//

#ifndef MXPageView_hpp
#define MXPageView_hpp

#include "cocos2d.h"
#include "cocosGUI.h"

class MXPageView : public cocos2d::ui::PageView
{
public:
    static MXPageView* create();
    bool init() override;

protected:
    virtual void handleReleaseLogic(cocos2d::Touch *touch) override;
};


#endif /* MXPageView_hpp */
