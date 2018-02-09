//
//  KSPopLayer.h
//  CrazyCalculate
//
//  Created by tangbowen on 15-1-8.
//
//

#ifndef __CrazyCalculate__KSPopLayer__
#define __CrazyCalculate__KSPopLayer__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class KSPopLayer : public LayerColor {
    
public:
    KSPopLayer();
    virtual ~KSPopLayer();
    
public:
    static KSPopLayer* create(Color4B color);
    virtual bool init(Color4B color);
    
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
public:
    enum class TouchEventType
    {
        BEGAN,
        MOVED,
        ENDED,
        CANCELED,
        REMOVE
    };
    typedef function<void(KSPopLayer*, TouchEventType)> KSPopLayerCallBack;
    typedef function<void(KSPopLayer*, TouchEventType, Touch*, Event*)> KSPopLayerMoveCallBack;
public:
    Size                m_sVisibleSize;
    KSPopLayerCallBack  m_fnCallBack;
    KSPopLayerMoveCallBack  m_fnMoveCallBack;
    bool m_bIsDialog;
    
public:
    bool isAncestorsVisible(Node* node);
    inline void setListener(KSPopLayerCallBack function){
        m_fnCallBack = function;
    }
    inline void setMoveListener(KSPopLayerMoveCallBack function){
        m_fnMoveCallBack = function;
    }
    inline void setIsDialog(bool b){
        m_bIsDialog = b;
    }
    
    
};

#endif /* defined(__CrazyCalculate__KSPopLayer__) */
