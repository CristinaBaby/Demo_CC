//
//  PaintScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#include "PaintScene.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "HomeScene.h"
#include "ChoosePaintScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/AppManager.h"
#include "../Model/PaintingManager.h"
#include "../Layer/ShopLayer.h"
#include "../Widget/ColorWidget.h"
#include "../Widget/PatternWidget.h"
#include "DressScene.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace extension;

#define RES_COCOSTUDIO_JSON          "ui03_print.json"

#define NAME_BUTTON_BACK             "ui08_btn_back"
#define NAME_BUTTON_SHOP             "ui08_btn_coins"
#define NAME_BUTTON_DONE             "ui08_btn_done"
#define NAME_BUTTON_ERASER           "ui03_eraser"

#define NAME_CHECKBOX_PEN            "ui03_color"
#define NAME_CHECKBOX_PATTERN        "ui03_patterns_h"
#define NAME_CHECKBOX_DRESS          "ui02_dress_h"
#define NAME_CHECKBOX_SHOES          "ui02_shoes_h"
#define NAME_LABEL_COIN              "ui08_btn_coins"

#define RES_BUTTON_ERASER            "ui03_eraser.png"
#define RES_BUTTON_ERASER_H          "ui03_eraser_h.png"
#define RES_BRUSH                    "solid_brush.png"

#define FONT_NAME                    "Helvetica_Rounded_LT_Black.ttf"
#define FONT_SIZE                    26

#define STRINGIFY(A)  #A
#include "../Model/ChangeToWhiteColor.frag"

PaintScene::PaintScene():
_selectColorPenWidget(nullptr),
_selectPatternPenWidget(nullptr),
_selectPenIndex(-1),
_selectPatternIndex(-1)
{}

