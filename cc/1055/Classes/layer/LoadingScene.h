//
//  LoadingScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-18.
//
//

#ifndef __OreoMaker__LoadingScene__
#define __OreoMaker__LoadingScene__

#include <iostream>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "./extensions/cocos-ext.h"
#include "Configure.h"

USING_NS_CC;

class LoadingScene : public Scene {
public:
    
    CREATE_FUNC(LoadingScene);
    CREATE_SCENE(LoadingScene);
    virtual bool init();
    
    void loading();
    void preLoad();
    void callBack();
    
private:
    int m_curNum;//当前加载到第几张
    int m_totalNum;//加载总数
};
#endif /* defined(__OreoMaker__LoadingScene__) */
