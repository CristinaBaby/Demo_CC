//
//  DecorationIceCreamLayer.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#include "DecorationIceCreamLayer.hpp"
#include "SSCFileUtility.h"
#include "IAPManager.h"
#include "DecorationConeLayer.hpp"
#include "AdsManager.h"

#define ORIGIN_POINT2    Vec2(73, 320 + kAdapterScreen->g_oOffset.y)
#define END_POINT2       Vec2(216, 320 + kAdapterScreen->g_oOffset.y)

std::string listItem2[]
{
    "images/dec/decIceCream/btn_candy2.png",
    "images/dec/decIceCream/btn_fruit2.png",
    "images/dec/decIceCream/btn_wafer2.png"
};

std::string listItem2_h[]
{
    "images/dec/decIceCream/btn_candy1.png",
    "images/dec/decIceCream/btn_fruit1.png",
    "images/dec/decIceCream/btn_wafer1.png"
};

enum DecorationIceCreamLayer_Tag{
    eCandy_tag,
    eFruit_tag,
    eWafer_tag,
    eResetBtn_tag,
    eBoxBtn_tag
};

DecorationIceCreamLayer::DecorationIceCreamLayer():m_pBottomLayer(nullptr),m_pUILayer(nullptr),m_pDecBottomLayer(nullptr),m_pIceCreamLayer(nullptr),m_pDecTopLayer(nullptr),m_pBg(nullptr),m_pWafer(nullptr),m_pSticker(nullptr),m_pMoveRect(nullptr),m_pListviewBg(nullptr),m_pListView(nullptr),m_pAdapter(nullptr),m_pUnitScrollView(nullptr),m_pLastItem(nullptr),m_iLastIndex(-1),m_iCurrtIndex(-1)
{
    
}

DecorationIceCreamLayer::~DecorationIceCreamLayer()
{
    
}

Scene* DecorationIceCreamLayer::scene()
{
    auto pScene = Scene::create();
    auto pLayer = DecorationIceCreamLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool DecorationIceCreamLayer::init()
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer());
        
        g_bIsFlag = true;
        this->setNextBtnIsVisible(true);
        isInit = true;
    }while(0);
    
    return isInit;
}

bool DecorationIceCreamLayer::initData()
{
    m_vTypes.clear();
    
    m_oTouchPos = Vec2::ZERO;
    m_oListViewOffset = Vec2::ZERO;
    
    for (int i = 0; i < 3; i++)
    {
        Sprite* pListItem = Sprite::create(listItem2[i]);
        m_vTypes.pushBack(pListItem);
    }
    
    CATEGORY_NUMS[0] = 10;
    CATEGORY_NUMS[1] = 10;
    CATEGORY_NUMS[2] = 10;
    
    CATEGORY_NAME[0] = "candy";
    CATEGORY_NAME[1] = "fruit";
    CATEGORY_NAME[2] = "wafer";
    
    return true;
}

