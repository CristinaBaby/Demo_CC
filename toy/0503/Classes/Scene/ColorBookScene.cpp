//
//  ColorBookScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "ColorBookScene.h"

#include "STVisibleRect.h"
#include "MiniGameManager.h"
#include "../Model/v2.0/ScribbleUtil.h"
#include "AppManager.h"
#include "STSystemFunction.h"
#include "ChoosePageScene.h"

USING_NS_CC;
USING_NS_ST;

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
    PARENTAL_GATE_TAG = 125
};

/* used to identifier tmp picture which saved by user */
#define Tmp_PictureName "tmp_picture.png"

#define RES_TAB                 "bg_2.png"
#define RES_TAB_MASK            "bg_1.png"
#define RES_LOADING_BG          "ui05_saving_box.png"
#define RES_PROGRESS_BG         "ui09_saving_progress_1.png"
#define RES_PROGRESS            "ui09_saving_progress_2.png"
#define RES_BRUSH               "solid_brush.png"

#define RES_PENS1_BUTTON        "crayon.png"
#define RES_PENS1_BUTTON_H      "crayon_4.png"

#define RES_PENS2_BUTTON        "crayon_2.png"
#define RES_PENS2_BUTTON_H      "crayon_5.png"

#define RES_PATTERN_BUTTON      "crayon-5.png"
#define RES_PATTERN_BUTTON_H    "crayon_3.png"

#define RES_REFRESH_BUTTON      "refesh2.png"
#define RES_REFRESH_BUTTON_H    "refesh_3.png"

#define RES_ERASER_BUTTON       "eraser.png"

#define RES_BACK_BUTTON         "back.png"

ColorBookScene::~ColorBookScene()
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

bool ColorBookScene::init()
{
    if (! Layer::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        //setup ui
        setupUI();
        
        //add scribble
        //setupScribble();
        
        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(ColorBookScene::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(ColorBookScene::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(ColorBookScene::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(ColorBookScene::onTouchCancelled, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        pens1Action();
        
        //iap delegate
        _iap.setIABDelegate(this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Game

void ColorBookScene::startGameWithGameLevel(const int level)
{
    CCASSERT(MiniGameManager::getInstance()->getColorBookDataVector().size() > level, "Invalid game level");
    
    //store current game level
    _gameLevel = level;
    
    //setup jigsaw tiles
    setupScribble();
}

#pragma mark- Setup UI

void ColorBookScene::showPurchaseDialog()
{
    auto maskLayer = Utils::createMaskLayer(200.0);
    addChild(maskLayer, 200);
    
    AppManager::getInstance()->setBannerAdVisible(true);
    
    auto dialogBg = Sprite::create("popup.png");
    dialogBg->setPosition(maskLayer->getContentSize()/2);
    dialogBg->setScale(0.0);
    maskLayer->addChild(dialogBg);
    dialogBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
        
        
    }), NULL));
    
    auto text = Sprite::create("text(3).png");
    text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    text->setPosition(Vec2(70.0, 187.0));
    dialogBg->addChild(text);
    
    auto yesButton = Button::create("buy.png");
    yesButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    yesButton->setPosition(Vec2(110.0, 52.0));
    yesButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
            
            _iap.purchase(IAP_UNLOCK_All);
        }
    });
    dialogBg->addChild(yesButton);
    
    auto noButton = Button::create("later.png");
    noButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    noButton->setPosition(Vec2(320.0, 52.0));
    noButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
        }
    });
    dialogBg->addChild(noButton);
}

