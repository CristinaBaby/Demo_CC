//
//  ChooseLayer.cpp
//  MexicanFood
//
//  Created by huxixiang on 16/3/25.
//
//

#include "ChooseLayer.hpp"
#include "DecorationIceCreamLayer.hpp"
#include "FileUtility.h"
#include "IAPManager.h"

#define HOME_BTN_TAG 10

enum ChooseLayer_Tag
{
    eForm1_tag,
    eForm2_tag,
    eForm3_tag,
    eNextBtn_tag
};

Scene* ChooseLayer::scene()
{
    auto pScene = Scene::createWithPhysics();
//    pScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    pScene->getPhysicsWorld()->setGravity(Vec2(0, -150));
    auto pLayer = ChooseLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

ChooseLayer::ChooseLayer():m_pUILayer(nullptr),m_pContentLayer(nullptr),m_pBg(nullptr),m_pWafer(nullptr),m_pShadeLayer(nullptr),m_iCount(0),m_pListView(nullptr),m_iCurrtIndex(-1),m_iCurrtTag(0),m_pResponseArea(nullptr),m_pNextBtn(nullptr),m_bIsFlag(false),m_pHandTip(nullptr)
{
    
}

ChooseLayer::~ChooseLayer()
{
    
}

bool ChooseLayer::init()
{
    bool isInit = false;
    
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer());
        
//        this->setNextBtnIsVisible(true);
        isInit = true;
        
    }while(0);
    
    return isInit;
}

bool ChooseLayer::initData()
{
    m_vContianer.clear();
    m_oListViewOffset = Vec2::ZERO;
    
    CATEGORY_NUMS[0] = 10;
    CATEGORY_NAME[0] = "icecream";
    
    return true;
}

bool ChooseLayer::initLayer()
{
    this->initBackground();
    
    m_pUILayer = Layer::create();
    this->addChild(m_pUILayer, 200);
    
    m_pContentLayer = Layer::create();
    this->addChild(m_pContentLayer, 300);
    
    // 添加遮罩层
    m_pShadeLayer = LayerColor::create(Color4B(20, 20, 20, 200));
    this->addChild(m_pShadeLayer, 500);
    
    // add wafer
    string str = FileUtility::getStoragePath() + "cone.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    if (!str.empty() || str != "") {
        m_pWafer = Sprite::create(str);
        kAdapterScreen->setExactPosition(m_pWafer, 480, 170);
        this->addChild(m_pWafer, 4);
    }
    
    // 如果有贴纸，则添加贴纸
    if (g_iStickerIndex != -1) {
        auto pSticker = Sprite::create(__String::createWithFormat("images/dec/decCone/sticker/sticker%d.png", g_iStickerIndex)->getCString());
        kAdapterScreen->setExactPosition(pSticker, 508, 221 - 150);
        this->addChild(pSticker, 5);
    }
    

    ////////////////////////////////////////////////////
    auto pType1 = ShapeButton::create("images/step/choose/form1.png");
    kAdapterScreen->setExactPosition(pType1, 480, 240);
    pType1->setTag(eForm1_tag);
    m_pShadeLayer->addChild(pType1);
    pType1->setClickCall(CC_CALLBACK_1(ChooseLayer::btnClickCallback, this));
    
    auto pType2 = ShapeButton::create("images/step/choose/form2.png");
    kAdapterScreen->setExactPosition(pType2, 200, 450);
    pType2->setTag(eForm2_tag);
    m_pShadeLayer->addChild(pType2);
    pType2->setClickCall(CC_CALLBACK_1(ChooseLayer::btnClickCallback, this));
    
    auto pType3 = ShapeButton::create("images/step/choose/form3.png");
    kAdapterScreen->setExactPosition(pType3, 760, 450);
    pType3->setTag(eForm3_tag);
    m_pShadeLayer->addChild(pType3);
    pType3->setClickCall(CC_CALLBACK_1(ChooseLayer::btnClickCallback, this));
    
    return true;
}

