
#include "BakeNormalPizzaScene.h"
BakeNormalPizzaScene::BakeNormalPizzaScene()
{
    m_bSholveReady =false;
    m_bBaked = false;
    m_bPlateReady = false;
    m_nBakeCount = 0;
}

BakeNormalPizzaScene::~BakeNormalPizzaScene()
{
    
}
bool BakeNormalPizzaScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("oven_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pOven = Sprite::create(localPath("oven.png"));
    m_pOven->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pOven);
    
    m_pSholve = _createDrageNode(localPath("make/pizza_shovel0.png"));
    this->addChildToContentLayer(m_pSholve);
    CMVisibleRect::setPositionAdapted(m_pSholve, -300, 0);
    m_pSholve->setTouchEnabled(false);
    m_pSholve->setActionPoint(Vec2(265, 636));
    
    Vec2 pos = m_pSholve->convertToNodeSpace(m_pSholve->getWorldSpaceActionPoint());
    
    m_pPizzaNode = Node::create();
    m_pSholve->getDragSprite()->addChild(m_pPizzaNode);
    m_pPizzaNode->setPosition(Vec2(265, 636));
    
    m_pPie = Sprite::create(localPath("make/finish_pie.png"));
    m_pPizzaNode->addChild(m_pPie);
    m_pieSize = m_pPie->getContentSize()*1.1;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pCheese = Sprite::create(FileUtility::getStoragePath()+"normal_cheese.png");
#else
    m_pCheese = Sprite::create(FileUtility::getStoragePath()+"/normal_cheese.png");
#endif
    m_pPizzaNode->addChild(m_pCheese);
//    m_pCheese->setPosition(pos);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoration = Sprite::create(FileUtility::getStoragePath()+"normal_dec.png");
#else
    m_pDecoration = Sprite::create(FileUtility::getStoragePath()+"/normal_dec.png");
#endif
    m_pPizzaNode->addChild(m_pDecoration);
//    m_pDecoration->setPosition(pos);
    m_pSholve->runAction( MoveBy::create(1, Vec2(600, 100)));
    m_pSholve->getDragSprite()->runAction(Sequence::create(
                                          Spawn::create(RotateBy::create(1, 87),
//                                                        MoveBy::create(1, Vec2(600, 100)),
                                                        ScaleTo::create(1, 0.33,0.66), NULL),
//                                          Spawn::create(RotateBy::create(3, Vec3(-50,-70,50)),
//                                                        MoveBy::create(3, Vec2(600, 100)),
//                                                        ScaleTo::create(3, 0.7), NULL),
                                          CallFunc::create([=](){
        m_pSholve->setOrgPositionDefault();
        m_pSholve->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pSholve->getPosition(), visibleSize*0.5);
        AudioHelp::getInstance()->playEffect("vo_drag2oven.mp3");
    }), NULL));
    
    m_pDoorClipping = ClippingNode::create(Sprite::create(localPath("oven_mask.png")));
    m_pOven->addChild(m_pDoorClipping);
    m_pDoorClipping->setPosition(Vec2(470, 370));
    m_pDoorClipping->setAlphaThreshold(0.5);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fire.plist");
    m_pDoorClipping->addChild(pParticle);
    pParticle->setPosition(Vec2(0,30));
    
    m_pPizzaRipeNode = Node::create();
    m_pSholve->getDragSprite()->addChild(m_pPizzaRipeNode);
//    m_pPizzaRipeNode->setPosition(Vec2(470, 350));
    m_pPizzaRipeNode->setPosition(Vec2(265, 636));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    m_pGameUI->showNormalLayout();
    return true;
}

void BakeNormalPizzaScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}
void BakeNormalPizzaScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pSholve) {
        if (m_bPlateReady){
            m_pSholve->getDragSprite()->setScale(1);
            m_pPizzaRipeNode->setRotation(-120);
            m_pPizzaRipeNode->setScale(1.09,1.36);
        }else if (!m_bSholveReady) {
            m_bSholveReady = true;
            m_pSholve->getDragSprite()->runAction(ScaleBy::create(0.3, 1.3));
            m_pBGLayer->runAction(ScaleTo::create(0.3, 1.3));
        }
    }
}
void BakeNormalPizzaScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pSholve) {
        if (m_bPlateReady) {
            Rect rect = m_pPlate->getBoundingBox();
            rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                m_pPizzaRipeNode->setScale(1);
                m_pPizzaRipeNode->setRotation(0);
                m_pSholve->setTouchEnabled(false);
                m_pPizzaRipeNode->retain();
                m_pPizzaRipeNode->removeFromParent();
                m_pPlate->addChild(m_pPizzaRipeNode);
                m_pPizzaRipeNode->setPosition(m_pPlate->getContentSize()*0.5);
                m_pPizzaRipeNode->release();
                
                m_pPlate->runAction(Sequence::create(DelayTime::create(0.5),
                                                     MoveTo::create(0.5, Vec2(visibleSize.width/2, visibleSize.height*0.4)),
                                                     CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("done.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(visibleSize*0.5);
                    
                    m_pGameUI->showNextLayout();
                }), NULL));
                m_pSholve->runAction(MoveBy::create(1, Vec2(-600-visibleSize.width, 100)));
                m_pSholve->getDragSprite()->runAction(Spawn::create(RotateBy::create(1, -87),
//                                                   MoveBy::create(1, Vec2(-600-visibleSize.width, 100)),
                                                   ScaleTo::create(1, 1), NULL));
            }
        }else if (m_bBaked) {
            Rect rect = Rect(visibleSize.width/2-100, visibleSize.height/2-100, 200, 200);
            if (rect.containsPoint(worldPoint)){
//                Node* pPizza = pDragNode->getChildByName("pizza");
//                if (pPizza) {
//                    pPizza->setVisible(true);
//                }
                m_pPizzaRipeNode->setScaleX(1.1);
                m_pPizzaRipeNode->setScaleY(1.4);
                m_pPizzaRipeNode->retain();
                m_pPizzaRipeNode->removeFromParent();
                m_pSholve->addChild(m_pPizzaRipeNode);
                m_pPizzaRipeNode->setPosition(m_pSholve->convertToNodeSpace(m_pSholve->getWorldSpaceActionPoint()));
                m_pPizzaRipeNode->release();
                
                m_pOven->removeAllChildren();
                
                _showPlate();
                pDragNode->setTouchEnabled(false);
            }
            
        }else{
            if (m_bSholveReady) {
                Rect rect = Rect(visibleSize.width/2-100, visibleSize.height/2-100, 200, 200);
                if (rect.containsPoint(worldPoint)) {
                    m_pGuideLayer->removeGuide();
//                    m_pPizzaNode->retain();
//                    m_pPizzaNode->removeFromParent();
//                    m_pOven->addChild(m_pPizzaNode);
//                    m_pPizzaNode->release();
                    
//                    m_pPizzaNode->setPosition(Vec2(470, 350));
//                    m_pPizzaNode->setPosition(m_pPizzaNode->getParent()->convertToNodeSpace(m_pOven->convertToWorldSpace(Vec2(470, 350))));
                    //            m_pPizzaNode->setScale(0.5);
                    
//                    m_pPizzaNode->runAction(Spawn::create(ScaleTo::create(0.3,0.35, 0.12),
//                                                          //                                                      RotateBy::create(0.3, Vec3(-80, 0, 0)),
//                                                          NULL));
                    m_pSholve->setTouchEnabled(false);
//                    m_pSholve->runAction(Spawn::create(RotateBy::create(1, -87),
//                                                       MoveBy::create(1, Vec2(-600, 100)),
//                                                       ScaleTo::create(1, 1), NULL));
                    CMVisibleRect::setPosition(m_pSholve, 410, 260);
                    m_pSholve->getDragSprite()->setRotation3D(Vec3(0, 0, 0));
                    m_pSholve->getDragSprite()->setRotation(90);
                    m_pSholve->getDragSprite()->setScale(0.2,0.5);
                    m_pSholve->setLocalZOrder(0);
                    m_pOvenFront = Sprite::create(localPath("oven2.png"));
                    m_pOvenFront->setPosition(visibleSize*0.5);
                    m_pOvenFront->setScale(1.3);
                    this->addChildToContentLayer(m_pOvenFront);
                    m_pPizzaNode->setScale(0.9);
                    m_pPizzaRipeNode->setRotation(-120);
                    m_pPizzaRipeNode->setScale(1.09,1.36);
                    AudioHelp::getInstance()->playEffect("vo_wait_seconds.mp3");
                    _doBake();
                }
            }else{
                pDragNode->back();
            }
        }
    }
    
}
void BakeNormalPizzaScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}
#pragma mark - initData
void BakeNormalPizzaScene::_initData()
{
    setExPath("content/make/normal/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
}

void BakeNormalPizzaScene::_doBake(float dt)
{
    m_pPizzaRipeNode->setLocalZOrder(5);
    Sprite* pBaked = Sprite::create(localPath("pizza.png"));
    m_pPizzaRipeNode->addChild(pBaked);
//    m_pPizzaRipeNode->setScale(0.4, 0.2);
    pBaked->setOpacity(0);
    
    Sprite* pBakedDec;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    pBakedDec = Sprite::create(FileUtility::getStoragePath()+"normal_decripe.png");
#else
    pBakedDec = Sprite::create(FileUtility::getStoragePath()+"/normal_decripe.png");
#endif
    m_pPizzaRipeNode->addChild(pBakedDec);
    pBakedDec->setPosition(Vec2(0, 28));
    pBakedDec->setOpacity(0);
    pBakedDec->setScale(0.9, 0.6);
    
    pBaked->runAction(Sequence::create(FadeIn::create(3),
                                       CallFunc::create([=](){
        m_pPizzaNode->removeAllChildren();
//        _finishBake();
        m_pSholve->setTouchEnabled(true);
        m_bPlateReady = true;
        AudioHelp::getInstance()->playEffect("vo_bake_finish.mp3");
        _showPlate();
    }), NULL));
    pBakedDec->runAction(FadeIn::create(1));
}

void BakeNormalPizzaScene::_finishBake()
{
    m_bBaked = true;
    
//    Size mixtureSize = m_pieSize;
//    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
//                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
//    Vec2 pos = m_pPizzaRipeNode->getPosition();
//    m_pPizzaRipeNode->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
//    
//    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
//    m_pPizzaRipeNode->visit();
//    render->end();
//    
//    m_pPizzaRipeNode->setPosition(pos);
//    
//    Director::getInstance()->getRenderer()->render();
//    
//    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
//    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
//    
//    Image* pImage = render->newImage();
//    Texture2D* pTextue = new Texture2D();
//    pTextue->initWithImage(pImage);
//    
//    Sprite* pSprite = Sprite::createWithTexture(pTextue);
//    pImage->release();
//    pTextue->release();
//    
//    m_pSholve->addChild(pSprite);
//    pSprite->setName("pizza");
//    pSprite->setPosition(m_pSholve->convertToNodeSpace(m_pSholve->getWorldSpaceActionPoint()));
//    pSprite->setVisible(false);
    
    m_pSholve->runAction(Spawn::create(RotateBy::create(1, 87),
                                       MoveBy::create(1, Vec2(600, -100)),
                                       ScaleTo::create(1, 0.33,0.66),
                                       CallFunc::create([=](){
        m_pSholve->setTouchEnabled(true);
    }), NULL));
}

void BakeNormalPizzaScene::_showPlate()
{
    m_pPlate = Sprite::create(localPath("bottom.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 750+visibleSize.width, 300);
    m_bPlateReady = false;
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_bPlateReady = true;
        m_pSholve->setTouchEnabled(true);
                                         }), NULL));
}