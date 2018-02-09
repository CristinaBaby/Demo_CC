
#include "ExtensionScene.h"
//#include "AdsManager.h"
#include "SceneManager.h"

ExtensionScene::ExtensionScene()
{
    m_pGuideLayer = nullptr;
    m_bShowAds = false;
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ExtensionScene::onShopItemBuy), ItemBuySuccessNotify, NULL);
}

ExtensionScene::~ExtensionScene()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

bool ExtensionScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_pBGLayer = Layer::create();
    m_pBGLayer->setZOrder(0);
    this->addChild(m_pBGLayer);
    
    m_pContentLayer = Layer::create();
    m_pContentLayer->setZOrder(100);
    this->addChild(m_pContentLayer);
    
    m_pUILayer = Layer::create();
    m_pUILayer->setZOrder(200);
    this->addChild(m_pUILayer);
    
    m_pGameUI = GameUILayoutLayer::create();
    this->addChildToUILayer(m_pGameUI);
    m_pGameUI->setLocalZOrder(10);
    m_pGameUI->onButtonClicked = CC_CALLBACK_1(ExtensionScene::onButtonCallback, this);
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        m_bShowAds = true;
    }
//    m_pGameUI->showNextLayout();
    return true;
}

void ExtensionScene::onEnter()
{
    Layer::onEnter();
    
    
    if (m_bShowAds && !UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->showAds(kTypeBannerAds);
    }
    KeypadDispatchCenter::getInstance()->addDelegate(this);
    
}

void ExtensionScene::onExit()
{
    KeypadDispatchCenter::getInstance()->removeDelegate(this);
    Layer::onExit();
}


void ExtensionScene::onButtonCallback(Button* btn)
{
    GameUIEvent::getInstance()->nextSceneTag = m_nNextSceneTag;
    GameUIEvent::getInstance()->onButtonCallback(btn->getTag());
//    int tag = btn->getTag();
    
}


// 添加一个Node到背景层
void ExtensionScene::addChildToBGLayer(Node* child)
{
    m_pBGLayer->addChild(child);
}

// 添加一个Node到UI层
void ExtensionScene::addChildToUILayer(Node* child)
{
    m_pUILayer->addChild(child);
}

// 添加一个Node到内容层
void ExtensionScene::addChildToContentLayer(Node* child)
{
    m_pContentLayer->addChild(child);
}



DragNode* ExtensionScene::_createDrageNode(std::string path,bool enable)
{
    DragNode* pTool = DragNode::create(path);
    pTool->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
    pTool->setTouchEnabled(enable);
    pTool->dragNodeTouchBegin = CC_CALLBACK_2(ExtensionScene::dragNodeTouchBegin, this);
    pTool->dragNodeTouchMoved = CC_CALLBACK_2(ExtensionScene::dragNodeTouchMoved, this);
    pTool->dragNodeTouchEnded = CC_CALLBACK_2(ExtensionScene::dragNodeTouchEnded, this);
    pTool->dragNodeClicked = CC_CALLBACK_2(ExtensionScene::dragNodeClicked, this);
    return pTool;
    
}

void ExtensionScene::onKeyBackClicked() {
    Node* pDialog = Director::getInstance()->getRunningScene()->getChildByTag(9999);
    if (pDialog) {
        pDialog->removeFromParent();
        return;
    }
    Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog->setContentText("Are you sure you want to leave this page?");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
        AudioHelp::getInstance()->stopLoopEffect();
//        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
//            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//            AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                SceneManager::popToRootScene();
//                SceneManager::replaceTheScene<HomeScene>();
//            };
//        }else{
        
//        AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//        AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//            SceneManager::popToRootScene();
//            SceneManager::replaceTheScene<HomeScene>();
//        };
        
        AdsManager::getInstance()->showAds(kTypeInterstitialAds);
        SceneManager::popToRootScene();
        SceneManager::replaceTheScene<HomeScene>();
//        }
    };
}