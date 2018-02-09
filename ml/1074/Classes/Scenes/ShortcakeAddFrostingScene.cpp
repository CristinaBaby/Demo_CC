
#include "ShortcakeAddFrostingScene.h"
#include "SceneManager.h"

ShortcakeAddFrostingScene::ShortcakeAddFrostingScene()
{
    m_nMoveCount = 0;
}

ShortcakeAddFrostingScene::~ShortcakeAddFrostingScene()
{
    
}
bool ShortcakeAddFrostingScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showFood();
    _showFrosting();
    _showSpoon();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void ShortcakeAddFrostingScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ShortcakeAddFrostingScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ShortcakeAddFrostingScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "spoon"){
        if(pDragNode->index == 1){
            Rect rect = Rect(-200, 80*(m_nCakeCount-1)-130, 400, 260);
            rect.origin = m_pFood->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                m_pFrostingScribble->paint(worldPoint);
                Director::getInstance()->getRenderer()->render();
            }
        }
        
    }
}

void ShortcakeAddFrostingScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("spoon" == name) {
        int index = pDragNode->index;
        if (index == 0) {
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(false);
                
                m_pGuideLayer->removeGuide();
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(50, 100)),
                                                      CallFunc::create([=](){
                    pDragNode->setLocalZOrder(5);;
                }),
                                                      Spawn::create(RotateBy::create(0.5, -30),
                                                                    MoveBy::create(0.5, Vec2(0, -30)), NULL),
                                                      CallFunc::create([=](){
                    pDragNode->getDragSprite()->setTexture("content/make/common/mix/agitator1_2.png");
                }),
                                                      RotateBy::create(0.3, 20),
                                                      CallFunc::create([=](){
                    pDragNode->getDragSprite()->setTexture("content/make/common/mix/agitator1_1.png");
                    Sprite* pCream = Sprite::create(localPath("cream"+std::to_string(m_nFrostingIndex+1)+"_4.png"));
                    pDragNode->addChild(pCream);
                    pCream->setName("cream");
                    pCream->setAnchorPoint(Vec2(0,0));
                    pCream->setPosition(pDragNode->getActionPoint()-pDragNode->getDragSprite()->getContentSize()*0.5-pCream->getContentSize()*0.5);
                    pDragNode->setLocalZOrder(20);
                    AudioHelp::getInstance()->playEffect("spoon_icecream.mp3");
                }),
                                                      Spawn::create(RotateBy::create(0.2, 10),
                                                                    JumpBy::create(0.5, Vec2(0, -200), 100, 1),
                                                                    ScaleTo::create(0.3, 1), NULL),
                                                      CallFunc::create([=](){
                    pDragNode->setTouchEnabled(true);
                }),
                                                      NULL));
            }
        }else{
            Rect rect = Rect(-200, 80*(m_nCakeCount-1)-130, 400, 260);
            rect.origin = m_pFood->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                m_pFrostingScribble->paint(worldPoint+pDragNode->getOffset(),worldPoint);
                Director::getInstance()->getRenderer()->render();
                m_nMoveCount++;
                AudioHelp::getInstance()->playLoopEffect("spoon_icecream.mp3");
                if (m_nMoveCount%10 == 0) {
                    int percent = m_pFrostingScribble->getCanvas()->getPercentageTransparent();
                    if (percent<80) {
                        Sprite* pCream = (Sprite*)pDragNode->getChildByName("cream");
                        if (pCream) {
                            pCream->setScale((percent+20)*0.01);
                        }
                    }
                }
            }
        }
    }
}

void ShortcakeAddFrostingScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "spoon"){
        if(pDragNode->index == 1){
            int percent = m_pFrostingScribble->getCanvas()->getPercentageTransparent();
            log("====percent===%d",percent);
            if (percent <= 40) {
//                Sprite* pCream = (Sprite*)pDragNode->getChildByName("cream");
//                if (pCream) {
//                    pCream->runAction(FadeOut::create(0.5));
//                }
                m_pFrostingScribble->resetCanvas();
                m_pFrostingScribble->paint(localRootPath("dec/cream"+std::to_string(m_nFrostingIndex+1)+".png"));
                
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(MoveTo::create(1, Vec2(visibleSize.width+200, pDragNode->getPositionY())));
                
                m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
                m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
                _finish();
            }
        }
    }
    
}

void ShortcakeAddFrostingScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ShortcakeAddFrostingScene::_initData()
{
    setExPath("content/make/shortcake/make2/");
    m_nFrostingIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void ShortcakeAddFrostingScene::_showFood()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
    std::string strCake = GameDataManager::getInstance()->m_sFlavorCakeStr;
    std::string strCream = GameDataManager::getInstance()->m_sFlavorCreamStr;
    std::string strFruit = GameDataManager::getInstance()->m_sFlavorStr;
    std::vector<std::string> flavorCakes = StringHelp::split(strCake, ",");
    std::vector<std::string> flavorCreams = StringHelp::split(strCream, ",");
    std::vector<std::string> flavorFruits = StringHelp::split(strFruit, ",");
    
    int cakeCount = flavorCakes.size();
    m_nCakeCount = cakeCount;
    m_pPlate = Sprite::create(localRootPath("dec/cake_plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 300-visibleSize.height);
    
    m_pFood = Node::create();
    this->addChildToContentLayer(m_pFood);
    CMVisibleRect::setPositionAdapted(m_pFood, 320, 350-visibleSize.height);
    
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                         CallFunc::create([=](){
        if (cakeCount>3) {
            m_pPlate->runAction(MoveBy::create(0.5, Vec2(0, -80*(cakeCount-3))));
        }
    }), NULL));
    m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                        CallFunc::create([=](){
        if (cakeCount>3) {
            m_pFood->runAction(MoveBy::create(0.5, Vec2(0, -80*(cakeCount-3))));
        }
    }), NULL));
    
    for(int i = 0;i<flavorCakes.size()-1;i++) {
        Sprite* pCake = Sprite::create(localRootPath("dec/cake"+std::to_string(1+atoi(flavorCakes.at(i).c_str()))+".png"));
        m_pFood->addChild(pCake);
        pCake->setPosition(Vec2(0, 80*i));
        
        Sprite* pCream = Sprite::create(localRootPath("dec/cream"+std::to_string(1+atoi(flavorCreams.at(i).c_str()))+".png"));
        m_pFood->addChild(pCream);
        pCream->setPosition(Vec2(0, 80*i));
        
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"fruit"+std::to_string(i)+".png");
        Sprite* pFruit = Sprite::create(FileUtils::getInstance()->getWritablePath()+"fruit"+std::to_string(i)+".png");
        if (pFruit) {
            m_pFood->addChild(pFruit);
            pFruit->setPosition(Vec2(0, 80*i+40));
        }
    }
    
    Sprite* pCake = Sprite::create(localRootPath("dec/cake"+std::to_string(1+atoi(flavorCakes.at(cakeCount-1).c_str()))+".png"));
    m_pFood->addChild(pCake);
    pCake->setPosition(Vec2(0, 80*(cakeCount-1)));
    
    Sprite* pColor = Sprite::create(localRootPath("dec/cream"+std::to_string(m_nFrostingIndex+1)+".png"));
    pColor->setPosition(pColor->getContentSize()*0.5);
    
    m_pFrostingScribble = ScribbleNode::create(pColor->getContentSize());
    m_pFood->addChild(m_pFrostingScribble);
    m_pFrostingScribble->setPosition(Vec2(0, 80*(m_nCakeCount-1)));
    m_pFrostingScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
    m_pFrostingScribble->setLocalZOrder(5);
    m_pFrostingScribble->antiAliasing();
    
    m_pFrostingScribble->useTarget(pColor);
}

void ShortcakeAddFrostingScene::_showSpoon()
{
    m_pSpoon = _createDrageNode("content/make/common/mix/agitator1_1.png");
    this->addChildToContentLayer(m_pSpoon);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 480+visibleSize.width, 720);
    m_pSpoon->setName("spoon");
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->setActionPoint(Vec2(68, 77));
    m_pSpoon->setScale(0.7);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_pBowl->getPosition());
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pSpoon->runAction(action);
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}

void ShortcakeAddFrostingScene::_showFrosting()
{
    m_pBowl = Sprite::create(localPath("bowl2_0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 780);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBowlFront = Sprite::create(localPath("bowl2_1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pCreamClipping = ClippingNode::create(Sprite::create(localPath("bowl2_mask.png")));
    m_pBowl->addChild(m_pCreamClipping);
    m_pCreamClipping->setAlphaThreshold(0.5);
    m_pCreamClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    
    m_pCream = Sprite::create(localPath("cream"+std::to_string(GameDataManager::getInstance()->m_nFlavorIndex+1)+"_3.png"));
    m_pCreamClipping->addChild(m_pCream);
    
    m_pBowl->setScale(0.7);
    m_pBowlFront->setScale(0.7);
}
void ShortcakeAddFrostingScene::_finish()
{
    AudioHelp::getInstance()->_playEffectNiceWork();
    std::string flavorStr = GameDataManager::getInstance()->m_sFlavorCreamStr;
    std::string str;
    if (flavorStr != "") {
        flavorStr.append(",");
    }
    flavorStr.append(std::to_string(m_nFrostingIndex));
    GameDataManager::getInstance()->m_sFlavorCreamStr = flavorStr;
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ShortcakeCutFruitScene>();
    }), NULL));
}