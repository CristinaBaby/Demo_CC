//
//  LoadingPage.h
//
//

#ifndef __MakeSnowMan__LoadingPage__
#define __MakeSnowMan__LoadingPage__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LoadingPage :public Layer
{
public:
    LoadingPage();
    ~LoadingPage();
    static Scene* scene();
    CREATE_FUNC(LoadingPage);
    bool init();
private:
    void goToNextPage();
//    virtual void onEnter();
//    virtual void onExit();
    
};


#endif /* defined(__MakeSnowMan__LoadingPage__) */
