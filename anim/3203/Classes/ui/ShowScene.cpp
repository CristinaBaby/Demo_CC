//
// ShowScene.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/26.
//
//

#include "ShowScene.h"
#include "DressUpScene.h"
#include "HomeScene.h"
#include "ShopLayer.h"
#include "STSystemFunction.h"
#include "LoadingLayer.h"
#include "IAPManager.h"

static const Color4B g_PenColor[] = {
    Color4B(0, 0, 0, 0),
    Color4B(255, 90, 0, 255),
    Color4B(254, 49, 247, 255),
    Color4B(255, 228, 0, 255),
    Color4B(15, 255, 26, 255),
    Color4B(0, 246, 255, 255)
};

ShowScene::ShowScene()
:m_pModel(nullptr)
,m_pPen(nullptr)
,m_pPhotoImage(nullptr)
,m_pPhotoTexture(nullptr)
,m_bFirstIn(true)
,m_bCamera(false)
,m_bButton(false)
{
    
}

void ShowScene::onEnter()
{
    BaseScene::onEnter();
    if (m_bFirstIn && !IAPManager::getInstance()->isAdRemoved() && !LoadingLayer::s_currentInstance) {
        m_bFirstIn = false;
        LoadingLayer::showLoading<LoadingLayer>(true, this, 99999);
        LoadingLayer::s_currentInstance->loadingDoneCallback = CC_CALLBACK_0(ShowScene::onAdsShowDone, this);
    } else {
        if (!IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
        }
    }
}

