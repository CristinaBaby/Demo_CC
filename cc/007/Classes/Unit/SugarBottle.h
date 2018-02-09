
#ifndef __SugarBottle__
#define __SugarBottle__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SugarBottle : public Node
{
public:
    SugarBottle();
    ~SugarBottle();
    
    CREATE_FUNC(SugarBottle);
    
    virtual bool init();
    
    void initSugar(int index);
    void openLid();
    void CoverLid();
    void graySugar();
    void pourSugar();
protected:
    ClippingNode* m_pClippingNode;
//    Sprite* m_pMask;
//    RenderTexture* m_pRender;
    Sprite* m_pSugar;
//    Sprite* m_pShadow;
    Sprite* m_pBottleFront;
    Sprite* m_pBottleBack;
    Sprite* m_pLid;
    Sprite* m_pLabel;
    Sprite* m_pTag;
    int m_nFlavor;
};

#endif