//
//  MakeStepEatEat.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/18.
//
//

#include "MakeStepEat.h"
#include "MakeOtherScene.h"
#include "AblumLayer.h"
#include "HomeScene.h"
#include "SelectLayer.h"
#include "MakeStepDec.h"
REGIST_SECENE(MakeStepEat)


string btnPath[4]={"next","back","home","photo"};

Vec2 btnAddPos[4]={Vec2(500,500),Vec2(-500,500),Vec2(-500,-500),Vec2(500,-500)};
MakeStepEat::MakeStepEat()
{
    m_bIsReward = false;
    m_bIsEat = false;
    _tapNum = 0;
}
MakeStepEat::~MakeStepEat()
{
    
}
bool MakeStepEat::init()
{
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStepEat.csb");
    addChild(_studioLayer);
    
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    
    _studioLayer->getNodeByName<Button*>("next")->setVisible(false);
    
    __String* path = __String::create("cocostudio/dec/plate.png");
    
    if (GameDataInstance->getBowlIndex() == -1) {
        
        path = __String::create("cocostudio/dec/plate.png");
        
    }else{
        
        path = __String::createWithFormat("cocostudio/dec/plate/plate%d.png",GameDataInstance->getBowlIndex()
                                          );
    }
    m_pBowl->setTexture(path->getCString());

    m_pEatNode = EatSprite::create();
//    auto food = Sprite::create("cocostudio/dec/cookie_chocolate_cat.png");
    
    log("%s",GameDataInstance->getEatPath().c_str());
    auto food = Sprite::create(GameDataInstance->getEatPath());
    food->setPosition(m_pBowl->getContentSize()/2);
    food->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pEatNode->addSprite(food);
    m_pEatNode->CloneSprite();
    m_pEatNode->_eatMaskPath = "cocostudio/dec/eat_mask.png";
    m_pBowl->addChild(m_pEatNode);
    m_pEatNode->eatPercentageCallback = CC_CALLBACK_1(MakeStepEat::checkEatNodePercentage, this);
    
    m_pEatNode->_touchCallBack=[=](){
    
        _tapNum++;
        
        if(_tapNum == 3){
        
             AudioHelp::getInstance()->playEffect("VoiceOver/tasty.mp3");
        
        }
        
        if(_tapNum == 10){
            
            AudioHelp::getInstance()->playEffect("VoiceOver/yummy.mp3");
            
        }
    
    };
    
    _bowlPos = m_pBowl->getPosition();
    
    m_pBowl->setPosition(Vec2(-500,-500));
    
    for (int i = 0;i<4;i++) {
        
        auto btn = _studioLayer->getNodeByName(btnPath[i].c_str());
        
        
        
        if(i > 1){
            btnPos[i] = btn->getPosition() + Vec2(0,80);
        }else{
            btnPos[i]=btn->getPosition();
        }
        
        btn->setPosition(btn->getPosition()+btnAddPos[i]);
        
    }
    
    return true;
}
void MakeStepEat::onEnter()
{
    BaseLayer::onEnter();
    
    this->runAction(Sequence::create(CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("success.mp3");
        _studioLayer->playAction("btnAction");
    
    }),DelayTime::create(1.5),CallFunc::create([=](){
    
        ActionHelper::showBezier(m_pBowl, _bowlPos, ActionHelper::ShowDirection::show_from_left,0.1,[=](){
            
            for (int i = 0;i<4;i++) {
                
                auto btn = _studioLayer->getNodeByName(btnPath[i].c_str());
                log("---%f %f",btnPos[i].x,btnPos[i].y);
                btn->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(1.0,btnPos[i]),1.0),CallFunc::create([=](){
                    
                    if(i == 3){
                        
                        _studioLayer->playAction("tapeat");
                        AudioHelp::getInstance()->playEffect("VoiceOver/tap to enjoy it.mp3");
                        
                    }
                    
                }), NULL));
            }
        });

    }), NULL));
}
void MakeStepEat::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStepEat::onExit()
{
    BaseLayer::onExit();
    
}
void MakeStepEat::checkEatNodePercentage(float dt)
{
    log("---%f",dt);
    if(dt>0.8){
    
        m_bIsEat = true;
        m_pEatNode->setEnable(false);
        
        _studioLayer->getNodeByName<Button*>("next")->setVisible(true);
        
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        if (gameState == GameStateOne) {
            
            if (!m_bIsReward) {
                m_bIsReward = true;
                
                RewardLayer* layer = RewardLayer::create(RewardType::typeNewFlavor);
                layer->btnNextClick=[=](){
                    
                    //赠送一个新的口味
                    GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
                    GameDataInstance->setFlavor(FlavorTypeVanilla);
                    GameDataInstance->setGameType(GameTypeOther);
                    UserDefault::getInstance()->setStringForKey(GameState,GameStateTwo);
                    UserDefault::getInstance()->flush();
                    
                    KitchenLayer::loading(KitchenType::kitchen_material,[=](){
                        
                        Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeOtherScene::createScene()));
                        
                    });
                };
                layer->btnEatClick=[=](){
                    layer->removeFromParent();
                    m_pEatNode->reset();
                    m_pEatNode->setEnable(true);
                };
                this->addChild(layer,100);
            }else{
            
                auto eatAgin = _studioLayer->getNodeByName<Button*>("eat_again");
                eatAgin->setEnabled(false);
                eatAgin->runAction(Sequence::create(MoveBy::create(0,Vec2(0,1000)),CallFunc::create([=](){
                    
                    eatAgin->setVisible(true);
                    
                }),EaseElasticOut::create(MoveTo::create(1.0,Director::getInstance()->getVisibleSize()/2),1.0),CallFunc::create([=](){
                    
                    eatAgin->setEnabled(true);
                    
                }), NULL));

            }
         }else if(gameState == GameStateTwo){
            
            if (!m_bIsReward) {
                m_bIsReward = true;
            
                RewardLayer* layer = RewardLayer::create(RewardType::typeAllFlavor);
                layer->btnNextClick=[=](){
                    
                    //赠送一个新的口味
                    UserDefault::getInstance()->setStringForKey(GameState,GameStateThree);
                    UserDefault::getInstance()->flush();
                    
//                    KitchenLayer::loading(KitchenType::kitchen_material,[=](){
//                        
//                        
//                        
//                    });
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
                };
                layer->btnEatClick=[=](){
                    
                    layer->removeFromParent();
                    m_pEatNode->reset();
                    m_pEatNode->setEnable(true);
                    
                };
                this->addChild(layer,100);
            
            
            }else{
                auto eatAgin = _studioLayer->getNodeByName<Button*>("eat_again");
                eatAgin->setEnabled(false);
                eatAgin->runAction(Sequence::create(MoveBy::create(0,Vec2(0,1000)),CallFunc::create([=](){
                    
                    eatAgin->setVisible(true);
                    
                }),EaseElasticOut::create(MoveTo::create(1.0,Director::getInstance()->getVisibleSize()/2),1.0),CallFunc::create([=](){
                    
                    eatAgin->setEnabled(true);
                    
                }), NULL));
            }
        }else{
        
            
            FlowerLayer::Loading(this,[=](){
            
                auto eatAgin = _studioLayer->getNodeByName<Button*>("eat_again");
                eatAgin->setEnabled(false);
                eatAgin->runAction(Sequence::create(MoveBy::create(0,Vec2(0,1000)),CallFunc::create([=](){
                    
                    eatAgin->setVisible(true);
                    
                }),EaseElasticOut::create(MoveTo::create(1.0,Director::getInstance()->getVisibleSize()/2),1.0),CallFunc::create([=](){
                    
                    eatAgin->setEnabled(true);
                    
                }), NULL));
                
            
            });
        }
    }
}
void MakeStepEat::touchEnd(ui::Widget* widget)
{
    

    int tag = widget->getTag();
    widget->setEnabled(false);
    
    ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
        
        widget->setEnabled(true);
        
    }));
    
    if(tag == TAGButtonNext){
    AudioHelp::getInstance()->playEffect("button_general.mp3");
        if (!m_bIsEat) {
            
//            //第三种游戏状态，弹出框
//            Dialog* dialog= Dialog::create((void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL);
//            dialog -> setAnchorPoint(Vec2(0,0));
//            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
//            dialog->setContentText("Let's to eat the food!");
//            dialog->setPositiveBtnText("");
//            dialog->setNegativeBtnText("");
//            dialog->setCallback(this);
//            this->addChild(dialog, 9999);
//            return;
            
        }
        
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        if (gameState == GameStateOne) {
            
            //如果没有奖励，就奖励，
            //如果奖励，就去选择界面
            if (!m_bIsReward) {
                RewardLayer* layer = RewardLayer::create(RewardType::typeNewFlavor);
                layer->btnNextClick=[=](){
                    
                    //赠送一个新的口味
                    GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
                    GameDataInstance->setFlavor(FlavorTypeVanilla);
                    GameDataInstance->setGameType(GameTypeOther);
                    UserDefault::getInstance()->setStringForKey(GameState,GameStateTwo);
                    UserDefault::getInstance()->flush();
                    
                    KitchenLayer::loading(KitchenType::kitchen_material,[=](){
                        
                        Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeOtherScene::createScene()));
                        
                    });

                };
                layer->btnEatClick=[=](){
                    
                    layer->removeFromParent();
                    m_pEatNode->reset();
                    m_pEatNode->setEnable(true);
                    
                };
                this->addChild(layer,100);
            }
            //去选择界面
            else{
            
                //进入第二种游戏状态
                //赠送一个新的口味
                GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
                GameDataInstance->setFlavor(FlavorTypeVanilla);
                GameDataInstance->setGameType(GameTypeOther);
                UserDefault::getInstance()->setStringForKey(GameState,GameStateTwo);
                UserDefault::getInstance()->flush();
                
                KitchenLayer::loading(KitchenType::kitchen_material,[=](){
                    
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeOtherScene::createScene()));
                    
                });

            }
        }else if(gameState == GameStateTwo){
        
            if (!m_bIsReward) {
                RewardLayer* layer = RewardLayer::create(RewardType::typeAllFlavor);
                layer->btnNextClick=[=](){
                    
                    //赠送一个新的口味
                   // GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
                    
                    UserDefault::getInstance()->setStringForKey(GameState,GameStateThree);
                    UserDefault::getInstance()->flush();
                    
                    
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
//                    KitchenLayer::loading(KitchenType::kitchen_material,[=](){
//                        
//                        Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
//                        
//                    });
                };
                layer->btnEatClick=[=](){
                    
                    layer->removeFromParent();
                    m_pEatNode->reset();
                    m_pEatNode->setEnable(true);
                    
                };
                this->addChild(layer,100);
            }
            //去选择界面
            else{
                
                //进入第二种游戏状态
                //赠送一个新的口味
//                GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
                UserDefault::getInstance()->setStringForKey(GameState,GameStateThree);
                UserDefault::getInstance()->flush();
                
                Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
                    
                
            }
            
            
        
        
        }else if(gameState == GameStateThree){
            //第三种游戏状态，弹出框
//            Dialog* dialog= Dialog::create((void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL);
//            dialog -> setAnchorPoint(Vec2(0,0));
//            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
//            dialog->setContentText(ReturnHome);
//            dialog->setPositiveBtnText("");
//            dialog->setNegativeBtnText("");
//            dialog->setCallback(this);
//            this->addChild(dialog, 9999);
            
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
            
            
        }
    }else if(tag == TAGButtonHome){
        AudioHelp::getInstance()->playEffect("button_general.mp3");
         m_bIsShowReturn = true;
        Dialog* dialog= Dialog::create((void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL);
        dialog -> setAnchorPoint(Vec2(0,0));
        kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
        dialog->setContentText(ReturnHome);
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        this->addChild(dialog, 9999);
    
    }else if(tag ==TAGButtonPhoto){
        AudioHelp::getInstance()->playEffect("camera.mp3");
        RenderTexture* texture = RenderTexture::create(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        texture->retain();
        texture->begin();
        
        _studioLayer->getNodeByName<Sprite*>("bg")->visit();
        m_pBowl->visit();
        
        texture->end();
        Director::getInstance()->getRenderer()->render();
        texture->getSprite()->getTexture()->setAntiAliasTexParameters();
        
        AblumLayer* layer = AblumLayer::create();
        layer->setRender(texture);
        this->addChild(layer,100);
    
    }else if(tag == TAGButtonEatAgain){
        AudioHelp::getInstance()->playEffect("button_general.mp3");
        auto eatAgin = _studioLayer->getNodeByName<Button*>("eat_again");
        eatAgin->setEnabled(false);
        eatAgin->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(0,1000)),1.0),CallFunc::create([=](){
            
            eatAgin->setEnabled(false);
            
            m_pEatNode->setEnable(true);
            m_pEatNode->reset();
            
        }), NULL));
    }else if(tag == TAGButtonBack){
    
        Director::getInstance()->replaceScene(MakeStepDec::createScene());
    
    }
}
void MakeStepEat::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void MakeStepEat::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    size_t _lType = (size_t)type;
    if (_lType == kDialogReturnHome) {
        
        if(IAPManager::getInstance()->isShowAds()){
        
            Director::getInstance()->popToRootScene();
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            AdLoadingLayerBase::loadingDoneCallback=[=](){
            
                Director::getInstance()->runWithScene(HomeScene::createScene());
            };
            
        }else{
        
            Director::getInstance()->popToRootScene();
            Director::getInstance()->runWithScene(HomeScene::createScene());
            
        }
    }
}


