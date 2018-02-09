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
#include "../Depends/base/BaseScene.h"
#include "cocostudio/CocoStudio.h"
#include "./extensions/cocos-ext.h"
class LoadingScene : public BaseScene {
public:
    
    CREATE_FUNC(LoadingScene);
    virtual bool init();
    LoadingScene();
    ~LoadingScene();
    void loading();
    void preLoad();
    void callBack();
    
private:
    int m_curNum;//当前加载到第几张
    int m_totalNum;//加载总数
    int time;
};
#endif /* defined(__OreoMaker__LoadingScene__) */
