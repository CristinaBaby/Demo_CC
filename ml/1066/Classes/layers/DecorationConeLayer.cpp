//
//  DecorationConeLayer.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#include "DecorationConeLayer.hpp"
#include "FileUtility.h"
#include "IAPManager.h"

#define ORIGIN_POINT    Vec2(73, 320 + kAdapterScreen->g_oOffset.y)
#define END_POINT       Vec2(210, 320 + kAdapterScreen->g_oOffset.y)

std::string listItem1[]
{
    "images/dec/decCone/btn_syrup1.png",
    "images/dec/decCone/btn_Sprinkle1.png",
    "images/dec/decCone/btn_Sticker1.png"
};

std::string listItem1_h[]
{
    "images/dec/decCone/btn_syrup2.png",
    "images/dec/decCone/btn_Sprinkle2.png",
    "images/dec/decCone/btn_Sticker2.png"
};

enum DecorationConeLayer_Tag{
    eSyrup_tag,
    eSprink_tag,
    eSticker_tag,
    eResetBtn_tag
};

DecorationConeLayer::DecorationConeLayer():m_pUILayer(nullptr),m_pContentLayer(nullptr),m_pBottomLayer(nullptr),m_pBg(nullptr),m_pListviewBg(nullptr),m_pListView(nullptr),m_pAdapter(nullptr),m_pUnitScrollView(nullptr),m_pLastItem(nullptr),m_iLastIndex(-1),m_iCurrtIndex(-1),m_pSyrup(nullptr),m_pSticker(nullptr),m_pMoveRect(nullptr)
{
    
}

DecorationConeLayer::~DecorationConeLayer()
{
    
}

Scene* DecorationConeLayer::scene()
{
    auto pScene = Scene::create();
    auto pLayer = DecorationConeLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool DecorationConeLayer::init()
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer());
        this->setNextBtnIsVisible(true);
        isInit = true;
    }while(0);
    
    return isInit;
}

bool DecorationConeLayer::initData()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    unsigned rand_seed = (unsigned)(now.tv_sec*1000 + now.tv_usec/1000);    //都转化为毫秒
    srand(rand_seed);
    
    m_oTouchPos = Vec2::ZERO;
    m_oListViewOffset = Vec2::ZERO;
    m_vTypes.clear();
    
    for (int i = 0; i < 3; i++)
    {
        Sprite* pListItem = Sprite::create(listItem1[i]);
        m_vTypes.pushBack(pListItem);
    }
    
    CATEGORY_NUMS[0] = 10;
    CATEGORY_NUMS[1] = 10;
    CATEGORY_NUMS[2] = 10;
    
    CATEGORY_NAME[0] = "syrup";
    CATEGORY_NAME[1] = "sprinkle";
    CATEGORY_NAME[2] = "sticker";
    
    return true;
}

bool DecorationConeLayer::initLayer()
{
    m_pUILayer = Layer::create();
    this->addChild(m_pUILayer, 200);
    
    m_pContentLayer = Layer::create();
    this->addChild(m_pContentLayer, 300);
    
    m_pBottomLayer = Layer::create();
    this->addChild(m_pBottomLayer, 100);
    
    initBackground();
    
    initScrollView();
    
    // add wafer
    auto pWafer = Sprite::create("images/dec/decCone/wafer_cone.png");
    kAdapterScreen->setExactPosition(pWafer, 510, 320);
    m_pBottomLayer->addChild(pWafer);
    
    m_pMoveRect = LayerColor::create(Color4B(0, 0, 0, 0));
    m_pMoveRect->setContentSize(Size(212, 254));
    m_pMoveRect->ignoreAnchorPointForPosition(false);
    kAdapterScreen->setExactPosition(m_pMoveRect, 509, 400);
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
    
    return true;
}

void DecorationConeLayer::initBackground()
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

