//
//  SelectMoldLayer.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#include "SelectMoldLayer.h"
#include "AdapterScreen.h"
#include "ParabolaTo.h"
#include "ActionUtils.h"
#include "AudioHelp.h"
#include "Global.h"
#include "SceneManager.h"

#define BgZorder 10
#define ContentZorder 20
#define UIZorder 20

//按钮之间的间隔
#define BtnSpace 20

#define BtnSelect 10000
#define BtnNormol 10001
#define BtnShadow 10002

string selectPath[6]={"bear","s","coke","fish","hambuger","heart"};

//按钮的位置以及旋转的角度
Vec2 btnPos[6]={Vec2(200,433),Vec2(456,449),Vec2(639,404),Vec2(416,233),Vec2(909 - 50  ,475),Vec2(799,259)};
int  rotateMold[6]={-20,40,20,-80,10,-15};
SelectMoldLayer::SelectMoldLayer()
{
    //选择有两种状态 false为默认状态   true为第二种状态
    m_bIsSelect = false;
    //记录第一种状态选中的btn
    m_pSelectBtn = nullptr;
    m_pFont = nullptr;
}
SelectMoldLayer::~SelectMoldLayer()
{

}

bool SelectMoldLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    
    m_pBg = Sprite::create("images/make_mold/select/choose_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg,480,320);
    this->addChild(m_pBg,0);
    m_pBg->setVisible(false);
    
    //初始化按钮的位置
    Size vislible = Director::getInstance()->getVisibleSize();
//    float marginH = (vislible.width-BtnSpace*2-3*227)/2;
//    float marginV = (vislible.height-BtnSpace-2*206)/2;
//    for (int i = 0; i<6; i++) {
//        
//        Vec2 pos;
//        if (i<3) {
//            pos.x = marginH + i*(227 + BtnSpace);
//            pos.y = marginV + 206 + BtnSpace;
//            
//            btnPos[i]=pos;
//        }else{
//            pos.x = marginH + (i-3)*(227 + BtnSpace);
//            pos.y = marginV ;
//            btnPos[i]=pos;
//        }
//    }
//    
    //初始化按钮
    for (int i = 0; i < 6; i++) {

        auto button = MySpriteButton::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s.png",selectPath[i].c_str())->getCString(), std::bind(&SelectMoldLayer::clickBtn, this,std::placeholders::_1));
        button->setTag(i);
        button->setScale(0.5);
//        button->setAnchorPoint(Vec2::ZERO);
        this->addChild(button,ContentZorder);
        button->setPosition(btnPos[i]);
        button->setEnable(false);
        if (i<3) {
            
            button->setPosition(Vec2(-300,vislible.height+500));
            
        }else{
        
            button->setPosition(Vec2(vislible.width+300,vislible.height+500));
        }
        m_vBtn.pushBack(button);
//        btnPos[i]=btnPos[i]+Vec2(-50,0);
    }
    
    showBtn();
    return true;
}
void SelectMoldLayer::onEnter()
{
    Layer::onEnter();
    
    //获取是否解锁
    
    if(m_vLock.size()>0)
    {
        bool isLockMold = UserDefault::getInstance()->getBoolForKey(GummyMoldLock,true);
        if(!isLockMold){
            
            for(auto lock : m_vLock){
                
                lock->removeFromParent();
                
            }
            m_vLock.clear();
            for (auto spriteBtn:m_vBtn) {
                int tag = spriteBtn->getTag();
                if (tag>1) {
                    
                    if (spriteBtn->getChildByTag(BtnShadow)) {
                        spriteBtn->getChildByTag(BtnShadow)->removeFromParent();
                    }
                    
                    
                }
            }
        }
    }
    
    
}
void SelectMoldLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void SelectMoldLayer::onExit()
{
    Layer::onExit();
}
void SelectMoldLayer::showFont()
{
    m_pFont = Sprite::create("images/make_mold/select/choose-mold.png");
    kAdapterScreen->setExactPosition(m_pFont,480,50 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(m_pFont,100);

    AudioHelp::getInstance()->playEffect("ingredients_fly_in");
    ActionUtils::moveByElasticOut(m_pFont,Vec2(0,-500));
}
#pragma mark -- 按钮相关
void SelectMoldLayer::showBtn()
{
    for(auto btn : m_vBtn){
    
        int tag = btn->getTag();
        
        ScaleTo* s1 = ScaleTo::create(0.1f,0.58f,0.50f);
        ScaleTo* s2 = ScaleTo::create(0.1f,0.50,0.58f);
        ScaleTo* s3 = ScaleTo::create(0.2f,0.5f,0.5f);
        
        if(tag!=5){
        
            ActionUtils::delayTimeAction(this,abs(tag-2)*1.0,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("mold in.mp3");
                
                
            }));
        
        }
        
        
        
        if (tag <3) {
//            btn->runAction(Sequence::create(DelayTime::create(abs(tag-2)*0.25),ParabolaTo::create(1.0, btn->getPosition(),btnPos[tag]+btn->getContentSize()/2,-50,10),s2,s1,s3, NULL));
            
            MoveBy* move = MoveBy::create(0.5,Vec2(-10,10));
            RotateBy* rota = RotateBy::create(0.5,rotateMold[tag]);
            Spawn* sp = Spawn::create(move,rota, NULL);
            
            btn->runAction(Sequence::create(DelayTime::create(abs(tag-2)*1.0),MoveTo::create(0.5,kAdapterScreen->getExactPostion(btnPos[tag])+Vec2(10 -50,-10 - 80)),sp ,s1,s2,s3, NULL));
        }else{
//            btn->runAction(Sequence::create(DelayTime::create((tag - 3)*0.25),ParabolaTo::create(1.0, btn->getPosition(),btnPos[tag]+btn->getContentSize()/2,-50,10),s2,s1,s3, NULL));
            MoveBy* move = MoveBy::create(0.5,Vec2(-10,-10));
            RotateBy* rota = RotateBy::create(0.5,rotateMold[tag]);
            Spawn* sp = Spawn::create(move,rota, NULL);
            btn->runAction(Sequence::create(DelayTime::create((tag - 3)*1.0),MoveTo::create(0.5,kAdapterScreen->getExactPostion(btnPos[tag])+Vec2(10 -50,10 - 80)),sp,s1,s2,s3,CallFunc::create([=](){
                if(tag == 5){
                
                    //获取是否解锁
                    bool isLockMold = UserDefault::getInstance()->getBoolForKey(GummyMoldLock, true);
                    
                    if (isLockMold) {
                        
                        addLock();
                        
                    }else{
                    
                    
                        for (auto sprite:m_vBtn) {
                            
                            sprite->setEnable(true);
                            
                        }
                    
                    }
                    
                    showFont();
                }
            }), NULL));
        }
    }
}

