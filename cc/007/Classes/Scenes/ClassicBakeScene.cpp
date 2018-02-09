
#include "ClassicBakeScene.h"
#include "SceneManager.h"

static std::string gsFlavors[] = {
    "vanilla",
    "chocolate",
    "mint",
    "strawberry",
};
static std::string gsModels[] = {
    "round",
    "heart",
    "cat",
    "star",
    "ginger",
    "tree",
};
ClassicBakeScene::ClassicBakeScene()
{
    m_pBar = nullptr;
}

ClassicBakeScene::~ClassicBakeScene()
{
    
}
bool ClassicBakeScene::init()
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
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 320, 400);
    
    m_pOven->onOvenStateCallback = [=](OvenNode::State state){
        
        if (state == OvenNode::State::eStateTurnOn) {
            m_pGuideLayer->removeGuide();
            _showProgressBar();
        }else if (state == OvenNode::State::eStateOK) {
            m_pGuideLayer->showGuideTap(m_pOven->getSwitchWorldPosition());
            m_pOven->setSwitchEnable();
            if(m_nFlavor == 5){
                m_pTray->getDragSprite()->setTexture(localPath("plate_cookie1.png"));
            }else{
                for (auto pCookie:m_CookieVector) {
                    std::string path;
                    if (m_nFlavor == 4) {
                        path = "content/make/classic/dec/chocolate_chip.png";
                    }else{
                        path = "content/make/classic/dec/"+gsModels[m_nShape]+"_"+std::to_string(m_nFlavor+1)+".png";
                    }
                    pCookie->setTexture(path);
                    pCookie->setScaleX(pCookie->getScaleX()*0.5);
                    pCookie->setScaleY(pCookie->getScaleY()*0.5);
                }
            }
        }else if(state == OvenNode::State::eStateTurnOff){
            m_pGuideLayer->removeGuide();
            _showBar(false);
            m_pOven->openDoor();
            m_pTray->setVisible(true);
            m_pTray->retain();
            m_pTray->removeFromParent();
            this->addChildToContentLayer(m_pTray);
            m_pTray->release();
            m_pTray->setPosition(m_pOven->getPosition()+Vec2(0, 0));
            AudioHelp::getInstance()->playEffect("slide.mp3");
            m_pTray->runAction(Sequence::create(Spawn::create(RotateBy::create(0.6, 360),
                                                              ScaleTo::create(0.6, 1),
                                                              MoveTo::create(0.6, Vec2(visibleSize.width/2, visibleSize.height/2)), NULL),
                                                JumpBy::create(0.5, Vec2::ZERO, 100, 2),
                                                CallFunc::create([=](){
                _finish();
            }), NULL));
        }
    };
    
    _showCookie();
    m_pGameUI->showNormalLayout();
    return true;
}

void ClassicBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ClassicBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ClassicBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void ClassicBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void ClassicBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if (name == "tray") {
        Rect rect = m_pOven->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->retain();
            m_pGuideLayer->removeGuide();
            pDragNode->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.6),
                                                                MoveTo::create(0.5, m_pOven->getPosition()+Vec2(0, 0)), NULL),
                                                  CallFunc::create([=](){
                m_pOven->closeDoor();
                m_pTray->removeFromParent();
                m_pOven->putinFood(pDragNode);
                pDragNode->setPosition(Vec2(0, 0));
                pDragNode->release();
                m_pOven->setSwitchEnable();
                m_pGuideLayer->showGuideTap(m_pOven->getSwitchWorldPosition());
            }), NULL));
        }
    }
}

void ClassicBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ClassicBakeScene::_initData()
{
    m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
    m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
    if(m_nFlavor == 5){
        setExPath("content/make/caramel/make4/");
    }else {
        setExPath("content/make/classic/make4/");
    }
}

void ClassicBakeScene::_showProgressBar()
{
    if (m_pBar) {
        m_pBar->beginProgress(5,false);
        _showBar();
        return;
    }
    m_pBar = ProgressBar::create("content/common/progress/bar_h.png", "content/common/progress/btn1.png", "content/common/progress/btn2.png", "content/common/progress/btn3.png","content/common/progress/bar_mash_h.png",ProgressBar::Direction::eDirectionH);
    this->addChildToUILayer(m_pBar);
    CMVisibleRect::setPositionAdapted(m_pBar, 320, 550,kBorderNone,kBorderTop);
    m_pBar->progressNewState = [=](int state){
        if (state == ProgressBar::eStateOK) {
            
        }else if (state == ProgressBar::eStateBad) {
            log("=== bad");
            
            
        }else if (state == ProgressBar::eStateOver) {
            log("=== Over");
            m_pOven->bakeOK();
        }
    };
    m_pBar->stopProgress();
    _showBar();
    m_pBar->setPercentStage(180.0*100/534, 360.0*100/534, 100);
    m_pBar->beginProgress(10,true);
}
void ClassicBakeScene::_showBar(bool show )
{
    if (!m_pBar) {
        return;
    }
    if (show) {
        m_pBar->stopAllActions();
        m_pBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height-150+500));
        m_pBar->setVisible(true);
        m_pBar->runAction(EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -500))));
    }else{
        m_pBar->stopAllActions();
        m_pBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height-150));
        m_pBar->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, 500))),
                                           CallFunc::create([=]()
                                                            {
                                                                m_pBar->setVisible(false);
                                                            }), NULL));
    }
}


void ClassicBakeScene::_showCookie()
{
    if(m_nFlavor == 5){
        m_pTray = _createDrageNode(localPath("plate_cookie.png"));
    }else{
        m_pTray = _createDrageNode(localPath("plate.png"));
    }
    this->addChildToContentLayer(m_pTray);
    CMVisibleRect::setPositionAdapted(m_pTray, 320-visibleSize.width, 230);
    m_pTray->setName("tray");
    m_pTray->setTouchEnabled(false);
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pTray->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),1.0),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pTray->getPosition(), m_pOven->getPosition());
        m_pTray->setOrgPositionDefault();
        m_pTray->setTouchEnabled(true);
        if(m_nFlavor == 5){
            AudioHelp::getInstance()->playEffect("vo_caramel_bake.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_bake_cookies.mp3");
        }
        
    }), NULL));
    
    int count = 4;
    if(m_nFlavor == 5){
        return;
    }
    for(int i = 0;i<count;i++){
        std::string path;
        
        if(m_nFlavor == 5){
            path = "make3/cookie2.png";
        }else if (m_nFlavor == 4) {
            path = "make3/chocolate_chips/chocolatechip_2.png";
        }else{
            path = "make3/"+gsModels[m_nShape]+"/"+std::to_string(m_nFlavor+1)+"_1.png";
        }
        Sprite* pCookie = Sprite::create(localRootPath(path));
        m_pTray->addChild(pCookie);
        
        pCookie->setPosition(Vec2(-100+20*(i/2)*(i%2==0?1:-1)+200*(i%2), -30+110*(i/2)));
        if (i/2==0) {
            pCookie->setScale(0.8,0.65);
        }else{
            pCookie->setScale(0.78,0.52);
        }
        
        m_CookieVector.pushBack(pCookie);
    }
    
}

void ClassicBakeScene::_finish()
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
        if(m_nFlavor == 5){
            SceneManager::replaceTheScene<CaramelDisplayScene>();
        }else{
            SceneManager::replaceTheScene<DecorateScene>();
        }
    }), NULL));
    
}