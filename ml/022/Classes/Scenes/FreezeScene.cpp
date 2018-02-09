
#include "FreezeScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"
#include "AnimationHelp.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

FreezeScene::FreezeScene()
{
    m_pTray = nullptr;
    
    m_bFreezed = false;
}

FreezeScene::~FreezeScene()
{
    
}
bool FreezeScene::init()
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
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_table2.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 480,kBorderNone,kBorderBottom);
    this->addChildToBGLayer(pTable);
    
    _showTray();
    _showFreezer();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
    pos = m_pGuideLayer->convertToNodeSpace(pos);
    m_pGuideLayer->showGuideTap(pos);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FreezeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FreezeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FreezeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    _hideShadow(pDragNode);
    pDragNode->setLocalZOrder(10);
}

void FreezeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{

}

void FreezeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="tray") {
        Rect rect = m_pFreezer->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)){
            _putTrayInFreezer();
        }
//        if (m_nStep==FreezeScene::eStepMakeIce) {
//            _putTrayInFreezer();
//        }
        
    }
    
}

void FreezeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    _showShadow(pDragNode);
    std::string name = pDragNode->getName();
    if (name=="tray") {
//        if(m_nStep==FreezeScene::eStepCrushIce){
//            m_pTray->setTouchEnabled(false);
//            m_pMachine->setTouchEnable(false);
//            pDragNode->setLocalZOrder(0);
//            _moveawayShelf();
//            m_pTray->runAction(Sequence::create(RotateBy::create(0.3, -50),
//                                                CallFunc::create([=](){
//                _addIceMachine();
//            }), NULL));
//        }
        
    }
}

void FreezeScene::onFreezerCallback(FreezerNode *)
{
    m_pGuideLayer->removeGuide();
    if (m_pFreezer->getIsOpen()) {
        if (!m_bFreezed) {
            m_pFreezer->setTouchEnable(false);
            m_pTray->setTouchEnabled(true);
            Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
            pos = m_pGuideLayer->convertToNodeSpace(pos);
            m_pGuideLayer->showGuideMove(m_pTray->getPosition(), pos);
        }else{
            m_pFreezer->setTouchEnable(false);
            
            AudioHelp::getInstance()->playEffect("vo_great.mp3");
            m_pTray->runAction(Sequence::create(DelayTime::create(0.5),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pTray->setVisible(true);
//                                                                     m_pFreezer->runAction(Sequence::create(DelayTime::create(0.5),
//                                                                                                            MoveBy::create(0.5, Vec2(visibleSize.width*1.5, 200)), NULL));
                                                                     m_pTrayInFreezer->setVisible(false);
                                                                 }),
                                                Spawn::create(ScaleTo::create(0.5, 1.1),
                                                              MoveBy::create(0.5, Vec2(0, -50)), NULL),
                                                DelayTime::create(0.3),
                                                Spawn::create(ScaleTo::create(0.3, 0.9,1.1),
                                                              JumpBy::create(0.5, Vec2::ZERO, 50, 1), NULL),
                                                ScaleTo::create(0.3, 1),
                                                CallFunc::create([=](){
                
//                m_pGameUI->showNextLayout();
                SceneManager::replaceTheScene<PourIceScene>();
            }),
//                                                DelayTime::create(1),
//                                                Spawn::create(MoveTo::create(0.6, CMVisibleRect::getPosition(526, 750)),
//                                                              ScaleTo::create(1, 0.4), NULL),
                                                NULL));
        }
    }else{
    }
}


#pragma mark - initData
void FreezeScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagPourIce;
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
    m_nPourCount = 0;
}

void FreezeScene::_showShadow(DragNode* pDragNode)
{
}

void FreezeScene::_hideShadow(DragNode* pDragNode)
{
}


