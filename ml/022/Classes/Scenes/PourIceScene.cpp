
#include "PourIceScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"

PourIceScene::PourIceScene()
{
}

PourIceScene::~PourIceScene()
{
    
}
bool PourIceScene::init()
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
    CMVisibleRect::setPosition(pTable, 320, 500);
    this->addChildToBGLayer(pTable);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        
        _showTray();
        _showBlenderCup();
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pTray->getPosition(), m_pCup->getPosition());
    }),
//                                     DelayTime::create(4),
//                                     CallFunc::create([=](){
//        m_pCup->showLid(true,true);
//        m_pCup->addWater(5);
//    }),
//                                     DelayTime::create(4),
//                                     CallFunc::create([=](){
//        m_pCup->crushIce();
//    }),
                                     NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void PourIceScene::onEnter()
{
    ExtensionScene::onEnter();
    
    
    Scene* scene = this->getScene();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    scene->getPhysicsWorld()->step(1/120.0f);
}

void PourIceScene::update(float dt)
{
//    for(int i=0;i<2;i++);
//    getScene()->getPhysicsWorld()->step(1/120.0f);
    
}
void PourIceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PourIceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(10);
}

void PourIceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void PourIceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name=="tray") {
        Rect rect = m_pTray->getDragSprite()->getBoundingBox();
        rect.origin = m_pTray->convertToWorldSpace(rect.origin);
        Vec2 point = pDragNode->getPosition();
        point= pDragNode->getParent()->convertToWorldSpace(point);
        if (rect.containsPoint(point)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setLocalZOrder(0);
            pDragNode->setPosition(m_pCup->getPosition()+Vec2(-100, 270));
            pDragNode->runAction(Sequence::create(RotateBy::create(0.4, Vec3(50, 40, 0)),
                                                  CallFunc::create([=](){
                _pourIce();
            }),
                                                  DelayTime::create(2),
                                                  RotateBy::create(0.4, Vec3(-50, -40, 0)),
                                                  MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=](){
                m_pGameUI->showNextLayout();
            }), NULL));
        }else{
            pDragNode->back();
        }
    }
    
}

void PourIceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
}

#pragma mark - initData
void PourIceScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagAddJuice;
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void PourIceScene::_showBlenderCup()
{
    m_pCup = BlenderCup::create();
    m_pCup->initWithIndex(0);
    m_pCup->setWaterPercent(100);
    m_pCup->setStateEmpty();
    this->addChildToContentLayer(m_pCup);
    CMVisibleRect::setPosition(m_pCup, 320, 370);
    m_pCup->setWaterPercent(0);
    m_pCup->setLocalZOrder(2);
    
    m_pCup->showLid(false);
}

void PourIceScene::_showTray()
{
    std::stringstream ostr;
    ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<m_nShapeIndex+1<<".png";
    m_pTray = _createDrageNode(localPath(ostr.str()));
    this->addChildToContentLayer(m_pTray);
    m_pTray->setName("tray");
    
    m_pTray->setLocalZOrder(1);
    m_pTray->setTouchEnabled(false);
    m_pTray->setScale(0.6);
    
    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
    m_nIceCount = pCocosNode->getChildrenCount()-1;
    for (int i = 0; i<m_nIceCount; i++) {
        std::stringstream ostr;
        ostr<<"freeze/"<<m_nShapeIndex+1<<"/ice"<<i+1<<".png";
        
        Sprite* pIce = Sprite::create(localPath(ostr.str()));
        m_pTray->addChild(pIce);
        pIce->setTag(i);
        m_IceVector.pushBack(pIce);
    }
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 300-visibleSize.width, 710);
    m_pTray->setTouchEnabled(false);
    AudioHelp::getInstance()->playEffect("slide_in_out.mp3");

    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=]()
                                                         {
                                                             m_pTray->setOrgPositionDefault();
                                                             m_pTray->setTouchEnabled(true);
                                                         }), NULL));
}


void PourIceScene::_pourIce()
{
    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
    for (int i = 0; i<m_nIceCount; i++) {
        Sprite* pIce = m_IceVector.at(i);
        
        Node* pCocosWater = pCocosNode->getChildByName("water"+std::to_string(i+1));
        Vec2 pos = pCocosWater->getPosition();
//        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(510+i%2*80,450+i/2*100));
        pos = m_pTray->convertToWorldSpace(pos);
        this->runAction(Sequence::create(DelayTime::create(0.1+0.2*i),
                                         CallFunc::create([=](){
            pIce->removeFromParent();
            m_pCup->addIce(m_nShapeIndex, pos);
            AudioHelp::getInstance()->playEffect("Ice_fall.mp3");
        }),
                                         DelayTime::create(0.3),
                                         CallFunc::create([=](){
            m_pCup->addIce(m_nShapeIndex, pos);
        }),NULL));
//        if (i == m_nIceCount-1) {
//            this->runAction(Sequence::create(DelayTime::create(5),
//                                             CallFunc::create([=](){
////                m_pGameUI->showNextLayout();
//                SceneManager::replaceTheScene<AddJuiceScene>();
//            }), NULL));
//        }
    }
}