void DecorationConeLayer::initScrollView()
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
    //    m_pListView->m_fMarginRight = 0;
    m_pListView->m_fMarginTop = 5;
    m_pListView->m_fMarginBottom = 5;
    // set adapter
    m_pListView->setAdapter(m_pAdapter);
    m_pListView->setPosition(m_pListviewBg->getContentSize() / 2);
    m_pListView->m_bNeedAdjust = false;
    m_pListviewBg->addChild(m_pListView);
    
    m_pListviewBg->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(0.5f, Vec2(200, 0))), CallFunc::create([=](){
        
        m_pListView->setItemClickCall(CC_CALLBACK_3(DecorationConeLayer::onTypeItemClick, this));
    }), nullptr));
    
    
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    // add bg
    m_pUnitViewBg = Sprite::create("images/dec/box_1.png");
    m_pUnitViewBg->setPosition(ORIGIN_POINT + kAdapterScreen->g_oOffset);
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
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(DecorationConeLayer::onGridItemCallback, this));
    m_pUnitScrollView->m_bNeedAdjust = false;
    m_pUnitScrollView->setIsNeedClickEffect(true);
    m_pUnitViewBg->addChild(m_pUnitScrollView);
}

void DecorationConeLayer::onTypeItemClick(int tag,MenuItem* menuItem,int index)
{
    SoundPlayer::getInstance()->playEffect(selectSound);
    m_iCurrtIndex = index;
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if (m_iLastIndex != -1) {
        if (m_iCurrtIndex != m_iLastIndex) {
            // 重新加载类别资源
            m_pLastItem->setNormalImage(Sprite::create(listItem1[m_iLastIndex].c_str()));
            item->setNormalImage(Sprite::create(listItem1_h[m_iCurrtIndex].c_str()));
            
            m_pUnitViewBg->stopAllActions();
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
            m_pUnitScrollView->setScrollEnable(false);
            m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((m_pUnitViewBg->getPositionX() - ORIGIN_POINT.x) / 600.0f, ORIGIN_POINT), CallFunc::create([=](){
                
                m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
            }), MoveTo::create(0.25f, END_POINT), CallFunc::create([=](){
            
                m_pUnitScrollView->m_pScrollView->setTouchEnabled(true);
                m_pUnitScrollView->setScrollEnable(true);
            }), nullptr));

        }else{
            item->setNormalImage(Sprite::create(listItem1[m_iCurrtIndex].c_str()));
            
            m_pUnitViewBg->stopAllActions();
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
            m_pUnitScrollView->setScrollEnable(false);
            m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((m_pUnitViewBg->getPositionX() - ORIGIN_POINT.x) / 600.0f, ORIGIN_POINT), CallFunc::create([=](){
                
                m_pUnitViewBg->setVisible(false);
            }), nullptr));
            m_iLastIndex = -1;
            
            return;
        }
    }else {
        item->setNormalImage(Sprite::create(listItem1_h[m_iCurrtIndex].c_str()));
        
        m_pUnitViewBg->stopAllActions();
        m_pUnitScrollView->m_pScrollView->setTouchEnabled(false);
        m_pUnitScrollView->setScrollEnable(false);
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
        m_pUnitViewBg->setVisible(true);
        m_pUnitViewBg->runAction(Sequence::create(MoveTo::create((END_POINT.x - m_pUnitViewBg->getPositionX()) / 600.0f, END_POINT), CallFunc::create([=](){
        
            m_pUnitScrollView->m_pScrollView->setTouchEnabled(true);
            m_pUnitScrollView->setScrollEnable(true);
        }), nullptr));
    }
    
    m_iLastIndex = m_iCurrtIndex;
    m_pLastItem = item;
}

