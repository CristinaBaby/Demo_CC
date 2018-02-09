//
//  ChooseShapLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__ChooseShapLayer__
#define __Make__ChooseShapLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ChooseShapeView.h"
USING_NS_CC;

class ChooseShapLayer :public BaseLayer, public ChooseShapeViewDelegate
{
public:
    static Scene* scene();
    CREATE_FUNC(ChooseShapLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual void nextBtnClicked();

    virtual void shopClicked() ;
    virtual void unitSelected(std::string pUnitImageName,int idx);
    void checkunlock();
    
private:
    ChooseShapeView* chooseView_;

    
};
#endif /* defined(__Make__ChooseShapLayer__) */
