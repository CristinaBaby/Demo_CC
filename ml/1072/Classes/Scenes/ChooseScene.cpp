
#include "ChooseScene.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"

ChooseScene::ChooseScene()
{
    m_bSelected = false;
}

ChooseScene::~ChooseScene()
{
    
}
bool ChooseScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGameUI->showNormalLayout();
    m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagMenu);
    
    Node* pChooseNode = CSLoader::createNode("res/ChooseLayer.csb");
    ui::Helper::doLayout(pChooseNode);
    CMVisibleRect::setPositionAdapted(pChooseNode, 0, 0);
//    this->addChildToContentLayer(pChooseNode);
    
    for (int i = 0; i<5; i++) {
        Node* pCocosNode = pChooseNode->getChildByTag(i);
        Node* pNode = Node::create();
        this->addChildToContentLayer(pNode);
        CMVisibleRect::setPositionAdapted(pNode, pCocosNode->getPosition()+Vec2(0, visibleSize.height));
        
        Sprite* pShadow = Sprite::create(localPath("cart"+std::to_string(i)+"_0.png"));
        pNode->addChild(pShadow);
        pShadow->setOpacity(0);
        
        pNode->runAction(Sequence::create(DelayTime::create(0.5+0.5*i),
                                          MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                          ScaleTo::create(0.5, 1.1,0.7),
                                          DelayTime::create(0.1),
                                          EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                          CallFunc::create([=](){
            pShadow->runAction(FadeIn::create(0.5));
        }), NULL));
        
        Button* pButton = Button::create(localPath("cart"+std::to_string(i)+"_1.png"));
        if (pButton) {
            pButton->addTouchEventListener(CC_CALLBACK_2(ChooseScene::onButtonCallback, this));
            pButton->setTag(i);
            pNode->addChild(pButton);
        }
        
        Sprite* pLock = Sprite::create("content/common/video.png");
        pNode->addChild(pLock,10,TAG_REWARD_LOCK);
        m_LockVector.push_back(pLock);
        
        if (ConfigManager::getInstance()->getVideoUnLocked("pack", i)){
            pLock->setVisible(false);
        }else{
            if (!ConfigManager::getInstance()->getVideoUnLocked("pack", i)) {
                LockUIManager::getInstance()->registerLock("pack", i, pNode);
            }
        }
        
        int total = 1;
        if (i==1) {
            total = 2;
        }
        for (int t = 0; t<total; t++) {
            Sprite* pSprite = Sprite::create(localPath(std::to_string(i)+"_"+std::to_string(t)+".png"));
            pNode->addChild(pSprite);
            pSprite->setPosition(Vec2(60+5*i-20*t, 80+5*i));
            if (i==1){
                pSprite->setPosition(Vec2(40-20*t, 80));
            }
            if (i==2) {
                pSprite->setLocalZOrder(-1);
                pSprite->setPosition(Vec2(-20, 130));
                pSprite->runAction(RepeatForever::create(Sequence::create(Spawn::create(RotateBy::create(2, 360),
                                                                                        JumpBy::create(1.5, Vec2::ZERO, 80, 2), NULL), NULL)));
            }else{
                pSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1+i*0.2),
                                                                          Spawn::create(RotateBy::create(1.5+i*0.2+t*0.3, 360),
                                                                                        JumpBy::create(1.5, Vec2::ZERO, 80+i*2, 3), NULL), NULL)));
            }
        }
    }
    Vector<Node*> pChildren = pChooseNode->getChildren();
    for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
        std::string name = pNode->getName();
        int k = pNode->getTag();
        Node* pBtn = pNode->getChildByName("button");
        Button* pButton = dynamic_cast<Button*>(pBtn);
        if (pButton) {
            pButton->addTouchEventListener(CC_CALLBACK_2(ChooseScene::onButtonCallback, this));
            pButton->setTag(k);
        }
        
        this->runAction(Sequence::create(DelayTime::create(0.1+k*0.3),
                                         CallFunc::create([=](){
            auto action = CSLoader::createTimeline(name+"Node.csb", pNode);
            action->play("fall", false);
            pNode->runAction(action);
            if (pButton) {
                pButton->setTouchEnabled(true);
            }
            
        }),
                                         DelayTime::create(3),
                                         CallFunc::create([=](){
            auto action = CSLoader::createTimeline(name+"Node.csb", pNode);
            action->play("animation0", true);
            pNode->runAction(action);
            if (name=="Fries" || name=="Burger") {
                action->setTimeSpeed(0.4);
            }
            
        }), NULL));
    });
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_page0.mp3");
        
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_page1.mp3");
        
    }),NULL));
    AudioHelp::getInstance()->playBackGroundMusic("bgm1.mp3");
    return true;
}

void ChooseScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ChooseScene::onButtonCallback(Ref *ref,Widget::TouchEventType type)
{
    if (type!=Widget::TouchEventType::ENDED) {
        return;
    }
    if(m_bSelected){
        return;
    }
    Button* pBtn = (Button*)ref;
    int tag = pBtn->getTag();
    std::string name = "pack"+std::to_string(tag);
    if(!ConfigManager::getInstance()->getVideoUnLocked("pack", tag)){
        ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
        this->addChildToUILayer(pLayer);
        pLayer->setLocalZOrder(100);
        pLayer->shopGetFree = [=](){
            RewardManager::getInstance()->showRewardAds("pack", tag);
        };
        return;
    }
    m_bSelected = true;
    GameDataManager::getInstance()->m_nPackage = tag;
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        
    }
    Node* pNode = pBtn->getParent();
    if (pNode) {
        pNode->runAction(Repeat::create(Sequence::create(ScaleTo::create(1, 1.2),
                                                         ScaleTo::create(1, 1), NULL), 4));
    }
    GameDataManager::getInstance()->m_nPackage = tag;
    MenuLayer::reset();
    AudioHelp::getInstance()->playBackGroundMusic("bgm_make.mp3");
    switch (tag) {
        case 0://hotdog
        {
            AudioHelp::getInstance()->playEffect("vo_choose_hotdog.mp3");
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<HotdogDisplayIngredientScene>();
            }), NULL));
        }
            break;
        case 1://nachos
        {
            AudioHelp::getInstance()->playEffect("vo_choose_nachos.mp3");
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<NachoDisplayIngredientScene>();
            }), NULL));
        }
            break;
        case 2://pizza
        {
            AudioHelp::getInstance()->playEffect("vo_choose_pizza.mp3");
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<PizzaDisplayIngredientScene>();
            }), NULL));
        }
            break;
        case 3://burger
        {
            AudioHelp::getInstance()->playEffect("vo_choose_burger.mp3");
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<BurgerDisplayIngredientScene>();
            }), NULL));
        }
            break;
        case 4://fries
        {
            AudioHelp::getInstance()->playEffect("vo_choose_fries.mp3");
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<FriesDisplayIngredientScene>();
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}

void ChooseScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ChooseScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void ChooseScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void ChooseScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void ChooseScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ChooseScene::_initData()
{
    setExPath("content/choose/");
}