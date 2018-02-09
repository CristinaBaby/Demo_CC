
#include "JuiceCutFruitScene.h"
#include "SceneManager.h"
#include "PolygonHelp.h"

static int gnFruitCutCount[] = {
    1,3,3,1,3,3,3,3
};
JuiceCutFruitScene::JuiceCutFruitScene()
{
    
    m_bItemEnable = false;
    m_nMoveCount = 0;
    m_nFlavorIndex = -1;
    m_nCutCount = 0;
    m_pKnife = nullptr;
}

JuiceCutFruitScene::~JuiceCutFruitScene()
{
    
}
bool JuiceCutFruitScene::init()
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
    
    
    _showBoard();
    
    _showFruit(m_nFlavorIndex);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void JuiceCutFruitScene::onEnter()
{
    ExtensionScene::onEnter();
    Scene* scene = this->getScene();
    
    //        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    scene->getPhysicsWorld()->setAutoStep(false);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(JuiceCutFruitScene::updateStart), 2);
}

void JuiceCutFruitScene::updateStart(float delta)
{
    Scene* scene = this->getScene();
    scene->getPhysicsWorld()->setAutoStep(false);
    scheduleUpdate();
}
void JuiceCutFruitScene::update(float delta)

{
    
    // use fixed time and calculate 3 times per frame makes physics simulate more precisely.
    
    //这里表示先走3步瞧瞧  如果fps是1/60  三个setp就是1/180
    
    for (int i = 0; i < 3; ++i)
        
    {
        Scene* scene = this->getScene();
        scene->getPhysicsWorld()->step(1/180.0f);
        
    }
    
}
void JuiceCutFruitScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void JuiceCutFruitScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "knife"){
    }
}

void JuiceCutFruitScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "knife"){
            
        int cutCount = gnFruitCutCount[m_nFlavorIndex];
#pragma mark                cut 草莓 葡萄
            if (cutCount==1) {
                bool flag = false;
                for (auto pFruit:m_Fruits) {
                    Rect rect = pFruit->getBoundingBox();
                    if (rect.containsPoint(worldPoint)) {
                        flag = true;
                        break;
                    }
                }
                if (flag) {
                    pDragNode->setTouchEnabled(false);
                    AudioHelp::getInstance()->playEffect("cut.mp3");
                    m_pGuideLayer->removeGuide();
                    auto pFruit = m_Fruits.at(0);
                    if (m_nCutCount == 2) {
                        AudioHelp::getInstance()->playEffect("vo_careful_fingers.mp3");
                    }
                    Vec2 pos = pFruit->getPosition()+Vec2(50, -40);
                    
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        m_nCutCount++;
                        
                        Sprite* pLeft = Sprite::create(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_2.png"));
                        this->addChildToContentLayer(pLeft);
                        pLeft->setFlippedX(true);
                        pLeft->setPosition(pFruit->getPosition());
                        m_FruitsBoard.pushBack(pLeft);
                        
                        pFruit->setTexture(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_1.png"));
                        pFruit->setPosition(pFruit->getPosition()+Vec2(80, 0));
                        m_FruitsBoard.pushBack(pFruit);
                        m_Fruits.eraseObject(pFruit);
                        
                        pFruit->setLocalZOrder(1);
                        pLeft->setLocalZOrder(1);
//                        pLeft->runAction(Sequence::create(DelayTime::create(0.5),
//                                                          MoveBy::create(0.5, Vec2(50, 0)), NULL));
//                        pFruit->runAction(Sequence::create(DelayTime::create(0.5),
//                                                           MoveBy::create(0.5, Vec2(50, 0)), NULL));
                        
                        if (m_Fruits.size()==0) {
                            _finishCut();
                        }else{
                            pDragNode->setTouchEnabled(true);
                            
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", m_Fruits.at(0)->getPosition());
                        }
                        
                    }), NULL));
                    
                }
            }else{
#pragma mark                cut 菠萝
                if(m_nFlavorIndex==4){
                    auto pFruit = m_Fruits.at(0);
                    Rect rect = pFruit->getBoundingBox();
                    if (!rect.containsPoint(worldPoint)) {
                        return;
                    }
                    pDragNode->setTouchEnabled(false);
                    if (m_nCutCount == 2) {
                        AudioHelp::getInstance()->playEffect("vo_careful_fingers.mp3");
                    }
                    AudioHelp::getInstance()->playEffect("cut.mp3");
                    m_pGuideLayer->removeGuide();
                    Vec2 pos = pFruit->getPosition()+Vec2(50, 0);
                    Vec2 delt = Vec2::ZERO;
                    if(m_nCutCount %3 == 0){
                        pos = pFruit->getPosition()+Vec2(-50, 50);
                        delt = Vec2(100, -100);
                    }else if (m_nCutCount %3 == 1) {
                        pos = pFruit->getPosition()+Vec2(50, 50);
                        delt = Vec2(-100, -100);
                    }else if (m_nCutCount %3 == 2) {
                        pos = pFruit->getPosition()+Vec2(100, 0);
                        delt = Vec2(-200, 0);
                    }
                    pDragNode->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos),
                                                                        RotateTo::create(0.5, -10+60*(m_nCutCount%3)), NULL),
                                                          MoveBy::create(0.5, delt),
                                                          CallFunc::create([=](){
                        if (m_nCutCount%3 == 2) {
                            m_Fruits.eraseObject(pFruit);
                            m_FruitsBoard.pushBack(pFruit);
                            
                            Sprite* pFruitLeft = Sprite::create(localPath("cut/fruit4_"+std::to_string(m_nCutCount%3+1)+".png"));
                            this->addChildToContentLayer(pFruitLeft);
                            pFruitLeft->setPosition(pFruit->getPosition()+Vec2(-50, 0));
                            m_FruitsBoard.pushBack(pFruitLeft);
                        }
                        
                        pFruit->setTexture(localPath("cut/fruit4_"+std::to_string(m_nCutCount%3+1)+".png"));
                        
                        m_nCutCount++;
                        
                        if (m_nCutCount == cutCount*2){
                            _finishCut();
                        }else{
                            pDragNode->setTouchEnabled(true);
                            
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", m_Fruits.at(0)->getPosition(),-30+60*(m_nCutCount%3));
                        }
                    }), NULL));
                    
                }else if(m_nFlavorIndex==5) {
#pragma mark                    cut 西瓜
                    auto pFruit = m_Fruits.at(0);
                    Rect rect = pFruit->getBoundingBox();
                    if (!rect.containsPoint(worldPoint)) {
                        return;
                    }
                    pDragNode->setTouchEnabled(false);
                    if (m_nCutCount == 2) {
                        AudioHelp::getInstance()->playEffect("vo_careful_fingers.mp3");
                    }
                    AudioHelp::getInstance()->playEffect("cut.mp3");
                    m_pGuideLayer->removeGuide();
                    Vec2 pos = pFruit->getPosition()+Vec2(100-m_nCutCount%3*50,0);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        if (m_FruitsBoard.find(pFruit)==m_FruitsBoard.end()) {
                            m_FruitsBoard.pushBack(pFruit);
                        }
                        Sprite* pFruitRight = Sprite::create(localPath("cut/fruit5_4.png"));
                        this->addChildToContentLayer(pFruitRight);
                        pFruitRight->setPosition(pFruit->getPosition()+Vec2(100-m_nCutCount%3*50, 0));
                        m_FruitsBoard.pushBack(pFruitRight);
                        
                        pFruit->setTexture(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(m_nCutCount%3+1)+".png"));
                        m_nCutCount++;
                        if (m_nCutCount%3 == 0) {
                            pFruit->runAction(Sequence::create(DelayTime::create(0.5),
                                                               JumpBy::create(1, Vec2(visibleSize.width,100), 300, 1),
                                                               CallFunc::create([=](){
                                pFruit->setVisible(false);
                            }), NULL));
                            m_Fruits.eraseObject(pFruit);
                        }
                        
                        
                        if (m_nCutCount == cutCount*2){
                            _finishCut();
                        }else{
                            Sprite* pNextFruit = nullptr;
                            pNextFruit = m_Fruits.at(0);
                            m_pGuideLayer->removeGuide();
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pNextFruit->getPosition()+Vec2(100-(m_nCutCount-1)%3*50,0));
                            
                            pDragNode->setTouchEnabled(true);
                        }
                    }), NULL));
                }else if(m_nFlavorIndex==7) {
#pragma mark                    cut 胡萝卜
                    auto pFruit = m_Fruits.at(0);
                    Rect rect = pFruit->getBoundingBox();
                    if (!rect.containsPoint(worldPoint)) {
                        return;
                    }
                    pDragNode->setTouchEnabled(false);
                    if (m_nCutCount == 2) {
                        AudioHelp::getInstance()->playEffect("vo_careful_fingers.mp3");
                    }
                    AudioHelp::getInstance()->playEffect("cut.mp3");
                    m_pGuideLayer->removeGuide();
                    Vec2 pos = pFruit->getPosition()+Vec2(100-m_nCutCount%3*50,0);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        if (m_FruitsBoard.find(pFruit)==m_FruitsBoard.end()) {
                            m_FruitsBoard.pushBack(pFruit);
                        }
                        Sprite* pFruitRight;
                        if (m_nCutCount%3==0) {
                            pFruitRight = Sprite::create(localPath("cut/fruit7_4.png"));
                        }else{
                            pFruitRight = Sprite::create(localPath("cut/fruit7_5.png"));
                        }
                        this->addChildToContentLayer(pFruitRight);
                        pFruitRight->setPosition(pFruit->getPosition()+Vec2(100-m_nCutCount%3*50, 0));
                        m_FruitsBoard.pushBack(pFruitRight);
                        
                        pFruit->setTexture(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(m_nCutCount%3+1)+".png"));
                        m_nCutCount++;
                        if (m_nCutCount%3 == 0) {
                            m_Fruits.eraseObject(pFruit);
                        }
                        
                        
                        if (m_nCutCount == cutCount*2){
                            _finishCut();
                        }else{
                            Sprite* pNextFruit = nullptr;
                            pNextFruit = m_Fruits.at(0);
                            m_pGuideLayer->removeGuide();
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pNextFruit->getPosition()+Vec2(50,0));
                            
                            pDragNode->setTouchEnabled(true);
                        }
                    }), NULL));
                }else {
                    
                    Sprite* pFruit = nullptr;
                    if (m_nCutCount%3 == 0){
                        pFruit = m_Fruits.at(0);
                    }else if (m_nCutCount%3 == 1){
                        pFruit = m_FruitsBoard.at(m_nCutCount/3*4);
                    }else{
                        pFruit = m_FruitsBoard.at(m_nCutCount/3*4+1);
                    }
                    Rect rect = pFruit->getBoundingBox();
                    if (!rect.containsPoint(worldPoint)) {
                        return;
                    }
                    pDragNode->setTouchEnabled(false);
                    AudioHelp::getInstance()->playEffect("cut.mp3");
                    m_pGuideLayer->removeGuide();
                    Vec2 pos = pFruit->getPosition()+Vec2(50, 0);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        int shape = 2;
                        if (m_nCutCount%3 == 0) {
                            shape = 1;
                            m_Fruits.eraseObject(pFruit);
                        }
                        if (m_FruitsBoard.find(pFruit)==m_FruitsBoard.end()) {
                            m_FruitsBoard.pushBack(pFruit);
                        }
                        Sprite* pFruitLeft = Sprite::create(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(shape)+".png"));
                        this->addChildToContentLayer(pFruitLeft);
                        pFruitLeft->setPosition(pFruit->getPosition());
                        m_FruitsBoard.pushBack(pFruitLeft);
                        pFruitLeft->setFlippedX(true);
                        pFruitLeft->setTag(1);
                        
                        pFruit->setTexture(localPath("cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(shape)+".png"));
                        pFruit->setPosition(pFruit->getPosition()+Vec2(80, 0));
                        pFruit->setFlippedX(false);
//                        pFruitRight->runAction(Sequence::create(DelayTime::create(0.5),
//                                                                MoveBy::create(0.5, Vec2(50, 0)), NULL));
//                        pFruit->runAction(Sequence::create(DelayTime::create(0.5),
//                                                           MoveBy::create(0.5, Vec2(50, 0)), NULL));
                        
                        
                        m_nCutCount++;
                        
                        if (m_nCutCount == cutCount*2){
                            _finishCut();
                        }else{
                            Sprite* pNextFruit = nullptr;
                            if (m_nCutCount%3 == 0){
                                pNextFruit = m_Fruits.at(0);
                            }else if (m_nCutCount%3 == 1){
                                pNextFruit = m_FruitsBoard.at(m_nCutCount/3*4);
                            }else{
                                pNextFruit = m_FruitsBoard.at(m_nCutCount/3*4+1);
                            }
                            m_pGuideLayer->removeGuide();
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pNextFruit->getPosition());
                            
                            pDragNode->setTouchEnabled(true);
                        }
                    }), NULL));
                }
            }
        
    }
}

void JuiceCutFruitScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void JuiceCutFruitScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void JuiceCutFruitScene::_initData()
{
    setExPath("content/make/juice/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}
void JuiceCutFruitScene::_showBoard()
{
    m_pBoard = Sprite::create(localPath("cut/board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 320-visibleSize.width, 380);
    m_pBoard->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
}

void JuiceCutFruitScene::_showKnife()
{
    if(m_pKnife){
        return;
    }
    m_pKnife = _createDrageNode(localPath("cut/knife.png"));
    this->addChildToContentLayer(m_pKnife);
    CMVisibleRect::setPositionAdapted(m_pKnife, 600-visibleSize.width, 400);
    m_pKnife->setName("knife");
    m_pKnife->setTouchEnabled(false);
    m_pKnife->setActionPoint(Vec2(25, 260));
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            //            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pKnife->runAction(action);
    
}

void JuiceCutFruitScene::_showFruit(int index)
{
    m_pGuideLayer->removeGuide();
    int cutCount = gnFruitCutCount[m_nFlavorIndex];
    m_nFlavorIndex = index;
    int fruitCount = 2;
    if (cutCount==1) {
        fruitCount = 4;
    }
    if(m_pKnife){
        m_pKnife->setTouchEnabled(false);
    }
    _showKnife();
    for (Sprite* pItem:m_Fruits) {
        
        pItem->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pItem->removeFromParent();
        }), NULL));
    }
    float dt = 1;
    if (m_Fruits.size() == 0){
        dt = 0.1;
    }
    m_Fruits.clear();
    for (int i = 0; i<fruitCount; i++) {
        Sprite* pFruit = Sprite::create(localPath("cut/fruit"+std::to_string(index)+"_0.png"));
        this->addChildToContentLayer(pFruit);
        if(cutCount==1){
            CMVisibleRect::setPositionAdapted(pFruit, 320+30*(fruitCount-1)-i*60+visibleSize.width, 400);
        }else{
            CMVisibleRect::setPositionAdapted(pFruit, 280+90*(fruitCount-1)-i*200+visibleSize.width, 400);
            if (m_nFlavorIndex == 7) {
                CMVisibleRect::setPositionAdapted(pFruit, 320+visibleSize.width, 350+100*i);
            }
        }
    
        m_Fruits.pushBack(pFruit);
        pFruit->setTag(i);
        
        pFruit->runAction(Sequence::create(DelayTime::create(dt),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
            m_bItemEnable = true;
            m_pKnife->setTouchEnabled(true);
            if(cutCount==1 && i == 0){
                m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
            }else if(cutCount!=1 && i == 0){
                if (m_nFlavorIndex == 7) {
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition()+Vec2(50, 50));
                }else if(m_nFlavorIndex == 8){
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                }else{
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                }
            }
        }), NULL));
    }
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}

void JuiceCutFruitScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = Sprite::create(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 300-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBowlFront = Sprite::create(localPath("bowl2.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatterClipping = ClippingNode::create(Sprite::create(localPath("bowl2.png")));
    m_pBowl->addChild(m_pBatterClipping);
    m_pBatterClipping->setAlphaThreshold(0.5);
    m_pBatterClipping->setInverted(true);
    m_pBatterClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
}

void JuiceCutFruitScene::_finishCut()
{
    MenuLayer::addIngredient(MenuLayer::ePackageJuice, MenuLayer::eJuice_fruit);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("vo_talented.mp3");
    
    m_pKnife->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    _showBowl();
    int i = 0;
    int cutCount = gnFruitCutCount[m_nFlavorIndex];
    for (auto pFruit:m_FruitsBoard) {
        pFruit->setLocalZOrder(6);
        pFruit->runAction(Sequence::create(Spawn::create(MoveTo::create(1, m_pBoard->getPosition()+Vec2(100-10*i+5.0*(arc4random()%10), 300+i*10-5.0*(arc4random()%10))),
                                                         RotateBy::create(1, -50.0+5.0*(arc4random()%10)), NULL),
                                           Spawn::create(MoveBy::create(0.5, Vec2(-100, -100)),
                                                         ScaleTo::create(0.5,cutCount==1?0.5:0.8), NULL),
                                           DelayTime::create(i*0.1),
                                           FadeOut::create(0.3), NULL));
        i++;
    }
    m_pBoard->setLocalZOrder(5);
    m_pBoard->runAction(Sequence::create(MoveBy::create(1, Vec2(200, 350)),
                                         RotateBy::create(0.5, -50),
                                         CallFunc::create([=](){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        
        PolygonHelp::getInstance()->addShapesWithFile("fruit_bowl", body,-m_pBowl->getContentSize().width/2,-m_pBowl->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pBowl->setPhysicsBody(body);
        for (int k = 0; k<14; k++) {
            std::string Path;
            if (cutCount==1) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(k%2+1)+".png";
            }else if (m_nFlavorIndex==10) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_6.png";
            }else if (m_nFlavorIndex==5) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_4.png";
            }else if (m_nFlavorIndex==7) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_5.png";
            }else if (m_nFlavorIndex==4) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_4.png";
            }else if (m_nFlavorIndex == 1) {
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(k%2+2)+".png";
            }else{
                Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_2.png";
            }
            Sprite* pSlice = Sprite::create(localPath(Path));
            this->addChildToContentLayer(pSlice);
            pSlice->setPosition(m_pBowl->getPosition()+Vec2(50-5.0*(arc4random()%10), 200+5.0*(arc4random()%10)));
            pSlice->setScale(0.5);
            if(m_nFlavorIndex==5){
                pSlice->setScale(0.4);
            }
            pSlice->setLocalZOrder(5);
            pSlice->setRotation(50.0-10.0*(arc4random()%10));
            pSlice->setVisible(false);
            pSlice->runAction(Sequence::create(DelayTime::create(0.1+0.05*k),
                                               Show::create(),
                                               CallFunc::create([=](){
                //                auto body = PhysicsBody::createBox(Size(pSlice->getContentSize().width*0.5,pSlice->getContentSize().height*0.5),PhysicsMaterial(1.0f, 0.0f, 1.0f));
                
                auto body = PhysicsBody::createCircle(MIN(pSlice->getContentSize().width, pSlice->getContentSize().height)*0.1,PhysicsMaterial(1.0f, 0.0f, 1.0f));
                body->setDynamic(true);
                pSlice->setPhysicsBody(body);
            }),NULL));
        }
        AudioHelp::getInstance()->playEffect("pour_corn_kernal.mp3");
    }),
                                         DelayTime::create(2),
                                         CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
    }),
                                         RotateTo::create(0.5,0),
                                         MoveBy::create(1,Vec2(visibleSize.width,0)),
                                         CallFunc::create([=](){
        _finish();
    }), NULL));
}
void JuiceCutFruitScene::_finish()
{
    //    ********** test *************
    //    {
    //        std::string cakeStr = GameDataManager::getInstance()->m_sFlavorCakeStr;
    //        std::string str;
    //        if (cakeStr != "") {
    //            cakeStr.append(",");
    //        }
    //        cakeStr.append(std::to_string(m_nFlavorIndex));
    //        GameDataManager::getInstance()->m_sFlavorCakeStr = cakeStr;
    //    }
    //    {
    //        std::string flavorStr = GameDataManager::getInstance()->m_sFlavorCreamStr;
    //        std::string str;
    //        if (flavorStr != "") {
    //            flavorStr.append(",");
    //        }
    //        flavorStr.append(std::to_string(m_nFlavorIndex));
    //        GameDataManager::getInstance()->m_sFlavorCreamStr = flavorStr;
    //    }
    
    GameDataManager::getInstance()->m_nFlavorIndex = m_nFlavorIndex;
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<JuiceMakeScene>();
    }), NULL));
}