//
//  PopFruitScene.hpp
//  test-cocos2dx
//
//  Created by wusonglin on 15/12/12.
//
//

#ifndef PopFruitScene_hpp
#define PopFruitScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "FruitSprite.h"
#include "FlurryEventManager.h"

USING_NS_CC;
using namespace std;
class PopFruitScene : public cocos2d::Layer{
public:
    PopFruitScene();
    ~PopFruitScene();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PopFruitScene);
private:
    //初始化
    void initFruit();
    void initView();
    void update(float dt);
    void initFreamFruit();
    
    void createL();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    void updateResponse(float dt);
private:
    Vec2 getPosByRowCol(int row,int col);
    FruitSprite* getFruitByTouch(Vec2 pos);
    void genSelectedList(FruitSprite* f);
    
    void _genSelectedList(FruitSprite* f);
    void deleteSelectedList();
    void adjustFruitPos();
    bool isOver();
    void clearFruitOneByOne();
    bool checkFruitIsAdjust();
    PointArray* doPointArray(Vec2 startPos,Vec2 endPos,int detal);
    void showTitle();
    void showMagicFont(int dt);
    void showPointTouch();
    void showParticle(float dt);
    void showHand(float dt);
public:
    const static int FRUIT_ROW_NUM = 8;
    const static int FRUIT_COL_NUM = 5;
private:
    LayerColor* m_pFream;
    Sprite* m_pFruitFream;
    FruitSprite* m_pFruits[FRUIT_ROW_NUM][FRUIT_COL_NUM];
    //队列,存放需要消除的水果
    deque<FruitSprite*> selectedList;
    
    deque<FruitSprite*> _selectedList;
    bool needClear;
    static float ONE_CLEAR_TIME;
    float clearSumTime;
    bool m_bIsEnable;
    int  m_iStep;
    Sprite* m_pTitle;
    bool m_bIsOver;
    int m_iIsNoResponse;
    int m_bIsStartTime;
    Sprite* m_pHand;
    Vector<ProgressTimer*> m_vTime;
    bool m_bIsStart;
};
#endif /* PopFruitScene_hpp */
