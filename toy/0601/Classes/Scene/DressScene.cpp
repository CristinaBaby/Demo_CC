//
//  DressScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#include "DressScene.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "HomeScene.h"
#include "ChoosePaintScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/AppManager.h"
#include "../Model/PaintingManager.h"
#include "../Layer/ShopLayer.h"
#include "PaintScene.h"
#include "PreviewScene.h"
#include "ChoosePaintScene.h"
#include "PaintScene.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace extension;

#define RES_COCOSTUDIO_JSON          "ui04_dress.json"

#define NAME_BUTTON_HOME             "ui08_btn_home"
#define NAME_BUTTON_NEXT             "ui08_btn_next"

#define NAME_CHECKBOX_HAIR           "ui04_hair_h"
#define NAME_CHECKBOX_BOTTOM         "ui02_bottom_h"
#define NAME_CHECKBOX_DRESS          "ui02_dress_h"
#define NAME_CHECKBOX_SHOES          "ui02_shoes_h"
#define NAME_CHECKBOX_TOP            "ui02_top_h"
#define NAME_CHECKBOX_ACCESSORY      "ui04_necklace_h"

#define RES_BUTTON_ERASER            "ui03_eraser.png"
#define RES_BUTTON_ERASER_H          "ui03_eraser_h.png"
#define RES_DEFAULT_BG               "ui04_dress_bg.jpg"
#define RES_BRUSH                    "solid_brush.png"
#define RES_DRESS_DEFAULT_BG         "ui04_dress.json"

#define FONT_NAME                    "Helvetica_Rounded_LT_Black.ttf"
#define FONT_SIZE                    26

DressScene::DressScene() :
_selectIconWidget(nullptr),
_beginTouch(nullptr),
_needShowFullScreenAds(false),
_currentType(ResType::ResType_None)
{}

DressScene::~DressScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* DressScene::createScene(const ResType type)
{
    auto scene = Scene::create();
    auto layer = DressScene::create(type);
    scene->addChild(layer);
    return scene;
}

DressScene* DressScene::create(const ResType type)
{
    DressScene *layer = new (std::nothrow)(DressScene);
    if (layer && layer->init(type))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool DressScene::init(const ResType type)
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //bg
        auto bg = ImageView::create(RES_DEFAULT_BG);
        bg->setPosition(getContentSize() / 2);
        addChild(bg);
        
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui);
        
        //model
        _modelLayer = ModelLayer::create();
        float fixOffsetX = (getContentSize().width - 278) / 2 - 185;
        _modelLayer->setPosition(Vec2(fixOffsetX, 40));
        addChild(_modelLayer);
        
        //model dress default hair
        _modelLayer->setRes(&ResManager::getInstance()->getHairRes().at(0));
        
        //home button
        auto homeButton = Helper::seekWidgetByName(ui, NAME_BUTTON_HOME);
        homeButton->setZOrder(100);
        homeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                homeAction();
            }
        });
        
        auto nextButton = Helper::seekWidgetByName(ui, NAME_BUTTON_NEXT);
        nextButton->setZOrder(100);
        nextButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                nextAction();
            }
        });
        
        //hair
        auto hairCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_HAIR));
        auto bottomCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_BOTTOM));
        auto dressCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_DRESS));
        auto shoesCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_SHOES));
        auto topCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_TOP));
        auto accessoryCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_ACCESSORY));

        hairCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                dressCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                accessoryCheckBox->setSelectedState(false);

                AppManager::getInstance()->playButtonEffect();
                selectHairAction();
            }
            else
            {
                hairCheckBox->setSelectedState(true);
            }
        });
        
        bottomCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                dressCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                hairCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                accessoryCheckBox->setSelectedState(false);
                
                AppManager::getInstance()->playButtonEffect();
                selectBottomAction();
            }
            else
            {
                bottomCheckBox->setSelectedState(true);
            }
        });
        
        dressCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                hairCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                accessoryCheckBox->setSelectedState(false);
               
                AppManager::getInstance()->playButtonEffect();
                selectDressAction();
            }
            else
            {
                dressCheckBox->setSelectedState(true);
            }
        });
        
        shoesCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                dressCheckBox->setSelectedState(false);
                hairCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                accessoryCheckBox->setSelectedState(false);
                
                AppManager::getInstance()->playButtonEffect();
                selectShoesAction();
            }
            else
            {
                shoesCheckBox->setSelectedState(true);
            }
        });
        
        topCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                dressCheckBox->setSelectedState(false);
                hairCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                accessoryCheckBox->setSelectedState(false);
                
                AppManager::getInstance()->playButtonEffect();
                selectTopAction();
            }
            else
            {
                topCheckBox->setSelectedState(true);
            }
        });
        
        accessoryCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                dressCheckBox->setSelectedState(false);
                hairCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
              
                AppManager::getInstance()->playButtonEffect();
                selectAccessoryAction();
            }
            else
            {
                accessoryCheckBox->setSelectedState(true);
            }
        });
        
        //init tableview
        initTableView();
        
        //default res type
        switch (type)
        {
            case ResType::ResType_Hair:
            {
                hairCheckBox->setSelectedState(true);
                selectHairAction();
                break;
            }
            case ResType::ResType_Top:
            {
                topCheckBox->setSelectedState(true);
                selectTopAction();
                break;
            }
            case ResType::ResType_Dress:
            {
                dressCheckBox->setSelectedState(true);
                selectDressAction();
                break;
            }
            case ResType::ResType_Bottom:
            {
                bottomCheckBox->setSelectedState(true);
                selectBottomAction();
                break;
            }
            case ResType::ResType_Shoes:
            {
                shoesCheckBox->setSelectedState(true);
                selectShoesAction();
                break;
            }
            case ResType::ResType_Accessory:
            {
                accessoryCheckBox->setSelectedState(true);
                selectAccessoryAction();
                break;
            }
            default:
                break;
        }

        //touch event
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [=](Touch* touch, Event* event) {
            
            _beginTouch = touch;
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        //show banner ads
        AppManager::getInstance()->showBannerAd();
        
        //play bgm
        AppManager::getInstance()->playDressBgm();
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void DressScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (!AppManager::getInstance()->isFirstTimeLaunchApp() && _needShowFullScreenAds)
    {
        AppManager::getInstance()->requestFullScreenAd();
    }
}

