
#include "NachoDecorateScene.h"
#include "SceneManager.h"

NachoDecorateScene::NachoDecorateScene()
{
    m_nAddCount = 0;
    m_pSalsaBowl = nullptr;
    m_pCreamBowl = nullptr;
    m_pSalsaSpoon = nullptr;
    m_pCreamSpoon = nullptr;
}

NachoDecorateScene::~NachoDecorateScene()
{
    
}
bool NachoDecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/nachos_bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
    Sprite* pTable = Sprite::create(localPath("nachos_bg4.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
    this->addChildToBGLayer(pTable);
    
    _showPan();
    _showSauce();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_sauce_cream.mp3");
    }), NULL));
    m_pGameUI->showNextLayout();
    m_pGameUI->showNormalLayout();
    return true;
}

void NachoDecorateScene::onEnter()
{
    ExtensionScene::onEnter();
}

void NachoDecorateScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if(tag == GameUILayoutLayer::eUIButtonTagNext){
        _saveNachos();
        SceneManager::replaceTheScene<NachoEatScene>();
    }else if(tag == GameUILayoutLayer::eUIButtonTagReset){
        _reset();
        m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagReset);
    }
}
void NachoDecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void NachoDecorateScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void NachoDecorateScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="cream_spoon") {
        if (pDragNode->index==0){
            Rect rect = m_pCreamBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->index = 1;
                pDragNode->getDragSprite()->setTexture(localPath("spoon_cream.png"));
                
                m_pCreamBowl->setTexture(localPath("cream2_2.png"));
            }
        }else if (pDragNode->index==1){
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pPan->getPosition()+Vec2(-50,200));
                pDragNode->runAction(RotateBy::create(0.5, -60));
                
                Sprite* pCream = Sprite::create(localPath("cream3.png"));
                m_pFood->addChild(pCream);
                pCream->setPosition(Vec2(-50, 50));
                pCream->setOpacity(0);
                m_nAddCount++;
                pCream->setName("cream");
                
                m_pCreamBowl->setLocalZOrder(10);
                pDragNode->setLocalZOrder(10);
                this->runAction(Sequence::create(DelayTime::create(2),
                                                 CallFunc::create([=](){
                    
                    m_pGameUI->showResetLayout();
                }), NULL));
                if (m_nAddCount<2) {
                    pCream->runAction(Sequence::create(FadeIn::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("spoon.png"));
                        pDragNode->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pCreamBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    }), NULL));
                }else{
                    m_pGameUI->hideNext();
                    pCream->runAction(Sequence::create(FadeIn::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("spoon.png"));
                    }),
                                                       DelayTime::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pCreamBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        _saveNachos();
                    }),
                                                       DelayTime::create(2),
                                                       CallFunc::create([=](){
                        SceneManager::replaceTheScene<NachoEatScene>();
                    }), NULL));
                }
                
                MenuLayer::addIngredient(MenuLayer::ePackageNachos, MenuLayer::eNachos_cream);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
    }else if (name=="salsa_spoon"){
        if (pDragNode->index==0){
            Rect rect = m_pSalsaBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->index = 1;
                pDragNode->getDragSprite()->setTexture(localPath("spoon_salsa.png"));
                
                m_pSalsaBowl->setTexture(localPath("salsa2_2.png"));
            }
        }else if (pDragNode->index==1){
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pPan->getPosition()+Vec2(50,200));
                pDragNode->runAction(RotateBy::create(0.5, -60));
                
                Sprite* pCream = Sprite::create(localPath("salsa3.png"));
                m_pFood->addChild(pCream);
                pCream->setPosition(Vec2(60, -5));
                pCream->setOpacity(0);
                pCream->setName("salsa");
                
                m_nAddCount++;
                m_pSalsaBowl->setLocalZOrder(10);
                pDragNode->setLocalZOrder(10);
                this->runAction(Sequence::create(DelayTime::create(2),
                                                 CallFunc::create([=](){
                    
                    m_pGameUI->showResetLayout();
                }), NULL));
                
                if (m_nAddCount<2) {
                    pCream->runAction(Sequence::create(FadeIn::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("spoon.png"));
                        pDragNode->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pSalsaBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    }), NULL));
                }else{
                    m_pGameUI->hideNext();
                    pCream->runAction(Sequence::create(FadeIn::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->getDragSprite()->setTexture(localPath("spoon.png"));
                    }),
                                                       DelayTime::create(1),
                                                       CallFunc::create([=](){
                        pDragNode->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pSalsaBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        _saveNachos();
                    }),
                                                       DelayTime::create(2),
                                                       CallFunc::create([=](){
                        SceneManager::replaceTheScene<NachoEatScene>();
                    }), NULL));
                }
                
                MenuLayer::addIngredient(MenuLayer::ePackageNachos, MenuLayer::eNachos_salsa);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
    }
}

void NachoDecorateScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void NachoDecorateScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void NachoDecorateScene::_initData()
{
    setExPath("content/make/nachos/1/");
}

void NachoDecorateScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan2_1.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 340);
    m_pPan->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pPanFront = Sprite::create(localPath("pan2_2.png"));
    this->addChildToContentLayer(m_pPanFront);
    CMVisibleRect::setPositionAdapted(m_pPanFront, 320-visibleSize.width, 340);
    m_pPanFront->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPanFront->setLocalZOrder(5);
    
    m_pFood = Node::create();
    m_pFood->setPosition(m_pPan->getAnchorPointInPoints()+Vec2(0, 20));
    m_pPan->addChild(m_pFood);
