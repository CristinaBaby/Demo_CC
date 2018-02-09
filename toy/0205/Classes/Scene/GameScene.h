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
#include "../Model/v2.0/ScribbleLineFrameNode.h"
#include "../Model/v2.0/ScribblePatternNode.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "BaseMode.h"
#include "LoadingLayer.h"
#include "ArmatureManager.h"


class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;
class Palette;

class  GameScene : public Scene, public ScribbleLineFrameLoadDelegate
{
public :
    struct Paint
    {
        Vec2    pos;
        Vec2    previousPos;
    };
    
    enum colorOffset{
        ERASER = 1,
        RAINBOW_PEN,
        GOLD_PEN,
        SPARY_GUN,
        PEN,
        WRTINGBRUSH,
        PENCIL,
        CRAYON
    };
    
    static GameScene* create(int packIndex, int pictureIndex);
    virtual bool init(int packIndex, int pictureIndex);
    ~GameScene();
    
    void onProgress(int percent);
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onColorPen(Ref* pSender, Widget::TouchEventType type);
    void setupScribble();
    void initAnimation();
    
    Layer * getColouringLayer();
    void switchMode(BaseMode::Mode mode);
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void paintStamp(Vec2 & current);
    void switchAnimation(float dt);
    void onMovementEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const string& movementID);
    void onMovementToStatic(Armature *armature, cocostudio::MovementEventType movementType);
    void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
    void updateColor(float dt);
    void stopPaint();
    
    /* register touches event */
    virtual void onTouchesBegan(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, cocos2d::Event *unused_event);
    
    //关于保存
    void generateCurrentPaintingPicture();
    void save();
    virtual void loadPercent(Node *pNode, float pPercent); //Pattern加载进度0.0~1.0
    virtual void loadEnded(Node *pNode);
    
    ActionInterval* createLoadingAction(int num);
    void makeColor(float dt);
    void resumePensList();
    void pushPensHandsOn(float dt);
    void backToOffset();
    void popupMenu();
    
private:
    void createColorsList(int index);
    void indent(ListView * list, bool isBefore);
    
    ImageView * _palette;
    Vec2        _beganLocation;
    Button    * _refresh;
    Button    * _saveBtn;
    ImageView * _center;
    
    Layer     * _saveLayer = nullptr;
    
    string      _picturePath;
    string      _pictureHistoryPath;
    Layer *     _colouringLayer = nullptr;
    Layer *     _cliperLayer = nullptr;
    
    int         _packIndex;
    int         _pictureIndex;
    ScribbleLineFrameNode *_pictureScribble = nullptr;
    
    Image     * _pictureImage = nullptr;
    
    Layout    * _root;
    ArmatureAnimation * animation;
    
public: //状态机模式
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);
    vector<ImageView*> _palettes;
    
private:
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    
    int         _oldIndex = 0;
    
    CC_SYNTHESIZE(ParticleSystemQuad *, _particle, Particle);
    
    Color4F     _color = Color4F(Color3B::RED);
    LoadingLayer* _loadingLayer;
    int         _condition = 0;
    Size        _winSize;
    Vec2        _offsetPicturePos;
    vector<Button*> btnVector;
    
    ArmatureManagerPtr      _model = nullptr;
 
    Color4F _continueColor;
    
    bool _isRandColorPen = false;
    bool _touchSaveBtn = false;
    
    ListView *          _pensList;
    vector<Armature *>  _pensAmt;
    int                 _pensIndex = 0;
    bool                _pensEnabled = true;
    ListView *          _colorsList;
    vector<Armature *>  _colorsAmt;
    int                 _chooseIndex = 0;
    bool                _colorsEnabled = false;
    
    float               _accUpdateTime = 0;
    float               _maxUpdateTime = 0;
    Button*             _backBtn = nullptr;
    
    float               _pictureHeight = 0;
    float               _suggestTime = 0;
    int                 _lastSuggestIndex = -1;
    Color3B             _suggestColor;
    bool                _onSuggest = false;
    bool                _defualtPen = true;
    bool                _haveDraw = false;
    bool                _blockAll = false;
};