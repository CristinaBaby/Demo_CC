//
//  DecorationBaseLayer.cpp
//  BHUG1064
//
//  Created by maxiang on 6/6/16.
//
//

#include "DecorationBaseLayer.h"
#include "SSCIAPManager.h"
#include "Component/lock/LockUIManager.h"
#include "Component/lock/RewardManager.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"
#include "component/makeComponent/basicComponent/ComponentDecoration.h"
#include "component/makeComponent/basicComponent/ComponentScribblePercent.h"
#include "component/makeComponent/basicComponent/ComponentScribblePattern.h"
#include "SoundConfig.h"
#include "CMVisibleRect.h"
#include "ActionHelper.h"
#include "AdsLoadingScene.h"
#include "DialogLayer.h"
#include "SceneHeader.h"
#include "SSCMoreGameButton.h"
#include "MiniShopScene.h"

DecorationBaseLayer::~DecorationBaseLayer()
{
//    CC_SAFE_RELEASE_NULL(_selectBg);
}

bool DecorationBaseLayer::init()
{
    if (!MakeBaseScene::init())
        return false;
    
    SSCMoreGameButton* moreGameButton = SSCMoreGameButton::create(true);
    auto sizeMore = moreGameButton->getButtonSize();
    moreGameButton->setPosition(CMVisibleRect::getPosition(80+sizeMore.width/2,sizeMore.height/2-10,kBorderLeft,kBorderTop));
    moreGameButton->setClickCall([=](SSCMoreGameButton*){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    });
    addChild(moreGameButton, 999);
    
    auto _homeButton = Button::create("ui/tap/home_btn.png");
    _homeButton->setAnchorPoint(Vec2(0, 1));
    _homeButton->setPosition(CMVisibleRect::getPosition(10, 10,kBorderLeft,kBorderTop));
    _homeButton->setVisible(true);
    static bool s_clicked = false;
    _homeButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!s_clicked)
            {
                kAudioUtil->stopAllEffect();
                SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                
                //                AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                //                AdsLoadingScene::loadingDoneCallback = [=]
                //                {
                //                    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                //                };
                
                kAudioUtil->playEffect(kSoundNext);
                
                s_clicked=true;
                
                this->runAction(Sequence::create(DelayTime::create(0.2),
                                                 CallFunc::create([=]
                                                                  {
                                                                      
                                                                      s_clicked=false;
                                                                  }),
                                                 NULL));
                
            }
            
        }
    });
    addChild(_homeButton, 999);

    
    _decorationContainerLayer = Layer::create();
    addChildToContentLayer(_decorationContainerLayer, 2);
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(DecorationBaseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif
    

    return true;
}

