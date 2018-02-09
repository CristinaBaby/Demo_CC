
#include "SnowconeShapScene.h"
#include "SceneManager.h"

SnowconeShapScene::SnowconeShapScene()
{
    m_bSelected = false;
}

SnowconeShapScene::~SnowconeShapScene()
{
    
}
bool SnowconeShapScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showShapeScrollView();
    m_pGameUI->showNormalLayout();
    return true;
}

void SnowconeShapScene::onEnter()
{
    ExtensionScene::onEnter();
}

void SnowconeShapScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeShapScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void SnowconeShapScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void SnowconeShapScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void SnowconeShapScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void SnowconeShapScene::_initData()
{
    setExPath("content/make/snowcone/3/");
}

void SnowconeShapScene::_showShapeScrollView()
{
//    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile("shape");
//    
//    m_pShapeScroll = ExpandScrollView::create();
//    m_pShapeScroll->setContentSize(Size(visibleSize.width, 500));
//    this->addChildToUILayer(m_pShapeScroll);
//    m_pShapeScroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
//    m_pShapeScroll->setScrollBarEnabled(false);
//    
//    
//    for (int i = 0; i<data.totalCount; i++) {
//        
//    }
    
    m_pItemScrollView = ItemScrollView::createWithSize(Size(visibleSize.width,600),false);
    this->addChildToUILayer(m_pItemScrollView);
    
    m_pItemScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile("shape");
    m_pItemScrollView->onItemCellSelected = CC_CALLBACK_3(SnowconeShapScene::_onItemSelected, this);
    
    m_pItemScrollView->setLocalZOrder(1);
    m_pItemScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
    m_pItemScrollView->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pItemScrollView,0+visibleSize.width,  350,kBorderLeft);
    m_pItemScrollView->setIAPPack(2);
    m_pItemScrollView->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_snowcone_choose_shape.mp3");
        
    }),
                                                  EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width,0))), NULL));
    
    m_pItemScrollView->btnPathEX = localPath("icon_shape");
//    m_pItemScrollView->bgHighlightPath = "content/category/icon/c.png";
//    m_pItemScrollView->boxPathEX = "content/category/icon/frame.png";
//    m_pItemScrollView->bgPath = "content/category/icon/box2_1.png";
    m_pItemScrollView->setMargin(50);
    m_pItemScrollView->reloadData();
}

void SnowconeShapScene::_onItemSelected(int index,int type,DecorateConfigData decData)
{
    if (m_bSelected) {
        return;
    }
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
                m_pItemScrollView->setSelected(false);
                pLayer->removeFromParent();
            };
            
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    m_bSelected = true;
    GameDataManager::getInstance()->m_nShapeIndex = index;
    m_pItemScrollView->setVisible(false);
    
    Vec2 pos = m_pItemScrollView->getWorldItemPos();
    
    Sprite* pSprite = Sprite::create(localPath("icon_shape"+std::to_string(index)+".png"));
    this->addChildToUILayer(pSprite);
    pSprite->setPosition(pos);
    pSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(visibleSize.width/2, visibleSize.height/2)),
                                                      ScaleTo::create(0.5, 1.5), NULL),
                                        DelayTime::create(1),
                                        CallFunc::create([=](){
        SceneManager::replaceTheScene<SnowconeSculptScene>();
    }), NULL));
}