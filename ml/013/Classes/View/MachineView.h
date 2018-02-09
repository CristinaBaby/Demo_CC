//
//  MachineView.h
//  Make
//
//  Created by liji on 15/5/28.
//
//

#ifndef __Make__MachineView__
#define __Make__MachineView__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class MachineViewDelegate
{
public:
    virtual void allStepFinished(){};

};


class MachineView : public Layer
{
public:
    CREATE_FUNC(MachineView);
    bool init();
    void setDelegate(MachineViewDelegate* delegate){delegate_ = delegate;};
    
private:
    void initMachine();
    void showIceBag();
    void iceBagClicked();
    void machineCoverClosed();
    void showMachineBtn();
    void runMachine();
    void iceEffect(float dt);
    void snowconeFall(float dt);
    void showIcePile();
    void stepFinish();
private:
    Sprite* lid_;
    Sprite* boxBottom_;
    Sprite* boxCover_;
    Sprite* hint_;
    Sprite* iceBag_;
    Sprite* pile_;
    Sprite* btn_;
    MachineViewDelegate* delegate_;
    
    Vector<Sprite*> vec_ice;
    
    int iceCount_ = 0;
    EventListenerTouchOneByOne* listener_;
    
    float _rectPosY = 0.f;
    const float _totalTime = 3.f;
    float _speed = 0.f;
    
};

#endif /* defined(__Make__MachineView__) */
