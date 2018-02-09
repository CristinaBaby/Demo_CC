//
// SPAScene.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/10.
//
//

#include "SPAScene.h"
#include "AnimalModel.h"
#include "ViewPager.h"
#include "XCPageView.h"
#include "DressUpScene.h"
#include "AnimalChooseScene.h"
#include "IAPManager.h"
#include "AdsManager.h"

typedef enum {
    kSoap,
    kBlower,
    kComb,
    kNailclipper,
    kShowerhead
}ToolType;

SPAScene::SPAScene()
:m_vTools(std::vector<ImageView *>())
,m_vShampoos(std::vector<ImageView *>())
,m_pSelectedTool(nullptr)
,m_pSelectedToolIcon(nullptr)
,m_nShampooType(0)
,m_bAnimate(false)
,m_bScroll(false)
,m_bScaleing(false)
,m_sBubblePath("")
,m_bSound(false)
,m_eMode(kNormalMode)
,m_bBeagan(false)
{
    
}

void SPAScene::onEnter()
{
    BaseScene::onEnter();
    if (!IAPManager::getInstance()->isAdRemoved()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
}

SPAScene* SPAScene::create(AnimalModel *aAnimalModel)
{
    auto pRet = new SPAScene();
    if(pRet && pRet->init(aAnimalModel))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool SPAScene::init(AnimalModel *aAnimalModel)
{
    if ( !BaseScene::init("bg/spa_bg.jpg") )
    {
        return false;
    }
    
    setName("SPAScene");
    
    m_pAnimalModel = aAnimalModel;
    
    loadstaticWidget();
    
    loadToolsBar();
    
    openTouchEvent();
    
    /*=================================设置默认泡泡================================================*/
    for (auto shampoo : m_vShampoos) {
        shampoo->setPositionY(90);
    }
    m_vShampoos.at(0)->setPositionY(110);
    changeTheShampoo(m_vShampoos.at(0)->getTag());
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/salon.mp3", true);
    /*==========================================================================================*/
    
    
    m_pCustom = _eventDispatcher->addCustomEventListener("ShopBuySuccess", [this](EventCustom *){
        if (IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    });
    
    return true;
}

void SPAScene::loadstaticWidget()
{
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/spa.json");
    m_pUILayer->addChild(pUI);
    
    m_pPrevious = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "public_btn_back"));
    m_pPrevious->setTag(1);
    m_pPrevious->addTouchEventListener(CC_CALLBACK_2(SPAScene::onButtonCallback, this));
    
    m_pNext = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "public_btn_next"));
    m_pNext->setTag(2);
    m_pNext->addTouchEventListener(CC_CALLBACK_2(SPAScene::onButtonCallback, this));
    
    m_pAnimalModel->setPosition(XCVisibleRect::getPosition(320, 480));
    m_pGameLayer->addChild(m_pAnimalModel, kGameZorder);
}