bool DecorationIceCreamLayer::initLayer()
{
    m_pBottomLayer = Layer::create();
    this->addChild(m_pBottomLayer, 100);
    
    m_pUILayer = Layer::create();
    this->addChild(m_pUILayer, 200);
    
    m_pDecBottomLayer = Layer::create();
    this->addChild(m_pDecBottomLayer, 300);
    
    m_pIceCreamLayer = Layer::create();
    this->addChild(m_pIceCreamLayer, 400);
    
    m_pDecTopLayer = Layer::create();
    this->addChild(m_pDecTopLayer, 500);

    
    initBackground();
    
    initScrollView();
    
    auto offset = 0;
    if (g_iIceCreamCount == 2){
        offset = -70;
    }else if (g_iIceCreamCount == 3){
        offset = -130;
    }
    // add wafer
    string str = SSCFileUtility::getStoragePath() + "cone.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    if (!str.empty() || str != "") {
        m_pWafer = Sprite::create(str);
        kAdapterScreen->setExactPosition(m_pWafer, 480, 320 + offset);
        m_pBottomLayer->addChild(m_pWafer);
    }
    
    // 如果有贴纸，则添加贴纸
    if (g_iStickerIndex != -1) {
        m_pSticker = Sprite::create(__String::createWithFormat("images/dec/decCone/sticker/sticker%d.png", g_iStickerIndex)->getCString());
        kAdapterScreen->setExactPosition(m_pSticker, 508, 221 + offset);
        m_pBottomLayer->addChild(m_pSticker, 2);
    }

    string str2 = SSCFileUtility::getStoragePath() + "icecream.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str2);
    if (!str2.empty() || str2 != "") {
        auto pIcecream = Sprite::create(str2);
        kAdapterScreen->setExactPosition(pIcecream, 480, 320 + offset + 150);
        m_pIceCreamLayer->addChild(pIcecream);
    }
    
    m_pMoveRect = LayerColor::create(Color4B(0, 0, 0, 0));
    m_pMoveRect->setContentSize(Size(212, 375));
    m_pMoveRect->ignoreAnchorPointForPosition(false);
    kAdapterScreen->setExactPosition(m_pMoveRect, 480, 320 + offset);
    this->addChild(m_pMoveRect, 30);
    
    // add reset button
    auto pResetBtn = ToolSprite::create("images/ui/btn_reset.png");
    kAdapterScreen->setExactPosition(pResetBtn, 26 + pResetBtn->getContentSize().width / 2 - 123, 105 + pResetBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
    pResetBtn->setTag(eResetBtn_tag);
    pResetBtn->setShadeBtn(true);
    m_pUILayer->addChild(pResetBtn);
    pResetBtn->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(1.0f, Vec2(-123, 0))), CallFunc::create([=](){
        
        pResetBtn->setDelegate(this);
    }), nullptr));
    
    
    ////////////////////////////////////////////////////////////////
    auto pBoxBtn = ToolSprite::create("images/dec/decIceCream/btn_box.png");
    kAdapterScreen->setExactPosition(pBoxBtn, -10 + pBoxBtn->getContentSize().width / 2 - 123, 120 + pBoxBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderNone);
    pBoxBtn->setTag(eBoxBtn_tag);
    pBoxBtn->setShadeBtn(true);
    m_pUILayer->addChild(pBoxBtn);
    pBoxBtn->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(1.0f, Vec2(-123, 0))), CallFunc::create([=](){
        
        pBoxBtn->setDelegate(this);
    }), nullptr));
    
    // add clipping node
    auto stencil = Sprite::create("images/dec/decIceCream/btn_mask.png");
    stencil->setPosition(Size(174.0f, 168.0f) / 2);
    
    auto pClipNode = ClippingNode::create();
    pClipNode->setContentSize(Size(174.0f, 168.0f));
    pClipNode->setStencil(stencil);
    pClipNode->setInverted(false);
    pClipNode->setAlphaThreshold(0);
    pClipNode->ignoreAnchorPointForPosition(false);
    pClipNode->setAnchorPoint(Point(0.5f, 0.5f));
    pClipNode->setPosition(Size(174.0f, 168.0f) / 2);
    pBoxBtn->addChild(pClipNode);
    
    auto pTarget = Sprite::create("images/dec/decIceCream/btn_1.png");
    pTarget->setPosition(Size(174.0f, 74.0f) / 2);
    pClipNode->addChild(pTarget);
    
    pTarget->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.6f, -16), RotateBy::create(1.2f, 32), RotateBy::create(0.6f, -16), nullptr)));
    
    return true;
}

void DecorationIceCreamLayer::initBackground()
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
    
    
    ////////////////////////////////////////////////////////
    auto pTable1 = Sprite::create("images/step/table_2.png");
    kAdapterScreen->setExactPosition(pTable1, 480, 40, Vec2(0.5f, 0));
    this->addChild(pTable1, 1);
    
    auto pTable2 = Sprite::create("images/step/table_1.png");
    kAdapterScreen->setExactPosition(pTable2, 480, 40, Vec2(0.5f, 1));
    this->addChild(pTable2, 2);
}

