
#include "SnowconeColorScene.h"

SnowconeColorScene::SnowconeColorScene()
{
    
}

SnowconeColorScene::~SnowconeColorScene()
{
    
}
bool SnowconeColorScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg/bg_2.png"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    return true;
}

void SnowconeColorScene::onEnter()
{
    ExtensionScene::onEnter();
}

void SnowconeColorScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeColorScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void SnowconeColorScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void SnowconeColorScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void SnowconeColorScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void SnowconeColorScene::_initData()
{
    setExPath("content/start/");
}