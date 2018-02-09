
#include "FunnelDecorateScene.h"
#include "SceneManager.h"

FunnelDecorateScene::FunnelDecorateScene()
{
    m_nMoveCount = 0;
    m_pClipping = nullptr;
}

FunnelDecorateScene::~FunnelDecorateScene()
{
    
}
bool FunnelDecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_dec.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _initDefaultDecorate();
    _showSugar();
    
    m_pGameUI->showNextLayout();
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("background_make.mp3");
    return true;
}

void FunnelDecorateScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FunnelDecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelDecorateScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if(tag == GameUILayoutLayer::eUIButtonTagNext){
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
        _saveSugar();
        SceneManager::pushTheScene<FunnelEatScene>();
    }else if(tag == GameUILayoutLayer::eUIButtonTagReset){
        AudioHelp::getInstance()->playResetEffect();
        m_pClipping->removeAllChildren();
        
    }
}
void FunnelDecorateScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode == m_pSugarBottle && pDragNode->index == 0) {
        m_pSugarBottle->runAction(RotateBy::create(0.5, -120));
        pDragNode->index = 1;
        pDragNode->getDragSprite()->setTexture(localPath("jar2.png"));
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sprinke.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setPosition(m_pSugarBottle->getWorldSpaceActionPoint());
        pParticle->setName("particle");
    }
}

void FunnelDecorateScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    Node* pParticle = m_pUILayer->getChildByName("particle");
    if (pParticle) {
        pParticle->setPosition(m_pSugarBottle->getWorldSpaceActionPoint());
    }
    Rect rect = m_pCake->getBoundingBox();
    rect.origin = rect.origin+Vec2(0, 200);
    //if (rect.containsPoint(worldPoint)) {
    m_nMoveCount++;
        if (m_nMoveCount%5 == 0) {
            for(int i = 0 ;i<6  ;i++){
                Vec2 origin = Vec2(-m_pCake->getContentSize().width/2 , m_pCake->getContentSize().height/2);
                
                Sprite* pSugar = Sprite::create(localPath("sugar_powder.png"));
                m_pClipping->addChild(pSugar);
                Vec2 sugarPos = m_pClipping->convertToNodeSpace(worldPoint);
                int radom = arc4random()%30*120;
                pSugar->setPosition(sugarPos+Vec2(0, -200)+Vec2(30*cos(radom), 30*sin(radom)));
                
            }
      //  }
        
//        static int sugarCount = 0;
//        sugarCount++;
//        for (int col = 0; col<7; col++,col+=sugarCount%2) {
//            for (int row = 0; row<11; row++) {
//                static int sugarIndex = 0;
//                
//                sugarIndex++;
//                sugarIndex = sugarIndex%5;
//                Sprite* pSugar = Sprite::create(localPath("sugar_powder.png"));
//                if (pSugar) {
//                    int rotation = arc4random()%360;
//                    int deltX = (arc4random()%5)*4;
//                    int deltY = (arc4random()%5)*8;
//                    float scale = 1.0f-0.05f*((arc4random()%4)*2);
//                    pSugar->setRotation(rotation);
//                    pSugar->setScale(scale);
//                    Vec2 sugarPos = origin+Vec2(col*20, -row*40)+Vec2(deltX, deltY);
//                    pSugar->setPosition(sugarPos);
//                    m_pClipping->addChild(pSugar);
//                    int radom = arc4random()%5;
//                    pSugar->setOpacity(0);
//                    pSugar->runAction(Sequence::create(DelayTime::create(0.1+0.1*radom),
//                                                       FadeIn::create(0.2), NULL));
//                }else{
//                    continue;
//                }
//            }
//        }
    }
}

void FunnelDecorateScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    Node* pParticle = m_pUILayer->getChildByName("particle");
    if (pParticle) {
        pParticle->removeFromParent();
    }
    pDragNode->stopAllActions();
    pDragNode->setTouchEnabled(false);
    pDragNode->back(0,[=]{
        pDragNode->index = 0;
        pDragNode->getDragSprite()->setTexture(localPath("jar1.png"));
        pDragNode->setTouchEnabled(true);
    });
    pDragNode->runAction(RotateTo::create(0.5, 0));
}

void FunnelDecorateScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FunnelDecorateScene::_initData()
{
    setExPath("content/make/funnel/dec/");
}

void FunnelDecorateScene::_initDefaultDecorate()
{
    //    ******************      Food        **********************
    Vec2 pos = Vec2(320, 400);
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, pos-Vec2(0,10));
    
    int funnelIndex = GameDataManager::getInstance()->m_nFunnelIndex;
    m_pCake = Sprite::create(localPath("cake"+std::to_string(funnelIndex)+".png"));
    this->addChildToContentLayer(m_pCake);
    CMVisibleRect::setPositionAdapted(m_pCake, pos);
    
    
    if (!m_pClipping) {
        Sprite* pStencil = Sprite::create(localPath("mask.png"));
        
        m_pClipping = ClippingNode::create(pStencil);
    }
    m_pClipping->setAlphaThreshold(0.5);
    this->addChildToContentLayer(m_pClipping);
    CMVisibleRect::setPositionAdapted(m_pClipping, pos);
    
    
}

void FunnelDecorateScene::_showSugar()
{
    m_pSugarBottle = _createDrageNode(localPath("jar1.png"));
    this->addChildToContentLayer(m_pSugarBottle);
    m_pSugarBottle->setActionPoint(Vec2(111, 241));
    
    ContainerNode* pSugar = ContainerNode::create(m_pSugarBottle);
    pSugar->addShadow(localPath("jar1_shadow.png"));
    m_pSugarBottle->setUserData(pSugar);
    
    CMVisibleRect::setPositionAdapted(m_pSugarBottle, 320+visibleSize.width, 780);
    m_pSugarBottle->setTouchEnabled(false);
    m_pSugarBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_funnel_decorate_sugar.mp3");
        m_pSugarBottle->setOrgPositionDefault();
        m_pSugarBottle->setTouchEnabled(true);
    }), NULL));
}

void FunnelDecorateScene::_saveSugar()
{
    Size mixtureSize = m_pPlate->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度

    Vec2 pos = m_pClipping->getPosition();
    m_pClipping->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pClipping->Node::visit();
    render->end();
    m_pClipping->setPosition(pos);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"funnel_dec.png", false);
    pImage->autorelease();
}