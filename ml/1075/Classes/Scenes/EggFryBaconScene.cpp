
#include "EggFryBaconScene.h"
#include "SceneManager.h"

EggFryBaconScene::EggFryBaconScene()
{
    
}

EggFryBaconScene::~EggFryBaconScene()
{
    
}
bool EggFryBaconScene::init()
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
    
    m_pGameUI->showNormalLayout();
    return true;
}

void EggFryBaconScene::onEnter()
{
    ExtensionScene::onEnter();
}

void EggFryBaconScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void EggFryBaconScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void EggFryBaconScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void EggFryBaconScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void EggFryBaconScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void EggFryBaconScene::_initData()
{
    setExPath("content/make/egg/make3/");
}