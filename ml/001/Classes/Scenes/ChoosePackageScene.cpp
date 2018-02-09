
#include "ChoosePackageScene.h"
#include "SceneManager.h"

ChoosePackageScene::ChoosePackageScene()
{
    m_bDonutShowed = false;
    m_bTwistShowed = false;
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
    Sprite* pBg = Sprite::create(localPath("choose_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGirl = RoleModel::create();
    m_pGirl->setRole(localCocosPath("cooker0/"), "cooker0", 4);
    this->addChildToContentLayer(m_pGirl);
    m_pGirl->setScale(0.7);
    CMVisibleRect::setPositionAdapted(m_pGirl, 320, 260-visibleSize.height);
    m_pGirl->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_make.mp3");
        m_pGirl->playArmation("Animation1", false);
    }), NULL));
    m_pGirl->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (movementID=="Animation1"){
                static int k = 0;
                if (k%2==0) {
                    m_pGirl->playArmation("Animation4", false);
                    m_bDonutShowed = true;
                    _showDonutPackage();
                }else{
                    m_pGirl->playArmation("Animation3", false);
                    m_bTwistShowed = true;
                    _showTwistPackage();
                }
            }else if (movementID=="Animation3"){
                m_bTwistShowed = true;
                if (!m_bDonutShowed) {
                    m_pGirl->playArmation("Animation4", false);
                    _showDonutPackage();
                }else{
                    m_pGirl->playArmation("Animation2", true);
                }
                
            }else if (movementID=="Animation4"){
                m_bDonutShowed = true;
                if (!m_bTwistShowed) {
                    m_pGirl->playArmation("Animation3", false);
                    _showTwistPackage();
                }else{
                    m_pGirl->playArmation("Animation2", true);
                }
                
            }
        }
        
    };
    AudioHelp::getInstance()->playBackGroundMusic("happy.mp3");
    return true;
}

void ChoosePackageScene::onExit()
{
    ExtensionScene::onExit();
}
#pragma mark - initData
void ChoosePackageScene::_initData()
{
    
    setExPath("content/choose/");
}


void ChoosePackageScene::_showDonutPackage()
{
    _playEffect();
    m_pDonutBubble0 = Sprite::create(localPath("bubble0.png"));
    CMVisibleRect::setPositionAdapted(m_pDonutBubble0, 430, 580);
    this->addChildToContentLayer(m_pDonutBubble0);
    
    m_pDonutBubble1 = Sprite::create(localPath("bubble1.png"));
    CMVisibleRect::setPositionAdapted(m_pDonutBubble1, 450, 640);
    this->addChildToContentLayer(m_pDonutBubble1);
    
    m_pDonut = Button::create(localPath("bubble2_1.png"));
    CMVisibleRect::setPositionAdapted(m_pDonut, 490, 800);
    this->addChildToContentLayer(m_pDonut);
    m_pDonut->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    m_pDonut->setTag(0);
    
    m_pDonutBubble0->setOpacity(0);
    m_pDonutBubble1->setOpacity(0);
    m_pDonut->setVisible(false);
    m_pDonut->setTouchEnabled(false);
    m_pDonut->setScale(0.2);
    m_pDonutBubble0->runAction(Sequence::create(FadeIn::create(0.3), NULL));
    m_pDonutBubble1->runAction(Sequence::create(DelayTime::create(0.3),
                                                FadeIn::create(0.3), NULL));
    m_pDonut->runAction(Sequence::create(DelayTime::create(0.6),
                                         CallFunc::create([=](){
        m_pDonut->setVisible(true);
//        m_pDonut->setTouchEnabled(true);
    }),
                                         EaseBackOut::create(ScaleTo::create(2, 1)),
                                         CallFunc::create([=](){
        m_pDonut->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                   CallFunc::create([=](){
            m_pDonut->loadTextures(localPath("bubble2_2.png"), localPath("bubble2_2.png"));
        }),
                                                                   DelayTime::create(0.3),
                                                                   CallFunc::create([=](){
            m_pDonut->loadTextures(localPath("bubble2_1.png"), localPath("bubble2_1.png"));
            
        }), NULL)));
    }), NULL));
}

void ChoosePackageScene::_showTwistPackage()
{
    _playEffect();
    m_pTwistBubble0 = Sprite::create(localPath("bubble0.png"));
    CMVisibleRect::setPositionAdapted(m_pTwistBubble0, 185, 595);
    this->addChildToContentLayer(m_pTwistBubble0);
    
    m_pTwistBubble1 = Sprite::create(localPath("bubble1.png"));
    CMVisibleRect::setPositionAdapted(m_pTwistBubble1, 195, 650);
    this->addChildToContentLayer(m_pTwistBubble1);
    
    m_pTwist = Button::create(localPath("bubble3_1.png"));
    CMVisibleRect::setPositionAdapted(m_pTwist, 150, 790);
    this->addChildToContentLayer(m_pTwist);
    m_pTwist->addTouchEventListener(CC_CALLBACK_2(ChoosePackageScene::onButtonCallback, this));
    m_pTwist->setTag(1);
    
    m_pTwistBubble0->setOpacity(0);
    m_pTwistBubble1->setOpacity(0);
    m_pTwist->setVisible(false);
    m_pTwist->setTouchEnabled(false);
    m_pTwist->setScale(0.2);
    m_pTwistBubble0->runAction(Sequence::create(FadeIn::create(0.3), NULL));
    m_pTwistBubble1->runAction(Sequence::create(DelayTime::create(0.3),
                                                FadeIn::create(0.3), NULL));
    m_pTwist->runAction(Sequence::create(DelayTime::create(0.6),
                                         CallFunc::create([=](){
        m_pTwist->setVisible(true);
    }),
                                         EaseBackOut::create(ScaleTo::create(2, 1)),
                                         CallFunc::create([=](){
        m_pTwist->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                   CallFunc::create([=](){
            m_pTwist->loadTextures(localPath("bubble3_2.png"), localPath("bubble3_2.png"));
        }),
                                                                   DelayTime::create(0.3),
                                                                   CallFunc::create([=](){
            m_pTwist->loadTextures(localPath("bubble3_1.png"), localPath("bubble3_1.png"));
            
        }), NULL)));
    }),
                                         CallFunc::create([=](){
        m_pTwist->setTouchEnabled(true);
        m_pDonut->setTouchEnabled(true);
    }), NULL));
}