void FreezeScene::_showTray()
{
    if (!m_pTray) {
        std::stringstream ostr;
        ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<m_nShapeIndex+1<<".png";
        m_pTray = _createDrageNode(localPath(ostr.str()));
        this->addChildToContentLayer(m_pTray);
        m_pTray->setName("tray");
        
        m_pTrayClipping=  ClippingNode::create();
        m_pTray->addChild(m_pTrayClipping);
        m_pTrayClipping->setPosition(Vec2::ZERO);
        m_pTrayClipping->setAlphaThreshold(0.5);
    }
    m_pTray->setLocalZOrder(1);
    m_pTray->setTouchEnabled(false);
    m_pTray->setScale(0.6);
    
    std::stringstream ostr;
    ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<m_nShapeIndex+1<<"_mask.png";
    m_pTrayMask = Sprite::create(localPath(ostr.str()));
    m_pTrayClipping->setStencil(m_pTrayMask);
    
    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
    m_nIceCount = pCocosNode->getChildrenCount()-1;
    for (int i = 0; i<m_nIceCount; i++) {
        std::stringstream ostr;
        ostr<<"freeze/"<<m_nShapeIndex+1<<"/water"<<i+1<<".png";
        Node* pCocosWater = pCocosNode->getChildByName("water"+std::to_string(i+1));
        Vec2 pos = pCocosWater->getPosition();
        
        Sprite* pWater = Sprite::create(localPath(ostr.str()));
        m_pTrayClipping->addChild(pWater);
        pWater->setTag(pCocosWater->getTag());
        pWater->setAnchorPoint(Vec2(0.5, 0.5));
        if(5==m_nShapeIndex){
            pWater->setPosition(pos+Vec2(-30, 10));
        }else{
            pWater->setPosition(pos);
        }
        m_WaterVector.pushBack(pWater);
    }
    std::sort(m_WaterVector.begin(), m_WaterVector.end(), [=](Sprite* pNode0,Sprite* pNode1){
        return pNode0->getTag()<pNode1->getTag();
    });
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 120-visibleSize.width,460,kBorderNone,kBorderBottom);
    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        Spawn::create(JumpBy::create(0.4, Vec2::ZERO, 10, 1),
                                                      RotateBy::create(0.3, 10), NULL),
                                        RotateBy::create(0.3, -10),
                                        CallFunc::create([=]()
                                                         {
                                                         }), NULL));
    
}
void FreezeScene::_showFreezer()
{
    m_pFreezer = FreezerNode::create();
    this->addChildToContentLayer(m_pFreezer);
    m_pFreezer->onDoorCallback = CC_CALLBACK_1(FreezeScene::onFreezerCallback, this);
    CMVisibleRect::setPositionAdapted(m_pFreezer, 370, 350,kBorderNone,kBorderBottom);
//    m_pFreezer->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
}


void FreezeScene::_putTrayInFreezer()
{
    Point worldPoint = m_pTray->getWorldSpaceActionPoint();
    
    Rect rect = m_pFreezer->getDoorWorldRect();
    if (rect.containsPoint(worldPoint)) {
        m_pTray->setTouchEnabled(false);
        m_pTray->setPosition(Vec2(visibleSize.width/2+100, m_pFreezer->getPositionY()+60));
        m_pTray->runAction(Sequence::create(ScaleTo::create(0.5, 0.5),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTrayInFreezer = Sprite::createWithTexture(m_pTray->getDragSprite()->getTexture());
                                                                 Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
                                                                 for (int i = 0; i<m_nIceCount; i++) {
                                                                     std::stringstream ostr;
                                                                     ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<"water"<<i+1<<".png";
                                                                     
                                                                     Node* pCocosWater = pCocosNode->getChildByName("water"+std::to_string(i+1));
                                                                     Vec2 pos = pCocosWater->getPosition();
                                                                     
                                                                     Sprite* pWater = Sprite::create(localPath(ostr.str()));
                                                                     m_pTrayInFreezer->addChild(pWater);
                                                                     pWater->setTag(i);
                                                                     pWater->setPosition(pos+m_pTrayInFreezer->getAnchorPointInPoints());
                                                                 }
                                                                 m_pFreezer->addChild(m_pTrayInFreezer);
                                                                 m_pTrayInFreezer->setScale(0.6,0.5);
                                                                 m_pTrayInFreezer->setPosition(Vec2(50, -50));
                                                                 
                                                                 m_pTray->setVisible(false);
                                                                 m_pFreezer->closeDoor();
                                                                 AudioHelp::getInstance()->playEffect("put_into_fridge.mp3");
                                                             }), NULL));
        m_pFreezer->runAction(Sequence::create(DelayTime::create(1),
                                               MoveBy::create(0.5, Vec2(-70, 0)), NULL));
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pFreezer->doFreeze();
                                                              m_pTrayClipping->setVisible(false);
                                                              m_pTray->setPosition(Vec2(visibleSize.width/2+50, m_pFreezer->getPositionY()));
                                                              
                                                              for (int i = 0; i<m_nIceCount; i++) {
                                                                  std::stringstream ostr;
                                                                  ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<"ice"<<i+1<<".png";
                                                                  Sprite* pIce = Sprite::create(localPath(ostr.str()));
                                                                  m_pTray->addChild(pIce);
                                                                  pIce->setTag(i);
                                                                  
                                                                  pIce->setPosition(Vec2::ZERO);
                                                                  m_IceVector.pushBack(pIce);
                                                                  
                                                                  
                                                              }
                                                          }),
                                         DelayTime::create(5),
                                         CallFunc::create([=]()
                                                          {
                                                              AudioHelp::getInstance()->playEffect("fridge_off.mp3");
                                                          }),
                                         DelayTime::create(0.5),
                                         CallFunc::create([=]()
                                                          {
                                                              m_bFreezed = true;
                                                              m_pFreezer->stopFreeze();
                                                              m_pFreezer->setTouchEnable(true);
                                                              Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
                                                              pos = m_pGuideLayer->convertToNodeSpace(pos);
                                                              m_pGuideLayer->showGuideTap(pos);
                                                          }), NULL));
    }
    
}