PaintScene::~PaintScene()
{
    delete _resPicturePixelMask;
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* PaintScene::createScene(const Res& res)
{
    auto scene = Scene::create();
    auto layer = PaintScene::create(res);
    scene->addChild(layer);
    return scene;
}

PaintScene* PaintScene::create(const Res& res)
{
    PaintScene *layer = new (std::nothrow)(PaintScene);
    if (layer && layer->init(res))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool PaintScene::init(const Res& res)
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //store res
        _res = res;
  
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui);
        
        //back button
        auto backButton = Helper::seekWidgetByName(ui, NAME_BUTTON_BACK);
        backButton->setZOrder(100);
        backButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                backAction();
            }
        });
    
        auto doneButton = Helper::seekWidgetByName(ui, NAME_BUTTON_DONE);
        doneButton->setZOrder(100);
        doneButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                doneAction();
            }
        });
        
        //eraser button
        _eraserButton = dynamic_cast<Button *>(Helper::seekWidgetByName(ui, NAME_BUTTON_ERASER));
        _eraserButton->setZOrder(100);
        _eraserButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                eraserAction();
            }
        });

        auto penCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_PEN));
        auto patternCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_PATTERN));
        
        //default select pen check box
        penCheckBox->setSelectedState(true);
        penCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                patternCheckBox->setSelectedState(false);
                selectPenAction();
            }
            else
            {
                penCheckBox->setSelectedState(true);
            }
        });
        
        patternCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                penCheckBox->setSelectedState(false);
                selectPatternAction();
            }
            else
            {
                patternCheckBox->setSelectedState(true);
            }
        });

        //setup scribble
        setupScribble();
        
        //init tableview
        initTableView();

        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(PaintScene::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(PaintScene::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(PaintScene::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(PaintScene::onTouchCancelled, this);
  
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        //play bgm
        AppManager::getInstance()->playColorBgm();

        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Scribble

bool PaintScene::setupScribble()
{
    bool rcode = false;
    
    do {
        //add colouring layer
        _paintingLayer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 0.0));
        addChild(_paintingLayer);

        //mask path
        std::string resPicturePath;
        std::string resPictureMaskPath;
        if (_res.isPaintRes)
        {
            resPicturePath = ResManager::getInstance()->getResClothesPath(_res) + _res.resOriginalName + "picture.png";
            resPictureMaskPath = ResManager::getInstance()->getResClothesPath(_res) + _res.resOriginalName + "mask.png";
        }
        else
        {
            resPicturePath = _res.resPicturePath;
            resPictureMaskPath = _res.resMaskPath;
        }
        /*-----DEBUG----*/
//        _resPicturePath = "res/dress/L1_picture.png";
//        _resPictureMaskPath = "res/dress/L1_mask.png";
        //_pictureHistoryPath= "adsfsadfas";
        
        //create front picture
        _resPicture = Sprite::create(resPicturePath);
        _resPicture->setPosition(Vec2(getContentSize().width/2 - 100, getContentSize().height/2));
        
        //create picture scribble
        _pictureScribble = ScribblePatternNode::create(_resPicture->getContentSize());//_picture->getContentSize()
        _pictureScribble->setPosition(Vec2(getContentSize().width/2 - 100, getContentSize().height/2));
        _paintingLayer->addChild(_resPicture, 2);//in front of scribble
        _paintingLayer->addChild(_pictureScribble, 1);
        
        auto whiteMask = Sprite::create(resPictureMaskPath);
        whiteMask->setPosition(Vec2(getContentSize().width/2 - 100, getContentSize().height/2));
        GLProgram *p = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ChangeToWhiteColor_frag);
        CHECK_GL_ERROR_DEBUG();
        if (nullptr != p) {
            ShaderCache::getInstance()->addGLProgram(p, "ChangeToWhiteColor");
        }
        whiteMask->setGLProgram(p);
        _paintingLayer->addChild(whiteMask, 0);
        
        if (_res.isPaintRes)
        {
            std::string historyPath = _res.resHistoryPath;
            Director::getInstance()->getTextureCache()->removeTextureForKey(historyPath);
            _pictureScribble->setPattern(historyPath);
            
            Sprite *history = Sprite::create(historyPath);
            history->setPosition(Vec2(_pictureScribble->getContentSize().width/2, _pictureScribble->getContentSize().height/2));
            _pictureScribble->getCanvas()->begin();
            history->visit();
            _pictureScribble->getCanvas()->end();
        }
        
        //others
        _pictureScribble->setPattern(resPictureMaskPath);
        _pictureScribble->useBrush(RES_BRUSH);
        _pictureScribble->useTarget(Sprite::create(resPictureMaskPath));
        
        _resPicturePixelMask = new Image();
        _resPicturePixelMask->initWithImageFile(resPictureMaskPath);

        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- ui

void PaintScene::initTableView()
{
    //pen table view
    _penTableView = extension::TableView::create(this, Size(190, 600));
    namespaceST::STVisibleRect::setPosition(_penTableView, 445, 280, true, false, false, true);
    _penTableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    _penTableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    _penTableView->setDelegate(this);
    _penTableView->setDataSource(this);
    addChild(_penTableView, 200);
    _penTableView->reloadData();
    
    //pattern table view
    _patternTableView = extension::TableView::create(this, Size(190, 600));
    namespaceST::STVisibleRect::setPosition(_patternTableView, 445, 280, true, false, false, true);
    _patternTableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    _patternTableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    _patternTableView->setDelegate(this);
    _patternTableView->setDataSource(this);
    _patternTableView->setVisible(false);
    addChild(_patternTableView, 200);
}

#pragma mark- Touch events

bool PaintScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    _beginTouch = touch;

    Point touchPoint = convertToWorldSpace(touch->getLocation());
    Vec2 touchInPicture = _resPicture->convertToNodeSpace(touchPoint);
    Color4B color = ScribbleUtil::getColor(_resPicturePixelMask, touchInPicture);
    
    if (color.a == 0)
    {
        _isColouring = false;
    }
    else
    {
        if (_selectColorPenWidget || _selectPatternPenWidget || _isSelectEraser)
        {
            AppManager::getInstance()->playColoringEffect();

            _isColouring = true;
            
            _pictureScribble->setPatternColor(Color4F(color));
            _pictureScribble->paint(touchPoint);
        }
    }
    
    return true;
}

void PaintScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (_isColouring && (_selectColorPenWidget || _selectPatternPenWidget || _isSelectEraser))
    {
        Point touchPoint = convertToWorldSpace(touch->getLocation());
        Point previousTouchPoint = this->convertToWorldSpace(touch->getPreviousLocation());
        _pictureScribble->paint(touchPoint, previousTouchPoint);
    }
}

void PaintScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    AppManager::getInstance()->pauseColoringEffect();

}

void PaintScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    AppManager::getInstance()->pauseColoringEffect();

}

#pragma mark- Action methods

void PaintScene::backAction()
{
    AppManager::getInstance()->playButtonEffect();

    auto scene = ChoosePaintScene::createScene(_res.type);
    Director::getInstance()->replaceScene(scene);
}

void PaintScene::doneAction()
{
    if (_isDone)
        return;
    
    AppManager::getInstance()->playButtonEffect();

    int saveIndex = 1;
    //add to res manager
    if (_res.isPaintRes)
    {
        //if this res has been painted, just overwirte the old one.
        saveIndex = atoi(_res.resName.c_str());
    }
    else
    {
        //this is new paint res
        _res.isNewPaintRes = true;
        std::string key = UserDefaultKey_IsNewPaintRes + _res.resOriginalName;
        UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
        UserDefault::getInstance()->flush();
        
        switch (_res.type)
        {
            case ResType::ResType_Top:
            {
                saveIndex = ResManager::getInstance()->getSaveIndex(ResManager::getInstance()->getPaintTopRes());
                ResManager::getInstance()->addPaintTopRes(_res);
                break;
            }
            case ResType::ResType_Dress:
            {
                saveIndex = ResManager::getInstance()->getSaveIndex(ResManager::getInstance()->getPaintDressRes());
                ResManager::getInstance()->addPaintDressRes(_res);
                break;
            }
            case ResType::ResType_Bottom:
            {
                saveIndex = ResManager::getInstance()->getSaveIndex(ResManager::getInstance()->getPaintBottomRes());
                ResManager::getInstance()->addPaintBottomRes(_res);
                break;
            }
            case ResType::ResType_Shoes:
            {
                saveIndex = ResManager::getInstance()->getSaveIndex(ResManager::getInstance()->getPaintShoesRes());
                ResManager::getInstance()->addPaintShoesRes(_res);
                break;
            }
            default:
                break;
        }
    }
    
    //save current painting mask
    char indexString[10];
    sprintf(indexString, "%d", saveIndex);
    std::string maskPath = ResManager::getInstance()->getResClothesPath(_res)+ std::string(indexString) + "_history.png";
    _pictureScribble->getCanvas()->saveToFile(maskPath.c_str());
    
    //save current painting picture
    auto renderTexture = RenderTexture::create(_pictureScribble->getContentSize().width, _pictureScribble->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->setKeepMatrix(true);
    Vec2 rtBegin = Vec2(_pictureScribble->getPosition().x - _pictureScribble->getContentSize().width/2, _pictureScribble->getPosition().y - _pictureScribble->getContentSize().height/2);
    Rect fullRect = Rect(0, 0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    Rect fullViewport = Rect(0, 0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
    
    renderTexture->setVirtualViewport(rtBegin, fullRect, fullViewport);
    renderTexture->begin();
    _paintingLayer->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    std::string picturePath = ResManager::getInstance()->getResClothesPath(_res) + std::string(indexString) + "_picture.png";
    _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
    renderTexture->saveToFile(picturePath.c_str());
    
    _callbackCommand.init(_globalZOrder);
    _callbackCommand.func = CC_CALLBACK_0(PaintScene::onSaveSuccess, this);
    Director::getInstance()->getRenderer()->addCommand(&_callbackCommand);
    _isDone = true;
}

void PaintScene::onSaveSuccess()
{
    auto scene = DressScene::createScene(_res.type);
    Director::getInstance()->replaceScene(scene);
}

void PaintScene::selectPenAction()
{
    AppManager::getInstance()->playButtonEffect();

    if (_penTableView)
    {
        _penTableView->setVisible(true);
        _patternTableView->setVisible(false);
    }
}

void PaintScene::selectPatternAction()
{
    AppManager::getInstance()->playButtonEffect();

    if (_patternTableView)
    {
        _patternTableView->setVisible(true);
        _penTableView->setVisible(false);
    }
}

void PaintScene::eraserAction()
{
    AppManager::getInstance()->playButtonEffect();

    if (_selectColorPenWidget) {
        _selectColorPenWidget->changeToUnSelectStyle();
    }
    
    if (_selectPatternPenWidget) {
        _selectPatternPenWidget->changeToUnSelectStyle();
    }
    
    _selectColorPenWidget = nullptr;
    _selectPatternPenWidget = nullptr;
    _selectPenIndex = -2;
    _isSelectEraser = true;
    
    _eraserButton->loadTextureNormal(RES_BUTTON_ERASER_H);
    
    Color4F color(0,0,0,0);
    _pictureScribble->useTarget(color);
}

#pragma mark- TableView delegate methods

ssize_t PaintScene::numberOfCellsInTableView(TableView *table)
{
    if (table == _penTableView)
    {
        return PaintingManager::getInstance()->getPens().size() / 2;
    }
    else
    {
        return PaintingManager::getInstance()->getPatterns().size() / 2;
    }
}

void PaintScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    AppManager::getInstance()->playButtonEffect();

    if (table == _penTableView)
    {
        auto colorWidget1 = dynamic_cast<ColorWidget*>(cell->getChildren().at(0));
        auto colorWidget2 = dynamic_cast<ColorWidget*>(cell->getChildren().at(1));
        
        auto hitTest = [=](ColorWidget *widget){
          
            Point point = cell->convertToNodeSpace(_beginTouch->getLocation());
            log("%f %f",point.x, point.y);
            
            Rect frame;
            frame.size = Size(widget->getContentSize().width - 20, widget->getContentSize().height - 20);
            frame.origin = Point(widget->getPosition().x - widget->getContentSize().width/2 + 10, widget->getPosition().y - widget->getContentSize().height/2 + 10);
            if (frame.containsPoint(point)) {
                return true;
            }
            
            return false;
        };

        ColorWidget *widget = nullptr;
        if (colorWidget1 && hitTest(colorWidget1))
        {
            widget = colorWidget1;
        }
        else if (colorWidget2 && hitTest(colorWidget2))
        {
            widget = colorWidget2;
        }
        
        if (widget)
        {
            if (_selectColorPenWidget)
            {
                _selectColorPenWidget->changeToUnSelectStyle();
            }
            
            _eraserButton->loadTextureNormal(RES_BUTTON_ERASER);
            
            widget->changeToSelectStyle();
            _selectPenIndex = widget->getPen().index;
            _selectColorPenWidget = widget;
            if (_selectPatternIndex >= 0)
            {
                _selectPatternIndex = -1;
                _selectPatternPenWidget = nullptr;
                _patternTableView->reloadData();
            }
            
            Pen pen = widget->getPen();
            Color4F color((float)pen.R/255.0,
                          (float)pen.G/255.0,
                          (float)pen.B/255.0,
                          1.0);
            _pictureScribble->useTarget(color);
        }
    }
    else
    {
        auto patternWidget1 = dynamic_cast<PatternWidget*>(cell->getChildren().at(0));
        auto patternWidget2 = dynamic_cast<PatternWidget*>(cell->getChildren().at(1));
        
        auto hitTest = [=](PatternWidget *widget){
            
            Point point = cell->convertToNodeSpace(_beginTouch->getLocation());
            log("%f %f",point.x, point.y);
            
            Rect frame;
            frame.size = Size(widget->getContentSize().width - 20, widget->getContentSize().height - 20);
            frame.origin = Point(widget->getPosition().x - widget->getContentSize().width/2 + 10, widget->getPosition().y - widget->getContentSize().height/2 + 10);
            if (frame.containsPoint(point)) {
                return true;
            }
            
            return false;
        };
        
        PatternWidget *widget = nullptr;
        if (patternWidget1 && hitTest(patternWidget1))
        {
            widget = patternWidget1;
        }
        else if (patternWidget2 && hitTest(patternWidget2))
        {
            widget = patternWidget2;
        }
        
        if (widget)
        {
            if (_selectPatternPenWidget)
            {
                _selectPatternPenWidget->changeToUnSelectStyle();
            }
            
            _eraserButton->loadTextureNormal(RES_BUTTON_ERASER);
            
            widget->changeToSelectStyle();
            _selectPatternIndex = widget->getPattern().index;
            _selectPatternPenWidget = widget;
            if (_selectPenIndex >= 0)
            {
                _selectPenIndex = -2;
                _selectColorPenWidget = nullptr;
                _penTableView->reloadData();
            }

            Pattern pattern = widget->getPattern();
            Sprite *patternSprite = Sprite::create(pattern.pattern);
            if (patternSprite)
                _pictureScribble->useTarget(patternSprite);
        }
    }
}

Size PaintScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 90 + 10);
}

