//
//  ChoosePaintScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#include "ChoosePaintScene.h"

#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "HomeScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/AppManager.h"
#include "../Layer/ShopLayer.h"
#include "PaintScene.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace extension;

#define RES_COCOSTUDIO_JSON          "ui02_select.json"
#define RES_GUIDE_ARROW              "ui08_guide.png"

#define NAME_BUTTON_BACK             "ui08_btn_back"
#define NAME_BUTTON_SHOP             "ui08_btn_coins"
#define NAME_BUTTON_DONE             "ui08_btn_done"
#define NAME_CHECKBOX_TOP            "ui02_top_h"
#define NAME_CHECKBOX_BOTTOM         "ui02_bottom_h "
#define NAME_CHECKBOX_DRESS          "ui02_dress_h"
#define NAME_CHECKBOX_SHOES          "ui02_shoes_h"
#define NAME_LABEL_COIN              "ui08_btn_coins"
#define NAME_BG                      "ui00_loading_bg"
#define NAME_CONTAINER               "ui02_dress_bar"


#define STRINGIFY(A)  #A
#include "../Model/ChangeToWhiteColor.frag"

#define FONT_NAME                    "Helvetica_Rounded_LT_Black.ttf"
#define FONT_SIZE                    26

ChoosePaintScene::ChoosePaintScene():
_guideArrow(nullptr),
_selectRes(nullptr),
_needShowFullScreenAds(false),
_isDone(false),
_selectTopIcon(nullptr),
_selectDressIcon(nullptr),
_selectBottomIcon(nullptr),
_selectShoesIcon(nullptr),
_selectTopIndex(-1),
_selectDressIndex(-1),
_selectBottomIndex(-1),
_selectShoesIndex(-1)
{}

ChoosePaintScene::~ChoosePaintScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* ChoosePaintScene::createScene(const ResType type)
{
    auto scene = Scene::create();
    auto layer = ChoosePaintScene::create(type);
    scene->addChild(layer);
    return scene;
}

