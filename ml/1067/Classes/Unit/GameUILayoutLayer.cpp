
#include "GameUILayoutLayer.h"
#include "Heads.h"
#include "GameUIEvent.h"

GameUILayoutLayer::GameUILayoutLayer()
{
    m_pHomeStart = nullptr;
    m_pHomeRestore = nullptr;
    m_pHomeMore = nullptr;
    m_pHomeFav = nullptr;
    m_pHomeShop = nullptr;
    m_pHomeRate = nullptr;
    m_pHomePolicy = nullptr;
    
    m_pNext = nullptr;
    m_pBack = nullptr;
    m_pReset = nullptr;
    
    m_pFav = nullptr;
    m_pEmail = nullptr;
    m_pHome = nullptr;
    m_pPhoto = nullptr;
    m_pMap = nullptr;
    m_pBg = nullptr;
    m_pEatAgain = nullptr;
    
    onButtonClicked = nullptr;
}

GameUILayoutLayer::~GameUILayoutLayer()
{
    
}
bool GameUILayoutLayer::init()
{
    if ( !Node::init() )
    {
        return false;
    }

    return true;
}

void GameUILayoutLayer::showHomeUILayout(bool animate)
{
//    if (!m_pHomeMore) {
//        m_pHomeMore = _createButton("content/start/btn_more.png");
//        this->addChild(m_pHomeMore);
//    }
    if (!m_pHomeShop) {
        m_pHomeShop = _createButton("content/start/btn_shop.png");
        this->addChild(m_pHomeShop);
    }
    if (!m_pHomeStart) {
        m_pHomeStart = _createButton("content/start/btn_start.png");
        this->addChild(m_pHomeStart);
    }
//    if (!m_pHomeRate) {
//        m_pHomeRate = _createButton("content/start/btn_rate.png");
//        this->addChild(m_pHomeRate);
//    }
    
    if (!m_pHomePolicy) {
        m_pHomePolicy = _createButton("content/start/btn_pp.png");
        this->addChild(m_pHomePolicy);
    }
    
    m_pHomeStart->setTag(GameUILayoutLayer::eUIButtonTagHomeStart);
//    m_pHomeMore->setTag(GameUILayoutLayer::eUIButtonTagHomeMore);
    m_pHomeShop->setTag(GameUILayoutLayer::eUIButtonTagHomeShop);
    //    m_pHomeRate->setTag(GameUILayoutLayer::eUIButtonTagHomeRate);
    m_pHomePolicy->setTag(GameUILayoutLayer::eUIButtonTagPolicy);
    
    if (gNoneIap) {
        m_pHomeShop->setEnabled(false);
        m_pHomeShop->setVisible(false);
    }
    if (!animate) {
//        CMVisibleRect::setPosition(m_pHomeRate, 15, 15,kBorderLeft,kBorderTop);
//        CMVisibleRect::setPosition(m_pHomeMore, 15, 15,kBorderRight,kBorderBottom);
        CMVisibleRect::setPosition(m_pHomeShop, 165, 15,kBorderRight,kBorderBottom);
        CMVisibleRect::setPositionAdapted(m_pHomeStart, 320, 400);
    }else{
        CMVisibleRect::setPosition(m_pHomePolicy, 5, 5,kBorderLeft,kBorderTop);
//        CMVisibleRect::setPosition(m_pHomeRate, 15, 15,kBorderLeft,kBorderTop);
//        CMVisibleRect::setPosition(m_pHomeMore, 15, 15,kBorderLeft,kBorderBottom);
        CMVisibleRect::setPosition(m_pHomeShop, 15, 15,kBorderRight,kBorderBottom);
        CMVisibleRect::setPositionAdapted(m_pHomeStart, 320, 240);

        m_pHomeStart->setEnabled(false);
//        m_pHomeMore->setEnabled(false);
        m_pHomeShop->setEnabled(false);
//        m_pHomeRate->setEnabled(false);
        
//        m_pHomeMore->setOpacity(0);
        m_pHomeShop->setOpacity(0);
//        m_pHomeRate->setOpacity(0);
        m_pHomeStart->setOpacity(0);
        m_pHomePolicy->setOpacity(0);
//        _showHomeButtonAnimation(m_pHomeRate,0.5,1.2);
//        _showHomeButtonAnimation(m_pHomeMore,0.5,0.5);
        _showHomeButtonAnimation(m_pHomePolicy,0.5,0.5);
        if (!gNoneIap) {
            _showHomeButtonAnimation(m_pHomeShop,0.5,0.5);
        }
        m_pHomeStart->setScale(0.01);
        m_pHomeStart->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Spawn::create(FadeIn::create(0.6),
                                                               RotateBy::create(1, 360*2),
                                                               ScaleTo::create(1, 1), NULL),
                                                 CallFunc::create([=]()
                                                                  {
                                                                      m_pHomeStart->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.1),
                                                                                                                                     ScaleTo::create(0.5, 1),
                                                                                                                                      NULL)));
                                                                      m_pHomeStart->setEnabled(true);
                                                                  }),
                                                 NULL));
    }
}