void DecorationIceCreamLayer::initScrollView()
{
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // add bg
    m_pListviewBg = Sprite::create("images/dec/box.png");
    kAdapterScreen->setExactPosition(m_pListviewBg, -6 - 200, 320, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    m_pUILayer->addChild(m_pListviewBg, 10);
    
    // add list item
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pListView = KDGridView::create(Size(178, 386));
    m_pListView->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_pListView->m_iCol = 1;
    m_pListView->m_fVSpace = 12;
    m_pListView->m_fMarginLeft = 2;
    m_pListView->m_fMarginTop = 5;
    m_pListView->m_fMarginBottom = 5;
    // set adapter
    m_pListView->setAdapter(m_pAdapter);
    m_pListView->setPosition(m_pListviewBg->getContentSize() / 2);
    m_pListView->m_bNeedAdjust = false;
    m_pListviewBg->addChild(m_pListView);
    
    m_pListviewBg->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(0.5f, Vec2(200, 0))), CallFunc::create([=](){
        
        m_pListView->setItemClickCall(CC_CALLBACK_3(DecorationIceCreamLayer::onTypeItemClick, this));
    }), nullptr));
    
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // add bg
    m_pUnitViewBg = Sprite::create("images/dec/box_1.png");
    m_pUnitViewBg->setPosition(ORIGIN_POINT2);
    m_pUILayer->addChild(m_pUnitViewBg, 5);
    m_pUnitViewBg->setVisible(false);
    
    // add unitScrollView
    m_pUnitScrollView = UnitScrollView::create(Size(144, 380), "", "images/ui/lock.png", "");
    m_pUnitScrollView->m_iCol = 1;
    m_pUnitScrollView->m_fVSpace = 23;
    m_pUnitScrollView->m_fMarginTop = 15;
    m_pUnitScrollView->m_fMarginBottom = 15;
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_pUnitScrollView->setPosition(m_pUnitViewBg->getContentSize() / 2);
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(DecorationIceCreamLayer::onGridItemCallback, this));
    m_pUnitScrollView->m_bNeedAdjust = false;
    m_pUnitScrollView->setIsNeedClickEffect(true);
    m_pUnitViewBg->addChild(m_pUnitScrollView);
}

void DecorationIceCreamLayer::onTypeItemClick(int tag,MenuItem* menuItem,int index)
{
    SoundPlayer::getInstance()->playEffect(selectSound);
    m_iCurrtIndex = index;
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if (m_iLastIndex != -1) {
        if (m_iCurrtIndex != m_iLastIndex) {
            // 重新加载类别资源
            m_pLastItem->setNormalImage(Sprite::create(listItem2[m_iLastIndex].c_str()));
            item->setNormalImage(Sprite::create(listItem2_h[m_iCurrtIndex].c_str()));
            
            m_pUnitViewBg->stopAllActions();
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
            m_pUnitScrollView->setScrollEnable(false);
            m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((m_pUnitViewBg->getPositionX() - ORIGIN_POINT2.x) / 600.0f, ORIGIN_POINT2), CallFunc::create([=](){
                
                m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
            }), MoveTo::create(0.25f, END_POINT2), CallFunc::create([=](){
                
                m_pUnitScrollView->m_pScrollView->setTouchEnabled(true);
                m_pUnitScrollView->setScrollEnable(true);
            }), nullptr));
            
        }else{
            item->setNormalImage(Sprite::create(listItem2[m_iCurrtIndex].c_str()));
            
            m_pUnitViewBg->stopAllActions();
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
            m_pUnitScrollView->setScrollEnable(false);
            m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((m_pUnitViewBg->getPositionX() - ORIGIN_POINT2.x) / 600.0f, ORIGIN_POINT2), CallFunc::create([=](){
                
                m_pUnitViewBg->setVisible(false);
            }), nullptr));
            m_iLastIndex = -1;
            
            return;
        }
    }else {
        item->setNormalImage(Sprite::create(listItem2_h[m_iCurrtIndex].c_str()));
        
        m_pUnitViewBg->stopAllActions();
        m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
        m_pUnitScrollView->setScrollEnable(false);
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
        m_pUnitViewBg->setVisible(true);
        m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((END_POINT2.x - m_pUnitViewBg->getPositionX()) / 600.0f, END_POINT2), CallFunc::create([=](){
            
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(true);
            m_pUnitScrollView->setScrollEnable(true);
        }), nullptr));
    }
    
    m_iLastIndex = m_iCurrtIndex;
    m_pLastItem = item;
}