void ChooseLayer::initBackground()
{
    // 由于背景图片过大，在部分android机型上无法加载，所以切分成4张图拼接而成
    m_pBg = Node::create();
    m_pBg->setContentSize(Size(2345, 1703));
    m_pBg->ignoreAnchorPointForPosition(false);
    m_pBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pBg, 480, 320);
    this->addChild(m_pBg);
    
    // 左上
    auto pPart_LT = Sprite::create("images/step/bg/bg_1.jpg");
    pPart_LT->setAnchorPoint(Vec2(1, 0));
    pPart_LT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LT);
    
    // 右上
    auto pPart_RT = Sprite::create("images/step/bg/bg_2.jpg");
    pPart_RT->setAnchorPoint(Vec2(0, 0));
    pPart_RT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RT);
    
    // 左下
    auto pPart_LD = Sprite::create("images/step/bg/bg_3.jpg");
    pPart_LD->setAnchorPoint(Vec2(1, 1));
    pPart_LD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LD);
    
    // 右下
    auto pPart_RD = Sprite::create("images/step/bg/bg_4.jpg");
    pPart_RD->setAnchorPoint(Vec2(0, 1));
    pPart_RD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RD);
}

void ChooseLayer::playAnimation(float ft)
{
    auto par = ParticleSystemQuad::create("plists/star.plist");
    par->setPosition(m_pShadeLayer->getChildByTag(m_iCount)->getPosition());
    this->addChild(par, 600);
    par->setAutoRemoveOnFinish(true);
    
    m_pShadeLayer->getChildByTag(m_iCount)->runAction(Repeat::create(Sequence::create(RotateBy::create(0.14f, 20), RotateBy::create(0.14f, -20), nullptr), 2));
    
    if (m_iCount == 2) {
        m_iCount = 0;
    }else{
        m_iCount++;
    }
}

