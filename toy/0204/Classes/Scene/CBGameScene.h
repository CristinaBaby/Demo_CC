//
//  CBGameScene.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__CBGameScene__
#define __ColorBook__CBGameScene__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "ScribbleLineFrameNode.h"//do not using name space in header file!
#include "../Layer/CBPatternPenSprite.h"
#include "../Layer/CBColorPenSprite.h"
#include "../Module/STInAppBilling.h"
#include "../Layer/CBGameLoadingLayer.h"
#include "CocosGUI.h"
#include "ui/UIListView.h"
#include "BaseMode.h"

class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;

using namespace cocos2d;
using namespace ui;

CB_BEGIN_NAMESPACE

class GameScene : public cocos2d::Layer, public STIABDelegate, public ScribbleLineFrameLoadDelegate
{
public:
    struct Paint
    {
        Color4F color;
        Vec2    pos;
        Vec2    previousPos;
    };
    
    virtual void onEnterTransitionDidFinish() override;
public:
    virtual ~GameScene();
    
    /* init with pack index and picture index */
    static cocos2d::Scene* createScene(const int packIndex, const int pictureIndex);
    static GameScene* create(const int packIndex, const int pictureIndex);
    virtual bool init(const int packIndex, const int pictureIndex);
    
    /* register touches event */
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);
    
    /* STIABDelegate */
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
    /*my function*/
    void onButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void onPencil(Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
    void onEraser(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type);
    void switchMode(BaseMode::Mode mode);
    Color4F getTouchColor(Touch * touche);
    Layer * getColouringLayer();
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void setBrushScale(float scale);
    void changeColor();
    void savingActionForEnterBackground();
    
    virtual void loadPercent(Node *pNode, float pPercent); //Pattern加载进度0.0~1.0
    virtual void loadEnded(Node *pNode);

protected:
    
    /* restore pens1 pens2 pattern button to original state */
    void restorePensButtons();
    
    /* choose pens1 type */
    void pens1Action();
    
    /* choose pens2 type */
    void pens2Action();
    
    /* choose pattern type */
    void patternAction();
    
    /* choose eraser */
    void eraserAction();

    /* setting action */
    void settingAction();
    
    /* saving action, will show finish painting layer */
    void savingAction();
    
    /* finish painting layer call back, no will save history and quit to picture scene, yes will save the picture to user album*/
    void finishPaintingQuit();
    void finishPaintingSave();
    
    /* setup ui element */
    bool setupUI();
    
    /* setup scribble */
    bool setupScribble();
    
    /* using RenderTexture to generate current painting picture, just store in _picutreImage
       DO NOT write to file */
    void generateCurrentPaintingPicture(const std::string& pictureName);
    
    /* When user touch in pens layer, whis method will be used to indicate which pen or pattern is been selected */
    void touchPensLayer(cocos2d::Touch *touch);
    void touchPensLayer(cocos2d::Sprite* pen);
    
    /* select color pen */
    void selectColorPen(ColorPenSprite *colorPen);
    
    /* select pattern pen */
    void selectPatternPen(PatternPenSprite *patternPen);

    cocos2d::ui::Layout* createLayout(ColorPenSprite* sp);
protected:
    
    /* pack index */
    int _packIndex;
    
    /* picture index */
    int _pictureIndex;
    
    /* used to indicate if is current painting? */
    bool _isColouring;
    
    /* because load picture/mask/history maybe taking a long time, so we need async load
       the texture to texturecache first, these bool value used to indicate if has
       finished load the texture */
    bool _finishLoadPicture;
    bool _finishLoadMask;
    bool _finishLoadHistory;
    
    /* used to indicate if is purchasing pattern pen now, avoid request more than one time
       purchase */
    bool _isPurchasing;
    
    bool choosePattern=false;
    
    /* used to indicate if is saving picture now? when user finish saving picture to
       album, the _pictureImage must be delete, it's very important, so set this value
       to true when user tap the save button, when back to GameScene again and touch 
       begin, check this value, if it is true, delete _pictureImage and set this value
       to false.
       */
    bool _isSaving = false;
    
    /* the iap class */
    STInAppBilling _appBilling;

    /* used to identifier the path of the texture */
    std::string _picturePath;
    std::string _pictureHistoryPath;
    std::string _pictureMaskPath;
    
    /* used to store picture image when user tap save method */
    cocos2d::Image *_pictureImage = nullptr;
    
    /* the current select pen or pattern */
    ColorPenSprite *_selectColorPen = nullptr;
    PatternPenSprite *_selectPatternPen = nullptr;
    
    /* container layers */
    cocos2d::Layer *_pensContainerLayer = nullptr;
    cocos2d::Layer* _pensLayer = nullptr;
    cocos2d::Layer *_currentVisbleLayer = nullptr;
    cocos2d::Layer *_patternLayer = nullptr;
    cocos2d::Layer *_colouringLayer = nullptr;
    
    /* used to loading big picture */
    GameLoadingLayer *_loadingLayer = nullptr;

    /* async load texture progress */
    cocos2d::ProgressTimer *_loadProgressTimer = nullptr;
    
    /* buttons */
    Button *_eraserButton = nullptr;
    Sprite *_pens1Button = nullptr;
    Sprite *_pens2Button = nullptr;

    /* scribble */
    Sprite *_picture = nullptr;
    ScribbleLineFrameNode *_pictureScribble = nullptr;
    
protected:
    Image *_picturePixelMask = nullptr;
    
    /*my code*/
    cocos2d::ui::ListView* _list = nullptr;
    float _prePosition;
    cocos2d::ui::Layout* _preLayout = nullptr;
    cocos2d::ui::Button* _pencil = nullptr;
    bool _isPencil = true;
    bool _isEraser = false;
    Vec2 _eraerPrePosition;
    int preIndex = 999;
    Sprite* _progressBg;
    Sprite* _table;
    Sprite* _guide;
   
    //��舵����烘ā寮�
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    
    int        _condition = 0;
    
public:
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);
};


CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBGameScene__) */