ChoosePaintScene* ChoosePaintScene::create(const ResType type)
{
    ChoosePaintScene *layer = new (std::nothrow)(ChoosePaintScene);
    if (layer && layer->init(type))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool ChoosePaintScene::init(const ResType type)
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui);
        
        auto container = Helper::seekWidgetByName(ui, NAME_CONTAINER);
        container->setZOrder(5);
        
        //back button
        auto backButton = Helper::seekWidgetByName(ui, NAME_BUTTON_BACK);
        backButton->setZOrder(100);
        backButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                backAction();
            }
        });
        
        //shop button
        auto shopButton = Helper::seekWidgetByName(ui, NAME_BUTTON_SHOP);
        shopButton->setZOrder(100);
        shopButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                shopAction();
            }
        });
        
        //done button
        _doneButton = Helper::seekWidgetByName(ui, NAME_BUTTON_DONE);
        _doneButton->setVisible(false);
        shopButton->setZOrder(100);
        _doneButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                doneAction();
            }
        });
        
        //top
        auto topCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_TOP));
        auto bottomCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_BOTTOM));
        auto dressCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_DRESS));
        auto shoesCheckBox = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_CHECKBOX_SHOES));

        topCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                bottomCheckBox->setSelectedState(false);
                dressCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                
                AppManager::getInstance()->playButtonEffect();
                selectTopAction();
            }
            else
            {
                topCheckBox->setSelectedState(true);
            }
        });
        
        bottomCheckBox->addEventListener([=](Ref*, CheckBox::EventType type){
            
            if (type == CheckBox::EventType::SELECTED)
            {
                dressCheckBox->setSelectedState(false);
                shoesCheckBox->setSelectedState(false);
                topCheckBox->setSelectedState(false);
                
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
                topCheckBox->setSelectedState(false);
                
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
                topCheckBox->setSelectedState(false);
                
                AppManager::getInstance()->playButtonEffect();
                selectShoesAction();
            }
            else
            {
                shoesCheckBox->setSelectedState(true);
            }
        });

        //init picture image view
        _pictureWhiteMask = Sprite::create();
        GLProgram *p = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ChangeToWhiteColor_frag);
        CHECK_GL_ERROR_DEBUG();
        if (nullptr != p) {
            ShaderCache::getInstance()->addGLProgram(p, "ChangeToWhiteColor");
        }
        _pictureWhiteMask->setGLProgram(p);
        namespaceST::STVisibleRect::setPosition(_pictureWhiteMask, 220, 495);
        ui->addChild(_pictureWhiteMask, 4);
        
        _pictureImageView = Sprite::create();
        namespaceST::STVisibleRect::setPosition(_pictureImageView, 220, 495);
        ui->addChild(_pictureImageView, 4);
        
        //coin label
        auto coinLabel = Helper::seekWidgetByName(ui, NAME_LABEL_COIN);
        _coinLabel = Text::create("0", FONT_NAME, FONT_SIZE);
        _coinLabel->setTextHorizontalAlignment(TextHAlignment::CENTER);
        _coinLabel->setColor(Color3B(0, 107, 187));
        _coinLabel->setPosition(Vec2(coinLabel->getContentSize().width/2 + 10, coinLabel->getContentSize().height/2));
        coinLabel->addChild(_coinLabel);
        refreshCoinLabel();
        
        //init tableview
        initTableView();
        
        //default res type
        switch (type)
        {
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
        
        //choose guide
        if (!UserDefault::getInstance()->getBoolForKey(UserDefaultKey_HasSawChooseGuide, false))
        {
            _guideArrow = Sprite::create(RES_GUIDE_ARROW);
            namespaceST::STVisibleRect::setPosition(_guideArrow, 450, 870);
            addChild(_guideArrow, 1000);
            _guideArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.3, Vec2(30, -30)), MoveBy::create(0.3, Vec2(-30, 30)),  NULL)));
        }
        
        //show banner ads
        AppManager::getInstance()->showBannerAd();
        
        //play bgm
        AppManager::getInstance()->playSelectBgm();
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void ChoosePaintScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (!AppManager::getInstance()->isFirstTimeLaunchApp() && _needShowFullScreenAds)
    {
        AppManager::getInstance()->requestCrossPromoAd();
    }
}

#pragma mark- ui

void ChoosePaintScene::initTableView()
{
    auto createTable = [=]()->TableView*{
        
        auto tableView = extension::TableView::create(this, Size(150, 600));
        
        namespaceST::STVisibleRect::setPosition(tableView, 464, 190, true, false, false, true);
        tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
        addChild(tableView, 1000);
        
        return tableView;
    };
    
    //top scrollview
    _topTableView = createTable();
    _topTableView->setVisible(false);

    //bottom scrollview
    _bottomTableView = createTable();
    _bottomTableView->setVisible(false);
    
    //dress scrollview
    _dressTableView = createTable();
    _dressTableView->setVisible(false);
    
    //shoes scrollview
    _shoesTableView = createTable();
    _shoesTableView->setVisible(false);
}

void ChoosePaintScene::refreshCoinLabel()
{
    char coinString[10];
    
    int coin = AppManager::getInstance()->getCoin();
    if (coin > MAX_SHOWN_COIN)
    {
        sprintf(coinString, "%d+", MAX_SHOWN_COIN);
    }
    else
    {
        sprintf(coinString, "%d", coin);
    }
    
    _coinLabel->setString(coinString);
}

#pragma mark- Action methods

void ChoosePaintScene::backAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void ChoosePaintScene::shopAction()
{
    AppManager::getInstance()->playButtonEffect();

    auto layer = ShopLayer::create();
    Director::getInstance()->getRunningScene()->addChild(layer, 1000);
}

