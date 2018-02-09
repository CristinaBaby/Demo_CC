
#ifndef ProgressStar_h
#define ProgressStar_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ProgressStar:public Node {
    
public:
public:
    ProgressStar();
    CREATE_FUNC(ProgressStar);
    bool init();
    
    void addStar();
private:
    Sprite* m_pBarBg;

    int m_nStarCount;
};
#endif /* ProgressStar_h */