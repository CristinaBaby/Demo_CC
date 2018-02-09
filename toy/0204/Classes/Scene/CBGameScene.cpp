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
#include "../Layer/CBFinishPaintingLayer.h"
#include "../Layer/CBSavingLayer.h"
#include "../Model/CBPackManager.h"
#include "../Layer/CBSettingLayer.h"
#include "../Scene/CBPictureScene.h"
#include "../Model/CBAppManager.h"
#include "../Module/STSystemFunction.h"
#include "../Layer/CBParentalGateLayer.h"
#include "STAds.h"
#include "../Layer/AdsLoadingLayer.h"
#include "AdLoadingLayerBase.h"
#include "ScribbleUtil.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"


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

#define RES_TAB_MASK            "ui04_tab_mask.png"
#define RES_LOADING_BG          "ui05_saving_box.png"
#define RES_PROGRESS_BG         "ui09_saving_bar.png"
#define RES_PROGRESS            "ui09_saving_sroll.png"
#define RES_BRUSH               "brush_1.png"

#define RES_ERASER_BUTTON       "ui04_eraser.png"

#define RES_SAVE_BUTTON         "ui07_public_save.png"

#define RES_SETTING_BUTTON      "ui07_public_btn_setting.png"

#define RES_PEN_PENCIL          "ui04_pencil_0.png"

#define OFFSET_X_LOADING        (120)

#define TAG_PENCIL              (1119)

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

    CC_SAFE_DELETE(_noneMode);
    CC_SAFE_DELETE(_waitMode);
    CC_SAFE_DELETE(_paintMode);
    CC_SAFE_DELETE(_moveMode);
    CC_SAFE_DELETE(_zoomMode);

}

Scene* GameScene::createScene(const int packIndex, const int pictureIndex)
{
    auto scene = Scene::create();
    auto layer = GameScene::create(packIndex, pictureIndex);
    layer->setName("GameScene");
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
        
        
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        listener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
       
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        _noneMode = new NoneMode(this);
        _waitMode = new WaitMode(this);
        _paintMode = new PaintMode(this);
        _moveMode = new MoveMode(this);
        _zoomMode = new ZoomMode(this);
        
        switchMode(BaseMode::Mode::NONE);

        pens1Action();

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
        
        auto setting = Button::create(RES_SETTING_BUTTON);
        setting->setName("setting");
        setting->setPressedActionEnabled(true);
        setting->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
        auto back = ui::Button::create("ui07_public_back.png");
        setting->setPosition(Vec2(back->getContentSize().width/4 + setting->getContentSize().width/2, Utils::SCREEN_HEIGHT - back->getContentSize().width/4 - setting->getContentSize().height/2));
        addChild(setting, (int)ZORDERS::SETTING_ZORDER);
        
        //pens layer
        _pensContainerLayer = Layer::create();
        addChild(_pensContainerLayer, (int)ZORDERS::PENS_LAYER_ZORDER);
        
        //tab mask
        Sprite *tabMask = Sprite::create(RES_TAB_MASK);
        CC_BREAK_IF(!tabMask);
        tabMask->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(tabMask, 785, 320);
        _pensContainerLayer->addChild(tabMask);
        
        //save
        auto save = Button::create(RES_SAVE_BUTTON);
        save->setName("save");
        save->setPressedActionEnabled(true);
        save->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
        save->setPositionX(tabMask->getPositionX() - (20 + setting->getContentSize().width / 2));
        save->setPositionY(setting->getPositionY());
        addChild(save, (int)ZORDERS::SAVE_ZORDER);
        
        //pens1 layer
        _pensLayer = Layer::create();
        _pensContainerLayer->addChild(_pensLayer);
        _currentVisbleLayer = _pensLayer;
        
        //eraser
        _eraserButton = Button::create(RES_ERASER_BUTTON,RES_ERASER_BUTTON);
        STVisibleRect::setPosition(_eraserButton, 920, 640 - 39);
        _pensLayer->addChild(_eraserButton);
        _eraserButton->setTouchEnabled(true);
        _eraserButton->setPressedActionEnabled(true);
        _eraserButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onEraser, this));
        _eraerPrePosition = _eraserButton->getPosition();
        
        float listStartY = STVisibleRect::getRealPositionForDesignPosition(Vec2(0, 130)).y;
        
        _list = ListView::create();
        _list->setBounceEnabled(false);
        _list->setItemsMargin(0);
        _list->setContentSize(Size(270, Director::getInstance()->getVisibleSize().height - listStartY));
        _list->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        _list->setClippingEnabled(true);
        _list->setAnchorPoint(Vec2(0.5, 0.5));
        STVisibleRect::setPosition(_list, 940, (640 - listStartY)/2);
        
        std::vector<Pen> *pens1 = ColouringManager::getInstance()->getPens1();
        for (int index = 0;index < pens1->size(); ++index)
        {
            Pen *pen = &pens1->at(index);
            ColorPenSprite *penSprite = ColorPenSprite::createWithPenData(pen);
            penSprite->setTag(index);
            auto layout = createLayout((ColorPenSprite*)penSprite);
            penSprite->setPosition(layout->getContentSize()/2);
            layout->addChild(penSprite);
            _prePosition = layout->getPositionX();
            _list->pushBackCustomItem(layout);
        }
        
        _pensLayer->addChild(_list,10);
        
        /*pencil pen*/
        _pencil = ui::Button::create(RES_PEN_PENCIL,RES_PEN_PENCIL);
        _pencil->setPosition(Vec2(_list->getPositionX(),_eraserButton->getPositionY()-_pencil->getContentSize().height-_eraserButton->getContentSize().height/2 + 12.5));
        _pencil->addTouchEventListener(CC_CALLBACK_2(GameScene::onPencil, this));
        _pencil->setTag(TAG_PENCIL);
        _pensLayer->addChild(_pencil,10);

        return true;
    } while (0);
    
    return rcode;
}

