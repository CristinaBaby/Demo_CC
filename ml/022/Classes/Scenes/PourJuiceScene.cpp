
#include "PourJuiceScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"

PourJuiceScene::PourJuiceScene()
{
}

PourJuiceScene::~PourJuiceScene()
{
    
}
bool PourJuiceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg_2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_table1.png"));
    CMVisibleRect::setPosition(pTable, 320, 320);
    this->addChildToBGLayer(pTable);
    
    _showBlenderCup();
    _showSlushyCup();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBlenderCup->getPosition(), m_pCupLayer->getPosition());
    }),  NULL));

    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    return true;
}

void PourJuiceScene::onEnter()
{
    ExtensionScene::onEnter();
    
    
    Scene* scene = this->getScene();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vect(0,0));
    scene->getPhysicsWorld()->step(1/120.0f);
}

void PourJuiceScene::update(float dt)
{
    for(int i=0;i<2;i++);
    getScene()->getPhysicsWorld()->step(1/120.0f);
    
}
void PourJuiceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PourJuiceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(10);
    m_pCup->showShadow(false);
}
void PourJuiceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}
void PourJuiceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    Rect rect = m_pCupLayer->getBoundingBox();
    if (pDragNode->getPosition().y<rect.getMaxY()) {
        pDragNode->setTouchEnabled(false);
        _pourJuice();
    }else{
        pDragNode->back(0,[=](){
            
            m_pCup->showShadow(true);
        });
    }
    
}
void PourJuiceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->back(0,[=](){
        
        m_pCup->showShadow(true);
    });
}
#pragma mark - initData
void PourJuiceScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
}
void PourJuiceScene::_showBlenderCup()
{
    
    DragNode* pCup = _createDrageNode("content/make/blender/blender_cup_down.png");
    pCup->getDragSprite()->setVisible(false);
    this->addChildToContentLayer(pCup);
    CMVisibleRect::setPosition(pCup, 360+visibleSize.width, 620);
    pCup->setTouchEnabled(false);
    m_pBlenderCup = pCup;
    
    m_pCup = BlenderCup::create(false);
    
    m_pCup->initWithIndex(GameDataManager::getInstance()->m_nFlavorIndex);
    m_pCup->setWaterPercent(100);
    m_pCup->showFinished();
    
    m_pCup->setLocalZOrder(2);
    
    m_pCup->showLid(false,false);
    pCup->addChild(m_pCup);
    
    AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    pCup->runAction(Sequence::create(DelayTime::create(0.5),
                                     MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                     CallFunc::create([=](){
        pCup->setOrgPositionDefault();
        pCup->setTouchEnabled(true);
    }), NULL));
    
    m_pCup->onFinishPour = [=](){
        _finish();
    };
}

void PourJuiceScene::_showSlushyCup(){
    int tag = GameDataManager::getInstance()->m_nCupIndex;
    m_pCupLayer = CupLayer::create();
    this->addChildToContentLayer(m_pCupLayer);
    m_pCupLayer->initWithIndex(tag);
    m_pCupLayer->showWater(GameDataManager::getInstance()->m_nFlavorIndex);
    m_pCupLayer->setWaterPercent(0);
    CMVisibleRect::setPositionAdapted(m_pCupLayer, 320, 300-visibleSize.height);
    m_pCupLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                            CallFunc::create([=](){
        m_pCupLayer->showOpenLid();
    }), NULL));
}
void PourJuiceScene::_pourJuice()
{
    m_pBlenderCup->setLocalZOrder(0);
    m_pCupLayer->setLocalZOrder(10);
    m_pBlenderCup->setPosition(m_pCupLayer->getPosition()+Vec2(250, 400));
    m_pBlenderCup->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                              CallFunc::create([=](){
        m_pCup->pourJuice();
        m_pCupLayer->setWaterStreamWorldPoint(m_pCup->getCupWorldPos());
        m_pCupLayer->addWater(3);
        AudioHelp::getInstance()->playLoopEffect("pouring_juice.mp3");
        
    }), NULL));
}

void PourJuiceScene::_finish()
{
    AudioHelp::getInstance()->playEffect("vo_goodjob.mp3");
    m_pCupLayer->pauseAddWater();
    AudioHelp::getInstance()->stopLoopEffect();
    m_pBlenderCup->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                              MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pCupLayer->showOpenLid(false);
    }),
                                              DelayTime::create(1.5),
                                              CallFunc::create([=](){
//        m_pGameUI->showNextLayout();
        SceneManager::replaceTheScene<DecorateScene>();
        
    }), NULL));
}