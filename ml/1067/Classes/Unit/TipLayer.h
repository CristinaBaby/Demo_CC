
#ifndef TipLayer_h
#define TipLayer_h

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

class TipLayer :public Layer
{
public:
    TipLayer();
    ~TipLayer();
    CREATE_FUNC(TipLayer);
    bool init();
    
    void onEnter();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    void showTip(std::string text,Vec2 pos = Vec2::ZERO);
private:
//    LayerColor *pLayerColor;
//    Sprite* m_pBg;
//    Label* m_pContentLabel;
    Sprite* m_pText;
};
#endif /* TipLayer_h */
