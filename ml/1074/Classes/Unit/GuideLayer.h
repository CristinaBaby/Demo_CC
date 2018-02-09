
#ifndef __GuideLayer__
#define __GuideLayer__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class GuideLayer : public Layer
{
public:
    GuideLayer();
    ~GuideLayer();
    
    CREATE_FUNC(GuideLayer);
    
    virtual bool init();
    
    void showGuideMove(Vec2 begin,Vec2 end,float delayTime = 0.0f);
    void showGuideTap(Vec2 position,float delayTime = 0.0f);
    void showGuideTapDirection(Vec2 position,bool isLeft = true,float delayTime = 0.0f);
    void showGuideTapVertical(Vec2 position,float delayTime = 0.0f);
    void showGuideTapHorizonal(Vec2 position,float delayTime = 0.0f);
    void showGuideShake(Vec2 position,float delayTime = 0.0f);
    void showGuideRotateRight(Vec2 position,float delayTime = 0.0f);
    void showGuideRotateLeft(Vec2 position,float delayTime = 0.0f);
    void showGuideMix(Vec2 position,float delayTime = 0.0f);
    //    direction -1 箭头超左  1 箭头朝右  2 朝上  －2 朝下
    void showGuideArrow(Vec2 position,int direction,float delayTime = 0.0f);
    void showGuideScrollHorizonal(Vec2 position,float delayTime = 0.0f);
    void showGuide(std::string path,Vec2 position,float rotate = 0,float delayTime = 0.0f);
    void removeGuide();
    
    Sprite* getGuide(){
        return m_pFinger;
    }
    std::function<void(int,bool)>onItemClicked;
protected:
    Sprite* m_pFinger;
    Sprite* m_pArrow;
    Sprite* m_pPoint;
};

#endif