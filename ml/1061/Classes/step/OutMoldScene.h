//
//  OutMoldScene.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/12.
//
//

#ifndef OutMoldScene_hpp
#define OutMoldScene_hpp

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "MySpriteButton.h"
#include "UnitScrollView.h"
using namespace ui;
USING_NS_CC;

//步骤的基本模板
class OutMoldScene:public GameBaseScene{
private:
    OutMoldScene();
    ~OutMoldScene();
public:
    virtual bool init();
    CREATE_FUNC(OutMoldScene);
    CREATE_SCENE(OutMoldScene);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();

    void initUI();
    void nextStep();
    
    void clickBtn(Ref* pSender);
    void showDecUIBtn();
    void onUnitCallback(int tag,MenuItem* menuItem,int index);
    Texture2D* addSugar(int index);
    Texture2D* addPhoto(Sprite* sp);
    
    bool onTouchLayer(Touch *pTouch, Event *pEvent);
    void resetBtn(Ref* pSender);
    void nextBtn(Ref* pSender);
    void shareBtn(Ref* pSender);
    void moldClickBtn(Ref* pSender);
    void onNegativeClick(void* type);
    void onPositiveClick(void* type);
    
    void addFavoriteCallback(bool isSuc);
    
    void photoClick(Ref* pSender);
    void emailClick(Ref* pSender);
    void closeClick(Ref* pSender);
    void emailOpen(float dt);
    void showPhotoBg();
    void hidePhotoBg();
    void showHandClickMold(float dt);
    void showHandAddToppings(float dt);
    
    void showFont(float dt);
private:
    Sprite* m_pBG;
    Sprite* m_pHand;
    Sprite* m_pArrow;
    MySpriteButton* m_pMoldShadow;
    Vector<MySpriteButton*> m_vDecBtn;
    Sprite* m_pDecBtnBg;
    UnitScrollView* m_pUnitScrollView;
    Sprite* m_pUnitScrollViewBg;
    int m_pIndex;
    Layer* m_pDecoritionLayer;
    Layer* m_pDecoritionToppingLayer;
    bool m_bIsTouchToppings;
    bool m_bIsTouchSugar;
    int m_iToppingsIndex;
    int m_iSugarIndex;
    MySpriteButton* m_pCurrentSprite;
    MySpriteButton* m_pReset;
    MySpriteButton* m_pNext;
    MySpriteButton* m_pShare;
    MySpriteButton* m_pPhoto;
    MySpriteButton* m_pEmail;
    MySpriteButton* m_pClose;
    Sprite* m_pItemBg;
    Sprite* m_pSugar;
    Sprite* m_pPhotoBg;
    LayerColor* m_pLayerColor;
    RenderTexture* m_pRenderTexture;
    string m_sPath;
    int m_iMoldClickNum;
    bool m_bIsShowBtn;
    Sprite* m_pMold;
    bool m_bIsFirst;
    bool m_bTouchBg;
    ClippingNode* m_pClippingNode;
};

#endif /* OutMoldScene_hpp */
