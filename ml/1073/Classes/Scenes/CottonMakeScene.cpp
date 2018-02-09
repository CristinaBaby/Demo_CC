
#include "CottonMakeScene.h"
#include "SceneManager.h"

CottonMakeScene::CottonMakeScene()
{
    m_pSilk = nullptr;
    m_pCotton = nullptr;
    m_nMoveCount = 0;
    m_nStepCount = 0;
}

CottonMakeScene::~CottonMakeScene()
{
    
}
bool CottonMakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localRootPath("bg/bg2.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    m_pMachine = CottonMachine::create();
    this->addChildToContentLayer(m_pMachine);
    CMVisibleRect::setPositionAdapted(m_pMachine, 320+visibleSize.width, 450,kBorderNone,kBorderBottom);
    m_pMachine->setScale(0.5);
    m_pMachine->setLocalZOrder(1);
    m_pMachine->setSwitchEnable(false);
    m_pMachine->onSwitchTurnOn = CC_CALLBACK_1(CottonMakeScene::onSwitchStateCallback, this);
    
    m_pSugarNode = _createDrageNode(localRootPath("1/bottle_down.png"));
    this->addChild(m_pSugarNode);
    m_pSugarNode->setName("sugarbottle");
    this->addChildToContentLayer(m_pSugarNode);
    m_pSugarNode->setScale(0.7);
    CMVisibleRect::setPosition(m_pSugarNode, 200-visibleSize.width, 720);
    
    m_pSugarBottle = SugarBottle::create();
    m_pSugarBottle->initSugar(m_nFlavorIndex);
    m_pSugarNode->addChild(m_pSugarBottle);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pSugarNode->setTouchEnabled(false);
    m_pSugarNode->setName("sugarbottle");
    m_pSugarNode->runAction(Sequence::create(DelayTime::create(0.5),
                                             MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                             CallFunc::create([=]()
                                                              {
                                                                  m_pSugarNode->setOrgPositionDefault();
                                                                  m_pSugarNode->setTouchEnabled(true);
                                                                  m_pGuideLayer->showGuideMove(m_pSugarNode->getPosition(), m_pMachine->getPosition());
                                                                  AudioHelp::getInstance()->playEffect("vo_cotton_pour_sugar.mp3");
                                                              }), NULL));
    
    m_pMachine->runAction(Sequence::create(DelayTime::create(0.5),
                                           JumpBy::create(1, Vec2(-visibleSize.width, 0), 300, 1),
//                                           DelayTime::create(0.3),
//                                           Spawn::create(ScaleTo::create(1, 1),
//                                                         MoveBy::create(0.8, Vec2(0, -200)),
//                                                         CallFunc::create([=]()
//                                                                          {
//                                                                              _foreground();
//                                                                          }), NULL),
//                                           CallFunc::create([=](){
//        m_pSugarNode->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
//                                                 ScaleTo::create(0.5, 1.3),
//                                                 CallFunc::create([=]()
//                                                                  {
//                                                                      m_pSugarBottle->pourSugar();
//                                                                      m_pMachine->addSugar(m_nFlavorIndex);
//                                                                  }),
//                                                 DelayTime::create(2),
//                                                 MoveBy::create(0.5, Vec2(-visibleSize.width, 0)), NULL));
//    }),
//                                           DelayTime::create(3),
//                                           Spawn::create(ScaleTo::create(1, 0.6),
//                                                         MoveBy::create(0.8, Vec2(0, 200)),
//                                                         CallFunc::create([=]()
//                                                                          {
//                                                                              _distant();
//                                                                          }), NULL),
//                                           CallFunc::create([=]()
//                                                            {
//                                                                m_pGuideLayer->showGuideTap(m_pMachine->getButtonWorldPosition());
//                                                                m_pMachine->setSwitchEnable();
//                                                            }),
                                           NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void CottonMakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

#pragma mark - initData
void CottonMakeScene::_initData()
{
    setExPath("content/make/cotton/2/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
    m_nStickIndex = GameDataManager::getInstance()->m_nStickIndex;
    m_nNextSceneTag = GameUIEvent::eSceneTagCottonChooseShape;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCottonChooseShape;
    m_nStepCount = 0;
    m_nMoveCount = 0;
    m_nFlag = 0;
}

void CottonMakeScene::onSwitchStateCallback(bool on)
{
    if (on) {
        m_pGuideLayer->removeGuide();
        m_pMachine->setSwitchEnable(false);
        
        m_pMachine->runAction(Spawn::create(ScaleTo::create(1, 1),
                                            MoveBy::create(1.1, Vec2(0, -50)), NULL));
        _foreground();
        m_pStick = _createDrageNode(localPath("stick.png"));
        m_pMachine->addChild(m_pStick);
        m_pStick->setPosition(visibleSize);
        m_pStick->setLocalZOrder(5);
        m_pStick->setTouchEnabled(false);
        m_pStick->setPosition(Vec2(visibleSize.width, visibleSize.height+200));
        Rect rect = Rect(-373+230, -373+460, 270, 300);
        rect.origin = m_pMachine->convertToWorldSpace(rect.origin);
        m_pStick->setLimitRect(rect);
        m_pStick->setActionPoint(Vec2(30, 354));
        m_pStick->runAction(Sequence::create(DelayTime::create(2),
                                             MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                             RotateBy::create(0.5,-160),
                                             CallFunc::create([=]()
                                                              {
                                                                  AudioHelp::getInstance()->playEffect("vo_cotton_drag_stick.mp3");
//                                                                  m_pStick->getDragSprite()->runAction(RotateBy::create(0.5,-140));
                                                                  m_pStick->setTouchEnabled(true);
                                                                  MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_stick);
                                                                  MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                                                              }), NULL));
    }
}

void CottonMakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("sugarbottle" == name){
        pDragNode->setLocalZOrder(10);
        return;
    }
    if (!m_pSilk) {
        std::stringstream ostr;
        ostr<<"content/make/cotton/2/silk/"<<m_nFlavorIndex+1<<"_";
        m_pSilk = Sprite::create(string(ostr.str()).append("1.png"));
        m_pSilk->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 4,false,true,0.5));
        m_pMachine->m_pOutClp->addChild(m_pSilk,1);
        //        m_pSilk->setAnchorPoint(Vec2(0.5, 1));
        m_pSilk->setPosition(m_pSilk->getParent()->convertToNodeSpace(worldPoint));
    }
    auto pAction = RepeatForever::create(Sequence::create(CallFunc::create([=](){
        m_pSilk->setScaleX(-1);
    }),DelayTime::create(0.1),
                                                          CallFunc::create([=](){
        m_pSilk->setScaleX(1);
    }),DelayTime::create(0.1), NULL));
    pAction->setTag(10);
    m_pSilk->runAction(pAction);
}
void CottonMakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("sugarbottle" == name) {
        return;
    }
    m_nMoveCount++;
    if (m_nMoveCount==20){
        if (!m_pCotton) {
            std::stringstream ostr;
            ostr<<"content/make/cotton/2/candy/"<<m_nFlavorIndex+1<<"_1.png";
            Sprite* pTempCotton = Sprite::create(string(ostr.str()));
            pDragNode->addChild(pTempCotton);
//            Vec2 pos = pTempCotton->getParent()->convertToNodeSpace(worldPoint);
//            pTempCotton->setPosition(pos);
            pTempCotton->setPosition(Vec2(0, 80));
            std::stringstream ostrAnim;
            ostrAnim<<"content/make/cotton/2/candy/"<<m_nFlavorIndex+1<<"_";
            pTempCotton->runAction(AnimationHelp::createAnimate(ostrAnim.str(), 1, 3,false,true,1));
            pTempCotton->setName("temp");
        }
        
    }
    if (m_nMoveCount%200==0) {
        bool finish = m_pMachine->makeCotton();
        m_nStepCount++;
        std::stringstream ostr;
        if(m_nStepCount==1){
            ostr<<"content/make/cotton/2/candy/"<<m_nFlavorIndex+1<<"_4.png";
            Node* pNode = pDragNode->getChildByName("temp");
            if (pNode) {
                pNode->removeFromParent();
            }
        }
        if (!m_pCotton) {
            m_pCotton = Sprite::create(string(ostr.str()));
            m_pMachine->m_pOutClp->addChild(m_pCotton);
            Vec2 pos = m_pCotton->getParent()->convertToNodeSpace(worldPoint);
            m_pCotton->setPosition(pos);
            m_pCotton->runAction(FadeIn::create(0.5));
            m_pCotton->setRotation(m_pStick->getRotation());
        }
        if(m_nStepCount==1){
            m_pCotton->stopAllActions();
            m_pCotton->setTexture(ostr.str());
            m_pCotton->setScale(0.6);
            m_pCotton->runAction(Sequence::create(ScaleTo::create(5, 1),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       m_nFlag++;
                                                                       if (m_nFlag==2) {
                                                                           _finish();
                                                                       }
                                                                   }), NULL));
        }
        //        if (m_nStepCount>2 && m_nStepCount<5) {
        //            m_pCotton->runAction(ScaleBy::create(0.5, 1.1));
        //        }
        //        if(m_nStepCount==4){
        //            m_pCotton->runAction(ScaleTo::create(1, 1));
        //        }
        if (finish) {
            std::stringstream ostr;
            ostr<<"content/make/cotton/2/silk/"<<m_nFlavorIndex+1<<"_";
            m_pSilk->runAction(Sequence::create(AnimationHelp::createAnimate(ostr.str(), 4, 1,false,true,0.5),
                                                FadeOut::create(1),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_nFlag++;
                                                                     if (m_nFlag==2) {
                                                                         _finish();
                                                                     }
                                                                 }),  NULL));
        }
    }
    if (m_pCotton){
        //        m_pCotton->setPosition(m_pCotton->getParent()->convertToNodeSpace(worldPoint));
        
        Vec2 pos = m_pCotton->getParent()->convertToNodeSpace(worldPoint);
        m_pCotton->setPosition(pos);
    }
    m_pSilk->setPosition(m_pSilk->getParent()->convertToNodeSpace(worldPoint));
}
void CottonMakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if ("sugarbottle" == name) {
        Rect rect = m_pMachine->getBodyRectWorld();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(ScaleTo::create(1, 1));
            pDragNode->getDragSprite()->setVisible(false);
            m_pMachine->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 1),
                                                                 MoveBy::create(1, Vec2(0, -50)),
                                                                 CallFunc::create([=]()
                                                                                  {
                                                                                      _foreground();
                                                                                  }), NULL),
                                                   CallFunc::create([=](){
                m_pSugarNode->setPosition(m_pMachine->getPosition()+Vec2(-200, 400));
                m_pSugarNode->runAction(Sequence::create(ScaleTo::create(0.5, 1.3),
                                                         CallFunc::create([=]()
                                                                          {
                                                                              m_pSugarBottle->pourSugar();
                                                                              m_pMachine->addSugar(m_nFlavorIndex);
                                                                              MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_sugar);
                                                                              MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                                                                          }),
                                                         DelayTime::create(3),
                                                         MoveBy::create(0.5, Vec2(-visibleSize.width, 0)), NULL));
            }),
                                                   DelayTime::create(4),
                                                   Spawn::create(ScaleTo::create(1, 0.6),
                                                                 MoveBy::create(0.8, Vec2(0, 50)),
                                                                 CallFunc::create([=]()
                                                                                  {
                                                                                      _distant();
                                                                                  }), NULL),
                                                   CallFunc::create([=]()
                                                                    {
                                                                        m_pGuideLayer->showGuideTap(m_pMachine->getButtonWorldPosition());
                                                                        m_pMachine->setSwitchEnable();
                                                                    }),
                                                   NULL));
            
        }
        return;
    }
    m_pSilk->stopActionByTag(10);
}
void CottonMakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void CottonMakeScene::_distant(){
    m_pBg->runAction(ScaleTo::create(0.8, 1));
    m_pBg->runAction(MoveBy::create(1,Vec2(0,90)));
//    m_pTable->runAction(MoveBy::create(0.8, Vec2(1, 350)));
//    m_pShed->runAction(MoveBy::create(0.8, Vec2(1, -100)));
}