ShowScene* ShowScene::create()
{
    auto pRet = new ShowScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool ShowScene::init()
{
    if ( !BaseScene::init("") )
    {
        return false;
    }
    
    setName("ShowScene");
    
    m_pBg = Sprite::create("bg/show_bg_1.jpg");
    m_pBg->setPosition(XCVisibleRect::getPosition(320, 480));
    m_pBg->setTag(1);
    m_pGameLayer->addChild(m_pBg, kBackgroundZorder);
    
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/show.json");
    m_pUILayer->addChild(pUI);
    
    auto pPrevious = Helper::seekWidgetByName(pUI, "public_btn_back");
    pPrevious->setTag(1);
    pPrevious->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pHome = Helper::seekWidgetByName(pUI, "show_btn_home");
    pHome->setTag(2);
    pHome->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pShop = Helper::seekWidgetByName(pUI, "show_btn_shop");
    pShop->setTag(3);
    pShop->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pBG = Helper::seekWidgetByName(pUI, "show_btn_bg");
    pBG->setTag(4);
    pBG->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pMoreGame = Helper::seekWidgetByName(pUI, "show_btn_more");
    pMoreGame->setTag(5);
    pMoreGame->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pCamera = Helper::seekWidgetByName(pUI, "show_btn_camera");
    pCamera->setTag(6);
    pCamera->addTouchEventListener(CC_CALLBACK_2(ShowScene::onButtonCallback, this));
    
    auto pEraser = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "show_btn_eraser"));
    pEraser->addTouchEventListener([this](Ref *aSender, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        m_pPen->reset();
    });
    
    auto pPen = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(pUI, "show_btn_pencil"));
    pPen->addEventListener(CC_CALLBACK_2(ShowScene::onChangePaintType, this));
    pPen->setSelectedState(true);
    
    m_pFirstBox = nullptr;
    CheckBox *pLastBox = nullptr;
    for (int i=1; i<=5; ++i) {
        auto pPen = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(pUI, StringUtils::format("show_btn_pencil_%d", i)));
        pPen->setTag(i);
        pPen->addEventListener(CC_CALLBACK_2(ShowScene::onChangePenColor, this));
        if (!m_pFirstBox) {
            m_pFirstBox = pPen;
        } else {
            pLastBox->setUserData(pPen);
        }
        pLastBox = pPen;
    }
    
    pLastBox->setUserData(m_pFirstBox);
    m_pModel = AnimalModel::create(AnimalModel::Animal::kNone);
    m_pModel->setPosition(XCVisibleRect::getPosition(300, 460));
    m_pGameLayer->addChild(m_pModel);
    
    m_pPhotoNode = Node::create();
    m_pPhotoNode->setLocalZOrder(kPopZorder * 2);
    m_pPhotoNode->setPosition(XCVisibleRect::getPosition(320, 480));
    m_pUILayer->addChild(m_pPhotoNode);
    m_pPhotoNode->setVisible(false);
    
    m_pPhotoFrame = Sprite::create("cocoStudio/ui05_show/show_frame.png");
    m_pPhotoFrame->setLocalZOrder(1);
    m_pPhotoNode->addChild(m_pPhotoFrame);
    
    m_pClose = Button::create("cocoStudio/public/public_btn_close.png");
    m_pClose->addTouchEventListener([this](Ref *aSender, Widget::TouchEventType aType ){
        if (Widget::TouchEventType::ENDED != aType) {
            return;
        }
        if (m_bButton) {
            return;
        }
        m_bButton = true;
        this->onCompleteCamera(0.1);
    });
    m_pClose->setLocalZOrder(10);
    m_pPhotoNode->addChild(m_pClose);
    
    m_pSave = Button::create("cocoStudio/ui05_show/show_btn_save.png");
    m_pSave->addTouchEventListener([this](Ref *aSender, Widget::TouchEventType aType ){
        if (Widget::TouchEventType::ENDED != aType) {
            return;
        }
        if (m_bButton) {
            return;
        }
        m_bButton = true;
        
        STSystemFunction func;
        func.saveImageWithImage(m_pPhotoImage);
        
    });
    m_pSave->setLocalZOrder(11);
    m_pPhotoNode->addChild(m_pSave);
    
    m_pPen = WriteTouchNode::create(Director::getInstance()->getOpenGLView()->getFrameSize());
    m_pPen->setPenType(WriteNode::PenType::ePencil, "spa/pen_brush_4.png");
    m_pGameLayer->addChild(m_pPen, kUIZorder);
    m_pPen->setPenColor(Color4B::BLACK);
    m_pPen->setPenScale(0.2);
    
    m_pCustom = _eventDispatcher->addCustomEventListener("ShopBuySuccess", [this](EventCustom *){
        if (IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    });
    
    Layer *pTouchLayer = Layer::create();
    pTouchLayer->setLocalZOrder(5);
    m_pUILayer->addChild(pTouchLayer);
    EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
    pTouch->setSwallowTouches(true);
    pTouch->onTouchBegan = [this](Touch *, Event *)->bool{
        if (m_bCamera) {
            return true;
        } else {
            return false;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, pTouchLayer);
    
    _eventDispatcher->addCustomEventListener("ImageSaveComplete", [this](EventCustom *){
        this->onCompleteCamera(0.1);
    });
    
    return true;
}

void ShowScene::onButtonCallback(cocos2d::Ref *aButton, Widget::TouchEventType aType)
{
    if (Widget::TouchEventType::ENDED != aType || m_bTouchEvent) {
        return;
    }
    
    m_bTouchEvent = true;
    
    switch (dynamic_cast<Node *>(aButton)->getTag()) {
        case 1://Previous
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            Director::getInstance()->replaceScene(DressUpScene::create());
        }
            break;
        case 2://home
        {
            
            auto m_pPopUp = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/popup.json");
            m_pUILayer->addChild(m_pPopUp, kPopZorder);
            
            EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
            pTouch->onTouchBegan = [](Touch *, Event *)->bool{
                return true;
            };
            pTouch->setSwallowTouches(true);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, m_pPopUp);
            
            auto pYes = Helper::seekWidgetByName(m_pPopUp, "popup_btn_yes");
            pYes->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
                auto pHome = HomeScene::create();
                pHome->showAds();
                Director::getInstance()->replaceScene(pHome);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            });
            auto pNo = Helper::seekWidgetByName(m_pPopUp, "popup_btn_quit");
            pNo->addTouchEventListener([this, m_pPopUp](Ref *, Widget::TouchEventType aType){
                m_pPopUp->removeFromParent();
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
                m_bTouchEvent = false;
            });
            
            auto pText = dynamic_cast<Text *>(Helper::seekWidgetByName(m_pPopUp, "text"));
            pText->setString("Are you sure you want to back to home?");
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
        }
            break;
        case 3://shop
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
            m_pUILayer->addChild(ShopLayer::create(), kUIZorder);
            m_bTouchEvent = false;
        }
            break;
        case 4://bg
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            int index = m_pBg->getTag();
            ++ index;
            if (index > 6) {
                index = 1;
            }
            
            m_pBg->removeFromParent();
            m_pBg = Sprite::create(StringUtils::format("bg/show_bg_%d.jpg", index));
            m_pBg->setPosition(XCVisibleRect::getPosition(320, 480));
            m_pBg->setTag(index);
            m_pGameLayer->addChild(m_pBg, kBackgroundZorder);
            m_bTouchEvent = false;
            
        }
            break;
        case 5://moregame
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            STSystemFunction func;
            func.showMoreGame();
            m_bTouchEvent = false;
        }
            break;
        case 6://camera
        {
            
            if (m_bCamera) {
                return;
            }
            
            m_bCamera = true;
            
            RenderTexture *pRender = RenderTexture::create(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8_OES);
            pRender->beginWithClear(0, 0, 0, 0);
            m_pGameLayer->visit();
            pRender->end();
            Director::getInstance()->getRenderer()->render();
            m_pPhotoImage = pRender->newImage();
            
            m_pPhotoTexture = new Texture2D();
            m_pPhotoTexture->initWithImage(m_pPhotoImage);
            m_pPhoto = Sprite::createWithTexture(m_pPhotoTexture);
            m_pPhoto->setLocalZOrder(2);
            m_pPhoto->setScale(0.6);
            
            float scaleX = (m_pPhoto->getBoundingBox().size.width + 40.0) / m_pPhotoFrame->getContentSize().width;
            float scaleY = (m_pPhoto->getBoundingBox().size.height + 182.0) / m_pPhotoFrame->getContentSize().height;
            
            m_pPhotoFrame->setScale(scaleX, scaleY);
            m_pClose->setPosition(Vec2(m_pPhotoFrame->boundingBox().origin.x + m_pPhotoFrame->boundingBox().size.width - (m_pClose->getContentSize().width / 2), m_pPhotoFrame->boundingBox().origin.y + m_pPhotoFrame->boundingBox().size.height - (m_pClose->getContentSize().height / 2)));
            m_pSave->setPosition(Vec2(m_pPhotoFrame->boundingBox().origin.x + (m_pPhotoFrame->boundingBox().size.width / 2), m_pPhotoFrame->boundingBox().origin.y + 80));
            m_pPhoto->setPosition(m_pPhotoFrame->boundingBox().origin.x + (m_pPhotoFrame->boundingBox().size.width / 2), m_pPhotoFrame->boundingBox().origin.y + 142 + (((m_pPhotoFrame->boundingBox().size.height  - 142) / 2)));
            
            m_pPhotoNode->addChild(m_pPhoto);
            
            m_pPhotoNode->setScale(0.1);
            m_pPhotoNode->runAction(EaseBackOut::create(ScaleTo::create(0.3, 1)));
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/camera.mp3");
            m_pPhotoNode->setVisible(true);
        }
            break;
            
        default:
            break;
    }
    
}

