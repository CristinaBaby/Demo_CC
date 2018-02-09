
#include "OmeletteDisplayIngredientScene.h"
#include "SceneManager.h"

static std::string gsIngredients[] {
    "egg_1.png","salt.png",
    "pepper.png","butter.png",
    "oil.png"
};
OmeletteDisplayIngredientScene::OmeletteDisplayIngredientScene()
{
    
}

OmeletteDisplayIngredientScene::~OmeletteDisplayIngredientScene()
{
    
}
bool OmeletteDisplayIngredientScene::init()
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
    
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChildToUILayer(color);
    
    int total = 5;
    //    total = 1;
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        Sprite* pFood =Sprite::create(localPath("food_4.png"));
        this->addChildToUILayer(pFood);
        pFood->setPosition(visibleSize*0.5);
        pFood->setVisible(false);
        pFood->setScale(0.2);
        pFood->runAction(Sequence::create(DelayTime::create(0.5),
                                          Show::create(),
                                          ScaleTo::create(0.5,1),
                                          ActionHelper::getJellyAction(), NULL));
        AudioHelp::getInstance()->playEffect("magic_effect.mp3");
        
        Sprite* pLight =Sprite::create(localPath("fall_light.png"));
        this->addChildToUILayer(pLight);
        pLight->setLocalZOrder(1);
        CMVisibleRect::setPositionAdapted(pLight, 320, 800);
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/light.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(1);
        CMVisibleRect::setPositionAdapted(pParticle, 320, 900);
        AudioHelp::getInstance()->playEffect("vo_all_ingredients.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_all_ingredients2.mp3");
    }), NULL));
    
    for (int i = 0; i<total; i++) {
        Sprite* pItem =Sprite::create(localPath(gsIngredients[i]));
        this->addChildToUILayer(pItem);
        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320, 750-270));
        CMVisibleRect::setPositionAdapted(pItem, 320-visibleSize.width, 750);
        pItem->runAction(Sequence::create(DelayTime::create(1+0.8*i),
                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CircleMoveLinear::create(total-0.8*i, 270, pos, (-360-90+i*360/total)*M_PI/180),
                                          DelayTime::create(1),
                                          CallFunc::create([=](){
            if (i==total-1) {
                AudioHelp::getInstance()->playEffect("magic_effect.mp3");
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setLocalZOrder(10);
                pParticle->setPosition(visibleSize*0.5);
                
                AudioHelp::getInstance()->playEffect("magic_effect.mp3");
            }
        }),
                                          DelayTime::create(1),
                                          CallFunc::create([=](){
            if (i==total-1) {
                SceneManager::replaceTheScene<OmeletteMixScene>();
            }
        }), NULL));
    }
    return true;
}

void OmeletteDisplayIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_OMELETTE);
}

void OmeletteDisplayIngredientScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void OmeletteDisplayIngredientScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void OmeletteDisplayIngredientScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void OmeletteDisplayIngredientScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void OmeletteDisplayIngredientScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void OmeletteDisplayIngredientScene::_initData()
{
    setExPath("content/make/common/menu/");
}