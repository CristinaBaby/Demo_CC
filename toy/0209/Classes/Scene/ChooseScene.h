//
//  ChooseScene.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/1.
//
//

#ifndef __ColorBook0818__ChooseScene__
#define __ColorBook0818__ChooseScene__

#include "cocos2d.h"
#include "PictureLayer.h"
#include "PackLayer.h"
USING_NS_CC;

class ChooseScene : public Scene{
public:
    static ChooseScene* create(bool forward, int packIndex = 0, int count = 0);
    virtual bool init(bool forward, int packIndex = 0, int count = 0);
    void onEventStartTurning(EventCustom* event);
    void onEventEndTurning(EventCustom* event);
    void onEventFromPictureToPack(EventCustom* event);
    void onScheduleFromPictureToPack(float dt);
    void beBack() { _beBack = true; };
    void onEnterTransitionDidFinish();
    ~ChooseScene();
private:
    PictureLayer *_pictureLayer;
    
    PackLayer* _packLayer;
    
    LayerColor* _layer;
    
    EventListenerCustom* _listener;
    
    vector<int> _data;
    
    int _packIndex;
    
    int _count;
    
    ParticleSystemQuad * _particle;
    
    bool _beBack = false;
};

#endif /* defined(__ColorBook0818__ChooseScene__) */
