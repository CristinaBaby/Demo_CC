
#include "AddJuiceScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"

AddJuiceScene::AddJuiceScene()
{
}

AddJuiceScene::~AddJuiceScene()
{
    
}
bool AddJuiceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg_2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
//    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
//    m_nIceCount = pCocosNode->getChildrenCount()-1;
    
//    this->runAction(Sequence::create(DelayTime::create(2),
//                                     CallFunc::create([=](){
//        
//        m_pCup = BlenderCup::create();
//        m_pCup->initWithIndex(0);
//        m_pCup->setWaterPercent(100);
//        m_pCup->setStateEmpty();
//        this->addChildToContentLayer(m_pCup);
//        CMVisibleRect::setPosition(m_pCup, 480, 300);
//        m_pCup->setWaterPercent(0);
//        
//        m_pCup->showLid(false);
//    }),
//                                     DelayTime::create(1),
//                                     CallFunc::create([=](){
//        _pourIce();
//    }),
//                                     DelayTime::create(4),
//                                     CallFunc::create([=](){
//        m_pCup->showLid(true,true);
//        m_pCup->addWater(5);
//    }),
//                                     DelayTime::create(4),
//                                     CallFunc::create([=](){
//        m_pCup->crushIce();
//    }),  NULL));
    
    _showMachines();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _showBlenderCup();
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    return true;
}

void AddJuiceScene::onEnter()
{
    ExtensionScene::onEnter();
    
    
    Scene* scene = this->getScene();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    scene->getPhysicsWorld()->step(1/120.0f);
}

void AddJuiceScene::update(float dt)
{
    for(int i=0;i<2;i++);
    getScene()->getPhysicsWorld()->step(1/120.0f);
    
}
void AddJuiceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void AddJuiceScene::onSwitchStateCallback(JuiceMachine* pMachine,bool on)
{
    int tag = pMachine->getTag();
    if (on) {
        m_pGuideLayer->removeGuide();
        if (pMachine->getChildByTag(TAG_REWARD_LOCK)) {
            RewardManager::getInstance()->showRewardAds("flavor", tag);
            pMachine->turnOff();
            return;
        }
        if (m_bReady) {
            m_pCup->resumeAddWater();
        }else{
            m_pMachineScrollView->setTouchEnabled(false);
            m_pCup->initWithIndex(m_nFlavorIndex);
            m_pCup->setStateEmpty();
            m_pCup->setWaterPercent(0);
            m_pCup->setWaterStreamWorldPoint(pMachine->getOutletWorldPosition());
            m_pCup->addWater(1);
            m_bReady = true;
            GameDataManager::getInstance()->m_nFlavorIndex = m_nFlavorIndex;
        }
    }else{
        if (pMachine->getChildByName("lock")) {
            return;
        }
        m_pCup->pauseAddWater();
        if (m_pCup->getWaterPercent()>=100) {
            if(!m_bFull){
                m_bFull = true;
                AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
                m_pGameUI->showNextLayout();
            }
        }
    }
}
#pragma mark - initData
void AddJuiceScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagCrushIce;
    m_nFlavorIndex = 0;
    m_bReady = false;
    m_bFull = false;
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void AddJuiceScene::_showBlenderCup()
{
    m_pCup = BlenderCup::create();
//    m_pCup->initWithIndex(0);
//    m_pCup->setWaterPercent(100);
    m_pCup->setStateEmpty();
    this->addChildToContentLayer(m_pCup);
    CMVisibleRect::setPosition(m_pCup, 350+visibleSize.width, 250);
    m_pCup->setWaterPercent(0);
    m_pCup->setLocalZOrder(2);
    
    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(m_nShapeIndex)+".csb");
    m_nIceCount = pCocosNode->getChildrenCount()-1;
    m_pCup->addIces(m_nShapeIndex,m_nIceCount*2);
    m_pCup->showLid(false,false);
    AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    m_pCup->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
}