void DecorationBaseLayer::saveEatSprite(const std::string& name, const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    auto renderTexture = RenderTexture::create(getContentSize().width, getContentSize().height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    for (auto decoration : _decorationsVector)
    {
        decoration->setFrameVisible(false);
    }
    
    renderTexture->begin();
    _decorationContainerLayer->visit();
    renderTexture->end();
    
    renderTexture->saveToFile(name, true, callback);
}

void DecorationBaseLayer::saveEatSprite(const std::string& name,
                                        const Rect& area,
                                        const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    auto renderTexture = RenderTexture::create(area.size.width, area.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    renderTexture->setKeepMatrix(true);
    
    Rect fullRect = Rect(0.0, 0.0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    Rect fullViewport = Rect(0.0, 0.0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
    
    for (auto decoration : _decorationsVector)
    {
        decoration->setFrameVisible(false);
    }
    
    renderTexture->setVirtualViewport(area.origin, fullRect, fullViewport);
    
    renderTexture->begin();
    _decorationContainerLayer->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    renderTexture->saveToFile(name, true, callback);
}

void DecorationBaseLayer::hideOrShowButtonAction()
{
    if (_isShowDecorationBar)
    {
        setDecorationBarVisible(false);
    }
    else
    {
        setDecorationBarVisible(true);
    }
}

void DecorationBaseLayer::setDecorationBarVisible(bool visible)
{
    if (_isAnimation || !_cycleScrollBg || !_showDecorationBarButton)
        return;
    
    if (visible)
    {
        _hideOrShowButton->loadTextureNormal(HideOrShowButtonDown);
        _isShowDecorationBar = true;
        
        _isAnimation = true;
        _cycleScrollBg->runAction(Sequence::create(MoveTo::create(0.3, Vec2(_cycleScrollBg->getPosition().x, _cycleScrollBg->getPosition().y + 160.0)), CallFunc::create([=](){
            
            _isAnimation = false;
        }), NULL));
    }
    else
    {
        _hideOrShowButton->loadTextureNormal(HideOrShowButtonUp);
        _isShowDecorationBar = false;
        
        _isAnimation = true;
        _cycleScrollBg->runAction(Sequence::create(MoveTo::create(0.3, Vec2(_cycleScrollBg->getPosition().x, _cycleScrollBg->getPosition().y - 160.0)), CallFunc::create([=](){
            
            _isAnimation = false;
        }), NULL));
    }
}

void DecorationBaseLayer::setupDecorationWithConfig(DecorationDecConfig config,
                                                    const std::function<void(Widget *item)>& itemClickCallback)
{
    CCASSERT(!_module.empty() &&
             !_iapIdentifier.empty(), "*****请初始化这些变量*****");
  
    clearPreviousDecoration();
    removeCycleScroll(false, [=](){
        
        this->createCycleScroll();
        
        auto adapter = ESUnitAdapter::create(config.iconFormat, config.begin, config.end);
        _scrollView->setItemAdapter(adapter);
        _scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
        _scrollView->setContentSize(Size(614-60,110));
        if (config.name == "straw")
        {
            _scrollView->setMarginX(80);
        }
        else
        {
            _scrollView->setMarginX(110);
        }
        _scrollView->setMarginY(110);
        _scrollView->loadScrollView();
        _scrollView->setAnchorPoint(Vec2(0.5, 0.5));
        _scrollView->loadScrollView();

        for (auto item : _scrollView->vec_Item)
        {
            item->setScale(0.8f);
            int index = item->getTag();
            auto key = kRewardManager->getItemKey(_module, config.name, index);
            item->setUserObject(__String::create(key));
            if(!LockUIManager::getInstance()->registerIAPLock(_iapIdentifier, config.name, index, item))
            {
                LockUIManager::getInstance()->registerLock(_module, config.name, index, item,Vec2(90,30));
            }
        }

        LockUIManager::getInstance()->updateLockStatus();
        
        adapter->unitTouchEnded = [=](ui::Widget* item)
        {
            bool hasPurchased = kIAPManager->isPackagePurchased(_iapIdentifier);
            if (!hasPurchased)
            {
                if (kIAPManager->isFree(config.name, item->getTag()))
                {
                    auto keyObject = item->getUserObject();
                    if(!keyObject)
                        return;
                    string key = ((__String*)(keyObject))->getCString();
                    if(kRewardManager->isLocked(key))
                    {
                        //如果能找到reward名字的lock，显示reward，否则显示iap
                        if (item->getChildByName(LOCK_NAME_REWARD))
                        {
                            static bool s_isClicked = false;
                            if (s_isClicked)
                            {
                                return;
                            }
                            s_isClicked = true;
                            this->runAction(Sequence::create(DelayTime::create(0.5f),
                                                             CallFunc::create([=]
                                                                              {
                                                                                  s_isClicked = false;
                                                                              }),
                                                             NULL));

                            auto node = MiniShopScene::create();
                            this->addChild(node,9999);
                            node->setPosition(CMVisibleRect::getPosition(0, 0));
                            node->showDecorationButton();
                            node->_onRewardButtonClicked = [=]
                            {
                                kRewardManager->showRewardAds(key);
                            };
                            node->_onShopButtonClicked = [=]
                            {
                                ShopScene* layer = ShopScene::create();
                                layer->setPosition(CMVisibleRect::getPosition(0, 0));
                                this->addChild(layer,9999);
                            };

                        }
                        return;
                    }
                }
            }
            
            markItemSelect(item);
            _selectWidget = item;

            if (itemClickCallback)
                itemClickCallback(item);
            kAudioUtil->playEffect(kSoundChoose);

        };
        
        this->showCycleScroll();
    });
}

void DecorationBaseLayer::clearPreviousDecoration()
{
    _selectWidget = nullptr;
    
    if (_clippingTouchComp)
    {
        _clippingTouchComp->getOwner()->removeComponent(_clippingTouchComp);
        _clippingTouchComp = nullptr;
    }
    
    if (_scribblePatternComp)
    {
        _scribblePatternComp->getOwner()->removeComponent(_scribblePatternComp);
        _scribblePatternComp = nullptr;
        
        _patternScribbleParentNode->removeChildByName("placeHolder");
        _patternScribbleParentNode = nullptr;
    }
    
    if (_scribbleTool)
    {
        _scribbleTool->removeFromParent();
        _scribbleTool = nullptr;
    }
}

void DecorationBaseLayer::showRotateAndZoomDecoration(DecorationDecConfig config)
{
    auto itemClickCallback = [=](Widget *item)
    {
        this->setDecorationBarVisible(false);

        auto decorationComp = ComponentDecoration::create();
        decorationComp->setFrame(DecorationFrameFile);
        decorationComp->setZoomButtonTexture(DecorationZoomFile);
        decorationComp->setDeleteButtonTexture(DecorationDeleteFile);
        decorationComp->setMoveArea(Rect(10.0, 10.0, getContentSize().width - 20.0, getContentSize().height - 20.0));
        
        auto file = StringUtils::format(config.fileFormat.c_str(), item->getTag());
        auto decoration = Sprite::create(file);
        decoration->addComponent(decorationComp);
        decoration->setPosition(getContentSize()/2);
        _decorationContainerLayer->addChild(decoration, config.zorder);
        decorationComp->start();
        
        decorationComp->onDeleteCallback = [=]()
        {
            auto iter = std::find(_decorationsVector.begin(), _decorationsVector.end(), decorationComp);
            if (iter != _decorationsVector.end())
            {
                _decorationsVector.erase(iter);
            }
        };
        _decorationsVector.push_back(decorationComp);
    };
    
    this->setupDecorationWithConfig(config, itemClickCallback);
}

void DecorationBaseLayer::showScribblePatternDecoration(DecorationDecConfig config,
                                                        Node *scribbleParent,
                                                        const std::function<void()>& finishCallback,
                                                        const std::function<std::string(int tag)>& getMaskCall,
                                                        bool isShowPlaceHolder)
{
    CCASSERT(scribbleParent, "父节点不能为空！");

    auto itemClickCallback = [=](Widget *item)
    {
        this->setDecorationBarVisible(false);
        
        std::string mask = "\0";
        if (getMaskCall)
            mask = getMaskCall(item->getTag());
        
        auto file = StringUtils::format(config.fileFormat.c_str(), item->getTag());
        if (isShowPlaceHolder)
        {
            _patternScribbleParentNode->removeChildByName("placeHolder");
            
            auto placeHolderSprite = Sprite::create(file);
            placeHolderSprite->setName("placeHolder");
            placeHolderSprite->setOpacity(80);
            _patternScribbleParentNode->addChild(placeHolderSprite, 1);
        }
            
        if (!_scribblePatternComp)
        {
            _scribblePatternComp = ComponentScribblePattern::create(mask);
            _scribblePatternComp->setBrush(ScribblePatterBrushFile);
            auto toolFile = std::string(ScribbleDragTool);
            if (!toolFile.empty())
            {
                _scribblePatternComp->setScribbleTool(toolFile, Vec2(getContentSize().width + 200.0, getContentSize().height/2 + 200.0), _contentLayer);
                _scribblePatternComp->showScribbleTool(Vec2(getContentSize().width/2 + 200.0, getContentSize().height/2 + 200.0));
            }
   
            _scribblePatternComp->touchBeganCall = [=](Touch* touch)
            {

            };
            _scribblePatternComp->touchMovedCall = [=](Touch* touch)
            {
//                SoundConfig::getInstance()->playLoopEffect(kSoundAddCream);
            };
            _scribblePatternComp->touchEndedCall = [=](Touch* touch)
            {
                SoundConfig::getInstance()->stopLoopEffect();
            };
            scribbleParent->addComponent(_scribblePatternComp);
            _scribblePatternComp->start();
            
            auto compScribbleComplete = ComponentScribblePercent::create();
            _scribblePatternComp->getScribbleNode()->addComponent(compScribbleComplete);
            compScribbleComplete->setScribbleTargetPath(file);
            compScribbleComplete->setIsScribbleBrushTypeEraser(false);
            
            compScribbleComplete->scribbleCompletePercentCall = [=](float percent)
            {
                if(percent > 0.9){
                    
                    if (finishCallback)
                        finishCallback();
                    
                    _scribblePatternComp->getScribbleNode()->removeComponent(compScribbleComplete);
                }
            };
            compScribbleComplete->start();
        }
        
        if (!mask.empty())
            _scribblePatternComp->setMask(mask);
        
        _scribblePatternComp->setPattern(Sprite::create(file));
    };
    
    this->setupDecorationWithConfig(config, itemClickCallback);
    _patternScribbleParentNode = scribbleParent;
}

void DecorationBaseLayer::showReplaceTextureDecoration(DecorationDecConfig config,
                                                       Sprite *replaceSprite,
                                                       const std::function<void(Widget *item)> callback,
                                                       bool moveAble)
{
    CCASSERT(replaceSprite, "\n *先添加需要替换的精灵* \n");
    
    auto itemClickCallback = [=](Widget *item)
    {
        this->setDecorationBarVisible(false);

        auto file = StringUtils::format(config.fileFormat.c_str(), item->getTag());
        replaceSprite->setTexture(file);
        
        if (callback)
            callback(item);
    };
    
    //如果可以移动，添加移动组件
    if (moveAble)
    {
        replaceSprite->removeComponent("DecorationMoveComp");
        
        auto moveComp = ComponentTouchMove::create();
        moveComp->setName("DecorationMoveComp");
        moveComp->setMoveArea(Rect(50.0, 50.0, Director::getInstance()->getRunningScene()->getContentSize().width - 100.0, Director::getInstance()->getRunningScene()->getContentSize().height - 100.0));
        replaceSprite->addComponent(moveComp);
    }
    
    this->setupDecorationWithConfig(config, itemClickCallback);
}

void DecorationBaseLayer::showClippingTextureDecoration(DecorationDecConfig config,
                                                        Node *clippingParent,
                                                        const std::string& mask)
{
    auto itemClickCallback = [=](Widget *item)
    {
        this->setDecorationBarVisible(false);
    };
    this->setupDecorationWithConfig(config, itemClickCallback);

    auto clipping = ClippingNode::create();
    clippingParent->addChild(clipping, config.zorder);
    
    auto stencil = Sprite::create(mask);
    stencil->setPosition(stencil->getContentSize()/2);
    clipping->setStencil(stencil);
    clipping->setAlphaThreshold(0.1);
    clipping->setContentSize(stencil->getContentSize());
    clipping->setPosition(Vec2(-stencil->getContentSize().width/2, -stencil->getContentSize().height/2));
    
    _clippingTouchComp = ComponentTouchMove::create();
    _clippingTouchComp->setIsCanMove(false);
    clipping->addComponent(_clippingTouchComp);
    _clippingTouchComp->start();
    _clippingTouchComp->touchClickedCall = [=](Touch* touch)
    {
        if (!_selectWidget)
            return;

        auto file = StringUtils::format(config.fileFormat.c_str(), _selectWidget->getTag());
        auto sprincle = Sprite::create(file);
        
        auto rotate = cocos2d::random(0.0f, 360.0f);
        sprincle->setRotation(rotate);
        sprincle->setPosition(clipping->convertTouchToNodeSpace(touch));
        clipping->addChild(sprincle, 10);
        sprincle->setScale(0.5);
        
        kAudioUtil->playEffect(kSoundAddWater);
    };
}

void DecorationBaseLayer::showCycleScroll()
{
//    _isAnimation = true;
//    _cycleScrollBg->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.8, CMVisibleRect::getPosition(320, 100,kBorderNone,kBorderBottom))),
//                                               CallFunc::create([=]()
//    {
//        auto call = [=]
//        {
//            for (auto w : _cycleScrollComp->getItems())
//            {
//                w->runAction(ActionHelper::createRollJelly());
//            }
//        };
//        
//        _cycleScrollComp->scrollBy(0.4f, Vec2(0, -400.0), call);
//        _isAnimation = false;
//        _isShowDecorationBar = true;
//    }), NULL));
    
    auto call = [=]
    {
        for (auto w : _scrollView->vec_Item)
        {
            ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.82f);
            ScaleTo* s2 = ScaleTo::create(0.2f,1.01f,0.93);
            ScaleTo* s3 = ScaleTo::create(0.2f,1.f,1.f);
//            w->runAction(Sequence::create(s1,s2,s3, nullptr));
//            w->runAction(ActionHelper::createRollJelly())
        }
    };
//    _cycleScrollComp->scrollBy(0.4f, Vec2(200, 0), call);
    _scrollView->show(nullptr);


}

void DecorationBaseLayer::createCycleScroll()
{
    removeCycleScroll(false, nullptr);
    
    _cycleScrollBg = Sprite::create(DecorationBgFile);
    _cycleScrollBg->setAnchorPoint(Vec2(0.5, 0));
    _cycleScrollBg->setPosition(CMVisibleRect::getPosition(320, 90,kBorderNone,kBorderBottom));
    addChild(_cycleScrollBg, 15);

    //隐藏装潢条button
    auto file = std::string(HideOrShowButtonDown);
    if (_showDecorationBarButton && !file.empty())
    {
        _hideOrShowButton = Button::create(file);
        _hideOrShowButton->setPosition(Vec2((_cycleScrollBg->getContentSize().width - getContentSize().width)/2 + getContentSize().width - _hideOrShowButton->getContentSize().width/2 - 200.0, _cycleScrollBg->getContentSize().height + _hideOrShowButton->getContentSize().height/2));
        _hideOrShowButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                this->hideOrShowButtonAction();
            }
        });
        _cycleScrollBg->addChild(_hideOrShowButton, 6);
    }
    
    auto scrollNode = Node::create();
    scrollNode->setPosition(Vec2(_cycleScrollBg->getContentSize().width/2, _cycleScrollBg->getContentSize().height/2));
    _cycleScrollBg->addChild(scrollNode, 5);

    _scrollView = ESScrollView::create();
    scrollNode->addChild(_scrollView);

    
//    _cycleScrollComp = ComponentCycleScroll::create();
//    _cycleScrollComp->setEnableMoveItem(false);
//    _cycleScrollComp->setCycleScrollSize(Size(_cycleScrollBg->getContentSize().width-30,_cycleScrollBg->getContentSize().height));
//    _cycleScrollComp->setCycleScrollDirection(ScrollView::Direction::HORIZONTAL);
//    _cycleScrollComp->setMargin(110+10);
//    scrollNode->addComponent(_cycleScrollComp);
}

