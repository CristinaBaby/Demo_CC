
#ifndef __SodaMachine__
#define __SodaMachine__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SodaMachine : public Layer
{
public:
    SodaMachine();
    ~SodaMachine();
    
    CREATE_FUNC(SodaMachine);
    
    virtual bool init();
    
    void initWithName(const std::string contentPath,const std::string lockPath,bool locked = false);
    void initWithFlavor(int indexh,const std::string lockPat,bool locked = false);
    
    virtual bool TouchBegan(Touch *pTouch, Event *pEvent);
    virtual void TouchMoved(Touch *pTouch, Event *pEvent);
    virtual void TouchEnded(Touch *pTouch, Event *pEvent);
    virtual void TouchCancelled(Touch *pTouch, Event *pEvent);
    
    std::function<void(SodaMachine* item)>onOpen;
    std::function<void(SodaMachine* item)>onClose;
    std::function<void(SodaMachine* item)>onItemClicked;
    void unlock();
    bool isLocked(){return m_bLocked;}
    Sprite* getContentSprite(){return m_pContentSrite;}
    Sprite* getLockSprite(){return m_pLock;}
    
    Vec2 getMaterialValveWorldPoint();
    void rotateFlaver();
    void stopRotateFlavor();
protected:
    bool m_bLocked;
    Sprite* m_pContentSrite;
    Sprite* m_pLock;
    Button *m_pSwitch;  //开关
    Sprite *m_pWater;
    ClippingNode *m_pFlavor;  //soda
    Sprite *m_pCoverBoad;  //盖板
    Sprite *m_pMaterialValve;  //出液阀
};

#endif