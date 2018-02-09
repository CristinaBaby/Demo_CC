//
//  MakeIceLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__MakeIceLayer__
#define __Make__MakeIceLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "MachineView.h"
USING_NS_CC;

class MakeIceLayer :public BaseLayer , public MachineViewDelegate
{
public:
    static Scene* scene();
    CREATE_FUNC(MakeIceLayer);
    bool init();

private:
    virtual void onEnter();
    virtual void onExit();
    virtual void nextBtnClicked();
    virtual void allStepFinished();

    
    

private:
    Sprite* machine_;
    Sprite* hint_;
    Sprite* machineCover_;
    Sprite* iceBag_;
    Sprite* hand_;
    Sprite* pile_;
    Vector<Sprite *> vec_ice;
    int iceCount_;
};
#endif /* defined(__Make__MakeIceLayer__) */
