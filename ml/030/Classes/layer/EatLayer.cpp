//
//  EatLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-10.
//
//

#include "EatLayer.h"
#include "AdapterScreen.h"
#include "ToolSprite.h"
#include "Dialog.h"
#include "FavoriteManager.h"
#include "AudioHelp.h"
#include "ActionUtils.h"
#include "IAPManager.h"

enum buttonType
{
    KBACKBUTTON,
    KFAVORITEBUTTON,
    KHOMEBUTTON,
    
};
EatLayer::EatLayer():m_pSpriteTip(nullptr)
{
    
}
EatLayer::~EatLayer()
{
    
}
bool EatLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        initMenu();
        
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void EatLayer::initUI()
{
    __String* bg_str = nullptr;
    if(g_dec_bg == -1)
        bg_str = __String::create("images/dec/decoration_bg.jpg");
    else
        bg_str = __String::createWithFormat("images/dec/categery/backgrounds/backgrounds%d.jpg",g_dec_bg);

    auto bg = Sprite::create(bg_str->getCString());
    kAdapterScreen->setExactPosition(bg, 480, 320);
    this->addToBackGroundLayer(bg,5);
    
    __String* cone_str = nullptr;
    if(g_dec_cone == -1)
        cone_str = __String::create("images/dec/categery/cone/cone0.png");
    else
        cone_str = __String::createWithFormat("images/dec/categery/cone/cone%d.png",g_dec_cone);

    
    
    auto pCone = Sprite::create(cone_str->getCString());
    kAdapterScreen->setExactPosition(pCone, 680, 20);
    this->addToContentLayer(pCone,2);
    
    
    auto _ice = Sprite::create(__String::createWithFormat("images/dec/cream_%s.png",g_choose_waste.c_str())->getCString());
    _ice->setAnchorPoint(Vec2(0.5,0));
    _ice->setPosition(Vec2(143,570));
    pCone->addChild(_ice);
    
    
    auto pCone1 = Sprite::create(cone_str->getCString());
    kAdapterScreen->setExactPosition(pCone1, 680, 20);
    this->addToContentLayer(pCone1,2);
    
    
    auto _ice1 = Sprite::create(__String::createWithFormat("images/dec/cream_%s.png",g_choose_waste.c_str())->getCString());
    _ice1->setAnchorPoint(Vec2(0.5,0));
    _ice1->setPosition(Vec2(143,570));
    pCone1->addChild(_ice1);
    
    Color3B color = pCone1->getColor();
    pCone1->setColor(Color3B(color.r*0.5,color.g*0.5,color.b*0.5));
    
    
    m_pEatAgain = ToolSprite::create("images/dec/eat again.png");
    kAdapterScreen->setExactPosition(m_pEatAgain, 480, 100 - 500,Vec2(0.5, 1),kBorderNone,kBorderTop);
    m_pEatAgain->setDelegate(this);
    m_pEatAgain->setIsMove(false);
    m_pEatAgain->setTag(100);
    m_pEatAgain->cancelEvent(true);
    this->addToUILayer(m_pEatAgain,20);
    
    
    RenderTexture *rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    
    pCone->visit();
    decorationLayer->visit();
    
    
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->saveToFile("eat.png");
    this->scheduleOnce(schedule_selector(EatLayer::getScribbleCallback), 0.2f);
  
    RenderTexture *rt1 = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt1->begin();
    
    pCone1->visit();
    
    
    
    rt1->end();
    Director::getInstance()->getRenderer()->render();
    rt1->saveToFile("eat1.png");
    this->scheduleOnce(schedule_selector(EatLayer::_getScribbleCallback), 0.2f);
    
    
    pCone1->setVisible(false);
    pCone->setVisible(false);
}
void EatLayer::initMenu()
{
    auto favBtn = ui::Button::create("images/button/btn_fav@2x.png");
    favBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    favBtn->setTag(KFAVORITEBUTTON);
    kAdapterScreen->setExactPosition(favBtn, 120, 10,Vec2(1, 1),kBorderRight,kBorderTop);
    this->addToUILayer(favBtn, 20);
    
    auto homeBtn = ui::Button::create("images/button/btn_home@2x.png");
    homeBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    homeBtn->setTag(KHOMEBUTTON);
    kAdapterScreen->setExactPosition(homeBtn, 10, 10,Vec2(1, 1),kBorderRight,kBorderTop);
    this->addToUILayer(homeBtn, 20);
    
    auto backBtn = ui::Button::create("images/button/btn_back@2x.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    backBtn->setTag(KBACKBUTTON);
    kAdapterScreen->setExactPosition(backBtn, 10, 320,Vec2(0, 0),kBorderLeft,kBorderNone);
    this->addToUILayer(backBtn, 20);
    
}

void EatLayer::onEnter()
{
    BaseLayer::onEnter();
    
    
}
void EatLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    _showTip();
    if(kIAPManager->isShowAds()){
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
}
void EatLayer::onExit()
{
    BaseLayer::onExit();
   
}
void EatLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    m_pEatAgain->cancelEvent(true);
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,500),0.2);

    this->scheduleOnce(schedule_selector(EatLayer::getScribbleCallback), 0.2f);
    this->scheduleOnce(schedule_selector(EatLayer::_getScribbleCallback), 0.2f);
}