void SelectMoldLayer::addLock()
{
    for (auto spriteBtn:m_vBtn) {
        int tag = spriteBtn->getTag();
        if (tag>1) {

            auto shade = Sprite::create(__String::createWithFormat("images/make_mold/select/choosemold_%s_mask.png",selectPath[tag].c_str())->getCString());
            shade->setAnchorPoint(Vec2::ZERO);
            spriteBtn->addChild(shade);
            shade->setOpacity(0);
            shade->setTag(BtnShadow);
            
            auto lock = Sprite::create("images/make_mold/select/choosemold_lock.png");
            lock->setPosition(spriteBtn->getContentSize()/2);
            spriteBtn->addChild(lock);
            lock->setScale(0);
            lock->setTag(tag);
            lock->setRotation(-rotateMold[tag]);
            m_vLock.pushBack(lock);
            
            shade->runAction(Sequence::create(FadeIn::create(0.5),CallFunc::create([=](){
            
                lock->runAction(Sequence::create(ScaleTo::create(0.5, 1.1),ScaleTo::create(0.15, 1.2), CallFunc::create([=](){
                
                    if (tag == 5) {
                        for (auto sprite:m_vBtn) {
                        
                            sprite->setEnable(true);
                        
                        }
                    }
                
                }),NULL));
            
            }), NULL));
        }
        
    }
}
void SelectMoldLayer::clickBtn(Ref *sender)
{
    MySpriteButton* btn = dynamic_cast<MySpriteButton*>(sender);
    int tag = btn->getTag();
    if(m_vLock.size()>0&&tag>1)
    {
    
        SceneChangeManager->enterShopScene();
        return;
    }
    
    //第一种选择的状态
    if(!m_bIsSelect){
 
        AudioHelp::getInstance()->playEffect("button_general.mp3");
        
        MySpriteButton* btn = dynamic_cast<MySpriteButton*>(sender);
        ScaleTo* s1 = ScaleTo::create(0.1f,0.58f,0.50f);
        ScaleTo* s2 = ScaleTo::create(0.1f,0.50,0.58f);
        ScaleTo* s3 = ScaleTo::create(0.2f,0.5f,0.5f);
        btn->runAction(Sequence::create(s1,s2,s3, NULL));
        btn->setEnable(false);
        //未选中的按钮，移动到边缘，可供继续选择
        Size vislible = Director::getInstance()->getVisibleSize();
        float marginV = (vislible.height-20-103*5)/2;
        float btnX = vislible.width - 10 - 227/2;
        int i = 0;
        btn->setUserData((void*)BtnSelect);
        
        ActionUtils::moveByElasticIn(m_pFont,Vec2(0,500));
        
        if(showPourLayer){
            
            showPourLayer();
            
        }
        
        for(auto sprite : m_vBtn){
            
            if (sprite == btn) {
                continue;
            }
            sprite->setUserData((void*)BtnNormol);
            sprite->setEnable(false);
            
            MoveTo* move = MoveTo::create(1.0, Vec2(btnX,marginV + i*(103+5))+sprite->getContentSize()/4);
            EaseElasticOut* eeo = EaseElasticOut::create(move,1.0);
            ScaleTo* scale = ScaleTo::create(0.5, 0.2);
            RotateTo* rota = RotateTo::create(0.5,0);
            Spawn* sp = Spawn::create(eeo,scale,rota,NULL);
            sprite->runAction(Sequence::create(DelayTime::create(i*0.3),sp,CallFunc::create([=](){
                
                for(auto lock : m_vLock){
                
                    lock->setRotation(0);
                
                }
                
                
                if(i == 4){
                    //选中按钮，移动到屏幕中间
                    selectBtnAction(btn);
                    
                    if (changeBgCallback) {
                        changeBgCallback();
                    }
                }
                
            }),NULL));
            i++;
        }
    }
    //第二种选择的状态
    else{
        
        if (m_pSelectBtn) {
            AudioHelp::getInstance()->playEffect("button_general.mp3");
            //防止连续点击
            for(auto sprite : m_vBtn){
                sprite->setEnable(false);
            }
            
            MySpriteButton* btn = dynamic_cast<MySpriteButton*>(sender);
            //点击到选中状态的按钮，弹动动画
            if(m_pSelectBtn->getPosition() == btn->getPosition()){
            
                ScaleTo* s1 = ScaleTo::create(0.1f,0.58f,0.50f);
                ScaleTo* s2 = ScaleTo::create(0.1f,0.50,0.58f);
                ScaleTo* s3 = ScaleTo::create(0.2f,0.5f,0.5f);
                //中间选中状态的按钮，活动到边缘
                BezierTo* parabola = ParabolaTo::create(1.0, m_pSelectBtn->getPosition(), btn->getPosition());
                ScaleTo* scaleP = ScaleTo::create(1.0, 1.0);
                Spawn* sp = Spawn::create(parabola,scaleP, NULL);
                m_pSelectBtn->runAction(Sequence::create(s1,s2,s3,sp,CallFunc::create([=](){
                    
                    for(auto sprite : m_vBtn){
                        
                        sprite->setEnable(true);
                    }
                    m_pSelectBtn = nullptr;
                    m_pSelectBtn = btn;
                    
                }), NULL));
                
            }
            //点击右边按钮,将点击的按钮，设置为选中状态
            else{
            
                //中间选中状态的按钮，活动到边缘
                BezierTo* parabola = ParabolaTo::create(1.0, m_pSelectBtn->getPosition(), btn->getPosition());
                ScaleTo* scaleP = ScaleTo::create(1.0, 0.2);
                Spawn* sp = Spawn::create(parabola,scaleP, NULL);
                m_pSelectBtn->setUserData((void*)BtnNormol);
                m_pSelectBtn->runAction(Sequence::create(sp,CallFunc::create([=](){
                    
                    for(auto sprite : m_vBtn){
                        
                        sprite->setEnable(true);
                    }
                    m_pSelectBtn = nullptr;
                    m_pSelectBtn = btn;
                    
                }), NULL));
                
                //边缘被点击的按钮，运动到中间,成为被选中的按钮
                MoveTo* move = MoveTo::create(1.0, m_pSelectBtn->getPosition());
                EaseElasticOut* eeo = EaseElasticOut::create(move);
                ScaleTo* scale = ScaleTo::create(0.5, 1.0);
                Spawn* sp1 = Spawn::create(eeo,scale, NULL);
                btn->setUserData((void*)BtnSelect);
                btn->runAction(Sequence::create(sp1,CallFunc::create([=](){
                }), NULL));
            }
        }
    }
}
//m_pBG->convertToWorldSpace(Vec2(550,290))
 //选中按钮，移动到屏幕中间
