//
//  CBGameScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "CBGameScene.h"
#include "../Model/CBUtils.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBColouringManager.h"
#include "../Model/v2.0/ScribbleUtil.h"
#include "../Layer/CBFinishPaintingLayer.h"
#include "../Layer/CBSavingLayer.h"
#include "../Model/CBPackManager.h"
#include "../Layer/CBSettingLayer.h"
#include "../Scene/CBPictureScene.h"
#include "../Model/CBAppManager.h"
#include "../Module/STSystemFunction.h"
#include "../Layer/CBParentalGateLayer.h"

USING_NS_CC;
USING_NS_ST;
CB_BEGIN_NAMESPACE

enum class ZORDERS
{
    SETTING_ZORDER = 100,
    SAVE_ZORDER = 100,
    SETTING_LAYER_ZORDER = 101,
    SAVING_LAYER_ZORDER = 101,
    PENS_LAYER_ZORDER = 99,
    
    MASK_ZORDER = 1000,
    PROGRESS_LAYER_ZORDER = 1001
};

enum class TAGS
{
    PROGRESS_TAG = 124,
    PARENTAL_GATE_TAG = 125
};

/* used to identifier tmp picture which saved by user */
#define Tmp_PictureName "tmp_picture.png"

#define RES_TAB                 "ui04_tab.png"
#define RES_TAB_MASK            "ui04_tab_mask.png"
#define RES_LOADING_BG          "ui05_saving_box.png"
#define RES_PROGRESS_BG         "ui09_saving_progress_1.png"
#define RES_PROGRESS            "ui09_saving_progress_2.png"
#define RES_BRUSH               "solid_brush.png"

#define RES_PENS1_BUTTON        "ui04_btn_pen1.png"
#define RES_PENS1_BUTTON_H      "ui04_btn_pen1_h.png"

#define RES_PENS2_BUTTON        "ui04_btn_pen2.png"
#define RES_PENS2_BUTTON_H      "ui04_btn_pen2_h.png"

#define RES_PATTERN_BUTTON      "ui04_btn_pattern.png"
#define RES_PATTERN_BUTTON_H    "ui04_btn_pattern_h.png"

#define RES_ERASER_BUTTON       "ui04_eraser.png"

#define RES_SAVE_BUTTON         "ui07_public_save.png"
#define RES_SAVE_BUTTON_H       "ui07_public_save_h.png"

#define RES_SETTING_BUTTON      "ui07_public_btn_setting.png"
#define RES_SETTING_BUTTON_H    "ui07_public_btn_setting_h.png"

GameScene::~GameScene()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(_picturePath);
    Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureMaskPath);
    Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureHistoryPath);
    Director::getInstance()->getTextureCache()->removeTextureForKey(Tmp_PictureName);
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    /* -----IMPORTANT------*/
    CC_SAFE_DELETE(_picturePixelMask);
    CC_SAFE_DELETE(_pictureImage);
}

Scene* GameScene::createScene(const int packIndex, const int pictureIndex)
{
    auto scene = Scene::create();
    auto layer = GameScene::create(packIndex, pictureIndex);
    
    scene->addChild(layer);
    return scene;
}

GameScene* GameScene::create(const int packIndex, const int pictureIndex)
{
    GameScene *pRet = new GameScene();
    if (pRet && pRet->init(packIndex, pictureIndex))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GameScene::init(const int packIndex, const int pictureIndex)
{
    _packIndex = packIndex;
    _pictureIndex = pictureIndex;
    
    if (! Layer::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        //setup ui
        setupUI();
        
        //add scribble
        setupScribble();
        
        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);

        pens1Action();
        
        AppManager::getInstance()->setBannerAdVisible(false);

        rcode = true;
    } while (0);
    
    return rcode;
}


#pragma mark- Setup UI

