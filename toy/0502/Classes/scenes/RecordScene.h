//
//  RecordScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"
#include "cocostudio/Cocostudio.h"

class TransitionFadeEx;

enum MENU_STATE{
    M_SHOW = 0,
    M_HIDE = 1,
    M_MOVE = 2,
};

class RecordScene : public Layer
{
public:
    static Scene* scene();
    static TransitionFadeEx* createScene();
    virtual bool init(bool isForward);
    static RecordScene* create(bool isForward);
    
protected:
    virtual void onExit() override;
    
public:
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        
        DOWN_LEFT,
        UP_LEFT,
        UP_RIGHT,
        DOWN_RIGHT,
    };
    
    enum class Period
    {
        REST,
        MOVE_STENCIL,
        MOVE_ARMATURE,
    };
    
    enum class Guide
    {
        NONE,
        MENU_WAIT,
        RECORD_WAIT,
    };
    
    
private:
    void onButton(Ref* sender, Widget::TouchEventType type);
    void onCheckBoxClicked(Ref* ,CheckBox::EventType type);
    void initMenu(float dt);
    void hideMenu();
    void showMenu();
    string getCurrentImageName();
    string getCurrentRecordName();
    string getCurrentPageName();
    void createFishes(Vec2& pos, Vec2& moveDistance);
    void disappear(float dt);
    void disableAllButtons();
    void enableAllButtons(float dt);
    void startRecord(float dt);
    void checkStopFinish(float dt);
    void onFinishStop(float dt);
    
    //场景动画
    void onEnterTransitionDidFinish();
    bool        _isForward;
    
    Layout*     _root;
    
    //录音相关
    void updateRecord(float delta);
    int         _time;
    Text*       _textTime;
    unsigned int _effectid = 0;
    bool        _bBuffer = false;
    
    int _pictureIndex = 0;
    ImageView * _image;
    
    void switchPicture(bool direction);
    void moveContent();
    void moveFinish();
    
    Sprite * _stencil;
    
    Layout * _layoutUI02;
    
    ClippingNode * _parentClipper = nullptr;
    ClippingNode * _newClipper = nullptr;
    ClippingNode * _underClipper = nullptr;
    Sprite *    _underSprite;
    
    Sprite *    _underContent;                       //分, under, over(parent, child)
    Sprite *    _childStencil;

    //菜单按钮
    void        unblockMenu(float dt);
    bool        _block = false;
    void        stopMenuActions();
    Button *    _btnHome;
    CheckBox *  _chkSound;
    Button *    _btnRecord;
    Button *    _btnRepeat;
    Button *    _btnStop;
    Button *    _btnClose;
    Button *    _btnMenu;
    vector<Vec2> _vecPos;
    Vector<Widget*> _vecMenus;
    Vec2        _hidePos;
    int         _menuState = M_SHOW;
    
    Layout *    _popup;
    Layout *    _popupRecord;
    
    //文字相关
    void setStoryText();
    Text *      _pictureLabel;
    
    //动画相关
    void unblockMove();
    void rotateWithAngle(Vec2& pos, float fDegree);
    Period      _period;
    Vec2        _endPos;
    cocostudio::Armature*      _fishes = nullptr;
    Direction   _direction;                 //随机产生一个Direction
    Direction   _directionOld;
    
    
    bool        _moveBlock  = false;
    
    Guide       _guideMode = Guide::NONE;
    
    //广告相关
    int         _moveCount = 0;
    int         _rateCount = 0;
    
    bool        _isFinishStop = true;
    bool        _bStartRecord = false;
};

