//
//  IceCreamIceCreamEatLayer.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "IceCreamEatLayer.h"
#include "AdapterScreen.h"
#include "ToolSprite.h"
#include "Dialog.h"
#include "CFSystemFunction.h"
#include "FavoriteManager.h"
#include "AudioHelp.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"
#include "FillMaterialModel.h"
#include "SceneManager.h"

enum buttonType
{
    KBACKBUTTON,
    KFAVORITEBUTTON,
    KHOMEBUTTON,
    kNEXTBUTTON,
    
};
IceCreamEatLayer::IceCreamEatLayer()
{
    
}
IceCreamEatLayer::~IceCreamEatLayer()
{
    
}
bool IceCreamEatLayer::init()
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

void IceCreamEatLayer::initUI()
{
  
    
    auto pBgSprite = Utils::getUISprite("images/ice_cream/decoration/bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 320, 480);
    this->addToBackGroundLayer(pBgSprite, 1);

    auto pBg = Utils::getUISprite("images/ice_cream/decoration/bg1.png");
    kAdapterScreen->setExactPosition(pBg, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToBackGroundLayer(pBg, 2);
    
    
    
    _cupSprite = Sprite::create(__String::createWithFormat("images/ice_cream/1/cup_%d_1.png",G_IceCream_CupNum)->getCString());
    kAdapterScreen->setExactPosition(_cupSprite, 320, 220 ,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(_cupSprite,3);
//        cupSprite->setScale(0.7);
    
//    kAdapterScreen->setExactPosition(G_IceCream_CupSprite, 320, 220,Vec2(0.5,0),kBorderNone,kBorderBottom);
//    this->addChild(G_IceCream_CupSprite,3);
    //    G_IceCream_CupSprite->retain();
    
    //加载盖子
    
        
    _cup = Sprite::create(__String::createWithFormat("images/ice_cream/1/cup_%d.png",G_IceCream_CupNum)->getCString());
    kAdapterScreen->setExactPosition(_cup, 320, 220 ,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(_cup,9);
        
    
    
    m_pSpriteTip = nullptr;
    
    RenderTexture *rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    G_IceCream_CupSprite->visit();
    decorationLayer->visit();
    
    fillDecorationLayer->visit();
    iceDecorationLayer->visit();
    
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->saveToFile("eat.png");
    this->scheduleOnce(schedule_selector(IceCreamEatLayer::getScribbleCallback), 0.2f);
}
void IceCreamEatLayer::initMenu()
{
    auto favBtn = ui::Button::create("images/button/btn_share.png");
    favBtn -> addTouchEventListener(CC_CALLBACK_2(IceCreamEatLayer::touchEvent, this));
    favBtn->setTag(KFAVORITEBUTTON);
    kAdapterScreen->setExactPosition(favBtn, 10, 110 + 100 + 10,Vec2(1, 0),kBorderRight,kBorderBottom);
    this->addToUILayer(favBtn, 20);
    
    auto homeBtn = ui::Button::create("images/button/btn_home.png");
    homeBtn -> addTouchEventListener(CC_CALLBACK_2(IceCreamEatLayer::touchEvent, this));
    homeBtn->setTag(KHOMEBUTTON);
    kAdapterScreen->setExactPosition(homeBtn, 10, 10 + 100 + 100 + 10,Vec2(0, 0),kBorderLeft,kBorderBottom);
    this->addToUILayer(homeBtn, 20);
    
    auto backBtn = ui::Button::create("images/button/btn_back.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(IceCreamEatLayer::touchEvent, this));
    backBtn->setTag(KBACKBUTTON);
    kAdapterScreen->setExactPosition(backBtn, 10, 110,Vec2(0, 0),kBorderLeft,kBorderBottom);
    this->addToUILayer(backBtn, 20);
    
    auto nextBtn = ui::Button::create("images/button/btn_next.png");
    nextBtn -> addTouchEventListener(CC_CALLBACK_2(IceCreamEatLayer::touchEvent, this));
    nextBtn->setTag(kNEXTBUTTON);
    kAdapterScreen->setExactPosition(nextBtn, 10, 110 ,Vec2(1, 0),kBorderRight,kBorderBottom);
    this->addToUILayer(nextBtn, 20);

    m_pEatAgain = ToolSprite::create("images/dec/eat again.png");
    kAdapterScreen->setExactPosition(m_pEatAgain, 320, 100 - 500,Vec2(0.5, 1),kBorderNone,kBorderTop);
    m_pEatAgain->setDelegate(this);
    m_pEatAgain->setIsMove(false);
    m_pEatAgain->cancelEvent(true);
    this->addChild(m_pEatAgain,20);
    
}
void IceCreamEatLayer::onEnter()
{
    BaseLayer::onEnter();
   
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IceCreamEatLayer::eatOver), "eatOver", NULL);

}

void IceCreamEatLayer::eatOver(Ref*)
{
    node->disable();
    node->removeFromParent();
    node = nullptr;
    this->unschedule(schedule_selector(IceCreamEatLayer::getScribbleCallback));
    auto func = CallFunc::create([&](){
        
        eatAgain();
        
    });
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,-500),1.0,func);
}
void IceCreamEatLayer::eatAgain()
{
    m_pEatAgain->cancelEvent(false);
    ScaleBy* move = ScaleBy::create(0.5, 1.1);
    m_pEatAgain->runAction(RepeatForever::create(Sequence::create(move,move->reverse(),NULL)));
}
void IceCreamEatLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
//    G_IceCream_CupSprite->setVisible(false);
}

