
#include "PizzaDisplayIngredientScene.h"
#include "ContainerNode.h"

static std::string sgIngredient[] = {
    "water0",
    "flour0",
    "yeast",
    "oil",
    "measuring_cup0",
    "salt",
    
};
PizzaDisplayIngredientScene::PizzaDisplayIngredientScene()
{
    
}

PizzaDisplayIngredientScene::~PizzaDisplayIngredientScene()
{
    
}
bool PizzaDisplayIngredientScene::init()
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
    
    Sprite* pTable = Sprite::create(localPath("bk1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
    this->addChildToBGLayer(pTable);
    
    int IngredientCount = 6;
    
    MenuLayer* pMenuLayer = MenuLayer::create();
    pMenuLayer->showMenu(GameDataManager::getInstance()->m_nPackage);
    this->addChildToUILayer(pMenuLayer);
    pMenuLayer->setLocalZOrder(100);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page0.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page1.mp3");
        
        Node* pCocosLayer = CSLoader::createNode("PizzaDisplayLayer.csb");
        
        for (int i = 0; i<IngredientCount; i++) {
            Sprite* pIngredient = Sprite::create(localPath(sgIngredient[i]+".png"));
            this->addChildToContentLayer(pIngredient);
            //        pIngredient->setPosition(pCocosLayer->getChildByName(sgIngredient[i])->getPosition()+Vec2(0, visibleSize.height));
            CMVisibleRect::setPositionAdapted(pIngredient, pCocosLayer->getChildByName(sgIngredient[i])->getPosition()+Vec2(0, visibleSize.height));
            pIngredient->runAction(Sequence::create(DelayTime::create(0.5+0.6*i),
                                                    EaseSineOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                    CallFunc::create([=](){
                m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu)->runAction(Sequence::create(ScaleTo::create(0.3, 1.2),
                                                                                                      ScaleTo::create(0.2, 1), NULL));
            }), NULL));
        }
    }),
                                     DelayTime::create(3+IngredientCount*0.6),
                                     CallFunc::create([=](){
        
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
    m_nNextSceneTag = GameUIEvent::eSceneTagPizzaMix;
    GameUIEvent::getInstance()->nextSceneTag = m_nNextSceneTag;
    
    m_pGameUI->showNormalLayout();
    return true;
}

void PizzaDisplayIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_PIZZA);
}

#pragma mark - initData
void PizzaDisplayIngredientScene::_initData()
{
    setExPath("content/make/pizza/0/");
}