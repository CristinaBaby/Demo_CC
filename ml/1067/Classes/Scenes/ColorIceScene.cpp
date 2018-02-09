
#include "ColorIceScene.h"
#include "CCImageColorSpace.h"

static Vec3 gHSB[10] = {
    Vec3(0, 0, 0),
    Vec3(-80, -10, 0),
    Vec3(59, 28, 25),
    Vec3(-10, 0, 0),
    Vec3(51, 100, 73),
    Vec3(0, 0, 0),
    Vec3(-4, 52, 37),
    Vec3(53, 61, 51),
    Vec3(35, 35, 12),
    Vec3(97, -11, 11),
};
ColorIceScene::ColorIceScene()
{
    
}

ColorIceScene::~ColorIceScene()
{
    
}
bool ColorIceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    Sprite* pBg = Sprite::create("content/make/color/color_bk.jpg");
    this->addChildToBGLayer(pBg);
    pBg->setPosition(visibleSize*0.5);

    bool b = GameDataManager::getInstance()->getFlavorLocked();
    b = true;// 后面要求每次都要碎冰
    if (!b) {
        
        _showColorBottle(true);
        _showSnowCone();
    }else{
        FileUtils::getInstance()->addSearchPath("cocos/");
        
        Node* scene = CSLoader::createNode("cocos/ColorScene.csb");
        this->addChildToContentLayer(scene);
        CMVisibleRect::setPosition(scene, 0, 0);
        ui::Helper::doLayout(scene);
        
        
        ImageView* pBottle = dynamic_cast<ImageView*>(scene->getChildByName("frozen_bottles"));
        pBottle->addTouchEventListener(CC_CALLBACK_2(ColorIceScene::onBottleClicked, this));
        pBottle->setPosition(Vec2(Vec2(0, -visibleSize.height), pBottle->getPosition()));
        pBottle->runAction(Sequence::create(DelayTime::create(1),
                                            CallFunc::create([=]()
                                                             {
                                                                 
                                                                 AudioHelp::getInstance()->playEffect("syrup_frozen.mp3");
                                                             }),
                                            EaseQuinticActionIn::create(MoveBy::create(0.8, Vec2(0, -visibleSize.height))),
                                            ScaleTo::create(0.1, 1,0.99),
                                            ScaleTo::create(0.2, 1),
                                            CallFunc::create([=]()
                                                             {
                                                                 pBottle->setTouchEnabled(true);
                                                                 pBottle->runAction(RepeatForever::create(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 5, 1),
                                                                                                                                         Sequence::create(ScaleTo::create(0.2, 1,0.95),
                                                                                                                                                          ScaleTo::create(0.3, 0.95,1),
                                                                                                                                                          ScaleTo::create(0.5, 1), NULL), NULL),
                                                                                                                           DelayTime::create(1), NULL)));
                                                             }), NULL));
        
        m_pUnlockLayer = scene->getChildByName("unlockLayer");
        m_pUnlockLayer->setVisible(false);
        
        DragNode* pTool0 = _createDrageNode("color/broom0.png");
        m_pUnlockLayer->addChild(pTool0,6,0);
        Node* p0 = m_pUnlockLayer->getChildByName("broom0");
        pTool0->setPosition(Vec2(visibleSize.width+p0->getPositionX(), p0->getPositionY()));
        pTool0->setActionPoint(Vec2(61, 51));
        
        DragNode* pTool1 = _createDrageNode("color/hammer.png");
        m_pUnlockLayer->addChild(pTool1,6,1);
        Node* p1 = m_pUnlockLayer->getChildByName("hammer");
        pTool1->setPosition(Vec2(visibleSize.width+p1->getPositionX(), p1->getPositionY()));
        pTool1->setActionPoint(Vec2(8, 138));
        
        m_ToolBox.pushBack(pTool0);
        m_ToolBox.pushBack(pTool1);
        
        m_pBoxBack = m_pUnlockLayer->getChildByName("tool_box0");
        m_pBoxFront = m_pUnlockLayer->getChildByName("tool_box1");
        m_pBoxBack->setLocalZOrder(5);
        m_pBoxFront->setLocalZOrder(7);
        m_pBoxBack->setPosition(Vec2(visibleSize.width+m_pBoxBack->getPositionX(), m_pBoxBack->getPositionY()));
        m_pBoxFront->setPosition(Vec2(visibleSize.width+m_pBoxFront->getPositionX(), m_pBoxFront->getPositionY()));
        
        for (int i = 0; i<3; i++) {
            std::stringstream ostr;
            ostr<<"bottle_snow"<<i;
            Node* pSnow = m_pUnlockLayer->getChildByName(ostr.str());
            if (pSnow) {
                pSnow->setTag(i);
                m_Snows.pushBack(pSnow);
            }
            
            std::stringstream ostr2;
            ostr2<<"frozen_bottle"<<i<<"_0";
            Node* pIce = m_pUnlockLayer->getChildByName(ostr2.str());
            if (pIce) {
                pIce->setTag(i);
                m_Ices.pushBack(pIce);
            }
            std::stringstream ostr3;
            ostr3<<"snow_"<<i;
            Node* pPos = m_pUnlockLayer->getChildByName(ostr3.str());
            if (pPos) {
                pPos->setTag(i);
                pPos->setVisible(false);
                m_SnowPos.pushBack(pPos);
            }
            
            std::stringstream ostr4;
            ostr4<<"ice_"<<i;
            Node* pPos2 = m_pUnlockLayer->getChildByName(ostr4.str());
            if (pPos2) {
                pPos2->setTag(i);
                pPos2->setVisible(false);
                m_IcePos.pushBack(pPos2);
            }
            
            
            
            std::stringstream ostr5;
            ostr5<<"crushed_"<<i;
            Node* pCrush = m_pUnlockLayer->getChildByName(ostr5.str());
            if (pCrush) {
                pCrush->setTag(i);
                pCrush->setVisible(false);
                m_CrushedIces.pushBack(pCrush);
            }
            
            
            
            std::stringstream ostr6;
            ostr6<<"arrow_1_"<<i;
            Node* pIceArrow = m_pUnlockLayer->getChildByName(ostr6.str());
            if (pIceArrow) {
                pIceArrow->setTag(i);
                pIceArrow->setVisible(false);
                pIceArrow->setLocalZOrder(11);
                m_IceArrows.pushBack(pIceArrow);
            }
            std::stringstream ostr7;
            ostr7<<"arrow_0_"<<i;
            Node* pSnowArrow = m_pUnlockLayer->getChildByName(ostr7.str());
            if (pSnowArrow) {
                pSnowArrow->setTag(i);
                pSnowArrow->setLocalZOrder(11);
                pSnowArrow->setVisible(false);
                m_SnowArrows.pushBack(pSnowArrow);
            }
        }

    }
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    return true;
}

