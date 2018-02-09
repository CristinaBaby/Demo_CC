//
//  ChangeLoading.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/20.
//
//

#ifndef ChangeLoading_hpp
#define ChangeLoading_hpp

#include "cocos2d.h"
USING_NS_CC;
class ChangeLoading : public Layer {
public:
    CREATE_FUNC(ChangeLoading);
    ChangeLoading();
    static void loading(std::function<void()>);
protected:
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();
    virtual void onExit();
    bool init();
    bool isEnd;
    void showLogo();
    Sprite* _logo;
public:
    std::function<void()> endFunc;
};

#endif /* ChangeLoading_hpp */