Layout* GameScene::createLayout(colorbook::ColorPenSprite *sp){
    auto layout = Layout::create();
    layout->setContentSize(Size(sp->getContentSize().width * 1.3, sp->getContentSize().height));
    layout->setTouchEnabled(true);
    layout->addTouchEventListener(CC_CALLBACK_2(GameScene::onPencil, this));
    return layout;
}

void GameScene::onButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != cocos2d::ui::Widget::TouchEventType::ENDED) return;
    
    auto widget = (Widget *)pSender;
    string name = widget->getName();
    
    if (name.compare("setting") == 0) {
        settingAction();
    }
    else if (name.compare("save") == 0)
    {
        savingAction();
    }
}

void GameScene::onEraser(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type)
{
    if (type != cocos2d::ui::Widget::TouchEventType::ENDED) return;
    
    if(_preLayout != nullptr){
        _preLayout->stopAllActions();
        _preLayout->setPositionX(_prePosition);
    }
    if(_isPencil)
    {
        _pencil->setPosition(Vec2(_pencil->getPositionX()+40,_pencil->getPositionY()));
        _isPencil = false;
    }
    
//    Color4F color(0, 0, 0, 0);
//    _pictureScribble->getScribble()->setBrushType(Scribble::BrushType::eEaser);
    if(!_isEraser)
    {
        _eraserButton->runAction(CCMoveBy::create(0.3f, Vec2(-40,0)));
        _isEraser = true;
    }
    Color4F color(1, 1, 1, 1);
    _pictureScribble->useTarget(color);
}

void GameScene::onPencil(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type)
{
    if (type != cocos2d::ui::Widget::TouchEventType::ENDED) return;
    
    if(_preLayout != nullptr){
        _preLayout->stopAllActions();
        _preLayout->setPositionX(_prePosition);
    }
    
    if (_isEraser) {
        _eraserButton->setPosition(_eraerPrePosition);
        _isEraser = false;
    }
    
    auto target = (Layout*)pSender;
    if (TAG_PENCIL == target->getTag()&&!_isPencil) {
        _pencil->runAction(MoveBy::create(0.3f,(Vec2(-40,0))));
        _isPencil = true;
        
//        std::vector<Pen> *pens1 = ColouringManager::getInstance()->getPens1();
//        int index = rand()%(pens1->size());
//        Sprite* pencil = dynamic_cast<Sprite*>(_list->getChildren().at(index)->getChildren().at(0));
//        ColorPenSprite* colorPen = dynamic_cast<ColorPenSprite*>(pencil);
//        selectColorPen(colorPen);
        return;
    }
    
    if (target->getPositionX() == _prePosition)
    {
        target->runAction(MoveBy::create(0.3, Vec2(-40, 0)));
        _preLayout = target;
        
        if(target->getName() == "pattern")
            choosePattern = true;
        else
            choosePattern = false;
        if(_isPencil)
        {
            _pencil->setPosition(Vec2(_pencil->getPositionX()+40,_pencil->getPositionY()));
            _isPencil = false;
        }
        touchPensLayer((Sprite*)target->getChildren().at(0));
    }
}