void ColorIceScene::onEnter()
{
    ExtensionScene::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_FLAVOR);
}

void ColorIceScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_FLAVOR);
    
    ExtensionScene::onExit();
}
void ColorIceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)){
        Vector<Node*> Children = m_pBottleList->getInnerContainer()->getChildren();
        for_each(Children.begin(), Children.end(), [=](Node* pBall){
            Node* pLock = pBall->getChildByName("lock");
            if (pLock) {
                pLock->setVisible(false);
            }
        });
        
    }
}


RenderTexture* ColorIceScene::getResultRender()
{
    Size size = m_pConeShape->getContentSize();
    RenderTexture* render = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 pos = m_pConeShape->getPosition();
    m_pConeShape->setPosition(Vec2(size.width/2, size.height/2));
    m_pSnow->setPosition(Vec2(size.width/2, size.height/2));
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pConeShape->visit();
    m_pSnow->visit();
    render->end();
    m_pConeShape->setPosition(pos);
    m_pSnow->setPosition(pos);
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}

void ColorIceScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        
        
        Image* pImage = getResultRender()->newImage();
        
        bool issuccess;
        
        issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"snow.png", false);
        pImage->autorelease();
        GameDataManager::getInstance()->m_bColored = true;
        log("===save success %d==",issuccess);

    }else if (btn->getTag()==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
//        Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//        dialog->setContentText("Are you sure you want to reset your Snow Cone flavor?");
//        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
//        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
//        dialog->onPositiveClick = [=](void*){
//            btn->setEnabled(true);
//            _resetColor();
//        };
//        dialog->onNegativeClick = [=](void*){
//            btn->setEnabled(true);
//            
//        };
//        btn->setEnabled(true);
        _resetColor();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    ExtensionScene::onButtonCallback(btn);
}
void ColorIceScene::onBottleClicked(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        ImageView* p = dynamic_cast<ImageView*>(ref);
        if (p) {
            p->setTouchEnabled(false);
            p->runAction(Sequence::create(DelayTime::create(1),
                                            CallFunc::create([=]()
                                                             {
                                                                 p->setVisible(false);
                                                             }), NULL));
            _showUnlockLayer();
            m_pTip->showTip("content/common/tip/lgtf.png");
        }
    }
}
void ColorIceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    pDragNode->stopAllActions();
    if ("bottle"==pDragNode->getName()){
        m_pGuideLayer->removeGuide();
        CCImageColorSpace cs;
        Sprite* pColor;
        std::stringstream ostr;
        m_nFlavorIndex = pDragNode->getTag();
        if (m_nFlavorIndex==0) {
            ostr<<"shape"<<m_nShapeIndex<<"_1.png";
            pColor = Sprite::create(localPath(ostr.str()));
        }else if (m_nFlavorIndex==5) {
            ostr<<"shape"<<m_nShapeIndex<<"_6.png";
            pColor = Sprite::create(localPath(ostr.str()));
        }else{
            ostr<<"shape"<<m_nShapeIndex<<"_1.png";
            Vec3 hsb = gHSB[m_nFlavorIndex];
            Texture2D* texture = cs.textureWithHSB(localPath(ostr.str()), hsb.x, hsb.y, hsb.z);
            pColor = Sprite::createWithTexture(texture);
        }
//        pColor->setAnchorPoint(Vec2::ZERO);
        pColor->setPosition(pColor->getContentSize()*0.5);
        m_pSnow->useTarget(pColor);
    }else{
        if (pDragNode->getTag()==0) {
            for_each(m_SnowArrows.begin(), m_SnowArrows.end(), [=](Node* pNode){
                pNode->setVisible(true);
            });
            if(!m_bGuide[0]){
                m_bGuide[0] = true;
                auto action = CSLoader::createTimeline("UnlockLayer.csb", m_pUnlockLayer);
                action->setTimeSpeed(0.2);
                action->play("animation0", true);
                m_pUnlockLayer->runAction(action);
                AudioHelp::getInstance()->playEffect("swipe_snow.mp3");
            }
            AudioHelp::getInstance()->playLoopEffect("swipe.mp3");
        }else{
            for_each(m_IceArrows.begin(), m_IceArrows.end(), [=](Node* pNode){
                pNode->setVisible(true);
            });
            if(!m_bGuide[1]){
                m_bGuide[1] = true;
                auto action = CSLoader::createTimeline("UnlockLayer.csb", m_pUnlockLayer);
                action->setTimeSpeed(0.2);
                action->play("animation1", true);
                m_pUnlockLayer->runAction(action);
            }
        }
    }
}

