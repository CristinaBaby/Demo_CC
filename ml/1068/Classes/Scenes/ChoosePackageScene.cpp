
#include "ChoosePackageScene.h"
#include "SceneManager.h"

ChoosePackageScene::ChoosePackageScene()
{
    
}

ChoosePackageScene::~ChoosePackageScene()
{
    
}
bool ChoosePackageScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderLeft,kBorderBottom);
    this->addChildToUILayer(m_pScrollView);
    m_pScrollView->setClippingEnabled(false);
    m_pScrollView->setTouchEnabled(true);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::BOTH);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setPosition(Vec2::ZERO);
    
    Node* pIcecream = CSLoader::createNode("IcecreamNode.csb");
    pIcecream->setPosition(Vec2(310+visibleSize.width*1.5, 440));
    Button* pIcecreamButton = dynamic_cast<Button*>(pIcecream->getChildByName("Button"));
    pIcecreamButton->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onPackageCallback, this));
    pIcecreamButton->setTag(1);
    pIcecreamButton->setSwallowTouches(false);
    Node* pIcecreamWheel0 = pIcecream->getChildByName("car1_wheels1");
    Node* pIcecreamWheel1 = pIcecream->getChildByName("car1_wheels2");
    pIcecreamWheel0->runAction(RepeatForever::create(RotateBy::create(0.3, -360)));
    pIcecreamWheel1->runAction(RepeatForever::create(RotateBy::create(0.1, -360)));
    pIcecream->runAction(Sequence::create(DelayTime::create(0.5),
                                          MoveBy::create(3, Vec2(-visibleSize.width*1.5, 0)),
                                          CallFunc::create([=](){
        pIcecreamWheel0->stopAllActions();
        pIcecreamWheel1->stopAllActions();
    }), NULL));
    
    Node* pSnowcone = CSLoader::createNode("SnowconeNode.csb");
    pSnowcone->setPosition(Vec2(670+visibleSize.width, 780));
    Button* pSnowconeButton = dynamic_cast<Button*>(pSnowcone->getChildByName("Button"));
    pSnowconeButton->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onPackageCallback, this));
    pSnowconeButton->setTag(0);
    pSnowconeButton->setSwallowTouches(false);
    Node* pSnowconeWheel0 = pSnowcone->getChildByName("car2_wheels1");
    Node* pSnowconeWheel1 = pSnowcone->getChildByName("car2_wheels2");
    pSnowconeWheel0->runAction(RepeatForever::create(RotateBy::create(0.3, -360)));
    pSnowconeWheel1->runAction(RepeatForever::create(RotateBy::create(0.1, -360)));
    pSnowcone->runAction(Sequence::create(DelayTime::create(0.5),
                                          MoveBy::create(2, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        pSnowconeWheel0->stopAllActions();
        pSnowconeWheel1->stopAllActions();
    }), NULL));
    
    Sprite* pBg = Sprite::create(localPath("bg_beach.png"));
    m_pScrollView->addChild(pBg);
    pBg->setAnchorPoint(Vec2::ZERO);

    NodeGrid* pGrid = NodeGrid::create();
    pGrid->setPosition(Vec2(-5,0));
    m_pScrollView->addChild(pGrid);
    
    Sprite* pWave = Sprite::create(localPath("bg_sea wave.png"));
    pGrid->addChild(pWave);
    pWave->setAnchorPoint(Vec2::ZERO);
    pWave->setPosition(Vec2::ZERO);
    
    pGrid->runAction(RepeatForever::create(Waves::create(3, Size(10, 10), 1, 5, true, true)));
    
    Sprite* pDec = Sprite::create(localPath("bg_decoration1.png"));
    m_pScrollView->addChild(pDec);
    pDec->setAnchorPoint(Vec2::ZERO);
    
    Sprite* pDec2 = Sprite::create(localPath("bg_decoration2.png"));
    m_pScrollView->addChild(pDec2);
    pDec2->setAnchorPoint(Vec2::ZERO);
    
    m_pScrollView->getInnerContainer()->addChild(pIcecream,1);
    m_pScrollView->getInnerContainer()->addChild(pSnowcone);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        m_pScrollView->scrollToPercentHorizontal(50, 0.5, true);
//        m_pScrollView->scrollToPercentVertical(50, 0.5, true);
    }), NULL));
    
    m_pScrollView->getInnerContainer()->setContentSize(pBg->getContentSize());
    return true;
}


void ChoosePackageScene::onPackageCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        if (m_pScrollView->isScrolled()) {
            return;
        }
        AudioHelp::getInstance()->playSelectedEffect();
        Button* pButton = (Button*)ref;
        int tag = pButton->getTag();
        GameDataManager::getInstance()->m_nPackage = tag;
        if (tag==0) {
            SceneManager::replaceTheScene<MakeSnowconeScene>();
        }else if (tag==1) {
            SceneManager::replaceTheScene<MakeIcecreamScene>();
        }
    }
}
#pragma mark - initData
void ChoosePackageScene::_initData()
{
    
    setExPath("content/make/choose/");
}