bool GameScene::setupUI()
{
    bool rcode = false;
    do {
        
        auto colorLayerBg = LayerColor::create(Color4B::WHITE);
        addChild(colorLayerBg);
        
        //setting
        _settingButton = Sprite::create(RES_SETTING_BUTTON);
        CC_BREAK_IF(!_settingButton);
        _settingButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        STVisibleRect::setPosition(_settingButton, Vec2(20, 620), true, false, true, false);
        addChild(_settingButton, (int)ZORDERS::SETTING_ZORDER);
        
        //save
        _saveButton = Sprite::create(RES_SAVE_BUTTON);
        _saveButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        STVisibleRect::setPosition(_saveButton, Vec2(580, 620), true, false, false, false);
        addChild(_saveButton, (int)ZORDERS::SAVE_ZORDER);
        
        //pens layer
        _pensContainerLayer = Layer::create();
        addChild(_pensContainerLayer, (int)ZORDERS::PENS_LAYER_ZORDER);
        
        //tab
        Sprite *tab = Sprite::create(RES_TAB);
        CC_BREAK_IF(!tab);
        tab->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(tab, 728, 320);
        _pensContainerLayer->addChild(tab);
        
        //pens1 button
        _pens1Button = Sprite::create(RES_PENS1_BUTTON_H);
        _pens1Button->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_pens1Button, 715, 527);
        _pensContainerLayer->addChild(_pens1Button);

        //pens2 button
        _pens2Button = Sprite::create(RES_PENS2_BUTTON);
        _pens2Button->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_pens2Button, 715, 430);
        _pensContainerLayer->addChild(_pens2Button);
        
        //pattern button
        _patternButton = Sprite::create(RES_PATTERN_BUTTON);
        _patternButton->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_patternButton, 715, 340);
        _pensContainerLayer->addChild(_patternButton);
        
        //tab mask
        Sprite *tabMask = Sprite::create(RES_TAB_MASK);
        CC_BREAK_IF(!tabMask);
        tabMask->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(tabMask, 785, 320);
        _pensContainerLayer->addChild(tabMask);
        
        //pens1 layer
        _pens1Layer = Layer::create();
        _pensContainerLayer->addChild(_pens1Layer);
        _currentVisbleLayer = _pens1Layer;

        std::vector<Pen> *pens1 = ColouringManager::getInstance()->getPens1();
        for (int index = 0;index < pens1->size(); ++index)
        {
            Pen *pen = &pens1->at(index);
            ColorPenSprite *penSprite = ColorPenSprite::createWithPenData(pen);
            penSprite->setTag(index);
            penSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            STVisibleRect::setPosition(penSprite, 825, 640 - (50 + index * (39/*sprite height*/ + 5/*pens interval*/)));
            _pens1Layer->addChild(penSprite);
        }
        
        //pens2 layer
        _pens2Layer = Layer::create();
        _pensContainerLayer->addChild(_pens2Layer);
        _pens2Layer->setVisible(false);
        
        std::vector<Pen> *pens2 = ColouringManager::getInstance()->getPens2();
        for (int index = 0;index < pens2->size(); ++index)
        {
            Pen *pen = &pens2->at(index);
            ColorPenSprite *penSprite = ColorPenSprite::createWithPenData(pen);
            penSprite->setTag(index);
            penSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            STVisibleRect::setPosition(penSprite, 825, 640 - (50 + index * (39/*sprite height*/ + 5/*pens interval*/)));
            _pens2Layer->addChild(penSprite);
        }
        
        //pattern layer
        _patternLayer = Layer::create();
        _pensContainerLayer->addChild(_patternLayer);
        _patternLayer->setVisible(false);
        
        std::vector<Pattern> *patterns = ColouringManager::getInstance()->getPatterns();
        for (int index = 0;index < patterns->size(); ++index)
        {
            Pattern *pattern = &patterns->at(index);
            PatternPenSprite *patternSprite = PatternPenSprite::createWithPatternData(pattern);
            patternSprite->setTag(index);
            patternSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            STVisibleRect::setPosition(patternSprite, 825, 640 - (50 + index * (39/*sprite height*/ + 5/*pens interval*/)));
            _patternLayer->addChild(patternSprite);
        }
        
        //eraser
        _eraserButton = PenSprite::create();
        _eraserButton->setTexture(RES_ERASER_BUTTON);
        CC_BREAK_IF(!_eraserButton);
        _eraserButton->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(_eraserButton, 845, 48);
        _pensContainerLayer->addChild(_eraserButton);

        return true;
    } while (0);
    
    return rcode;
}