void SPAScene::loadToolsBar()
{
    m_pToolsView = XCPageView::create();
    m_pToolsView->setContentSize(Size(Director::getInstance()->getWinSize().width, 180));
    m_pToolsView->setPosition(Vec2(0, XCVisibleRect::getPositionY(100, kBottom)));
    m_pToolsView->addEventListener(CC_CALLBACK_2(SPAScene::onPageScrollDone, this));
    m_pUILayer->addChild(m_pToolsView);
    
    const std::string shampoos[] = {"spa/shampoo_1.png", "spa/shampoo_2.png", "spa/shampoo_3.png"};
    Layout *pFirstLayout = Layout::create();
    pFirstLayout->setContentSize(Size(Director::getInstance()->getWinSize().width, 180));
    m_pToolsView->insertPage(pFirstLayout, 0);
    float singleItemWidth = Director::getInstance()->getWinSize().width / 5;
    float x = singleItemWidth / 2;
    for (int i=0; i<3; ++i) {
        ImageView *pShampoo = ImageView::create(shampoos[i]);
        pShampoo->setTag(i + 1);
        pShampoo->addTouchEventListener(CC_CALLBACK_2(SPAScene::onShampooCallback, this));
        pShampoo->setPosition(Vec2(x, 90));
        pShampoo->setName(shampoos[i]);
        pShampoo->setTouchEnabled(true);
        pFirstLayout->addChild(pShampoo);
        m_vShampoos.push_back(pShampoo);
        x += singleItemWidth;
    }
    
    ImageView *pSponge = ImageView::create("spa/facial_cleanser.png");
    pSponge->setPosition(Vec2(x, 90));
    pSponge->setName("spa/facial_cleanser.png");
    pSponge->setTag(kSoap);
    pFirstLayout->addChild(pSponge);
    m_vTools.push_back(pSponge);
    
    x += singleItemWidth;
    ImageView *pBlower = ImageView::create("spa/spa_blower_1.png");
    pBlower->setPosition(Vec2(x, 90));
    pBlower->setName("spa/spa_blower_1.png");
    pBlower->setTag(kBlower);
    pFirstLayout->addChild(pBlower);
    m_vTools.push_back(pBlower);
    
    m_pViewPager = ViewPager::create("cocoStudio/public/public_point.png", "cocoStudio/public/public_point_h.png");
    m_pViewPager->setPosition(XCVisibleRect::getPosition(320, 130));
    m_pViewPager->setDotCount(2);
    m_pUILayer->addChild(m_pViewPager);
    
    Layout *pSecondLayout = Layout::create();
    pSecondLayout->setContentSize(Size(Director::getInstance()->getWinSize().width, 180));
    m_pToolsView->insertPage(pSecondLayout, 1);
    
    ImageView *pComb = ImageView::create("spa/comb.png");
    pComb->setPosition(Vec2(Director::getInstance()->getWinSize().width * 0.25, 90));
    pComb->setName("spa/comb.png");
    pComb->setTag(kComb);
    pSecondLayout->addChild(pComb);
    m_vTools.push_back(pComb);
    
    ImageView *pNailclipper = ImageView::create("spa/nailclippers_1.png");
    pNailclipper->setPosition(Vec2(Director::getInstance()->getWinSize().width * 0.75, 90));
    pNailclipper->setName("spa/nailclippers_1.png");
    pNailclipper->setTag(kNailclipper);
    pSecondLayout->addChild(pNailclipper);
    m_vTools.push_back(pNailclipper);
    
    m_pShowerhead = Sprite::create("spa/spa_shower_2.png");
    m_pShowerhead->setAnchorPoint(Vec2(0.5, 0.3));
    m_pShowerhead->setPosition(XCVisibleRect::getPosition(320, Director::getInstance()->getWinSize().height));
    m_pUILayer->addChild(m_pShowerhead);
}

void SPAScene::changeSPAMode(SPAScene::SPAMode aMode)
{
    
    if (m_bScaleing) {
        return;
    }
    
    switch (aMode) {
            
        case kNormalMode:
        {
            m_bScaleing = true;
            delete (Vec2 *)m_pNailclipper->getUserData();
            m_pNailclipper->removeFromParent();
            m_pNailclipper = nullptr;
            m_pUILayer->removeChildByName("sud83qjfdsa");
            m_pGameLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_pGameLayer->runAction(Spawn::create(ScaleTo::create(0.3, 1), MoveBy::create(0.3, Vec2(0, -100)), CallFunc::create([this](){
                m_pToolsView->resumeScroll();
                m_pToolsView->setVisible(true);
                m_pNext->setTouchEnabled(true);
                m_eMode = kNormalMode;
                m_bScaleing = false;
            }), nullptr));
        }
            break;
            
        case kZoomInMode:
        {
            m_bScaleing = true;
            
            m_pToolsView->setVisible(false);
            m_pToolsView->pauseScroll();
            m_pNext->setTouchEnabled(false);
            
            Vec2 scaleCenterPos = m_pAnimalModel->getAnimalFootPos();
            m_pGameLayer->setAnchorPoint(Vec2(scaleCenterPos.x / Director::getInstance()->getWinSize().width, scaleCenterPos.y / Director::getInstance()->getWinSize().height));
            m_pGameLayer->runAction(Spawn::create(ScaleTo::create(0.3, 1.5), MoveBy::create(0.3, Vec2(0, 100)), CallFunc::create([this, scaleCenterPos](){
                
                Node *pStencil = Node::create();
                
                Sprite *pCircle = Sprite::create("circle.png");
                pCircle->setPosition(scaleCenterPos);
                pStencil->addChild(pCircle);
                
                Sprite *pPreviousCircle = Sprite::create("cocoStudio/public/public_btn_back.png");
                pPreviousCircle->setPosition(m_pPrevious->getParent()->convertToWorldSpace(m_pPrevious->getPosition()));
                pStencil->addChild(pPreviousCircle);
                
                ClippingNode *pNode = ClippingNode::create(pStencil);
                pNode->setAlphaThreshold(0.5);
                pNode->setInverted(true);
                pNode->setName("sud83qjfdsa");
                m_pUILayer->addChild(pNode);
                
                LayerColor *pBackgroundLayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.9));
                pNode->addChild(pBackgroundLayer, 2);
                m_eMode = kZoomInMode;
                
                m_pNailclipper = Sprite::create("spa/nailclippers_1.png");
                m_pNailclipper->setPosition(pCircle->getPositionX(), pCircle->getPosition().y + pCircle->getContentSize().height / 2);
                m_pNailclipper->setUserData(new Vec2(pCircle->getPositionX(), pCircle->getPosition().y + pCircle->getContentSize().height / 2));
                m_pUILayer->addChild(m_pNailclipper);
                
                m_bBeagan = true;
                m_bScaleing = false;
            }), nullptr));
        }
            break;
            
        default:
            break;
    }
}