void ChoosePackageScene::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        Button* pButton = (Button*)ref;
        int tag = pButton->getTag();
        AudioHelp::getInstance()->playEffect("vo_hmm3.mp3");
        AudioHelp::getInstance()->playEffect("vo_nice_choice.mp3");
        if(m_pTwist){
            m_pTwist->setTouchEnabled(false);
        }
        if(m_pDonut){
            m_pDonut->setTouchEnabled(false);
        }
        GameDataManager::getInstance()->m_nPackage = tag;
        AudioHelp::getInstance()->playSelectedEffect();
        if (0==tag) {
//            donut
            m_pGirl->playArmation("Animation4", false);
            m_pTwist->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                               MoveBy::create(0.5, Vec2(50, -150)), NULL),
                                                 CallFunc::create([=](){
                m_pTwist->removeFromParent();
            }), NULL));
            
            m_pTwistBubble0->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                               MoveBy::create(0.5, Vec2(10, -50)), NULL),
                                                 CallFunc::create([=](){
                m_pTwistBubble0->removeFromParent();
            }), NULL));
            m_pTwistBubble1->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                               MoveBy::create(0.5, Vec2(50, -20)), NULL),
                                                 CallFunc::create([=](){
                m_pTwistBubble1->removeFromParent();
            }), NULL));
            
            m_pDonut->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5, 1.1)),
                                                 CallFunc::create([=](){
                m_pDonut->removeFromParent();
                m_pDonutBubble0->removeFromParent();
                m_pDonutBubble1->removeFromParent();
            }), NULL));
            
            Sprite* pBomb = Sprite::create(localPath("break_bubble.png"));
            this->addChildToContentLayer(pBomb);
            pBomb->setPosition(m_pDonut->getPosition());
            pBomb->setScale(0.2);
            pBomb->runAction(Sequence::create(EaseSineIn::create(ScaleTo::create(0.5, 1)),
                                              FadeOut::create(0.1),
                                              DelayTime::create(1),
                                              CallFunc::create([=](){
//                AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//                AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                    SceneManager::replaceTheScene<MixFlourScene>();
//                };
                
                AdsManager::getInstance()->showAds(kTypeInterstitialAds);
                SceneManager::replaceTheScene<MixFlourScene>();
                pBomb->removeFromParent();
            }), NULL));
        }else if (1==tag) {
            m_pGirl->playArmation("Animation3", false);
            m_pDonut->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                               MoveBy::create(0.5, Vec2(-50, -150)), NULL),
                                                 CallFunc::create([=](){
                m_pDonut->removeFromParent();
            }), NULL));
            
            m_pDonutBubble0->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                                      MoveBy::create(0.5, Vec2(-10, -50)), NULL),
                                                        CallFunc::create([=](){
                m_pDonutBubble0->removeFromParent();
            }), NULL));
            m_pDonutBubble1->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.2),
                                                                      MoveBy::create(0.5, Vec2(-50, -20)), NULL),
                                                        CallFunc::create([=](){
                m_pDonutBubble1->removeFromParent();
            }), NULL));
            
            m_pTwist->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5, 1.1)),
                                                 CallFunc::create([=](){
                m_pTwist->removeFromParent();
                m_pTwistBubble0->removeFromParent();
                m_pTwistBubble1->removeFromParent();
            }), NULL));
            Sprite* pBomb = Sprite::create(localPath("break_bubble.png"));
            this->addChildToContentLayer(pBomb);
            pBomb->setPosition(m_pTwist->getPosition());
            pBomb->setScale(0.2);
            pBomb->runAction(Sequence::create(EaseSineIn::create(ScaleTo::create(0.5, 1)),
                                              FadeOut::create(0.1),
                                              DelayTime::create(1),
                                              CallFunc::create([=](){
//                AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//                AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                    SceneManager::replaceTheScene<TwistRiseDoughScene>();
//                };
                
                AdsManager::getInstance()->showAds(kTypeInterstitialAds);
                SceneManager::replaceTheScene<TwistRiseDoughScene>();
                pBomb->removeFromParent();
            }), NULL));
        }
    }
}

void ChoosePackageScene::_playEffect()
{
    static int effect = arc4random()%2;
    effect = (effect+1)%2+1;
    std::string effectName = "vo_hmm"+std::to_string(effect)+".mp3";
    AudioHelp::getInstance()->playEffect(effectName);
}