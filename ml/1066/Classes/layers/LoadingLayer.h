//
//  LoadingLayer.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-18.
//
//

#ifndef __OreoMaker__LoadingLayer__
#define __OreoMaker__LoadingLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class LoadingLayer :public Layer
{
public:
    LoadingLayer();
    
    ~LoadingLayer();
    
    static Scene* scene();
    CREATE_FUNC(LoadingLayer);
    
    virtual bool init();
    
    bool preLoadingData();
    bool initUI();
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
private:
    int m_curNum;//当前加载到第几张
    
    int m_totalNum;//加载总数
};
#endif /* defined(__OreoMaker__LoadingLayer__) */