void CottonMakeScene::_foreground()
{
    m_pBg->runAction(ScaleTo::create(1, 1.2));
    
    m_pBg->runAction(MoveBy::create(1,Vec2(0,-90)));
//    m_pTable->runAction(MoveBy::create(1, Vec2(1, -350)));
//    m_pShed->runAction(MoveBy::create(1, Vec2(1, 100)));
}
void CottonMakeScene::_finish()
{
    MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_machine);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("vo_done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
    
    m_pStick->setTouchEnabled(false);
    Node* pFinishCandy = Node::create();
    pFinishCandy->setRotation(-140);
    Sprite* pStick = Sprite::create(localPath("stick.png"));
    pFinishCandy->addChild(pStick);
    pStick->setAnchorPoint(Vec2(0.5, 1));
    
    std::stringstream ostr;
    ostr<<"content/make/cotton/2/candy/"<<m_nFlavorIndex+1<<"_4.png";
    Sprite* pCandy = Sprite::create(ostr.str());
//    pCandy->setPosition(<#const cocos2d::Vec2 &pos#>)
//    if (m_nShapeIndex==1) {
//        pCandy->setRotation(16);
//    }
//    pCandy->setAnchorPoint(gCandyAnc[m_nShapeIndex]);
    pFinishCandy->addChild(pCandy);
    
    pCandy->runAction(ScaleTo::create(1, 1.5));
    this->addChildToUILayer(pFinishCandy);
    pFinishCandy->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.7));
    pFinishCandy->runAction(Sequence::create(RotateBy::create(1, 140),
//                                             Spawn::create(RotateBy::create(1, 140),
//                                                           ScaleTo::create(1, 1.5), NULL),
                                             CallFunc::create([=](){
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/star0.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setPosition(visibleSize*0.5);
    }),
                                             DelayTime::create(2),
                                             CallFunc::create([=](){
        SceneManager::replaceTheScene<CottonChooseShapeScene>();
    }), NULL));
    m_pCotton->setVisible(false);
    m_pStick->setVisible(false);
    m_pSilk->setVisible(false);
}