
#include "FriesCutPotatoScene.h"
#include "SceneManager.h"

FriesCutPotatoScene::FriesCutPotatoScene()
{
    m_nCutCount = 0;
    m_nCutStep = 0;
    m_pKnife = nullptr;
    m_pChipLeft = nullptr;
    m_pChipRight = nullptr;
    m_pChipNode = nullptr;
    m_pPlateLeft = nullptr;
}

FriesCutPotatoScene::~FriesCutPotatoScene()
{
    
}
bool FriesCutPotatoScene::init()
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
    
    Sprite* pTable = Sprite::create(localRootPath("bg/bg_make1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320,500);
    this->addChildToBGLayer(pTable);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_cut_potato.mp3");
        _showBoard();
        _showPotatoPlate();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_cut_potato1.mp3");
        
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    return true;
}

void FriesCutPotatoScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FriesCutPotatoScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FriesCutPotatoScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    std::string name = pDragNode->getName();
    if (name == "potato") {
        if (pDragNode->index==0) {
            pDragNode->setVisible(true);
            m_pPotatoRight->setTexture(localPath("basin"+std::to_string(m_nCutStep+2)+".png"));
        }
    }
}

void FriesCutPotatoScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    std::string name = pDragNode->getName();
    if (name == "knife") {
        Rect rect = m_pChipLeft->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            if (m_nCutCount==0) {
                m_pKnife->setDragSprite(Sprite::create(localPath("knife1.png")));
            }
            if (m_nCutCount<=3) {
                pDragNode->setPosition(m_pChipLeft->getPosition()+Vec2(50-m_nCutCount*45, 0));
                pDragNode->runAction(Sequence::create(MoveBy::create(0.2, Vec2(2, -30)),
                                                      CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("cut.mp3");
//                    pDragNode->setLocalZOrder(5);
                }),
                                                      MoveBy::create(0.3, Vec2(2, -50)),
                                                      CallFunc::create([=](){
                    m_pChipLeft->setTexture(localPath("potatoes_1/"+std::to_string(m_nCutCount+3)+".png"));
                    m_pChipRight->setTexture(localPath("potatoes_1/"+std::to_string(m_nCutCount+3)+"_0.png"));
                    Sprite* pChip = Sprite::create(localPath("potatoes_1/"+std::to_string(m_nCutCount+2)+"_0.png"));
                    m_pChipNode->addChild(pChip);
                    
                    
                    if (m_nCutCount==3){
                        m_pChipLeft->runAction(Sequence::create(RotateBy::create(0.2, 30),
                                                                 CallFunc::create([=](){
                            m_pChipLeft->setRotation(0);
//                            m_pChipLeft->setTexture(localPath("potatoes_1/5_2.png"));
                            m_pChipLeft->setTexture(localPath("potatoes_2/1.png"));
//                            m_pChipRight->setTexture(localPath("potatoes_2/"+std::to_string(m_nCutCount-1)+"_2.png"));
                            m_pChipRight->setVisible(false);
                            m_pChipLeft->setLocalZOrder(2);
                            m_pChipRight->setLocalZOrder(6);
                        }), NULL));
//                        m_pChipRight->setPosition(m_pChipLeft->getPosition());
                        m_pChipRight->setPosition(m_pBoard->getPosition()+Vec2(-2,30));
                        m_pKnife->setLocalZOrder(20);
                    }
                    pChip->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("potatoes_1/")+std::to_string(m_nCutCount+2)+"_", 0, m_nCutCount==0?2:1,false),
                                                      CallFunc::create([=](){
                        m_nCutCount++;
                        if (m_nCutCount==4) {
//                            m_pChipRight->setTexture(localPath("potatoes_1/potatoes_1.png"));
                            m_pChipNode->removeAllChildren();
                        }else{
                        }
                        m_pKnife->setTouchEnabled(true);
                    }), NULL));
                }), NULL));
            }else{
                
                pDragNode->setPosition(m_pChipLeft->getPosition()+Vec2(150-(m_nCutCount-4)*50, 0));
                pDragNode->runAction(Sequence::create(MoveBy::create(0.2, Vec2(2, -30)),
                                                      CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("cut.mp3");
//                    pDragNode->setLocalZOrder(5);
                }),
                                                      MoveBy::create(0.3, Vec2(2, -50)),
                                                      CallFunc::create([=](){
                    m_pChipLeft->setTexture(localPath("potatoes_2/"+std::to_string(m_nCutCount-2)+"_1.png"));
//                    m_pChipRight->setTexture(localPath("potatoes_2/"+std::to_string(m_nCutCount-1)+"_2.png"));
                    int count = 3;
                    if (m_nCutCount==4) {
                        count = 1;
                    }else if (m_nCutCount==5 || m_nCutCount==8){
                        count = 2;
                    }else if (m_nCutCount==9){
                        count = 6;
                    }
                    for (int i = 0; i<count; i++) {
                        Sprite* pChip = Sprite::create(localPath("potatoes_2/"+std::to_string(m_nCutCount-2)+"_"+std::to_string(i+3)+".png"));
                        m_pChipNode->addChild(pChip);
                    }
                    
                    
                    if (m_nCutCount==9){
                        m_pKnife->setTouchEnabled(false);
                        _showLeftPlate();
                        m_pPotato->index = 1;
                        
                        if (m_nCutStep==0) {
                            _showLeftPlate();
                        }
                        m_nCutStep++;
                        m_pKnife->back(10,[=](){
                            m_pKnife->setDragSprite(Sprite::create(localPath("knife2.png")));
                            
                            m_pChipLeft->setVisible(false);
                            m_pChipRight->setVisible(false);
                            m_pChipNode->removeAllChildren();
                            m_pPotato->getDragSprite()->setTexture(localPath("potatoes_2/potatoes.png"));
                            m_pPotato->setVisible(true);
                            m_pPotato->setPosition(m_pChipRight->getPosition());
                            m_pPotato->setTouchEnabled(true);
                            m_pGuideLayer->showGuideMove(m_pBoard->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(220, 600)));
                        });
                    }else{
                        
                        m_pKnife->setTouchEnabled(true);
                    }
                    m_nCutCount++;
                }), NULL));
            }
        }
    }
}

void FriesCutPotatoScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "potato") {
        if (pDragNode->index==0) {
            Rect rect = m_pBoard->getBoundingBox();
            Rect potatoRect = pDragNode->getDragSprite()->getBoundingBox();
            potatoRect.origin = pDragNode->convertToWorldSpace(potatoRect.origin);
            if (rect.intersectsRect(potatoRect)) {
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pBoard->getPosition()+Vec2(0,30));
                pDragNode->setVisible(false);
                if (!m_pChipLeft) {
                    m_pChipLeft = Sprite::create(localPath("potatoes_1/2.png"));
                    this->addChildToContentLayer(m_pChipLeft);
                    m_pChipLeft->setPosition(m_pBoard->getPosition()+Vec2(0,30));
                }else{
                    m_pChipLeft->setTexture(localPath("potatoes_1/2.png"));
                }
                if (!m_pChipRight) {
                    m_pChipRight = Sprite::create(localPath("potatoes_1/2_0.png"));
                    this->addChildToContentLayer(m_pChipRight);
                    m_pChipRight->setPosition(m_pBoard->getPosition()+Vec2(-1,30));
                }else{
                    m_pChipRight->setTexture(localPath("potatoes_1/2_0.png"));
                }
                m_pChipLeft->setLocalZOrder(6);
                m_pChipRight->setLocalZOrder(2);
                m_pChipLeft->setVisible(true);
                m_pChipRight->setVisible(true);
                if (!m_pChipNode) {
                    m_pChipNode = Node::create();
                    this->addChildToContentLayer(m_pChipNode);
                    m_pChipNode->setPosition(m_pBoard->getPosition()+Vec2(0,30));
                    m_pChipNode->setLocalZOrder(1);
                }else{
                    
                }
                
                
                
                _showKnife();
            }else{
                pDragNode->setTouchEnabled(false);
                pDragNode->back(0,[=](){
                    pDragNode->setVisible(false);
                    m_pPotatoRight->setTexture(localPath("basin"+std::to_string(m_nCutStep+1)+".png"));
                    pDragNode->setTouchEnabled(true);
                });
            }
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlateLeft->getPosition()+Vec2(50, 100));
            pDragNode->setVisible(false);
            
            int radom = arc4random()%10;
            for (int i = 0; i<10; i++) {
                Sprite* pSprite = Sprite::create(localPath("potatoes_2/potatoes_2_"+std::to_string((radom+i)%4+1)+".png"));
                m_pPlateLeft->addChild(pSprite);
                Vec2 pos = Vec2(30+i/10*3+20.0*(i%10), 100);
                pSprite->setPosition(Vec2(50, 100));
                float rotation = -10.0+5.0*(arc4random()%4);
                pSprite->setRotation(rotation);
                pSprite->setPosition(Vec2(50, 100));
                
                pSprite->runAction(Spawn::create(RotateBy::create(1, (5*(arc4random()%50))),
                                                 MoveTo::create(1, pos),
                                                 CallFunc::create([=](){
                    pSprite->setRotation(rotation);
                }), NULL));
                pSprite->setTag(i);
            }
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                if (m_nCutStep>=3) {
                    _finish();
                }else{
                    pDragNode->setVisible(false);
                    m_pGuideLayer->showGuideMove(m_pPlateRight->getPosition(), m_pBoard->getPosition());
                    m_pPotato->setDragSprite(Sprite::create(localPath("potatoes.png")));
                    m_pPotato->index = 0;
                    CMVisibleRect::setPositionAdapted(m_pPotato, 500, 600);
                    m_pPotato->setOrgPositionDefault();
                    m_pPotato->setTouchEnabled(true);
                }
            }), NULL));
        }
    }
    m_pGuideLayer->removeGuide();
}

void FriesCutPotatoScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FriesCutPotatoScene::_initData()
{
    setExPath("content/make/fries/1/");
}

void FriesCutPotatoScene::_showBoard()
{
    m_pBoard = Sprite::create(localPath("cutting board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 320-visibleSize.width, 310);
    m_pBoard->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void FriesCutPotatoScene::_showPotatoPlate()
{
    m_pPlateRight = Sprite::create(localPath("basin.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 500+visibleSize.width, 600);
    
    m_pPotatoRight = Sprite::create(localPath("basin1.png"));
    m_pPlateRight->addChild(m_pPotatoRight);
    m_pPotatoRight->setPosition(m_pPlateRight->getAnchorPointInPoints());
    
    m_pPotato = _createDrageNode(localPath("potatoes.png"));
    this->addChildToContentLayer(m_pPotato);
    CMVisibleRect::setPositionAdapted(m_pPotato, 500, 600);
    m_pPotato->setVisible(false);
    m_pPotato->setTouchEnabled(false);
    m_pPotato->index = 0;
    m_pPotato->setName("potato");
    
    m_pPlateRight->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pPotato->setOrgPositionDefault();
        m_pPotato->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pPotato->getPosition(), m_pBoard->getPosition());
    }), NULL));
}

void FriesCutPotatoScene::_showLeftPlate()
{
    if (!m_pPlateLeft) {
        m_pPlateLeft = Sprite::create(localPath("plate.png"));
        this->addChildToContentLayer(m_pPlateLeft);
        CMVisibleRect::setPositionAdapted(m_pPlateLeft, 220-visibleSize.width, 600);
        m_pPlateLeft->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    }
}

void FriesCutPotatoScene::_showKnife()
{
    m_nCutCount = 0;
    if (m_pKnife) {
        m_pKnife->back();
        m_pKnife->setTouchEnabled(true);
    }else{
        m_pKnife = _createDrageNode(localPath("knife2.png"));
        this->addChildToContentLayer(m_pKnife);
        CMVisibleRect::setPositionAdapted(m_pKnife, 550+visibleSize.width, 320);
        m_pKnife->setName("knife");
        m_pKnife->setTouchEnabled(false);
        m_pKnife->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
            m_pKnife->setOrgPositionDefault();
            m_pKnife->setTouchEnabled(true);
        }), NULL));
    }
}
void FriesCutPotatoScene::_finish()
{
    MenuLayer::addIngredient(MenuLayer::ePackageFries, MenuLayer::eFries_potato);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FriesFryPotatoScene>();
    }), NULL));
}