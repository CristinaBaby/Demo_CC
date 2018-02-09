//
//  ColorBookScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "ColorBookScene.h"

#include "MiniGameSelectScene.h"
#include "HomeScene.h"
#include "ChoosePageScene.h"
#include "ColorPenSprite.h"
#include "PatternPenSprite.h"
#include "ScribbleUtil.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace extension;

ColorBookScene::ColorBookScene():
_loadProgressTimer(nullptr),
_pensContainerLayer(nullptr),
_pens1Layer(nullptr),
_pens2Layer(nullptr),
_currentVisbleLayer(nullptr),
_patternLayer(nullptr),
_colouringLayer(nullptr),
_selectColorPen(nullptr),
_selectPatternPen(nullptr),
_picture(nullptr),
_pictureScribble(nullptr),
_picturePixelMask(nullptr),
_finishLoadPicture(false),
_finishLoadMask(false),
_finishLoadHistory(false),
_gameLevel(0),
_pensListView(nullptr),
_patternsListView(nullptr),
_isColouring(false)
{
    
}

ColorBookScene::~ColorBookScene()
{
    
}

#pragma mark- Init methods

bool ColorBookScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("colorbook.json");
    addChild(_rootLayout, 1);
    
    //ui
    setupUI();
    
    //scribble
    setupScribble();
    
    //register touch listener
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(ColorBookScene::onTouchBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(ColorBookScene::onTouchMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(ColorBookScene::onTouchEnded, this);
    touchEventListener->onTouchCancelled = CC_CALLBACK_2(ColorBookScene::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    return true;
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

bool ColorBookScene::setupUI()
{
    bool rcode = false;
    do {
        auto colorLayerBg = LayerColor::create(Color4B::WHITE);
        addChild(colorLayerBg);
        
        //setup pens listview
        _pensListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(_ui, "ListView_Pens"));
        _pensListView->setContentSize(Size(_pensListView->getContentSize().width - 70, _pensListView->getContentSize().height));
        _pensListView->setGravity(ListView::Gravity::RIGHT);
        _pensListView->setBounceEnabled(true);
        _pensListView->addEventListener([=](Ref* item, ListView::EventType type){
            
            if (type == ListView::EventType::ON_SELECTED_ITEM_END)
            {
                selectColorPen(_pensListView->getCurSelectedIndex());
            }
        });

        for (auto penData : *MiniGameManager::getInstance()->getPens())
        {
            auto penSprite = ColorPenSprite::createWithPenData(&penData);
            penSprite->setTouchEnabled(true);
            _pensListView->pushBackCustomItem(penSprite);
        }
        
        //setup patterns listview
        _patternsListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(_ui, "ListView_Patterns"));
        _patternsListView->setContentSize(Size(_patternsListView->getContentSize().width - 70, _patternsListView->getContentSize().height));
        _patternsListView->setGravity(ListView::Gravity::RIGHT);
        _patternsListView->setBounceEnabled(true);
        _patternsListView->addEventListener([=](Ref* item, ListView::EventType type){
            
            if (type == ListView::EventType::ON_SELECTED_ITEM_END)
            {
                selectPatternPen(_patternsListView->getCurSelectedIndex());
            }
        });
        
        for (auto patternData : *MiniGameManager::getInstance()->getPatterns())
        {
            auto patternSprite = PatternPenSprite::createWithPatternData(&patternData);
            patternSprite->setTouchEnabled(true);
            _patternsListView->pushBackCustomItem(patternSprite);
        }
        _patternsListView->setVisible(false);
        
        //setup buttons
        _penButton = dynamic_cast<Button*>(Helper::seekWidgetByName(_ui, "Button_4"));
        _penButton->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::penButtonAction, this));
        
        _patternButton = dynamic_cast<Button*>(Helper::seekWidgetByName(_ui, "Button_4_0_1"));
        _patternButton->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::patternButtonAction, this));
        
        _deleteButton = dynamic_cast<Button*>(Helper::seekWidgetByName(_ui, "Button_4_0"));
        _deleteButton->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::deleteButtonAction, this));
        
        _eraserButton = dynamic_cast<Button*>(Helper::seekWidgetByName(_ui, "Button_3"));
        _eraserButton->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::eraserButtonAction, this));
        
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
        
        //create loading layer
        auto layer = LayerColor::create(Color4B(0, 0, 0, 200));
        addChild(layer, 100);
        //        auto loadingBg = Sprite::create("");
        //        loadingBg->setPosition(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT/2);
        //        layer->addChild(loadingBg);
        
        //progress bg
        auto progressBg = Sprite::create("ui09_saving_progress_1.png");
        progressBg->setPosition(getContentSize()/2);
        layer->addChild(progressBg);
        
        _loadProgressTimer = ProgressTimer::create(Sprite::create("ui09_saving_progress_2.png"));
        _loadProgressTimer->setTag(123);
        _loadProgressTimer->setType(ProgressTimer::Type::BAR);
        _loadProgressTimer->setMidpoint(Vec2(0, 1));
        _loadProgressTimer->setBarChangeRate(Vec2(1, 0));
        _loadProgressTimer->setPosition(Vec2(progressBg->getPosition()));
        _loadProgressTimer->runAction(ProgressFromTo::create(2, 0, 90));
        layer->addChild(_loadProgressTimer);
        
        //add colouring layer
        _colouringLayer = Layer::create();
        addChild(_colouringLayer);
        
        //pack info
        auto colorBookData = MiniGameManager::getInstance()->getColorBookDataVector().at(_gameLevel);
        //auto pictureHistoryPath = PackManager::getInstance()->getPackHistoryPicturePath(packIndex, pictureIndex, true);
        //mask path
        auto picturePath = colorBookData->pictureFileName;
        auto pictureMaskPath = colorBookData->maskFileName;
        
        /*-----DEBUG----*/
        //        _picturePath = "pictures/Pack_1_1_picture.png";
        //        _pictureMaskPath = "pictures/Pack_1_1_mask.png";
        //_pictureHistoryPath= "adsfsadfas";
        
        Director::getInstance()->getTextureCache()->addImageAsync(picturePath, [=](Texture2D *texture){
            
            //create front picture
            _picture = Sprite::create(picturePath);
            _picture->setPosition(getContentSize()/2);
            
            //create picture scribble
            _pictureScribble = ScribblePatternNode::create(Director::getInstance()->getWinSize());//_picture->getContentSize()
            _pictureScribble->setPosition(getContentSize()/2);
            _colouringLayer->addChild(_pictureScribble);
            _colouringLayer->addChild(_picture);//in front of scribble
            
            _finishLoadPicture = true;
            if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
            {
                _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([=](){
                    
                    removeChildByTag(123);
                    layer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });
        //
        //        //check sandbox history
        //        bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
        //        if (isExsit)
        //        {
        //            Director::getInstance()->getTextureCache()->addImageAsync(_pictureHistoryPath, [this](Texture2D *texture){
        //
        //                _pictureScribble->setPattern(_pictureHistoryPath);
        //
        //                Sprite *history = Sprite::create(_pictureHistoryPath);
        //                history->setPosition(Utils::getInstance()->getScreenCenter());
        //
        //                _pictureScribble->getCanvas()->begin();
        //                history->visit();
        //                _pictureScribble->getCanvas()->end();
        //
        //                _finishLoadHistory = true;
        //                if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
        //                {
        //                    _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([this](){
        //
        //                        removeChildByTag((int)TAGS::PROGRESS_TAG);
        //                        _loadingLayer->removeFromParentAndCleanup(true);
        //                    }), NULL));
        //                }
        //            });
        //        }
        //        else
        //        {
        //            _finishLoadHistory = true;
        //        }
        
        _finishLoadHistory = true;
        Director::getInstance()->getTextureCache()->addImageAsync(pictureMaskPath, [=](Texture2D *texture){
            
            //others
            _pictureScribble->setPattern(pictureMaskPath);
            _pictureScribble->useBrush("solid_brush.png");
            _pictureScribble->useTarget(Sprite::create(pictureMaskPath));
            
            CC_SAFE_DELETE(_picturePixelMask);
            _picturePixelMask = new Image();
            _picturePixelMask->initWithImageFile(pictureMaskPath);
            
            _finishLoadMask = true;
            if (_finishLoadHistory && _finishLoadMask && _finishLoadPicture)
            {
                _loadProgressTimer->runAction(Sequence::create(ProgressFromTo::create(0.5, _loadProgressTimer->getPercentage(), 100), CallFunc::create([=](){
                    
                    removeChildByTag(123);
                    layer->removeFromParentAndCleanup(true);
                }), NULL));
            }
        });
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Select Pen

void ColorBookScene::selectColorPen(const int index)
{
    auto penSprite = dynamic_cast<ColorPenSprite*>(_pensListView->getItem(index));
    if (_selectColorPen)
        _selectColorPen->restoreWithAnimation(true);
    _selectColorPen = penSprite;
    _selectColorPen->showWithAnimation(true);
    
    Pen *pen = _selectColorPen->getColorPenData();
    Color4F color((float)pen->R/255.0,
                  (float)pen->G/255.0,
                  (float)pen->B/255.0,
                  1.0);
    _pictureScribble->useTarget(color);
}

void ColorBookScene::selectPatternPen(const int index)
{
    auto patternSprite = dynamic_cast<PatternPenSprite*>(_patternsListView->getItem(index));
    if (_selectPatternPen)
        _selectPatternPen->restoreWithAnimation(true);
    _selectPatternPen = patternSprite;
    _selectPatternPen->showWithAnimation(true);
}

#pragma mark- Button Action methods

void ColorBookScene::penButtonAction(Ref* ref, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (_pensListView->isVisible()) {
            return;
        }
        
        _pensListView->setVisible(true);
        _patternsListView->setVisible(false);
        
        if (_selectPatternPen)
        {
            _selectPatternPen->restoreWithAnimation(false);
            _selectPatternPen = nullptr;
        }
        
        _penButton->loadTextureNormal("ui09_btn_brush_h.png");
        _patternButton->loadTextureNormal("ui09_btn_pattern.png");
    }
}

void ColorBookScene::patternButtonAction(Ref* ref, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (_patternsListView->isVisible()) {
            return;
        }
        
        _pensListView->setVisible(false);
        _patternsListView->setVisible(true);
        
        if (_selectColorPen)
        {
            _selectColorPen->restoreWithAnimation(false);
            _selectColorPen = nullptr;
        }

        _penButton->loadTextureNormal("ui09_btn_brush.png");
        _patternButton->loadTextureNormal("ui09_btn_pattern_h.png");
    }
}

void ColorBookScene::deleteButtonAction(Ref* ref, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void ColorBookScene::eraserButtonAction(Ref* ref, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

#pragma mark- Touch Actions

bool ColorBookScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Point touchPoint = convertToWorldSpace(touch->getLocation());
    Vec2 touchInPicture = _picture->convertToNodeSpace(touchPoint);
    Color4B color = ScribbleUtil::getColor(_picturePixelMask, touchInPicture);
    
    if (_selectColorPen || _selectPatternPen)
    {
        _isColouring = true;
        
        _pictureScribble->setPatternColor(Color4F(color));
        _pictureScribble->paint(touchPoint);
    }
    
    return true;
}

void ColorBookScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (_isColouring && (_selectColorPen || _selectPatternPen))
    {
        Point touchPoint = convertToWorldSpace(touch->getLocation());
        Point previousTouchPoint = convertToWorldSpace(touch->getPreviousLocation());
        _pictureScribble->paint(touchPoint, previousTouchPoint);
    }
}

void ColorBookScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    _isColouring = false;
}

void ColorBookScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    _isColouring = false;
}

#pragma mark- TableView delegate methods

ssize_t ColorBookScene::numberOfCellsInTableView(TableView *table)
{
    return 0;
}

void ColorBookScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size ColorBookScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size::ZERO;
}

TableViewCell* ColorBookScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = TableViewCell::create();
    }
    
    cell->removeAllChildren();

    
    return cell;
}

void ColorBookScene::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}