#pragma mark- Setup scribble

bool GameScene::setupScribble()
{
    bool rcode = false;
    
    do {
        _finishLoadPicture = false;
        _finishLoadMask = false;
        _finishLoadHistory = false;

        //create loading layer
        _loadingLayer = GameLoadingLayer::create();
        addChild(_loadingLayer, (int)ZORDERS::PROGRESS_LAYER_ZORDER);
        auto loadingBg = Sprite::create(RES_LOADING_BG);
        loadingBg->setPosition(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT/2);
        _loadingLayer->addChild(loadingBg);
        
        //progress bg
        auto progressBg = Sprite::create(RES_PROGRESS_BG);
        progressBg->setPosition(Vec2(378.0, 126.0));
        loadingBg->addChild(progressBg);

        _loadProgressTimer = ProgressTimer::create(Sprite::create(RES_PROGRESS));
        _loadProgressTimer->setTag((int)TAGS::PROGRESS_TAG);
        _loadProgressTimer->setType(ProgressTimer::Type::BAR);
        _loadProgressTimer->setMidpoint(Vec2(0, 1));
        _loadProgressTimer->setBarChangeRate(Vec2(1, 0));
        _loadProgressTimer->setPosition(Vec2(progressBg->getPosition()));
        _loadProgressTimer->runAction(ProgressFromTo::create(2, 0, 90));
        loadingBg->addChild(_loadProgressTimer);

        //add colouring layer
        _colouringLayer = Layer::create();
        addChild(_colouringLayer);
        
        //pack info
        PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex);
        
        //index
        int packIndex = _packIndex + 1; //pack name index begin with "1"
        int pictureIndex = _pictureIndex;

        _pictureHistoryPath = PackManager::getInstance()->getPackHistoryPicturePath(packIndex, pictureIndex, true);
        //mask path
        if (pack->isServerPack)
        {
            _pictureMaskPath = PackManager::getInstance()->getPackMaskPicturePath(packIndex, pictureIndex, true);
            _picturePath = PackManager::getInstance()->getPackPicturePath(packIndex, pictureIndex, true);
        }
        else
        {
            _pictureMaskPath = PackManager::getInstance()->getPackMaskPicturePath(packIndex, pictureIndex, false);
            _picturePath = PackManager::getInstance()->getPackPicturePath(packIndex, pictureIndex, false);
        }
        
        /*-----DEBUG----*/
//        _picturePath = "pictures/Pack_1_1_picture.png";
//        _pictureMaskPath = "pictures/Pack_1_1_mask.png";
        //_pictureHistoryPath= "adsfsadfas";
        
        Director::getInstance()->getTextureCache()->addImageAsync(_picturePath, [this](Texture2D *texture){
        
            //create front picture
            _picture = Sprite::create(_picturePath);
            _picture->setPosition(Utils::getInstance()->getScreenCenter());
            
            //create picture scribble
            _pictureScribble = ScribblePatternNode::create(Director::getInstance()->getWinSize());//_picture->getContentSize()
            _pictureScribble->setPosition(Utils::getInstance()->getScreenCenter());
            _colouringLayer->addChild(_pictureScribble);
            _colouringLayer->addChild(_picture);//in front of scribble
            
            _finishLoadPicture = true;
            if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
            {
                _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([this](){
                    
                    removeChildByTag((int)TAGS::PROGRESS_TAG);
                    _loadingLayer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });

        //check sandbox history
        bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
        if (isExsit)
        {
            Director::getInstance()->getTextureCache()->addImageAsync(_pictureHistoryPath, [this](Texture2D *texture){
                
                _pictureScribble->setPattern(_pictureHistoryPath);
                
                Sprite *history = Sprite::create(_pictureHistoryPath);
                history->setPosition(Utils::getInstance()->getScreenCenter());

                _pictureScribble->getCanvas()->begin();
                history->visit();
                _pictureScribble->getCanvas()->end();
                
                _finishLoadHistory = true;
                if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
                {
                    _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([this](){
                        
                        removeChildByTag((int)TAGS::PROGRESS_TAG);
                        _loadingLayer->removeFromParentAndCleanup(true);
                    }), NULL));
                }
            });
        }
        else
        {
            _finishLoadHistory = true;
        }
        
        Director::getInstance()->getTextureCache()->addImageAsync(_pictureMaskPath, [this](Texture2D *texture){
            
            //others
            _pictureScribble->setPattern(_pictureMaskPath);
            _pictureScribble->useBrush(RES_BRUSH);
            _pictureScribble->useTarget(Sprite::create(_pictureMaskPath));
            
            _selectColorPen = nullptr;
            _selectPatternPen = nullptr;
            
            CC_SAFE_DELETE(_picturePixelMask);
            _picturePixelMask = new Image();
            _picturePixelMask->initWithImageFile(_pictureMaskPath);
            
            _finishLoadMask = true;
            if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
            {
                _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([this](){
                    
                    removeChildByTag((int)TAGS::PROGRESS_TAG);
                    _loadingLayer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });

        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Touch events

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    /* -----IMPORTANT---- */
    if (_isSaving)
    {
        _isSaving = false;
        CC_SAFE_DELETE(_pictureImage);
    }
    
    //indicate setting action
    Point point2 = convertToNodeSpace(touch->getLocation());
    if (_settingButton->getBoundingBox().containsPoint(point2))
    {
        _settingButton->setTexture(RES_SETTING_BUTTON_H);
        _isColouring = false;
        return true;
    }
    
    //indicate save action
    if (_saveButton->getBoundingBox().containsPoint(point2))
    {
        _saveButton->setTexture(RES_SAVE_BUTTON_H);
        _isColouring = false;
        return true;
    }
    
    Point touchPoint = convertToWorldSpace(touch->getLocation());
    Vec2 touchInPicture = _picture->convertToNodeSpace(touchPoint);
    Color4B color = ScribbleUtil::getColor(_picturePixelMask, touchInPicture);
    
    if (color.a == 0)
    {
        _isColouring = false;
        
        //indicate choose pen type aciton
        Point point = _pensContainerLayer->convertToNodeSpace(touch->getLocation());
        if (_pens1Button->getBoundingBox().containsPoint(point))
        {
            pens1Action();
            return true;
        }
        else if (_pens2Button->getBoundingBox().containsPoint(point))
        {
            pens2Action();
            return true;
        }
        else if (_patternButton->getBoundingBox().containsPoint(point))
        {
            patternAction();
            return true;
        }
        else if (_eraserButton->getBoundingBox().containsPoint(point))
        {
            eraserAction();
            return true;
        }

        //touch pens layer
        touchPensLayer(touch);
    }
    else
    {
        if (_selectColorPen || _selectPatternPen || _eraserButton->isShow())
        {
            _isColouring = true;
            
            _pictureScribble->setPatternColor(Color4F(color));
            _pictureScribble->paint(touchPoint);
        }
    }
    
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (_isColouring && (_selectColorPen || _selectPatternPen || _eraserButton->isShow()))
    {
        Point touchPoint = convertToWorldSpace(touch->getLocation());
        Point previousTouchPoint = this->convertToWorldSpace(touch->getPreviousLocation());
        _pictureScribble->paint(touchPoint, previousTouchPoint);
    }
    else
    {
        Point point = convertToNodeSpace(touch->getLocation());
        
        _saveButton->setTexture(RES_SAVE_BUTTON);
        _settingButton->setTexture(RES_SETTING_BUTTON);
        
        if (_settingButton->getBoundingBox().containsPoint(point))
        {
            _settingButton->setTexture(RES_SETTING_BUTTON_H);
            return;
        }
        else if(_saveButton->getBoundingBox().containsPoint(point))
        {
            _saveButton->setTexture(RES_SAVE_BUTTON_H);
            return;
        }
        else if(_eraserButton->getBoundingBox().containsPoint(point))
        {
            eraserAction();
            return;
        }
        
        //touch pens layer
        touchPensLayer(touch);
    }
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (_isColouring)
    {
        
    }
    else
    {
        Point point = convertToNodeSpace(touch->getLocation());
        if (_settingButton->getBoundingBox().containsPoint(point))
        {
            _settingButton->setTexture(RES_SETTING_BUTTON);
            settingAction();
        }
        else if (_saveButton->getBoundingBox().containsPoint(point))
        {
            _saveButton->setTexture(RES_SAVE_BUTTON);
            savingAction();
        }
    }
}

void GameScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    _saveButton->setTexture(RES_SAVE_BUTTON);
    _settingButton->setTexture(RES_SETTING_BUTTON);
}

#pragma mark- Pen select

void GameScene::touchPensLayer(Touch *touch)
{
    //touch in pens layer
    Point point = _currentVisbleLayer->convertToNodeSpace(touch->getLocation());
    Sprite *pen = nullptr;
    bool find = false;
    for (int i = 0; i < _currentVisbleLayer->getChildren().size(); ++i)
    {
        pen = dynamic_cast<Sprite*>(_currentVisbleLayer->getChildByTag(i));
        if (pen && pen->getBoundingBox().containsPoint(point))
        {
            find = true;
            break;
        }
    }
    
    if (!find)
        return;

    if (ColouringManager::getInstance()->getCurrentPensType() == PensType::PensType_Patter)
    {
        //pattern layer
        PatternPenSprite *patternPen = dynamic_cast<PatternPenSprite*>(pen);
        if (patternPen->isLoced())
        {
            if (!_isPurchasing)
            {
                _isPurchasing = true;
                _appBilling.setIABDelegate(this);
                _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//                if (!getChildByTag((int)TAGS::PARENTAL_GATE_TAG))
//                {
//                    ParentalGateLayer *layer = ParentalGateLayer::create();
//                    layer->setTag((int)TAGS::PARENTAL_GATE_TAG);
//                    layer->setCallback([=](){
//                        
//                        _appBilling.setIABDelegate(this);
//                        _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
//                        _isPurchasing = true;
//                    });
//                    layer->showInLayer(this);
//                }
//#else
//                _appBilling.setIABDelegate(this);
//                _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
//                _isPurchasing = true;
//#endif
            }
            return;
        }
        
        if (_selectPatternPen)
            _selectPatternPen->restoreWithAnimation(true);
        
        patternPen->showWithAnimation(true);
        selectPatternPen(patternPen);
    }
    else
    {
        //pens1 and pens2 layer
        ColorPenSprite *colorPen = dynamic_cast<ColorPenSprite*>(pen);
        
        if (_selectColorPen)
            _selectColorPen->restoreWithAnimation(true);
        colorPen->showWithAnimation(true);
        selectColorPen(colorPen);
    }

    _eraserButton->restoreWithAnimation(true);
}

void GameScene::selectColorPen(ColorPenSprite *colorPen)
{
    _selectColorPen = colorPen;
    
    Pen *pen = _selectColorPen->getColorPenData();
    Color4F color((float)pen->R/255.0,
                  (float)pen->G/255.0,
                  (float)pen->B/255.0,
                  (float)pen->A/255.0);
    _pictureScribble->useTarget(color);
}

void GameScene::selectPatternPen(PatternPenSprite *patternPen)
{
    _selectPatternPen = patternPen;
    Pattern *pattern = patternPen->getPatternPenData();
    
    Sprite *patternSprite = Sprite::create(pattern->pattern);
    
    if (patternSprite)
        _pictureScribble->useTarget(patternSprite);
}

#pragma mark- Action methods

void GameScene::restorePensButtons()
{
    _pens1Button->setTexture(RES_PENS1_BUTTON);
    _pens2Button->setTexture(RES_PENS2_BUTTON);
    _patternButton->setTexture(RES_PATTERN_BUTTON);
    
    _pens1Layer->setVisible(false);
    _pens2Layer->setVisible(false);
    _patternLayer->setVisible(false);
    
    if (_selectPatternPen)
        _selectPatternPen->restoreWithAnimation(false);
    
    if (_selectColorPen)
        _selectColorPen->restoreWithAnimation(false);
    
    if (_eraserButton)
        _eraserButton->restoreWithAnimation(false);

    _selectColorPen = nullptr;
    _selectPatternPen = nullptr;
}

void GameScene::eraserAction()
{
    if (ColouringManager::getInstance()->getCurrentPensType() == PensType::PensType_Patter)
    {
       if (_selectPatternPen)
           _selectPatternPen->restoreWithAnimation(true);
    }
    else
    {
       if (_selectColorPen)
           _selectColorPen->restoreWithAnimation(true);
    }
    
    _eraserButton->showWithAnimation(true);
    
    Color4F color(0,0,0,0);
    _pictureScribble->useTarget(color);
}

void GameScene::pens1Action()
{
    PensType currentType = ColouringManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Pen1) {
        return;
    }
    
    if (currentType != PensType::PensType_None)
        AppManager::getInstance()->playPensChangeEffect();

    //restore all buttons
    restorePensButtons();
    
    //set select image
    _pens1Button->setTexture(RES_PENS1_BUTTON_H);

    //set pens1 layer visible
    _pens1Layer->setVisible(true);
    _currentVisbleLayer = _pens1Layer;
    
    //set current pens type
    ColouringManager::getInstance()->setCurrentPensType(PensType::PensType_Pen1);
}

void GameScene::pens2Action()
{
    PensType currentType = ColouringManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Pen2) {
        return;
    }
    
    AppManager::getInstance()->playPensChangeEffect();

    //restore all buttons
    restorePensButtons();

    //set select image
    _pens2Button->setTexture(RES_PENS2_BUTTON_H);
    
    //set pens1 layer visible
    _pens2Layer->setVisible(true);
    _currentVisbleLayer = _pens2Layer;

    //set current pens type
    ColouringManager::getInstance()->setCurrentPensType(PensType::PensType_Pen2);
}

void GameScene::patternAction()
{
    PensType currentType = ColouringManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Patter) {
        return;
    }
    
    AppManager::getInstance()->playPensChangeEffect();
    
    //restore all buttons
    restorePensButtons();

    //set select image
    _patternButton->setTexture(RES_PATTERN_BUTTON_H);
    
    //set pens1 layer visible
    _patternLayer->setVisible(true);
    _currentVisbleLayer = _patternLayer;

    //set current pens type
    ColouringManager::getInstance()->setCurrentPensType(PensType::PensType_Patter);
}

