
#include "EggFryScene.h"
#include "SceneManager.h"

EggFryScene::EggFryScene()
{
    m_nStep = 0;
    m_nEggCount = 0;
    m_nMoveCount = 0;
    m_pButter = nullptr;
    m_pPlate = nullptr;
    m_bEffectTurn = false;
    m_bEffectBaconOK = false;
}

EggFryScene::~EggFryScene()
{
    
}
bool EggFryScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showStove();
    _showPan();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_50gram_butter_pan.mp3");
        _showIngredient(0);
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void EggFryScene::onEnter()
{
    ExtensionScene::onEnter();
}

void EggFryScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void EggFryScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
    }else if (name == "model"){
        int tag = pDragNode->getTag();
        EggModel* pEggModel = m_pEggVector.at(tag);
        if (pEggModel) {
            pEggModel->setVisible(false);
            Sprite* pEgg = Sprite::create(localRootPath("make2/"+gEggShapeName[m_nShapeIndex]+".png"));
            pEgg->setPosition(pEggModel->getPosition()+Vec2(0, -30));
            pEgg->setName("fried_egg");
            if (m_nShapeIndex<4) {
                pEgg->setScale(0.5);
            }else{
                pEgg->setPosition(pEggModel->getPosition());
            }
            this->addChildToContentLayer(pEgg);
            m_pEggFriedVector.push_back(pEgg);
        }
        
        pDragNode->setVisible(true);
        
    }else if(name == "clip"){
        if (pDragNode->index == 0) {
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("clip1_down.png")));
            Sprite* pClipOn = Sprite::create(localPath("clip1_on.png"));
            pDragNode->addChild(pClipOn,20);
            pClipOn->setName("clip_on");
        }
    }
}

void EggFryScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "butter")
    {
        int index = pDragNode->index;
        Rect rect = m_pPan->getBoundingBox();
        if (index == 0) {
            if (rect.containsPoint(worldPoint)) {
                rect.origin = rect.origin+Vec2(100, 100);
                rect.size = Size(250,160);
                pDragNode->setLimitRect(rect);
                pDragNode->index = 1;
            }
        }else if (index == 1){
            pDragNode->setDragSprite(Sprite::create(localPath("pan_butter2.png")));
            AudioHelp::getInstance()->playLoopEffect("smear_cream.mp3");
            pDragNode->index = 2;
        }else if (index == 2){
            m_pButterScribble->paint(worldPoint+Vec2(-50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
            m_pButterScribble->paint(worldPoint+pDragNode->getOffset(),worldPoint);
            m_pButterScribble->paint(worldPoint+Vec2(50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
            Director::getInstance()->getRenderer()->render();
            AudioHelp::getInstance()->playLoopEffect("smear_cream.mp3");
            _paintButter();
        }
        
    }else if (name == "shovel"){
        int index = pDragNode->index;
        if (index==0) {
#pragma mark ===== 铲起煎蛋
            for (auto pEgg:m_pEggFriedVector) {
                Rect rect = pEgg->getBoundingBox();
                if (rect.containsPoint(worldPoint)) {
                    pEgg->retain();
                    pEgg->removeFromParent();
                    pDragNode->addChild(pEgg);
                    pEgg->release();
                    pEgg->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                    auto iter = std::find(m_pEggFriedVector.begin(), m_pEggFriedVector.end(), pEgg);
                    if (iter != m_pEggFriedVector.end())
                    {
                        m_pEggFriedVector.erase(iter);
                    }
                    pDragNode->index = 1;
                    m_pGuideLayer->removeGuide();
                    m_pGuideLayer->showGuideMove(worldPoint, m_pPlate->getPosition());
//                    AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                    break;
                }
            }
            if (m_pEggFriedVector.size()==0) {
                
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }else if (index==1){
            Rect rect = m_pPlate->getBoundingBox();
#pragma mark ===== 将煎蛋放入盘中
            if (rect.containsPoint(worldPoint)) {
                AudioHelp::getInstance()->playEffect("bread_fall.mp3");
                Node* pEgg = pDragNode->getChildByName("fried_egg");
                if (pEgg) {
                    pEgg->removeFromParent();
                }
                int count = m_pEggFriedVector.size();
                
                Sprite* pEggPlate = Sprite::create(localRootPath("make2/"+gEggShapeName[m_nShapeIndex]+".png"));
                m_pPlate->addChild(pEggPlate);
                if (m_nShapeIndex<4) {
                    pEggPlate->setScale(0.7);
                    pEggPlate->setPosition(m_pPlate->getAnchorPointInPoints()+Vec2(-100+200*count, 0));
                }else{
                    pEggPlate->setPosition(m_pPlate->getAnchorPointInPoints()+Vec2(0, 30));
                }
                pDragNode->index = 0;
                m_pGuideLayer->removeGuide();
                if (count == 0) {
                    //finish fry
                    m_pShovel->setTouchEnabled(false);
                    m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                    _finish();
                }
            }
        }
    }else if(name == "clip"){
        int index = pDragNode->index;
        if (index == 1) {
#pragma mark ===== 夹起生的bacon index = 1
            Rect rect = m_pBaconPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 2;
                AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip2_on.png"));
                }
                Sprite* pBacon = Sprite::create(localPath("bacon.png"));
                pDragNode->addChild(pBacon);
                pBacon->setName("bacon");
                pBacon->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                
                Sprite* pBaconInPlate = m_pBaconVector.at(0);
                auto iter = std::find(m_pBaconVector.begin(), m_pBaconVector.end(), pBaconInPlate);
                if (iter != m_pBaconVector.end())
                {
                    m_pBaconVector.erase(iter);
                }
                pBaconInPlate->setVisible(false);
                
                m_pGuideLayer->removeGuide();
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPanIn->getPosition());
            }
        }else if (index==2) {
#pragma mark ===== 将bacon放锅里煎 index = 2
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(false);
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPan->getPosition()+Vec2(0, 50)),
                                                      CallFunc::create([=](){
                    auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                    if (!pMilkDrop) {
                        auto pParticle = ParticleSystemQuad::create("particle/smoke1.plist");
                        this->addChildToContentLayer(pParticle);
                        pParticle->setPosition(m_pPanIn->getPosition());
                        pParticle->setName("smoke");
                    }
                    Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                    if (pOn) {
                        pOn->setTexture(localPath("clip1_on.png"));
                    }
                    Sprite* pBacon = (Sprite*)pDragNode->getChildByName("bacon");
                    if (pBacon) {
                        pBacon->removeFromParent();
                    }
                    pDragNode->setTouchEnabled(true);
                    int size = m_pBaconVector.size();
                    FriedUnit* pFriedBacon = FriedUnit::create();
                    pFriedBacon->setFood(localPath("bacon.png"));
                    pFriedBacon->setOil(localPath("bacon5_"+std::to_string(size+1)+"_1.png"));
                    pFriedBacon->addParticle("particle/fry_bubble1.plist");
                    pFriedBacon->addParticle("particle/fry_bubble2.plist");
                    pFriedBacon->setTag(m_pBaconVector.size());
                    this->addChildToContentLayer(pFriedBacon);
                    pFriedBacon->setPosition(m_pPanIn->getPosition()+Vec2(0, -50+100*(1-size)));
                    log("=======%f,%f",pFriedBacon->getPositionX(),pFriedBacon->getPositionY());
                    m_pBaconFriedVector.push_back(pFriedBacon);
                    
                    _doFryBacon(pFriedBacon);
                    
                    if (m_pBaconVector.size()==1) {
                        m_pPlate->runAction(Sequence::create(DelayTime::create(1),
                                                             MoveBy::create(1, Vec2(-visibleSize.width+100, 0)), NULL));
                    }
                    m_pGuideLayer->removeGuide();
                    if (m_pBaconVector.size()==0) {
                        m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pClip->setTouchEnabled(false);
                        m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)));
                    }
                }), NULL));
            }
            
        }else if (index==3) {
#pragma mark ===== 夹起煎好的bacon index = 3
            for (auto pFriedBacon:m_pBaconFriedVector) {
                if (pFriedBacon->flag==0) {
                    continue;
                }
                Rect rect = pFriedBacon->getRectWorld();
                if (rect.containsPoint(worldPoint)) {
                    if (pFriedBacon->flag == 1) {
                        pDragNode->index = 5;
                        
                    }else if(pFriedBacon->flag == 2) {
                        if (!pFriedBacon->isFriedOK) {
                            continue;
                        }
                        pDragNode->index = 4;
                    }
                    AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                    pDragNode->setTouchEnabled(false);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pFriedBacon->getPosition()+Vec2(100, 100)),
                                                          CallFunc::create([=](){
                        Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                        if (pOn) {
                            pOn->setTexture(localPath("clip2_on.png"));
                        }
                        Sprite* pBacon;
                        if (pFriedBacon->flag == 1) {
                            pDragNode->index = 5;
                            pBacon = Sprite::create(localPath("bacon2.png"));
                            
                        }else if(pFriedBacon->flag == 2) {
                            pDragNode->index = 4;
                            pBacon = Sprite::create(localPath("bacon5_"+std::to_string(pFriedBacon->getTag()+1)+".png"));
                            
                            auto iter = std::find(m_pBaconFriedVector.begin(), m_pBaconFriedVector.end(), pFriedBacon);
                            if (iter != m_pBaconFriedVector.end())
                            {
                                m_pBaconFriedVector.erase(iter);
                            }
                            m_pGuideLayer->removeGuide();
                            m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPanIn->getPosition());
                        }
                        pDragNode->addChild(pBacon);
                        pBacon->setName("bacon");
                        pBacon->setTag(pFriedBacon->getTag());
                        pBacon->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                        
                        pFriedBacon->stopFry();
                        if(m_pBaconFriedVector.size()>0){
                            AudioHelp::getInstance()->playLoopEffect("frying_food.mp3");
                        }
                        pFriedBacon->setVisible(false);
                        
                        pDragNode->setTouchEnabled(true);
                    }), NULL));
                    break;
                }
            }
            
        }else if (index==4) {
#pragma mark ===== 将煎好的bacon放盘子 index = 4
            Rect rect = m_pPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                Node* pBacon = pDragNode->getChildByName("bacon");
                int tag = pBacon->getTag();
                pBacon->removeFromParent();
                
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                int count = m_pBaconFriedVector.size();
                
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip1_on.png"));
                }
                
                Sprite* pBaconInPlate = Sprite::create(localPath("bacon5_"+std::to_string(tag+1)+".png"));
                m_pPlate->addChild(pBaconInPlate);
                
                pBaconInPlate->setPosition(m_pPlate->getAnchorPointInPoints()+Vec2(0, -50));
                pBaconInPlate->setRotation(-15+30*count);
                
                pDragNode->index = 3;
                m_pGuideLayer->removeGuide();
                AudioHelp::getInstance()->_playEffectNiceWork();
                if (count == 0) {
                    //finish fry bacon
                    m_pClip->setTouchEnabled(false);
                    m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
//                    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    {
                        auto pParticle = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                        if (pParticle) {
                            pParticle->stopSystem();
                        }
                    }
                    
                    AudioHelp::getInstance()->playEffect("done.mp3");
                    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(visibleSize*0.5);
                    this->runAction(Sequence::create(DelayTime::create(2),
                                                     CallFunc::create([=](){
                        SceneManager::replaceTheScene<EatEggScene>();
                    }), NULL));
                }else{
                    m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlate->getPosition());
                }
            }
        }
    }
}

void EggFryScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
//        AudioHelp::getInstance()->stopLoopEffect();
        Rect rectBowl = m_pPan->getBoundingBox();
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
            m_pGuideLayer->removeGuide();
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
        
    }else if (name == "model"){
        pDragNode->runAction(JumpBy::create(1, Vec2(visibleSize.width, 0), 200, 1));
        auto iter = std::find(m_pModelVector.begin(), m_pModelVector.end(), pDragNode);
        if (iter != m_pModelVector.end())
        {
            m_pModelVector.erase(iter);
        }
        AudioHelp::getInstance()->playEffect("soda_cup.mp3");
        if (m_pModelVector.size()==0) {
            auto pParticle = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
            if (pParticle) {
                pParticle->stopSystem();
                pParticle->removeFromParent();
            }
            m_pGuideLayer->removeGuide();
            _showShovel();
            _showPlate();
        }
    }else if(name == "clip"){
#pragma mark        bacon翻面 index = 5
        if (pDragNode->index==5) {
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPan->getPosition()+Vec2(0, 50)),
                                                  CallFunc::create([=](){
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip1_on.png"));
                }
                Sprite* pBacon = (Sprite*)pDragNode->getChildByName("bacon");
                for (auto pFriedBacon:m_pBaconFriedVector){
                    int tag = pFriedBacon->getTag();
                    if (tag == pBacon->getTag()) {
                        pFriedBacon->setVisible(true);
                        pFriedBacon->flag = 2;
                        pFriedBacon->getFood()->runAction(JumpBy::create(1, Vec2::ZERO, 50, 1));
                        pFriedBacon->getFood()->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0, 90, 0)),
                                                                CallFunc::create([=](){
                            pFriedBacon->getFood()->setTexture(localPath("bacon3.png"));
                        }),
                                                                           RotateBy::create(0.5, Vec3(0, 90, 0)),
                                                                           CallFunc::create([=](){
                            _doFryBacon(pFriedBacon);
                        }),
                                                                NULL));
                        break;
                    }
                }
                if (pBacon) {
                    pBacon->removeFromParent();
                }
                
                pDragNode->index = 3;
                pDragNode->setTouchEnabled(true);
                AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
            }), NULL));
        }
    }
}

void EggFryScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void EggFryScene::_initData()
{
    setExPath("content/make/egg/make1/");
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void EggFryScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 320);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    this->addChildToContentLayer(m_pFire);
    m_pFire->setPosition(m_pStove->getPosition()+Vec2(0,-10));
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}

void EggFryScene::_showPan()
{
    m_pPan = Sprite::create("content/make/common/stove/pan_down.png");
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320+150+visibleSize.width, 420);
    
    m_pPanFront = Sprite::create("content/make/common/stove/pan_on.png");
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setPosition(m_pPan->getPosition());
    m_pPanFront->setLocalZOrder(5);
    
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    m_pPanFront->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    
    m_pPanIn = Node::create();
    this->addChildToContentLayer(m_pPanIn);
    CMVisibleRect::setPositionAdapted(m_pPanIn, 320, 400);
    
    Sprite* pButter = Sprite::create(localPath("pan_butter0.png"));
    pButter->setPosition(pButter->getContentSize()*0.5);
    m_pButterScribble = ScribbleNode::create(pButter->getContentSize());
    m_pButterScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
    m_pPanIn->addChild(m_pButterScribble);
    m_pButterScribble->antiAliasing();
    m_pButterScribble->useTarget(pButter);
}

void EggFryScene::_showModel()
{
    if(m_nShapeIndex<4){
        for (int i=0; i<2; i++) {
            DragNode* pModel = _createDrageNode(localPath(gEggShapeName[m_nShapeIndex]+"/"+gEggShapeName[m_nShapeIndex]+".png"));
            this->addChildToContentLayer(pModel);
            CMVisibleRect::setPositionAdapted(pModel, visibleSize.width+200, 600);
            pModel->setName("model");
            pModel->setTag(i);
            m_pModelVector.push_back(pModel);
            
            EggModel* pEgg = EggModel::create();
            pEgg->setModelType(m_nShapeIndex, gEggShapeName[m_nShapeIndex], localPath(gEggShapeName[m_nShapeIndex]+"/"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pPanIn->getPosition()+Vec2(-90+i*180, 30));
            pEgg->setName("egg_model");
            m_pEggVector.push_back(pEgg);
            
            pEgg->setVisible(false);
            pModel->setTouchEnabled(false);
            pModel->runAction(MoveTo::create(1, m_pPanIn->getPosition()+Vec2(-90+i*180, 30)));
        }
    }else{
        DragNode* pModel = _createDrageNode(localPath("special"+std::to_string(m_nShapeIndex-3)+"/"+"special"+std::to_string(m_nShapeIndex-3)+".png"));
        this->addChildToContentLayer(pModel);
        CMVisibleRect::setPositionAdapted(pModel, visibleSize.width+200, 600);
        pModel->setName("model");
        pModel->setTag(0);
        m_pModelVector.push_back(pModel);
        
        EggModel* pEgg = EggModel::create();
        pEgg->setModelType(m_nShapeIndex, "special"+std::to_string(m_nShapeIndex-3), localPath("special"+std::to_string(m_nShapeIndex-3)+"/"));
        this->addChildToContentLayer(pEgg);
        pEgg->setPosition(m_pPanIn->getPosition()+Vec2(0, 0));
        pEgg->setName("egg_model");
        m_pEggVector.push_back(pEgg);
        
        pEgg->setVisible(false);
        pModel->setTouchEnabled(false);
        pModel->runAction(MoveTo::create(1, m_pPanIn->getPosition()+Vec2(0, 0)));
    }
    
    this->runAction(Sequence::create(DelayTime::create(1.2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_add2eggs_molds.mp3");
        _showIngredient(1);
        _showIngredient(2);
    }), NULL));
}