bool SPAScene::touchTool(cocos2d::Touch *aTouch)
{
    
    m_bScroll = false;
    m_pSelectedToolIcon = nullptr;
    m_pSelectedTool = nullptr;
    m_bSound = false;
    
    for (auto tool : m_vTools) {
        Vec2 localPos = tool->getParent()->convertToNodeSpace(aTouch->getLocation());
        if (tool->getBoundingBox().containsPoint(localPos)) {
            m_pSelectedToolIcon = tool;
            return true;
        }
    }
    
    if (m_pShowerhead->getBoundingBox().containsPoint(aTouch->getLocation())) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/showerhead_pick.mp3");
        ParticleSystemQuad *pParticle = ParticleSystemQuad::create("shower.plist");
        pParticle->setPosition(m_pShowerhead->getContentSize().width * 0.5, m_pShowerhead->getContentSize().height * 0.1);
        pParticle->setName("idpkjasdjiqwj");
        m_pShowerhead->addChild(pParticle);
        m_pSelectedTool = m_pShowerhead;
        return true;
    }
    
    return false;
}

void SPAScene::changeTheShampoo(int aType)
{
    static const std::string bubblePath[] = {"", "spa/spa_spongia_1.png", "spa/spa_spongia_2.png", "spa/spa_spongia_3.png"};
    m_nShampooType = aType;
    m_sBubblePath = bubblePath[aType];
}

void SPAScene::excuteLogic()
{
    
    if (!m_pSelectedToolIcon) {
        if (!m_bSound) {
            m_bSound = true;
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/showerhead_use.mp3", true);
        }
        Vec2 pos = Vec2(m_pShowerhead->getBoundingBox().origin.x + m_pShowerhead->getContentSize().width * 0.5 , m_pShowerhead->getBoundingBox().origin.y + m_pShowerhead->getContentSize().height * 0.1);
        m_pAnimalModel->removeBubble(45, pos);
        return;
    }
    
    switch (m_pSelectedToolIcon->getTag()) {
        case kSoap:
        {
            if (!m_bSound) {
                m_bSound = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/sponge_use.mp3", true);
            }
            m_pAnimalModel->addBubble(Sprite::create(m_sBubblePath), m_pSelectedTool->getPosition());
        }
            break;
        case kBlower:
        {
            if (!m_bSound) {
                m_bSound = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/hairdry_use_loop.mp3", true);
            }
            Vec2 pos = Vec2::ZERO;
            if (m_pSelectedTool->getPosition().x < XCVisibleRect::getPositionX(320)) {
                m_pSelectedTool->setFlippedX(true);
                pos = Vec2(m_pSelectedTool->getBoundingBox().origin.x + m_pSelectedTool->getContentSize().width * 0.9 , m_pSelectedTool->getBoundingBox().origin.y  +  m_pSelectedTool->getContentSize().height * 0.5);
            } else {
                m_pSelectedTool->setFlippedX(false);
                pos = Vec2(m_pSelectedTool->getBoundingBox().origin.x + m_pSelectedTool->getContentSize().width * 0.1 , m_pSelectedTool->getBoundingBox().origin.y  +  m_pSelectedTool->getContentSize().height * 0.5);
            }
            
            m_pAnimalModel->removeBubble(45, pos);
            m_pAnimalModel->blowAnimalFurs(pos);
        }
            break;
        case kComb:
        {
            if (!m_bSound) {
                m_bSound = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/comb_use.mp3", true);
            }
            Vec2 pos = Vec2::ZERO;
            if (m_pSelectedTool->getPosition().x < XCVisibleRect::getPositionX(320)) {
                m_pSelectedTool->setFlippedX(true);
                pos = Vec2(m_pSelectedTool->getBoundingBox().origin.x + m_pSelectedTool->getContentSize().width * 0.82 , m_pSelectedTool->getBoundingBox().origin.y  +  m_pSelectedTool->getContentSize().height * 0.7);
            } else {
                m_pSelectedTool->setFlippedX(false);
                pos = Vec2(m_pSelectedTool->getBoundingBox().origin.x + m_pSelectedTool->getContentSize().width * 0.18 , m_pSelectedTool->getBoundingBox().origin.y  +  m_pSelectedTool->getContentSize().height * 0.7);
            }
            m_pAnimalModel->cutFurs(40, pos);
        }
            break;
            
            break;
        default:
            break;
    }
}

