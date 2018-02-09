
#include "PourWaterScene.h"
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

PourWaterScene::PourWaterScene()
{
    m_pTray = nullptr;
    m_pWaterBottle = nullptr;
}

PourWaterScene::~PourWaterScene()
{
    
}
bool PourWaterScene::init()
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
    
    _showTray();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pWaterBottle->getPosition(), m_pTray->getPosition());
    }),  NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    return true;
}

void PourWaterScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PourWaterScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PourWaterScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    _hideShadow(pDragNode);
    pDragNode->setLocalZOrder(10);
}

void PourWaterScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{

}

void PourWaterScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="WaterBottle") {
        Rect rect = m_pTray->getDragSprite()->getBoundingBox();
        rect.origin = m_pTray->convertToWorldSpace(rect.origin);
        Vec2 point = pDragNode->getPosition();
        point= pDragNode->getParent()->convertToWorldSpace(point);
        if (rect.containsPoint(point)) {
            _pourWater();
        }else{
            pDragNode->back(0,[=](){
                _showShadow(pDragNode);
            });
        }
    }else if (pDragNode==m_pTray) {
//        if (m_nStep==PourWaterScene::eStepMakeIce) {
//            _putTrayInFreezer();
//        }
        
    }
    
}

void PourWaterScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    _showShadow(pDragNode);
//    if (pDragNode==m_pTray) {
//        if(m_nStep==PourWaterScene::eStepCrushIce){
//            m_pTray->setTouchEnabled(false);
//            m_pMachine->setTouchEnable(false);
//            pDragNode->setLocalZOrder(0);
//            _moveawayShelf();
//            m_pTray->runAction(Sequence::create(RotateBy::create(0.3, -50),
//                                                CallFunc::create([=](){
//                _addIceMachine();
//            }), NULL));
//        }
//        
//    }
}


#pragma mark - initData
void PourWaterScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagFreeze;
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
    m_nPourCount = 0;
}

void PourWaterScene::_showShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("freeze/water1.png"));
    }
}

void PourWaterScene::_hideShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("freeze/water1_1.png"));
    }
    
}

void PourWaterScene::_showTray()
{
    if (!m_pTray) {
        std::stringstream ostr;
        ostr<<"freeze/"<<m_nShapeIndex+1<<"/"<<m_nShapeIndex+1<<".png";
        m_pTray = _createDrageNode(localPath(ostr.str()));
        this->addChildToContentLayer(m_pTray);
        
        m_pTrayClipping=  ClippingNode::create();
        m_pTray->addChild(m_pTrayClipping);
        m_pTrayClipping->setPosition(Vec2::ZERO);
        m_pTrayClipping->setAlphaThreshold(0.5);
    }
    m_pTray->setLocalZOrder(1);
    m_pTray->setTouchEnabled(false);
    
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
        pWater->setOpacity(0);
        m_WaterVector.pushBack(pWater);
    }
    std::sort(m_WaterVector.begin(), m_WaterVector.end(), [=](Sprite* pNode0,Sprite* pNode1){
        return pNode0->getTag()<pNode1->getTag();
    });
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 320-visibleSize.width, 212);
    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        Spawn::create(JumpBy::create(0.4, Vec2::ZERO, 10, 1),
                                                      RotateBy::create(0.3, 10), NULL),
                                        RotateBy::create(0.3, -10),
                                        CallFunc::create([=]()
                                                         {
                                                         }), NULL));
    
    if (!m_pWaterBottle) {
        m_pWaterBottle = _createDrageNode(localPath("freeze/water1.png"));
        this->addChildToContentLayer(m_pWaterBottle);
        m_pWaterBottle->setActionPoint(Vec2(13, 302));
        m_pWaterBottle->setName("WaterBottle");
    }
    CMVisibleRect::setPositionAdapted(m_pWaterBottle, 400+visibleSize.width, 580);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(DelayTime::create(1),
                                               CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    }),
                                               EaseSineOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0))),
                                               CallFunc::create([=](){
        m_pWaterBottle->setOrgPositionDefault();
        m_pWaterBottle->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pWaterBottle->getPosition(), m_pTray->getPosition());
    }), NULL));
}