void ChoosePaintScene::doneAction()
{
    if (_isDone)
        return;
    
    if (_guideArrow) {
        UserDefault::getInstance()->setBoolForKey(UserDefaultKey_HasSawChooseGuide, true);
        UserDefault::getInstance()->flush();
    }
    
    AppManager::getInstance()->playButtonEffect();
    if (_selectRes)
    {
        _isDone = true;
        
        auto scene = PaintScene::createScene(*_selectRes);
        Director::getInstance()->replaceScene(scene);
    }
}

void ChoosePaintScene::selectTopAction()
{
    if (_topTableView)
    {
        _topTableView->setVisible(true);
        _topTableView->reloadData();
        
        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _shoesTableView->setVisible(false);
    }
}

void ChoosePaintScene::selectBottomAction()
{
    if (_bottomTableView)
    {
        _bottomTableView->setVisible(true);
        _bottomTableView->reloadData();

        _topTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _shoesTableView->setVisible(false);
    }
}

void ChoosePaintScene::selectDressAction()
{
    if (_dressTableView)
    {
        _dressTableView->setVisible(true);
        _dressTableView->reloadData();

        _bottomTableView->setVisible(false);
        _topTableView->setVisible(false);
        _shoesTableView->setVisible(false);
    }
}

void ChoosePaintScene::selectShoesAction()
{
    if (_shoesTableView)
    {
        _shoesTableView->setVisible(true);
        _shoesTableView->reloadData();

        _bottomTableView->setVisible(false);
        _dressTableView->setVisible(false);
        _topTableView->setVisible(false);
    }
}

#pragma mark- TableView delegate methods

ssize_t ChoosePaintScene::numberOfCellsInTableView(TableView *table)
{
    if (table == _topTableView)
    {
        return ResManager::getInstance()->getTopRes().size();
    }
    else if (table == _bottomTableView)
    {
        return ResManager::getInstance()->getBottomRes().size();
    }
    else if (table == _dressTableView)
    {
        return ResManager::getInstance()->getDressRes().size();
    }
    else if (table == _shoesTableView)
    {
        return ResManager::getInstance()->getShoesRes().size();
    }
    else
        return 0;
}

void ChoosePaintScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    AppManager::getInstance()->playButtonEffect();

    auto container = dynamic_cast<PaintIconWidget*>(cell->getChildren().at(0));
    if (container)
    {
        Point point = cell->convertToNodeSpace(_beginTouch->getLocation());
        log("%f %f",point.x, point.y);
        
        Rect frame;
        frame.origin = Point(10, 10);
        frame.size = Size(container->getContentSize().width - 20, container->getContentSize().height - 20);
        if (!frame.containsPoint(point)) {
            return;
        }
        
        auto doSelectIcon = [=](PaintIconWidget* container, int& selectIndex, PaintIconWidget** selectIcon){
            
            Res *res = container->getRes();
            std::string key = UserDefaultKey_HasPurchasedItem + res->resName;
            bool hasPurchased = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
            if (hasPurchased || res->price <= 0)
            {
                _pictureImageView->setTexture(res->resPicturePath);
                _pictureWhiteMask->setTexture(res->resMaskPath);
                _selectRes = res;
                
                if ((*selectIcon))
                {
                    (*selectIcon)->changeToUnSelectStyle();
                }
                
                container->changeToSelectStyle();
                container->hideNewLogo();
                std::string key = UserDefaultKey_IsNewPurchaseRes + res->resName;
                if (UserDefault::getInstance()->getBoolForKey(key.c_str(), false)) {
                    UserDefault::getInstance()->setBoolForKey(key.c_str(), false);
                }
                res->isNewPurchasedRes = false;
                *selectIcon = container;
                selectIndex = cell->getIdx();
                
                _doneButton->setVisible(true);
                if (_guideArrow)
                {
                    _guideArrow->setPosition(Vec2(122, 300));
                }
            }
            else
            {
                if (!container->isBuyStyle())
                {
                    if ((*selectIcon))
                    {
                        (*selectIcon)->changeToUnSelectStyle();
                    }
                    
                    container->changeToSelectStyle();
                    *selectIcon = container;
                    selectIndex = cell->getIdx();

                    container->changeToBuyStyle();
                }
                else
                {
                    bool success = AppManager::getInstance()->purchaseRes(*res);
                    if (!success)
                    {
                        //show shop?
                        auto layer = ShopLayer::create();
                        Director::getInstance()->getRunningScene()->addChild(layer, 1000);
                    }
                    else
                    {
                        container->changeToSelectStyle();
                        container->hidePriceButton();
                        container->showNewLogo();
                        *selectIcon = container;
                        selectIndex = cell->getIdx();

                        refreshCoinLabel();
                    }
                }
            }
        };
        
        _selectTopIndex = -1;
        _selectBottomIndex = -1;
        _selectDressIndex = -1;
        _selectShoesIndex = -1;
        
        if (table == _topTableView)
        {
            doSelectIcon(container, _selectTopIndex, &_selectTopIcon);
            _selectDressIcon = nullptr;
            _selectBottomIcon = nullptr;
            _selectShoesIcon = nullptr;
        }
        else if (table == _bottomTableView)
        {
            doSelectIcon(container, _selectBottomIndex, &_selectBottomIcon);
            _selectDressIcon = nullptr;
            _selectTopIcon = nullptr;
            _selectShoesIcon = nullptr;
        }
        else if (table == _dressTableView)
        {
            doSelectIcon(container, _selectDressIndex, &_selectDressIcon);
            _selectBottomIcon = nullptr;
            _selectTopIcon = nullptr;
            _selectShoesIcon = nullptr;
        }
        else
        {
            doSelectIcon(container, _selectShoesIndex, &_selectShoesIcon);
            _selectBottomIcon = nullptr;
            _selectTopIcon = nullptr;
            _selectDressIcon = nullptr;
        }
    }
}

