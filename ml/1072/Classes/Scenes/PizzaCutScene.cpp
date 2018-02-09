
#include "PizzaCutScene.h"
#include "SceneManager.h"

PizzaCutScene::PizzaCutScene()
{
    m_nCutStep = 0;
    m_nMoveCount = 0;
}

PizzaCutScene::~PizzaCutScene()
{
    
}
bool PizzaCutScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localRootPath("0/bk1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
    this->addChildToBGLayer(pTable);
    
    _showPan();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToContentLayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    m_pGameUI->showNormalLayout();
    return true;
}

void PizzaCutScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PizzaCutScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PizzaCutScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void PizzaCutScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="knife"){
        m_nMoveCount++;
        if (m_nMoveCount%30==0) {
            pDragNode->setTouchEnabled(false);
            m_nMoveCount = 0;
            if (m_nCutStep==0) {
                m_nCutStep++;
                pDragNode->setPosition(m_pFood->getPosition()+Vec2(-100, 100));
//                pDragNode->setLocalZOrder(4);
                pDragNode->runAction(Sequence::create(MoveBy::create(2, Vec2(400, 0)),
                                                      CallFunc::create([=](){
                    Sprite* pSlice = Sprite::create(localPath("silk1.png"));
                    this->addChildToContentLayer(pSlice);
                    pSlice->setPosition(m_pFood->getPosition()+Vec2(0, 10));
                    pSlice->setOpacity(0);
                    pSlice->setScale(1, 0);
                    pSlice->runAction(Spawn::create(FadeTo::create(0.5, 255),
                                                    ScaleTo::create(1, 1),
                                                    CallFunc::create([=](){
                        m_pGuideLayer->showGuide("content/common/tip/cut_dotline.png", m_pFood->getPosition());
                        m_pGuideLayer->getGuide()->setRotation(55);
                        pDragNode->setTouchEnabled(true);
                    }), NULL));
                }), NULL));
                for (int i = 0; i<3; i++) {
                    Node* pPiece = m_pFood->getChildByTag(3+i);
                    if (pPiece) {
                        pPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                           MoveBy::create(1, Vec2(0, 20)), NULL));
                    }
                    Node* pDecPiece = m_pFood->getChildByTag(103+i);
                    if (pDecPiece) {
                        pDecPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                              MoveBy::create(1, Vec2(0, 20)), NULL));
                    }
                }
            }else if (m_nCutStep==1) {
                m_nCutStep++;
                pDragNode->setDragSprite(Sprite::create(localPath("pizza_knife1.png")));
                pDragNode->setScaleX(-1);
                pDragNode->setPosition(m_pFood->getPosition()+Vec2(-30, 220));
                pDragNode->runAction(Sequence::create(MoveBy::create(2, Vec2(180, -260)),
                                                      CallFunc::create([=](){
                    Sprite* pSlice = Sprite::create(localPath("silk1.png"));
                    pSlice->setRotation(45);
                    this->addChildToContentLayer(pSlice);
                    pSlice->setPosition(m_pFood->getPosition());
                    pSlice->setOpacity(0);
                    pSlice->setScale(1, 0);
                    pSlice->runAction(Spawn::create(FadeTo::create(0.5, 255),
                                                    ScaleTo::create(1, 1),
                                                    CallFunc::create([=](){
                        m_pGuideLayer->showGuide("content/common/tip/cut_dotline.png", m_pFood->getPosition());
                        m_pGuideLayer->getGuide()->setRotation(140);
                        pDragNode->setTouchEnabled(true);
                    }), NULL));
                }), NULL));
                
                int index[3] = {4,5,1};
                for (int i = 0; i<3; i++) {
                    Node* pPiece = m_pFood->getChildByTag(index[i]);
                    if (pPiece) {
                        pPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                           MoveBy::create(1, Vec2(20, 20)), NULL));
                    }
                    Node* pDecPiece = m_pFood->getChildByTag(100+index[i]);
                    if (pDecPiece) {
                        pDecPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                              MoveBy::create(1, Vec2(20, 20)), NULL));
                    }
                }
            }else if (m_nCutStep==2) {
                m_nCutStep++;
                pDragNode->setScaleX(1);
                pDragNode->setPosition(m_pFood->getPosition()+Vec2(110, 220));
                pDragNode->runAction(Sequence::create(MoveBy::create(2, Vec2(-240, -240)),
                                                      CallFunc::create([=](){
                    m_pGuideLayer->removeGuide();
                    Sprite* pSlice = Sprite::create(localPath("silk1.png"));
                    pSlice->setRotation(130);
                    this->addChildToContentLayer(pSlice);
                    pSlice->setPosition(m_pFood->getPosition());
                    pSlice->setOpacity(0);
                    pSlice->setScale(1, 0);
                    pSlice->runAction(Spawn::create(FadeTo::create(0.5, 255),
                                                    ScaleTo::create(1, 1),
                                                    CallFunc::create([=](){
//                        pDragNode->setTouchEnabled(true);
                        pDragNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                        _finishAll();
                    }), NULL));
                }), NULL));
                
                int index[3] = {0,1,4};
                for (int i = 0; i<3; i++) {
                    Node* pPiece = m_pFood->getChildByTag(index[i]);
                    if (pPiece) {
                        pPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                           MoveBy::create(1, Vec2(20, -20)), NULL));
                    }
                    Node* pDecPiece = m_pFood->getChildByTag(100+index[i]);
                    if (pDecPiece) {
                        pDecPiece->runAction(Sequence::create(DelayTime::create(1.9),
                                                              MoveBy::create(1, Vec2(20, -20)), NULL));
                    }
                }
            }
        }
    }
}

void PizzaCutScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name=="pizza") {
        Rect rect = m_pPlate->getBoundingBox();
        rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
        
        Rect pizzaRect = m_pPizza->getDragSprite()->getBoundingBox();
        pizzaRect.origin = m_pPizza->convertToWorldSpace(pizzaRect.origin);
        if (pizzaRect.intersectsRect(rect)) {
            m_pPizza->removeFromParent();
            
            m_pFood->setVisible(true);
            _showKnifeTool();
        }else{
            m_pPizza->back(1);
        }
    }
}

void PizzaCutScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void PizzaCutScene::_initData()
{
    setExPath("content/make/pizza/2/");
}
void PizzaCutScene::_showPan()
{
    
    m_pPan = Sprite::create(localPath("pan1.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 720);
    
    m_pPanFront = Sprite::create(localPath("pan1_h.png"));
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setPosition(m_pPan->getPosition());
    m_pPanFront->setLocalZOrder(10);
    
    m_pPizza = _createDrageNode(localPath("bake_pizza1.png"));
    m_pPizza->setName("pizza");
    this->addChildToContentLayer(m_pPizza);
    m_pPizza->setPosition(m_pPan->getPosition());
    
    
    Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"normal_decripe.png");
    if(pDec){
        m_pPizza->addChild(pDec);
        pDec->setName("pizza_decripe");
        pDec->setScale(0.7, 0.4);
    }
    
    m_pPlate = Sprite::create(localPath("dec_pan.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 400);
    
    m_pFood = Node::create();
    this->addChildToContentLayer(m_pFood);
    CMVisibleRect::setPositionAdapted(m_pFood, 320, 430);
    m_pFood->setLocalZOrder(2);
    m_pFood->setVisible(false);
    
    Sprite* pDecTemp = Sprite::create(FileUtils::getInstance()->getWritablePath()+"normal_decripe.png");
    Size sizePizza = Size(414, 262);
    if(pDecTemp){
        pDecTemp->setScale(0.9, 0.7);
        pDecTemp->setPosition(sizePizza*0.5);
    }
    for (int i = 0; i<6; i++) {
        Sprite* pPiece = Sprite::create(localPath("pizza"+std::to_string(6-i)+".png"));
        m_pFood->addChild(pPiece,i+1);
        pPiece->setTag(5-i);
        
        Sprite* pMask = Sprite::create(localPath("pizza"+std::to_string(6-i)+"_mask.png"));
        RenderTexture* pRT = RenderTexture::create(sizePizza.width, sizePizza.height,Texture2D::PixelFormat::RGBA8888,
                                                   GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
        if(pDecTemp){
            pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
            pMask->setPosition(sizePizza*0.5);
            pRT->begin();
            pDecTemp->visit();
            pMask->visit();
            pRT->end();
            
        }
        Director::getInstance()->getRenderer()->render();
        
        __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
        
        if(i==5){
            Image* pImage = pRT->newImage();
            bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"pizza_piece.png", false);
            pImage->autorelease();
        }
        
        Sprite* pDecPiece = pRT->getSprite();
        m_pFood->addChild(pDecPiece,i+1);
        pDecPiece->setPosition(Vec2::ZERO);
        pDecPiece->setTag(105-i);
        
//        pPiece->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
//        pPiece->setPosition(Vec2::ZERO);
    }
    
    m_pPizza->setTouchEnabled(false);
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),NULL));
    
    m_pPanFront->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),NULL));
    
    m_pPlate->runAction(Sequence::create(DelayTime::create(0.5),
                                         MoveBy::create(1, Vec2(visibleSize.width, 0)),NULL));
    
    m_pPizza->runAction(Sequence::create(DelayTime::create(0.5),
                                         MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pPizza->setOrgPositionDefault();
        m_pPizza->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pPizza->getPosition(), m_pPlate->getPosition());
    }), NULL));
}

void PizzaCutScene::_showKnifeTool()
{
    m_pKnife = _createDrageNode(localPath("pizza_knife0.png"));
    this->addChildToContentLayer(m_pKnife);
    m_pKnife->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pKnife, 320+visibleSize.width, 450);
    m_pKnife->setTouchEnabled(false);
    m_pKnife->setName("knife");
    m_pKnife->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pKnife->setOrgPositionDefault();
        m_pKnife->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_cut_pizza.mp3");
        m_pGuideLayer->showGuide("content/common/tip/cut_dotline.png", m_pFood->getPosition());
    }), NULL));
}
void PizzaCutScene::_removeKnifeTool()
{
    AudioHelp::getInstance()->stopLoopEffect();
}

void PizzaCutScene::_savePizza()
{
    Size mixtureSize = m_pPan->getContentSize()*0.7;
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pFood) {
        Node* pPiece = m_pFood->getChildByTag(1);
        Node* pPieceDec = m_pFood->getChildByTag(101);
        if(pPiece){
            Vec2 pos = pPiece->getPosition();
            pPiece->setPosition(Vec2(0, mixtureSize.height+0));
            
            Vec2 posDec = pPieceDec->getPosition();
            pPieceDec->setPosition(Vec2(0, mixtureSize.height+0));
            
            render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
            pPiece->visit();
            if (pPieceDec) {
                pPieceDec->visit();
            }
            render->end();
            pPiece->setPosition(pos);
            pPieceDec->setPosition(posDec);
        }
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"pizza_dec.png", false);
    pImage->autorelease();
}

void PizzaCutScene::_finishAll()
{
    _savePizza();
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<PizzaEatScene>();
    }), NULL));
}