void ColorIceScene::dragNodeTouchMoved(DragNode* pDagNode,Point worldPoint)
{
    int tag = pDagNode->getTag();
    if ("bottle"==pDagNode->getName()) {
        if (pDagNode->isMoving()) {
            if (!IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap && tag>=5){
                pDagNode->setTouchEnabled(false);
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                this->runAction(Sequence::create(DelayTime::create(0.5),
                                                 CallFunc::create([=]()
                                                                  {
                                                                      pDagNode->setTouchEnabled(true);
                                                                  }), NULL));
                return;
            }
            m_pBottleList->setTouchEnabled(false);
            if (pDagNode->getRotation()>-0.1) {
                if (pDagNode->getPositionY()<visibleSize.height*0.5) {
                    pDagNode->setRotation(-0.1);
                    pDagNode->runAction(Sequence::create(RotateBy::create(0.3, -150),
                                                         CallFunc::create([=]()
                                                                          {
                                                                              int tag = pDagNode->getTag();
                                                                              std::stringstream ostr;
                                                                              ostr<<"cocos/color/bottles/"<<tag+1<<".png";
                                                                              pDagNode->getDragSprite()->setTexture(ostr.str());
                                                                              
                                                                          }), NULL));
                    schedule(schedule_selector(ColorIceScene::_playEffectYummy), 5);
                }
            }else{
                Rect rect = m_pConeShape->getBoundingBox();
                rect.origin = m_pConeShape->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    Vec2 pre = Vec2(pDagNode->getOffset(), worldPoint);
                    m_pSnow->paint(pre,worldPoint);
                    Director::getInstance()->getRenderer()->render();
                    if(!m_bResetShown){
                        m_bResetShown = true;
                        m_pGameUI->showResetLayout();
                    }
                }
            }
        }
    }else{
        if (tag==0) {
            for (int m = 0; m<3; m++) {
                std::stringstream ostr;
                ostr<<"snow"<<m;
                ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pUnlockLayer->getChildByName(ostr.str());
                if (pParticle) {
                    pParticle->setPosition(m_pUILayer->convertToNodeSpace(worldPoint));
                }
            }
            
            int count = m_Snows.size();
            for (int i = 0; i<count; i++) {
                Node* pNode = m_Snows.at(i);
                Node* pNodePos = m_SnowPos.at(i);
                Rect rect = pNodePos->getBoundingBox();
                rect.origin = pNodePos->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    std::stringstream ostr;
                    ostr<<"snow"<<pNode->getTag();
                    ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pUnlockLayer->getChildByName(ostr.str());
                    if (!pParticle) {
                        pParticle = ParticleSystemQuad::create("particle/snow2.plist");
                        m_pUnlockLayer->addChild(pParticle);
                        pParticle->setLocalZOrder(5);
                        pParticle->setName(ostr.str());
                    }
                    pParticle->setPosition(m_pUnlockLayer->convertToNodeSpace(worldPoint));
                    
                    
                    int opacity = pNode->getOpacity();
                    pNode->setOpacity(opacity-2);
                    
                    static bool isOn = false;
                    
                    for_each(m_SnowArrows.begin(), m_SnowArrows.end(), [=](Node* pArrow){
                        pArrow->setVisible(false);
                        pArrow->setOpacity(0);
                    });
                    Node* pArrow = m_SnowArrows.at(i);
                    pArrow->setVisible(false);
                    pArrow->setOpacity(0);
                    
                    if (opacity==125) {
                        
                        m_SnowArrows.eraseObject(pArrow);
                        m_SnowPos.eraseObject(pNodePos);
                        m_Snows.eraseObject(pNode);
                        int count = m_Snows.size();
                        pNode->runAction(Sequence::create(FadeTo::create(2,0),
                                                          CallFunc::create([=]()
                                                                           {
                                                                               std::stringstream ostr;
                                                                               ostr<<"snow"<<pNode->getTag();
                                                                               ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pUnlockLayer->getChildByName(ostr.str());
                                                                               if (pParticle){
                                                                                   pParticle->stopSystem();
                                                                                   pParticle->removeFromParent();
                                                                               }
                                                                               isOn = false;
                                                                               
                                                                               pArrow->setVisible(false);
                                                                               pArrow->setOpacity(0);
                                                                               
                                                                               AudioHelp::getInstance()->playEffect("work_right.mp3");
                                                                               
                                                                               if (count==0) {
                                                                                   pDagNode->ignoreMoving = true;
                                                                                   pDagNode->setTouchEnabled(false);
                                                                                   pDagNode->back(6,[=]()
                                                                                                  {
                                                                                                      _moveawayTool(tag);
                                                                                                      this->runAction(Sequence::create(DelayTime::create(1),
                                                                                                                                       CallFunc::create([=]()
                                                                                                                                                        {
                                                                                                                                                            _showTool(tag+1);
                                                                                                                                                            AudioHelp::getInstance()->playEffect("break_ice.mp3");
                                                                                                                                                   }), NULL));
                                                                                                  });
                                                                               }else{
                                                                                   pDagNode->setTouchEnabled(true);
                                                                                   pDagNode->ignoreMoving = false;
                                                                               }
                                                                           }), NULL));
                    }
                    
                    if (isOn) {
                        break;
                    }
                    isOn = true;
                    
                    
                    pDagNode->getDragSprite()->runAction(Sequence::create(Repeat::create(AnimationHelp::createAnimate("color/broom", 1, 2), 3),
                                                                          CallFunc::create([=]()
                                                                                           {
                                                                                           }), NULL));
                    break;
                }
            }
        }
    }
    
}