void GameUILayoutLayer::showMakeUILayout(bool animate)
{
    showNextLayout(animate);
    showBackLayout(animate);
}

void GameUILayoutLayer::showNextLayout(bool animate)
{
    if (!m_pNext) {
        m_pNext = _createButton("content/common/button/btn_next.png");
        this->addChild(m_pNext);
    }
    m_pNext->setVisible(true);
    m_pNext->setEnabled(true);
    
    m_pNext->setTag(GameUILayoutLayer::eUIButtonTagNext);
    
    m_pNext->stopAllActions();
    m_pNext->setRotation(0);
    
    if (!animate) {
        CMVisibleRect::setPosition(m_pNext, 10, 110,kBorderRight,kBorderBottom);
    }else{
        CMVisibleRect::setPosition(m_pNext, 10, 110,kBorderRight,kBorderBottom);
        m_pNext->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(ScaleTo::create(0.5, 1.1)),
                                                                  ScaleTo::create(0.5, 1),
                                                                  DelayTime::create(2),
                                                                  NULL)));
    }
    
}

void GameUILayoutLayer::showBackLayout(bool animate)
{
    if (!m_pBack) {
        m_pBack = _createButton("content/common/button/btn_back.png");
        this->addChild(m_pBack);
    }
    
    m_pBack->setVisible(true);
    m_pBack->setEnabled(true);
    m_pBack->setTag(GameUILayoutLayer::eUIButtonTagBack);
    
    m_pBack->stopAllActions();
    m_pBack->setRotation(0);
    
    if (!animate) {
        CMVisibleRect::setPosition(m_pBack, 10, 110,kBorderLeft,kBorderBottom);
    }else{
        CMVisibleRect::setPosition(m_pBack, 10, 110,kBorderLeft,kBorderBottom);
        m_pBack->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(ScaleTo::create(0.5, 1.1)),
                                                                  ScaleTo::create(0.5, 1),
                                                                  DelayTime::create(2),
                                                                  NULL)));
    }
    
}

void GameUILayoutLayer::showResetLayout(bool animate)
{
    if (!m_pReset) {
        m_pReset = _createButton("content/common/button/btn_reset.png");
        this->addChild(m_pReset);
    }
    m_pReset->setVisible(true);
    m_pReset->setEnabled(true);
    
    m_pReset->setTag(GameUILayoutLayer::eUIButtonTagReset);
    
    m_pReset->stopAllActions();
    m_pReset->setRotation(0);
    
    if (!animate) {
        CMVisibleRect::setPosition(m_pReset, 10, 220,kBorderRight,kBorderBottom);
    }else{
        CMVisibleRect::setPosition(m_pReset, 10, 220,kBorderRight,kBorderBottom);
        m_pReset->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(JumpBy::create(3, Vec2::ZERO, 10, 1)),
                                                                  DelayTime::create(5),
                                                                  NULL)));
    }
    
}

