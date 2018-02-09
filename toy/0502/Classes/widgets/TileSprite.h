//
//  TileSprite.h
//  cocosTest
//
//  Created by zhangguangzong1 on 1/27/14.
//
//

#ifndef __cocosTest__TileSprite__
#define __cocosTest__TileSprite__

#include <iostream>
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

#define LOCK_STATUS  999    //固定在界面了
static int www = 2;
static int mmm = 0;

class PuzzleScene;
class JPSideBar;

//可以实现和其它瓦片组合  记录
class TileSprite : public Sprite {
    
public:
    TileSprite();
    CCActionInterval* action;
    bool isTouch;
    int isInMap;//是否在游戏中， 默认是在滑动条上0
    private :
    float mGroupIndex;
   
    int m_index;

    
    ~TileSprite();
public:
    static TileSprite* create();
    static TileSprite* create(const char* fileName);
    static TileSprite* create(CCTexture2D* texture);
    static TileSprite* create(CCTexture2D* texture , Rect rect);
public:
    
    virtual bool init();
    
    virtual void onEnter();
	virtual void onExit();
    virtual void setRotation(float fRotation);
    virtual void setPosition(const Vec2& pos);
    
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch , Event *event);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    virtual void rotateActionOver();
    
    virtual void combination();
    
    virtual Vec2 getLeftTopPoint();
    
    void setOriginalPoint(Vec2 point);
    
    void supportRotate(bool isSupport);
    
    void setGroupIndex(int groupIndex);
    
    void delayOpenTouch();
    
    int getGroupIndex();
    //锁住精灵  用户不可操作
    void lockSprite();
    
    bool isLock();
    
    int get_m_Index();
    
    void set_m_Index(int  index);  //瓦片地图的序号排列
    
    void lockAction(float dt);
    
    Vec2 getOriginalPoint();
    
    Vec2 getCombinePoint(); // use to combine original point's distance , it's no matter with anchpoint
    
    void recoverOriginalPostion(); //resume original position
    
    void recoverCenterAnchpoint();
    
    void goTop();
    
    void setMyPriority();
    
     void goMainCallSetPriority(); // 设置touch 优先级
    
    void kickBackAction(Vec2 distance);  //回弹动画
    
    void openTouchEvent(int type);//两种方式   拖动开启  和  resume模式开启   区别在于是否做边界检测
    
    void setSideBarBorder(float sidebar, int sideLen);
    void setJPSideBar(JPSideBar * sidebar);
    void setStartScale(float startScale);
    
    bool isInRightPlace();

public:
    Vec2 m_touchPoint;
public:
    /*拼图块类型*/
    int mType;
    /*拼图块是否被放置*/
    bool mIsPlace;
    //是否支持旋转
    bool isSurpportRotate;

//    std::vector<TileSprite> combineList;
    //是否为正方形的四个角
    bool isInAngel(int row , int column);
    
    std::vector<int> moveSprite;
    
    PuzzleScene *_puzzleScene;
    
private :
    //在地图中的正确位置  当移动到指定位置  可以和该值作比较  如果偏差很小  表示位置正确， 但旁边必须得有别的图形连接才行，否则也不算放置正确。
    float originalX;
    float originalY;
    float _sidebar;
    float _startScale;
    JPSideBar * _bar;
    
    EventListenerTouchOneByOne * _listener;
    
    Vec2        _startPos;
    
    Size        _parentSize;
    int         _sideLen;
    int         _compare = 0;
};

#endif /* defined(__cocosTest__TileSprite__) */