void ColorIceScene::dragNodeTouchEnded(DragNode* pDagNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    int tag = pDagNode->getTag();
    if ("bottle"==pDagNode->getName()) {
        unschedule(schedule_selector(ColorIceScene::_playEffectYummy));
        pDagNode->setTouchEnabled(false);
        m_pBottleList->setTouchEnabled(true);
        pDagNode->runAction(Sequence::create(RotateTo::create(0.3, 0),
                                             CallFunc::create([=]()
                                                              {
                                                                  int tag = pDagNode->getTag();
                                                                  std::stringstream ostr;
                                                                  ostr<<"cocos/color/bottles/icon/"<<tag+1<<".png";
                                                                  pDagNode->getDragSprite()->setTexture(ostr.str());
                                                              }), NULL));
        pDagNode->back(0,[=](){
            pDagNode->setTouchEnabled(true);
        });
    }else{
        if(m_bGuide[0]){
            for_each(m_SnowArrows.begin(), m_SnowArrows.end(), [=](Node* pArrow){
                pArrow->setVisible(true);
                pArrow->setOpacity(255);
            });
        }
        if(m_bGuide[1]){
            for_each(m_IceArrows.begin(), m_IceArrows.end(), [=](Node* pArrow){
                pArrow->setVisible(true);
                pArrow->setOpacity(255);
            });
        }
        for (int m = 0; m<3; m++) {
            std::stringstream ostr;
            ostr<<"snow"<<m;
            ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pUnlockLayer->getChildByName(ostr.str());
            if (pParticle){
                pParticle->stopSystem();
                pParticle->removeFromParent();
            }
        }
        if (tag==1) {
            int count = m_IcePos.size();
            for (int i = 0; i<count; i++) {
                Node* pNodePos = m_IcePos.at(i);
                int index = pNodePos->getTag();
                Sprite* pNode = (Sprite*)m_Ices.at(index);
                Rect rect = pNodePos->getBoundingBox();
                rect.origin = pNode->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    std::stringstream ostr;
                    ostr<<"color/frozen_bottle"<<pNode->getTag()<<"_";
                    
                    for_each(m_IceArrows.begin(), m_IceArrows.end(), [=](Node* pArrow){
                        pArrow->setVisible(false);
                        pArrow->setOpacity(0);
                    });
                    Node* pArrow = m_IceArrows.at(i);
                    pArrow->setVisible(false);
                    pArrow->setOpacity(0);
                    m_IceArrows.eraseObject(pArrow);
                    
                    pNode->runAction(Sequence::create(DelayTime::create(0.3),
                                                      AnimationHelp::createAnimate(ostr.str(), 0, 3,false,true,0.6),
                                                      DelayTime::create(0.8),
                                                      CallFunc::create([=]()
                                                                       {
                                                                           AudioHelp::getInstance()->playEffect("work_right.mp3");
                                                                           
                                                                           std::stringstream ostr;
                                                                           ostr<<"color/frozen_bottle"<<pNode->getTag()<<"_4.png";
                                                                           pNode->setTexture(ostr.str());
                                                                           ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/crush.plist");
                                                                           this->addChildToUILayer(pParticle);
                                                                           pParticle->setLocalZOrder(10);
                                                                           pParticle->setPosition(m_pUILayer->convertToNodeSpace(worldPoint));
                                                                           
                                                                           Node* pCrushedIce = m_CrushedIces.at(index);
                                                                           if (pCrushedIce) {
                                                                               pCrushedIce->setVisible(true);
                                                                           }
                                                                           
                                                                           if(m_bGuide[1]){
                                                                               for_each(m_IceArrows.begin(), m_IceArrows.end(), [=](Node* pArrow){
                                                                                   pArrow->setVisible(true);
                                                                                   pArrow->setOpacity(255);
                                                                               });
                                                                           }
                                                                           if (count==1) {
                                                                               //finished
                                                                               
                                                                               pDagNode->back(6,[=]()
                                                                                              {
                                                                                                  _moveawayTool(tag);
                                                                                                  this->runAction(Sequence::create(DelayTime::create(1),
                                                                                                                                   CallFunc::create([=]()
                                                                                                                                                    {
                                                                                                                                                        _moveawayUnlockLayer();
                                                                                            
                                                                                                                                                        GameDataManager::getInstance()->setFlavorLocked(false);
                                                                                                                                                        _showColorBottle(false);
                                                                                                                                                        _showSnowCone();
                                                                                                                                                    }), NULL));
                                                                                              });
                                                                           }else{
                                                                               pDagNode->setTouchEnabled(true);
                                                                           }
                                                                       }), NULL));
                    
                    pDagNode->setTouchEnabled(false);
                    
                    pDagNode->runAction(Repeat::create(Sequence::create(Spawn::create(RotateBy::create(0.3, 50),
                                                                                      MoveBy::create(0.3, Vec2(10, 60)), NULL),
                                                                        Spawn::create(RotateBy::create(0.3, -50),
                                                                                      MoveBy::create(0.3, Vec2(-10, -60)), NULL),
                                                                        CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("hammer.mp3");
                    }),
                                                                        DelayTime::create(0.3), NULL), 4));
                    m_IcePos.eraseObject(pNodePos);
                    break;
                }
            }
            
        }

    }
}

