
#include "DonutPourBatterScene.h"
#include "SceneManager.h"

DonutPourBatterScene::DonutPourBatterScene()
{
    
}

DonutPourBatterScene::~DonutPourBatterScene()
{
    
}
bool DonutPourBatterScene::init()
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
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutPourBatterScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutPourBatterScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DonutPourBatterScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if(pContainer){
        pContainer->showShadow(false);
    }
}

void DonutPourBatterScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void DonutPourBatterScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    for (int i = 0;i<m_smallBowls.size();i++) {
        Rect rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        Node* pBowl = m_smallBowls.at(i);
        Rect rectBowl = Rect(pBowl->getPositionX()-50, pBowl->getPositionY()-50, 100, 100);
        if (rect.intersectsRect(rectBowl)) {
            pDragNode->setTouchEnabled(false);
            
            m_smallBowls.erase(m_smallBowls.begin()+i);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pBowl->getPosition()+Vec2(0, 280)),
                                                  RotateBy::create(0.5, -90),
                                                  CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pouring_sauce.mp3");

                Sprite* pStream = Sprite::create(localPath("basin_batter1.png"));
                pDragNode->addChild(pStream);
                pStream->setLocalZOrder(20);
                pStream->setRotation(90);
                pStream->setAnchorPoint(Vec2(0.5,0.85));
                pStream->setPosition(pDragNode->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                pStream->setName("stream");
                pStream->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("basin_batter"), 1, 2)));
                
                KettleNode* pContainer = (KettleNode*) pDragNode->getUserData();
                if (pContainer) {
                    if (m_smallBowls.size()==0) {
                        pContainer->pourWaterOnce(100);
                    }else{
                        pContainer->pourWaterOnce((6-m_smallBowls.size())*100/30);
                    }
                }
                
                ContainerNode* pContainer2 = (ContainerNode*) pBowl->getUserData();
                if (pContainer2) {
                    pContainer2->getFood()->runAction(FadeIn::create(1.6));
                }
            }),
                                                  MoveBy::create(2,Vec2(80, 30)),
                                                  CallFunc::create([=](){
                auto pStream = pDragNode->getChildByName("stream");
                if (pStream) {
                    pStream->stopAllActions();
                    pStream->runAction(Sequence::create(FadeOut::create(0.5),
                                                        CallFunc::create([=](){
                        pStream->removeFromParent();
                    }), NULL));
                }
                AudioHelp::getInstance()->_playEffectNiceWork();
                
            }),
                                                  DelayTime::create(0.5),
                                                  RotateTo::create(1, 0),
                                                  CallFunc::create([=](){
                if (m_smallBowls.size() == 0) {
                    m_pBigBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                    _finish();
                }else{
                    m_pBigBowl->back(0,[=](){
                        ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
                        if(pContainer){
                            pContainer->showShadow(true);
                        }
                        m_pBigBowl->setTouchEnabled(true);
                    });
                }
                
            }), NULL));
            break;
        }
    }
}

void DonutPourBatterScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if(pContainer){
        pContainer->showShadow(true);
    }
}

#pragma mark - initData
void DonutPourBatterScene::_initData()
{
    setExPath("content/make/donut/make1/");
}


void DonutPourBatterScene::_showBowls()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBigBowl = _createDrageNode(localPath("bowl.png"));
    this->addChildToContentLayer(m_pBigBowl);
    KettleNode* pContainner = KettleNode::create(m_pBigBowl);
    pContainner->addShadow(localPath("bowl_shadow.png"));
    pContainner->addFront(localPath("bowl_on.png"));
    pContainner->addWater(localPath("stir5.png"), localPath("stir5.png"));
    m_pBigBowl->setUserData(pContainner);
    m_pBigBowl->setActionPoint(Vec2(63, 160));
    
    m_pBigBowl->setScale(0.7);
    m_pBigBowl->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pBigBowl, 320-visibleSize.width, 760);
    m_pBigBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                           CallFunc::create([=](){
        m_pBigBowl->setOrgPositionDefault();
        m_pBigBowl->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_pour_mixture_bowl.mp3");
    }), NULL));
    
    setExPath("content/make/donut/make2/");
    for (int i = 0; i<5; i++) {
        Node* pBowl = Node::create();
        this->addChildToContentLayer(pBowl);
        if (i<2) {
            CMVisibleRect::setPositionAdapted(pBowl, 218+235*(i%2)+visibleSize.width, 510);
        }else{
            CMVisibleRect::setPositionAdapted(pBowl, 105+210*((i-2)%3)+visibleSize.width, 340);
        }
        pBowl->setScale(0.6);
        
        ContainerNode* pContainner = ContainerNode::create(pBowl);
        pContainner->addBody(localPath("bowl_down.png"));
        pContainner->addShadow(localPath("bowl_shadow.png"));
        pContainner->addFront(localPath("bowl_on.png"));
        pContainner->addFood(localPath("bowl_batter.png"));
        pBowl->setUserData(pContainner);
        pContainner->getFood()->setOpacity(0);
        
        m_smallBowls.push_back(pBowl);
        pBowl->runAction(Sequence::create(DelayTime::create(0.1*5),
                                          JumpBy::create(1, Vec2(-visibleSize.width, 0), 50, 1), NULL));
    }
}

void DonutPourBatterScene::_finish(){
    
    AudioHelp::getInstance()->stopLoopEffect();
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DonutMixFlavorScene>();
    }), NULL));
}