void SPAScene::completeUseOfTool()
{
    m_bSound = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    
    if (!m_pSelectedToolIcon) {
        m_pShowerhead->removeChildByName("idpkjasdjiqwj");
        m_pShowerhead->setPosition(XCVisibleRect::getPosition(320, Director::getInstance()->getWinSize().height));
        m_pAnimalModel->completeRemoveBubble();
        m_bSound = false;
        return;
    }
    
    switch (m_pSelectedToolIcon->getTag()) {
        case kSoap:
        {
            m_pAnimalModel->completeAddBubble();
            m_pSelectedTool->removeFromParent();
            m_pSelectedTool = nullptr;
            m_pSelectedToolIcon->setVisible(true);
        }
            break;
        case kBlower:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/hairdry_use_end.mp3");
            m_pSelectedTool->setFlippedX(false);
            m_pAnimalModel->completeRemoveBubble();
            m_pAnimalModel->endBlowAnimalFurs();
            m_pSelectedTool->removeFromParent();
            m_pSelectedTool = nullptr;
            m_pSelectedToolIcon->setVisible(true);
        }
            break;
        case kComb:
        {
            m_pSelectedTool->removeFromParent();
            m_pSelectedTool = nullptr;
            m_pSelectedToolIcon->setVisible(true);
        }
            break;
        default:
            break;
    }
}

void SPAScene::onShampooCallback(cocos2d::Ref *aButton, Widget::TouchEventType aType)
{
    if (aType != Widget::TouchEventType::ENDED || m_bAnimate) {
        return;
    }
    
    ImageView *pSelectedShampoo = dynamic_cast<ImageView *>(aButton);
    
    if (pSelectedShampoo->getTag() == m_nShampooType) {
        return;
    }
    
    m_bAnimate = true;
    
    for (auto shampoo : m_vShampoos) {
        shampoo->setPositionY(90);
    }
    
    pSelectedShampoo->setPositionY(110);
    changeTheShampoo(pSelectedShampoo->getTag());
    
    m_pAnimalModel->clearBubble();
    
    m_bAnimate = false;
    
}

void SPAScene::onPageScrollDone(cocos2d::Ref *aRef, PageView::EventType aType)
{
    m_pViewPager->setCurrentDot(m_pToolsView->getCurPageIndex());
}