void GameScene::settingAction()
{
    AppManager::getInstance()->playButtonEffect();
    SettingLayer *layer = SettingLayer::create();
    addChild(layer, (int)ZORDERS::SETTING_LAYER_ZORDER);
}

void GameScene::savingAction()
{
    AppManager::getInstance()->playButtonEffect();
    /* -----IMPORTANT---- */
    if (_isSaving)
    {
        _isSaving = false;
        CC_SAFE_DELETE(_pictureImage);
    }
    
    generateCurrentPaintingPicture(Tmp_PictureName);
    
    log("MX: generate current painting picture success!");

    FinishPaintingLayer *layer = FinishPaintingLayer::create(_pictureImage);
    
    layer->setQuitCallback(CC_CALLBACK_0(GameScene::finishPaintingQuit, this));
    layer->setSaveCallback(CC_CALLBACK_0(GameScene::finishPaintingSave, this));
    addChild(layer, (int)ZORDERS::SAVING_LAYER_ZORDER);
    
    log("MX: finish painting layer create success!");
    
    /* save the picture logo, include wireframe */
    std::string path = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex+1, _pictureIndex, true);
    _pictureImage->saveToFile(path.c_str());
    
    log("MX: save picture logo success: %s!", path.c_str());

    /* save the picture history, just color mask, not include wireframe */
    std::string picturePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, _pictureIndex, false);
    _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
    
    log("MX: save picture history success: %s!", picturePath.c_str());

    _isSaving = true;
}

