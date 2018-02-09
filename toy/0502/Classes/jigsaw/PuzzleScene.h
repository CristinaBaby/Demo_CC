//
//  PuzzleScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"
#include "AppConfig.h"

class TileSprite;
class JPSideBar;

class PuzzleScene : public Layer
{
public:
    static Scene* scene();
    virtual bool init();
    static PuzzleScene* create();
    
    void checkCommbine(TileSprite *sprite);
    bool isCoundInsertMap(int index);
    void lock_deal(TileSprite *sprite ,float frameDelayX, float frameDelayY);
    bool checkSingleSpriteCombine(TileSprite *sprite);
    void moveGroupSprite(Vec2 delayPointDelay, int groupIndex , int m_index);
    void removeGroupMap(int groupIndex);
    void removeCurHuitanGroupMap(int groupIndex);
    void setGroupTouchPriority(int groupIndex);
    void groupToTop(int groupIndex);
    void runMoveAction();
    void runRotateAction(float dt);
    bool checkIsSuccess();
    void victory(float dt);
    void removeBatchNode();
    void pauseGame();
    void resumeGame();
    void recycleRes();
    int  getRandomFromRange(int a , int b , int index);
    static bool SortByM1( TileSprite *v1, TileSprite *v2);
    void saveUserResumeData(int time);
    void saveUserVictoryData();
    void resumeUserData();
    void checkGroupBorder(int groupIndex , int m_index);
    int getGroupMaxY(int groupIndex);
    bool isInGameItem(int tag);
    
    bool checkIsLock(TileSprite * tile);
    
    
    
    void timing(float delta);
    
protected:
    virtual void onExit() override;
    
private:
    void onDialog(const string& name);
    void onButton(Ref* sender, Widget::TouchEventType type);
    void prepareTiles();
    
private:
    Layout * _root;
    float _compare_distance = GAME_SCALE*45 * 1.2;             //加了0.25的加成
    
    int cur_pic_index = 1;                  //使用的图片index, 从第1张开始
    int _sideLen = 2;                        //仅测试用
    int row ;
    int column;
    
    bool isVicotry = false;
    
    vector<TileSprite*> sprites;
    
    JPSideBar * _scroll;
    ImageView * _frame;
    
    //Group 相关
    vector< vector<TileSprite*> > groupArray;
    map<int, int>  curMoveGroupMap;
    map<int, int>  curHuitanGroupMap;
    
    map<int, int>   map_lock_tile;          //已经连在一起或者说是一个Group的tile
    
    //时间相关
    Text *  _txtTime;
    int _seconds = 0;
};