void EggFryScene::_showShovel()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pShovel = _createDrageNode("content/make/common/stove/shovel.png");
    this->addChildToContentLayer(m_pShovel);
    CMVisibleRect::setPositionAdapted(m_pShovel, 400+visibleSize.width, 600);
    m_pShovel->setTouchEnabled(false);
    m_pShovel->setActionPoint(Vec2(85, 70));
    m_pShovel->setName("shovel");
    m_pShovel->setLocalZOrder(20);
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pShovel->getPosition(), m_pPanIn->getPosition());
        m_pShovel->setTouchEnabled(true);
    }), NULL));
}

void EggFryScene::_showPlate()
{
    if (m_pPlate) {
        return;
    }
    m_pPlate = Sprite::create(localRootPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 680);
    m_pPlate->setScale(0.5, 0.3);
    m_pPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
}

void EggFryScene::_showBacons()
{
    AudioHelp::getInstance()->playEffect("vo_fry_bacon2eggs.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_add_bacon2pan.mp3");
    }), NULL));
    setExPath("content/make/bacon/");
    m_pBaconPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pBaconPlate);
    CMVisibleRect::setPositionAdapted(m_pBaconPlate, 200-50+visibleSize.width, 680);
    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    for (int i = 0; i<2; i++) {
        Sprite* pBacon = Sprite::create(localPath("bacon.png"));
        this->addChildToContentLayer(pBacon);
        CMVisibleRect::setPositionAdapted(pBacon, 200-50+visibleSize.width, 680+30-30*i);
        pBacon->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        pBacon->setScale(0.6);
        m_pBaconVector.push_back(pBacon);
    }
    m_pClip = _createDrageNode(localPath("clip.png"));
    this->addChildToContentLayer(m_pClip);
    CMVisibleRect::setPositionAdapted(m_pClip, 480+visibleSize.width, 700);
    m_pClip->setTouchEnabled(false);
    m_pClip->setActionPoint(Vec2(85, 70));
    m_pClip->setName("clip");
    m_pClip->setLocalZOrder(20);
    m_pClip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pClip->getPosition(), m_pBaconPlate->getPosition());
        m_pClip->setTouchEnabled(true);
    }), NULL));
}

void EggFryScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"butter_plate1.png","butter_plate2.png","","butter.png","butter_shadow.png"}, //batter
        {"egg.png","egg_shadow.png"}, //egg
        {"egg.png","egg_shadow.png"}, //egg
        {"pepper.png","pepper_shadow.png"}, //pepper
        {"salt.png","salt_shadow.png"}, //salt
        {},
    };
    
    
    Vec2 actionPoint[] = {
        Vec2(71, 61),
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(26, 165),
        Vec2(38, 155),
    };
    Vec2 pos[] = {
        Vec2(320, 690),
        Vec2(370, 700),
        Vec2(300, 690),
        Vec2(220, 750),
        Vec2(480, 740),
    };
    DragNode* pDragNode = _createDrageNode(localPath(sIngredents[index][0]));
    this->addChildToContentLayer(pDragNode);
    pDragNode->setTag(index);
    pDragNode->setLocalZOrder(index);
    pDragNode->setName("ingredient");
    pDragNode->setTouchEnabled(true);
    pDragNode->setActionPoint(actionPoint[index]);
    CMVisibleRect::setPositionAdapted(pDragNode, pos[index]+Vec2(visibleSize.width,0));
    
    IngredientManager::getInstance()->addIngredient(pDragNode);
    IngredientManager::getInstance()->addContainer(pDragNode,localPath(""), sIngredents[index]);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    pDragNode->runAction(action);
}

void EggFryScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "pan_butter1.png",
        "",
        "",
        "pan_pepper1.png",
        "pan_salt.png",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    if (index != 4 || index != 3) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            pIngredient->setLocalZOrder(5);
        }), NULL));
    }
    
    switch (index) {
            
#pragma mark =========  add Butter ==========
        case 0:
        {
            posAdd = Vec2(0, 0);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPan->getPosition()+Vec2(-60, 60)),
                                                    RotateTo::create(0.5,-60),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood(true,0.5);
                }
                if (!m_pButter) {
                    m_pButter = _createDrageNode(localPath(lNameStr));
                    m_pPanIn->addChild(m_pButter,5);
                    m_pButter->setVisible(false);
                    m_pButter->setTouchEnabled(false);
                    m_pButter->setName("butter");
                    m_pButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                          Show::create(),
                                                          CallFunc::create([=](){
                        m_pButter->getDragSprite()->runAction(AnimationHelp::createAnimate(localPath("pan_butter"), 1, 2,false,true,0.3));
                    }), NULL));
                }
            }),
                                                    MoveTo::create(0.5,m_pPan->getPosition()+Vec2(40, 100)),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
                
                m_pGuideLayer->removeGuide();
                m_pGuideLayer->showGuideMove(m_pPanIn->getPosition(), m_pPanIn->getPosition()+Vec2(100, 80));
                m_pButter->setTouchEnabled(true);
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 5) {
                }
                MenuLayer::addIngredient(MenuLayer::ePackageFriedEgg, MenuLayer::eFriedEgg_butter);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
            return;
        }
            break;