void AddJuiceScene::_showMachines()
{
    Sprite* pBg = Sprite::create("content/make/machine/machine_1.png");
    this->addChildToContentLayer(pBg);
    CMVisibleRect::setPositionAdapted(pBg, 320, 480);
    
    m_pMachineScrollView = ExpandScrollView::create();
    m_pMachineScrollView->setContentSize(Size(visibleSize.width, 500));
    this->addChildToContentLayer(m_pMachineScrollView);
    CMVisibleRect::setPositionAdapted(m_pMachineScrollView, visibleSize.width, 510,kBorderLeft);
    m_pMachineScrollView->setClippingEnabled(false);
    m_pMachineScrollView->onExpandScrollViewCallback = CC_CALLBACK_2(AddJuiceScene::_onScrollCallback, this);
    m_pMachineScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pMachineScrollView->setScrollBarEnabled(false);
    m_pMachineScrollView->setInertiaScrollEnabled(false);
    m_pMachineScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                     CallFunc::create([=](){
        m_pMachineScrollView->scrollToRight(1, true);
    }),
                                                     DelayTime::create(1),
                                                     CallFunc::create([=](){
        m_pMachineScrollView->scrollToLeft(0.5, true);
    }),
                                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideTap(Vec2(visibleSize.width/2, m_pMachineScrollView->getPositionY()+150));
    }), NULL));
    int flavorCount = 15;
    for (int i = 0; i<flavorCount; i++) {
        JuiceMachine* pMachine = JuiceMachine::create();
        pMachine->setFlavor(i);
        pMachine->setTag(i);
        pMachine->setPosition(Vec2(visibleSize.width/2+337*i, 200));
        m_pMachineScrollView->getInnerContainer()->addChild(pMachine);
        pMachine->onSwitchStateCallback = CC_CALLBACK_2(AddJuiceScene::onSwitchStateCallback, this);
        m_MachineVector.push_back(pMachine);
        
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(1)){
            if (i%2==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("flavor", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        if (locked) {
            Sprite* pLock = Sprite::create(lockName);
            pMachine->addChild(pLock,6);
            pLock->setTag(TAG_REWARD_LOCK);
            pLock->setName("lock");
            LockUIManager::getInstance()->registerLock("flavor", i, pLock->getParent());
//            CMVisibleRect::setPositionAdapted(pLock, 167+30+205*i, 250-50);
            pLock->setPosition(Vec2(100, 0));
        }
        
    }
    AudioHelp::getInstance()->playEffect("vo_juice_flavors.mp3");
    m_pMachineScrollView->setInnerContainerSize(Size(visibleSize.width/2+337*flavorCount, 500));
}

void AddJuiceScene::_pourIce()
{
    for (int i = 0; i<8; i++) {
        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(510+i%2*80,450+i/2*100));
        this->runAction(Sequence::create(DelayTime::create(0.1+0.2*i),
                                         CallFunc::create([=](){
            m_pCup->addIce(0, pos);
        }),NULL));
    }
}


void AddJuiceScene::_onScrollCallback(ExpandScrollView* pScrollView,int type)
{
    if (ExpandScrollView::eBeginScroll==type) {
        m_pGuideLayer->removeGuide();
        
    }else if(ExpandScrollView::eEndedScroll==type){
        
        Vec2 beginPos = pScrollView->getTouchBeginPosition();
        Vec2 endPos = pScrollView->getTouchEndPosition();
        Vec2 delt = endPos-beginPos;
        
        JuiceMachine* pPreMachine = m_MachineVector.at(m_nFlavorIndex);
        pPreMachine->setSwitchEnable(false);
        
        if (delt.x<-600) {
            m_nFlavorIndex+=2;
        }else if (delt.x<-20) {
            m_nFlavorIndex++;
        }else if (delt.x>20) {
            m_nFlavorIndex--;
        }else if (delt.x>600) {
            m_nFlavorIndex-=2;
        }else{
            
        }
        if(m_nFlavorIndex<0){
            m_nFlavorIndex = 0;
        }else if (m_nFlavorIndex>=15) {
            m_nFlavorIndex = 14;
        }
        JuiceMachine* pCurMachine = m_MachineVector.at(m_nFlavorIndex);
        pCurMachine->setSwitchEnable();
        m_pGuideLayer->showGuideTap(Vec2(visibleSize.width/2, m_pMachineScrollView->getPositionY()+150));
        
//        pScrollView->getInnerContainer()->setPosition(Vec2(-337*m_nFlavorIndex, 0));
        pScrollView->getInnerContainer()->runAction(MoveTo::create(0.3, Vec2(-337*m_nFlavorIndex, 0)));
        
    }
}

