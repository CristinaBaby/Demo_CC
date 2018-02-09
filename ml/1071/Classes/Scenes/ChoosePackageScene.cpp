
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
    
    Sprite* pBg = Sprite::create(localPath("start_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    HomeCart* pCart = HomeCart::create();
    this->addChildToContentLayer(pCart);
    pCart->setPosition(Vec2(visibleSize.width/2+50, visibleSize.height/2));
    
    pCart->setLocalZOrder(1);
    pCart->runCar();
    pCart->setName("cart");
    
    Sprite* pMask = Sprite::create(localPath("mask.png"));
    this->addChildToUILayer(pMask);
    pMask->setPosition(visibleSize*0.5);
    pMask->setScale(1.2, 1.5);
    pMask->setVisible(false);
    
    
    Sprite* pFrame = Sprite::create(localPath("choose_frame.png"));
    this->addChildToUILayer(pFrame);
    pFrame->setAnchorPoint(Vec2(0, 0.2));
    CMVisibleRect::setPosition(pFrame, 80, 600);
    pFrame->setVisible(false);
    pFrame->setScale(0.1);
    
    Button* pItem0 = Button::create(localPath("btn_redog.png"));
    pFrame->addChild(pItem0);
    pItem0->setTag(0);
    pItem0->setPosition(Vec2(150, 300));
    
    Button* pItem1 = Button::create(localPath("btn_jadog.png"));
    pFrame->addChild(pItem1);
    pItem1->setTag(1);
    pItem1->setPosition(Vec2(400, 300));
    pItem0->setScale(0.2);
    pItem1->setScale(0.2);
    pItem0->setVisible(false);
    pItem1->setVisible(false);
    pBg->runAction(Sequence::create(DelayTime::create(0.5),
                                      ScaleTo::create(1, 1), NULL));
    pCart->runAction(Sequence::create(DelayTime::create(0.5),
                                        ScaleTo::create(1, 1),
                                        CallFunc::create([=](){
        pMask->setVisible(true);
        pFrame->setVisible(true);
        pFrame->runAction(RotateBy::create(0.2, 360));
        pFrame->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("choose_pop.mp3");
        }), NULL));
        pItem0->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_choose_page.mp3");
            pItem0->setVisible(true);
        }),
                                           EaseBackOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            pItem0->setTouchEnabled(true);
        }), NULL));
        pItem1->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pItem1->setVisible(true);
        }),
                                           EaseBackOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            pItem1->setTouchEnabled(true);
        }), NULL));
    }), NULL));
    
    auto listener = [=](Ref* ref,Widget::TouchEventType type){
        if (type!=Widget::TouchEventType::ENDED) {
            return;
        }
        pItem0->setTouchEnabled(false);
        pItem1->setTouchEnabled(false);
        AudioHelp::getInstance()->playSelectedEffect();
        Button* pButton = (Button*)ref;
        Sprite* pOK = Sprite::create(localPath("chosen.png"));
        pFrame->addChild(pOK);
        pOK->setPosition(pButton->getPosition()+Vec2(70, -50));
        int tag = pButton->getTag();
        if (tag==1) {
            AudioHelp::getInstance()->playEffect("vo_choose_japanese.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_nice_choice.mp3");
        }
        pOK->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(100-200*tag, 0)), NULL));
        pButton->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(0.5, Vec2(100-200*tag, 0)),
                                            DelayTime::create(0.5),
                                            CallFunc::create([=](){
            pFrame->runAction(Sequence::create(ScaleTo::create(1, 0.1),
                                               CallFunc::create([=](){
                pFrame->setVisible(false);
                pMask->setVisible(false);
                pCart->runCar();
                GameDataManager::getInstance()->m_nPackage = tag;
                AudioHelp::getInstance()->playLoopEffect("driving.mp3");
                pCart->runAction(Sequence::create(MoveBy::create(3, Vec2(-visibleSize.width, -400)),
                                                  CallFunc::create([=](){
                    AudioHelp::getInstance()->stopLoopEffect();
                    AdsManager::getInstance()->showAds(kTypeCrosspromoAds);
                    if (tag==0) {
                        SceneManager::replaceTheScene<GrillSausageScene>();
                    }else{
                        SceneManager::replaceTheScene<MixFlourScene>();
                    }
                }), NULL));
            }), NULL));
        }), NULL));
        if (pButton==pItem0) {
            pItem1->setVisible(false);
        }else{
            pItem0->setVisible(false);
        }
    };
    pItem0->addTouchEventListener(listener);
    pItem1->addTouchEventListener(listener);
    
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    return true;
}

void ChoosePackageScene::onExit()
{
    ExtensionScene::onExit();
}
#pragma mark - initData
void ChoosePackageScene::_initData()
{
    
    setExPath("content/start/");
}