#pragma mark- Touches event
void GameScene::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesBegan(touches, unused_event);
}

void GameScene::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    _currentMode->onTouchesMoved(touches, unused_event);
}

void GameScene::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    _currentMode->onTouchesEnded(touches, unused_event);
}

void GameScene::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    _currentMode->onTouchesCancelled(touches, unused_event);
}

#pragma mark- Setup scribble

bool GameScene::setupScribble()
{
    bool rcode = false;
    
    do {
        AppManager::getInstance()->showBannerAd();
        
        _finishLoadPicture = false;
        _finishLoadMask = false;
        _finishLoadHistory = false;

        //create loading layer
        _loadingLayer = GameLoadingLayer::create();
        addChild(_loadingLayer, (int)ZORDERS::PROGRESS_LAYER_ZORDER);
        
        Size winSize = Director::getInstance()->getWinSize();
        //progress bg
        _progressBg = Sprite::create(RES_PROGRESS_BG);
        _progressBg->setPosition(Vec2(winSize.width/2,150));
        _loadingLayer->addChild(_progressBg);
        _progressBg->setName("progressBg");

        _loadProgressTimer = ProgressTimer::create(Sprite::create(RES_PROGRESS));
        _loadProgressTimer->setTag((int)TAGS::PROGRESS_TAG);
        _loadProgressTimer->setType(ProgressTimer::Type::BAR);
        _loadProgressTimer->setMidpoint(Vec2(0, 1));
        _loadProgressTimer->setBarChangeRate(Vec2(1, 0));
        _loadProgressTimer->setPosition(Vec2(_progressBg->getPosition()));
        _loadingLayer->addChild(_loadProgressTimer);

        
        //add guide
        _guide = Sprite::create("guide.png");
        _guide->setPosition(Vec2(winSize.width/2,winSize.height/2+85));
        _loadingLayer->addChild(_guide);
        
        // add tab
        _table = Sprite::create("guide_taptostart.png");
        _table->setVisible(false);
        _table->setPosition(_progressBg->getPosition());
        _loadingLayer->addChild(_table);
        _table->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.1), ScaleTo::create(0.5f, 1.0), NULL)));
        
        //add colouring layer
        _colouringLayer = Layer::create();
        _colouringLayer->ignoreAnchorPointForPosition(false);
        _colouringLayer->setAnchorPoint(Vec2::ZERO);
        addChild(_colouringLayer);
        _colouringLayer->setName("colouringLayer");
        
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
        
        _pictureScribble = ScribbleLineFrameNode::create(Size(1136, 768));
        _pictureScribble->setDelegate(this);
        _pictureScribble->setDefaultColor(Color4F::WHITE);
        _pictureScribble->setAntiAliasingBrush(true);
        _pictureScribble->setPosition(Utils::getInstance()->getScreenCenter());
        
        _pictureScribble->setLineFrame(Sprite::create(_picturePath));
        _pictureScribble->useBrush(RES_BRUSH);
        _colouringLayer->addChild(_pictureScribble, 1);
        

        //check sandbox history
        bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
        if (isExsit)
        {
            Sprite *history = Sprite::create(_pictureHistoryPath);
            history->setPosition(_pictureScribble->getContentSize()/2);
            
            _pictureScribble->getCanvas()->begin();
            history->visit();
            _pictureScribble->getCanvas()->end();
        }
        
        _pictureScribble->setPattern(_picturePath, true, 10);
        
        _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(2.0, _loadProgressTimer->getPercentage(), 100), CallFunc::create([this](){
            _condition++;
            if (_condition > 1) {
                removeChildByTag((int)TAGS::PROGRESS_TAG);
                _loadProgressTimer->removeFromParentAndCleanup(true);
                _progressBg->removeFromParentAndCleanup(true);
                _table->setVisible(true);
            }
        }), NULL));
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void GameScene::loadPercent(Node *pNode, float pPercent)
{
}