void DecorationBaseLayer::removeCycleScroll(bool animation, const std::function<void()>& callback)
{
    if (_cycleScrollBg)
    {
        if (animation)
        {
            _isAnimation = true;
            _cycleScrollBg->runAction(Sequence::create(Spawn::create(MoveTo::create(0.3, Vec2(_cycleScrollBg->getPosition().x, -_cycleScrollBg->getContentSize().height)), FadeOut::create(0.3), NULL), CallFunc::create([=](){
                
                _cycleScrollBg->removeFromParent();
                _cycleScrollBg = nullptr;
                _isAnimation = false;
                _isShowDecorationBar = false;

                if (callback)
                    callback();
            }), NULL));
        }
        else
        {
            _cycleScrollBg->removeFromParent();
            _cycleScrollBg = nullptr;
            _isShowDecorationBar = false;

            if (callback)
                callback();
        }
    }
    else
    {
        if (callback)
            callback();
    }
}

void DecorationBaseLayer::markItemSelect(Widget *item)
{
    if (!_selectBg){
        _selectBg = Sprite::create(SelectBgFile);
        _selectBg->retain();
    }
    
    if (_selectBg->getParent())
        _selectBg->removeFromParent();
    _selectBg->setPosition(item->getContentSize()/2);
    item->addChild(_selectBg, -1);
}


