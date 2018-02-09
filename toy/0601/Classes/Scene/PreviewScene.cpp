//
//  PreviewScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#include "PreviewScene.h"
#include "../Model/STVisibleRect.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "../Widget/PreviewPage.h"
#include "HomeScene.h"
#include "../Layer/ModelLayer.h"
#include "PhotoScene.h"
#include "../Model/AppManager.h"
#include "../Layer/PreviewGuideLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace extension;

#define RES_COCOSTUDIO_JSON          "ui05_photo.json"
#define RES_BG                       "res/bg/bg_%d.jpg"
#define RES_PHOTO_FRAME              "ui05_frame1.png"
#define RES_BUTTON_CLOSE             "ui08_btn_close.png"
#define RES_BUTTON_SAVE              "ui05_btn_save.png"

#define BG_COUNT                     10

#define NAME_BUTTON_HOME             "ui08_btn_home"
#define NAME_BUTTON_TAKE_PHOTO       "ui08_btn_camara"

PreviewScene::PreviewScene() :
_photoImage(nullptr)
{}

PreviewScene::~PreviewScene()
{
    CC_SAFE_DELETE(_photoImage);
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

cocos2d::Scene* PreviewScene::createScene(const Model* model)
{
    auto scene = cocos2d::Scene::create();
    auto layer = PreviewScene::create(model);
    scene->addChild(layer);
    return scene;
}

PreviewScene* PreviewScene::create(const Model* model)
{
    PreviewScene *layer = new (std::nothrow)(PreviewScene);
    if (layer && layer->init(model))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool PreviewScene::init(const Model* model)
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        //init photo layer
        _photoLayer = Layer::create();
        addChild(_photoLayer);

        //backgroud
        _pageController = STPageController::create(this, getContentSize());
        CC_BREAK_IF(!_pageController);
        _pageController->ignoreAnchorPointForPosition(false);
        _pageController->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        _photoLayer->addChild(_pageController);
        _pageController->reloadData();
        
        //model
        auto modelLayer = ModelLayer::create();
        modelLayer->setRes(model->hairRes);
        modelLayer->setRes(model->bottomRes);
        modelLayer->setRes(model->topRes);
        modelLayer->setRes(model->dressRes);
        modelLayer->setRes(model->shoesRes);
        modelLayer->setRes(model->accessoryRes);
        float fixOffsetX = getContentSize().width / 2 - 185;
        modelLayer->setPosition(Vec2(fixOffsetX, 20));
        //namespaceST::STVisibleRect::setPosition(modelLayer, 120, 60, false, true, true, false);
        _photoLayer->addChild(modelLayer);
        
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui, 100);
        
        auto homeButton = Helper::seekWidgetByName(ui, NAME_BUTTON_HOME);
        homeButton->setZOrder(100);
        homeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                homeAction();
            }
        });
        
        auto takePhotoButton = Helper::seekWidgetByName(ui, NAME_BUTTON_TAKE_PHOTO);
        takePhotoButton->setZOrder(100);
        takePhotoButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                takePhotoAction();
            }
        });
        
        //
        _photoFrameContainer = Widget::create();
        _photoFrameContainer->setContentSize(getContentSize());
        _photoFrameContainer->setTouchEnabled(true);
        _photoFrameContainer->setPosition(getContentSize()/2);
        _photoFrameContainer->setVisible(false);
        addChild(_photoFrameContainer, 101);
        
        _photoFrame = extension::Scale9Sprite::create(RES_PHOTO_FRAME, Rect(0, 0, 114, 185), Rect(38, 38, 40, 33));
        _photoFrame->setPreferredSize(getContentSize() * PHOTO_DISPLAY_SCALE);
        _photoFrame->setPosition(getContentSize()/2);
        _photoFrameContainer->addChild(_photoFrame);
        
        auto closeButton = Button::create(RES_BUTTON_CLOSE);
        closeButton->setPosition(Vec2(_photoFrame->getContentSize().width, _photoFrame->getContentSize().height));
        closeButton->setZOrder(100);
        closeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                closeAction();
            }
        });
        _photoFrame->addChild(closeButton);
        
        auto albumButton = Button::create(RES_BUTTON_SAVE);
        albumButton->setPosition(Vec2(_photoFrame->getContentSize().width/2, 13 + albumButton->getContentSize().height/2));
        albumButton->setZOrder(100);
        albumButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                albumAction();
            }
        });
        _photoFrame->addChild(albumButton);

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        //see preview guide
        if (!UserDefault::getInstance()->getBoolForKey(UserDefaultKey_HasSawPreviewGuide, false))
        {
            auto layer = PreviewGuideLayer::create();
            addChild(layer, 100);
            
            UserDefault::getInstance()->setBoolForKey(UserDefaultKey_HasSawPreviewGuide, true);
            UserDefault::getInstance()->flush();
        }
        
        //play bgm
        AppManager::getInstance()->playStartBgm();
 
        rcode = true;
    } while (0);
    
    return rcode;
}

void PreviewScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (!AppManager::getInstance()->isFirstTimeLaunchApp())
    {
        AppManager::getInstance()->requestCrossPromoAd();
    }
}

#pragma mark- Action methods

void PreviewScene::getScreenShot()
{
    auto renderTexture = RenderTexture::create(getContentSize().width, getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _photoLayer->visit();
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    renderTexture->saveToFile("test.png");
}

void PreviewScene::homeAction()
{
    AppManager::getInstance()->playButtonEffect();

    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void PreviewScene::takePhotoAction()
{
    AppManager::getInstance()->playCameraEffect();

    //animation
    auto layer = LayerColor::create(Color4B(255.0, 255.0, 255.0, 255.0));
    addChild(layer, 300);
    layer->runAction(Sequence::create(FadeIn::create(0.4), FadeOut::create(0.7), NULL));
    
    //screen shot
    auto renderTexture = RenderTexture::create(getContentSize().width, getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->begin();
    _photoLayer->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    CC_SAFE_DELETE(_photoImage);
    _photoImage = renderTexture->newImage();
    
    auto texture = new Texture2D();
    texture->initWithImage(_photoImage);
    texture->autorelease();
    auto sprite = Sprite::createWithTexture(texture);
    
    sprite->setPosition(Vec2(_photoFrame->getContentSize().width/2, _photoFrame->getContentSize().height/2));
    _photoFrameContainer->setVisible(true);
    _photoFrame->addChild(sprite);
    sprite->setScale(PHOTO_DISPLAY_SCALE - 0.03);
}

void PreviewScene::closeAction()
{
    _photoFrameContainer->setVisible(false);
}

void PreviewScene::albumAction()
{
    if (_isDone) {
        return;
    }
    struct timeval now;
    gettimeofday(&now, nullptr);
    std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + _time + ".jpg";
    log("MX: %s", fullPath.c_str());

    ResManager::getInstance()->savePhoto(_time + ".jpg");
    _photoImage->saveToFile(fullPath);
    CC_SAFE_DELETE(_photoImage);
    
    //show rate us
    AppManager::getInstance()->rateUsLogic();
    
    _isDone = true;
    auto scene = PhotoScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

#pragma mark- TableViewDataSource

Size PreviewScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return getContentSize();
}

TableViewCell* PreviewScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto page = (PreviewPage *)table->dequeueCell();
    if (page)
    {
        page->reset();
    }
    else
    {
        page = PreviewPage::create();
        page->setContentSize(getContentSize());
    }
    
    char filename[100];
    sprintf(filename, RES_BG, (int)idx + 1);
    page->setBackground(filename);
    
    return page;
}

ssize_t PreviewScene::numberOfCellsInTableView(TableView *table)
{
    return BG_COUNT;
}




