//
//  PourMoldLayer.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/22.
//
//

#include "PourMoldLayer.h"
#include "OutMoldScene.h"
#include "FilePathUtil.h"
#include "AdapterScreen.h"
#include "VictoryLayer.h"
#include "ChangeLoading.h"

string pourMacPath[5]={"blueberry","kiwi","lemon","orange","strawberry"};

string pourMoldPath[6]={"bear","s","coke","fish","hambuger","heart"};

#define shadowTag 1000

PourMoldLayer::PourMoldLayer()
{
    m_fMoldPercentage = 0.0;
    m_iMoldTag = 0;
    m_bStartPour = false;
}
PourMoldLayer::~PourMoldLayer()
{

}
bool PourMoldLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    this->initData();
    this->initView();
    
    return true;
}
void PourMoldLayer::initData()
{
    m_vType.clear();
    for (int i = 0; i < 5; i++) {
        
        __String* str = __String::createWithFormat("images/make_mold/mold/pourmachine_%s.png",pourMacPath[i].c_str());
        Sprite* data = Sprite::create(str->getCString());
        
        m_vType.pushBack(data);
        
    }
}
void PourMoldLayer::initView()
{
    m_pMoldMacLayer = MoldMacLayer::create();
    this->addChild(m_pMoldMacLayer,10);
    m_pMoldMacLayer->ignoreAnchorPointForPosition(false);
    m_pMoldMacLayer->setAnchorPoint(Vec2(0,1));
    m_pMoldMacLayer->setPosition(Vec2(0,Director::getInstance()->getVisibleSize().height + 50));
    
    m_pMoldMacLayer->setPositionX(m_pMoldMacLayer->getPositionX()+1500);
    
    m_pMoldMacLayer->firstBtnDown=[=](){
    
        if(selectMoldHideCallback){
            selectMoldHideCallback();
        }
    
    };
    
    m_pMoldMacLayer->pushBtnDown=[=](int index){
        
        pushBtnDown(index);
        
    };

    m_pMoldMacLayer->pushBtnUp=CC_CALLBACK_0(PourMoldLayer::pushBtnUp, this);
    
    m_pNextBtn = MySpriteButton::create("images/button/public_btn_next.png",std::bind(&PourMoldLayer::nextBtn, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(m_pNextBtn, 10, 10,Vec2(1,1),kBorderRight,kBorderTop);
    m_pNextBtn->setOpacity(0);
    this->addChild(m_pNextBtn,20);
    m_pNextBtn->setEnable(false);
}
void PourMoldLayer::pushBtnDown(int index)
{
    if (m_fMoldPercentage<99.8 && g_Mold_Sprite) {
        
        m_iMoldTag++;
        Sprite* sp = Sprite::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s_%s.png",pourMoldPath[g_Mold_Sprite->getTag()].c_str(),pourMacPath[index].c_str())->getCString());
        auto mold = ProgressTimer::create(sp);
        mold->setType(ProgressTimer::Type::BAR);
        mold->setBarChangeRate(Point(0,1));
        mold->setMidpoint(Point(0.5,0));
        mold->setPosition(Vec2::ZERO);
        mold->setTag(m_iMoldTag);
        mold->setPercentage(m_fMoldPercentage);
        mold->setAnchorPoint(Vec2::ZERO);
        g_Mold_Sprite->addChild(mold,-(100 + m_iMoldTag));
    }
    
    this->scheduleOnce([=](float dt){
    
        m_bStartPour = true;
         this->schedule(schedule_selector(PourMoldLayer::updateMold),0.3f);
    
    },1.0,"mold");
    
   
}
void PourMoldLayer::pushBtnUp()
{
//    this->unschedule(schedule_selector(PourMoldLayer::updateMold));
     unscheduleAllCallbacks();
    m_bStartPour = false;
    if(g_Mold_Sprite&&g_Mold_Sprite->getChildByTag(m_iMoldTag)){
        
        m_fMoldPercentage = dynamic_cast<ProgressTimer*>(g_Mold_Sprite->getChildByTag(m_iMoldTag))->getPercentage();
        
        //结束之后，保存图片到本地
        if(m_fMoldPercentage>95){
            
            for (auto btn:m_vBtn) {
                
                
                btn->cancelEvent(true);
                
                
            }
            
            m_pMoldMacLayer->setIsOver();
            
            
            Vec2 pos = g_Mold_Sprite->getPosition();
            RenderTexture* texture = RenderTexture::create(g_Mold_Sprite->getContentSize().width, g_Mold_Sprite->getContentSize().height);
            texture->retain();
            texture->beginWithClear(0, 0,0,0);
            g_Mold_Sprite->setPosition(Vec2::ZERO);
            g_Mold_Sprite->setAnchorPoint(Vec2::ZERO);
            g_Mold_Sprite->setOpacity(0);
            g_Mold_Sprite->getChildByTag(shadowTag)->setOpacity(0);
            g_Mold_Sprite->visit();
            texture->end();
            
            Director::getInstance()->getRenderer()->render();
            texture->getSprite()->getTexture()->setAntiAliasTexParameters();
            
//            string path = FilePathUtil::newCachePath(GummyCaheDir);
            struct timeval tv;
            gettimeofday(&tv,NULL);
            long nowtime =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
            
            auto path = StringUtils::format("%s%ld.png","gummy.png",nowtime);
            bool success = texture->saveToFile(path.c_str());
            Director::getInstance()->getRenderer()->render();
            if (success) {
                log("----------->success!");
                g_Mold_Sprite->setOpacity(255);
                g_Mold_Sprite->getChildByTag(shadowTag)->setOpacity(255);
                g_Mold_Sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                g_Mold_Sprite->setPosition(pos);
                g_Mold_Sprite->setLocalZOrder(100);
                m_pMoldMacLayer->hideMacCenter();
                
                g_Mold_Sprite->runAction(Sequence::create(Spawn::create(ScaleTo::create(1.0,1.2),MoveBy::create(1.0, Vec2(0,100)),nullptr),CallFunc::create([=](){
                
                    
                    
                    showParticle(path);
                    
                }), NULL));
                
                //              g_Mold_Sprite->getChildByTag(shadowTag)->runAction(FadeIn::create(0));
                
                
                //                m_pNextBtn->runAction(FadeIn::create(0.5));
                
                
                
                
                
                //                vector<string> str;
                //                FilePathUtil::getFilesPathByDir(GummyCaheDir, str);
                //                string path = FileUtils::getInstance()->getWritablePath() + "image.png";
                //                auto sp = Sprite::create(str.at(0).c_str());
                //                sp->setPosition(Vec2(400,400));
                //                this->addChild(sp);
                
            }
        }
    }

}
void PourMoldLayer::showParticle(string path)
{
    AudioHelp::getInstance()->playEffect("cheer_star.mp3");
    for(int i=0;i<9;i++){
        
        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([=](){
            
            auto* effect = ParticleSystemQuad::create("particle/powerStar.plist");
            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
            this->addChild(effect,200);
            
            if(i == 8){
                
                g_Mold_Sprite->removeFromParent();
                VictoryLayer* vic = VictoryLayer::create(kVictoryGummy,path.c_str());
                this->addChild(vic,100);
                vic->btnNext=[=](){
                    
                    ChangeLoading::loading([](){
                        
                        
                        
                        Scene* scene = OutMoldScene::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0,scene));
                        
                    });
                    
                };
                vic->btnAgain=[=](){
                    
                    SceneChangeManager->enterMakeMoldScene();
                    
                };
                
            }
            
        }), nullptr));
    }

}
void PourMoldLayer::onEnter()
{
    Layer::onEnter();
}
void PourMoldLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void PourMoldLayer::onExit()
{
    Layer::onExit();
}
void PourMoldLayer::showPourMac()
{
    ActionUtils::moveByElasticOut(m_pMoldMacLayer, Vec2(-1500,0),1.0,CallFunc::create([=](){
        
        
        m_pMoldMacLayer->showMacCenter();
    }));
}

void PourMoldLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    
    toolSprite->setTexture("images/make_mold/mold/push1.png");
}
void PourMoldLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->setTexture("images/make_mold/mold/push0.png");
    this->unschedule(schedule_selector(PourMoldLayer::updateMold));
    unscheduleAllSelectors();
}

void PourMoldLayer::showMold()
{
    
}
void PourMoldLayer::updateMold(float dt)
{
    if(m_bStartPour){
        if(g_Mold_Sprite){
            
            if (g_Mold_Sprite->getChildByTag(m_iMoldTag)) {
                dynamic_cast<ProgressTimer*>(g_Mold_Sprite->getChildByTag(m_iMoldTag))->setPercentage(dynamic_cast<ProgressTimer*>(g_Mold_Sprite->getChildByTag(m_iMoldTag))->getPercentage()+2.5);
            }
        }
    }
    
    log("111111");
}
void PourMoldLayer::nextBtn(Ref* pSender)
{
    
    Scene* scene = OutMoldScene::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0,scene));
}
void PourMoldLayer::startPour(int tag,Vec2 pos)
{
    g_Mold_Sprite = Sprite::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s.png",pourMoldPath[tag].c_str())->getCString());
    this->addChild(g_Mold_Sprite,11);
    
    //retain一次，保存到下个界面
    g_Mold_Sprite->retain();
    
    g_Mold_Sprite->setPosition(Director::getInstance()->getVisibleSize()/2);
//    g_Mold_Sprite->setOpacity(0);
    g_Mold_Sprite->setTag(tag);
    g_Mold_Sprite->setAnchorPoint(Vec2(0.5,0.5));
    g_Mold_Sprite->setPositionY(pos.y);
//    g_Mold_Sprite->setPositionX(g_Mold_Sprite->getPositionX() -20);
//    g_Mold_Sprite->runAction(FadeIn::create(0.5));
    g_Mold_Vector.pushBack(g_Mold_Sprite);
    
    m_pMoldMacLayer->setMoldType(g_Mold_Sprite->getTag());
    
    auto moldShadow = Sprite::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s_shadow.png",pourMoldPath[tag].c_str())->getCString());
    moldShadow->setAnchorPoint(Vec2::ZERO);
    moldShadow->setTag(shadowTag);
    g_Mold_Sprite->addChild(moldShadow,-201);
    
//    g_Mold_Sprite->runAction(MoveBy::create(1.0,Vec2(-20,0)));
    
}
