
#include "BaconHamEatScene.h"
#include "SceneManager.h"

BaconHamEatScene::BaconHamEatScene()
{
    m_nEatCount = 0;
}

BaconHamEatScene::~BaconHamEatScene()
{
    
}
bool BaconHamEatScene::init()
{
    if ( !ShareScene::init() )
    {
        return false;
    }
    _initData();
    
    
    _showFood();
//    AudioHelp::getInstance()->playEffect("vo_cant_wait_ry.mp3");
    
    m_pGameUI->showShareUILayout(false);
//    m_pGameUI->showBackLayout();
//    m_pGameUI->showNextLayout2();
    m_pGameUI->showSSCMore();
    m_pGameUI->showMenuUILayout();
    m_pGameUI->hideBack();
    return true;
}

void BaconHamEatScene::onEnter()
{
    ShareScene::onEnter();
}

void BaconHamEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BaconHamEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
//    if (name == "food") {
//        pDragNode->setVisible(true);
//        Sprite* pfoodIn = (Sprite*)pDragNode->getUserData();
//        pfoodIn->setVisible(false);
//    }
}

void BaconHamEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BaconHamEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "food") {
        Rect rect = m_pPlate->getBoundingBox();
//        rect.origin = rect.origin+Vec2(60, 120);
//        rect.size = rect.size-Size(120, 120);
        if (!rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            
            if (m_pTip) {
                m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                   CallFuncN::create([=](Node* node)
                                                                     {
                                                                         node->removeFromParent();
                                                                     }),
                                                   NULL));
                m_pTip = nullptr;
            }
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                
                m_nEatCount++;
                
                if (m_nEatCount>=m_FoodVector.size() && !m_pGameUI->eatAgainIsShow()) {
                    m_pGameUI->showEatAgainLayout();
                    m_pGameUI->showNextLayout2();
                    
                    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(visibleSize*0.5);
                }
            }), NULL));
            
            int type = pDragNode->getTag()%2;
            int count = 3;
            for (int i = 0; i<count; i++) {
                this->runAction(Sequence::create(DelayTime::create(0.1+0.5*i),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
                    if (i<count-1) {
                        if(type==0){
                            pDragNode->getDragSprite()->setTexture(localPath("ham"+std::to_string(i+2)+".png"));
                        }else{
                            pDragNode->getDragSprite()->setTexture(localPath("bacon"+std::to_string(i+2)+".png"));
                        }
                    }else{
                        
                        pDragNode->setVisible(false);
                    }
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
                    this->addChildToUILayer(pParticle);
                    Vec2 pos = pDragNode->getDragSprite()->convertToWorldSpace(Vec2(pDragNode->getDragSprite()->getContentSize().width/2,pDragNode->getDragSprite()->getContentSize().height*(count-0.5-i)/count));
                    pParticle->setPosition(pos);
                }), NULL));
            }
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(0,[=]{
//                Sprite* pfoodIn = (Sprite*)pDragNode->getUserData();
//                pfoodIn->setVisible(true);
//                pDragNode->setVisible(false);
                pDragNode->setTouchEnabled(true);
            });
        }
    }

}

void BaconHamEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setTouchEnabled(false);
    pDragNode->runAction(MoveBy::create(0.5, Vec2(0, 200)));
    if (m_pTip) {
        m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFuncN::create([=](Node* node)
                                                             {
                                                                 node->removeFromParent();
                                                             }),
                                           NULL));
        m_pTip = nullptr;
    }
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        
        m_nEatCount++;
        
        if (m_nEatCount>=m_FoodVector.size() && !m_pGameUI->eatAgainIsShow()) {
            m_pGameUI->showEatAgainLayout();
            m_pGameUI->showNextLayout2();
            
            AudioHelp::getInstance()->playEffect("magic_effect.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
        }
    }), NULL));
    
    int type = pDragNode->getTag()%2;
    int count = 3;
    for (int i = 0; i<count; i++) {
        this->runAction(Sequence::create(DelayTime::create(0.5+0.5*i),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
            if (i<count-1) {
                if(type==0){
                    pDragNode->getDragSprite()->setTexture(localPath("ham"+std::to_string(i+2)+".png"));
                }else{
                    pDragNode->getDragSprite()->setTexture(localPath("bacon"+std::to_string(i+2)+".png"));
                }
            }else{
                
                pDragNode->setVisible(false);
            }
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
            this->addChildToUILayer(pParticle);
            Vec2 pos = pDragNode->getDragSprite()->convertToWorldSpace(Vec2(pDragNode->getDragSprite()->getContentSize().width/2,pDragNode->getDragSprite()->getContentSize().height*(count-0.5-i)/count));
            pParticle->setPosition(pos);
        }), NULL));
    }
}

#pragma mark - initData
void BaconHamEatScene::_initData()
{
    ShareScene::_initData();
    setExPath("content/make/bacon/eat/");
}

void BaconHamEatScene::_showFood()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 320+60);
    m_pPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    Size size = m_pPlate->getContentSize();
//    if (pFood) {
//        size = pFood->getContentSize();
//    }
    m_pFood = MakeEatLayer::create();
    
    for (int i = 0; i<6; i++) {
        std::string path = "";
        if(i%2==0){
            path = "ham1.png";
        }else{
            path = "bacon1.png";
        }
//        Sprite* pSprite = Sprite::create(localPath(path));
//        pSprite->setLocalZOrder(i);
//        pSprite->setPosition(m_pPlate->getPosition()+Vec2(80-(i%2*160)-visibleSize.width, 80-70*(i/2)));
//        if (i==2) {
//            pSprite->setPosition(pSprite->getPosition()+Vec2(30,0));
//        }
//        this->addChildToContentLayer(pSprite);
        
        DragNode* pFood = _createDrageNode(localPath(path));
        
        pFood->setLocalZOrder(i);
        pFood->setPosition(m_pPlate->getPosition()+Vec2(80-(i%2*160), 90-70*(i/2)));
        pFood->setTouchEnabled(false);
//        pFood->setVisible(false);
        pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
            pFood->setOrgPositionDefault();
            pFood->setTouchEnabled(true);
        }), NULL));
//        pFood->setUserData(pSprite);
        pFood->setName("food");
        pFood->setTag(i);
        this->addChildToContentLayer(pFood);
        m_FoodVector.push_back(pFood);
    }
}

void BaconHamEatScene::_reset()
{
    m_nEatCount = 0;
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    
    for (int i = 0; i<m_FoodVector.size(); i++) {
        std::string path = "";
        if(i%2==0){
            path = "ham1.png";
        }else{
            path = "bacon1.png";
        }
        DragNode* pFood = m_FoodVector.at(i);
        pFood->getDragSprite()->setTexture(localPath(path));
        pFood->setPosition(pFood->getOrgPosition());
        pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pFood->setVisible(true);
        }),
                                          MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pFood->setTouchEnabled(true);
        }), NULL));
    }
}



//void BaconHamEatScene::_showEatScene()
//{
//    m_pTip = Sprite::create("content/common/tte.png");
//    this->addChildToUILayer(m_pTip);
//    CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
//    m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
//                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
//                                       DelayTime::create(2),
//                                       CallFunc::create([=](){
//        AudioHelp::getInstance()->playEffect("vo_tap_enjoy.mp3");
//    }), NULL));
//    
//}

void BaconHamEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
    
}

void BaconHamEatScene::onEatEndCallback(Vec2 location)
{
}

