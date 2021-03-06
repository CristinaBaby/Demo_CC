
#include "BaconHamDisplayIngredientScene.h"
#include "SceneManager.h"

static std::string gsIngredients[] {
    "ham.png","bacon.png",
    "butter.png",
};
BaconHamDisplayIngredientScene::BaconHamDisplayIngredientScene()
{
    
}

BaconHamDisplayIngredientScene::~BaconHamDisplayIngredientScene()
{
    
}
bool BaconHamDisplayIngredientScene::init()
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
    
    int total = 3;
    //    total = 1;
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        Sprite* pFood =Sprite::create(localPath("food_2.png"));
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
                SceneManager::replaceTheScene<BaconHamFryBaconScene>();
            }
        }), NULL));
    }
    return true;
}

void BaconHamDisplayIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_BACON);
}

void BaconHamDisplayIngredientScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BaconHamDisplayIngredientScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void BaconHamDisplayIngredientScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BaconHamDisplayIngredientScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void BaconHamDisplayIngredientScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void BaconHamDisplayIngredientScene::_initData()
{
    setExPath("content/make/common/menu/");
}