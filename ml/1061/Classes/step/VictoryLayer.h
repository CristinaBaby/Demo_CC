//
//  VictoryLayer.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/14.
//
//

#ifndef VictoryLayer_hpp
#define VictoryLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "MySpriteButton.h"

USING_NS_CC;
using namespace std;

enum VictoryType{
    kVictoryFruit         =       0 ,
    kVictoryGummy         ,
    kVictoryPot           ,
    
};

class VictoryLayer : public LayerColor {
    VictoryLayer();
    ~VictoryLayer();
    
public:
    static VictoryLayer* create(int type,string moldPath = "");
    virtual bool init(int type,string moldPath = "");
    
    std::function<void()> btnNext = nullptr;
    std::function<void()> btnAgain = nullptr;
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    void btnNextClick(Ref* pSender);
    void btnAgainClick(Ref* pSender);
    void showTitleBg(float dt);
    void showVictory(float dt);
    void showGummy(float dt);
    void showStarFly(float dt);
    void showBtn(float dt);
    void showContent(float dt);
    void showlight(float dt);
    void showStarNum(float dt);
    void showBear(float dt);
    void _showBear(float dt);
private:
    int m_iType ;
    Sprite* m_pTitleBg;
    MySpriteButton* m_pBtnNext;
    MySpriteButton* m_pBtnAgain;
    Vector<Sprite*> m_vStar;
    Vector<Sprite*> m_vStarFly;
    Sprite* m_pContent;
    Sprite* m_pVictory;
    Sprite* m_pGummys;
    Sprite* m_pLight;
    string  m_sMoldPath;
    vector<int> m_vIndex;
    Sprite* m_pBearSuccess;
};



#endif /* VictoryLayer_hpp */
