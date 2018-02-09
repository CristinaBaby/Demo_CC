
#ifndef __BlenderCup__
#define __BlenderCup__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class BlenderCup : public Layer
{
public:
    BlenderCup();
    ~BlenderCup();
    
//    CREATE_FUNC(BlenderCup);
    static BlenderCup* create(bool physics = true)
    { 
        BlenderCup *pRet = new(std::nothrow) BlenderCup();
        if (pRet && pRet->init(physics)) 
        { 
            pRet->autorelease(); 
            return pRet; 
        } 
        else 
        { 
            delete pRet; 
            pRet = NULL; 
            return NULL; 
        } 
    }
    virtual bool init(bool physics);
    
    void initWithIndex(int flavor);
    
//    state set
    void setStateEmpty();
    void addWater();
    void addIce(int iceType,Vec2 posWorld);
    void addIces(int iceType,int count);
    void addFruits(int fruitType,int range,int count);
    
    void crushIce();
    void crushIceManual(int step);
    void pauceCrushIceManual(int step);
    void resumeCrushIceManual(int step);
    void finishedCrush();
    
    void showFinished(bool strawDirectionLeft = true);
    
    void pourJuice();
    
    std::function<void()>onOverspill;
    std::function<void()>onBeginCrushIce;
    std::function<void()>onFinishCrushIce;
    std::function<void()>onFinishPour;
    std::function<void()>onLidOpen;
    
    void updateWater(int index);
    void addJuice(float height = 5);
    void pauseAddJuice();
    void resumeAddJuice();
    
    void setBodyData();
    float getWaterPercent(){return m_fPercent;}
    void setWaterPercent(int percent = 0);
    void setWaterStartPositionWorldPoint(Vec2 pos){waterStartPos = this->convertToNodeSpace(pos);}
    void setWaterStreamWorldPoint(Vec2 pos){waterStreamPos = this->convertToNodeSpace(pos);}
    
    void saveWaterImage();
    void showShadow(bool show = true);
    void showLid(bool show = true,bool animate = true);
    Vec2 getCupWorldPos(){
        Vec2 pos = Vec2(8,350);
        pos = m_pCupFront->convertToWorldSpace(pos);
        return pos;
    }
    Rect getCupWorldRect(){
        Rect rect = m_pCupBack->getBoundingBox();
        rect.origin = m_pCupBack->getParent()->convertToWorldSpace(rect.origin);
        return rect;
    }
    
    bool getLidShown(){
        return m_bLidIsShow;
    }
    
    void setTouchEnable(bool enable = true){
        m_bTouchEnable = enable;
    }
protected:
    void update(float dt);
    
    Sprite* m_pCupBack;
    Sprite* m_pCupFront;
    Sprite* m_pLid;
    Sprite* m_pShadow;
    Sprite *m_pWater;
    Sprite *m_pWaterFront;
    Sprite *m_pWaterDrop;
    ClippingNode *m_pClippingNode;  //juice
    NodeGrid* m_pWaterGrid;
    NodeGrid* m_pWaterFrontGrid;
    Node* m_pIceNode;
    std::vector<Sprite*> m_IcesVector;
    std::vector<Sprite*> m_FruitsVector;
    
    Vec2 waterStreamPos;
    Vec2 waterStartPos;
    float m_fCurHeight;
    float m_fHeightDelta;
    int m_fPercent;
    int m_nFlavorIndex;
    int m_nIceType;
    int m_nStep;
    bool m_bPhysics;
    
    int m_nFruitType;
    int m_nFruitRange;
    
    bool m_bLidIsShow;
    bool m_bTouchEnable;
};

#endif