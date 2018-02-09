

#ifndef MICAudioManager_h
#define MICAudioManager_h

#include "SingleTon.h"
#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;


class MICAudioManager : public SingleTon<MICAudioManager>
{
public:
    void requestPermision();
    bool deviceReady();
    void startRecord();
    void stopRecord();
    int getPower();
    std::function<void(bool)> onRequestPermisionCallback;
};
#endif /* MICAudioManager_h */