void PourWaterScene::_pourWater()
{
    m_pWaterBottle->setTouchEnabled(false);
    int index = m_nPourCount;
    //    if (m_nPourCount>=m_nUpCount) {
    //        index = m_nPourCount-m_nUpCount;
    //    }
    log("=======%d",index);
    Node* pNode = m_WaterVector.at(index);
    Vec2 point = pNode->getPosition();
    point = pNode->getParent()->convertToWorldSpace(point);
    point = m_pWaterBottle->getParent()->convertToNodeSpace(point);
    //    point.x+=100;
    point.x+=200;
    point.y+=260;
    
    m_pWaterBottle->setPosition(point);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pWaterBottle->setDragSprite(Sprite::create(localPath("freeze/water2.png")));
                                                                    m_pWaterBottle->setRotation(0);
                                                                    m_pWaterBottle->setActionPoint(Vec2(3, 138));
                                                                    
                                                                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                                                                    this->addChildToContentLayer(pParticle);
                                                                    pParticle->setName("dropwater");
                                                                    pParticle->setLocalZOrder(10);
                                                                    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    ParticleSystemQuad* pParticle2  = ParticleSystemQuad::create("particle/water.plist");
                                                                    this->addChildToContentLayer(pParticle2);
                                                                    pParticle2->setName("water");
                                                                    pParticle2->setLocalZOrder(10);
                                                                    
                                                                    pParticle2->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    AudioHelp::getInstance()->playPourWaterEffect();
                                                                }), NULL));
    
    
    if (pNode) {
        pNode->setScale(0.2);
        int temp = m_nPourCount;
        log("====temp %d====",temp);
        pNode->runAction(Sequence::create(DelayTime::create(1),
                                          FadeIn::create(0.1),
                                          ScaleTo::create(1.5, 1),
                                          CallFunc::create([=]()
                                                           {
                                                               log("====temp %d====",temp);
                                                               AudioHelp::getInstance()->stopPourWaterEffect();
                                                               ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("water");
                                                               if (pParticle) {
                                                                   pParticle->stopSystem();
                                                                   pParticle->removeFromParent();
                                                               }
                                                               ParticleSystemQuad* pParticle2 = (ParticleSystemQuad*)m_pContentLayer->getChildByName("dropwater");
                                                               if (pParticle2) {
                                                                   pParticle2->stopSystem();
                                                                   pParticle2->removeFromParent();
                                                               }
                                                               if (temp>=m_nIceCount-1) {
                                                                   log("====1====");
                                                                   
                                                                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 50),
                                                                                                              
                                                                                                              MoveBy::create(0.5, Vec2(visibleSize.width, 100)),
                                                                                                              CallFunc::create([=](){
                                                                       m_pTray->runAction(Spawn::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5, visibleSize.height*0.3)),
                                                                                                        ScaleTo::create(0.5, 1.3), NULL));
                                                                       
                                                                       AudioHelp::getInstance()->playEffect("vo_goodjob.mp3");
//                                                                       m_pGameUI->showNextLayout();
                                                                   }),
                                                                                                              DelayTime::create(1),
                                                                                                              CallFunc::create([=](){
                                                                       SceneManager::replaceTheScene<FreezeScene>();
                                                                   }),NULL));
//                                                                   m_pWaterBottle->setTouchEnabled(true);
                                                                   
                                                               }else{
                                                                   log("====3====");
                                                                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 50),
                                                                                                              CallFunc::create([=](){
                                                                       
                                                                       m_pWaterBottle->setDragSprite(Sprite::create(localPath("freeze/water1_1.png")));
                                                                       m_pWaterBottle->setRotation(0);
                                                                                                                               }),
                                                                                                              MoveTo::create(0.5, m_pWaterBottle->getOrgPosition()),
                                                                                                              CallFunc::create([=]()
                                                                                                                               {
                                                                                                                                   m_pWaterBottle->setTouchEnabled(true);
                                                                                                                                   //                                                                                   _pourWater();
                                                                                                                               }),NULL));
                                                               }
                                                           }), NULL));
    }
    m_nPourCount++;
}