void GameScene::loadEnded(Node *pNode)
{
    _condition++;
    if (_condition > 1) {
        removeChildByTag((int)TAGS::PROGRESS_TAG);
        _loadProgressTimer->removeFromParentAndCleanup(true);
        _progressBg->removeFromParentAndCleanup(true);
        _table->setVisible(true);
    }
}

#pragma mark- Pen select

void GameScene::touchPensLayer(Sprite* pen)
{
    if (choosePattern)
    {
        //pattern layer
        PatternPenSprite *patternPen = dynamic_cast<PatternPenSprite*>(pen);
        if (patternPen->isLoced())
        {
            if (!_isPurchasing)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                if (!getChildByTag((int)TAGS::PARENTAL_GATE_TAG))
                {
                    ParentalGateLayer *layer = ParentalGateLayer::create();
                    layer->setTag((int)TAGS::PARENTAL_GATE_TAG);
                    layer->setCallback([=](){
                        
                        _appBilling.setIABDelegate(this);
                        _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
                        _isPurchasing = true;
                    });
                    layer->showInLayer(this);
                }
#else
                _appBilling.setIABDelegate(this);
                _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
                _isPurchasing = true;
#endif
            }
            return;
        }
        
        if (_selectPatternPen)
            _selectPatternPen->restoreWithAnimation(true);
        selectPatternPen(patternPen);
    }
    else
    {
        //pens1 and pens2 layer
        ColorPenSprite *colorPen = dynamic_cast<ColorPenSprite*>(pen);
        
        if (_selectColorPen)
            _selectColorPen->restoreWithAnimation(true);
      //colorPen->showWithAnimation(true);
        selectColorPen(colorPen);
    }
}

void GameScene::touchPensLayer(Touch* touch)
{
    //touch in pens layer
    Point point = _currentVisbleLayer->convertToNodeSpace(touch->getLocation());
    Sprite *pen = nullptr;
    bool find = false;
    for (int i = 0; i < _list->getChildren().size(); ++i)
    {
        pen = dynamic_cast<Sprite*>(_list->getChildren().at(i));
        if (pen && pen->getBoundingBox().containsPoint(point))
        {
            find = true;
            break;
        }
    }
    
    if (!find)
        return;
    
    touchPensLayer(pen);
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
    if(_preLayout!= NULL){
        _preLayout->setPosition(_preLayout->getPosition() + Vec2(20,0));
        _preLayout = NULL;
    }
    
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

    _currentVisbleLayer = _pensLayer;
    
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
    _currentVisbleLayer = _pensLayer;

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
    auto scale = _colouringLayer->getScale();
    auto pos = _colouringLayer->getPosition();
    auto anchor = _colouringLayer->getAnchorPoint();
    
    _colouringLayer->setScale(1);
    _colouringLayer->setPosition(Vec2::ZERO);
    _colouringLayer->setAnchorPoint(Vec2::ZERO);
    
    
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
    
    _colouringLayer->setScale(scale);
    _colouringLayer->setPosition(pos);
    _colouringLayer->setAnchorPoint(anchor);
}

void GameScene::savingActionForEnterBackground()
{
    auto scale = _colouringLayer->getScale();
    auto pos = _colouringLayer->getPosition();
    auto anchor = _colouringLayer->getAnchorPoint();
    
    _colouringLayer->setScale(1);
    _colouringLayer->setPosition(Vec2::ZERO);
    _colouringLayer->setAnchorPoint(Vec2::ZERO);
    
    generateCurrentPaintingPicture(Tmp_PictureName);
    
    /* save the picture logo, include wireframe */
    std::string path = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex+1, _pictureIndex, true);
    _pictureImage->saveToFile(path.c_str());
    
    log("MX: save picture logo success: %s!", path.c_str());
    
    /* save the picture history, just color mask, not include wireframe */
    std::string picturePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, _pictureIndex, false);
    _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
    
    log("MX: save picture history success: %s!", picturePath.c_str());
    
    _colouringLayer->setScale(scale);
    _colouringLayer->setPosition(pos);
    _colouringLayer->setAnchorPoint(anchor);
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
    AppManager::getInstance()->hideBannerAd();

    auto scene = PictureScene::createSceneWithPackIndex(_packIndex);
    Director::getInstance()->replaceScene(scene);
}

