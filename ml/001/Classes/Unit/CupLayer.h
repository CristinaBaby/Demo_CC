
#ifndef __CupLayer__
#define __CupLayer__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class CupLayer : public Layer
{
public:
    CupLayer();
    ~CupLayer();
    
    CREATE_FUNC(CupLayer);
    
    virtual bool init();
    
    void initWithIndex(int index);
    void showWater(int flavor);
    void updatePattern(int index);
    
    void showFinished(bool strawDirectionLeft = true);
    std::function<void()>onOverspill;
    
    void updateWater(int index);
    void addWater(float height = 5);
    void pauseAddWater();
    void resumeAddWater();
    
    float getWaterPercent(){return m_fPercent;}
    void setWaterPercent(int percent = 0);
    void setWaterStartPositionWorldPoint(Vec2 pos){waterStartPos = this->convertToNodeSpace(pos);}
    void setWaterStreamWorldPoint(Vec2 pos){waterStreamPos = this->convertToNodeSpace(pos);}
    
    void saveWaterImage();
    void showOpenLid(bool open = true);
protected:
    void update(float dt);
    
    Sprite* m_pCupShadow;
    Sprite* m_pCupBack;
    Sprite* m_pCupFront;
    Sprite* m_pLidBack;
    Sprite* m_pLidFront;
    Sprite* m_pPattern;
    Sprite* m_pStraw;
    Sprite *m_pWater;
    ClippingNode *m_pSodaClippingNode;  //soda
    Vec2 waterStartPos;
    float m_fCurHeight;
    float m_fHeightDelta;
    int m_fPercent;
    int m_nFlavorIndex;
    Vec2 waterStreamPos;
};

#endif