void IceCreamEatLayer::onExit()
{
    BaseLayer::onExit();
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "eatOver");
}

void IceCreamEatLayer::onBtnCallBack(Ref* sender)
{
    
}
void IceCreamEatLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    m_pEatAgain->cancelEvent(true);
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,500));
    
    
    this->scheduleOnce(schedule_selector(IceCreamEatLayer::getScribbleCallback), 0.2f);
    
}
void IceCreamEatLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    

}
void IceCreamEatLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
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
                    _cupSprite->visit();
                    m_pContentLayer->visit();
                    _cup->visit();
                    m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    
                    //saveToPhoto();
                    AudioHelp::getInstance()->playEffect("button_camera.mp3");
                    scheduleOnce(schedule_selector(IceCreamEatLayer::saveToPhoto), 0.01f);
                    btn->setTouchEnabled(false);
                }
                    break;
                case KHOMEBUTTON:
                {
                    AudioHelp::getInstance()->playSelectEffect();
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
                    break;
                case KBACKBUTTON:
                {
                    AudioHelp::getInstance()->playEffect("previous page.mp3");
                    SceneManager::getInstance()->popScene();
                    remove(__String::createWithFormat("%seat.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    if (node) {
                        node->removeFromParent();
                    }
                    
                    
                    if (G_IceCream_CupSprite) {
                        G_IceCream_CupSprite->setVisible(true);
                    }
                                       
                }
                    break;
                case kNEXTBUTTON:
                {
                    //回到选择游戏界面
                    Director::getInstance()->popToRootScene();
                    SceneChangeManager->enterSelectScene();
                    
                    if (UserDefault::getInstance()->getIntegerForKey(IcePop, 0) != 2) {
                        UserDefault::getInstance()->setIntegerForKey(IcePop, 1);
                        UserDefault::getInstance()->flush();
                        
                    }
                    
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


void IceCreamEatLayer::_showTip()
{
    
    if (!m_pSpriteTip) {
        m_pSpriteTip = Utils::getUISprite("images/button/tap@2x.png");
        this->addToUILayer(m_pSpriteTip, 255);
        m_pSpriteTip->setZOrder(101);
    }
    showTip();
}
void IceCreamEatLayer::showTip()
{
    if (m_pSpriteTip) {
        m_pSpriteTip->setPosition(Vec2(visibleSize.width+534.30f,577.51f));
        m_pSpriteTip->runAction(Sequence::create(MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(534.30f,577.51f))),
                                                 DelayTime::create(1.0f),
                                                 MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(-534.30f,577.51f))),
                                                 nullptr));
        
    }
}

void IceCreamEatLayer::onNegativeClick(void*)
{
    
}

void IceCreamEatLayer::getScribbleCallback(float ft)
{
    
    node  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height),0);
    string str = FileUtils::getInstance()->getWritablePath()+"eat.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    node->useTarget(str);
    node->useBrush(str,Scribble::eBrush);
    node->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(node, 320, 480);
    this->addToContentLayer(node, 10);
    node->enable();
    node->enableTouchMoved(false);
//    node->enableTouchEnded(false);
    node->useBrush("images/button/eat_mask.png",Scribble::eEaser);
    
}


void IceCreamEatLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
   
//        if (kIAPManager->isShowAds()) {
//            STAds ads;
//            ads.preloadCrosspromo();
//            
//            
//            
//            
//        }
        Director::getInstance()->popToRootScene();
        auto scene = SceneChangeManager->enterHomeScene();
        //cross
        if (kIAPManager->isShowAds()) {
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true,scene);
            
        }
        if (UserDefault::getInstance()->getIntegerForKey(IcePop, 0) != 2) {
            UserDefault::getInstance()->setIntegerForKey(IcePop, 1);
            UserDefault::getInstance()->flush();
            
        }
        
        
    }
    if (lType == kDialogTypeNone)
    {
        
    }
    
}

void IceCreamEatLayer::saveToPhoto(float ft)
{
    Image* image1 = m_pFavoriteTexture->newImage();
    CFSystemFunction sys;
    sys.saveToAlbum(image1, CC_CALLBACK_1(IceCreamEatLayer::addFavoriteCallback, this));
}

void IceCreamEatLayer::addFavoriteCallback(bool isSuc)
{
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your photo has been saved to your album!")->getCString());
    }
    else
    {
        dialog->setContentText("Fail to save photo!");
    }
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KFAVORITEBUTTON));
    btn->setTouchEnabled(true);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
}



// 监听Android返回键事件
void IceCreamEatLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            isReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(__String::createWithFormat("Are you sure you want to start over with a new %s?",MAKERNAME)->getCString());
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}
