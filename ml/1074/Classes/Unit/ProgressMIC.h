
#ifndef ProgressMIC_h
#define ProgressMIC_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ProgressMIC:public Node {
    
public:
public:
    ProgressMIC();
    CREATE_FUNC(ProgressMIC);
    bool init();
    
    void addPercent(int dt);
    void updatePercent(int pecent);
    
    std::function<void()> onFinishCallback;
private:
    Sprite* m_pBarBg;
    Sprite* m_pProgress;
    ClippingNode* m_pClipping;

    int m_nPercent;
    float lenght;
    bool m_bFinished;
};
#endif /* ProgressMIC_h */