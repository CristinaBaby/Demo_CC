//
//  RecipeLayer.cpp
//  Muffin
//
//  Created by huxixiang on 15/11/27.
//
//

#include "RecipeLayer.hpp"
#include "AdapterScreen.h"
#include "SoundPlayer.h"
#include "GameData.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"

RecipeLayer::RecipeLayer():m_pBg(nullptr),m_pCloseBtn(nullptr)
{
    
}

RecipeLayer::~RecipeLayer()
{
    
}

RecipeLayer* RecipeLayer::create(const Color4B& color)
{
    auto layer = new RecipeLayer();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE_NULL(layer);
    return nullptr;
}

bool RecipeLayer::initWithColor(const Color4B& color)
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!LayerColor::initWithColor(color));
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initUI());
        isInit = true;
    }while(0);
    
    return isInit;
}

bool RecipeLayer::initData()
{
    
    return true;
}

bool RecipeLayer::initUI()
{
    // add bg
    m_pBg = Sprite::create("recipe/pupop_bg1.png");
    kAdapterScreen->setExactPosition(m_pBg, 320, 480);
    this->addChild(m_pBg);
    m_pBg->setScale(0.9f);
    m_pBg->runAction(Sequence::create(ScaleTo::create(0.25f, 0.95f), ScaleTo::create(0.3f, 0.9f), nullptr));
    
    m_pCloseBtn = Button::create("recipe/pupop_btn_close.png");
    m_pCloseBtn->setPosition(m_pBg->getContentSize() + Size(-12, -12));
    m_pBg->addChild(m_pCloseBtn, INT16_MAX);
    m_pCloseBtn->addTouchEventListener(CC_CALLBACK_2(RecipeLayer::onButtontouchCallback, this));
    
    ////////////////////////////////////////////////////////
    auto pIngredientText = Sprite::create("recipe/pupop_text4.png");
    pIngredientText->setPosition(310, 692);
    m_pBg->addChild(pIngredientText);
    
    Vector<Sprite*> m_vTypes1;
    m_vTypes1.clear();
    Sprite* pContent1 = Sprite::create("recipe/pupop_text5.png");
    m_vTypes1.pushBack(pContent1);
    // add scrollview
    auto pAdapter1 = KSSimpleAdapter::create(m_vTypes1);
    auto pScrollView1 = KDGridView::create(Size(298, 235));
    pScrollView1->setDirection(extension::ScrollView::Direction::VERTICAL);
    pScrollView1->m_iCol = 1;
    pScrollView1->setPosition(m_pBg->getContentSize() / 2 + Size(100, 156));
    // set adapter
    pScrollView1->setAdapter(pAdapter1);
    pScrollView1->m_bNeedAdjust = false;
    m_pBg->addChild(pScrollView1);
    
    pScrollView1->m_pScrollView->setContentOffset(Vec2(0, 7));
    pScrollView1->m_pMenu->setEnabled(false);
    pScrollView1->setScrollEnable(false);
    pScrollView1->m_pScrollView->getContainer()->runAction(Sequence::create(MoveBy::create(0.45f, Vec2(0, -42)), CallFunc::create([=](){
        
        pScrollView1->m_pMenu->setEnabled(true);
        pScrollView1->setScrollEnable(true);
        
    }), nullptr));
    
    ////////////////////////////////////////////////////////
    auto pMethodText = Sprite::create("recipe/pupop_text3.png");
    pMethodText->setPosition(75, 355);
    m_pBg->addChild(pMethodText);
    
    Vector<Sprite*> m_vTypes2;
    m_vTypes2.clear();
    Sprite* pContent2 = Sprite::create("recipe/pupop_text6.png");
    m_vTypes2.pushBack(pContent2);
    // add scrollview
    auto pAdapter2 = KSSimpleAdapter::create(m_vTypes2);
    auto pScrollView2 = KDGridView::create(Size(530, 233));
    pScrollView2->setDirection(extension::ScrollView::Direction::VERTICAL);
    pScrollView2->m_iCol = 1;
    pScrollView2->setPosition(m_pBg->getContentSize() / 2 + Size(0, -175));
    // set adapter
    pScrollView2->setAdapter(pAdapter2);
    pScrollView2->m_bNeedAdjust = false;
    m_pBg->addChild(pScrollView2);
    
    pScrollView2->m_pScrollView->setContentOffset(Vec2(0, 3));
    pScrollView2->m_pMenu->setEnabled(false);
    pScrollView2->setScrollEnable(false);
    pScrollView2->m_pScrollView->getContainer()->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, -253)), CallFunc::create([=](){
        
        pScrollView2->m_pMenu->setEnabled(true);
        pScrollView2->setScrollEnable(true);
        
    }), nullptr));
    
    return true;
}

void RecipeLayer::onButtontouchCallback(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        m_pCloseBtn->setTouchEnabled(false);
        SoundPlayer::getInstance()->playEffect(buttonSound);
        
        this->removeFromParentAndCleanup(true);
    }
    
    if (type == Widget::TouchEventType::CANCELED)
    {
        onButtontouchCallback(pSender,Widget::TouchEventType::ENDED);
    }
}

void RecipeLayer::onEnter()
{
    Layer::onEnter();
    
    EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
    
    pListener->setSwallowTouches(true);
    
    pListener->onTouchBegan = [=](Touch* touch, Event* event){
        
        
        return this->isVisible();
    };
    pListener->onTouchMoved = [=](Touch* touch, Event* event){
        
        
    };
    pListener->onTouchEnded = [=](Touch* touch, Event* event){
        
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

void RecipeLayer::onExit()
{
    Layer::onExit();
}