void DecorationConeLayer::onGridItemCallback(int tag,MenuItem* menuItem,int index)
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
        case eSyrup_tag:
        {
            if (!m_pSyrup) {
                m_pSyrup = Sprite::create(str);
                kAdapterScreen->setExactPosition(m_pSyrup, 509, 400);
                m_pContentLayer->addChild(m_pSyrup, 10);
            }else{
                m_pSyrup->setTexture(str);
            }
            g_iSyrupIndex = index;
            
            SoundPlayer::getInstance()->playEffect(decSound);
            auto par = ParticleSystemQuad::create("plists/decorate.plist");
            par->setPosition(m_pSyrup->getContentSize() / 2 + Size(0, -35));
            m_pSyrup->addChild(par, 100);
            par->setAutoRemoveOnFinish(true);
        }
            break;
            
        case eSprink_tag:
        {
            auto pSprite = ToolSprite::create(str);
            pSprite->setPosition(m_oTouchPos);
            pSprite->setDrag(true);
            pSprite->setMoveRectByNode(m_pMoveRect, true);
            m_pContentLayer->addChild(pSprite, 20);
            pSprite->setRotation(rand() % 360);
            
            // 曲线进场
            ccBezierConfig oBezier;
            oBezier.controlPoint_1 = Vec2(200 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            oBezier.controlPoint_2 = Vec2(350 + kAdapterScreen->g_oOffset.x, 600 + kAdapterScreen->g_oOffset.y);
            oBezier.endPosition = Vec2(508 + rand() % 120 - 60 + kAdapterScreen->g_oOffset.x, 360 + rand() % 100 - 50 + kAdapterScreen->g_oOffset.y);
            auto pBezierTo = BezierTo::create(0.8f, oBezier);
            pSprite->runAction(Sequence::create(pBezierTo, CallFunc::create([=](){
            
                pSprite->setDelegate(this);
            }), nullptr));
            
        }
            break;
            
        case eSticker_tag:
        {
            if (!m_pSticker) {
                m_pSticker = Sprite::create(str);
                kAdapterScreen->setExactPosition(m_pSticker, 508, 221);
                this->addChild(m_pSticker, 301);
            }else{
                m_pSticker->setTexture(str);
            }
            g_iStickerIndex = index;
            
            SoundPlayer::getInstance()->playEffect(decSound);
            auto par = ParticleSystemQuad::create("plists/decorate.plist");
            par->setPosition(m_pSticker->getContentSize() / 2 + Size(0, 15));
            m_pSticker->addChild(par, 100);
            par->setAutoRemoveOnFinish(true);
        }
            break;
    }
}

void DecorationConeLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eResetBtn_tag) {
        SoundPlayer::getInstance()->playEffect(buttonSound);
        isReturn = true;
        
        Dialog* dialog = Dialog::create((void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
        dialog->setContentText("Do you want to reset your Ice Cream?");
        dialog->setCallback(this);
        this->addChild(dialog, INT16_MAX);
    }
}

void DecorationConeLayer::onNextCallback()
{
    if (m_pSticker) {
        m_pSticker->removeAllChildrenWithCleanup(true);
    }
    if (m_pSyrup) {
        m_pSyrup->removeAllChildrenWithCleanup(true);
    }
    //////////////////////////////////////////////////////////////////////////
    string str = FileUtility::getStoragePath() + "cone.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    m_pBottomLayer->visit();
    m_pContentLayer->visit();
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    rt->saveToFile("cone.png");
    
    this->scheduleOnce(schedule_selector(DecorationConeLayer::enterNextScene), 0.1f);
 
}

void DecorationConeLayer::enterNextScene(float ft)
{
    if (!g_bIsFlag) {
        SceneManager::getInstance()->enterMakeIceCreamStep01Scene();
    }else{
        g_bIsFlag = false;
        Director::getInstance()->popScene();
    }
}

void DecorationConeLayer::onNegativeClick(void* type)
{
    BaseLayer::onNegativeClick(type);
}

void DecorationConeLayer::onPositiveClick(void* type)
{
    isReturn = false;
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReset){
        SoundPlayer::getInstance()->playEffect(resetSound);
        m_pContentLayer->removeAllChildrenWithCleanup(true);
        
        m_pSyrup = nullptr;
        g_iSyrupIndex = -1;
        
        if (m_pSticker) {
            m_pSticker->removeFromParentAndCleanup(true);
            m_pSticker = nullptr;
            g_iStickerIndex = -1;
        }
    }
}

void DecorationConeLayer::onEnter()
{
    BaseLayer::onEnter();
    
    if (g_bIsFromShop) {
        g_bIsFromShop = false;
        
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_iCurrtIndex);
        m_pUnitScrollView->m_pScrollView->setContentOffset(m_oListViewOffset);
        this->removeLockScreenLayer();
    }
    
    if (!this->getNextBtnEnabled()) {
        this->setNextBtnIsEnabled(true);
    }
    
    if (g_bIsFlag) {
        g_iStickerIndex = -1;
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
}

void DecorationConeLayer::onExit()
{
    BaseLayer::onExit();
}