void ChooseLayer::btnClickCallback(ShapeButton* sb)
{
    if (g_isDebugMode) {
        log("btnClickCallback=================");
    }
    this->addLockScreenLayer();
    
    SoundPlayer::getInstance()->playEffect(selectSound);
    sb->runAction(Sequence::create(clickAnimation(sb), CallFunc::create([=](){
    
        this->unschedule(schedule_selector(ChooseLayer::playAnimation));
        
        g_iIceCreamType = sb->getTag();
        
        for (Node* node : m_pShadeLayer->getChildren()) {
            node->runAction(FadeOut::create(1.0f));
        }
        m_pShadeLayer->runAction(Sequence::create(FadeOut::create(1.0f), CallFunc::create([=](){
            
            this->removeLockScreenLayer();
        }), RemoveSelf::create(), nullptr));
        
        // 在冰淇淋上添加一根线作为边界
        PhysicsBody* line = PhysicsBody::createEdgeSegment(m_pWafer->getPosition() + Vec2(-140, 115), m_pWafer->getPosition() + Vec2(185, 115));
        auto edgeNode = Node::create();
        edgeNode->setPhysicsBody(line);
        this->addChild(edgeNode, 4);
        
        ///////////////////////////////////////////////
        // add next button
        m_pNextBtn = ToolSprite::create("images/ui/btn_next.png");
        kAdapterScreen->setExactPosition(m_pNextBtn, 18 + m_pNextBtn->getContentSize().width / 2 - 123, 18 + m_pNextBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
        m_pNextBtn->setTag(eNextBtn_tag);
        m_pNextBtn->setShadeBtn(true);
        m_pNextBtn->setDelegate(this);
        m_pUILayer->addChild(m_pNextBtn);
        m_pNextBtn->setVisible(false);
        
        
        ///////////////////////////////////////////////
        m_pResponseArea = LayerColor::create(Color4B(0, 0, 0, 0));
        m_pResponseArea->setContentSize(Size(300, 250));
        m_pResponseArea->ignoreAnchorPointForPosition(false);
        kAdapterScreen->setExactPosition(m_pResponseArea, 510, 360);
        this->addChild(m_pResponseArea, 100);
        
        this->scheduleUpdate();
        ///////////////////////////////////////////////
        // add scroll view
        this->addScrollView();
    }), nullptr));
}

void ChooseLayer::addScrollView()
{
    // add unitScrollView
    m_pListView = UnitScrollView::create(Size(200, visibleSize.height), "", "images/ui/lock.png", "images/step/choose/icon_icecream/box.png");
    m_pListView->m_iCol = 1;
    m_pListView->m_fVSpace = 15;
    m_pListView->m_fMarginTop = 15;
    m_pListView->m_fMarginBottom = 15;
    m_pListView->setDirection(extension::ScrollView::Direction::VERTICAL);
    kAdapterScreen->setExactPosition(m_pListView, 105, 320, Vec2(0.5f, 0.5f), kBorderLeft, kBorderNone);
    m_pListView->m_bNeedAdjust = false;
    m_pListView->setIsNeedClickEffect(true);
    m_pUILayer->addChild(m_pListView, 5);
    m_pListView->setOffsetX(40);
    m_pListView->setOffsetY(-20);
    m_pListView->loadingUnitScrollViewWithType();
    m_pListView->m_pScrollView->setContentOffset(Vec2(0, visibleSize.height));
    m_pListView->m_pScrollView->getContainer()->runAction(Sequence::create(DelayTime::create(0.5f), EaseSineInOut::create(MoveBy::create(2.0f, Vec2(0, -m_pListView->m_pScrollView->getContainer()->getContentSize().height))), EaseSineInOut::create(MoveBy::create(1.0f, Vec2(0, m_pListView->m_pScrollView->getContainer()->getContentSize().height - visibleSize.height))), EaseSineInOut::create(MoveBy::create(0.8f, Vec2(0, visibleSize.height - m_pListView->m_pScrollView->getContainer()->getContentSize().height))), CallFunc::create([=](){
    
        m_pListView->setItemClickCall(CC_CALLBACK_3(ChooseLayer::onListItemCallback, this));
        m_pListView->setViewMoveCall(CC_CALLBACK_3(ChooseLayer::onChooseItemCallback, this));
        m_pListView->setTouchDownCall(CC_CALLBACK_3(ChooseLayer::onTouchDownCallback, this));
        m_pListView->setTouchMoveCall(CC_CALLBACK_3(ChooseLayer::onTouchMoveCallback, this));
        m_pListView->setTouchEndCall(CC_CALLBACK_3(ChooseLayer::onTouchEndCallback, this));
        
        // 添加手指提示
        m_pHandTip = ToolSprite::create("images/ui/fanger.png");
        m_pHandTip->setAnchorPoint(Vec2(0, 1));
        m_pHandTip->setPosition(m_pListView->getPosition() + Vec2(0, 110));
        this->addChild(m_pHandTip, INT16_MAX);
        this->runHandTipAnimation();
    }), nullptr));
}

void ChooseLayer::runHandTipAnimation()
{
    if (m_pHandTip) {
        m_pHandTip->stopAllActions();
        m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
        m_pHandTip->setVisible(true);
        m_pHandTip->setOpacity(0);
        m_pHandTip->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
            
            m_pHandTip->runAction(RepeatForever::create(Sequence::create(Repeat::create(Sequence::create(FadeIn::create(0.25f), MoveTo::create(1.0f, visibleSize / 2), FadeOut::create(0.15f), CallFunc::create([=](){
                
                m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
            }), DelayTime::create(0.5f), nullptr), 2), DelayTime::create(6.0f), nullptr)));
        }), nullptr));
    }
}

void ChooseLayer::onListItemCallback(int tag,MenuItem* menuItem,int index)
{
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(1))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[0].c_str(), index))
            {
                SoundPlayer::getInstance()->playEffect(selectSound);
                this->addLockScreenLayer(INT16_MAX);
                
                // 记录滚动视图的偏移量
                m_oListViewOffset = m_pListView->m_pScrollView->getContentOffset();
                // enter shop layer
                SceneManager::getInstance()->enterShopScene();
                return;
            }
        }
    }
}

