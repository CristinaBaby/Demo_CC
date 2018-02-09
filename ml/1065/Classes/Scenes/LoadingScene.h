#ifndef __LoadingScene__
#define __LoadingScene__

#include "cocos2d.h"

USING_NS_CC;

class LoadingScene : public LayerColor{
public:
    static Scene* scene();
    
    virtual bool init();
    void preLoad();
    void onCallBack();
    
    CREATE_FUNC(LoadingScene);
    int m_nCount = 0;
};

#endif /* defined(__LoadingScene__) */