void DecorationBaseLayer::ignorAllTouch(bool ignor)
{
    if (_cycleScrollComp)
        _cycleScrollComp->setIgnoreAllTouch(ignor);

}

void DecorationBaseLayer::showDecorationParticle(Vec2 pos,int zOder)
{
    kAudioUtil->playEffect(kSoundShow);

    auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
    particle->setPosition(pos);
    this->addChild(particle,zOder);

}

void DecorationBaseLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (Director::getInstance()->getRunningScene()->getChildByTag(129))
    {
        
        Director::getInstance()->getRunningScene()->removeChildByTag(129);
        return;
    }
    
    if (!AdsLoadingScene::s_enableBackClick)
    {
        return;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        DialogLayer::showWithMessage("Are you sure you want to \nreturn to the main menu? \nYour current progress will \nbe lost.", [=](){
            
            SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());

//            AdsLoadingScene::showLoading<AdsLoadingScene>(true);
//            AdsLoadingScene::loadingDoneCallback=[]
//            {
//                SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
//            };
            
            
        }, nullptr, Director::getInstance()->getRunningScene(), 40);
    }
    
}

void DecorationBaseLayer::initTypeButton()
{
    for (auto btn : _typeBtnVec)
    {
        btn->removeFromParent();
    }
    
    _typeBgVec.clear();
    for (int i = 0; i<_configVec.size(); i++)
    {
        auto bg = Sprite::create("ui/dec/icon_bg.png");
        this->addChild(bg,13);
        bg->setPosition(CMVisibleRect::getPosition(115+140*i,255,kBorderLeft,kBorderBottom));
        _typeBgVec.push_back(bg);
    }

    _typeBtnVec.clear();
    for (int i = 0; i<_configVec.size(); i++)
    {
        auto config = _configVec.at(i);
        auto btn = Button::create(config.typeUnselected);
        btn->loadTextureDisabled(config.typeSelected);
        btn->loadTexturePressed(config.typeUnselected);
        this->addChild(btn,14);
        btn->setPosition(CMVisibleRect::getPosition(115+140*i,255,kBorderLeft,kBorderBottom));
        btn->setAnchorPoint(Vec2(0.5f, 0.5f));
        btn->setTag(i);
        btn->setScale(0.9f);
        _typeBtnVec.push_back(btn);
        if (i==0)
        {
            _curBtn = btn;
            btn->setBright(false);
            _curTag = _curBtn->getTag();
            _curBtn->setZOrder(30);
            _curBtnBg = _typeBgVec.at(0);
            _curBtnBg->setTexture("ui/dec/icon_bg_up.png");
        }
        
        btn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type)
                                   {
                                       auto btn = dynamic_cast<Button*>(ref);
                                       
                                       if (type == Widget::TouchEventType::ENDED)
                                       {
                                           if (_curBtn->getTag() == btn->getTag())
                                           {
                                               return;
                                           }
                                           _curBtnBg->setTexture("ui/dec/icon_bg.png");
                                           _curBtn->setBright(true);
                                           _curBtn->setZOrder(14);
                                           _curBtn = btn;
                                           _curBtnBg = _typeBgVec.at(_curBtn->getTag());
                                           _curBtnBg->setTexture("ui/dec/icon_bg_up.png");
                                           btn->setBright(false);
                                           if(_typeBtnClicked)
                                           {
                                               _typeBtnClicked(btn);
                                           }
                                           _curBtn->setZOrder(30);
                                           _curTag = _curBtn->getTag();
                                       }
                                   });
        
    }
    

}

void DecorationBaseLayer::removeTypeButton()
{
    if (_typeBtnVec.size()<=0)
    {
        return;
    }
    for(auto btn :_typeBtnVec)
    {
        btn->removeFromParent();
    }
    _typeBtnVec.clear();
    
    for(auto bg :_typeBgVec)
    {
        bg->removeFromParent();
    }
    _typeBgVec.clear();

}

void DecorationBaseLayer::setTypeEnable(bool enable)
{
    for (auto btn : _typeBtnVec)
    {
        btn->setTouchEnabled(enable);
    }
    
    if (_scrollView)
    {
        _scrollView->setEnabled(enable);
    }
    _nextButton->setTouchEnabled(enable);
    _resetButton->setTouchEnabled(enable);
}


