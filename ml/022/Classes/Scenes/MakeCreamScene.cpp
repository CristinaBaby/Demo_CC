
#include "MakeCreamScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"

MakeCreamScene::MakeCreamScene()
{
}

MakeCreamScene::~MakeCreamScene()
{
    
}
bool MakeCreamScene::init()
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
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_table2.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 400,kBorderNone,kBorderBottom);
    this->addChildToBGLayer(pTable);
    
    _showChooseBlenderUI();
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
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    return true;
}

void MakeCreamScene::onEnter()
{
    ExtensionScene::onEnter();
    
    
    Scene* scene = this->getScene();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    scene->getPhysicsWorld()->step(1/120.0f);
}

void MakeCreamScene::update(float dt)
{
    for(int i=0;i<2;i++);
    getScene()->getPhysicsWorld()->step(1/120.0f);
    
}
void MakeCreamScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

#pragma mark - initData
void MakeCreamScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagChooseCup;
}

void MakeCreamScene::_showChooseBlenderUI()
{
    AudioHelp::getInstance()->playEffect("vo_choose_blender.mp3");
    m_pBlenderLayer = ChooseLayer::create();
    this->addChildToUILayer(m_pBlenderLayer);
    m_pBlenderLayer->showChooseBlender();
    m_pBlenderLayer->onItemCallback = [=](int index){
        AudioHelp::getInstance()->playEffect("vo_greate_choice.mp3");
        
        int flavor = GameDataManager::getInstance()->m_nFlavorIndex;
        m_pBlender = BlenderNode::create(index);
        this->addChildToContentLayer(m_pBlender);
        CMVisibleRect::setPosition(m_pBlender, 280+visibleSize.width, 400);
        m_pCup = m_pBlender->getCup();
        m_pCup->initWithIndex(flavor);
        m_pCup->setWaterPercent(100);
        if(0==index){
            m_pCup->onFinishCrushIce = CC_CALLBACK_0(MakeCreamScene::_finish, this);
        }else{
            m_pBlender->onBlenderSwitchTurnOff = CC_CALLBACK_0(MakeCreamScene::_finish, this);
            m_pBlender->onBlenderStopWorking = [=](){
                Rect rect = m_pBlender->getButtonWorldRect();
                if (index==0) {
                }else{
                    m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
                }
            };
        }
        m_pCup->onBeginCrushIce = CC_CALLBACK_0(MakeCreamScene::_beginCrush, this);
        
        int shapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
        Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(shapeIndex)+".csb");
        int iceCount = pCocosNode->getChildrenCount()-1;
        m_pCup->addIces(shapeIndex,iceCount*2);
        m_pCup->showLid(true,false);
        
        m_pBlender->runAction(Sequence::create(DelayTime::create(0.5),
                                           CallFunc::create([=]{
            AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
        }),
                                           MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=]{
            Rect rect = m_pBlender->getButtonWorldRect();
            if (index==0) {
                m_pGuideLayer->showGuideArrow(Vec2(rect.getMidX(), rect.getMidY()),1);
//                m_pGuideLayer->showGuideMove(Vec2(rect.getMaxX(), rect.getMaxY()), Vec2(rect.getMinX(), rect.getMinY()));
            }else{
                m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
            }
        }),
                                               DelayTime::create(0.2),
                                               CallFunc::create([=]{
            m_pBlender->setReadyWork();
        }), NULL));
    };
}


void MakeCreamScene::_beginCrush()
{
    m_pGuideLayer->removeGuide();
}

void MakeCreamScene::_finish()
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->playEffect("vo_goodjob.mp3");
    m_pGameUI->showNextLayout();
//    SceneManager::replaceTheScene<ChooseCupScene>();
}