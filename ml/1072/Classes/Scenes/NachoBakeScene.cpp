
#include "NachoBakeScene.h"
#include "SceneManager.h"

NachoBakeScene::NachoBakeScene()
{
    
}

NachoBakeScene::~NachoBakeScene()
{
    
}
static std::string sgIngredient[] = {
    "nachos",
    "bean",
    "tomoto",
    "onion",
    "pepper",
    "cheese",
    //    {"",""},
};
bool NachoBakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/nachos_bg3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    _showPan();
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 320, 450);
    m_pOven->setTouchEnable(true);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuide("content/common/tip/arrow2.png", m_pOven->getPosition());
    }), NULL));
    m_pOven->onDoorCallback = [=](OvenNode* pOven){
        if (pOven->getIsOpen() && !pOven->getBaked()) {
            m_pGuideLayer->removeGuide();
            m_pGuideLayer->showGuideMove(m_pPan->getPosition(), m_pOven->getPosition());
            m_pPan->setTouchEnabled(true);
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
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void NachoBakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void NachoBakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void NachoBakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void NachoBakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (name == "pan") {
        
    }else if (name == "glove"){
        if (pDragNode->index==0) {
            Rect rect = m_pPan->getDragSprite()->getBoundingBox();
            rect.origin = m_pPan->convertToWorldSpace(rect.origin);
            
            Rect rectGlove = pDragNode->getDragSprite()->getBoundingBox();
            rectGlove.origin = pDragNode->convertToWorldSpace(rectGlove.origin);
            if (rectGlove.intersectsRect(rect)) {
                pDragNode->index = 1;
                m_pPan->retain();
                m_pPan->removeFromParent();
                m_pGlove->addChild(m_pPan);
                m_pPan->release();
                m_pPan->setPosition(Vec2(-130, 130));
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), CMVisibleRect::getPositionAdapted(Vec2(320, 860)));
                
                Sprite* pGloveFront = Sprite::create("content/make/common/bake/glove1.png");
                m_pGlove->addChild(pGloveFront,10);
                pGloveFront->setName("front");
//                m_pGuideLayer->removeGuide();
            }
        }
    }
}

void NachoBakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    m_pGuideLayer->removeGuide();
    if (name == "pan") {
        Rect rect = m_pOven->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setLocalZOrder(1);
            pDragNode->setScale(0.8);
            
            m_pPan->setPosition(Vec2(0,-50));
            m_pPan->retain();
            m_pPan->removeFromParent();
            m_pOven->addChild(m_pPan);
            m_pPan->release();
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
            Vec2 posPan = CMVisibleRect::getPositionAdapted(Vec2(320, 860));
            if (pos.y>posPan.y-200) {
                
                m_pPan->retain();
                m_pPan->removeFromParent();
                this->addChildToContentLayer(m_pPan);
                m_pPan->release();
                m_pPan->setPosition(posPan);
//                m_pPan->setScale(1);
                Sprite* pGloveFront = (Sprite*)m_pGlove->getChildByName("front");
                if (pGloveFront) {
                    pGloveFront->removeFromParent();
                }
                
                m_pGuideLayer->removeGuide();
                m_pGlove->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                    _finish();
                }), NULL));
            }
        }
    }
}

void NachoBakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void NachoBakeScene::_initData()
{
    setExPath("content/make/nachos/1/");
}

void NachoBakeScene::_showPan()
{
    float scale = 0.6;
    m_pPan = _createDrageNode(localPath("pan2_1.png"));
    m_pPan->setName("pan");
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 860);
    m_pPan->setScale(scale);
//    Sprite* pFront = Sprite::create(localPath("pan2_2.png"));
//    m_pPan->addChild(pFront,5);
    
    m_pFood = Node::create();
    m_pPan->addChild(m_pFood,5);
    m_pFood->setPosition(Vec2(-10, 20));
    m_pFood->setScale(1.2, 0.62);
    
    std::vector<IngredientData> dataVector = GameDataManager::getInstance()->m_VectorIngredient;
    int index = 0;
    for_each(dataVector.begin(), dataVector.end(), [=,&index](IngredientData data){
        std::string name = sgIngredient[data.type];
        Sprite* pUnit = Sprite::create(localRootPath("0/"+name+std::to_string(data.index)+".png"));
        m_pFood->addChild(pUnit);
        pUnit->setTag(index);
        pUnit->setName(name);
        pUnit->setRotation(data.rotation);
        pUnit->setPosition(Vec2(data.posX, data.posY));
        index++;
    });
    
    m_pPan->setTouchEnabled(false);
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
//        m_pPan->setTouchEnabled(true);
    }), NULL));
}