void GameUILayoutLayer::showEatAgainLayout(bool isDrink,bool animate)
{
    if (!m_pEatAgain) {
        if (isDrink) {
            m_pEatAgain = _createButton("content/common/button/btn_eat.png");
        }else{
            m_pEatAgain = _createButton("content/common/button/btn_eat.png");
        }
        this->addChild(m_pEatAgain);
    }
    
    m_pEatAgain->setVisible(true);
    m_pEatAgain->setEnabled(true);
    m_pEatAgain->setTag(GameUILayoutLayer::eUIButtonTagEatAgain);
    
    m_pEatAgain->stopAllActions();
    m_pEatAgain->setRotation(0);
    
    if (!animate) {
        CMVisibleRect::setPositionAdapted(m_pEatAgain, 320, 600);
    }else{
        m_pEatAgain->setEnabled(false);
        CMVisibleRect::setPositionAdapted(m_pEatAgain, 320, 600+visibleSize.height);
        m_pEatAgain->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(2, Vec2(0, -visibleSize.height))),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pEatAgain->setEnabled(true);
                                                                     m_pEatAgain->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(ScaleTo::create(1, 1.1)),
                                                                                                                                   DelayTime::create(1),
                                                                                                                                   ScaleTo::create(1, 1),
                                                                                                                                   NULL)));
                                                                 }), NULL));
    }
    
}


void GameUILayoutLayer::showDecorateUILayout(bool animate)
{
    
}

void GameUILayoutLayer::showShareUILayout(bool animate)
{
//    if (!m_pFav) {
//        m_pFav = _createButton("content/common/button/btn_fav.png");
//        this->addChild(m_pFav);
//    }
//    if (!m_pPhoto) {
//        m_pPhoto = _createButton("content/common/button/btn_photo.png");
//        this->addChild(m_pPhoto);
//    }
//    if (!m_pEmail) {
//        m_pEmail = _createButton("content/common/button/btn_email.png");
//        this->addChild(m_pEmail);
//    }
    if (!m_pHome) {
        m_pHome = _createButton("content/common/button/btn_home.png");
        this->addChild(m_pHome);
    }
    if (!m_pBg) {
        m_pBg = _createButton("content/common/button/btn_summer.png");
        this->addChild(m_pBg);
    }
//
////    CMVisibleRect::setPositionAdapted(m_pFav, 900, 480,kBorderRight,kBorderTop);
//    CMVisibleRect::setPosition(m_pPhoto, 10, 120,kBorderLeft,kBorderTop);
//    CMVisibleRect::setPosition(m_pEmail, 10, 10,kBorderLeft,kBorderTop);
    CMVisibleRect::setPosition(m_pHome, 10, 10,kBorderRight,kBorderTop);
    CMVisibleRect::setPosition(m_pBg, 10, 120,kBorderRight,kBorderTop);
//
////    m_pFav->setTag(GameUILayoutLayer::eUIButtonTagFav);
//    m_pPhoto->setTag(GameUILayoutLayer::eUIButtonTagPhoto);
//    m_pEmail->setTag(GameUILayoutLayer::eUIButtonTagEmail);
    m_pHome->setTag(GameUILayoutLayer::eUIButtonTagHome);
    m_pBg->setTag(GameUILayoutLayer::eUIButtonTagBg);
    
//    showNextLayout(false);
//    showBackLayout(false);
}

void GameUILayoutLayer::_showHomeButtonAnimation(cocos2d::ui::Button *pButton,float duration,float delay)
{
    pButton->runAction(Sequence::create(DelayTime::create(delay),
                                        EaseBounceOut::create(FadeIn::create(duration)),
                                        CallFunc::create([=]()
                                                        {
                                                            pButton->setEnabled(true);
                                                        }),
                                        NULL));
}

void GameUILayoutLayer::_onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        Node* btn = (Node*)ref;
        if (onButtonClicked) {
            onButtonClicked((Button*)ref);
        }
    }
}

Button* GameUILayoutLayer::_createButton(std::string path)
{
    Button* pBtn = Button::create();
    pBtn->loadTextures(path, path, path);
    pBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayoutLayer::_onButtonCallback, this));
    return pBtn;
}