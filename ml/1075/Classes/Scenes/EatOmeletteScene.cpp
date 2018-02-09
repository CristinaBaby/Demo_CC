
#include "EatOmeletteScene.h"
#include "SceneManager.h"

EatOmeletteScene::EatOmeletteScene()
{
    
}

EatOmeletteScene::~EatOmeletteScene()
{
    
}
bool EatOmeletteScene::init()
{
    if ( !ShareScene::init() )
    {
        return false;
    }
    _initData();
    
    m_pGameUI->hideBack();
    //    bg
    
//    AudioHelp::getInstance()->playEffect("vo_cant_wait_ry.mp3");
    return true;
}

void EatOmeletteScene::onEnter()
{
    ShareScene::onEnter();
}

void EatOmeletteScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


#pragma mark - initData
void EatOmeletteScene::_initData()
{
    setExPath("content/make/omelette/make3/");
}

void EatOmeletteScene::_showEatScene()
{
    m_bCanEat = true;
    m_pPlate = Sprite::create(localPath("plate.png"));
    m_pDecoLayer->addChild(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 450);
    
    {
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"omelette_dec.png");
        Sprite* pFood = Sprite::create(localPath("omelet2_on.png"));
        Sprite* pFoodDown = Sprite::create(localPath("omelet2_down.png"));
        
        ClippingNode* pDec = ClippingNode::create(Sprite::create(localPath("mask2.png")));
        pDec->setAlphaThreshold(0.5);
        Sprite* pFoodDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"omelette_dec.png");
        pFoodDec->setScale(0.9);
        pDec->addChild(pFoodDec);
        
        Size size = pFood->getContentSize();
        
        pFood->setPosition(Vec2(size.width/2, size.height/2));
        pFoodDown->setPosition(Vec2(size.width/2, size.height/2));
        pFoodDec->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height,Texture2D::PixelFormat::RGBA8888,
                                                      GL_DEPTH24_STENCIL8_OES);
        
        rt->beginWithClear(0, 0, 0, 0);
        pFoodDown->visit();
        pFoodDec->Node::visit();
        pFood->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        
        m_pEatLayerDown = MakeEatLayer::create();
        m_pEatLayerDown->setRenderTexture(rt);
        
        CMVisibleRect::setPositionAdapted(rt, 300, 450);
        //        CMVisibleRect::setPositionAdapted(rt, 320+pCake->getContentSize().width/2, 500+rt->getContentSize().height/2);
        //        CMVisibleRect::setPositionAdapted(m_pEatLayerDown, 320-pCake->getContentSize().width/2, 500-pCake->getContentSize().height/2);
        //        rt->setPosition(Vec2(-pCake->getContentSize().width/2, -pCake->getContentSize().height/2));
        m_pDecoLayer->addChild(m_pEatLayerDown);
    }
    
}