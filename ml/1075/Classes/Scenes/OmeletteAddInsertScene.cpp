
#include "OmeletteAddInsertScene.h"
#include "SceneManager.h"

OmeletteAddInsertScene::OmeletteAddInsertScene()
{
    m_bCanTouch = false;
    m_pFoodRender = nullptr;
    m_bButtonEable = true;
}

OmeletteAddInsertScene::~OmeletteAddInsertScene()
{
    
}
bool OmeletteAddInsertScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showOmelette();
    _showDecorations();
    m_pGameUI->showNormalLayout();
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(OmeletteAddInsertScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(OmeletteAddInsertScene::TouchMoved, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(OmeletteAddInsertScene::TouchEnded, this);
    
    return true;
}

void OmeletteAddInsertScene::onEnter()
{
    ExtensionScene::onEnter();
}

void OmeletteAddInsertScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    if (!m_bButtonEable) {
        return;
    }
    
    m_bButtonEable = false;
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        m_bButtonEable = true;
    }), NULL));
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        GameDataManager::getInstance()->m_VectorCandle.clear();
        _saveFood();
        btn->runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
        SceneManager::pushTheScene<DecorateScene>();
    }else if (GameUILayoutLayer::eUIButtonTagReset==tag){
        btn->setEnabled(false);
        _reset();
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
    }
}

void OmeletteAddInsertScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->reloadData();
}

void OmeletteAddInsertScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void OmeletteAddInsertScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void OmeletteAddInsertScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void OmeletteAddInsertScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void OmeletteAddInsertScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void OmeletteAddInsertScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    
}
void OmeletteAddInsertScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    
    std::stringstream ostr;
    ostr<<"1/"<<m_nIndex<<".png";
    Sprite* unit = Sprite::create(localPath(ostr.str()));
    int angle = 20*(arc4random()%10);
    unit->setRotation(angle);
    
    int alpha = 0;
    if (m_pFoodRender) {
        Vec2 posDelt = Vec2::ZERO;
        alpha = m_pFoodRender->getAlphaWithLocation(m_pOmelette->convertToNodeSpace(pos)+posDelt);
    }
    if(alpha>0){
        unit->setPosition(m_pDecNode->convertToNodeSpace(pos));
        m_pDecNode->addChild(unit);
        m_pGuideLayer->removeGuide();
    }else{
        unit->setPosition(m_pContentLayer->convertToNodeSpace(pos));
        this->addChildToContentLayer(unit);
        unit->setLocalZOrder(20);
        unit->runAction(Sequence::create(Spawn::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                                       RotateBy::create(1, 360), NULL),
                                         CallFunc::create([=](){
            unit->removeFromParent();
        }), NULL));
    }
}

void OmeletteAddInsertScene::onDecorationCallback(int index,int type,DecorateConfigData decData)
{
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(100);
                    pParticle->setPosition(m_pDecorationScrollView->getWorldItemPos());
                    m_pDecorationScrollView->unlcok(index);
                };
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
                m_pDecorationScrollView->setSelected(false);
                pLayer->removeFromParent();
            };
            
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    AudioHelp::getInstance()->_playEffectLaugh();
    m_nIndex = index;
    m_bCanTouch = true;
    if (!m_bGuide) {
        m_bGuide = true;
        m_pGuideLayer->showGuideTap(m_pOmelette->getParent()->convertToWorldSpace(m_pOmelette->getPosition()));
//        AudioHelp::getInstance()->playEffect("vo_decorate_fruits.mp3");
    }
}
#pragma mark - initData
void OmeletteAddInsertScene::_initData()
{
    setExPath("content/make/omelette/make3/");
}

void OmeletteAddInsertScene::_showOmelette()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 400);
    
    m_pOmelette = Sprite::create(localPath("omelet1.png"));
    this->addChildToContentLayer(m_pOmelette);
    CMVisibleRect::setPositionAdapted(m_pOmelette, 320+visibleSize.width, 400);
    
    m_pPlate->runAction(Sequence::create(DelayTime::create(1),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    m_pOmelette->runAction(Sequence::create(DelayTime::create(1),
                                            MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        _getFoodRenderData();
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout();
    }), NULL));
    
    m_pDecNode = Node::create();
    this->addChildToContentLayer(m_pDecNode);
    m_pDecNode->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pDecNode, 320, 400);
    m_FoodSize = m_pOmelette->getContentSize();
}

void OmeletteAddInsertScene::_showDecorations()
{
    m_pDecorationScrollView = ItemScrollView::createWithSize(Size(590,150),false);
    this->addChildToUILayer(m_pDecorationScrollView);
    
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile("omelette");
    m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(OmeletteAddInsertScene::onDecorationCallback, this);
    
    m_pDecorationScrollView->setLocalZOrder(1);
    m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
    m_pDecorationScrollView->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  80,kBorderNone,kBorderBottom);
    //        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
    
    m_pDecorationScrollView->btnPathEX = localPath("1/icon/");
    m_pDecorationScrollView->bgHighlightPath = "content/category/icon/fram_1.png";
    m_pDecorationScrollView->boxPathEX = "content/category/icon/frame.png";
    //    m_pDecorationScrollView->bgPath = "content/category/icon/kk0.png";
    m_pDecorationScrollView->setMargin(40);
    m_pDecorationScrollView->setLocalZOrder(20);
    
    m_pDecorationScrollView->reloadData();
    AudioHelp::getInstance()->playEffect("bar_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_veggies_meats_fruits.mp3");
}

void OmeletteAddInsertScene::_getFoodRenderData()
{
    Size foodSize;
    Node* pFoodMask = Sprite::create(localPath("mask.png"));
    foodSize = pFoodMask->getContentSize();
    
    m_pFoodRender = MyRenderTexture::create(foodSize.width, foodSize.height,Texture2D::PixelFormat::RGBA8888,
                                            GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 pos = Vec2(foodSize.width/2,foodSize.height/2);
    Vec2 posFood = pFoodMask->getPosition();
    
    pFoodMask->setPosition(pos);
    
    m_pFoodRender->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    pFoodMask->visit();
    m_pFoodRender->end();
    
    Director::getInstance()->getRenderer()->render();
    pFoodMask->setPosition(posFood);
    
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_FOREGROUND);
    m_pFoodRender->retain();
    
}

void OmeletteAddInsertScene::_saveFood()
{
    float scale = 1.2;
    Size foodSize = Size(m_FoodSize.width,m_FoodSize.height)*scale;
    RenderTexture* render = RenderTexture::create(foodSize.width, foodSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecNode->getPosition();
    
    Vec2 pos = Vec2(foodSize.width/2,foodSize.height/2);
    
    m_pDecNode->setPosition(pos);
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pDecNode->visit();
    render->end();
    
    m_pDecNode->setPosition(posOrigin);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ "omelette_dec.png", false);
    pImage->autorelease();
}


void OmeletteAddInsertScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    m_bCanTouch = false;
    
    m_pDecNode->removeAllChildren();
}
