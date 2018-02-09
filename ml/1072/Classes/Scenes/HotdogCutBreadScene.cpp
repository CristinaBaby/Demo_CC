
#include "HotdogCutBreadScene.h"
#include "SceneManager.h"

HotdogCutBreadScene::HotdogCutBreadScene()
{    
    m_pSausage = nullptr;
    m_pKnife = nullptr;
    m_nMoveCount = 0;
    m_pSauseRed = nullptr;
    m_pSauseYellow = nullptr;
}

HotdogCutBreadScene::~HotdogCutBreadScene()
{
    
}
bool HotdogCutBreadScene::init()
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
    
    Sprite* pTable = Sprite::create(localRootPath("bg/bg_decoration.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320,500);
    this->addChildToBGLayer(pTable);
    
    _showBread();
    _showKnife();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void HotdogCutBreadScene::onEnter()
{
    ExtensionScene::onEnter();
}


void HotdogCutBreadScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setTouchEnabled(false);
        m_pSauce->setDrawing(false);
        _saveHotdog();
        SceneManager::replaceTheScene<HotdogEatScene>();
        return;
    }else if (GameUILayoutLayer::eUIButtonTagReset==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
        _reset();
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
void HotdogCutBreadScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void HotdogCutBreadScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name=="knife") {
        if (pDragNode->index==0) {
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("knife1.png")));
            m_pGuideLayer->showGuide("content/common/tip/cut_dotline.png", m_pBoard->getPosition());
        }
    }else if (name == "sause"){
        if (pDragNode->index==0) {
            pDragNode->index = 1;
            pDragNode->getDragSprite()->setTexture(localPath("sauce"+std::to_string(pDragNode->getTag()+1)+"_1.png"));
            if (pDragNode==m_pSauseRed && m_pSauseYellow->index!=0) {
                m_pSauseYellow->setTouchEnabled(true);
                m_pSauseYellow->back(0,[=](){
                    m_pSauseYellow->getDragSprite()->setTexture(localPath("sauce1_2.png"));
                });
                m_pSauseYellow->runAction(RotateTo::create(0.5, 0));
                m_pSauseYellow->index = 0;
            }else if (pDragNode==m_pSauseYellow && m_pSauseRed->index!=0){
                m_pSauseRed->setTouchEnabled(true);
                m_pSauseRed->back(0,[=](){
                    m_pSauseRed->getDragSprite()->setTexture(localPath("sauce2_2.png"));
                });
                m_pSauseRed->runAction(RotateTo::create(0.5, 0));
                m_pSauseRed->index = 0;
            }
        }else if (pDragNode->index ==2) {
            m_pSauce->paint(worldPoint);
        }
    }
}

void HotdogCutBreadScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="knife") {
        Rect rect = m_pBread->getBoundingBox();
        rect.origin = m_pBread->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            
            m_nMoveCount++;
            int opacity = m_pCrumbs->getOpacity()+1;
            m_pCrumbs->setOpacity(opacity>255?255:opacity);
            if (m_nMoveCount%20==0) {
                m_nMoveCount = 0;
                AudioHelp::getInstance()->playEffect("cut.mp3");
                m_pCrumbs->runAction(FadeTo::create(3, 255));
                pDragNode->setTouchEnabled(false);
                Vec2 pos = m_pBoard->getPosition();
                pos.y = m_pBoard->getPositionY()-50;
                pos.x += m_pBread->getContentSize().width*0.8;
                pDragNode->setPosition(pos);
                
                Sprite* pTempBread = Sprite::create(localPath("bun1_2.png"));
                this->addChildToContentLayer(pTempBread);
                pTempBread->setPosition(m_pBoard->getPosition());
                pTempBread->setLocalZOrder(15);
                m_pBread->setLocalZOrder(2);
                pDragNode->setLocalZOrder(5);
                pDragNode->runAction(Sequence::create(MoveBy::create(4, Vec2(-m_pBread->getContentSize().width, 0)),
                                                      CallFunc::create([=](){
                    pTempBread->removeFromParent();
                    m_pBread->setTexture(localPath("bun2.png"));
                }),
                                                      MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                      DelayTime::create(1),
                                                      CallFunc::create([=](){
                    _finishCutBread();
                }), NULL));
            }
            
            m_pGuideLayer->removeGuide();
        }
    }else if (name == "sause"){
        if (pDragNode->index==1) {
            pDragNode->index = 2;
            pDragNode->runAction(RotateBy::create(0.5, -170));
            
            m_pSauce->setBrush(Sprite::create(localPath(std::to_string(pDragNode->getTag())+".png")));
        }else if (pDragNode->index==2) {
            Rect rect = m_pBreadFront->getBoundingBox();
            rect.origin = m_pBreadFront->getParent()->convertToWorldSpace(rect.origin);
//            if (rect.containsPoint(worldPoint)) {
                m_pSauce->paint(worldPoint-pDragNode->getOffset(), worldPoint);
//            }
        }
    }
    
}

void HotdogCutBreadScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name=="sausage"){
        Rect rect = m_pBoard->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->removeFromParent();
            m_pFoodNode->addChild(Sprite::create(localPath("bun3_sausage.png")));
            _finishAddSausage();
        }
    }else if (name == "onion"){
        Rect rect = m_pBoard->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pBreadFront->getPosition()+Vec2(0, 240));
            pDragNode->setLocalZOrder(0);
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/onion.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(5);
            pParticle->setPosition(pDragNode->getPosition());
            pDragNode->runAction(ScaleTo::create(1, 0.4));
            
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                Sprite* pOnion = Sprite::create(localPath("bun3_onions.png"));
                m_pFoodNode->addChild(pOnion);
                pOnion->setOpacity(0);
                pOnion->runAction(FadeIn::create(0.6));
            }),
                                             DelayTime::create(0.6),
                                             CallFunc::create([=](){
                _finishAddOnion();
                pDragNode->removeFromParent();
            }), NULL));
        }
    }
    
}

void HotdogCutBreadScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void HotdogCutBreadScene::_initData()
{
    setExPath("content/make/hotdog/5/");
}


void HotdogCutBreadScene::_reset()
{
    m_pSauce->reset();
    if (m_pSauseYellow) {
        m_pSauseYellow->setTouchEnabled(true);
        m_pSauseYellow->back(0,[=](){
            m_pSauseYellow->getDragSprite()->setTexture(localPath("sauce1_2.png"));
        });
        m_pSauseYellow->runAction(RotateTo::create(0.5, 0));
        m_pSauseYellow->index = 0;
    }
    if (m_pSauseRed){
        m_pSauseRed->setTouchEnabled(true);
        m_pSauseRed->back(0,[=](){
            m_pSauseRed->getDragSprite()->setTexture(localPath("sauce2_2.png"));
        });
        m_pSauseRed->runAction(RotateTo::create(0.5, 0));
        m_pSauseRed->index = 0;
    }
}

void HotdogCutBreadScene::_showBread()
{
    m_pBoard = Sprite::create(localPath("board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 320+visibleSize.width, 320);
    m_pBoard->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pBread = Sprite::create(localPath("bun1.png"));
    this->addChildToContentLayer(m_pBread);
    CMVisibleRect::setPositionAdapted(m_pBread, 320+visibleSize.width, 320);
    m_pBread->runAction(action);
    
    m_pCrumbs = Sprite::create(localPath("crumbs.png"));
    this->addChildToContentLayer(m_pCrumbs);
    CMVisibleRect::setPositionAdapted(m_pCrumbs, 320, 320);
    m_pCrumbs->setOpacity(0);
}

void HotdogCutBreadScene::_showSausage()
{
    if (m_pSausage) {
        return;
    }
    AudioHelp::getInstance()->playEffect("vo_hotdog_drag_sausage2bread.mp3");
    m_pSausagePlate = Sprite::create(localPath("plate_sausage.png"));
    this->addChildToContentLayer(m_pSausagePlate);
    CMVisibleRect::setPositionAdapted(m_pSausagePlate, 320+visibleSize.width, 620);
    m_pSausagePlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pSausage = _createDrageNode(localPath("sausage_1.png"));
    this->addChildToContentLayer(m_pSausage);
    m_pSausage->setName("sausage");
    CMVisibleRect::setPositionAdapted(m_pSausage, 320+visibleSize.width, 620);
    m_pSausage->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSausage->setOrgPositionDefault();
        m_pSausage->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pSausage->getPosition(), m_pBoard->getPosition());
    }), NULL));
}

void HotdogCutBreadScene::_showKnife()
{
    
    if (m_pKnife) {
        return;
    }
    AudioHelp::getInstance()->playEffect("vo_hotdog_cut_bread.mp3");
    m_pKnife = _createDrageNode(localPath("knife2.png"));
    this->addChildToContentLayer(m_pKnife);
    CMVisibleRect::setPositionAdapted(m_pKnife, 320+visibleSize.width, 620);
    m_pKnife->setTouchEnabled(false);
    m_pKnife->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pKnife->setOrgPositionDefault();
        m_pKnife->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pKnife->getPosition(), m_pBoard->getPosition());
    }), NULL));
    m_pKnife->setLocalZOrder(1);
    m_pKnife->setName("knife");
    m_pKnife->setActionPoint(Vec2(20, 164));
}

