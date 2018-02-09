//
//  PourMoldLayer.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/22.
//
//

#ifndef PourMoldLayer_hpp
#define PourMoldLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MySpriteButton.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "AdapterScreen.h"
#include "ActionUtils.h"
#include "ToolSprite.h"
#include "Global.h"
#include "MySpriteButton.h"
#include "MoldMacLayer.h"

USING_NS_CC;
using namespace std;

/*
 *  倾倒mold的Layer
 */

class PourMoldLayer : public Layer,public ToolSpriteDelegate,public KDGridViewScrollCallback{
    
public:
    PourMoldLayer();
    ~PourMoldLayer();
    
    virtual bool init();
    CREATE_FUNC(PourMoldLayer);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    void initData();
    void initView();
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void showMold();
    
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
   
    void updateMold(float dt);
    void nextBtn(Ref* pSender);
    
    void pushBtnDown(int index);
    void pushBtnUp();
public:
    void showPourMac();
    void startPour(int i,Vec2 pos);
    void showParticle(string path);
public:
    //选中之后的回掉
    std::function<void()> selectMoldHideCallback = nullptr;
private:
    Vector<Sprite*>  m_vType;
    Vector<ToolSprite*>  m_vBtn;
//    KDGridView*      m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
//    bool isStartPour;
    Vec2 itemPos;
//    Sprite* m_pMold;
//    int m_iMoldType;
    float m_fMoldPercentage;
    int m_iMoldTag;
    MySpriteButton*  m_pNextBtn;
    MoldMacLayer* m_pMoldMacLayer;
    bool m_bStartPour;
};
#endif /* PourMoldLayer_hpp */