#pragma mark- Get painting picture

void GameScene::generateCurrentPaintingPicture(const std::string& pictureName)
{
    auto renderTexture = RenderTexture::create(Utils::SCREEN_WIDTH - (310 - (1136 - Utils::SCREEN_WIDTH)/2.0), _colouringLayer->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _colouringLayer->visit();
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    _pictureImage = renderTexture->newImage();
    
    //issue001: android app will crash!
//    /* remove last one */
//    Director::getInstance()->getTextureCache()->removeTextureForKey(pictureName);
//    
//    /* used in saving layer */
//    Director::getInstance()->getTextureCache()->addImage(_pictureImage, pictureName);
}

#pragma mark- Finish Painting Layer Call back

void GameScene::finishPaintingQuit()
{
    AppManager::getInstance()->setBannerAdVisible(false);

    auto scene = PictureScene::createSceneWithPackIndex(_packIndex);
    Director::getInstance()->replaceScene(scene);
}

void GameScene::finishPaintingSave()
{
    AppManager::getInstance()->setBannerAdVisible(false);

    STSystemFunction function = STSystemFunction();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    function.saveLocalImage(_pictureImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string writePath = function.getSdCardPath()+"/ColorBook/";
    
    if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
    {
        PackManager::getInstance()->createDirectory(writePath.c_str());
        log("MX: android writeable path: %s", writePath.c_str());
    }
    
    struct timeval now;
    gettimeofday(&now, nullptr);
    std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
    std::string fullPath = writePath + _time + ".jpg";
    
    log("MX: %s", fullPath.c_str());

    bool isSaveSuccess = _pictureImage->saveToFile(fullPath.c_str());
    if(isSaveSuccess)
        function.makeToast("Save successfully!");
    else
        function.makeToast("Save failed!");
    function.refreshDCIM(fullPath);
#endif
    
    /* -----Ads Logic----- */
    AppManager::getInstance()->requestFullScreenAd();
    
    /* -----Rate Us Logic----- */
    AppManager::getInstance()->rateUsLogic();
}

#pragma mark- STIABDelegate

void GameScene::purchaseSuccessful(const char* pid)
{
    _isPurchasing = false;
    if (std::string(pid) == IAP_UNLOCK_PATTERN_PEN) {
        
        //write to user default, pattern pen has been unlocked
        ColouringManager::getInstance()->unlockPatternPen();
        
        //remove pattern pen lock logo
        Vector<Node*> children = _patternLayer->getChildren();
        for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
        {
            PatternPenSprite *patternSprite = dynamic_cast<PatternPenSprite*>(*iterator);
            patternSprite->removeLock();
        }
    }
}

void GameScene::purchaseFailed(const char *pid, int errorCode)
{
    _isPurchasing = false;

}

void GameScene::restoreSuccessful(const char* pid)
{
    _isPurchasing = false;

}

void GameScene::restoreFailed(const char* pid, int errorCode)
{
    _isPurchasing = false;

}



CB_END_NAMESPACE