//    m_pFood->setPosition(Vec2(-6, 16));
//    m_pFood->setScale(1.3, 0.65);
    
    Sprite* pNachos = Sprite::create(FileUtils::getInstance()->getWritablePath()+"nachos.png");
    if (pNachos) {
        m_pFood->addChild(pNachos);
        pNachos->setPosition(Vec2(-5, -10));
    }
    
}

void NachoDecorateScene::_reset()
{
    m_nAddCount = 0;
    _showSauce();
    Node* pCream = m_pFood->getChildByName("cream");
    if (pCream) {
        pCream->removeFromParent();
    }
    Node* pSalsa = m_pFood->getChildByName("salsa");
    if (pSalsa) {
        pSalsa->removeFromParent();
    }
    MenuLayer::addIngredient(MenuLayer::ePackageNachos, MenuLayer::eNachos_cream,false);
    MenuLayer::addIngredient(MenuLayer::ePackageNachos, MenuLayer::eNachos_salsa,false);
}

void NachoDecorateScene::_showSauce()
{
    if (!m_pCreamBowl) {
        m_pCreamBowl = Sprite::create(localPath("cream2_1.png"));
        this->addChildToContentLayer(m_pCreamBowl);
        CMVisibleRect::setPositionAdapted(m_pCreamBowl, 200+visibleSize.width, 600);
        m_pCreamBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }else{
        if (m_pCreamBowl->getPositionX()<0) {
            m_pCreamBowl->setLocalZOrder(10);
            m_pCreamBowl->setTexture(localPath("cream2_1.png"));
            CMVisibleRect::setPositionAdapted(m_pCreamBowl, 200+visibleSize.width, 600);
            m_pCreamBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
    }
    if (!m_pCreamSpoon) {
        
        m_pCreamSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pCreamSpoon);
        CMVisibleRect::setPositionAdapted(m_pCreamSpoon, 220+visibleSize.width, 630);
        m_pCreamSpoon->setName("cream_spoon");
        
        m_pCreamSpoon->setTouchEnabled(false);
        m_pCreamSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=](){
            m_pCreamSpoon->setOrgPositionDefault();
            m_pCreamSpoon->setTouchEnabled(true);
        }), NULL));
    }else{
        if (m_pCreamSpoon->getPositionX()<0) {
            m_pCreamSpoon->setRotation(0);
            m_pCreamSpoon->index = 0;
            m_pCreamSpoon->setLocalZOrder(10);
            CMVisibleRect::setPositionAdapted(m_pCreamSpoon, 220+visibleSize.width, 630);
            m_pCreamSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                      CallFunc::create([=](){
                m_pCreamSpoon->setOrgPositionDefault();
                m_pCreamSpoon->setTouchEnabled(true);
            }), NULL));
        }
    }
    if (!m_pSalsaBowl) {
        m_pSalsaBowl = Sprite::create(localPath("salsa2_1.png"));
        this->addChildToContentLayer(m_pSalsaBowl);
        CMVisibleRect::setPositionAdapted(m_pSalsaBowl, 450+visibleSize.width, 600);
        m_pSalsaBowl->runAction(Sequence::create(DelayTime::create(1.5),
                                                 CallFunc::create([=](){
            
        }),
                                                 MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    }else{
        if (m_pSalsaBowl->getPositionX()<0) {
            m_pSalsaBowl->setLocalZOrder(10);
            m_pSalsaBowl->setTexture(localPath("salsa2_1.png"));
            CMVisibleRect::setPositionAdapted(m_pSalsaBowl, 450+visibleSize.width, 600);
            m_pSalsaBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
    }
    if (!m_pSalsaSpoon) {
        m_pSalsaSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pSalsaSpoon);
        CMVisibleRect::setPositionAdapted(m_pSalsaSpoon, 470+visibleSize.width, 630);
        
        m_pSalsaSpoon->setName("salsa_spoon");
        
        m_pSalsaSpoon->setTouchEnabled(false);
        m_pSalsaSpoon->runAction(Sequence::create(DelayTime::create(1.5),
                                                  MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=](){
            m_pSalsaSpoon->setOrgPositionDefault();
            m_pSalsaSpoon->setTouchEnabled(true);
        }), NULL));
    }else{
        if (m_pSalsaSpoon->getPositionX()<0) {
            m_pSalsaSpoon->setRotation(0);
            m_pSalsaSpoon->index = 0;
            m_pSalsaSpoon->setLocalZOrder(10);
            CMVisibleRect::setPositionAdapted(m_pSalsaSpoon, 470+visibleSize.width, 630);
            m_pSalsaSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                      CallFunc::create([=](){
                m_pSalsaSpoon->setOrgPositionDefault();
                m_pSalsaSpoon->setTouchEnabled(true);
            }), NULL));
        }
    }
    
    
    
    
}
void NachoDecorateScene::_saveNachos()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pFood) {
        Vec2 pos = m_pFood->getPosition();
        m_pFood->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pFood->visit();
        render->end();
        m_pFood->setPosition(pos);
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"nachos_dec.png", false);
    pImage->autorelease();
}