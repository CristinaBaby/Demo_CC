//
//  CFLayer.h
//  CodeBase
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#ifndef CODEBASE_COMPONENT_LAYEREXT_CFLAYER_H_
#define CODEBASE_COMPONENT_LAYEREXT_CFLAYER_H_
#include <iostream>
#include "cocos2d.h"
#include "depends/component/layerext/KeypadDispatchCenter.h"

USING_NS_CC;

typedef enum
{
    kBackground         =       0,
    kContent            =       50,
    kUI                 =       100,
    kPop                =       200,
}LayerZOrder;


class CFLayer : public Layer,public KeypadDelegate
{
public:
    CREATE_FUNC(CFLayer);
    
    virtual bool init() override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    /**
     *  this method only valid in Android
     *
     *  @return
     */
    virtual bool onBackKeyDown() override;
    
    void addToBgLayer(Node*,int zorder = 0,int tag = 0);
    void addToContentLayer(Node*,int zorder = 0,int tag = 0);
    void addToUILayer(Node*,int zorder = 0,int tag = 0);
    
    /*暂时把方法放这,以后要换个位置*/
    bool isNodeVisiable(Node*);

    
    virtual void setBackFunc(std::function<bool()> func)
    { _fnBackFunc = func;}
    
protected:
    CFLayer(){}
    virtual ~CFLayer(){}
    
    
    /*background layer*/
    Layer*       _bgLayer;
    /*content layer*/
    Layer*       _contentLayer;
    /*UI layer*/
    Layer*       _uiLayer;
    
    std::function<bool()>   _fnBackFunc;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(CFLayer);
};

#endif /* defined(CODEBASE_COMPONENT_LAYEREXT_CFLAYER_H_) */
