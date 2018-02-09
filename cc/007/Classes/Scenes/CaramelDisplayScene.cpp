
#include "CaramelDisplayScene.h"
#include "SceneManager.h"

CaramelDisplayScene::CaramelDisplayScene()
{
    
}

CaramelDisplayScene::~CaramelDisplayScene()
{
    
}
bool CaramelDisplayScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/classic/dec/bg.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        this->_showCookie();
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){        
        m_pGameUI->showSaleLayout();
        m_pGameUI->showEatLayout();
        AudioHelp::getInstance()->playEffect("vo_eat_serve.mp3");
    }), NULL));
    m_bButtonEable = true;
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
    return true;
}

void CaramelDisplayScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CaramelDisplayScene::onButtonCallback(Button* btn)
{
    if (!m_bButtonEable) {
        return;
    }
    m_bButtonEable = false;
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagSale==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                             m_bButtonEable = true;
                                                         }), NULL));
        SceneManager::pushTheScene<SaleScene>();
    }else if (GameUILayoutLayer::eUIButtonTagEat==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                             m_bButtonEable = true;
                                                         }), NULL));
        SceneManager::pushTheScene<ShareScene>();
    }else{
        ExtensionScene::onButtonCallback(btn);
        m_bButtonEable = true;
    }
}

#pragma mark - initData
void CaramelDisplayScene::_initData()
{
    setExPath("content/make/caramel/dec/");
}

void CaramelDisplayScene::_showCookie()
{
    Sprite* pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(pPlate);
    CMVisibleRect::setPositionAdapted(pPlate, 320, 440);
    
    Sprite* pCookie = Sprite::create(localPath("cookie1.png"));
    this->addChildToContentLayer(pCookie);
    CMVisibleRect::setPositionAdapted(pCookie, 320, 440);
    
    ActionHelper::showBezier(pPlate, pPlate->getPosition(), ActionHelper::ShowDirection::show_from_bottom);
    ActionHelper::showBezier(pCookie, pCookie->getPosition(), ActionHelper::ShowDirection::show_from_bottom);
}