TableViewCell* PaintScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = TableViewCell::create();
    }
    
    if (cell->getChildren().size() > 0)
    {
        auto widget1 = dynamic_cast<BasePenWidget*>(cell->getChildren().at(0));
        auto widget2 = dynamic_cast<BasePenWidget*>(cell->getChildren().at(1));
        if (widget1 == _selectColorPenWidget) {
            _selectColorPenWidget = nullptr;
        }
        
        if (widget2 == _selectColorPenWidget) {
            _selectColorPenWidget = nullptr;
        }

        if (widget1 == _selectPatternPenWidget) {
            _selectPatternPenWidget = nullptr;
        }
        
        if (widget2 == _selectPatternPenWidget) {
            _selectPatternPenWidget = nullptr;
        }
    }
    
    
    cell->removeAllChildren();
    
    if (table == _penTableView)
    {
        Pen pen1 = PaintingManager::getInstance()->getPens().at(idx * 2);
        Pen pen2 = PaintingManager::getInstance()->getPens().at(idx * 2 + 1);

        auto colorPen1 = ColorWidget::create(pen1);
        auto colorPen2 = ColorWidget::create(pen2);
        
        if (_selectPenIndex == pen1.index) {
            _selectColorPenWidget = colorPen1;
            colorPen1->changeToSelectStyle();
        }
        
        if (_selectPenIndex == pen2.index) {
            _selectColorPenWidget = colorPen2;
            colorPen2->changeToSelectStyle();
        }
        
        if (_selectPenIndex == -1)
        {
            //default
            Pen pen = colorPen1->getPen();
            Color4F color((float)pen.R/255.0,
                          (float)pen.G/255.0,
                          (float)pen.B/255.0,
                          1.0);
            _pictureScribble->useTarget(color);
            _selectPenIndex = 0;
        }
 
        colorPen1->setPosition(Vec2(colorPen1->getContentSize().width/2, colorPen1->getContentSize().height/2));
        colorPen2->setPosition(Vec2(5 + colorPen1->getContentSize().width * 3 / 2, colorPen1->getContentSize().height/2));

        cell->addChild(colorPen1);
        cell->addChild(colorPen2);
    }
    else
    {
        Pattern pattern1 = PaintingManager::getInstance()->getPatterns().at(idx * 2);
        Pattern pattern2 = PaintingManager::getInstance()->getPatterns().at(idx * 2 + 1);
        
        auto patternPen1 = PatternWidget::create(pattern1);
        auto patternPen2 = PatternWidget::create(pattern2);
        
        if (_selectPatternIndex == pattern1.index) {
            _selectPatternPenWidget = patternPen1;
            patternPen1->changeToSelectStyle();
        }
        
        if (_selectPatternIndex == pattern2.index) {
            _selectPatternPenWidget = patternPen2;
            patternPen2->changeToSelectStyle();
        }
        
        patternPen1->setPosition(Vec2(patternPen1->getContentSize().width/2, patternPen1->getContentSize().height/2));
        patternPen2->setPosition(Vec2(5 + patternPen2->getContentSize().width * 3 / 2, patternPen2->getContentSize().height/2));
        
        cell->addChild(patternPen1);
        cell->addChild(patternPen2);
    }

    return cell;
}