void EatLayer::onBtnCallBack(Ref* sender)
{
    
}
void EatLayer::eatAgain()
{
    if(node&&nodeBom){
      
        AudioHelp::getInstance()->stopLoofEffect();
        node->disable();
        node->removeFromParent();
        node = nullptr;
        
        nodeBom->disable();
        nodeBom->removeFromParent();
        nodeBom = nullptr;
       
        
//        this->unschedule(schedule_selector(EatLayer::getScribbleCallback));
//        this->unschedule(schedule_selector(EatLayer::_getScribbleCallback));
        m_pEatAgain->cancelEvent(false);
        
        ScaleBy* move = ScaleBy::create(0.5, 1.1);
        m_pEatAgain->runAction(RepeatForever::create(Sequence::create(move,move->reverse(),NULL)));
        
    }


}
void EatLayer::onScribbleTouchOver()
{
    if(node && nodeBom){
        
        node->disable();
        nodeBom->disable();
    }
    auto func = CallFunc::create([&](){
        
        eatAgain();
    });
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,-500),1.0,func);

}
void EatLayer::onScribbleTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
    AudioHelp::getInstance()->playEffect("eat.mp3");
    
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/biting.plist");
    emitter1->setDuration(0.2);
    //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    emitter1->setPosition(pTouch->getLocation());
    this->getParent()->addChild(emitter1, 30);
}
void EatLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case KFAVORITEBUTTON:
                {
                    m_pFavoriteTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
                    m_pFavoriteTexture->retain();
                    m_pFavoriteTexture->begin();
                    m_pBackGroundLayer->visit();
                    m_pContentLayer->visit();
                    m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    
                    Sprite* brush = Sprite::create("images/shop_fav/mask.png");
                    Sprite* mixFavImage = Sprite::create("images/shop_fav/frame.png");
                    Sprite* bg = Sprite::createWithTexture(m_pFavoriteTexture->getSprite()->getTexture());
                    bg->setFlippedY(true);

                    bg->setScale(440/this->getContentSize().height);
                   
                    
                    m_pFavoriteTexture = RenderTexture::create(brush->getContentSize().width, brush->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
                    m_pFavoriteTexture->retain();
                    m_pFavoriteTexture->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    BlendFunc ccbFunc = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
                    brush->setBlendFunc(ccbFunc);
                    brush->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    m_pFavoriteTexture->begin();
                    mixFavImage->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    mixFavImage->visit();
                    bg->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    bg->visit();
                    brush->visit();
                    m_pFavoriteTexture->end();

                    m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    Director::getInstance()->getRenderer()->render();
                    
//                    auto sp =  Sprite::createWithTexture(m_pFavoriteTexture->getSprite()->getTexture());
//                    sp->setPosition(Vec2(500,300));
//                    this->addChild(sp,20);
                    
                    
                    scheduleOnce(schedule_selector(EatLayer::saveToPhoto), 0.05f);
                    btn->setTouchEnabled(false);
                }
                    break;
                case KHOMEBUTTON:
                {
                    AudioHelp::getInstance()->playSelectEffect();
                    Dialog* dialog= Dialog::create(Size(480,350), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
                    //                    dialog -> setAnchorPoint(Vec2(0,0));
                    //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
                    dialog -> setAnchorPoint(Vec2(0,0));
                    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                    dialog->setContentText(__String::createWithFormat("Do you want to start over with a new food?")->getCString());
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    this->addToUILayer(dialog, 9999);
                    m_bIsShowReturn=true;
                }
                    break;
                case KBACKBUTTON:
                {
                    AudioHelp::getInstance()->playEffect("previous page.mp3");
                    SceneManager::getInstance()->popScene();
                    remove(__String::createWithFormat("%seat.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    remove(__String::createWithFormat("%seat1.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    node->removeFromParent();
                    nodeBom->removeFromParent();
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}


void EatLayer::_showTip()
{
    
    if (!m_pSpriteTip) {
        m_pSpriteTip = Sprite::create("images/button/tap@2x.png");
        this->addToUILayer(m_pSpriteTip, 255);
        
        m_pSpriteTip->setZOrder(101);
    }
    showTip();
}
void EatLayer::showTip()
{
    if (m_pSpriteTip) {
        m_pSpriteTip->setPosition(Vec2(visibleSize.width+534.30f,577.51f));
        m_pSpriteTip->runAction(Sequence::create(MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(534.30f,577.51f))),
                                                 DelayTime::create(1.0f),
                                                 MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(-534.30f,577.51f))),
                                                 nullptr));
        
    }
}

