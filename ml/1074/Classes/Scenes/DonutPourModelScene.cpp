
#include "DonutPourModelScene.h"
#include "SceneManager.h"

DonutPourModelScene::DonutPourModelScene()
{
    m_pSpoon = nullptr;
    m_nPourCount = 0;
}

DonutPourModelScene::~DonutPourModelScene()
{
    for_each(m_renderTextures.begin(), m_renderTextures.end(), [=](MyRenderTexture* pRT){
        CC_SAFE_RELEASE(pRT);
    });
}
bool DonutPourModelScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showBowls();
    _showModel();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pDonutGuide->setVisible(true);
        m_pDonutGuideHighlight->setVisible(true);
        _showSpoon();
        _lightBowl();
    }), NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutPourModelScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutPourModelScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DonutPourModelScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void DonutPourModelScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "spoon") {
        if (pDragNode->index == 0) {
            if(m_nPourCount>m_smallBowls.size()-1){
                return;
            }
            Node* pNode = m_smallBowls.at(m_nPourCount);
            Vec2 pos = pNode->getPosition();
            Rect rect = Rect(pos.x-125, pos.y-80, 250, 160);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                pDragNode->setTag(pNode->getTag());
                m_pGuideLayer->removeGuide();
                pDragNode->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos+Vec2(50, 100)),
                                                                    ScaleTo::create(0.3, 0.6), NULL),
                                                      RotateBy::create(0.3, -20),
                                                      CallFunc::create([=](){
                    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
                    if (pContainer) {
                        pContainer->addFood(localPath("mixture/"+std::to_string(pNode->getTag()+1)+"_4.png"),localPath("spoon_mask.png"),Vec2(-50, -180));
                    }
                    AudioHelp::getInstance()->playEffect("spoon_icecream.mp3");
                }),
                                                      RotateTo::create(0.5, 0),
                                                      Spawn::create(MoveBy::create(0.5,Vec2(50, -50)),
                                                                    ScaleTo::create(0.3, 1), NULL),
                                                      CallFunc::create([=](){
                    pDragNode->index = 1;
                    pDragNode->setTouchEnabled(true);
                    pNode->stopAllActions();
                    pNode->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    if(m_nPourCount<m_smallBowls.size()-2){
                        for (int i = m_nPourCount+1; i<m_smallBowls.size(); i++) {
                            Node* pBowl = m_smallBowls.at(i);
                            pBowl->runAction(MoveBy::create(1, Vec2(-200, 0)));
                        }
                    }
                }), NULL));
            }
        }else if (pDragNode->index == 1) {
            
            for (int i = 0; i<m_renderTextures.size(); i++) {
                MyRenderTexture* pRT = m_renderTextures.at(i);
                int tag = pRT->getTag();
                if(tag<0){
                    continue;
                }
                Vec2 pos = m_pModel->getPosition();
                Rect rect = Rect::ZERO;
                rect.size = pRT->getSprite()->getContentSize();
                rect.origin = pos-Vec2(rect.size.width/2, rect.size.height/2);
                if (rect.containsPoint(worldPoint)) {
                    Vec2 location = m_pModel->convertToNodeSpace(worldPoint)+Vec2(rect.size.width/2, rect.size.height/2);
                    if (pRT->getAlphaWithLocation(location)>200) {
                        Node* pGuideHighlight = m_pDonutGuideHighlight->getChildByTag(tag);
                        if (pGuideHighlight) {
                            pGuideHighlight->setVisible(true);
                        }
                        for (auto pGuide:m_pDonutGuideHighlight->getChildren()) {
                            if (pGuide->getTag() != tag) {
                                pGuide->setVisible(false);
                            }
                        }
                        break;
                    }else{                        
                        for (auto pGuide:m_pDonutGuideHighlight->getChildren()) {
                            pGuide->setVisible(false);
                        }
                    }
                }
            }
        }
    }
}

void DonutPourModelScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "spoon") {
        if (pDragNode->index == 1) {
            for (int i = 0; i<m_renderTextures.size(); i++) {
                MyRenderTexture* pRT = m_renderTextures.at(i);
                int tag = pRT->getTag();
                if(tag<0){
                    continue;
                }
                Vec2 pos = m_pModel->getPosition();
                Rect rect = Rect::ZERO;
                rect.size = pRT->getSprite()->getContentSize();
                rect.origin = pos-Vec2(rect.size.width/2, rect.size.height/2);
                if (rect.containsPoint(worldPoint)) {
                    Vec2 location = m_pModel->convertToNodeSpace(worldPoint)+Vec2(rect.size.width/2, rect.size.height/2);
                    if (pRT->getAlphaWithLocation(location)>200) {
                        pDragNode->setTouchEnabled(false);
                        Node* pGuideHighlight = m_pDonutGuideHighlight->getChildByTag(tag);
                        if (pGuideHighlight) {
                            pGuideHighlight->removeFromParent();
                        }
                        Node* pGuide = m_pDonutGuide->getChildByTag(tag);
                        if (pGuide) {
                            pGuide->removeFromParent();
                        }
                        pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -20),
                                                              CallFunc::create([=](){
                            ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
                            if (pContainer) {
                                pContainer->pourFood(true,0.5);
                            }
                            _addBatter(pDragNode->getTag(),pRT->getTag());
                            AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                        }),
                                                              RotateTo::create(0.5, 0),
                                                              CallFunc::create([=](){
                            pDragNode->index = 0;
                            m_nPourCount++;
                            pRT->setTag(-1);
                            if(m_nPourCount>=m_smallBowls.size()){
                                pDragNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                                _finish();
                            }else{
                                _lightBowl();
                                pDragNode->setTouchEnabled(true);
                            }
                        }), NULL));
                        break;
                    }
                }
            }
            
        }
    }
}

void DonutPourModelScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void DonutPourModelScene::_initData()
{
    setExPath("content/make/donut/make2/");
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void DonutPourModelScene::_showBowls()
{
    std::string flavorStr = GameDataManager::getInstance()->m_sFlavorStr;
    std::vector<std::string> flavorVector = StringHelp::split(flavorStr, ",");
    for (int i = 0; i<5; i++) {
        Node* pBowl = Node::create();
        this->addChildToContentLayer(pBowl);
        CMVisibleRect::setPositionAdapted(pBowl, 150+200*i+visibleSize.width, 750);
        pBowl->setScale(0.5);
        pBowl->setColor(Color3B::GRAY);
        
        int flavorIndex = atoi(flavorVector.at(i).c_str());
        ContainerNode* pContainner = ContainerNode::create(pBowl);
        pContainner->addBody(localPath("bowl_down.png"));
        pContainner->addShadow(localPath("bowl_shadow.png"));
        pContainner->addFront(localPath("bowl_on.png"));
        pContainner->addFood(localPath("mixture/"+std::to_string(flavorIndex+1)+"_4.png"));
        pBowl->setUserData(pContainner);
        
        pBowl->setTag(flavorIndex);
        m_smallBowls.push_back(pBowl);
        pBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width-160*5, 0)),
                                          MoveBy::create(0.3, Vec2(160*5, 0)), NULL));
    }
    
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_add_colored_mixture_mold.mp3");
    }), NULL));
}