void ColorIceScene::dragNodeClicked(DragNode* dragNode,Point worldPoint)
{
    dragNodeTouchEnded(dragNode,worldPoint);
    int tag = dragNode->getTag();
    if ("bottle"==dragNode->getName()) {
        if (!IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap && tag>=5){
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
        }
    }else{
        if(m_bGuide[0]){
            for_each(m_SnowArrows.begin(), m_SnowArrows.end(), [=](Node* pArrow){
                pArrow->setVisible(true);
                pArrow->setOpacity(255);
            });
        }
        if(m_bGuide[1]){
            for_each(m_IceArrows.begin(), m_IceArrows.end(), [=](Node* pArrow){
                pArrow->setVisible(true);
                pArrow->setOpacity(255);
            });
        }
        
    }
}

#pragma mark - initData
void ColorIceScene::_initData()
{
    setExPath("content/make/color/");
    m_nNextSceneTag = GameUIEvent::eSceneTagMap;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagMap;
    
    m_nShapeIndex = GameDataManager::getInstance()->getIceShape();
    m_bResetShown = false;
    std::memset(m_bGuide, 0, 2);
}


void ColorIceScene::_showColorBottle(bool unlock)
{
    m_pBottleList = ExpandScrollView::create();
    m_pBottleList->setContentSize(Size(visibleSize.width, 200));
    CMVisibleRect::setPositionAdapted(m_pBottleList, visibleSize.width, 600+90,kBorderNone,kBorderTop);
    this->addChildToUILayer(m_pBottleList);
    m_pBottleList->setClippingEnabled(false);
    m_pBottleList->setTouchEnabled(false);
    m_pBottleList->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pBottleList->setScrollBarEnabled(false);
    
    float delt = 0.1;
    if (!unlock){
        delt = 1;
    }
    m_pBottleList->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                              DelayTime::create(delt),
                                              CallFunc::create([=]()
                                                               {
                                                                   if (!unlock) {
                                                                       _showAllBottleAnimation();
                                                                       this->runAction(Sequence::create(DelayTime::create(1),
                                                                                                        CallFunc::create([=](){
                                                                           
                                                                           m_pBottleList->scrollToRight(3, true);
                                                                       }), NULL));
                                                                   }else{
                                                                       m_pBottleList->scrollToRight(3, true);
                                                                   }
                                                               }),
                                              DelayTime::create(3),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pBottleList->scrollToLeft(1, true);
                                                               }),
                                              DelayTime::create(1),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.8), visibleSize*0.5);
                                                                   m_pBottleList->setTouchEnabled(true);
                                                               }),
                                              NULL));
    
    int count = 10;
    for (int i = 0; i<10; i++) {
        std::stringstream ostr;
        ostr<<"cocos/color/bottles/icon/"<<i+1<<".png";
        DragNode* pBottle = _createDrageNode(ostr.str());
        pBottle->setPosition(Vec2(50+150*i, 100));
        pBottle->setTag(i);
        pBottle->setCanSwallow(false);
        pBottle->setDirection(DragNode::Direction::VERTICAL);
        pBottle->setName("bottle");
        pBottle->setActionPoint(Vec2(45, 347));
        pBottle->setOrgPositionDefault();
        m_pBottleList->getInnerContainer()->addChild(pBottle);
        if (!unlock) {
            pBottle->setVisible(false);
        }
        pBottle->setScale(0.8);
        if (!IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap && i>=5) {
            Sprite* pLock = Sprite::create("content/common/lock.png");
            pBottle->addChild(pLock,5);
            pLock->setPosition(Vec2(30, -140));
            pLock->setName("lock");
            if (!unlock) {
                pLock->setVisible(false);
            }
        }
    }
    m_pBottleList->getInnerContainer()->setContentSize(Size(50+150*count, 200));
    
}

