//
//  EatLayer.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/8.
//
//

#ifndef EatLayer_hpp
#define EatLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "BaseLayer.h"
#include "Utils.h"
#include "SceneManager.h"
#include "KDGridView.h"
#include "KSDecorationAdapter.h"
#include "UnitScrollView.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "Global.h"
#include "extensions/cocos-ext.h"
#include "ActionUtils.h"
#include "WriteTouchNode.h"
#include "FilePathUtil.h"
#include "MySpriteButton.h"
#include "FlurryEventManager.h"

using namespace cocos2d::extension;

USING_NS_CC;
using namespace std;

class EatLayer : public Layer,public DialogCallback{

private:
    EatLayer();
    ~EatLayer();
public:
    static EatLayer* create();
    static Scene* createScene();
    virtual bool init();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
private:
    void initView();
    void initData();
    void initMold(float dt);
    
    void updateStart(float dt);
    void update(float dt);
    
    void moldMove(float dt);
    void glassAction(float dt);
    void moldAction(float dt);
    void clickMold(Ref* sender);
    void clickEatAgain(Ref* sender);
    void showEatAgain(float dt);
    void showParticle(float dt);
    void readAction(float dt);
    void goAction(float dt);
    void returnHome(Ref* sender);
    void nextGame(Ref* sender);
    
    void onNegativeClick(void* type);
    void onPositiveClick(void* type);
    
    void showTapToEat(float dt);
private:
    Sprite* m_pBg;
    vector<string> m_vMoldPath;
    vector<int> m_vMoldIndex;
    Sprite* m_pGlass;
    Sprite* m_pGlassCover;
    int m_iMoldInit;
    Vector<Sprite*> m_vMoldGlass;
    Vector<MySpriteButton*> m_vMoldTouch;
    MySpriteButton* m_pEatAgain;
    MySpriteButton* m_pReturnHome;
    MySpriteButton* m_pNextGame;
    bool m_bIsFirst;
    Sprite* m_pEat;
    int m_iEatNum;
    bool m_bTapEatHide;
};

#endif /* EatLayer_hpp */