void DonutPourModelScene::_showModel()
{
    m_pModel = Node::create();
    this->addChildToContentLayer(m_pModel);
    CMVisibleRect::setPositionAdapted(m_pModel, 320+visibleSize.width, 440);
    m_pModel->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    ContainerNode* pContainer = ContainerNode::create(m_pModel);
    pContainer->addShadow(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_shadow.png"));
    pContainer->addFront(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_on.png"));
    pContainer->addBody(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+".png"));
    m_pModel->setUserData(pContainer);
    
    m_pDonutGuide = Node::create();
    this->addChildToContentLayer(m_pDonutGuide);
    CMVisibleRect::setPositionAdapted(m_pDonutGuide, 320, 440);
    
    m_pDonutGuideHighlight = Node::create();
    this->addChildToContentLayer(m_pDonutGuideHighlight);
    CMVisibleRect::setPositionAdapted(m_pDonutGuideHighlight, 320, 440);
    m_pDonutGuide->setVisible(false);
    m_pDonutGuideHighlight->setVisible(false);
    m_pDonutGuide->setLocalZOrder(15);
    m_pDonutGuideHighlight->setLocalZOrder(15);
    for (int i = 4; i>=0; i--) {
        Sprite* pStencil = Sprite::create(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_mask"+std::to_string(i+1)+".png"));
        
        MyRenderTexture* pRT = MyRenderTexture::create(pStencil->getContentSize().width, pStencil->getContentSize().height);
        pStencil->setPosition(pStencil->getContentSize()*0.5);
        pRT->beginWithClear(1, 1, 1, 0);
        pStencil->visit();
        pRT->end();
        Director::getInstance()->getRenderer()->render();
        
        __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
        pRT->retain();
        pRT->setTag(i);
        m_renderTextures.push_back(pRT);
        
        Sprite*pGuide = Sprite::create(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_dotline"+std::to_string(i+1)+".png"));
        m_pDonutGuide->addChild(pGuide);
        pGuide->setTag(i);
        
        Sprite*pGuideHighlight = Sprite::create(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_brighten"+std::to_string(i+1)+".png"));
        m_pDonutGuideHighlight->addChild(pGuideHighlight);
        pGuideHighlight->setTag(i);
        pGuideHighlight->setVisible(false);
    }
}

void DonutPourModelScene::_showSpoon()
{
    if (!m_pSpoon) {
        m_pSpoon = _createDrageNode(localPath("spoon_down.png"));
        this->addChildToContentLayer(m_pSpoon);
        m_pSpoon->setName("spoon");
        ContainerNode* pContainer = ContainerNode::create(m_pSpoon);
        pContainer->addFront(localPath("spoon_on.png"));
        m_pSpoon->setUserData(pContainer);
    }else{
        m_pSpoon->setDragSprite(Sprite::create(localPath("spoon_down.png")));
    }
    m_pSpoon->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 420+visibleSize.width, 500);
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pSpoon->setTag(0);
    m_pSpoon->setActionPoint(Vec2(78, 40));
    m_pSpoon->setLocalZOrder(20);
    
    auto action = Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0),200,1),
                                   CallFuncN::create([=](Node* pNode){
        m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_smallBowls.at(m_nPourCount)->getPosition());
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pSpoon->runAction(action);
}

void DonutPourModelScene::_lightBowl()
{
    if(m_nPourCount>m_smallBowls.size()-1){
        return;
    }
    Node* pNode = m_smallBowls.at(m_nPourCount);
    pNode->runAction(RepeatForever::create(Sequence::create(JumpBy::create(0.8, Vec2(0, 0), 20, 2), NULL)));
}

void DonutPourModelScene::_addBatter(int flavorIndex,int maskIndex)
{
    m_nFlavors[maskIndex] = flavorIndex;
    Sprite* pStencil = Sprite::create(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_mask"+std::to_string(maskIndex+1)+".png"));
    Sprite* pSprite = Sprite::create(localPath("batter/"+std::to_string(flavorIndex+1)+".png"));
    
    pStencil->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    pSprite->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pStencil->setPosition(pStencil->getContentSize()*0.5);
    pSprite->setPosition(pStencil->getContentSize()*0.5);
    
    MyRenderTexture* pRT = MyRenderTexture::create(pStencil->getContentSize().width, pStencil->getContentSize().height);
    pStencil->setPosition(pStencil->getContentSize()*0.5);
    pRT->beginWithClear(1, 1, 1, 0);
    pSprite->visit();
    pStencil->visit();
    pRT->end();
    Director::getInstance()->getRenderer()->render();
    pStencil->setPosition(Vec2::ZERO);
    
    __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
    
    pRT->setLocalZOrder(6-maskIndex);
    pRT->setOpacity(0);
    pRT->setTag(flavorIndex);
    pRT->setName("num"+std::to_string(maskIndex));
    pRT->runAction(FadeIn::create(2));
    m_pModel->addChild(pRT);
}

void DonutPourModelScene::_finish(){
    
    AudioHelp::getInstance()->stopLoopEffect();
    std::string str;
    for (int i = 0; i<5; i++) {
        Node* pNode = m_pModel->getChildByName("num"+std::to_string(i));
        if (i==0) {
            str = std::to_string(pNode->getTag());
        }else{
            str.append(",");
            str.append(std::to_string(pNode->getTag()));
        }
    }
    GameDataManager::getInstance()->m_sFlavorStr = str;
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DonutBakeScene>();
    }), NULL));
}