bool ColorBookScene::setupUI()
{
    bool rcode = false;
    do {
        
        auto colorLayerBg = LayerColor::create(Color4B::WHITE);
        addChild(colorLayerBg);
        
        //setting
        auto backButton = Button::create(RES_BACK_BUTTON);
        backButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        STVisibleRect::setPosition(backButton, Vec2(20, 620), true, false, true, false);
        backButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
            
            if (eventType == Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playEffect(SOUND_POPUB);

                auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 200.0));
                auto eventListener = EventListenerTouchOneByOne::create();
                eventListener->setSwallowTouches(true);
                eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
                   
                    return true;
                };
                Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
                addChild(layer, 110);
                
                auto ui = GUIReader::getInstance()->widgetFromJsonFile("save_dialog(2).json");
                ui->setAnchorPoint(Point::ANCHOR_MIDDLE);
                ui->setPosition(getContentSize()/2);
                layer->addChild(ui, 1);
            
                _isSaving = true;

                CC_SAFE_DELETE(_pictureImage);
                generateCurrentPaintingPicture(Tmp_PictureName);
                auto texture = new Texture2D();
                texture->initWithImage(_pictureImage);
                auto sprite = Sprite::createWithTexture(texture);
                float scalex = 282.0 / sprite->getContentSize().width;
                float scaley = 205.0 / sprite->getContentSize().height;
                float maxscale = scalex > scaley ? scalex : scaley;
                sprite->setScale(maxscale);
                texture->autorelease();
                
                auto clipping = ClippingNode::create();
                clipping->setStencil(Sprite::create("picture-1.png"));
                clipping->addChild(sprite);
                clipping->setAlphaThreshold(0.01);

                auto frame = Sprite::create("picture.png");
                
                frame->addChild(clipping);
                clipping->setPosition(frame->getContentSize()/2);
                
                frame->setPosition(ui->getContentSize()/2);
                ui->addChild(frame, 2);
                
                saveHistory();
                
                AppManager::getInstance()->setBannerAdVisible(true);
                ui->setScale(0.1);
                ui->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7));
                
                Utils::addEventToButton(ui, "close_button", Widget::TouchEventType::ENDED, [=](){
                    
                    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
                    AppManager::getInstance()->setBannerAdVisible(false);

                    layer->removeFromParent();
                });
                
                Utils::addEventToButton(ui, "next_button", Widget::TouchEventType::ENDED, [=](){
                    
                    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
                    AppManager::getInstance()->setBannerAdVisible(false);

                    int level = _gameLevel + 1;
                    if (MiniGameManager::getInstance()->getColorBookDataVector().size() <= level)
                    {
                        level = 0;//如果已经到达最后一张，循环回到第一张
                    }

                    if (level >= 15 && !AppManager::getInstance()->getHasUnlockAll())
                    {
                        showPurchaseDialog();
                        return;
                    }
                    
                    restorePensButtons();
                    MiniGameManager::getInstance()->setCurrentPensType(PensType::PensType_None);
                    pens1Action();
                    
                    _pictureScribble->resetCanvas();
                    
                    startGameWithGameLevel(level);
                    
                    layer->removeFromParent();
                });
                
                Utils::addEventToButton(ui, "photo_button", Widget::TouchEventType::ENDED, [=](){
                    
                    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
                    AppManager::getInstance()->setBannerAdVisible(false);

                    auto system = STSystemFunction();
                    system.saveLocalImage(_pictureImage);
                });
                
                Utils::addEventToButton(ui, "level_button", Widget::TouchEventType::ENDED, [=](){
                    
                    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
                    AppManager::getInstance()->setBannerAdVisible(false);

                    auto scene = BaseScene::createScene<ChoosePageScene>();
                    ChoosePageScene::needShowFullScreen = true;
                    auto fadeTransition = TransitionFade::create(0.3, scene);
                    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
                    layer->setupGameType(GameType::ColorBook);
                    
                    Director::getInstance()->replaceScene(fadeTransition);
                });
            }
        });
        addChild(backButton, (int)ZORDERS::SETTING_ZORDER);
        
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
        STVisibleRect::setPosition(_pens1Button, 730, 527);
        _pensContainerLayer->addChild(_pens1Button);
        
        //pens2 button
        _pens2Button = Sprite::create(RES_PENS2_BUTTON);
        _pens2Button->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_pens2Button, 730, 430);
        _pensContainerLayer->addChild(_pens2Button);
        
        //pattern button
        _patternButton = Sprite::create(RES_PATTERN_BUTTON);
        _patternButton->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_patternButton, 730, 340);
        _pensContainerLayer->addChild(_patternButton);
        
        //refresh button
        _refreshButton = Sprite::create(RES_REFRESH_BUTTON);
        _refreshButton->setAnchorPoint(Point::ZERO);
        STVisibleRect::setPosition(_refreshButton, 730, 250);
        _pensContainerLayer->addChild(_refreshButton);
        
        //tab mask
        Sprite *tabMask = Sprite::create(RES_TAB_MASK);
        CC_BREAK_IF(!tabMask);
        tabMask->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(tabMask, 775, 320);
        _pensContainerLayer->addChild(tabMask);
        
        //pens1 layer
        _pens1Layer = Layer::create();
        _pensContainerLayer->addChild(_pens1Layer);
        _currentVisbleLayer = _pens1Layer;
        
        std::vector<Pen> *pens = MiniGameManager::getInstance()->getPens();
        for (int index = 0;index < 12; ++index)
        {
            Pen *pen = &pens->at(index);
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
        
        for (int index = 12;index < 24; ++index)
        {
            Pen *pen = &pens->at(index);
            ColorPenSprite *penSprite = ColorPenSprite::createWithPenData(pen);
            penSprite->setTag(index - 12);
            penSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            STVisibleRect::setPosition(penSprite, 825, 640 - (50 + (index - 12) * (39/*sprite height*/ + 5/*pens interval*/)));
            _pens2Layer->addChild(penSprite);
        }
        
        //pattern layer
        _patternLayer = Layer::create();
        _pensContainerLayer->addChild(_patternLayer);
        _patternLayer->setVisible(false);
        
        std::vector<Pattern> *patterns = MiniGameManager::getInstance()->getPatterns();
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
        _eraserButton = BasePenSprite::create();
        _eraserButton->loadTexture(RES_ERASER_BUTTON);
        CC_BREAK_IF(!_eraserButton);
        _eraserButton->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        STVisibleRect::setPosition(_eraserButton, 845, 48);
        _pensContainerLayer->addChild(_eraserButton);
        
        return true;
    } while (0);
    
    return rcode;
}

