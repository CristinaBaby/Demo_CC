
#include "ShortcakeBakeScene.h"
#include "SceneManager.h"
#include "ContainerNode.h"

ShortcakeBakeScene::ShortcakeBakeScene()
{
    
}

ShortcakeBakeScene::~ShortcakeBakeScene()
{
    
}
bool ShortcakeBakeScene::init()
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
    
    _showModel();
    m_pOven->onDoorCallback = [=](OvenNode* pOven){
        if (pOven->getIsOpen() && !pOven->getBaked()) {
            m_pGuideLayer->removeGuide();
            m_pModel->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(m_pModel->getPosition(), m_pOven->getPosition());
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

void ShortcakeBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ShortcakeBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


void ShortcakeBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void ShortcakeBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
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

void ShortcakeBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
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
            Vec2 posModel = CMVisibleRect::getPositionAdapted(Vec2(320, 900));
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

void ShortcakeBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ShortcakeBakeScene::_initData()
{
    setExPath("content/make/shortcake/make1/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void ShortcakeBakeScene::_showModel()
{
    m_pModel = _createDrageNode(localPath("cake_mod0.png"));
    m_pModel->setName("model");
    this->addChildToContentLayer(m_pModel);
    CMVisibleRect::setPositionAdapted(m_pModel, 320-visibleSize.width, 870);
    m_pModel->setLocalZOrder(10);
    m_pModel->setScale(0.6);
    
    ContainerNode* pContainer = ContainerNode::create(m_pModel);
    pContainer->addFront(localPath("cake_mod1.png"));
    //    pContainer->addBody(localPath("mould/mould"+std::to_string(m_nShapeIndex+1)+".png"));
    m_pModel->setUserData(pContainer);
    
    m_pFood = Node::create();
    m_pModel->addChild(m_pFood);
    
    m_pBatter = Sprite::create(localPath("mod/mod_batter"+std::to_string(m_nFlavorIndex+1)+".png"));
    m_pFood->addChild(m_pBatter);
    
    m_pModel->setTouchEnabled(false);
    m_pModel->runAction(Sequence::create(DelayTime::create(0.5),
                                         MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pModel->setOrgPositionDefault();
//        m_pModel->setTouchEnabled(true);
//        m_pGuideLayer->showGuideMove(m_pModel->getPosition(), m_pOven->getPosition());
    }), NULL));
}

void ShortcakeBakeScene::_doBake()
{
    AudioHelp::getInstance()->playEffect("vo_bake30minutes.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        m_pBatter->setTexture(localPath("mod/mod_cake"+std::to_string(m_nFlavorIndex+1)+".png"));
    }), NULL));
}

void ShortcakeBakeScene::_showGlove()
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
        m_pGuideLayer->showGuideMove(m_pGlove->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(330, 870)));
    }), NULL));
}

void ShortcakeBakeScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ShortcakeMakeFrostingScene>();
    }), NULL));
}