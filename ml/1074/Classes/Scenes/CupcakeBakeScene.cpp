
#include "CupcakeBakeScene.h"
#include "SceneManager.h"

CupcakeBakeScene::CupcakeBakeScene()
{
    
}

CupcakeBakeScene::~CupcakeBakeScene()
{
    
}
bool CupcakeBakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 320, 350);
    m_pOven->setTouchEnable(true);
    m_pOven->setOvenSettings(350, 30);
    m_pGuideLayer->showGuide("content/common/tip/arrow.png", m_pOven->getPosition(),180);
    
    _showTray();
    m_pOven->onDoorCallback = [=](OvenNode* pOven){
        if (pOven->getIsOpen() && !pOven->getBaked()) {
            m_pGuideLayer->removeGuide();
            m_pTray->setTouchEnabled(true);
            m_pGuideLayer->removeGuide();
            m_pGuideLayer->showGuideMove(m_pTray->getPosition(), m_pOven->getPosition());
        }
//        AudioHelp::getInstance()->playEffect("vo_great.mp3");
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

void CupcakeBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CupcakeBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CupcakeBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void CupcakeBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (name == "glove"){
        if (pDragNode->index==0) {
            Rect rect = m_pTray->getDragSprite()->getBoundingBox();
            rect.origin = m_pTray->convertToWorldSpace(rect.origin);
            
            Rect rectGlove = pDragNode->getDragSprite()->getBoundingBox();
            rectGlove.origin = pDragNode->convertToWorldSpace(rectGlove.origin);
            if (rectGlove.intersectsRect(rect)) {
                pDragNode->index = 1;
                m_pTray->retain();
                m_pTray->removeFromParent();
                m_pGlove->addChild(m_pTray);
                m_pTray->release();
                m_pTray->setPosition(Vec2(-130, 100));
                m_pGuideLayer->removeGuide();
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(320, 860)));
                
                Sprite* pGloveFront = Sprite::create("content/make/common/bake/glove1.png");
                m_pGlove->addChild(pGloveFront,10);
                pGloveFront->setName("front");
                //                m_pGuideLayer->removeGuide();
            }
        }
    }
}

void CupcakeBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    
    m_pGuideLayer->removeGuide();
    if (name == "tray") {
        Rect rect = m_pOven->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setLocalZOrder(1);
            pDragNode->setScale(0.7);
            
            m_pTray->setPosition(Vec2(0,70));
            m_pTray->retain();
            m_pTray->removeFromParent();
            m_pOven->addChild(m_pTray);
            m_pTray->release();
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
                
                m_pTray->retain();
                m_pTray->removeFromParent();
                this->addChildToContentLayer(m_pTray);
                m_pTray->release();
                m_pTray->setPosition(posModel);
                m_pTray->setScale(0.6);
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

void CupcakeBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CupcakeBakeScene::_initData()
{
    setExPath("content/make/cupcake/make2/");
}

void CupcakeBakeScene::_showTray()
{
    m_pTray = _createDrageNode(localPath("plate.png"));
    this->addChildToContentLayer(m_pTray);
    CMVisibleRect::setPositionAdapted(m_pTray, 330, 870-visibleSize.height);
    m_pTray->setLocalZOrder(1);
    m_pTray->setName("tray");
    m_pTray->setScale(0.6);
    
    ContainerNode* pContainer = ContainerNode::create(m_pTray);
    pContainer->addShadow(localPath("plate_shadow.png"));
    m_pTray->setUserData(pContainer);
    
    m_pTray->setTouchEnabled(false);
    m_pTray->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                        CallFunc::create([=](){
        m_pTray->setOrgPositionDefault();
//        m_pTray->setTouchEnabled(true);
    }), NULL));
    
    int cupIndex = GameDataManager::getInstance()->m_nShapeIndex;
    std::vector<std::string> flavorVector = StringHelp::split(GameDataManager::getInstance()->m_sFlavorStr, ",");
    int count = 2;
    for (int i = 0; i<4; i++) {
        ClippingNode* pClipping = ClippingNode::create(Sprite::create(localPath("batter/batter_mask.png")));
        pClipping->setAlphaThreshold(0.5);
        m_pTray->getDragSprite()->addChild(pClipping,5);
        pClipping->setTag(i);
        if (i/2>0) {
            pClipping->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
            pClipping->setScale(0.83);
        }else{
            pClipping->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
        }
        for (int k = 0; k<3; k++) {
            std::string flavorStr = flavorVector.at(i*3+k);
            int flavorIndex = atoi(flavorStr.c_str());
            Sprite* pBatter = Sprite::create(localPath("batter/"+std::to_string(flavorIndex+1)+".png"));
            pClipping->addChild(pBatter);
            pBatter->setPosition(Vec2(0, -40+20*k));
        }
        
        m_batterItems.pushBack(pClipping);
        
        Sprite* pCup = Sprite::create(localPath("cup/"+std::to_string(cupIndex+1)+".png"));
        m_pTray->getDragSprite()->addChild(pCup);
        if (i/2>0) {
            pCup->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
            pCup->setScale(0.83);
        }else{
            pCup->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
        }
        pCup->setTag(i);
    }
    
}

void CupcakeBakeScene::_doBake()
{
    AudioHelp::getInstance()->playEffect("vo_bake30minutes.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        std::vector<std::string> flavorVector = StringHelp::split(GameDataManager::getInstance()->m_sFlavorStr, ",");
        for(auto pClipping:m_batterItems) {
            int tag = pClipping->getTag();
            for (int k = 0; k<3; k++) {
                std::string flavorStr = flavorVector.at(tag*3+k);
                int flavorIndex = atoi(flavorStr.c_str());
                Sprite* pBatter = Sprite::create(localRootPath("dec/cake/"+std::to_string(flavorIndex+1)+".png"));
                pClipping->addChild(pBatter);
                pBatter->setPosition(Vec2(32, -34-40+20*tag));
                pBatter->setOpacity(0);
                pBatter->runAction(FadeTo::create(2, 255));
            }
            
        }
    }), NULL));
}

void CupcakeBakeScene::_showGlove()
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

void CupcakeBakeScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CupcakeAddFrostingScene>();
    }), NULL));
}