void HotdogCutBreadScene::_showOnion()
{
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){        
        AudioHelp::getInstance()->playEffect("vo_add_hotdog_onions.mp3");
    }), NULL));
    m_pOnionPlate = Sprite::create(localPath("plate_onions.png"));
    this->addChildToContentLayer(m_pOnionPlate);
    CMVisibleRect::setPositionAdapted(m_pOnionPlate, 320+visibleSize.width, 620);
    m_pOnionPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pOnionInPlate = _createDrageNode(localPath("onions.png"));
    this->addChildToContentLayer(m_pOnionInPlate);
    CMVisibleRect::setPositionAdapted(m_pOnionInPlate, 320+visibleSize.width, 620);
    m_pOnionInPlate->setTouchEnabled(false);
    m_pOnionInPlate->setName("onion");
    
    m_pOnionInPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pOnionInPlate->setOrgPositionDefault();
        m_pOnionInPlate->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pOnionInPlate->getPosition(), m_pBoard->getPosition());
    }), NULL));
}

void HotdogCutBreadScene::_showSause()
{
    m_pGameUI->showResetLayout();
    AudioHelp::getInstance()->playEffect("vo_add_ketchup_mustard.mp3");
    m_pSauseYellow = _createDrageNode(localPath("sauce1_2.png"));
    this->addChildToUILayer(m_pSauseYellow);
    CMVisibleRect::setPositionAdapted(m_pSauseYellow, 250+visibleSize.width, 700);
    m_pSauseYellow->setTouchEnabled(false);
    m_pSauseYellow->setName("sause");
    m_pSauseYellow->setTag(0);
    m_pSauseYellow->setActionPoint(Vec2(64, 299));
    
    m_pSauseRed = _createDrageNode(localPath("sauce2_2.png"));
    this->addChildToUILayer(m_pSauseRed);
    CMVisibleRect::setPositionAdapted(m_pSauseRed, 400+visibleSize.width, 700);
    m_pSauseRed->setTouchEnabled(false);
    m_pSauseRed->setName("sause");
    m_pSauseRed->setTag(1);
    m_pSauseRed->setActionPoint(Vec2(64, 299));
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pSauseYellow->runAction(action);
    m_pSauseRed->runAction(action->clone());
    
    m_pSauseClipping = ClippingNode::create(Sprite::create(localPath("bun3_mask.png")));
//    m_pFoodNode->addChild(m_pSauseClipping);
    this->addChildToContentLayer(m_pSauseClipping);
    m_pSauseClipping->setPosition(m_pFoodNode->getPosition());
    m_pSauseClipping->setLocalZOrder(11);
    m_pSauseClipping->setAlphaThreshold(0.5);
    
    m_pSauce = ScribbleSauce::create(m_pBreadFront->getContentSize());
    m_pSauseClipping->addChild(m_pSauce);
    m_pSauce->setPosition(Vec2(-m_pBreadFront->getContentSize().width/2, -m_pBreadFront->getContentSize().height/2));
}

void HotdogCutBreadScene::_finishCutBread()
{
    m_pBreadBack = Sprite::create(localPath("bun3_down.png"));
    this->addChildToContentLayer(m_pBreadBack);
    m_pBreadBack->setPosition(m_pBoard->getPosition());
    
    m_pBreadFront = Sprite::create(localPath("bun3_on.png"));
    this->addChildToContentLayer(m_pBreadFront);
    m_pBreadFront->setLocalZOrder(10);
    m_pBreadFront->setPosition(m_pBoard->getPosition());
    
    m_pFoodNode = Node::create();
    this->addChildToContentLayer(m_pFoodNode);
    m_pFoodNode->setLocalZOrder(5);
    m_pFoodNode->setPosition(m_pBoard->getPosition());
    
    m_pBread->removeFromParent();
    _showSausage();
}

void HotdogCutBreadScene::_finishAddSausage()
{
    m_pSausagePlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=](){
        _showOnion();
    }), NULL));
}

void HotdogCutBreadScene::_finishAddOnion()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
//    this->runAction(Sequence::create(DelayTime::create(2),
//                                     CallFunc::create([=](){
//        SceneManager::replaceTheScene<HotdogGreaseScene>();
//    }), NULL));
    
    m_pOnionPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                              CallFunc::create([=](){
        _showSause();
    }),
                                              DelayTime::create(1.5),
                                              CallFunc::create([=](){
        m_pGameUI->showNextLayout();
    }), NULL));
}
void HotdogCutBreadScene::_saveHotdog()
{
    Size mixtureSize = m_pBreadBack->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    
    Vec2 posFood = m_pFoodNode->getPosition();
    m_pFoodNode->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    
    Vec2 posBread = m_pBreadBack->getPosition();
    m_pBreadBack->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    m_pBreadFront->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    
    Vec2 posSauce = m_pSauseClipping->getPosition();
    m_pSauseClipping->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pBreadBack->visit();
    m_pFoodNode->visit();
    m_pBreadFront->visit();
    m_pSauseClipping->Node::visit();
    
    render->end();
    m_pFoodNode->setPosition(posFood);
    m_pBreadBack->setPosition(posBread);
    m_pBreadFront->setPosition(posBread);
    m_pSauseClipping->setPosition(posSauce);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"hotdog_dec.png", false);
    pImage->autorelease();
}