#pragma mark =========  add Egg ==========
        case 1:
        case 2:
        {
            dt = 4;
            posAdd = Vec2(10+20*(index-3), 10-20*(index-3));
            Vec2 posEgg = m_pPanIn->getPosition()+Vec2(-130+m_nEggCount*200, 120);
            if (m_nShapeIndex>=4) {
                posEgg = m_pPanIn->getPosition()+Vec2(-50+m_nEggCount*100, 120);
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPan->getPosition()+Vec2(40, 100)),
                                                    JumpBy::create(1,Vec2::ZERO,20,2),
                                                    CallFunc::create([=](){
                pIngredient->setVisible(false);
                
                Sprite* pEgg = Sprite::create(localPath("egg1.png"));
                this->addChildToContentLayer(pEgg);
                pEgg->setPosition(pIngredient->getWorldSpaceActionPoint());
                pEgg->setLocalZOrder(20);
                
                pEgg->runAction(Sequence::create(MoveTo::create(0.5, posEgg),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playAddEggEffect();
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pEgg->setTexture(localPath("egg2.png"));
                    if (m_nEggCount==1) {
                        AudioHelp::getInstance()->playEffect(arc4random()%100>0?"vo_sweet.mp3":"vo_fabulous.mp3");
                    }
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    DragNode* pModel;
                    if (m_nShapeIndex<4) {
                        pModel = m_pModelVector.at(m_nEggCount);
                    }else{
                        pModel = m_pModelVector.at(0);
                    }
                    if (pModel) {
                        pModel->setVisible(false);
                    }
                    EggModel* pEggModel;
                    if (m_nShapeIndex<4) {
                        pEggModel = m_pEggVector.at(m_nEggCount);
                    }else{
                        pEggModel = m_pEggVector.at(0);
                    }
                    if (pEggModel) {
                        pEggModel->setVisible(true);
                        pEggModel->addEgg();
                    }
                    IngredientManager::getInstance()->frozeIngredients(index,false);
                    
                    m_nStep++;
                    if (m_nStep == 3) {
                        _doFryEgg();
                    }
                    m_nEggCount++;
                    if (m_nEggCount==2) {
                        MenuLayer::addIngredient(MenuLayer::ePackageFriedEgg, MenuLayer::eFriedEgg_egg);
                        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    }
                    IngredientManager::getInstance()->removeIngredient(pIngredient);
                    pIngredient->removeFromParent();
                    pEgg->removeFromParent();
                }), NULL));
            }), NULL));
        }
            break;
#pragma mark =========  add pepper ==========
        case 3:
        {
            posAdd = Vec2(40, -30);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPanIn->getPosition()+Vec2(-50, 150)),
                                                    RotateTo::create(0.5,-100),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("salt_shaker.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pepper.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(100);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                for (auto pEggModel:m_pEggVector) {
                    if(m_nShapeIndex<4){
                        pEggModel->addIngredient(localPath(lNameStr),1);
                    }else{
                        pEggModel->addIngredient(localPath(lNameStr),1,Vec2(-50, 30));
                        pEggModel->addIngredient(localPath(lNameStr),1,Vec2(50, -20));
                    }
                }
                
            }),
                                                    JumpBy::create(1.5,Vec2(250,0),50,3),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 5) {
                    _finishFry();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageFriedEgg, MenuLayer::eFriedEgg_pepper);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add salt ==========
        case 4:
        {
            posAdd = Vec2(-90, -20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPanIn->getPosition()+Vec2(-50, 150)),
                                                    RotateTo::create(0.5,-140),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("salt_shaker.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/salt.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(140);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                
                for (auto pEggModel:m_pEggVector) {
                    pEggModel->addIngredient(localPath(lNameStr),1);
                }
            }),
                                                    JumpBy::create(2,Vec2(250,0),50,3),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_great.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 5) {
                    _finishFry();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageFriedEgg, MenuLayer::eFriedEgg_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
        default:
            break;
    }

}

void EggFryScene::_paintButter()
{
    m_nMoveCount++;
    if (m_nMoveCount==0) {
        
        AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
    }
    if (m_nMoveCount%50 == 0) {
        int percent = m_pButterScribble->getCanvas()->getPercentageTransparent();
        log("====percent===%d",percent);
        int step = m_nMoveCount/50;
        if (step<4) {
            m_pButter->getDragSprite()->setTexture(localPath("pan_butter"+std::to_string(step+2)+".png"));
        }
        if (percent<=28) {
            m_pGuideLayer->removeGuide();
            m_pButterScribble->resetCanvas();
            m_pButterScribble->paint(localPath("pan_butter0.png"));
            m_pButter->ignoreMoving = true;
            
            m_pButter->getDragSprite()->runAction(FadeOut::create(0.5));
            m_pButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                AudioHelp::getInstance()->stopLoopEffect();
                m_pButter->setTouchEnabled(false);
                m_pButter->setVisible(false);
            }),
                                                     DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                AudioHelp::getInstance()->stopLoopEffect();
                AudioHelp::getInstance()->playEffect("ding.mp3");
                AudioHelp::getInstance()->_playEffectNiceWork();
                //                finish Scribble Butter
                _showModel();
            }), NULL));
        }
    }
}

