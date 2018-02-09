//
//  HomeLayer.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/18.
//
//

#ifndef HomeLayer_hpp
#define HomeLayer_hpp

#include "cocos2d.h"
#include "CocosHelper.h"
#include "MySpriteButton.h"
#include "BaseHomeLayer.h"

USING_NS_CC;
class HomeLayer:public BaseHomeLayer{
       DECLARE_DYNCLASS(HomeLayer);
public:
    CREATE_SECENE(HomeLayer);
protected:
    bool init();
    virtual  void onEnterTransitionDidFinish();
    virtual  void onEnter();
    virtual  void onExit();
    void showAdsCallBack(){}
private:
    void initView();
    void initUI();
    void clickBtn(Ref* pSender);
    
    void showTable();
    void showUIBar();
    void showLogo();
    void showUI();
private:
    Sprite* m_pBg;
    Sprite* m_pTable;
    Sprite* m_pCake;
    Sprite* m_pCookies;
    Sprite* m_pFruitTart;
    
    MySpriteButton* m_pBtnPlay;
    MySpriteButton* m_pBtnShop;
    MySpriteButton* m_pBtnMore;
    
    Sprite* m_pUiBar;
    Sprite* m_pLogo;
    
};
#endif /* HomeLayer_hpp */
