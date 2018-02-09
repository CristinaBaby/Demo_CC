
#include "DonutBakeScene.h"
#include "SceneManager.h"
#include "ContainerNode.h"

DonutBakeScene::DonutBakeScene()
{
    
}

DonutBakeScene::~DonutBakeScene()
{
    
}
bool DonutBakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 320, 350);
    m_pOven->setTouchEnable(true);
    m_pOven->setOvenSettings(350, 30);
    m_pGuideLayer->showGuide("content/common/tip/arrow.png", m_pOven->getPosition(),180);
    
    Sprite* pCloth = Sprite::create(localRootPath("common/bg.png"));
    this->addChildToContentLayer(pCloth);
    pCloth->setPosition(visibleSize*0.5);
    
    m_pOven->onDoorCallback = [=](OvenNode* pOven){
        if (pOven->getIsOpen() && !pOven->getBaked()) {
            m_pGuideLayer->removeGuide();
            _showModel();
//            m_pModel->setTouchEnabled(true);
//            m_pGuideLayer->showGuideMove(m_pModel->getPosition(), m_pOven->getPosition());
        }
    };
    m_pOven->onBakeCallback = [=](OvenNode* ,int state){
        if (state==0) {
            _doBake();
        }else{
            m_pOven->openDoor();
            _showGlove();
        }
    };
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


void DonutBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void DonutBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (name == "glove"){
        if (pDragNode->index==0) {
            Rect rect = m_pModel->getDragSprite()->getBoundingBox();
            rect.origin = m_pModel->convertToWorldSpace(rect.origin);
            
            Rect rectGlove = pDragNode->getDragSprite()->getBoundingBox();
            rectGlove.origin = pDragNode->convertToWorldSpace(rectGlove.origin);
            if (rectGlove.intersectsRect(rect)) {
                pDragNode->index = 1;
                m_pModel->retain();
                m_pModel->removeFromParent();
                m_pGlove->addChild(m_pModel);
                m_pModel->release();
                m_pModel->setPosition(Vec2(-130, 50));
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(320, 860)));
                
                Sprite* pGloveFront = Sprite::create("content/make/common/bake/glove1.png");
                m_pGlove->addChild(pGloveFront,10);
                pGloveFront->setName("front");
                //                m_pGuideLayer->removeGuide();
            }
        }
    }
}

void DonutBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    m_pGuideLayer->removeGuide();
    if (name == "model") {
        Rect rect = m_pOven->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setLocalZOrder(1);
            pDragNode->setScale(0.8);
            
            m_pModel->setPosition(Vec2(0,70));
            m_pModel->retain();
            m_pModel->removeFromParent();
            m_pOven->addChild(m_pModel);
            m_pModel->release();
            m_pOven->closeDoor();
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                m_pOven->showControlFrame();
            }),
                                             DelayTime::create(1),
                                             CallFunc::create([=](){
                //                m_pGuideLayer->showguidem();
            }), NULL));
        }else{
            pDragNode->back();
        }
    }else if (name == "glove"){
        if (pDragNode->index==1){
            Vec2 pos = pDragNode->getPosition();
            Vec2 posModel = CMVisibleRect::getPositionAdapted(Vec2(320, 860));
            if (pos.y>posModel.y-200) {
                AudioHelp::getInstance()->playEffect("put.mp3");
                
                m_pModel->retain();
                m_pModel->removeFromParent();
                this->addChildToContentLayer(m_pModel);
                m_pModel->release();
                m_pModel->setPosition(posModel);
                //                m_pModel->setScale(1);
                Sprite* pGloveFront = (Sprite*)m_pGlove->getChildByName("front");
                if (pGloveFront) {
                    pGloveFront->removeFromParent();
                }
                
                m_pGuideLayer->removeGuide();
                m_pGlove->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                    _finishAll();
                }), NULL));
            }
        }
    }
}

void DonutBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void DonutBakeScene::_initData()
{
    setExPath("content/make/donut/make2/");
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void DonutBakeScene::_showModel()
{
    m_pModel = _createDrageNode(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+".png"));
    m_pModel->setName("model");
    this->addChildToContentLayer(m_pModel);
    CMVisibleRect::setPositionAdapted(m_pModel, 320-visibleSize.width, 870);
    m_pModel->setLocalZOrder(10);
    m_pModel->setScale(0.6);
    
    ContainerNode* pContainer = ContainerNode::create(m_pModel);
    pContainer->addShadow(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_shadow.png"));
    pContainer->addFront(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_on.png"));
//    pContainer->addBody(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+".png"));
    m_pModel->setUserData(pContainer);
    
    m_pFood = Node::create();
    m_pModel->addChild(m_pFood);
    
    std::string flavorStr = GameDataManager::getInstance()->m_sFlavorStr;
    std::vector<std::string> flavorVector = StringHelp::split(flavorStr, ",");
    for (int i = 0; i<5; i++) {
        int flavorIndex = atoi(flavorVector.at(i).c_str());
        _addBatter(flavorIndex, i);
    }
    m_pModel->setTouchEnabled(false);
    m_pModel->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pModel->setOrgPositionDefault();
        m_pModel->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pModel->getPosition(), m_pOven->getPosition());
        //        m_pModel->setTouchEnabled(true);
    }), NULL));
}

void DonutBakeScene::_doBake()
{
    AudioHelp::getInstance()->playEffect("vo_bake30minutes.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        
        m_pFood->removeAllChildren();
        ContainerNode* pContainer = (ContainerNode*)m_pModel->getUserData();
        if (pContainer) {
            std::string flavorStr = GameDataManager::getInstance()->m_sFlavorStr;
            std::vector<std::string> flavorVector = StringHelp::split(flavorStr, ",");
            for (int i = 0; i<5; i++) {
                int flavorIndex = atoi(flavorVector.at(i).c_str());
                _addBatter(flavorIndex, i,true);
            }
        }
    }), NULL));
}

void DonutBakeScene::_showGlove()
{
    m_pGlove = _createDrageNode("content/make/common/bake/glove0.png");
    m_pGlove->setName("glove");
    m_pGlove->index = 0;
    this->addChildToContentLayer(m_pGlove);
    CMVisibleRect::setPositionAdapted(m_pGlove, 480+visibleSize.width, 300);
    m_pGlove->setTouchEnabled(false);
    m_pGlove->runAction(Sequence::create(Sequence::create(DelayTime::create(0.5), NULL),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pGlove->setTouchEnabled(true);
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pGlove->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(330, 870)));
    }), NULL));
}

void DonutBakeScene::_addBatter(int flavorIndex,int maskIndex,bool baked)
{
    Sprite* pStencil = Sprite::create(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+"_mask"+std::to_string(maskIndex+1)+".png"));
    std::string str;
    if (baked) {
        str = localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/"+std::to_string(flavorIndex+1)+".png");
    }else{
        str = localPath("batter/"+std::to_string(flavorIndex+1)+".png");
    }
    Sprite* pSprite = Sprite::create(str);
    
    pStencil->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    pSprite->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pStencil->setPosition(pStencil->getContentSize()*0.5);
    pSprite->setPosition(pStencil->getContentSize()*0.5);
    if (baked) {
        str = localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/"+std::to_string(flavorIndex+1)+".png");
        float dt = 0;
        if (m_nShapeIndex==2) {
            dt = 20;
        }else if (m_nShapeIndex==1){
            dt = -20;
        }
        pSprite->setPosition(Vec2(pStencil->getContentSize().width*0.5, pStencil->getContentSize().height*0.5+dt));
    }
    
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
    m_pFood->addChild(pRT);
}


void DonutBakeScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DonutMakeSauceScene>();
    }), NULL));
}