void ChooseLayer::onTouchDownCallback(int index, Touch *pTouch, Event *pEvent)
{
    if (m_pHandTip) {
        m_pHandTip->stopAllActions();
        m_pHandTip->setVisible(false);
    }
    
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(1))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[0].c_str(), 9 - index))
            {
                return;
            }
        }
    }

    Vec2 realPos = this->convertToNodeSpace(pTouch->getLocation());
    auto pIcecream =  MyPhysicsNode::create(__String::createWithFormat("images/step/choose/icecream/icecream%d.png", 9 - index)->getCString());
    pIcecream->setPosition(realPos);
    m_pContentLayer->addChild(pIcecream, 6);
    pIcecream->setTouchPoint(pTouch->getLocation());
    m_iCurrtTag++;
    pIcecream->setTag(1000 * (index + 1) + m_iCurrtTag);
    pIcecream->getChildren().at(0)->setOpacity(0);
    pIcecream->getNode()->getPhysicsBody()->setGravityEnable(false);
    pIcecream->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([=](){
    
        pIcecream->TouchBegan(pTouch, pEvent);
    }), nullptr));
}

void ChooseLayer::onTouchMoveCallback(int index, Touch *pTouch, Event *pEvent)
{
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(1))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[0].c_str(), 9 - index))
            {
                return;
            }
        }
    }
    
    if (m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag)) {
        static_cast<MyPhysicsNode*>(m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag))->TouchMoved(pTouch, pEvent);
    }
}

void ChooseLayer::onChooseItemCallback(int index, Touch *pTouch, Event *pEvent)
{
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(1))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[0].c_str(), 9 - index))
            {
                return;
            }
        }
    }
    
    if (m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag) && m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag)->getChildren().at(0)->getOpacity() == 0) {
        static_cast<MyPhysicsNode*>(m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag))->getChildren().at(0)->setOpacity(255);
        static_cast<MyPhysicsNode*>(m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag))->getNode()->getPhysicsBody()->setGravityEnable(true);
        // 将冰淇淋保存到向量中
        m_vContianer.pushBack(static_cast<MyPhysicsNode*>(m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag)));
        
        m_pListView->setScrollEnable(false);
        m_pListView->m_pScrollView->setTouchEnabled(false);
        
        // 分三种冰淇淋,1,2,3个冰淇淋
        if (g_iIceCreamType == 0) {
            if (m_vContianer.size() > 1) {
                for (MyPhysicsNode* node : m_vContianer) {
                    if (node != m_vContianer.at(m_vContianer.size() - 1)) {
                        node->getNode()->setPhysicsBody(nullptr);
                        m_vContianer.eraseObject(node);
                        node->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, -960)), RemoveSelf::create(), nullptr));
                    }
                }
            }
        }else if(g_iIceCreamType == 1){
            if(m_vContianer.size() > 2){
                for (MyPhysicsNode* node : m_vContianer) {
                    if (node != m_vContianer.at(m_vContianer.size() - 1) && node != m_vContianer.at(m_vContianer.size() - 2)) {
                        node->getNode()->setPhysicsBody(nullptr);
                        m_vContianer.eraseObject(node);
                        node->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, -960)), RemoveSelf::create(), nullptr));
                    }
                }
            }
        }else if(g_iIceCreamType == 2){
            if(m_vContianer.size() > 3){
                for (MyPhysicsNode* node : m_vContianer) {
                    if (node != m_vContianer.at(m_vContianer.size() - 1) && node != m_vContianer.at(m_vContianer.size() - 2) && node != m_vContianer.at(m_vContianer.size() - 3)) {
                        node->getNode()->setPhysicsBody(nullptr);
                        m_vContianer.eraseObject(node);
                        node->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, -960)), RemoveSelf::create(), nullptr));
                    }
                }
            }
        }
    }
}

void ChooseLayer::onTouchEndCallback(int index, Touch *pTouch, Event *pEvent)
{
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(1))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[0].c_str(), 9 - index))
            {
                return;
            }
        }
    }
    
    static_cast<MyPhysicsNode*>(m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag))->TouchEnded(pTouch, pEvent);
    if (m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag) && m_pContentLayer->getChildByTag(1000 * (index + 1) + m_iCurrtTag)->getChildren().at(0)->getOpacity() == 0){
        m_pContentLayer->removeChildByTag(1000 * (index + 1) + m_iCurrtTag);
    }
    m_pListView->setScrollEnable(true);
    m_pListView->m_pScrollView->setTouchEnabled(true);
}

