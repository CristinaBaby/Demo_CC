
#include "CaramelMakeBallScene.h"
#include "SceneManager.h"

CaramelMakeBallScene::CaramelMakeBallScene()
{
    m_bCanTouch = false;
    m_nStep = 0;
}

CaramelMakeBallScene::~CaramelMakeBallScene()
{
    
}
bool CaramelMakeBallScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showAll();
    
    for (int i = 0; i<6; i++) {
        Vec2 pos = Vec2(130+210*(i%3), 140+180*(i/3));
        m_CookiePosVector.push_back(pos);
    }
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(CaramelMakeBallScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(CaramelMakeBallScene::TouchMoved, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(CaramelMakeBallScene::TouchEnded, this);
    m_pGameUI->showNormalLayout();
    return true;
}

void CaramelMakeBallScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CaramelMakeBallScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CaramelMakeBallScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if (name == "ball"){
        if (pDragNode->index == 0) {
            pDragNode->setVisible(true);
            pDragNode->index = 1;
            m_pMixture->setTexture(localPath("bowl_"+std::to_string(m_CookieVector.size()+2)+".png"));
        }
    }else if (name == "sugar"){
        pDragNode->setVisible(true);
    }
}

void CaramelMakeBallScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (name == "ball"){
        if (pDragNode->index == 1){
            Rect rect = Rect(350, 250, 200, 200);
            rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                pDragNode->setOrgPositionDefault();
                pDragNode->setPosition(Vec2(rect.getMidX(), rect.getMidY()));
                m_nMoveCount = 0;
                m_bCanTouch = true;
                m_pGuideLayer->showGuideMove(pDragNode->getPosition()+Vec2(0, -30), pDragNode->getPosition()+Vec2(0, 30));
            }
        }else if (pDragNode->index == 2){
            for (int i = m_CookiePosVector.size()-1; i>=0; i--) {
                Vec2 pos = m_CookiePosVector.at(i);
                pos = m_pTray->convertToWorldSpace(pos);
                Rect cookie = Rect(pos.x-100, pos.y-100, 200, 200);
                if (cookie.containsPoint(worldPoint)) {
                    pDragNode->setVisible(false);
                    m_pGuideLayer->removeGuide();
                    
                    auto pCookie = Sprite::create(localPath("cookie1.png"));
                    pCookie->setPosition(pos);
                    this->addChildToContentLayer(pCookie);
                    
                    pCookie->setTag(0);
                    m_CookieVector.push_back(pCookie);
                    
                    if(m_CookieVector.size() < 6){
                        pDragNode->getDragSprite()->setTexture(localPath("1/cookie1.png"));
                        pDragNode->setPosition(m_pBowl->getPosition());
                        pDragNode->setOrgPositionDefault();
                        pDragNode->index = 3;
                        pDragNode->setTouchEnabled(true);
                    }else{
                        _finishRoll();
                    }
                    m_CookiePosVector.erase(m_CookiePosVector.begin()+i);
                    break;
                }
            }
        }
    }
}

void CaramelMakeBallScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "ball"){
        if(pDragNode->index ==3){
            pDragNode->index = 0;
            pDragNode->setPosition(m_pBowl->getPosition());
            pDragNode->setOrgPositionDefault();
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(0,[=](){
                pDragNode->setTouchEnabled(true);
            });
        }
    }else if (name == "sugar"){
        bool bBack = true;
        for (int i = m_CookieVector.size()-1; i>=0; i--) {
            Sprite* pCookie = m_CookieVector.at(i);
            Vec2 pos = pCookie->getPosition();
            Rect cookieRect = Rect(pos.x-100, pos.y-100, 200, 200);
            if (cookieRect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(pos);
                m_pCurCookie = pCookie;
                
                if (m_CookieVector.size() == 6) {
                    m_pGuideLayer->removeGuide();
                    m_pGuideLayer->showGuideTap(pCookie->getPosition());
                    AudioHelp::getInstance()->playEffect("vo_tap_press_cookies.mp3");
                }
                
                m_nStep = 1;
                m_bCanTouch = true;
                m_pBall->setOrgPositionDefault();
                m_CookieVector.erase(m_CookieVector.begin()+i);
                bBack = false;
                break;
            }
        }
        if (bBack) {
            pDragNode->back(2);
        }
    }
}

void CaramelMakeBallScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void CaramelMakeBallScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if(m_nStep == 0){
        Rect rect = m_pBall->getDragSprite()->getBoundingBox();
        rect.origin = m_pBall->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(pTouch->getLocation())) {
            auto action = ActionHelper::createRollJelly();
            action->setTag(100);
            m_pBall->runAction(action);
        }
    }
    
}
void CaramelMakeBallScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if(m_nStep == 0){
        Rect rect = m_pBall->getDragSprite()->getBoundingBox();
        rect.origin = m_pBall->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(pTouch->getLocation())) {
            m_nMoveCount++;
            static int total = 60;
            auto action = m_pBall->getActionByTag(100);
            if (!action) {
                auto action = ActionHelper::createRollJelly();
                action->setTag(100);
                m_pBall->runAction(action);
            }
            if (m_nMoveCount%10==0){
                m_pFlour->setOpacity(m_pFlour->getOpacity()-1);
            }
            
            if (m_nMoveCount%total==0) {
                int step = m_nMoveCount/total;
                m_pBall->getDragSprite()->setTexture(localPath("1/cookie"+std::to_string(step+1)+".png"));
                AudioHelp::getInstance()->playEffect("press_cookie.mp3");
                if (step == 1) {
                    m_pGuideLayer->removeGuide();
                }else if(step == 3){
                    m_bCanTouch = false;
                    m_pBall->index = 2;
                    m_pGuideLayer->showGuideMove(m_pBall->getPosition(), m_pTray->getPosition());
                    m_pBall->stopAllActions();
                    m_pBall->setScale(1);
                    m_pBall->setTouchEnabled(true);
                }
            }
        }
    }
    
}
void CaramelMakeBallScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if(m_nStep == 0){
        m_pBall->stopAllActions();
        m_pBall->setScale(1);
    }else if(m_nStep == 1){
        Rect rect = m_pCurCookie->getBoundingBox();
        Vec2 pos = pTouch->getLocation();
        if (rect.containsPoint(pos)) {
            m_pCurCookie->setTexture(localPath("cookie2.png"));
            
            AudioHelp::getInstance()->playEffect("press_cookie.mp3");
            m_pGuideLayer->removeGuide();
            if(m_CookieVector.size()==6){
                m_pGuideLayer->showGuideMove(m_pSugar->getPosition(), m_CookieVector.at(0)->getPosition());
            }
            m_pSugar->setVisible(false);
            m_pSugar->setPosition(m_pPlate->getPosition());
            m_bCanTouch = false;
            if (m_CookieVector.size() == 0) {
                _finishPress();
            }else{
                m_pSugar->setTouchEnabled(true);
            }
        }
    }
}
#pragma mark - initData
void CaramelMakeBallScene::_initData()
{
    setExPath("content/make/caramel/make3/");
}

void CaramelMakeBallScene::_showAll()
{
    m_pFlour = Sprite::create(localPath("flour.png"));
    this->addChildToContentLayer(m_pFlour);
    CMVisibleRect::setPositionAdapted(m_pFlour, 420, 300);
    
    m_pTray = Sprite::create(localPath("plant2.png"));
    this->addChildToContentLayer(m_pTray);
    CMVisibleRect::setPositionAdapted(m_pTray, 320+visibleSize.width, 785);
    m_pTray->runAction(EaseElasticOut::create(MoveBy::create(1.0,Vec2(-visibleSize.width,0)),1.0));
    m_pTray->setScale(0.8);
    
    m_pBowl = Sprite::create(localPath("bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 142-visibleSize.width, 480);
    m_pBowl->runAction(Spawn::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                     EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
    
    m_pMixture = Sprite::create(localPath("bowl_1.png"));
    m_pBowl->addChild(m_pMixture);
    m_pMixture->setPosition(m_pBowl->getContentSize()*0.5);
    
    m_pBall = _createDrageNode(localPath("1/cookie1.png"));
    this->addChildToContentLayer(m_pBall);
    m_pBall->setContentSize(m_pBowl->getContentSize());
    m_pBall->setPosition(m_pBowl->getPosition());
    m_pBall->setName("ball");
    m_pBall->setTouchEnabled(false);
    m_pBall->setVisible(false);
    m_pBall->setContentSize(m_pBowl->getContentSize());
    m_pBall->runAction(Sequence::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), CMVisibleRect::getPosition(450, 350));
        m_pBall->setOrgPositionDefault();
        m_pBall->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_caramel_cookie_twist.mp3");
    }), NULL));
}

void CaramelMakeBallScene::_finishRoll()
{
    m_pBowl->runAction(EaseElasticIn::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)), 1.0f));
    m_pTray->runAction(Sequence::create(DelayTime::create(0.5),
                                        Spawn::create(MoveBy::create(0.8, Vec2(0, -400)),
                                                      ScaleTo::create(0.8, 1), NULL),
                                        CallFunc::create([=](){
        m_bCanTouch = true;
    }), NULL));
    
    for(auto ref:m_CookieVector){
        ref->runAction(Sequence::create(DelayTime::create(0.5),
                                        Spawn::create(MoveBy::create(0.8, Vec2(0, -400)),
                                     ScaleTo::create(0.8, 1), NULL),
                                        NULL));
    }
    m_pPlate = Sprite::create(localPath("sugar_plant.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 730);
    m_pPlate->runAction(Spawn::create(MoveBy::create(1.0,Vec2(-visibleSize.width,0)),
                                     EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
    
    for (int i = 0; i<10; i++) {
        Sprite* pSugar = Sprite::create(localPath("sugar"+std::to_string(i%3+1)+".png"));
        m_pPlate->addChild(pSugar);
        pSugar->setPosition(Vec2(m_pPlate->getContentSize().width*0.5-50.0+10.0*(arc4random()%10), m_pPlate->getContentSize().height*0.5-50.0+10.0*(arc4random()%10)));
    }
    
    m_pSugar = _createDrageNode(localPath("sugar3.png"));
    this->addChildToContentLayer(m_pSugar);
    CMVisibleRect::setPositionAdapted(m_pSugar, 320, 730);
    m_pSugar->setName("sugar");
    m_pSugar->setVisible(false);
    m_pSugar->setTouchEnabled(false);
    m_pSugar->setOrgPositionDefault();
    m_pSugar->setContentSize(m_pPlate->getContentSize()*1.2);
    m_pSugar->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pSugar->getPosition(), m_CookieVector.at(0)->getPosition());
        m_pSugar->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_caramel_put_caramel.mp3");
    }), NULL));
    
    m_pFlour->runAction(FadeOut::create(0.5));
}

void CaramelMakeBallScene::_finishPress()
{
    m_pPlate->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), 1.0f),
                                         CallFunc::create([=](){
        
    }), NULL));
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