Size ChoosePaintScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 143 + 10);
}

TableViewCell* ChoosePaintScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = TableViewCell::create();
    }
    
    if (cell->getChildren().size() > 0)
    {
        auto widget = dynamic_cast<PaintIconWidget*>(cell->getChildren().at(0));

        if (widget == _selectTopIcon) {
            _selectTopIcon = nullptr;
        }
        
        if (widget == _selectBottomIcon) {
            _selectBottomIcon = nullptr;
        }
        
        if (widget == _selectDressIcon) {
            _selectDressIcon = nullptr;
        }
        
        if (widget == _selectShoesIcon) {
            _selectShoesIcon = nullptr;
        }
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    auto setupContainer = [=](Res* res, int selectIndex, PaintIconWidget** selectIcon){
        
        auto container = PaintIconWidget::create(res);
        container->setPosition(Vec2(cell->getContentSize().width/2, cell->getContentSize().height/2));
        if (idx == selectIndex) {
            container->changeToSelectStyle();
            container->changeToPriceStyle();
            *selectIcon = container;
        }
        
        if (res->isNewPurchasedRes){
            container->showNewLogo();
        }
        
        cell->addChild(container);
    };
    
    Res *res;
    int selectIndex;
    if (table == _topTableView)
    {
        selectIndex = _selectTopIndex;
        res = &ResManager::getInstance()->getTopRes().at(idx);
        setupContainer(res, selectIndex, &_selectTopIcon);
    }
    else if (table == _bottomTableView)
    {
        selectIndex = _selectBottomIndex;
        res = &ResManager::getInstance()->getBottomRes().at(idx);
        setupContainer(res, selectIndex, &_selectBottomIcon);
    }
    else if (table == _dressTableView)
    {
        selectIndex = _selectDressIndex;
        res = &ResManager::getInstance()->getDressRes().at(idx);
        setupContainer(res, selectIndex, &_selectDressIcon);
    }
    else
    {
        selectIndex = _selectShoesIndex;
        res = &ResManager::getInstance()->getShoesRes().at(idx);
        setupContainer(res, selectIndex, &_selectShoesIcon);
    }
    
    return cell;
}



