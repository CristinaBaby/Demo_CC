
#include "PizzaBakeScene.h"
#include "SceneManager.h"

PizzaBakeScene::PizzaBakeScene()
{
    
}

PizzaBakeScene::~PizzaBakeScene()
{
    
}
bool PizzaBakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("pizza_bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
//    Sprite* pTable = Sprite::create(localPath("pizza_bg2.jpg"));
//    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
//    this->addChildToBGLayer(pTable);
    
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    _showPan();
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 320, 450);
    m_pOven->setTouchEnable(true);
    m_pOven->setOvenSettings(250, 8);
    m_pGuideLayer->showGuide("content/common/tip/arrow2.png", m_pOven->getPosition());
    
    m_pOven->onDoorCallback = [=](OvenNode* pOven){
        if (pOven->getIsOpen() && !pOven->getBaked()) {
            m_pPan->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(m_pPan->getPosition(), m_pOven->getPosition());
        }
    };
    m_pOven->onBakeCallback = [=](OvenNode* ,int state){
        if (state==0) {
            _doBake();
        }else{
            m_pOven->openDoor();
            _showGlove();
        }
    };
    m_pGameUI->showNormalLayout();
    return true;
}

void PizzaBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PizzaBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


void PizzaBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void PizzaBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (name == "pan") {
        
    }else if (name == "glove"){
        if (pDragNode->index==0) {
            Rect rect = m_pPan->getDragSprite()->getBoundingBox();
            rect.origin = m_pPan->convertToWorldSpace(rect.origin);
            
            Rect rectGlove = pDragNode->getDragSprite()->getBoundingBox();
            rectGlove.origin = pDragNode->convertToWorldSpace(rectGlove.origin);
            if (rectGlove.intersectsRect(rect)) {
                pDragNode->index = 1;
                m_pPan->retain();
                m_pPan->removeFromParent();
                m_pGlove->addChild(m_pPan);
                m_pPan->release();
                m_pPan->setPosition(Vec2(-130, 50));
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(320, 860)));
                
                Sprite* pGloveFront = Sprite::create("content/make/common/bake/glove1.png");
                m_pGlove->addChild(pGloveFront,10);
                pGloveFront->setName("front");
                //                m_pGuideLayer->removeGuide();
            }
        }
    }
}

void PizzaBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    m_pGuideLayer->removeGuide();
    if (name == "pan") {
        Rect rect = m_pOven->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setLocalZOrder(1);
            pDragNode->setScale(0.8);
            
            m_pPan->setPosition(Vec2(0,-50));
            m_pPan->retain();
            m_pPan->removeFromParent();
            m_pOven->addChild(m_pPan);
            m_pPan->release();
            m_pOven->closeDoor();
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                m_pOven->showControlFrame();
            }),
                                             DelayTime::create(1),
                                             CallFunc::create([=](){
                //                m_pGuideLayer->showguidem();
            }), NULL));
        }else{
            pDragNode->back();
        }
    }else if (name == "glove"){
        if (pDragNode->index==1){
            Vec2 pos = pDragNode->getPosition();
            Vec2 posPan = CMVisibleRect::getPositionAdapted(Vec2(320, 860));
            if (pos.y>posPan.y-200) {
                
                m_pPan->retain();
                m_pPan->removeFromParent();
                this->addChildToContentLayer(m_pPan);
                m_pPan->release();
                m_pPan->setPosition(posPan);
                //                m_pPan->setScale(1);
                Sprite* pGloveFront = (Sprite*)m_pGlove->getChildByName("front");
                if (pGloveFront) {
                    pGloveFront->removeFromParent();
                }
                
                m_pGuideLayer->removeGuide();
                m_pGlove->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                    _finishAll();
                }), NULL));
            }
        }
    }
}

void PizzaBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void PizzaBakeScene::_initData()
{
    setExPath("content/make/pizza/2/");
}

void PizzaBakeScene::_showPan()
{
    m_pPan = _createDrageNode(localPath("pan1.png"));
    m_pPan->setName("pan");
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 870);
    
    Sprite* pFront = Sprite::create(localPath("pan1_h.png"));
    m_pPan->addChild(pFront,5);
    
    m_pFood = Node::create();
    m_pPan->addChild(m_pFood);
//    m_pFood->setPosition(Vec2(-6, 16));
    
    Sprite* pPizza = Sprite::create(localRootPath("1/dough4.png"));
    if(pPizza){
        m_pFood->addChild(pPizza);
        pPizza->setName("pizza");
        pPizza->setScale(0.6, 0.4);
    }
    
    
    Sprite* pSauce = Sprite::create(localRootPath("1/sauce1.png"));
    if(pSauce){
        m_pFood->addChild(pSauce);
        pSauce->setName("sauce");
        pSauce->setScale(0.6, 0.4);
    }
    
    Sprite* pCheese = Sprite::create(FileUtils::getInstance()->getWritablePath()+"normal_cheese.png");
    if(pCheese){
        m_pFood->addChild(pCheese);
        pCheese->setName("pizza_cheese");
        pCheese->setScale(0.6, 0.4);
    }
    
    
    Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"normal_dec.png");
    if(pDec){
        m_pFood->addChild(pDec);
        pDec->setName("pizza_dec");
        pDec->setScale(0.6, 0.4);
    }
    
    m_pPan->setTouchEnabled(false);
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pPan->setOrgPositionDefault();
//        m_pPan->setTouchEnabled(true);
    }), NULL));
}

void PizzaBakeScene::_doBake()
{
    Sprite* pPizza = (Sprite*) m_pFood->getChildByName("pizza");
    if (pPizza) {
        pPizza->runAction(Sequence::create(DelayTime::create(4),
                                           CallFunc::create([=](){
            pPizza->setTexture(localPath("bake_pizza1.png"));
            pPizza->setScale(1);
            
            Sprite* pSauce = (Sprite*) m_pFood->getChildByName("sauce");
            if (pSauce) {
                pSauce->setVisible(false);
            }
        }), NULL));
    }
    Sprite* pCheese = (Sprite*) m_pFood->getChildByName("pizza_cheese");
    if (pCheese) {
        pCheese->runAction(Sequence::create(DelayTime::create(3),
                                         FadeOut::create(3), NULL));
    }
    Sprite* pDec = (Sprite*) m_pFood->getChildByName("pizza_dec");
    if (pDec) {
        pDec->runAction(Sequence::create(DelayTime::create(3),
                                         FadeOut::create(3), NULL));
    }
    Sprite* pDecRipe = Sprite::create(FileUtils::getInstance()->getWritablePath()+"normal_decripe.png");
    if(pDecRipe){
        m_pFood->addChild(pDecRipe);
        pDecRipe->setName("pizza_decripe");
        pDecRipe->setOpacity(0);
        pDecRipe->setScale(0.6, 0.4);
        pDecRipe->runAction(Sequence::create(DelayTime::create(1.5),
                                             FadeIn::create(2), NULL));
    }
}

void PizzaBakeScene::_showGlove()
{
    m_pGlove = _createDrageNode("content/make/common/bake/glove0.png");
    m_pGlove->setName("glove");
    m_pGlove->index = 0;
    this->addChildToContentLayer(m_pGlove);
    CMVisibleRect::setPositionAdapted(m_pGlove, 480+visibleSize.width, 300);
    m_pGlove->setTouchEnabled(false);
    m_pGlove->runAction(Sequence::create(Sequence::create(DelayTime::create(0.5), NULL),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pGlove->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pGlove->getPosition(), m_pOven->getPosition());
    }), NULL));
}
void PizzaBakeScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<PizzaCutScene>();
    }), NULL));
}