void ColorIceScene::_showAllBottleAnimation()
{
    Vector<Node*> pBottles = m_pBottleList->getInnerContainer()->getChildren();
    for_each(pBottles.begin(), pBottles.end(), [=](Node* pNode){
        DragNode* pBottle = dynamic_cast<DragNode*>(pNode);
        if (pBottle) {
            int tag = pBottle->getTag();
//            if (tag>2) {
                pBottle->runAction(Sequence::create(DelayTime::create(0.2*tag-0.2),
                                                    CallFunc::create([=]()
                                                                     {
                                                                         pBottle->setPosition(pBottle->getParent()->convertToNodeSpace(Vec2(visibleSize.width*0.5, visibleSize.height*0.3)));
                                                                         pBottle->setVisible(true);
                                                                         pBottle->setScale(0.5);
                                                                         AudioHelp::getInstance()->playEffect("syrup_up.mp3");
                                                                     }),
                                                    Spawn::create(MoveTo::create(0.5, pBottle->getOrgPosition()),
                                                                  ScaleTo::create(0.5, 0.8), NULL),
                                                    CallFunc::create([=]()
                                                                     {
                                                                         if (tag==pBottles.size()-1) {
                                                                             
                                                                             AudioHelp::getInstance()->playEffect("flavor_finish.mp3");
                                                                         }
                                                                         Node* pLock = pBottle->getChildByName("lock");
                                                                         if (pLock) {
                                                                             pLock->setVisible(true);
                                                                         }
                                                                     }), NULL));
//                pBottle->back();
//            }
        }
    });
}
void ColorIceScene::_showSnowCone()
{
    Sprite* pCone = Sprite::create(localPath("color_cone0.png"));
    this->addChildToContentLayer(pCone);
    CMVisibleRect::setPosition(pCone, 320+2, 40+90-visibleSize.height);
    
    Sprite* pConeFront = Sprite::create(localPath("color_cone1.png"));
    this->addChildToContentLayer(pConeFront);
    pConeFront->setLocalZOrder(1);
    CMVisibleRect::setPosition(pConeFront, 320+2, 40+90-visibleSize.height);
    
    std::stringstream ostr;
    ostr<<"shape"<<m_nShapeIndex<<"_0.png";
    m_pConeShape = Sprite::create(localPath(ostr.str()));
    this->addChildToContentLayer(m_pConeShape);
    CMVisibleRect::setPosition(m_pConeShape, 320, 320+90-visibleSize.height);
    
    
    m_pSnow = ScribbleNode::create(m_pConeShape->getContentSize().width, m_pConeShape->getContentSize().height);
    m_pSnow->useBrush(localPath("brush.png"),Scribble::BrushType::eBrush);
    m_pSnow->antiAliasing();
    
    this->addChildToContentLayer(m_pSnow);
    CMVisibleRect::setPosition(m_pSnow, 320, 320+90);
    
//    pTempRT = MyRenderTexture::create(m_pConeShape->getContentSize().width, m_pConeShape->getContentSize().height);
//    CMVisibleRect::setPosition(pTempRT, 320, 320);
//    pTempRT->retain();
    
    m_pConeShape->runAction(Sequence::create(DelayTime::create(3),
                                             MoveBy::create(0.8, Vec2(0, visibleSize.height)), NULL));
    pCone->runAction(Sequence::create(DelayTime::create(3),
                                      MoveBy::create(0.8, Vec2(0, visibleSize.height)), NULL));
    pConeFront->runAction(Sequence::create(DelayTime::create(3),
                                      MoveBy::create(0.8, Vec2(0, visibleSize.height)), NULL));
    
    m_pGameUI->showNextLayout();
    GameDataManager::getInstance()->setStepCount(4);
    
    AudioHelp::getInstance()->playEffect("finish_particle.mp3");
    int radoom = arc4random()%100;
    if (radoom>50) {
        AudioHelp::getInstance()->playEffect("fantastic.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("wonderful.mp3");
    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
}

void ColorIceScene::_showUnlockLayer()
{
    m_pUnlockLayer->setPosition(Vec2(0, visibleSize.height));
    m_pUnlockLayer->setVisible(true);
    m_pUnlockLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                               EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                               CallFunc::create([=]()
                                                                {
                                                                    _showTool(0);
//                                                                    auto action = CSLoader::createTimeline("UnlockLayer.csb", m_pUnlockLayer);                                           action->setTimeSpeed(0.2);  
//                                                                    action->play("animation0", true);
//                                                                    m_pUnlockLayer->runAction(action);
                                                                }), NULL));
}