void EatLayer::onNegativeClick(void*)
{
    m_bIsShowReturn = false;
}

void EatLayer::getScribbleCallback(float ft)
{
    
    node  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height));
    string str = FileUtils::getInstance()->getWritablePath()+"eat.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    node->getScribble()->antiAliasing(node->getCanvas());
    node->useTarget(str);
    node->setDelegate(this);
    node->useBrush(str,Scribble::eBrush);
    node->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(node, 280, 320);
    this->addToContentLayer(node, 10);
    node->enable();
    node->enableTouchMoved(false);
    node->useBrush("images/button/eat_mask1.png",Scribble::eEaser);
    
}
void EatLayer::_getScribbleCallback(float ft)
{
    
    nodeBom  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height));
    string str = FileUtils::getInstance()->getWritablePath()+"eat1.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    nodeBom->getScribble()->antiAliasing(nodeBom->getCanvas());
    nodeBom->useTarget(str);
    nodeBom->setDelegate(nullptr);
    nodeBom->useBrush(str,Scribble::eBrush);
    nodeBom->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(nodeBom, 280, 320);
    this->addToContentLayer(nodeBom, 5);
    nodeBom->enable();
    nodeBom->enableTouchMoved(false);
    nodeBom->useBrush("images/button/eat_mask.png",Scribble::eEaser);
    
}


void EatLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        
        
        if(kIAPManager->isShowAds())
        {
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
        Director::getInstance()->popToRootScene();
        SceneChangeManager->enterHomeScene();
        
      
        
    }
    if (lType == kDialogTypeNone)
    {
        
    }
    
}

void EatLayer::saveToPhoto(float ft)
{
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    string rootPath = FavoriteManager::getInstance()->saveFavoritePath(FAVORITE_DIR);
    Director::getInstance()->getRenderer()->render();
    bool isSus = m_pFavoriteTexture->newImage()->saveToFile(rootPath.c_str(),false);
    
    log("%s",rootPath.c_str());
    addFavoriteCallback(isSus);
    
#else
     FavoriteManager::getInstance()->saveToFavorite(m_pFavoriteTexture,CC_CALLBACK_1(EatLayer::addFavoriteCallback, this),FAVORITE_DIR);
#endif
    
}
void EatLayer::addFavoriteCallback(bool isSuc)
{
    m_bIsShowReturn=true;
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your food has been added to your favorites.")->getCString());
    }
    else
    {
        dialog->setContentText("Save Favorite Failed!");
    }
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KFAVORITEBUTTON));
    btn->setTouchEnabled(true);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
}
// 监听Android返回键事件
void EatLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(__String::createWithFormat("Are you sure you want to start over?")->getCString());
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}


