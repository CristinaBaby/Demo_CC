
#include "ClassicRollScene.h"
#include "SceneManager.h"

static std::string gsFlavors[] = {
    "vanilla",
    "chocolate",
    "mint",
    "strawberry",
};
static std::string gsModels[] = {
    "round",
    "heart",
    "cat",
    "star",
    "ginger",
    "tree",
};
ClassicRollScene::ClassicRollScene()
{
    m_nRollCount = 0;
    m_nCutCount = 0;
    m_pCutTool = nullptr;
    m_pShapeBtn = nullptr;
}

ClassicRollScene::~ClassicRollScene()
{
    
}
bool ClassicRollScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _createRollUI();
    _showRollTool();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void ClassicRollScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ClassicRollScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ClassicRollScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if(name == "rolltool"){
        Rect rect = m_pDough->getBoundingBox();
        if (rect.containsPoint(worldPoint)){
            auto action = ActionHelper::createRollJelly();
            action->setTag(100);
            m_pDough->runAction(action);
        }        
    }
}

void ClassicRollScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if(name == "rolltool"){
        Rect rect = m_pDough->getBoundingBox();
        if (rect.containsPoint(worldPoint)) {
            if(m_nRollCount ==0){
                m_pRollTool->runAction(RotateBy::create(0.3, 20));
            }
            auto action = m_pDough->getActionByTag(100);
            if (!action) {
                auto action = ActionHelper::createRollJelly();
                action->setTag(100);
                m_pDough->runAction(action);
            }
            m_nRollCount++;
            AudioHelp::getInstance()->playLoopEffect("roll_dough.mp3");
            if (m_nRollCount%50 == 0) {
//                m_pDough->runAction(ActionHelper::createRollScaleAction());
                
                m_pGuideLayer->removeGuide();
            }
            if (m_nRollCount%100 == 0){
                int step = m_nRollCount/100;
                if(step<8){
                    m_pDough->setTexture(localPath(gsFlavors[m_nFlavorIndex]+"/"+std::to_string(step)+".png"));
                }else{
                    m_pDough->stopAllActions();
                    m_pDough->setScale(1);
                    _finishRoll();
                }
                
            }
        }
    }else if (name == "cuttool"){
        if ( pDragNode->index == 0) {
            for (int i = m_ModelPosVector.size()-1; i>=0; i--) {
                Vec2 pos = m_ModelPosVector.at(i);
                Rect cookie = Rect(pos.x-115, pos.y-100, 230, 200);
                Rect rect = pDragNode->getDragSprite()->getBoundingBox();
                rect.origin = pDragNode->convertToWorldSpace(rect.origin);
                if (cookie.intersectsRect(rect)) {
                    pDragNode->setTouchEnabled(false);
                    pDragNode->index = 1;
                    m_ModelPosVector.erase(m_ModelPosVector.begin()+i);
                    m_pGuideLayer->removeGuide();
                    if (m_ModelVector.size() == 0) {
                        if(m_pShapeBtn){
                            m_pShapeBtn->setTouchEnabled(false);
                            m_pShapeBtn->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
                        }
                    }
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("cut.mp3");
                    }),
                                                          ScaleTo::create(0.25,0.9),
//                                                          DelayTime::create(0.1),
//                                                          ScaleTo::create(0.15,1.0),
                                                          DelayTime::create(1.0),
                                                          CallFunc::create([=](){
                        
                        auto pCookie = Sprite::create(localPath(gsModels[m_nModuleIndex]+"/"+std::to_string(m_nFlavorIndex+1)+".png"));
                        pCookie->setPosition(pos);
                        this->addChildToContentLayer(pCookie);
                        m_ModelVector.push_back(pCookie);
                        
                        auto chooseParticle = ParticleSystemQuad::create("particle/fallStarParticle.plist");
                        chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                        chooseParticle->setVisible(true);
                        chooseParticle->setPosition(pos);
                        chooseParticle->setScale(1.0);
                        this->addChildToContentLayer(chooseParticle);
                        
                        AudioHelp::getInstance()->playEffect("all_star.mp3");
                        
                    }),
                                                          ScaleTo::create(0.25,1.2),
                                                          DelayTime::create(1.0),
                                                          CallFunc::create([=](){
                        pDragNode->index = 0;
                        pDragNode->back(0,[=](){
                            pDragNode->setTouchEnabled(true);
                        });
                        pDragNode->runAction(ScaleTo::create(0.5, 1));
                        if (m_ModelPosVector.size()==0) {
                            _finishCut();
                            pDragNode->setTouchEnabled(false);
                            
                        }
                    }),
                                                          ScaleTo::create(0.5,1),
                                                          NULL));
                    
                    break;
                }
            }
        }
    }
}

void ClassicRollScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if(name == "rolltool"){        
        m_pDough->stopAllActions();
        m_pDough->setScale(1);
    }else if (name == "cuttool"){
        if (pDragNode->index == 0) {
            pDragNode->setTouchEnabled(false);
            pDragNode->back(0,[=](){
                pDragNode->setTouchEnabled(true);
            });
        }else {
            pDragNode->index = 0;
        }
    }
}

void ClassicRollScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
    AudioHelp::getInstance()->stopLoopEffect();
}

#pragma mark - initData
void ClassicRollScene::_initData()
{
    setExPath("content/make/classic/make2/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    m_nNextSceneTag = GameUIEvent::eSceneTagClassicBall;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagClassicBall;
}

#pragma mark ==== roll UI =====
void ClassicRollScene::_createRollUI()
{
    m_pCloth = Sprite::create(localPath("bg.png"));
    this->addChildToContentLayer(m_pCloth);
    CMVisibleRect::setPositionAdapted(m_pCloth, 320, 480);
    ActionHelper::showBezier(m_pCloth, m_pCloth->getPosition(), ActionHelper::ShowDirection::show_from_right,0.5,[=](){
    });
    
    m_pDough = Sprite::create(localPath(gsFlavors[m_nFlavorIndex]+"/0.png"));
    this->addChildToContentLayer(m_pDough);
    CMVisibleRect::setPositionAdapted(m_pDough, 320, 480);
    
    ActionHelper::showBezier(m_pDough, m_pDough->getPosition(), ActionHelper::ShowDirection::show_from_left,1,[=](){
    });
}

void ClassicRollScene::_showRollTool()
{
//    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pRollTool = _createDrageNode(localPath("rolling.png"));
    this->addChildToContentLayer(m_pRollTool);
    m_pRollTool->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pRollTool, 520+visibleSize.width, 750);
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->setName("rolltool");
    m_pRollTool->runAction(Sequence::create(DelayTime::create(1),
                                            EaseElasticOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),1.0),
                                            CallFunc::create([=](){
        m_pGuideLayer->showGuideArrow(m_pDough->getPosition(), 3);
        m_pRollTool->setOrgPositionDefault();
        m_pRollTool->setTouchEnabled(true);
    }), NULL));
}

void ClassicRollScene::_removeRollTool()
{
    AudioHelp::getInstance()->stopLoopEffect();
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->runAction(EaseElasticIn::create(MoveBy::create(1.0,Vec2(visibleSize.width*1.5,0)),1.0));
                           
}

void ClassicRollScene::_finishRoll()
{
    setExPath("content/make/classic/make3/");
    for (int i = 0; i<4; i++) {
        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(185+290*(i%2), 370+250*(i/2)));
        m_ModelPosVector.push_back(pos);
    }
    m_pCloth->setTexture(localPath("bg2.png"));
    AudioHelp::getInstance()->stopLoopEffect();
    _removeRollTool();
    _showChooseModelLayer();
    
//    m_pShapeBtn = Button::create();
//    this->addChildToUILayer(m_pShapeBtn);
//    m_pShapeBtn->loadTextures("content/common/button/btn_shapes.png", "content/common/button/btn_shapes.png", "content/common/button/btn_shapes.png");
//    CMVisibleRect::setPositionAdapted(m_pShapeBtn, 540+visibleSize.width, 800,kBorderNone,kBorderTop);
//    m_pShapeBtn->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
//                                            ActionHelper::getJellyAction(), NULL));
//    m_pShapeBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
//        if (type!=Widget::TouchEventType::ENDED) {
//            return;
//        }
//        _showChooseModelLayer();
//    });
}