void SPAScene::onButtonCallback(cocos2d::Ref *aButton, Widget::TouchEventType aType)
{
    if (aType != Widget::TouchEventType::ENDED) {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
    if (dynamic_cast<Button *>(aButton)->getTag() == 1) {
        if (m_eMode == kNormalMode) {
            Director::getInstance()->replaceScene(AnimalChooseScene::create());
        } else {
            changeSPAMode(kNormalMode);
        }
    } else {
        m_pAnimalModel->clearBubble();
        Director::getInstance()->replaceScene(DressUpScene::create());
    }
}

bool SPAScene::onTouchBegan(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    
    bool touch = false;
    switch (m_eMode) {
        case kNormalMode:
        {
            touch = touchTool(aTouch);
        }
            break;
        case kZoomInMode:
        {
            if (m_pNailclipper->getBoundingBox().containsPoint(aTouch->getLocation())) {
                touch = true;
            }
        }
            break;
        default:
            break;
    }
    
    return touch;
}

void SPAScene::onTouchMoved(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    
    if (m_bBeagan) {
        return;
    }
    
    switch (m_eMode) {
        case kNormalMode:
        {
            
            if (m_bScroll) {
                return;
            }
            
            if (m_pSelectedTool) {
                if (m_pSelectedTool == m_pShowerhead) {
                    float nextPosY = aTouch->getDelta().y + m_pShowerhead->getBoundingBox().origin.y + m_pShowerhead->getContentSize().height;
                    if ( nextPosY < Director::getInstance()->getWinSize().height ) {
                        m_pSelectedTool->setPosition(aTouch->getDelta().x + m_pSelectedTool->getPositionX(), m_pSelectedTool->getPositionY());
                    } else {
                        m_pSelectedTool->setPosition(aTouch->getDelta() + m_pSelectedTool->getPosition());
                    }
                    
                } else {
                    m_pSelectedTool->setPosition(m_pSelectedTool->getPosition() + aTouch->getDelta());
                }
                excuteLogic();
                return;
            }
            
            if (fabsf(aTouch->getStartLocation().x - aTouch->getLocation().x) > 40) {
                m_bScroll = true;
                return;
            }
            
            if (m_pSelectedToolIcon) {
                if (aTouch->getLocation().y - aTouch->getStartLocation().y >= 20) {
                    switch (m_pSelectedToolIcon->getTag()) {
                        case kSoap:
                        {
                            m_pToolsView->pauseScroll();
                            m_pSelectedToolIcon->setVisible(false);
                            m_pSelectedTool = Sprite::create(m_pSelectedToolIcon->getName());
                            m_pSelectedTool->setPosition(aTouch->getLocation());
                            m_pUILayer->addChild(m_pSelectedTool);
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/sponge_pick.mp3");
                        }
                            break;
                        case kBlower:
                        {
                            m_pToolsView->pauseScroll();
                            m_pSelectedToolIcon->setVisible(false);
                            m_pSelectedTool = Sprite::create(m_pSelectedToolIcon->getName());
                            m_pSelectedTool->setPosition(aTouch->getLocation());
                            m_pUILayer->addChild(m_pSelectedTool);
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/hairdry_pick.mp3");
                        }
                            break;
                        case kComb:
                        {
                            m_pToolsView->pauseScroll();
                            m_pSelectedToolIcon->setVisible(false);
                            m_pSelectedTool = Sprite::create(m_pSelectedToolIcon->getName());
                            m_pSelectedTool->setPosition(aTouch->getLocation());
                            m_pUILayer->addChild(m_pSelectedTool);
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/comb_pick.mp3");
                        }
                            break;
                        case kNailclipper:
                        {
                            m_pSelectedToolIcon->setVisible(true);
                            changeSPAMode(kZoomInMode);
                        }
                            break;
                        default:
                            break;
                    }
                    
                }
            }
            
        }
            break;
        case kZoomInMode:
        {
            
            if (!m_pNailclipper->isVisible()) {
                return;
            }
            
            m_pNailclipper->setPosition(aTouch->getDelta() + m_pNailclipper->getPosition());
            
            Vec2 pos = Vec2::ZERO;
            if (m_pNailclipper->getPosition().x < XCVisibleRect::getPositionX(320)) {
                m_pNailclipper->setFlippedX(true);
                pos = Vec2(m_pNailclipper->getBoundingBox().origin.x + m_pNailclipper->getContentSize().width , m_pNailclipper->getBoundingBox().origin.y  +  m_pNailclipper->getContentSize().height * 0.9);
            } else {
                m_pNailclipper->setFlippedX(false);
                pos = Vec2(m_pNailclipper->getBoundingBox().origin.x , m_pNailclipper->getBoundingBox().origin.y  +  m_pNailclipper->getContentSize().height * 0.9);
            }
            
            if (!m_pAnimalModel->cutFingernail(pos)) {
                return;
            }
            
            ImageView *pNailClipperClose = ImageView::create("spa/nailclippers_2.png");
            pNailClipperClose->setPosition(m_pNailclipper->getPosition());
            if (m_pNailclipper->isFlippedX()) {
                pNailClipperClose->setFlippedX(true);
            }
            m_pUILayer->addChild(pNailClipperClose);
            pNailClipperClose->runAction(Sequence::create(DelayTime::create(0.5), RemoveSelf::create(), nullptr));
            m_pNailclipper->setVisible(false);
            m_pNailclipper->runAction(Sequence::create(DelayTime::create(0.5), Show::create(), MoveTo::create(0, m_pNailclipper->getPosition()), nullptr));
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Tools/tweezers_use.mp3");
            
        }
            break;
        default:
            break;
    }
    
}

void SPAScene::onTouchEnded(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    
    if (m_bBeagan) {
        m_bBeagan = false;
        return;
    }
    
    switch (m_eMode) {
        case kNormalMode:
        {
            m_pToolsView->resumeScroll();
            if (m_pSelectedTool) {
                completeUseOfTool();
            }
        }
            break;
        case kZoomInMode:
        {
            Vec2 *pVec = (Vec2 *)m_pNailclipper->getUserData();
            m_pNailclipper->setPosition(pVec->x, pVec->y);
        }
            break;
        default:
            break;
    }
    
}

void SPAScene::onExit()
{
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    BaseScene::onExit();
}
SPAScene::~SPAScene()
{
    _eventDispatcher->removeEventListener(m_pCustom);
}