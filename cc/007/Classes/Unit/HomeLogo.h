
#ifndef HomeLogo_h
#define HomeLogo_h

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

class HomeLogo :public Layer
{
public:
    HomeLogo();
    ~HomeLogo();
    CREATE_FUNC(HomeLogo);
    bool init();
    
    void onEnter();
    
    void showAnimation();
    
    
    Sprite *m_pFrame;
    Sprite *m_pTomato;
    Sprite *m_pTool;
    Sprite *m_pLeaveLeft;
    Sprite *m_pLeaveRight;
    Sprite *m_pLeaveRight2;
    Sprite *m_pText;
    Sprite *m_pStar;
};
#endif /* HomeLogo_h */