#pragma mark- ui

void DressScene::initTableView()
{
    auto createTable = [=]()->TableView*{
        
        auto tableView = extension::TableView::create(this, Size(150, 600));
        
        namespaceST::STVisibleRect::setPosition(tableView, 464, 190, true, false, false, true);
        tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
        tableView->setDelegate(this);
        tableView->setDataSource(this);
        tableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
        addChild(tableView, 1000);
        
        return tableView;
    };

    _hairTableView = createTable();
    _hairTableView->reloadData();
    
    _bottomTableView = createTable();
    _bottomTableView->setVisible(false);
    
    _dressTableView = createTable();
    _dressTableView->setVisible(false);

    _shoesTableView = createTable();
    _shoesTableView->setVisible(false);
    
    _topTableView = createTable();
    _topTableView->setVisible(false);
    
    _accessoryTableView = createTable();
    _accessoryTableView->setVisible(false);
}

#pragma mark- Action methods

void DressScene::homeAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void DressScene::nextAction()
{
    if (_isDone)
        return;
    
    AppManager::getInstance()->playButtonEffect();
    _isDone = true;
    auto scene = PreviewScene::createScene(&_modelLayer->getModel());
    Director::getInstance()->replaceScene(scene);
}

void DressScene::selectHairAction()
{
    if (_hairTableView)
    {
        _hairTableView->setVisible(true);
        _hairTableView->reloadData();
        _currentType = ResType::ResType_Hair;

        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _shoesTableView->setVisible(false);
        _topTableView->setVisible(false);
        _accessoryTableView->setVisible(false);
    }
}

void DressScene::selectBottomAction()
{
    if (_bottomTableView)
    {
        _bottomTableView->setVisible(true);
        _bottomTableView->reloadData();
        _currentType = ResType::ResType_Bottom;

        _hairTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _shoesTableView->setVisible(false);
        _topTableView->setVisible(false);
        _accessoryTableView->setVisible(false);
    }
    
}

void DressScene::selectDressAction()
{
    if (_dressTableView)
    {
        _dressTableView->setVisible(true);
        _dressTableView->reloadData();
        _currentType = ResType::ResType_Dress;

        _bottomTableView->setVisible(false);
        _hairTableView->setVisible(false);
        _shoesTableView->setVisible(false);
        _topTableView->setVisible(false);
        _accessoryTableView->setVisible(false);
    }
}

void DressScene::selectShoesAction()
{
    if (_shoesTableView)
    {
        _shoesTableView->setVisible(true);
        _shoesTableView->reloadData();
        _currentType = ResType::ResType_Shoes;

        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _hairTableView->setVisible(false);
        _topTableView->setVisible(false);
        _accessoryTableView->setVisible(false);
    }
}

void DressScene::selectTopAction()
{
    if (_topTableView)
    {
        _topTableView->setVisible(true);
        _topTableView->reloadData();
        _currentType = ResType::ResType_Top;

        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _hairTableView->setVisible(false);
        _shoesTableView->setVisible(false);
        _accessoryTableView->setVisible(false);
    }
}

void DressScene::selectAccessoryAction()
{
    if (_accessoryTableView)
    {
        _accessoryTableView->setVisible(true);
        _accessoryTableView->reloadData();
        _currentType = ResType::ResType_Accessory;

        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _hairTableView->setVisible(false);
        _shoesTableView->setVisible(false);
        _topTableView->setVisible(false);
    }
}

#pragma mark- TableView delegate methods

