
#ifndef __GameUILayoutLayer__
#define __GameUILayoutLayer__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SSCMoreGameButton.h"

USING_NS_CC;
using namespace cocos2d::ui;


class GameUILayoutLayer : public Node
{
public:
    enum
    {
        eUIButtonTagHomeStart,
        eUIButtonTagHomeRestore,
        eUIButtonTagHomeMore,
        eUIButtonTagHomeFav,
        eUIButtonTagHomeShop,
        eUIButtonTagHomeRate,
        eUIButtonTagPolicy,
        eUIButtonTagBack,
        eUIButtonTagNext,
        eUIButtonTagReset,
        eUIButtonTagFav,
        eUIButtonTagPhoto,
        eUIButtonTagEmail,
        eUIButtonTagMap,
        eUIButtonTagBg,
        eUIButtonTagHome,
        eUIButtonTagEatAgain,
        eUIButtonTagEat,
        eUIButtonTagSale,
        eUIButtonTagMoreGame,
        eUIButtonTag,
        eUILabelTagCoint,
        eUISpriteTagCoint,
    };
    GameUILayoutLayer();
    ~GameUILayoutLayer();
    
    CREATE_FUNC(GameUILayoutLayer);
    
    virtual bool init();
    
    void showHomeUILayout(bool animate = true);
    void showMakeUILayout(bool animate = true);
    void showDecorateUILayout(bool animate = true);
    void showShareUILayout(bool animate = true);
    
    void showNextLayout(bool animate = true);
    void showBackLayout(bool animate = true);
    void showResetLayout(bool animate = true);
    void showEatLayout(bool animate = true);
    void showSaleLayout(bool animate = true);
    
    void showNormalLayout(bool animate = true);
    void showSSCMore();
    
    void showCointLayout(bool animate = true);
    void updateCoint();
    
    void showEatAgainLayout(bool isDrink = false,bool animate = true);
    
    void hideNext(){
        if (!m_pNext) {
            return;
        }
        m_pNext->setVisible(false);
        m_pNext->setEnabled(false);
    };
    
    void hideReset(){
        if (!m_pReset) {
            return;
        }
        m_pReset->setVisible(false);
        m_pReset->setEnabled(false);
    };
    void hideBack(){
        if (!m_pBack) {
            return;
        }
        m_pBack->setVisible(false);
        m_pBack->setEnabled(false);
    };
    
    Button* getButton(int tag){
        Button* btn = (Button*)this->getChildByTag(tag);
        return btn;
    };
    bool eatAgainIsShow(){
        return !m_pEatAgain?false:m_pEatAgain->isVisible();
    }
    std::function<void(Button*)>onButtonClicked;
protected:
    
    Button* _createButton(std::string path);
    void _onButtonCallback(Ref* ref,Widget::TouchEventType type);
    void _showHomeButtonAnimation(Button* pButton,float duration = 1,float delay = 0);
    
    //    首页
    Button* m_pHomeStart;
    Button* m_pHomeRestore;
    //    Button* m_pHomeMore;
    Button* m_pHomeFav;
    Button* m_pHomeShop;
    Button* m_pHomeRate;
    Button* m_pHomePolicy;
    
    //    Make
    Button* m_pNext;
    Button* m_pBack;
    Button* m_pReset;
    Button* m_pEat;
    Button* m_pSale;
    Button* m_pMore;
    SSCMoreGameButton* m_pSSCMore;
    
    //    share
    Button* m_pFav;
//    Button* m_pEmail;
    Button* m_pPhoto;
    Button* m_pHome;
    Button* m_pMap;
    Button* m_pBg;
    Button* m_pEatAgain;
    
    Label* m_pCointLabel;
    Sprite* m_pCointBg;
    float m_nCoint;
};

#endif