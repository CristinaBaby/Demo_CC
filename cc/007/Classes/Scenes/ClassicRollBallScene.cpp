
#include "ClassicRollBallScene.h"
#include "SceneManager.h"

ClassicRollBallScene::ClassicRollBallScene()
{
    m_bCanTouch = false;
}

ClassicRollBallScene::~ClassicRollBallScene()
{
    
}
bool ClassicRollBallScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showAll();
    
    for (int i = 0; i<4; i++) {
        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(200+220*(i%2), 280+180*(i/2)));
        m_CookiePosVector.push_back(pos);
    }
    m_pGameUI->showNormalLayout();
    return true;
}

void ClassicRollBallScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ClassicRollBallScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ClassicRollBallScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    if (pDragNode->index == 0){
        pDragNode->setDragSprite(Sprite::create(localPath("chocolate_chips/scooper0.png")));
        pDragNode->index = 1;
    }
}

void ClassicRollBallScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "scooper"){
        if (pDragNode->index == 1) {
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
#pragma mark =========  舀    =========
                pDragNode->setTouchEnabled(false);
                pDragNode->index = 2;
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(100, 50)),
                                                      RotateBy::create(0.3, -30),
                                                      RotateBy::create(0.2, 10),
                                                      CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("cookie_scoop_down.mp3");
                    pDragNode->setDragSprite(Sprite::create(localPath("chocolate_chips/scooper0_1.png")));
                    m_pMixture->setTexture(localPath("chocolate_chips/bowl_"+std::to_string(m_CookieVector.size()+2)+".png"));
                }),
                                                      RotateBy::create(0.3, 20),
                                                      CallFunc::create([=](){
                    if (m_CookieVector.size() == 0) {
                        m_pGuideLayer->showGuideMove(m_pScooper->getPosition(), m_pTray->getPosition());
                    }
                    pDragNode->setTouchEnabled(true);
                    if(m_CookieVector.size()==3){
                        m_pBowl->runAction(EaseElasticIn::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)), 1));
                    }
                }), NULL));
            }
        }else if (pDragNode->index == 2) {
#pragma mark =========   倒    =========
            for (int i = m_CookiePosVector.size()-1; i>=0; i--) {
                Vec2 pos = m_CookiePosVector.at(i);
                Rect cookie = Rect(pos.x-90, pos.y-90, 180, 180);
                if (cookie.containsPoint(worldPoint)) {
                    pDragNode->setTouchEnabled(false);
                    pDragNode->index = 1;
                    m_CookiePosVector.erase(m_CookiePosVector.begin()+i);
                    m_pGuideLayer->removeGuide();
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos+Vec2(100, 50)),
                                                          CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("chocolate_chips/scooper1.png"));
                        
                    }),
                                                          RotateBy::create(0.25,30),
                                                          CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("cookie_scoop_down.mp3");
                        
                    }),
                                                          RotateBy::create(0.25,-30),
                                                          RotateBy::create(0.25,30),CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("cookie_scoop_down.mp3");
                        
                    }),
                                                          RotateBy::create(0.25,-30),
                                                          CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("chocolate_chips/scooper1_1.png"));
                        
                        AudioHelp::getInstance()->playEffect("pour_cookie.mp3");
                        
                        auto pCookie = Sprite::create(localPath("chocolate_chips/chocolatechip_1.png"));
                        pCookie->setPosition(pos);
                        this->addChildToContentLayer(pCookie);
                        m_CookieVector.push_back(pCookie);
                        pCookie->setTag(0);
                    }),
                                                          DelayTime::create(1),
                                                          CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("chocolate_chips/scooper.png"));
                        pDragNode->back(0,[=](){
                            pDragNode->setTouchEnabled(true);
                        });
                        if (m_CookiePosVector.size()==0) {
                            _finishRoll();
                            pDragNode->setTouchEnabled(false);
                        }
                    }),
                                                          NULL));
                    
                    break;
                }
            }
        }
    }
}

void ClassicRollBallScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "scooper"){
        pDragNode->setTouchEnabled(false);
        pDragNode->back(0,[=](){
            pDragNode->setTouchEnabled(true);
        });
    }
}

void ClassicRollBallScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void ClassicRollBallScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void ClassicRollBallScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    
}
void ClassicRollBallScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    for(int k = 0;k<m_CookieVector.size();k++) {
        Sprite* pCookie = m_CookieVector.at(k);
        Rect rect = pCookie->getBoundingBox();
        if (rect.containsPoint(pos)) {
            m_bCanTouch = false;
            int tag = pCookie->getTag();
            AudioHelp::getInstance()->playEffect("press_cookie.mp3");
            if (tag == 0) {
                pCookie->runAction(Sequence::create(ScaleTo::create(0.3, 1.2),
                                                    CallFunc::create([=](){
                    m_bCanTouch = true;
                }), NULL));
                pCookie->setTag(tag+1);
            }else if (tag ==1) {
                m_pGuideLayer->removeGuide();
                pCookie->runAction(Sequence::create(ScaleTo::create(0.3, 1.4),
                                                    CallFunc::create([=](){
                    pCookie->setScale(1);
                    pCookie->setTexture(localPath("chocolate_chips/chocolatechip_2.png"));
                    m_bCanTouch = true;
                    AudioHelp::getInstance()->playEffect("present_tap.mp3");
                }), NULL));
                m_CookieVector.erase(m_CookieVector.begin()+k);
            }
            
            break;
        }
    }
    if (m_CookieVector.size() == 0) {
        _finishPress();
    }
}
#pragma mark - initData
void ClassicRollBallScene::_initData()
{
    setExPath("content/make/classic/make3/");
}

void ClassicRollBallScene::_showAll()
{
    m_pBowl = Sprite::create(localPath("chocolate_chips/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 166-visibleSize.width, 750);
    m_pBowl->runAction(Spawn::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                     EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
    
    m_pMixture = Sprite::create(localPath("chocolate_chips/bowl_1.png"));
    m_pBowl->addChild(m_pMixture);
    m_pMixture->setPosition(m_pBowl->getContentSize()*0.5);
    
    m_pTray = Sprite::create(localPath("plate_small.png"));
    this->addChildToContentLayer(m_pTray);
    CMVisibleRect::setPositionAdapted(m_pTray, 320-visibleSize.width, 365);
    m_pTray->runAction(EaseElasticOut::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),1.0));
    
    m_pScooper = _createDrageNode(localPath("chocolate_chips/scooper.png"));
    this->addChildToContentLayer(m_pScooper);
    CMVisibleRect::setPositionAdapted(m_pScooper, 505+visibleSize.width, 730);
    m_pScooper->setName("scooper");
    m_pScooper->setTouchEnabled(false);
    m_pScooper->runAction(Sequence::create(MoveBy::create(1.0,Vec2(-visibleSize.width,0)),
                                           CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pScooper->getPosition(), m_pBowl->getPosition());
        m_pScooper->setOrgPositionDefault();
        m_pScooper->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_scoop2baking_pan.mp3");
    }), NULL));
}

void ClassicRollBallScene::_finishRoll()
{
    m_pScooper->runAction(EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), 1.0f));
    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 100)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideTap(m_CookieVector.at(0)->getPosition());
        m_bCanTouch = true;
    }), NULL));
    
    for(auto ref:m_CookieVector){
        ref->runAction(MoveBy::create(0.5, Vec2(0, 100)));
    }
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(ClassicRollBallScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(ClassicRollBallScene::TouchMoved, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(ClassicRollBallScene::TouchEnded, this);
    AudioHelp::getInstance()->playEffect("vo_tap_press_cookies.mp3");
    
}

void ClassicRollBallScene::_finishPress()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    static std::string voName[] = {
        "vo_wonderful.mp3",
        "vo_well_done.mp3",
        "vo_fantastic.mp3",
        "vo_done.mp3",
        "vo_fantastic.mp3",
        "vo_great.mp3",
    };
    AudioHelp::getInstance()->playEffect(voName[arc4random()%6]);
    
    Sprite* pAnimation = Sprite::create("content/make/common/animation/donhua_1.png");
    this->addChildToUILayer(pAnimation);
    pAnimation->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pAnimation, 320, 0,kBorderNone,kBorderBottom);
    pAnimation->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/common/animation/donhua_", 1, 8),
                                           CallFunc::create([=](){
        pAnimation->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->playEffect("success.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ClassicBakeScene>();
    }), NULL));
    
}