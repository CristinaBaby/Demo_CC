//
//  MakeIceCreamStep02.hpp
//  IceCream
//
//  Created by huxixiang on 16/5/19.
//
//

#ifndef MakeIceCreamStep02_hpp
#define MakeIceCreamStep02_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

USING_NS_CC;
class MakeIceCreamStep02 : public BaseLayer, public ToolSpriteDelegate{
public:
    MakeIceCreamStep02();
    ~MakeIceCreamStep02();
    
    static Scene* scene();
    CREATE_FUNC(MakeIceCreamStep02);
    virtual bool init() override;
    
protected:
    bool initData();
    bool initLayer();
    
    void installAgitator1();
    void installAgitator2();
    void addIceCake();
    void createIceCake(float ft);
    
    void addCoverEdge(bool isShow);
    void addHandle();
    
    void makeIcecreamTime(float ft);
    // 添加水滴
    void addDrip();
    void finishMix();
    
    void addProgress();
    void runHandTipAnimation(int step);
    
    virtual void onNextCallback() override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    virtual void onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index) override;
    virtual void onTouchUpOutRect(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onMoveBackCallback(ToolSprite* toolSprite) override;
    
private:
    Node* m_pBg;
    Layer* m_pTable;
    // 搅拌机的所有零件位于该层，方便移动
    Layer* m_pMovableLayer;
    
    // 搅拌机外面的木桶，前后两层
    Sprite* m_pBarrel;
    Sprite* m_pBarrel_up;
    Sprite* m_pBarrel_up_temp;
    Sprite* m_pRing;
    
    // 搅拌器中间装冰淇淋的容器，前后两层
    Sprite* m_pAgitator_down;
    Sprite* m_pAgitator_up;
    
    // 搅拌器的转子
    Sprite* m_pRotor;
    
    // 碗里的冰淇淋
    Sprite* m_pIcecream;
    
    Vec2 m_vSaveMoveLayerPos;
    bool isMoving;
    int m_iStep;
    
    // 碗的响应区域
    Node* m_pResponseNode;
    
    // 生成的冰块计数
    int m_iIceCakeCount;
    
    // 搅拌机中间容器的盖子
    Sprite* m_pCover;
    Sprite* m_pCover_on;
    
    // 搅拌机的盖子
    Sprite* m_pLid;
    
    // 摇杆
    Armature* m_pHandle;
    
    // 标示摇杆动画持续的时间
    int m_iTime;
    
    EventListenerPhysicsContact* m_pListener;
    int m_iIceCreamSoundId;
    
    // 背景上的雪花
    ParticleSystemQuad* m_pSnowParticle;
    
    // 搅拌机搅拌的音效
    int m_iMachineMixSoundId;
    
    // 进度条
    Sprite* m_pProgressBg;
    ProgressTimer* m_pProgressTimer;
    Sprite* m_pFlag;
    
    ToolSprite* m_pHandTip;
};

#endif /* MakeIceCreamStep02_hpp */
