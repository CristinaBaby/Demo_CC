//
// LoadingLayer.h
// ANIM3203
//
// Created by dengcheng on 15/10/10.
//
//

#ifndef __ANIM3203__LoadingLayer__
#define __ANIM3203__LoadingLayer__

#include "cocos2d.h"
#include "AdLoadingLayerBase.h"

USING_NS_CC;

class LoadingLayer : public AdLoadingLayerBase
{
public:
    LoadingLayer();
    ~LoadingLayer();

    static LoadingLayer* create();
    virtual bool init();

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    std::vector<Sprite *> m_vDots;
private://method
    
    void onChangeDotStatus(float aTime);

};

#endif /* defined(__ANIM3203__LoadingLayer__) */