void NachoBakeScene::_doBake()
{
    AudioHelp::getInstance()->playEffect("vo_bake.mp3");
    Vector<Node*> pChildren = m_pFood->getChildren();
    int index = 0;
    int nachosCount = GameDataManager::getInstance()->m_nNachosCount;
    std::string name = "nachos_0.png";
    if (nachosCount>3&& nachosCount<6) {
        name = "nachos_1.png";
    }else if (nachosCount>=6) {
        name = "nachos_2.png";
    }
    m_pNachos = Sprite::create(localPath(name));
    m_pPan->addChild(m_pNachos);
    m_pNachos->setPosition(Vec2(0, 30));
    m_pNachos->setOpacity(0);
    m_pNachos->runAction(Sequence::create(DelayTime::create(1),
                                        FadeIn::create(1), NULL));
    
    for_each(pChildren.begin(), pChildren.end(), [=,&index](Node* pChild){
        std::string name = pChild->getName();
        if (name == "cheese") {
            Sprite* pUnit = (Sprite*)pChild;
            index++;
            if (pUnit) {
                float dt = 2+0.02*index;
                if (dt >4) {
                    dt = 2+0.4*(arc4random()%5);
                }
                pUnit->runAction(Sequence::create(DelayTime::create(dt),
                                                  FadeTo::create(1, 180),
                                                  CallFunc::create([=](){
                    Sprite* bubble = Sprite::create(localPath("cheese_bubble.png"));
                    m_pPan->addChild(bubble,10);
                    bubble->setPosition(pUnit->getPosition());
                    bubble->setOpacity(0);
                    bubble->setAnchorPoint(Vec2(0.5, 0));
                    bubble->setScale(0.1);
                    bubble->runAction(Sequence::create(Spawn::create(FadeIn::create(1),
                                                                     ScaleTo::create(3, 1), NULL),
                                                       CallFunc::create([=](){
                        if (index%2==0) {
                            bubble->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 0.9),
                                                                                     ScaleTo::create(1, 1),
                                                                                     DelayTime::create(1), NULL)));
                        }else{
                            bubble->runAction(Sequence::create(DelayTime::create(5),
                                                               FadeOut::create(0.5), NULL));
                        }
                    }),
                                                       NULL));
                }),
                                                  AnimationHelp::createAnimate(localPath("cheese"), 0, 1,false,true,1.5),
                                                  FadeTo::create(2, 100), NULL));
            }
        }else if (name == "nachos") {
            Sprite* pUnit = (Sprite*)pChild;
            if (pUnit) {
                float dt = 2+0.02*index;
                if (dt >4) {
                    dt = 2+0.4*(arc4random()%5);
                }
                int k = pUnit->getTag();
                pUnit->runAction(Sequence::create(DelayTime::create(dt+1),
                                                  CallFunc::create([=,&k](){
                    pUnit->setTexture(localPath("toast_nachos"+std::to_string(k%2)+".png"));
                }),
                                                  DelayTime::create(0.5),
                                                  FadeOut::create(1), NULL));
            }
        }else if (name == "tomoto") {
            Sprite* pUnit = (Sprite*)pChild;
            if (pUnit) {
                float dt = 2.5+0.02*index;
                if (dt >4) {
                    dt = 2.5+0.4*(arc4random()%5);
                }
                int k = pUnit->getTag();
                pUnit->runAction(Sequence::create(DelayTime::create(dt+1),
                                                  CallFunc::create([=,&k](){
                    pUnit->setTexture(localRootPath("0/toast_tomoto"+std::to_string(k%2)+".png"));
                }),NULL));
            }
        }else if (name == "onion") {
            Sprite* pUnit = (Sprite*)pChild;
            if (pUnit) {
                float dt = 2+0.02*index;
                if (dt >4) {
                    dt = 2+0.4*(arc4random()%5);
                }
                int k = pUnit->getTag();
                pUnit->runAction(Sequence::create(DelayTime::create(dt+1),
                                                  CallFunc::create([=,&k](){
                    pUnit->setTexture(localRootPath("0/toast_onion"+std::to_string(k%2)+".png"));
                }), NULL));
            }
        }else if (name == "pepper") {
            Sprite* pUnit = (Sprite*)pChild;
            if (pUnit) {
                float dt = 2.3+0.02*index;
                if (dt >4) {
                    dt = 2.3+0.4*(arc4random()%5);
                }
                int k = pUnit->getTag();
                pUnit->runAction(Sequence::create(DelayTime::create(dt+1),
                                                  CallFunc::create([=,&k](){
                    pUnit->setTexture(localRootPath("0/toast_pepper.png"));
                }), NULL));
            }
        }
    });
}

void NachoBakeScene::_showGlove()
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
        _saveNachos();
        m_pGlove->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pGlove->getPosition(), m_pOven->getPosition());
    }), NULL));
}

void NachoBakeScene::_saveNachos()
{    
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pFood) {
        Vec2 pos = m_pFood->getPosition();
        m_pFood->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        Vec2 posNachos = m_pNachos->getPosition();
        m_pNachos->setPosition(Vec2(mixtureSize.width/2+10, mixtureSize.height/2+10));
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pNachos->visit();
        m_pFood->visit();
        render->end();
        m_pFood->setPosition(pos);
        m_pNachos->setPosition(posNachos);
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"nachos.png", false);
    pImage->autorelease();
}
void NachoBakeScene::_finish()
{
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<NachoDecorateScene>();
    }), NULL));
}