void ShowScene::onChangePaintType(cocos2d::Ref *aSender, CheckBox::EventType aType)
{
    if (aType == CheckBox::EventType::UNSELECTED) {
        m_pFirstBox->setVisible(false);
        m_pFirstBox->setTouchEnabled(false);
    } else {
        m_pFirstBox->setVisible(true);
        m_pFirstBox->setTouchEnabled(true);
    }

    CheckBox *pNextColor = (CheckBox *)m_pFirstBox->getUserData();
    while (pNextColor != m_pFirstBox) {
        if (aType == CheckBox::EventType::UNSELECTED) {
            pNextColor->setVisible(false);
            pNextColor->setTouchEnabled(false);
        } else {
            pNextColor->setVisible(true);
            pNextColor->setTouchEnabled(true);
        }
        pNextColor = (CheckBox *)pNextColor->getUserData();
    }

}

void ShowScene::onChangePenColor(cocos2d::Ref *aSender, CheckBox::EventType aType)
{
    auto pSender = dynamic_cast<CheckBox *>(aSender);
    
    if (aType == CheckBox::EventType::SELECTED) {
        m_pPen->setPenColor(g_PenColor[pSender->getTag()]);
    } else {
        m_pPen->setPenColor(Color4B::BLACK);
    }
    
    CheckBox *pNextColor = (CheckBox *)pSender->getUserData();
    while (pSender != pNextColor) {
        pNextColor->setSelectedState(false);
        pNextColor = (CheckBox *)pNextColor->getUserData();
    }
}

void ShowScene::onCompleteCamera(float aTime)
{
    m_pPhotoNode->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.3, 0.1)), Hide::create(), CallFunc::create([this](){
        m_pPhoto->removeFromParent();
        delete m_pPhotoTexture;
        delete m_pPhotoImage;
        m_pPhotoImage = nullptr;
        m_pPhotoTexture = nullptr;
        m_bTouchEvent = false;
        m_bCamera = false;
        m_bButton = false;
    }), nullptr));
}

void ShowScene::onAdsShowDone()
{
    if (!IAPManager::getInstance()->isAdRemoved()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
}

void ShowScene::onExit()
{
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    BaseScene::onExit();
}
ShowScene::~ShowScene()
{
    _eventDispatcher->removeEventListener(m_pCustom);
    _eventDispatcher->removeCustomEventListeners("ImageSaveComplete");
}