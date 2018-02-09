//
//  ScribbleLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__ScribbleLayer__
#define __Make__ScribbleLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "MakeLayer.h"
#include "ChooseFlavorView.h"
USING_NS_CC;

class ScribbleLayer :public BaseLayer, public ChooseFlavorViewDelegate
{
public:
    static Scene* scene();
    CREATE_FUNC(ScribbleLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual void nextBtnClicked();
    virtual void resetBtnClicked();

    virtual void shopClicked();
    virtual void unitSelected(const char* pUnitImageName);

    MakeLayer* makeLayer_;
    ChooseFlavorView* chooseFlavorView_;
};
#endif /* defined(__Make__ScribbleLayer__) */