void ColorIceScene::_moveawayUnlockLayer()
{
    int m = m_Ices.size();
    for (int i = 0; i<m; i++) {
        Node* pNode = m_Ices.at(i);
        pNode->setVisible(false);
        
        std::stringstream ostr;
        ostr<<"color/bottles/icon/"<<i+1<<".png";
        Sprite* pIce = Sprite::create(ostr.str());
        this->addChildToContentLayer(pIce);
        pIce->setPosition(m_pContentLayer->convertToNodeSpace(pNode->getParent()->convertToWorldSpace(pNode->getPosition())));
        
        Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(50+150*i, 700));
        pIce->runAction(Sequence::create(JumpBy::create(0.5, Vec2::ZERO, 100, 2),
                                         ScaleTo::create(0.3, 1,0.9),
                                         RotateBy::create(0.5, 30),
                                         Spawn::create(ScaleTo::create(0.3, 0.5),
                                                       RotateBy::create(0.3, -30), NULL),
                                         MoveTo::create(0.5, Vec2(visibleSize.width*0.5, visibleSize.height*0.3)),
                                         CallFunc::create([=]()
                                                          {
                                                              pIce->setVisible(false);
                                                          }), NULL));
    }
    m_pUnlockLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                               EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))), NULL));
}

void ColorIceScene::_showTool(int index)
{
    if(index == 0){
        AudioHelp::getInstance()->playEffect("have_tools.mp3");
    }
    DragNode* pTool = m_ToolBox.at(index);
    
    m_pBoxBack->setVisible(true);
    m_pBoxFront->setVisible(true);
    m_pBoxBack->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.6, Vec2(-visibleSize.width, 0))),
                                            Spawn::create(JumpBy::create(0.5, Vec2::ZERO, 50, 1),
                                                          Sequence::create(ScaleTo::create(0.3, 1,1.1),
                                                                           ScaleTo::create(0.2, 0.9,1),
                                                                           ScaleTo::create(0.2, 1), NULL), NULL), NULL));
    
    m_pBoxFront->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.6, Vec2(-visibleSize.width, 0))),
                                            Spawn::create(JumpBy::create(0.5, Vec2::ZERO, 50, 1),
                                                          Sequence::create(ScaleTo::create(0.3, 1,1.1),
                                                                           ScaleTo::create(0.2, 0.9,1),
                                                                           ScaleTo::create(0.2, 1), NULL), NULL), NULL));
    pTool->setTouchEnabled(false);
    pTool->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.6, Vec2(-visibleSize.width, 0))),
                                            Spawn::create(JumpBy::create(0.5, Vec2::ZERO, 50, 1),
                                                          Sequence::create(ScaleTo::create(0.3, 1,1.1),
                                                                           ScaleTo::create(0.2, 0.9,1),
                                                                           ScaleTo::create(0.2, 1), NULL), NULL),
                                      CallFunc::create([=]()
                                                       {
                                                           pTool->setOrgPositionDefault();
                                                           this->runAction(Sequence::create(DelayTime::create(1),
                                                                                            CallFunc::create([=](){
                                                               pTool->setTouchEnabled(true);
                                                           }), NULL));
                                                           pTool->runAction(RepeatForever::create(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 50, 2),
                                                                                                                                 Sequence::create(ScaleTo::create(0.2, 1,1.1),
                                                                                                                                                  ScaleTo::create(0.2, 0.9,1),                                                                                                                                                  ScaleTo::create(0.1, 1), NULL), NULL),
                                                                                                                   DelayTime::create(2), NULL)));
                                                       }), NULL));
}
void ColorIceScene::_moveawayTool(int index)
{
    DragNode* pTool = m_ToolBox.at(index);
    pTool->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    
    m_pBoxBack->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    m_pBoxFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    
}

void ColorIceScene::_resetColor()
{
    if(m_bResetShown){
        m_bResetShown = false;
        m_pGameUI->hideReset();
    }
    m_pSnow->resetCanvas();
}

void ColorIceScene::_playEffectYummy(float)
{
    int radom = arc4random()%100;
    if (radom<100&& radom>85){
        AudioHelp::getInstance()->playEffect("tasty.mp3");
    }else if (radom>70) {
        AudioHelp::getInstance()->playEffect("yummy.mp3");
    }
}