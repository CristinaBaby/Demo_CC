
#include "NachoDisplayIngredientScene.h"
#include "ContainerNode.h"

static std::string sgIngredient[][2] = {
    {"bowl_b","bowl_beans1"},
    {"bowl_c","bowl_cheese1"},
    {"bowl_n","bowl_nachos1"},
    {"bowl_o","bowl_onion1"},
    {"bowl_p","bowl_pepper1"},
    {"bowl_t","bowl_tomato1"},
    {"cream0",""},
    {"salsa0",""},
//    {"",""},
};
NachoDisplayIngredientScene::NachoDisplayIngredientScene()
{
    
}

NachoDisplayIngredientScene::~NachoDisplayIngredientScene()
{
    
}
bool NachoDisplayIngredientScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/nachos_bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
    MenuLayer* pMenuLayer = MenuLayer::create();
    pMenuLayer->showMenu(GameDataManager::getInstance()->m_nPackage);
    this->addChildToUILayer(pMenuLayer);
    pMenuLayer->setLocalZOrder(100);
    
    int IngredientCount = 8;
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page0.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_recipe_page1.mp3");
        
        Node* pCocosLayer = CSLoader::createNode("NachoDisplayLayer.csb");
        
        for (int i = 0; i<IngredientCount; i++) {
            Node* pIngredient = Node::create();
            this->addChildToContentLayer(pIngredient);
            ContainerNode* pContainer = ContainerNode::create(pIngredient);
            pContainer->addBody(localPath(sgIngredient[i][0]+".png"));
            if (sgIngredient[i][1] != "") {
                pContainer->addFood(localPath(sgIngredient[i][1]+".png"));
            }
            CMVisibleRect::setPositionAdapted(pIngredient, pCocosLayer->getChildByName(sgIngredient[i][0])->getPosition()+Vec2(0, visibleSize.height));
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
    m_nNextSceneTag = GameUIEvent::eSceneTagNachoAddIngredient;
    GameUIEvent::getInstance()->nextSceneTag = m_nNextSceneTag;
    
    m_pGameUI->showNormalLayout();
    return true;
}

void NachoDisplayIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_NACHOS);
}

#pragma mark - initData
void NachoDisplayIngredientScene::_initData()
{
    setExPath("content/make/nachos/0/");
}