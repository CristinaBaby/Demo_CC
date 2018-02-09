#ifndef coffee_BaseStepLayer
#define coffee_BaseStepLayer

#include"cocos2d.h"
#include "DynBase.h"
#include"IAPManager.h"
#include"AdsManager.h"
#include "cocostudio/cocostudio.h"
#include "STSystemFunction.h"
#include "ui/CocosGUI.h"
#include "SoundPlayer.h"
#include "StudioLayer.h"
#include "CocosHelper.h"
#include "ActionHelper.h"
#include "StudioLayer.h"
USING_NS_CC;
using namespace cocostudio;
class BaseStepLayer : public Layer,public DynObject
{
public:
    BaseStepLayer();
    virtual ~BaseStepLayer();
    StudioLayer * getOperateLayer(){return _operate;}
protected:
    virtual void touchEnd(ui::Widget* widget);
    virtual bool onBack();
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    void stopPrompt();
protected:
    StudioLayer *_operate;
    Sprite* _prompt;
    STSystemFunction _cfsys;
    int _loopSound;
};

#endif