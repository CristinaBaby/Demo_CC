//
//  KitchenLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/30.
//
//

#include "KitchenLayer.h"
#include "AudioHelp.h"
//REGIST_SECENE(KitchenLayer)
#include "GameData.h"
#include "IAPManager.h"
#include "AdsManager.h"

Vec2 kitchenTypePos[6]={Vec2(632,479),Vec2(1075,477),Vec2(1342,424),Vec2(1778,465),Vec2(1735,279),Vec2(2190 - 100,481)};

KitchenLayer::KitchenLayer()
{
    _type = KitchenType::kitchen_material;
    endFunc = nullptr;
}
KitchenLayer::~KitchenLayer()
{

}
Scene* KitchenLayer::createScene(KitchenType type)
{
    Scene* scene = Scene::create();
    KitchenLayer* layer = KitchenLayer::create(type);
    scene->addChild(layer);
    return scene;
}

void KitchenLayer::loading(KitchenType type,std::function<void()> func)
{
    auto _scene = Scene::create();
    auto _layer = KitchenLayer::create(type);
    _layer->endFunc = func;
    _scene->addChild(_layer);
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(_scene);
}
KitchenLayer* KitchenLayer::create(KitchenType type)
{
    KitchenLayer *pRet = new(std::nothrow) KitchenLayer();
    if (pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
bool KitchenLayer::init(KitchenType type)
{
    if (!Layer::init()) {
        return false;
    }

    _type = type;
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    m_pBg = Sprite::create("cocostudio/kitchen/kitchen_bg1.jpg");
    m_pBg->setAnchorPoint(Vec2(0,0.5));
    m_pBg->setPosition(Vec2(0,_visibleSize.height/2));
    this->addChild(m_pBg);

    auto bg = Sprite::create("cocostudio/kitchen/kitchen_bg2.jpg");
    bg->setAnchorPoint(Vec2(0,0.5));
    bg->setPosition(Vec2(m_pBg->getContentSize().width,m_pBg->getContentSize().height/2));
    m_pBg->addChild(bg);
    
    
    m_pDoor = Sprite::create("cocostudio/kitchen/door_close.png");
    m_pDoor->setAnchorPoint(Vec2(0.45,0.11));
    m_pDoor->setPosition(Vec2(977,269));
    bg->addChild(m_pDoor);
    
    if(_type != kitchen_material){
    
        Vec2 _typePos = kitchenTypePos[_type-1];
        
        float _offsetX = _visibleSize.width*0.5 -  1.0*m_pBg->convertToWorldSpace(_typePos).x;
        m_pBg->runAction(Sequence::create(MoveBy::create(0,Vec2(_offsetX,0)),nullptr));
    
    }
    
    
    
    return true;
}
void KitchenLayer::onEnter()
{
    Layer::onEnter();
    
//    GameDataInstance->setIsShowAcg(false);
    
    if (!GameDataInstance->getIsShowAcg()) {
        GameDataInstance->setIsShowAcg(true);
        
       
        
        PopAcgLayer::loading(this,[=](){
            
            if(this->getChildByTag(1001)){
                
                this->getChildByTag(1001)->removeFromParent();
                
            }
            
            this->runAction(Sequence::create(CallFunc::create([=](){
                
//                AudioHelp::getInstance()->playEffect("VoiceOver/Let's make more cookies.mp3");
                
            }),DelayTime::create(1.0),CallFunc::create([=](){
                
                bgAction();
                
            }), NULL));
            
            
            
        });
    
    }else{
    
        this->runAction(Sequence::create(CallFunc::create([=](){
            
//            AudioHelp::getInstance()->playEffect("VoiceOver/Let's make more cookies.mp3");
            
        }),DelayTime::create(1.0),CallFunc::create([=](){
            
            bgAction();
            
        }), NULL));
    
    }
}
void KitchenLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (IAPManager::getInstance()->isShowAds()) {
        
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
        
    }
    
}
void KitchenLayer::onExit()
{
    Layer::onExit();
}
void KitchenLayer::bgAction()
{
    Vec2 typePos = kitchenTypePos[_type];
    
    if (_type == kitchen_material) {
        
        string gamestate = UserDefault::getInstance()->getStringForKey(GameState);
        if (gamestate == GameStateThree) {
            
            
            float offsetX = _visibleSize.width*0.5 -  1.0*m_pBg->convertToWorldSpace(typePos).x;
            m_pBg->runAction(Sequence::create(MoveBy::create(5.0,Vec2(-_visibleSize.width,0)),MoveBy::create(2.0,Vec2(_visibleSize.width,0)),MoveBy::create(fabs(offsetX)*0.004,Vec2(offsetX,0)),CallFunc::create([=](){
                
                m_pBg->setAnchorPoint(Vec2(typePos.x/m_pBg->getContentSize().width,typePos.y/m_pBg->getContentSize().height));
                m_pBg->setPosition(m_pBg->getPosition() + Vec2(typePos.x,m_pBg->getContentSize().height*(typePos.y/m_pBg->getContentSize().height - 0.5)));
                
            }),ScaleTo::create(2.0,1.5),CallFunc::create([=](){
                
                if (endFunc) {
                    endFunc();
                }
                
                
            }), NULL));
            
            
            
            
        }else{
        
            float offsetX = _visibleSize.width*0.5 -  1.0*m_pBg->convertToWorldSpace(typePos).x;
            m_pBg->runAction(Sequence::create(MoveBy::create(fabs(offsetX)*0.004,Vec2(offsetX,0)),CallFunc::create([=](){
                
                m_pBg->setAnchorPoint(Vec2(typePos.x/m_pBg->getContentSize().width,typePos.y/m_pBg->getContentSize().height));
                m_pBg->setPosition(m_pBg->getPosition() + Vec2(typePos.x,m_pBg->getContentSize().height*(typePos.y/m_pBg->getContentSize().height - 0.5)));
                
            }),ScaleTo::create(2.0,1.5),CallFunc::create([=](){
                
                if (endFunc) {
                    endFunc();
                }
                
                
            }), NULL));
        
        }
        
       
    }else {
    
        float delaytime = 0.0;
        if (_type == kitchen_door)
            delaytime = 1.0;
          
        
        Vec2 _typePos = kitchenTypePos[_type-1];
        float offsetX = _visibleSize.width*0.5 -  1.0*m_pBg->convertToWorldSpace(typePos).x;
        float _offsetX = _visibleSize.width*0.5 -  1.0*m_pBg->convertToWorldSpace(_typePos).x;
        m_pBg->runAction(Sequence::create(MoveBy::create(fabs(offsetX)*0.004,Vec2(offsetX,0)),CallFunc::create([=](){
        
            if (_type == kitchen_door) {
                
                m_pDoor->setTexture("cocostudio/kitchen/door_open.png");
            
            }
        
        }),DelayTime::create(delaytime),CallFunc::create([=](){
            
            m_pBg->setAnchorPoint(Vec2(typePos.x/m_pBg->getContentSize().width,typePos.y/m_pBg->getContentSize().height));
            m_pBg->setPosition(m_pBg->getPosition() + Vec2(typePos.x,m_pBg->getContentSize().height*(typePos.y/m_pBg->getContentSize().height - 0.5)));
            
        }),ScaleTo::create(2.0,1.5),CallFunc::create([=](){
            
            
            
            
            if (endFunc) {
                endFunc();
            }
            
            
        }), NULL));

    
    
    }
}