void EggFryScene::_doFryEgg()
{
    for (auto pEggModel:m_pEggVector) {
        pEggModel->fryEgg(6,3);
    }
    auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
    if (!pMilkDrop) {
        auto pParticle = ParticleSystemQuad::create("particle/smoke1.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pPanIn->getPosition());
        pParticle->setName("smoke");
    }
    this->runAction(Sequence::create(DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_fried_eggs_ready_soon.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_add_salt_pepper_eggs.mp3");
        _showIngredient(3);
        _showIngredient(4);
    }), NULL));
    m_pButter->runAction(FadeTo::create(6, 180));
}

void EggFryScene::_finishFry()
{
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    for (auto pModel:m_pModelVector) {
        pModel->setTouchEnabled(true);
    }
    m_pGuideLayer->removeGuide();
    m_pGuideLayer->showGuideMove(m_pPanIn->getPosition(), m_pPanIn->getPosition()+Vec2(10, 100));
}

void EggFryScene::_doFryBacon(FriedUnit* pBacon)
{
    pBacon->doFry();
    if (pBacon->flag == 0) {
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bacon"), 1, 3,false,true,2),
                                                      CallFunc::create([=](){
            if (!m_bEffectTurn) {
                m_bEffectTurn = true;
                AudioHelp::getInstance()->playEffect("vo_turn_over_bacon.mp3");
            }
            pBacon->flag = 1;
            if (m_pClip->getPositionX()>visibleSize.width) {
                m_pClip->setTouchEnabled(false);
                m_pClip->index = 3;
                m_pClip->runAction(Sequence::create(MoveTo::create(1, m_pPan->getPosition()+Vec2(0, 100)),
                                                    CallFunc::create([=](){
                    m_pClip->setTouchEnabled(true);
                }), NULL));
            }
        }), NULL));
    }else if (pBacon->flag==2) {
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bacon"), 3, 4,false,true,3),
                                                      DelayTime::create(3),
                                                      CallFunc::create([=](){
            pBacon->getFood()->setTexture(localPath("bacon5_"+std::to_string(pBacon->getTag()+1)+".png"));
            pBacon->isFriedOK = true;
            if (!m_bEffectBaconOK) {
                m_bEffectBaconOK = true;
                AudioHelp::getInstance()->playEffect("vo_bacon_ready_soon.mp3");
            }
            AudioHelp::getInstance()->playEffect("ding.mp3");
            AudioHelp::getInstance()->playEffect("vo_great.mp3");
            if (m_pClip->getPositionX()>visibleSize.width) {
                m_pClip->setTouchEnabled(false);
                m_pClip->index = 3;
                m_pClip->runAction(Sequence::create(MoveTo::create(1, m_pPan->getPosition()+Vec2(0, 100)),
                                                    CallFunc::create([=](){
                    m_pClip->setTouchEnabled(true);
                }), NULL));
            }
        }), NULL));
    }
}
void EggFryScene::_finishFryBacon()
{
    
}
void EggFryScene::_ingredientOut(DragNode* pNode)
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    IngredientManager::getInstance()->frozeIngredients(pNode->getTag(),false);
    
}

void EggFryScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        if (m_nShapeIndex<4) {
            SceneManager::replaceTheScene<EatEggScene>();
        }else{
            m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                _showBacons();
            }), NULL));
//            SceneManager::replaceTheScene<EggFryBaconScene>();
        }
    }), NULL));
    
}