void ClassicRollScene::_finishCut()
{
    m_pGameUI->hideReset();
    m_pDough->runAction(Sequence::create(FadeOut::create(0.5),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
        _finish();
    }), NULL));
    
    for(auto ref:m_ModelVector){
        ref->runAction(FadeOut::create(1.0));
        Sprite* pCookie = Sprite::create(localPath(gsModels[m_nModuleIndex]+"/"+std::to_string(m_nFlavorIndex+1)+"_1.png"));
        this->addChildToContentLayer(pCookie);
        pCookie->setPosition(ref->getPosition());
        pCookie->setLocalZOrder(5);
        
        Vec2 pos = Vec2(0, ref->getPositionY()<visibleSize.height/2?50:0);
        
        pCookie->runAction(Sequence::create(FadeIn::create(0.5),
                                            DelayTime::create(1),
                                            JumpBy::create(1, Vec2::ZERO+pos, 150+pos.y, 1), NULL));
    }
    m_pCloth->runAction(Sequence::create(DelayTime::create(1),
                                         EaseElasticIn::create(MoveBy::create(1, Vec2(1000, 0)), 1), NULL));
    m_pCutTool->runAction(Sequence::create(DelayTime::create(1),
                                           EaseElasticIn::create(MoveBy::create(1,Vec2(1000,0)),1.0), NULL));
    
    Sprite* pTray = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(pTray);
    CMVisibleRect::setPositionAdapted(pTray, 320, 500);
    ActionHelper::showBezier(pTray, pTray->getPosition(), ActionHelper::ShowDirection::show_from_left,1.5,[=](){
    });
    
}

void ClassicRollScene::_showChooseModelLayer()
{
    ChooseModelLayer* pLayer = ChooseModelLayer::create();
    this->addChildToUILayer(pLayer);
    pLayer->setLocalZOrder(2);
    pLayer->showChooseModel();
    pLayer->onItemCallback = CC_CALLBACK_1(ClassicRollScene::_onModelCallback, this);
    AudioHelp::getInstance()->playEffect("vo_choose_shapes.mp3");
}

void ClassicRollScene::_onModelCallback(int tag)
{
    GameDataManager::getInstance()->m_nShapeIndex = tag;
    _showCutTool(tag);
}

void ClassicRollScene::_showCutTool(int index)
{
    std::string path = localPath(gsModels[index]+"/"+gsModels[index]+".png");
    m_nModuleIndex = index;
    if (m_pCutTool) {
        m_pCutTool->setTouchEnabled(false);
        m_pCutTool->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.6, Vec2(visibleSize.width, 0)), 1),
                                               CallFunc::create([=](){
            m_pCutTool->setDragSprite(Sprite::create(path));
        }),
                                               EaseElasticOut::create(MoveBy::create(0.6, Vec2(-visibleSize.width, 0)), 1),
                                               ActionHelper::getJellyAction(),
                                               CallFunc::create([=](){
            m_pCutTool->setOrgPositionDefault();
            m_pCutTool->setTouchEnabled(true);
        }), NULL));
    }else{
        AudioHelp::getInstance()->playEffect("vo_drag_cut4cookies.mp3");
        m_pCutTool = _createDrageNode(path);
        this->addChildToContentLayer(m_pCutTool);
        CMVisibleRect::setPositionAdapted(m_pCutTool, 380+visibleSize.width, 860);
        m_pCutTool->setTouchEnabled(false);
        m_pCutTool->setName("cuttool");
        m_pCutTool->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),1),
                                               ActionHelper::getJellyAction(),
                                               CallFunc::create([=](){
            m_pGuideLayer->showGuideMove(m_pCutTool->getPosition(), m_pDough->getPosition());
            m_pCutTool->setOrgPositionDefault();
            m_pCutTool->setTouchEnabled(true);
        }), NULL));
    }
    m_pCutTool->setTag(index);
}

void ClassicRollScene::_finish()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    static std::string voName[] = {
        "vo_wonderful.mp3",
        "vo_well_done.mp3",
        "vo_fantastic.mp3",
        "vo_done.mp3",
        "vo_fantastic.mp3",
        "vo_great.mp3",
    };
    AudioHelp::getInstance()->playEffect(voName[arc4random()%6]);
    
    Sprite* pAnimation = Sprite::create("content/make/common/animation/donhua_1.png");
    this->addChildToUILayer(pAnimation);
    pAnimation->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pAnimation, 320, 0,kBorderNone,kBorderBottom);
    pAnimation->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/common/animation/donhua_", 1, 8),
                                           CallFunc::create([=](){
        pAnimation->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->playEffect("success.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ClassicBakeScene>();
    }), NULL));
}