#pragma mark- Setup scribble

bool ColorBookScene::setupScribble()
{
    bool rcode = false;
    
    do {
        _finishLoadPicture = false;
        _finishLoadMask = false;
        _finishLoadHistory = false;
        
        auto layer = LayerColor::create(Color4B(0, 0, 0, 200));
        addChild(layer, 100);
        //        auto loadingBg = Sprite::create("");
        //        loadingBg->setPosition(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT/2);
        //        layer->addChild(loadingBg);
        
        //progress bg
        auto progressBg = Sprite::create("progress_bg.png");
        progressBg->setPosition(getContentSize()/2);
        layer->addChild(progressBg);
        
        auto loadProgressTimer = ProgressTimer::create(Sprite::create("progress_bar.png"));
        loadProgressTimer->setType(ProgressTimer::Type::BAR);
        loadProgressTimer->setMidpoint(Vec2(0, 1));
        loadProgressTimer->setBarChangeRate(Vec2(1, 0));
        loadProgressTimer->setPosition(Vec2(progressBg->getPosition()));
        loadProgressTimer->runAction(ProgressFromTo::create(2, 0, 90));
        layer->addChild(loadProgressTimer);
        
        //add colouring layer
        if (!_colouringLayer)
        {
            _colouringLayer = Layer::create();
            addChild(_colouringLayer);
        }
        
        //pack info
        auto colorBookData = MiniGameManager::getInstance()->getColorBookDataVector().at(_gameLevel);
        
        //index
        //auto pictureHistoryPath = PackManager::getInstance()->getPackHistoryPicturePath(packIndex, pictureIndex, true);
        //mask path
        Director::getInstance()->getTextureCache()->removeTextureForKey(_picturePath);
        Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureMaskPath);
        Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureHistoryPath);

        _picturePath = colorBookData->pictureFileName;
        _pictureMaskPath = colorBookData->maskFileName;
        _pictureHistoryPath = FileUtils::getInstance()->getWritablePath() + std::to_string(_gameLevel) + "_mask.png";

        Director::getInstance()->getTextureCache()->addImageAsync(_picturePath, [=](Texture2D *texture){
            
            //create front picture
            if (!_picture)
            {
                _picture = Sprite::create();
                _picture->setPosition(getContentSize()/2);
            }
            
            _picture->setTexture(_picturePath);
            
            if (!_pictureScribble)
            {
                //create picture scribble
                _pictureScribble = ScribblePatternNode::create(Director::getInstance()->getWinSize());//_picture->getContentSize()
                _pictureScribble->setPosition(getContentSize()/2);
                
                _colouringLayer->addChild(_pictureScribble);
                _colouringLayer->addChild(_picture);//in front of scribble
            }

            _finishLoadPicture = true;
            if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
            {
                loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, loadProgressTimer->getPercentage(), 100), CallFunc::create([=](){
                    
                    layer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });
        
        //check sandbox history
        bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
        if (isExsit)
        {
            Director::getInstance()->getTextureCache()->addImageAsync(_pictureHistoryPath, [=](Texture2D *texture){
                
                _pictureScribble->setPattern(_pictureHistoryPath);
                
                Sprite *history = Sprite::create(_pictureHistoryPath);
                history->setPosition(getContentSize()/2);
                
                _pictureScribble->getCanvas()->begin();
                history->visit();
                _pictureScribble->getCanvas()->end();
                
                _finishLoadHistory = true;
                if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
                {
                    loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, loadProgressTimer->getPercentage(), 100), CallFunc::create([=](){
                        
                        layer->removeFromParentAndCleanup(true);
                    }), NULL));
                }
            });
        }
        else
        {
            _finishLoadHistory = true;
        }
        
        Director::getInstance()->getTextureCache()->addImageAsync(_pictureMaskPath, [=](Texture2D *texture){
            
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
                loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, loadProgressTimer->getPercentage(), 100), CallFunc::create([=](){
                    
                    layer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Touch events

bool ColorBookScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    /* -----IMPORTANT---- */
    if (_isSaving)
    {
        _isSaving = false;
        CC_SAFE_DELETE(_pictureImage);
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
        else if (_refreshButton->getBoundingBox().containsPoint(point))
        {
            refreshAction();
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

void ColorBookScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (_isColouring && (_selectColorPen || _selectPatternPen || _eraserButton->isShow()))
    {
        AppManager::getInstance()->playLoopEffect(SOUND_COLOR_BRUSHING);

        Point touchPoint = convertToWorldSpace(touch->getLocation());
        Point previousTouchPoint = this->convertToWorldSpace(touch->getPreviousLocation());
        _pictureScribble->paint(touchPoint, previousTouchPoint);
    }
    else
    {
        Point point = convertToNodeSpace(touch->getLocation());
        
        if(_eraserButton->getBoundingBox().containsPoint(point))
        {
            eraserAction();
            return;
        }
        
        //touch pens layer
        touchPensLayer(touch);
    }
}

void ColorBookScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    AppManager::getInstance()->stopLoopEffect(SOUND_COLOR_BRUSHING);

    if (_isColouring)
    {

    }
    else
    {

    }
}

void ColorBookScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    AppManager::getInstance()->stopLoopEffect(SOUND_COLOR_BRUSHING);

}

#pragma mark- Pen select

void ColorBookScene::touchPensLayer(Touch *touch)
{
    //touch in pens layer
    Point point = _currentVisbleLayer->convertToNodeSpace(touch->getLocation());
    Node *pen = nullptr;
    bool find = false;
    for (int i = 0; i < _currentVisbleLayer->getChildren().size(); ++i)
    {
        pen = _currentVisbleLayer->getChildByTag(i);
        if (pen && pen->getBoundingBox().containsPoint(point))
        {
            find = true;
            break;
        }
    }
    
    if (!find)
        return;
    
    if (MiniGameManager::getInstance()->getCurrentPensType() == PensType::PensType_Pattern)
    {
        //pattern layer
        PatternPenSprite *patternPen = dynamic_cast<PatternPenSprite*>(pen);
//        if (patternPen->isLoced())
//        {
//            if (!_isPurchasing)
//            {
//                _isPurchasing = true;
//                _appBilling.setIABDelegate(this);
//                _appBilling.purchase(IAP_UNLOCK_PATTERN_PEN);
//            }
//            return;
//        }
        
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

void ColorBookScene::selectColorPen(ColorPenSprite *colorPen)
{
    _selectColorPen = colorPen;
    
    Pen *pen = _selectColorPen->getColorPenData();
    Color4F color((float)pen->R/255.0,
                  (float)pen->G/255.0,
                  (float)pen->B/255.0,
                  (float)pen->A/255.0);
    _pictureScribble->useTarget(color);
}

void ColorBookScene::selectPatternPen(PatternPenSprite *patternPen)
{
    _selectPatternPen = patternPen;
    Pattern *pattern = patternPen->getPatternPenData();
    
    Sprite *patternSprite = Sprite::create(pattern->pattern);
    
    if (patternSprite)
        _pictureScribble->useTarget(patternSprite);
}

#pragma mark- Action methods

void ColorBookScene::restorePensButtons()
{
    _pens1Button->setTexture(RES_PENS1_BUTTON);
    _pens2Button->setTexture(RES_PENS2_BUTTON);
    _patternButton->setTexture(RES_PATTERN_BUTTON);
    _refreshButton->setTexture(RES_REFRESH_BUTTON);
    
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

void ColorBookScene::refreshAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    _pictureScribble->resetCanvas();
}

void ColorBookScene::eraserAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    if (MiniGameManager::getInstance()->getCurrentPensType() == PensType::PensType_Pattern)
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

void ColorBookScene::pens1Action()
{
    PensType currentType = MiniGameManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Pen1) {
        return;
    }
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    //restore all buttons
    restorePensButtons();
    
    //set select image
    _pens1Button->setTexture(RES_PENS1_BUTTON_H);
    
    //set pens1 layer visible
    _pens1Layer->setVisible(true);
    _currentVisbleLayer = _pens1Layer;
    
    //set current pens type
    MiniGameManager::getInstance()->setCurrentPensType(PensType::PensType_Pen1);
}

void ColorBookScene::pens2Action()
{
    PensType currentType = MiniGameManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Pen2) {
        return;
    }
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
    
    //restore all buttons
    restorePensButtons();
    
    //set select image
    _pens2Button->setTexture(RES_PENS2_BUTTON_H);
    
    //set pens1 layer visible
    _pens2Layer->setVisible(true);
    _currentVisbleLayer = _pens2Layer;
    
    //set current pens type
    MiniGameManager::getInstance()->setCurrentPensType(PensType::PensType_Pen2);
}

void ColorBookScene::patternAction()
{
    PensType currentType = MiniGameManager::getInstance()->getCurrentPensType();
    if (currentType == PensType::PensType_Pattern) {
        return;
    }
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    //restore all buttons
    restorePensButtons();
    
    //set select image
    _patternButton->setTexture(RES_PATTERN_BUTTON_H);
    
    //set pens1 layer visible
    _patternLayer->setVisible(true);
    _currentVisbleLayer = _patternLayer;
    
    //set current pens type
    MiniGameManager::getInstance()->setCurrentPensType(PensType::PensType_Pattern);
}

void ColorBookScene::settingAction()
{
//    AppManager::getInstance()->playButtonEffect();
//    SettingLayer *layer = SettingLayer::create();
//    addChild(layer, (int)ZORDERS::SETTING_LAYER_ZORDER);
}

void ColorBookScene::savingAction()
{
//    AppManager::getInstance()->playButtonEffect();
//    /* -----IMPORTANT---- */
//    if (_isSaving)
//    {
//        _isSaving = false;
//        CC_SAFE_DELETE(_pictureImage);
//    }
//    
//    generateCurrentPaintingPicture(Tmp_PictureName);
//    
//    log("MX: generate current painting picture success!");
//    
//    FinishPaintingLayer *layer = FinishPaintingLayer::create(_pictureImage);
//    
//    layer->setQuitCallback(CC_CALLBACK_0(ColorBookScene::finishPaintingQuit, this));
//    layer->setSaveCallback(CC_CALLBACK_0(ColorBookScene::finishPaintingSave, this));
//    addChild(layer, (int)ZORDERS::SAVING_LAYER_ZORDER);
//    
//    log("MX: finish painting layer create success!");
//    
//    /* save the picture logo, include wireframe */
//    std::string path = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex+1, _pictureIndex, true);
//    _pictureImage->saveToFile(path.c_str());
//    
//    log("MX: save picture logo success: %s!", path.c_str());
//    
//    /* save the picture history, just color mask, not include wireframe */
//    std::string picturePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, _pictureIndex, false);
//    _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
//    
//    log("MX: save picture history success: %s!", picturePath.c_str());
//    
//    _isSaving = true;
}

#pragma mark- Save history

void ColorBookScene::saveHistory()
{
    /* save the picture logo, include wireframe */
    std::string path = std::to_string(_gameLevel) + ".png";
    float scalex = 192.0 / _pictureImage->getWidth();
    float scaley = 176.0 / _pictureImage->getHeight();
    float maxscale = scalex > scaley ? scalex : scaley;
    auto texture = new Texture2D();
    texture->initWithImage(_pictureImage);
    auto sprite = Sprite::createWithTexture(texture);
    sprite->setScale(maxscale);
    sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
    sprite->setPosition(80.0, 80.0);
    texture->release();
    
    auto stencil = Sprite::create("story_3.png");
    stencil->getTexture()->setAntiAliasTexParameters();
    stencil->setAnchorPoint(Point::ANCHOR_MIDDLE);
    stencil->setPosition(80.0, 80.0);
    
    auto clipping = ClippingNode::create();
    clipping->setStencil(stencil);
    clipping->addChild(sprite);
    clipping->setAlphaThreshold(0.0);
    clipping->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);

    auto renderTexture = RenderTexture::create(160.0, 160.0, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    renderTexture->begin();
    clipping->cocos2d::Node::visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    renderTexture->saveToFile(path);
    log("MX: save picture logo success: %s!", path.c_str());
    
    /* save the picture history, just color mask, not include wireframe */
    std::string maskPath = std::to_string(_gameLevel) + "_mask.png";
    _pictureScribble->getCanvas()->saveToFile(maskPath.c_str());
}

#pragma mark- Get painting picture

void ColorBookScene::generateCurrentPaintingPicture(const std::string& pictureName)
{
    auto renderTexture = RenderTexture::create(getContentSize().width - (310.0 - (1136.0 - getContentSize().width)/2.0), _colouringLayer->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _colouringLayer->visit();
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    _pictureImage = renderTexture->newImage();
    
    //issue001: android app will crash!
    /* remove last one */
    Director::getInstance()->getTextureCache()->removeTextureForKey(pictureName);

    /* used in saving layer */
    Director::getInstance()->getTextureCache()->addImage(_pictureImage, pictureName);
}

#pragma mark- Finish Painting Layer Call back

void ColorBookScene::finishPaintingQuit()
{
//    AppManager::getInstance()->setBannerAdVisible(false);
//    
//    auto scene = PictureScene::createSceneWithPackIndex(_packIndex);
//    Director::getInstance()->replaceScene(scene);
}

void ColorBookScene::finishPaintingSave()
{
//    AppManager::getInstance()->setBannerAdVisible(false);
//    
//    STSystemFunction function = STSystemFunction();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    function.saveLocalImage(_pictureImage);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    std::string writePath = function.getSdCardPath()+"/ColorBook/";
//    
//    if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
//    {
//        PackManager::getInstance()->createDirectory(writePath.c_str());
//        log("MX: android writeable path: %s", writePath.c_str());
//    }
//    
//    struct timeval now;
//    gettimeofday(&now, nullptr);
//    std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
//    std::string fullPath = writePath + _time + ".jpg";
//    
//    log("MX: %s", fullPath.c_str());
//    
//    bool isSaveSuccess = _pictureImage->saveToFile(fullPath.c_str());
//    if(isSaveSuccess)
//        function.makeToast("Save successfully!");
//    else
//        function.makeToast("Save failed!");
//    function.refreshDCIM(fullPath);
//#endif
//    
//    /* -----Ads Logic----- */
//    AppManager::getInstance()->requestFullScreenAd();
//    
//    /* -----Rate Us Logic----- */
//    AppManager::getInstance()->rateUsLogic();
}

#pragma mark- STIABDelegate

void ColorBookScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void ColorBookScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void ColorBookScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void ColorBookScene::restoreFailed(const char* pid, int errorCode)
{
    
}





