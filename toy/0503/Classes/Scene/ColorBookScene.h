//
//  ColorBookScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__ColorBookScene__
#define __BedtimeStory__ColorBookScene__

#include "BaseScene.h"
#include "../Model/v2.0/ScribblePatternNode.h"//do not using name space in header file!
#include "PatternPenSprite.h"
#include "ColorPenSprite.h"
#include "STInAppBilling.h"

class ColorBookScene : public BaseScene, public STIABDelegate
{
public:
    ColorBookScene():
    _picturePixelMask(nullptr),
    _pictureScribble(nullptr),
    _colouringLayer(nullptr),
    _picture(nullptr),
    _selectPatternPen(nullptr),
    _selectColorPen(nullptr),
    _eraserButton(nullptr),
    _pictureImage(nullptr)
    {};
    virtual ~ColorBookScene();
  
    /* init */
    virtual bool init() override;
    
    void startGameWithGameLevel(const int level);

    /* register touch event */
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    
    /* STIABDelegate */
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
    
protected:
    
    void saveHistory();
    
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
    
    /* refresh */
    void refreshAction();
    
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
    
    /* select color pen */
    void selectColorPen(ColorPenSprite *colorPen);
    
    /* select pattern pen */
    void selectPatternPen(PatternPenSprite *patternPen);
    void showPurchaseDialog();

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
    bool _isPurchasing = false;
    
    /* used to indicate if is saving picture now? when user finish saving picture to
     album, the _pictureImage must be delete, it's very important, so set this value
     to true when user tap the save button, when back to GameScene again and touch
     begin, check this value, if it is true, delete _pictureImage and set this value
     to false.
     */
    bool _isSaving;
    
    /* the iap class */
    //STInAppBilling _appBilling;
    
    /* used to identifier the path of the texture */
    std::string _picturePath;
    std::string _pictureHistoryPath;
    std::string _pictureMaskPath;
    
    /* used to store picture image when user tap save method */
    cocos2d::Image *_pictureImage;
    
    /* the current select pen or pattern */
    ColorPenSprite *_selectColorPen;
    PatternPenSprite *_selectPatternPen;
    
    /* container layers */
    cocos2d::Layer *_pensContainerLayer;
    cocos2d::Layer *_pens1Layer;
    cocos2d::Layer *_pens2Layer;
    cocos2d::Layer *_currentVisbleLayer;
    cocos2d::Layer *_patternLayer;
    cocos2d::Layer *_colouringLayer;
    
    /* used to loading big picture */
    //GameLoadingLayer *_loadingLayer;

    /* buttons */
    BasePenSprite *_eraserButton;
    Sprite *_pens1Button;
    Sprite *_pens2Button;
    Sprite *_patternButton;
    Sprite *_refreshButton;

    int _gameLevel;
    STInAppBilling _iap;

    /* scribble */
    Sprite *_picture;
    ScribblePatternNode *_pictureScribble;
    Image *_picturePixelMask;
};

#endif /* defined(__BedtimeStory__ColorBookScene__) */