ssize_t DressScene::numberOfCellsInTableView(TableView *table)
{
    if (table == _hairTableView)
    {
        return ResManager::getInstance()->getHairRes().size();
    }
    else if (table == _bottomTableView)
    {
        return ResManager::getInstance()->getPaintBottomRes().size() + 1;
    }
    else if (table == _dressTableView)
    {
        return ResManager::getInstance()->getPaintDressRes().size() + 1;
    }
    else if (table == _shoesTableView)
    {
        return ResManager::getInstance()->getPaintShoesRes().size() + 1;
    }
    else if (table == _topTableView)
    {
        return ResManager::getInstance()->getPaintTopRes().size() + 1;
    }
    else if (table == _accessoryTableView)
    {
        return ResManager::getInstance()->getAccessory().size();
    }
    else
        return 0;
}

void DressScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    AppManager::getInstance()->playDressEffect();

    auto widget = dynamic_cast<BaseIconWidget*>(cell->getChildren().at(0));
    Res *res = widget->getRes();
    Point point = cell->convertToNodeSpace(_beginTouch->getLocation());
    log("%f %f",point.x, point.y);
    
    Rect frame;
    frame.origin = Point(10, 10);
    frame.size = Size(widget->getContentSize().width - 20, widget->getContentSize().height - 20);
    if (!frame.containsPoint(point)) {
        return;
    }

    if (res == nullptr)
    {
        auto scene = ChoosePaintScene::createScene(_currentType);
        Director::getInstance()->replaceScene(scene);
    }
    else
    {
        auto dressWidget = dynamic_cast<DressIconWidget*>(widget);
        if (dressWidget != nullptr)
        {
            Rect paintButtonFrame;
            paintButtonFrame.origin = Vec2(dressWidget->getPaintButton()->getPosition().x - dressWidget->getPaintButton()->getContentSize().width/2, dressWidget->getPaintButton()->getPosition().y - dressWidget->getPaintButton()->getContentSize().height/2);
            paintButtonFrame.size = dressWidget->getPaintButton()->getContentSize();
            if (paintButtonFrame.containsPoint(point)) {
                auto scene = PaintScene::createScene(*res);
                Director::getInstance()->replaceScene(scene);
                return;
            }
        }
        
        _modelLayer->setRes(res);
        if (res->isNewPaintRes)
        {
            std::string key = UserDefaultKey_IsNewPaintRes + res->resOriginalName;
            UserDefault::getInstance()->setBoolForKey(key.c_str(), false);
            UserDefault::getInstance()->flush();
            res->isNewPaintRes = false;
            widget->hideNewLogo();
        }
     
        if (_selectIconWidget)
        {
            _selectIconWidget->changeToUnSelectStyle();
        }
        
        widget->changeToSelectStyle();
        widget->hideNewLogo();
        _selectIconWidget = widget;
    }
}

Size DressScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 143 + 10);
}

TableViewCell* DressScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = TableViewCell::create();
    }
    
    if (cell->getChildren().size() > 0)
    {
        auto container = cell->getChildren().at(0);
        if (container == _selectIconWidget) {
            _selectIconWidget = nullptr;
        }
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    Res *res = nullptr;
    if (table == _hairTableView)
    {
        res = &ResManager::getInstance()->getHairRes().at(idx);
    }
    else if (table == _bottomTableView)
    {
        int size = (int)ResManager::getInstance()->getPaintBottomRes().size();
        if (idx <= size - 1) {
            res = &ResManager::getInstance()->getPaintBottomRes().at(size - 1 - idx);
        }
    }
    else if (table == _dressTableView)
    {
        int size = (int)ResManager::getInstance()->getPaintDressRes().size();
        if (idx <= size - 1) {
            res = &ResManager::getInstance()->getPaintDressRes().at(size - 1 - idx);
        }
    }
    else if (table == _shoesTableView)
    {
        int size = (int)ResManager::getInstance()->getPaintShoesRes().size();
        if (idx <= size - 1) {
            res = &ResManager::getInstance()->getPaintShoesRes().at(size - 1 - idx);
        }
    }
    else if (table == _topTableView)
    {
        int size = (int)ResManager::getInstance()->getPaintTopRes().size();
        if (idx <= size - 1) {
            res = &ResManager::getInstance()->getPaintTopRes().at(size - 1 - idx);
        }
    }
    else if (table == _accessoryTableView)
    {
        res = &ResManager::getInstance()->getAccessory().at(idx);
    }
    
    BaseIconWidget *widget = nullptr;
    if (res == nullptr)
    {
        widget = DressIconWidget::create(res);
        ((DressIconWidget *)widget)->showAddButton();
    }
    else
    {
        if (res->type == ResType::ResType_Hair || res->type == ResType::ResType_Accessory)
        {
            widget = PaintIconWidget::create(res);
        }
        else
        {
            widget = DressIconWidget::create(res);
            if (res->isNewPaintRes)
            {
                widget->showNewLogo();
            }
        }
    }
    
    widget->setPosition(Vec2(cell->getContentSize().width/2, cell->getContentSize().height/2));
    cell->addChild(widget);
    
    return cell;
}
