//
//  GameScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "BaseMode.h"
#include "LoadingLayer.h"
#include "ArmatureManager.h"
#include "FillNode.h"


class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;
class Palette;

class  GameScene : public Scene
{
public :
    struct Paint
    {
        Vec2    pos;
        Vec2    previousPos;
    };
    
    enum class FillType{
        eColoring,                  //填色
        eScribble                   //涂色
    };
    
    static GameScene* create(int packIndex, int pictureIndex);
    virtual bool init(int packIndex, int pictureIndex);
    ~GameScene();
    
    void onProgress(int percent);
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void setupScribble();
    
    Layer * getColouringLayer();
    void switchMode(BaseMode::Mode mode);
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void stopPaint();
    
    /* register touches event */
    virtual void onTouchesBegan(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    
    //关于保存
    void generateCurrentPaintingPicture();
    void save();
    ActionInterval* createLoadingAction(int num);
    void MoveToOffset(Widget* current, bool offset);
    void updateColor(float dt);
    void updateFillModeColor(float dt);
    void onAfterVisit(EventCustom * event);
    
    //解锁广告
    void craeteUnlockAds(Ref* pSender, Widget::TouchEventType type);
    void updatePensPack(int group);
    void networkChanged();
    void networkChangedAciton(bool state);
    bool getNetWorkState();
    float getScaleAntialias();
    void closeAds();
    void onDialog(const string& name);
    void onEventFillnode(EventCustom* event);
    
private:
    string      _picturePath;
    string      _pictureHistoryPath;
    Layer *     _colouringLayer = nullptr;
    
    int         _packIndex;
    int         _pictureIndex;
    
    Image     * _pictureImage = nullptr;
    Layout    * _root;
    
    CC_SYNTHESIZE(bool, _show, pictureShow); // 真 vedeo 假 RMB
    
public: //状态机模式
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);
    
private:
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    
    Size                _winSize;
    bool                _haveDraw = false;
    
    FillNode*           _pictureScribble = nullptr;
    LoadingLayer*       _loadingLayer;
    FillType            _fillType = FillType::eColoring;
    
    int                 _pensIndex = 0;
    ListView*           _pensList;
    ImageView*          _pencil;
    Button *            _fillTypeBtn;
    Button *            _undoBtn;
    Widget*             _currentBrush;
    Size                _drawSize;
    Point               _originPoint;
    bool                _useRainbowPen = false;
    bool                _init = true;
    EventListenerCustom* _event;
    bool                _skip = true;
    int                 _lockIndex;
    
    bool                _isRewardAd;
    
    vector<Button*>     _chooseBrushes;
    float               _magicScale;
    int                 _finishCount = 0;
};