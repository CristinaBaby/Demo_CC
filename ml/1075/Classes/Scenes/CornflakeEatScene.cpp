
#include "CornflakeEatScene.h"
#include "SceneManager.h"

CornflakeEatScene::CornflakeEatScene()
{
    m_nEatCount = 0;
}

CornflakeEatScene::~CornflakeEatScene()
{
    
}
bool CornflakeEatScene::init()
{
    if ( !ShareScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+"Food.png");
    if (pFood) {
//        m_pFood->addChild(pFood);
    }
    
    _showFood();
    _showSpoon();
//    AudioHelp::getInstance()->playEffect("vo_cant_wait_ry.mp3");
    
    m_pGameUI->showShareUILayout(false);
//    m_pGameUI->showBackLayout();
//    m_pGameUI->showNextLayout2();
    m_pGameUI->showSSCMore();
    m_pGameUI->showMenuUILayout();
    m_pGameUI->hideBack();
    return true;
}

void CornflakeEatScene::onEnter()
{
    ShareScene::onEnter();
}

void CornflakeEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CornflakeEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void CornflakeEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="spoon"){
        Rect rect = m_pPlate->getBoundingBox();
        rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
        int count = 3;
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlate->getPosition()+Vec2(110-100*(m_nEatCount/count), 100-70*(m_nEatCount%count)));
            
            if (m_pTip) {
                m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                   CallFuncN::create([=](Node* node)
                                                                     {
                                                                         node->removeFromParent();
                                                                     }),
                                                   NULL));
                m_pTip = nullptr;
            }
            
            pDragNode->setLocalZOrder(1);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                  RotateBy::create(0.5, -20),
                                                  CallFunc::create([=](){
                pDragNode->setLocalZOrder(20);
                m_pFood->doEat(pDragNode->getWorldSpaceActionPoint());
                
                Sprite* pFood = Sprite::create(localPath("cornflake3.png"));
                Sprite* pMask = Sprite::create("content/eatShape2.png");
                Size size = pMask->getContentSize();
                if(pFood){
                    Size size2 = pMask->getContentSize();
                    pFood->setPosition(Vec2(size.width*0.5-size2.width/count-30+110*(m_nEatCount/count), size.height*0.5-size2.height/count-20+70*(m_nEatCount%count)));
                }
                
                RenderTexture* pRT = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                           GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
                if(pFood){
                    pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
                    pMask->setPosition(size*0.5);
                    pRT->begin();
                    pFood->visit();
                    pMask->visit();
                    pRT->end();
                    
                }
                Director::getInstance()->getRenderer()->render();
                
                __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
                __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
                
                
                Sprite* pSpoonIn = Sprite::createWithTexture(pRT->getSprite()->getTexture());
                pSpoonIn->setFlippedY(true);
                //            Sprite* pSpoonIn = pRT->getSprite();
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("in");
                //            pSpoonIn->setScale(0.7);
                pSpoonIn->setPosition(m_pSpoon->convertToNodeSpace(m_pSpoon->getWorldSpaceActionPoint()));
                
                m_nEatCount++;
                pDragNode->runAction(Sequence::create(DelayTime::create(1),
                                                      MoveTo::create(0.5, Vec2(visibleSize.width/2,visibleSize.height*0.2)),
                                                      CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("eat_cream.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setPosition(pDragNode->getWorldSpaceActionPoint());
                    
                    Node* pNode = pDragNode->getChildByName("in");
                    if (pNode) {
                        pNode->removeFromParent();
                    }
                    
                    if(m_nEatCount<count*count){
                        pDragNode->setTouchEnabled(true);
                    }
                    if (m_nEatCount>=count*count && !m_pGameUI->eatAgainIsShow()) {
                        m_pGameUI->showEatAgainLayout();
                        m_pGameUI->showNextLayout2();
                        
                        AudioHelp::getInstance()->playEffect("magic_effect.mp3");
                        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                        this->addChildToUILayer(pParticle);
                        pParticle->setLocalZOrder(10);
                        pParticle->setPosition(visibleSize*0.5);
                    }
                }), NULL));
            }), NULL));
            
        }
        
    }
}

void CornflakeEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void CornflakeEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CornflakeEatScene::_initData()
{
    setExPath("content/make/cornmeal/");
}

void CornflakeEatScene::_showFood()
{
    m_pPlate = Sprite::create(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 320+60);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    
    m_pPanFront = Sprite::create(localPath("bowl1.png"));
    this->addChildToContentLayer(m_pPanFront);
    CMVisibleRect::setPositionAdapted(m_pPanFront, 320-visibleSize.width, 320+60);
    m_pPanFront->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPanFront->setLocalZOrder(8);
    
    
    Sprite* pFood = Sprite::create(localPath("cornflake3.png"));
    Size size = m_pPlate->getContentSize();
    if (pFood) {
        size = pFood->getContentSize();
    }
    m_pFood = MakeEatLayer::create();
    
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    if (pFood) {
        pFood->setPosition(Vec2(size.width/2, size.height/2));
        pFood->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    m_pFood->setRenderTexture(rt);
    m_pFood->setPosition(Vec2(-size.width/2, -size.height/2));
    m_pFood->setShapeTexture(Sprite::create("content/eatShape2.png")->getTexture());
    this->addChildToContentLayer(m_pFood);
    CMVisibleRect::setPositionAdapted(m_pFood, 320-size.width/2-visibleSize.width, 350-size.height/2+60);
    m_pFood->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pFood->setLocalZOrder(5);
}

void CornflakeEatScene::_showSpoon()
{
    m_pSpoon = _createDrageNode(localPath("spoon.png"));
    this->addChildToContentLayer(m_pSpoon);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 350+visibleSize.width, 400);
    
    m_pSpoon->setName("spoon");
    m_pSpoon->setActionPoint(Vec2(70, 38));
    m_pSpoon->getDragSprite()->setAnchorPoint(Vec2(70.0/m_pSpoon->getContentSize().width, 38.0/m_pSpoon->getContentSize().width));
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pSpoon->setOrgPositionDefault();
        m_pSpoon->setTouchEnabled(true);
    }), NULL));
    m_pSpoon->setLocalZOrder(20);
}

void CornflakeEatScene::_reset()
{
    m_nEatCount = 0;
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pPlate->removeFromParent();
    }), NULL));
    m_pPanFront->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pPanFront->removeFromParent();
    }), NULL));
    m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pFood->removeFromParent();
    }), NULL));
    m_pSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSpoon->removeFromParent();
    }), NULL));
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showFood();
        _showSpoon();
    }), NULL));
}

//void CornflakeEatScene::_showEatScene()
//{
//    m_pTip = Sprite::create("content/common/tte.png");
//    this->addChildToUILayer(m_pTip);
//    CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
//    m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
//                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
//                                       DelayTime::create(2),
//                                       CallFunc::create([=](){
//        AudioHelp::getInstance()->playEffect("vo_tap_enjoy.mp3");
//    }), NULL));
//    
//}

void CornflakeEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
    
}

void CornflakeEatScene::onEatEndCallback(Vec2 location)
{
}