void SelectMoldLayer::selectBtnAction(MySpriteButton* btn)
{
    Size vislible = Director::getInstance()->getVisibleSize();
    btn->runAction(Sequence::create(MoveTo::create(0.5, Vec2(vislible.width/2,m_pBg->convertToWorldSpace(Vec2(550,290)).y)),ScaleTo::create(0.2, 1.3),ScaleTo::create(0.2, 1.0),RotateTo::create(0.2, 0),CallFunc::create([=](){
        for(auto sprite : m_vBtn){
          sprite->setEnable(true);
        }
        btn->setEnable(false);
        m_pSelectBtn=btn;
        m_bIsSelect = true;
        
        //选中之后的回掉
        if(isSelectCallback){
            isSelectCallback();
        }
        
    }), NULL));
}
void SelectMoldLayer::hideSelectBtn()
{
    int i = 0;
    for(auto sprite : m_vBtn){
        sprite->setEnable(false);
        i++;
        //按钮的两个状态
        size_t btnState = (size_t)sprite->getUserData();
        if (btnState == BtnNormol) {
            MoveBy* move = MoveBy::create(1.0, Vec2(300,0));
            EaseElasticIn* eeo = EaseElasticIn::create(move,0.5);
            sprite->runAction(Sequence::create(DelayTime::create(i*0.1),eeo, NULL));
        }else{
            
//            sprite->setCascadeOpacityEnabled(true);
//            sprite->runAction(Sequence::create(MoveTo::create(0.5,Vec2(Director::getInstance()->getVisibleSize().width/2,m_pBg->convertToWorldSpace(Vec2(550,290)).y)),FadeOut::create(0.5),CallFunc::create([=](){
//            
//                
//            
//            }), NULL));
            if (startPourCallback) {
                startPourCallback(sprite->getTag());
            }
        }
    }
}
void SelectMoldLayer::_hideSelectBtn()
{
    for(auto sprite : m_vBtn){
        sprite->setEnable(false);
        //按钮的两个状态
        size_t btnState = (size_t)sprite->getUserData();
        if (btnState == BtnNormol) {
            
        }else{
            
            sprite->setCascadeOpacityEnabled(true);
            sprite->runAction(Sequence::create(FadeOut::create(0.5),CallFunc::create([=](){
            
                
            
            }), NULL));
            
        }
    }


    

}