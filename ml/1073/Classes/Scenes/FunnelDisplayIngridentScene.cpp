
#include "FunnelDisplayIngridentScene.h"

FunnelDisplayIngridentScene::FunnelDisplayIngridentScene()
{
    
}

FunnelDisplayIngridentScene::~FunnelDisplayIngridentScene()
{
    
}
bool FunnelDisplayIngridentScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    MenuLayer* pMenuLayer = MenuLayer::create();
    pMenuLayer->showMenu(GameDataManager::getInstance()->m_nPackage);
    this->addChildToUILayer(pMenuLayer);
    pMenuLayer->setLocalZOrder(100);
    
    
    Node* pCocosLayer = CSLoader::createNode("FunnelDisplayILayer.csb");
    pCocosLayer->retain();
    
    int IngredientCount = pCocosLayer->getChildrenCount();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page0.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page1.mp3");
        Vector<Node*> pChildrenVector = pCocosLayer->getChildren();
        int i = 0;
        for_each(pChildrenVector.begin(), pChildrenVector.end(), [=,&i](Node* pNode){
            std::string name = pNode->getName();
            
            Sprite* pIngredient = Sprite::create(localPath(name+".png"));
            this->addChildToContentLayer(pIngredient);
            CMVisibleRect::setPositionAdapted(pIngredient, pNode->getPosition()+Vec2(0, visibleSize.height));
            pIngredient->runAction(Sequence::create(DelayTime::create(0.5+0.6*i),
                                                    EaseSineOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                    CallFunc::create([=](){
                m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu)->runAction(Sequence::create(ScaleTo::create(0.3, 1.2),
                                                                                                      ScaleTo::create(0.2, 1), NULL));
            }), NULL));
            i++;
        });
        
    }),
                                     DelayTime::create(3+IngredientCount*0.6),
                                     CallFunc::create([=](){
        
        pCocosLayer->release();
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }),
                                     DelayTime::create(0.5),
                                     CallFunc::create([=](){
        GameUIEvent::getInstance()->onButtonCallback(GameUILayoutLayer::eUIButtonTagNext);
    }), NULL));
    m_pGameUI->showMenuUILayout();
    
    m_nNextSceneTag = GameUIEvent::eSceneTagFunnelMix;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFunnelMix;
    
    m_pGameUI->showNormalLayout();
    
    return true;
}

void FunnelDisplayIngridentScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_FUNNELCAKE);
}

void FunnelDisplayIngridentScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelDisplayIngridentScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void FunnelDisplayIngridentScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void FunnelDisplayIngridentScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void FunnelDisplayIngridentScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FunnelDisplayIngridentScene::_initData()
{
    setExPath("content/make/funnel/0/");
}