void GameScene::finishPaintingSave()
{
    AppManager::getInstance()->hideBannerAd();

    STSystemFunction function = STSystemFunction();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    function.saveLocalImage(_pictureImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string writePath = function.getSdCardPath()+"/Secret Fantasy/";
    
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
    //AppManager::getInstance()->requestFullScreenAd();
    
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

void GameScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void GameScene::switchMode(BaseMode::Mode mode)
{
    if (_currentMode)
    {
        _currentMode->stop();
    }
    
    switch (mode) {
        case BaseMode::Mode::NONE:
        {
            log("change to none");
            _currentMode = _noneMode;
        }
            break;
        case BaseMode::Mode::PAINT:
        {
            _currentMode = _paintMode;
            log("change to paint");
        }
            break;
        case BaseMode::Mode::WAIT:
        {
            log("change to wait");
            _currentMode = _waitMode;
        }
            break;
        case BaseMode::Mode::ZOOM:
        {
            log("change to zoom");
            _currentMode = _zoomMode;
        }
            break;
        case BaseMode::Mode::MOVE:
        {
            log("change to move");
            _currentMode = _moveMode;
        }
            break;
        default:
            break;
    }

    _currentMode->start();
}

void GameScene::paint(Vec2 & current, Vec2 & previous)
{
    _pictureScribble->paint(current, previous);
}

void GameScene::paintPrevious()
{
    for (int i = 0; i < _paints.size(); i ++) {
        Paint paint = _paints.at(i);
        
        if (i == 0) {
            _pictureScribble->paint(paint.pos);
        }
        else
        {
            _pictureScribble->paint(paint.pos, paint.previousPos);
        }
    }
    
    _paints.clear();
}

void GameScene::changeColor()
{
    if (_isPencil) {
        std::vector<Pen> *pens1 = ColouringManager::getInstance()->getPens1();
        int index = rand()%(pens1->size());
        while (index==preIndex) {
            index = rand()%(pens1->size());
        }
        preIndex = index;
        Sprite* pencil = dynamic_cast<Sprite*>(_list->getChildren().at(index)->getChildren().at(0));
        ColorPenSprite* colorPen = dynamic_cast<ColorPenSprite*>(pencil);
        selectColorPen(colorPen);
    }
}

Color4F GameScene::getTouchColor(Touch * touch)
{
    Vec2 touchInPicture = _picture->convertToNodeSpace(touch->getLocation());
    Color4B color = ScribbleUtil::getInstance()->getColor(_picturePixelMask, touchInPicture);
    
    return std::move(Color4F(color));
}

Layer * GameScene::getColouringLayer()
{
    return _colouringLayer;
}

void GameScene::setBrushScale(float scale)
{
    scale = 1/scale;
    
    if (scale > 0.95) {
        _pictureScribble->useBrush(RES_BRUSH);
    }
    else if (scale > 0.85)
    {
        _pictureScribble->useBrush("brush_0.9.png");
    }
    else if (scale > 0.75)
    {
        _pictureScribble->useBrush("brush_0.8.png");
    }
    else if (scale > 0.65)
    {
        _pictureScribble->useBrush("brush_0.7.png");
    }
    else if (scale > 0.55)
    {
        _pictureScribble->useBrush("brush_0.6.png");
    }
    else if (scale > 0.45)
    {
        _pictureScribble->useBrush("brush_0.5.png");
    }
    else if (scale > 0.35)
    {
        _pictureScribble->useBrush("brush_0.4.png");
    }
    else
    {
        _pictureScribble->useBrush("brush_0.33.png");
    }
}

CB_END_NAMESPACE
