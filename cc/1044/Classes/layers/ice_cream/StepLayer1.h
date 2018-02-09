//
//  StepLayer1.h
//  CookiesMaker
//
//  Created by wangbosheng1 on 14-11-24.
//
//

#ifndef __CookiesMaker__StepLayer1__
#define __CookiesMaker__StepLayer1__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;


enum ToolType
{
    KSALT,
    KYEAST,
    KSUGAR,
    KWATER,
    KSPOON,
    KCHOCOLATECHIPS,
    KBAKE,
    KFLOUR,
};
class StepLayer1: public StepLayer,public ToolSpriteDelegate
{
public:
    StepLayer1(GameLayer* gameLayer);
    StepLayer1();
    ~StepLayer1();
    static StepLayer1* create(GameLayer* gameLayer);
    
private:
    virtual bool init();
    bool initUI();
 
public:
    void onTouchCancel(ToolSprite* toolSprite,Touch *pTouch){};
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    void onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch);
public:
    //各个材料进入动画
    void MaterialComing1();
    void MaterialComing2();
    void MaterialComing3();
    void MaterialComing4();
    
    //步骤控制器
    void stepControl(float dt);
    
    virtual void onEnter();
    virtual void onExit();

public:
    ToolSprite*  m_pSalt;
    ToolSprite*  m_pYeast;
    ToolSprite*  m_pSugar;
    ToolSprite*  m_pWater;
    ToolSprite*  m_pEgg1;
    ToolSprite*  m_pEgg2;
    ToolSprite*  m_pEgg3;
    ToolSprite*  m_pSpoon;
    ToolSprite*  m_pChocoChips;
    ToolSprite*  m_pBake;
    ToolSprite*  m_pFlour;
    
    Sprite*  m_pBakePlate;
    Sprite*  m_pPlate;
    //盘子里的内容
    Sprite*  m_pPlateContent;
   
    
   
public:
    Vec2 oldPosition;
    
    //记录操作数量
    int  count;
    bool m_pIsTouch;
    Vector<ToolSprite*>    m_vToolSprite;
    int m_iRote;
    bool m_bIsSounds;
    
    
    
};
#endif /* defined(__CookiesMaker__StepLayer1__) */