ActionInterval* ChooseLayer::clickAnimation(Node* node)
{
    if (node) {
        node->stopAllActions();
        node->setScale(1.0f);
    }
    ActionInterval *ret = nullptr;
    
    ScaleTo *lScale1 = ScaleTo::create(0.13, 1.0, 0.90);
    ScaleTo *lScale2 = ScaleTo::create(0.11, 0.92, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10, 1.0, 0.96);
    ScaleTo *lScale4 = ScaleTo::create(0.09, 0.98, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.99);
    ScaleTo *lScale6 = ScaleTo::create(0.07, 0.99, 1.0);
    
    ret = Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, nullptr);
    return ret;
}

void ChooseLayer::update(float ft)
{
    if (m_vContianer.size() != 0) {
        for (MyPhysicsNode* node : m_vContianer) {
            Vec2 pos = node->getNode()->getPosition() + node->getTouchPoint();
            if (!Rect(0, 0, visibleSize.width, visibleSize.height).containsPoint(pos)) {
                m_vContianer.eraseObject(node);
            }
        }
    }
    
    if (m_vContianer.size() != 0) {
        bool isInRect = false;
        for (MyPhysicsNode* node : m_vContianer) {
            Vec2 pos = node->getNode()->getPosition() + node->getTouchPoint();
            if (m_pResponseArea->getBoundingBox().containsPoint(pos)) {
                isInRect = true;
            }
        }
        
        if (isInRect) {
            if (!m_bIsFlag) {
                m_bIsFlag = true;
                m_pNextBtn->setSpriteEnabled(false);
                m_pNextBtn->setVisible(true);
                m_pNextBtn->stopAllActions();
                kAdapterScreen->setExactPosition(m_pNextBtn, 18 + m_pNextBtn->getContentSize().width / 2 - 123, 18 + m_pNextBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
                m_pNextBtn->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(1.0f, Vec2(-123, 0))), CallFunc::create([=](){
                    
                    m_pNextBtn->setSpriteEnabled(true);
                }), nullptr));
            }
        }else{
            if (m_bIsFlag) {
                m_bIsFlag = false;
                m_pNextBtn->setSpriteEnabled(false);
                m_pNextBtn->stopAllActions();
                kAdapterScreen->setExactPosition(m_pNextBtn, 18 + m_pNextBtn->getContentSize().width / 2, 18 + m_pNextBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
                m_pNextBtn->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.4f, Vec2(123, 0))), CallFunc::create([=](){
                    
                    m_pNextBtn->setSpriteEnabled(true);
                    m_pNextBtn->setVisible(false);
                }), nullptr));
            }
        }
        
        isInRect = false;
    }
}

void ChooseLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eNextBtn_tag) {
        this->unscheduleUpdate();
        onNextCallback();
    }
}

void ChooseLayer::onNextCallback()
{
    //////////////////////////////////////////////////////////////////////////
    string str = FileUtility::getStoragePath() + "icecream.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    m_pContentLayer->visit();
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    rt->saveToFile("icecream.png");
    
    this->scheduleOnce(schedule_selector(ChooseLayer::enterNextScene), 0.1f);
}

void ChooseLayer::enterNextScene(float ft)
{
    g_iIceCreamCount = m_vContianer.size();
    SceneManager::getInstance()->enterDecorationIceCreamScene();
}

void ChooseLayer::onEnter()
{
    BaseLayer::onEnter();
    
    if (g_bIsFromShop) {
        g_bIsFromShop = false;
        
        m_pListView->loadingUnitScrollViewWithType();
        m_pListView->m_pScrollView->setContentOffset(m_oListViewOffset);
        this->removeLockScreenLayer();
    }else{
        this->schedule(schedule_selector(ChooseLayer::playAnimation), 2.5f);
    }
}

void ChooseLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void ChooseLayer::onExit()
{
    BaseLayer::onExit();
}