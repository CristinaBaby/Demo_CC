//
//  RewardLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/15.
//
//

#ifndef RewardLayer_hpp
#define RewardLayer_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "KSPopLayer.h"
enum RewardType{

    typeMoldAll = 0,
    typeDec,
    typeMoldHeart,
    typeRolling,
    typeScoop,
    typeMix,
    typeNewFlavor,
    typeAllFlavor

};
USING_NS_CC;

using namespace std;
using namespace ui;

class RewardLayer : public KSPopLayer  {
public:
    RewardLayer();
    ~RewardLayer();
public:
    bool init(RewardType type);
//    CREATE_FUNC(RewardLayer);
    static RewardLayer* create(RewardType type);
//    CREATE_SCENE(RewardLayer);
    
    static void showRewardLayer(RewardType type,std::function<void()> btnOkClick = nullptr,std::function<void()> btnNextClick = nullptr,std::function<void()> btnEatClick = nullptr);
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void showGiftBox(float dt);
    void showGift(float dt);
    void showBtn(float dt);
    void showAnumation(float dt);
private:
    Sprite* m_pGiftBox;
    Sprite* m_pGiftBox1;
    Sprite* m_pGiftBox2;
    Sprite* m_pRewards;
    Sprite* m_pLight1;
    Sprite* m_pLight2;
    Sprite* m_pGift;
    Sprite* m_pBg;

    int m_iTouchNum;
    Vec2 _pos;
    
    RewardType _type;
    
    StudioLayer* _studioLayer;
    
    Sprite* m_pAnumation;
    bool m_bIsFirst;
    bool m_bIsTouch;
    
    LayerColor* _layerColor;
public:
    std::function<void()> btnOkClick   = nullptr;
    std::function<void()> btnNextClick = nullptr;
    std::function<void()> btnEatClick  = nullptr;
};


#endif /* RewardLayer_hpp */