void DecorationIceCreamLayer::onGridItemCallback(int tag,MenuItem* menuItem,int index)
{
    if (!kIAPManager->isPackagePurchased(0))
    {
        if (!kIAPManager->isPackagePurchased(2))
        {
            if(!kIAPManager->isFree(CATEGORY_NAME[m_iCurrtIndex].c_str(), index))
            {
                SoundPlayer::getInstance()->playEffect(selectSound);
                this->addLockScreenLayer(INT16_MAX);
                
                // 记录滚动视图的偏移量
                m_oListViewOffset = m_pUnitScrollView->m_pScrollView->getContentOffset();
                // enter shop layer
                SceneManager::getInstance()->enterShopScene();
                return;
            }
        }
    }
    
    SoundPlayer::getInstance()->playEffect(addSound);
    auto str = __String::createWithFormat("%s/%s%d.png", CATEGORY_NAME[m_iCurrtIndex].c_str(), CATEGORY_NAME[m_iCurrtIndex].c_str(), index)->getCString();
    
    switch (m_iCurrtIndex) {
        case eCandy_tag:
        case eFruit_tag:
        {
            auto pSprite = ToolSprite::create(str);
            pSprite->setPosition(m_oTouchPos);
            pSprite->setDrag(true);
            pSprite->setMoveRectByNode(m_pMoveRect, true);
            m_pDecTopLayer->addChild(pSprite, 30);
            pSprite->setRotation(rand() % 360);
            
            // 曲线进场
            ccBezierConfig oBezier;
            oBezier.controlPoint_1 = Vec2(200 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            oBezier.controlPoint_2 = Vec2(350 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            oBezier.endPosition = Vec2(480 + rand() % 120 - 60 + kAdapterScreen->g_oOffset.x, 320 + rand() % 200 - 100 + kAdapterScreen->g_oOffset.y);
            auto pBezierTo = BezierTo::create(0.8f, oBezier);
            pSprite->runAction(Sequence::create(pBezierTo, CallFunc::create([=](){
                
                pSprite->setDelegate(this);
            }), nullptr));
            
        }
            break;
            
        case eWafer_tag:
        {
            auto pSprite = ToolSprite::create(str);
            pSprite->setPosition(m_oTouchPos);
            pSprite->setDrag(true);
            m_pDecBottomLayer->addChild(pSprite);
            
            // 曲线进场
            ccBezierConfig oBezier;
            oBezier.controlPoint_1 = Vec2(200 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            oBezier.controlPoint_2 = Vec2(350 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            float posX = 0;
            while (1) {
                posX = rand() % 200 - 100;
                if (posX <= -50 || posX >= 50) {
                    break;
                }
            }
            auto offset = 0;
            if (g_iIceCreamCount == 1){
                pSprite->setMoveRectByNode(m_pMoveRect, -15, 100, 30, 0, true);
            }else if (g_iIceCreamCount == 2){
                offset = -70;
                pSprite->setMoveRectByNode(m_pMoveRect, -20, 150, 40, 0, true);
            }else if (g_iIceCreamCount == 3){
                offset = -100;
                pSprite->setMoveRectByNode(m_pMoveRect, -20, 200, 40, 0, true);
            }
            oBezier.endPosition = Vec2(480 + posX + kAdapterScreen->g_oOffset.x, 530 + rand() % 60 - 30 + kAdapterScreen->g_oOffset.y + offset);
            auto pBezierTo = BezierTo::create(0.8f, oBezier);
            pSprite->runAction(Sequence::create(pBezierTo, CallFunc::create([=](){
                
                pSprite->setDelegate(this);
            }), nullptr));
        }
            break;
    }
}

void DecorationIceCreamLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag()) {
        case eResetBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            isReturn = true;
            
            Dialog* dialog = Dialog::create((void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
            dialog->setContentText("Do you want to reset your Ice Cream?");
            dialog->setCallback(this);
            this->addChild(dialog, INT16_MAX);
        }
            break;
            
        case eBoxBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            isReturn = true;
            
            Dialog* dialog = Dialog::create((void*)kDialogNotice, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
            dialog->setContentText("Do you want to redecorate your cone?");
            dialog->setCallback(this);
            this->addChild(dialog, INT16_MAX);
        }
            break;
    }
}

void DecorationIceCreamLayer::onNextCallback()
{
    //////////////////////////////////////////////////////////////////////////
    string str = SSCFileUtility::getStoragePath() + "food.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    m_pBottomLayer->visit();
    m_pDecBottomLayer->visit();
    m_pIceCreamLayer->visit();
    m_pDecTopLayer->visit();
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    rt->saveToFile("food.png");
    
    this->scheduleOnce(schedule_selector(DecorationIceCreamLayer::enterNextScene), 0.1f);
    
}

void DecorationIceCreamLayer::enterNextScene(float ft)
{
    g_bIsFlag = false;
    if(kIAPManager->isShowAds())
    {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    SceneManager::getInstance()->enterEatScene();
}
void DecorationIceCreamLayer::onNegativeClick(void* type)
{
    BaseLayer::onNegativeClick(type);
}

void DecorationIceCreamLayer::onPositiveClick(void* type)
{
    isReturn = false;
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReset){
        SoundPlayer::getInstance()->playEffect(resetSound);
        
        m_pDecBottomLayer->removeAllChildrenWithCleanup(true);
        m_pDecTopLayer->removeAllChildrenWithCleanup(true);
    }else if (lType == kDialogNotice){
        g_bIsFlag = true;
        Director::getInstance()->pushScene(DecorationConeLayer::scene());
    }
}

void DecorationIceCreamLayer::onEnter()
{
    BaseLayer::onEnter();
    
    if (g_bIsFromShop) {
        g_bIsFromShop = false;
        
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
        m_pUnitScrollView->m_pScrollView->setContentOffset(m_oListViewOffset);
        this->removeLockScreenLayer();
    }

    if (!g_bIsFlag) {
        CATEGORY_NUMS[0] = 10;
        CATEGORY_NUMS[1] = 10;
        CATEGORY_NUMS[2] = 10;
        
        CATEGORY_NAME[0] = "candy";
        CATEGORY_NAME[1] = "fruit";
        CATEGORY_NAME[2] = "wafer";
        
        string str = SSCFileUtility::getStoragePath() + "cone.png";
        Director::getInstance()->getTextureCache()->removeTextureForKey(str);
        m_pWafer->setTexture(str);
        
        // 如果有贴纸，则添加贴纸
        if (g_iStickerIndex != -1) {
            if (m_pSticker) {
                m_pSticker->setTexture(__String::createWithFormat("images/dec/decCone/sticker/sticker%d.png", g_iStickerIndex)->getCString());
            }else{
                auto offset = 0;
                if (g_iIceCreamCount == 2){
                    offset = -70;
                }else if (g_iIceCreamCount == 3){
                    offset = -130;
                }
                m_pSticker = Sprite::create(__String::createWithFormat("images/dec/decCone/sticker/sticker%d.png", g_iStickerIndex)->getCString());
                kAdapterScreen->setExactPosition(m_pSticker, 508, 221 + offset);
                m_pBottomLayer->addChild(m_pSticker, 2);
            }
        }else{
            if (m_pSticker) {
                m_pSticker->removeFromParentAndCleanup(true);
                m_pSticker = nullptr;
            }
        }
    }
    
    auto pTempLayer = Layer::create();
    this->addChild(pTempLayer, 500);
    EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(false);
    pListener->onTouchBegan = [=](Touch* touch, Event* event){
        
        Vec2 touchPoint = touch->getLocation();
        m_oTouchPos = pTempLayer->getParent()->convertToNodeSpace(touchPoint);
        return this->isVisible();
    };
    pListener->onTouchMoved = [=](Touch* touch, Event* event){
        
        
    };
    pListener->onTouchEnded = [=](Touch* touch, Event* event){
        
        
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, pTempLayer);
    
    
    if (this->getNextBtnVisible() && !this->getNextBtnEnabled()){
        this->setNextBtnIsEnabled(true);
    }
}

void DecorationIceCreamLayer::onExit()
{
    BaseLayer::onExit();
}