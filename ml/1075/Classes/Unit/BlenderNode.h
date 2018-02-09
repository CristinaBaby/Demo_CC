
#ifndef __BlenderNode__
#define __BlenderNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BlenderCup.h"

USING_NS_CC;
using namespace cocos2d::ui;

class BlenderNode : public Layer
{
public:
    BlenderNode();
    ~BlenderNode();
    
    static BlenderNode* create(int type)
    { 
        BlenderNode *pRet = new(std::nothrow) BlenderNode();
        if (pRet && pRet->init(type))
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
    
    virtual bool init(int type);
    
    BlenderCup* getCup(){
        return m_pCup;
    }
    
    void setReadyWork(bool ready = true){
        m_bReadyWork = ready;
        if(m_nType==1){
            m_pSwitch->setTouchEnabled(ready);
            m_pSwitch->setEnabled(ready);
        }
    }
    Rect getButtonWorldRect(){
        Rect rect = Rect::ZERO;
        if(m_nType==0){
            rect = m_pHandle->getBoundingBox();
        }else if(m_nType==1){
            rect = m_pSwitch->getBoundingBox();
        }
        rect.origin = this->convertToWorldSpace(rect.origin);
        return rect;
    }
    
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    std::function<void()>onBlenderWorking;
    std::function<void()>onBlenderStopWorking;
    std::function<void()>onBlenderSwitchTurnOff;
    
    void onButtonCallback(Ref* ref,Widget::TouchEventType type);
protected:
    
    Sprite* m_pBody;
    Sprite* m_pHandle;
    Button* m_pSwitch;
    BlenderCup* m_pCup;
    int m_nType;
    bool m_bReadyWork;
    int m_nStep;
    int m_nCount;
    bool m_bFinishCrush;
};

#endif