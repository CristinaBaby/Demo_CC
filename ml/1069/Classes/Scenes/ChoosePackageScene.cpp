
#include "ChoosePackageScene.h"
#include "SceneManager.h"

ChoosePackageScene::ChoosePackageScene()
{
    m_bSelected = false;
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
    
    Node* pChooseNode = CSLoader::createNode("res/ChooseNode.csb");
    ui::Helper::doLayout(pChooseNode);
    
    Node* pccGummy = pChooseNode->getChildByName("gummy");
    Node* pccGummyDoor = pChooseNode->getChildByName("gummy_door");
    Node* pccNormal = pChooseNode->getChildByName("normal");
    Node* pccNormalDoor = pChooseNode->getChildByName("normal_door");
    Node* pccBox = pChooseNode->getChildByName("box");
    Node* pccTable = pChooseNode->getChildByName("table");
    Node* pccArrowGummy = pChooseNode->getChildByName("arrow_gummy");
    Node* pccArrowNormal = pChooseNode->getChildByName("arrow_nomal");
    
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderLeft,kBorderBottom);
    this->addChildToUILayer(m_pScrollView);
    m_pScrollView->setClippingEnabled(false);
    m_pScrollView->setTouchEnabled(true);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::BOTH);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setPosition(Vec2::ZERO);
    
    Sprite* pBg = Sprite::create(localPath("choose_bg.jpg"));
    m_pScrollView->addChild(pBg);
    pBg->setPosition(pBg->getContentSize()*0.5);
    m_pBg = pBg;

    Vec2 pos = pBg->getContentSize()*0.5;
    
    m_pGummyClipping = ClippingNode::create(Sprite::create(localPath("gummy_mask.png")));
    pBg->addChild(m_pGummyClipping);
    m_pGummyClipping->setPosition(pccGummyDoor->getPosition()+pos);
    m_pGummyClipping->setAlphaThreshold(0.5);
    
    m_pGummyDoor = Sprite::create(localPath("gummy_door.png"));
    m_pGummyClipping->addChild(m_pGummyDoor);
    m_pGummyDoor->setAnchorPoint(Vec2(0.375,0));
    m_pGummyDoor->setPosition(Vec2(-(0.5-0.375)*m_pGummyDoor->getContentSize().width, -m_pGummyDoor->getContentSize().height/2));
    
    LayerColor* pGummyColor = LayerColor::create(cocos2d::Color4B::WHITE);
    pGummyColor->setPosition(Vec2(-m_pGummyDoor->getContentSize().width/2, -m_pGummyDoor->getContentSize().height/2));
    m_pGummyClipping->addChild(pGummyColor,-1);
    
    LockItem* pGummyItem = LockItem::create();
    pGummyItem->initWithName(localPath("gummy_doorframe.png"), "",false);
    pBg->addChild(pGummyItem);
    pGummyItem->setPosition(m_pGummyClipping->getPosition());
    pGummyItem->setTag(1);
    pGummyItem->onItemClicked = CC_CALLBACK_1(ChoosePackageScene::onPackageCallback, this);
    
    m_pNormalClipping = ClippingNode::create(Sprite::create(localPath("normal_mask.png")));
    pBg->addChild(m_pNormalClipping);
    m_pNormalClipping->setPosition(pccNormalDoor->getPosition()+pos);
    m_pNormalClipping->setAlphaThreshold(0.5);
    
    m_pNormalDoor = Sprite::create(localPath("normal_door.png"));
    m_pNormalClipping->addChild(m_pNormalDoor);
    m_pNormalDoor->setAnchorPoint(Vec2(0.63,0));
    m_pNormalDoor->setPosition(Vec2((0.63-0.5)*m_pNormalDoor->getContentSize().width, -m_pNormalDoor->getContentSize().height/2));
    
    LayerColor* pNormalColor = LayerColor::create(cocos2d::Color4B::WHITE);
    pNormalColor->setPosition(Vec2(-m_pNormalDoor->getContentSize().width/2, -m_pNormalDoor->getContentSize().height/2));
    m_pNormalClipping->addChild(pNormalColor,-1);
    
    LockItem* pNormalItem = LockItem::create();
    pNormalItem->initWithName(localPath("normal_doorframe.png"), "",false);
    pBg->addChild(pNormalItem);
    pNormalItem->setPosition(m_pNormalClipping->getPosition());
    pNormalItem->setTag(0);
    pNormalItem->onItemClicked = CC_CALLBACK_1(ChoosePackageScene::onPackageCallback, this);
    
    
    m_pGirl = RoleModel::create();
    m_pGirl->setRole(localCocosPath("cooker0/"), "cooker0", 3);
    pBg->addChild(m_pGirl);
    m_pGirl->setScale(0.4);
    CMVisibleRect::setPositionAdapted(m_pGirl, 480+230, 0);
    m_pGirl->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 350)),
                                        CallFunc::create([=](){
        m_pGirl->playArmation(0, false);
    }), NULL));
    m_pGirl->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "Animation1") ){
                m_pGirl->playArmation(1, true);
            }
        }
    };
    Button* pArrowGummy = Button::create(localPath("arrow_gummy.png"),localPath("arrow_gummy.png"));
    pBg->addChild(pArrowGummy);
    pArrowGummy->setPosition(pccArrowGummy->getPosition()+pos);
    pArrowGummy->setTag(1);
    pArrowGummy->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    
    Button* pArrowNormal = Button::create(localPath("arrow_nomal.png"),localPath("arrow_nomal.png"));
    pBg->addChild(pArrowNormal);
    pArrowNormal->setPosition(pccArrowNormal->getPosition()+pos);
    pArrowNormal->setTag(0);
    pArrowNormal->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    pBg->addChild(pTable);
    pTable->setPosition(pccTable->getPosition()+pos);
    
    Button* pDec = Button::create(localPath("gummy.png"),localPath("gummy.png"));
    pBg->addChild(pDec);
    pDec->setPosition(pccGummy->getPosition()+pos);
    pDec->setTag(1);
    pDec->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    
    Button* pDec2 = Button::create(localPath("normal.png"),localPath("normal.png"));
    pBg->addChild(pDec2);
    pDec2->setPosition(pccNormal->getPosition()+pos);
    pDec2->setTag(0);
    pDec2->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    
    Sprite* pBox = Sprite::create(localPath("box.png"));
    pBg->addChild(pBox);
    pBox->setPosition(pccBox->getPosition()+pos);
    
    pArrowGummy->runAction(RepeatForever::create(Sequence::create(EaseSineOut::create(MoveBy::create(1, Vec2(-50, 0))),
                                                                  MoveBy::create(0.3, Vec2(20, 0)),
                                                                  MoveBy::create(0.3, Vec2(-20, 0)),
                                                                  MoveBy::create(1, Vec2(50, 0)),
                                                                  DelayTime::create(1), NULL)));
    
    pArrowNormal->runAction(RepeatForever::create(Sequence::create(EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                                   MoveBy::create(0.3, Vec2(-20, 0)),
                                                                   MoveBy::create(0.3, Vec2(20, 0)),
                                                                   MoveBy::create(1, Vec2(-50, 0)),
                                                                   DelayTime::create(1), NULL)));
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        m_pScrollView->scrollToPercentHorizontal(50, 0.5, true);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        
        pDec->runAction(RepeatForever::create(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 20, 1),
                                                                             ScaleTo::create(0.8, 1,0.95), NULL),
                                                               ScaleTo::create(0.3, 1),
                                                               DelayTime::create(5),
                                                               NULL)));
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        
        pDec2->runAction(RepeatForever::create(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 20, 1),
                                                                              ScaleTo::create(0.8, 1,0.95), NULL),
                                                                ScaleTo::create(0.3, 1),
                                                                DelayTime::create(5),
                                                                NULL)));
    }), NULL));
    
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->getInnerContainer()->setContentSize(pBg->getContentSize());
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playEffect("vo_choose_pizza.mp3");
    return true;
}


