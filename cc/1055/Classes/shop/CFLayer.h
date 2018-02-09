//
//  CFLayer.h
//  KidsFramework_CocosV3.2
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#ifndef __KidsFramework_CocosV3_2__CFLayer__
#define __KidsFramework_CocosV3_2__CFLayer__

#include <iostream>
#include "cocos2d.h"
//#include "KeypadDispatchCenter.h"
//#include "LockScreenLayer.h"
USING_NS_CC;

typedef enum
{
    kBackground         =       0,
    kContent            =       50,
    kUI                 =       100,
    kPop                =       200,
}LayerZOrder;


class CFLayer : public Layer
{
protected:
    CFLayer(){}
    virtual ~CFLayer(){}
public:
    CREATE_FUNC(CFLayer);
    
    virtual bool init() override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
public:
    /*background layer*/
    Layer*       m_pBgLayer;
    /*content layer*/
    Layer*       m_pContentLayer;
    /*UI layer*/
    Layer*       m_pUILayer;
public:
    void addToBgLayer(Node*,int zorder = 0,int tag = 0);
    void addToContentLayer(Node*,int zorder = 0,int tag = 0);
    void addToUILayer(Node*,int zorder = 0,int tag = 0);
    
public:
    /**
     *  this method only valid in Android
     *
     *  @return
     */
    virtual bool onBackKeyDown();
    
    virtual void setBackFunc(std::function<bool()> func)
    { m_fnBackFunc = func;}
protected:
    std::function<bool()>   m_fnBackFunc;
    
public:
//    void addLockScreenLayer();
//    
//    LockScreenLayer* m_pLockLayer;
//    
//    void removeLockScreenLayer();
    
   
};

#endif /* defined(__KidsFramework_CocosV3_2__CFLayer__) */