void ChoosePackageScene::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        if (m_pScrollView->isScrolled()) {
            return;
        }
        if(m_bSelected){
            return;
        }
        m_bSelected = true;
        Button* pButton = (Button*)ref;
        int tag = pButton->getTag();
        onSelectCallback(tag);
    }
    
}
void ChoosePackageScene::onPackageCallback(LockItem* item)
{
    if (m_pScrollView->isScrolled()) {
        return;
    }
    if(m_bSelected){
        return;
    }
    int tag = item->getTag();
    onSelectCallback(tag);
}
void ChoosePackageScene::onSelectCallback(int index)
{
    m_bSelected = true;
    AudioHelp::getInstance()->playEffect("selected_stars.mp3");
    AudioHelp::getInstance()->playEffect("vo_nice_choice.mp3");
    int tag = index;
    GameDataManager::getInstance()->m_nPackage = tag;
    if (tag==0) {
        m_pScrollView->scrollToRight(0.3, true);
        m_pNormalDoor->runAction(Sequence::create(RotateBy::create(1, Vec3(0, -130, 10)),
                                                  CallFunc::create([=](){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/door.plist");
            m_pBg->addChild(pParticle);
            pParticle->setPosition(m_pNormalClipping->getPosition());
        }),
                                                  DelayTime::create(1),
                                                  CallFunc::create([=](){
            SceneManager::replaceTheScene<MakeNormalPizzaScene>();
        }),  NULL));
    }else if (tag==1) {
        m_pScrollView->scrollToLeft(0.3, true);
        m_pGummyDoor->runAction(Sequence::create(RotateBy::create(1, Vec3(0, 130, -10)),
                                                 CallFunc::create([=](){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/door.plist");
            m_pBg->addChild(pParticle);
            pParticle->setPosition(m_pGummyClipping->getPosition());
        }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
            SceneManager::replaceTheScene<MakeGummyPizzaScene>();
        }),  NULL));
    }
}
#pragma mark - initData
void ChoosePackageScene::_initData()
{
    
    setExPath("content/choose/");
    setExCocosPath("res/");
}