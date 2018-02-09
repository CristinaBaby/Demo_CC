//
//  MarketScene.cpp
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/6.
//
//

#include "MarketScene.h"
#include "CircleBy.h"

//背景上矩形区域的坐标
Vec2 rectSub[10]={Vec2(-346,150),Vec2(-89,166),Vec2(167,167),Vec2(340,135),Vec2(400,-125),Vec2(-557,-171),Vec2(-309,-98),Vec2(-140,-61),Vec2(18,-55),Vec2(165,-59)};
//背景上矩形区域的大小
Size rectSize[10]={Size(99,177),Size(200,170),Size(150,150),Size(219,100),Size(150,110),Size(160,160),Size(100,150),Size(90,140),Size(118,140),Size(172,88)};

//物品的路径名
string cartPath[10]={"cart_whisk.png","cart_bowl.png","cart_milk.png","cart_sugar.png","cart_egg.png","cart_vanilla.png","cart_greentea.png","cart_chocolate.png","cart_cookie.png","cart_strawberry.png"};

//对应物品在购物篮的坐标
Vec2 carPos[10]={Vec2(90,180),Vec2(161,160),Vec2(210,195),Vec2(295,169),Vec2(383,163),Vec2(318,230),Vec2(100,160),Vec2(121,160),Vec2(360,195),Vec2(270,195)};

//清单已选物品的坐标,与carPath对应
Vec2 tickPos[6]={Vec2(236,313),Vec2(235,221),Vec2(539,221),Vec2(539,313),Vec2(235,129),Vec2(540,129)};

MarketScene::MarketScene():m_bIsTouch(true),m_iCarItemNum(0)
{
   
}
MarketScene::~MarketScene()
{
    
}
bool MarketScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
    
}
void MarketScene::onEnter()
{
    GameBaseScene::onEnter();
}
void MarketScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void MarketScene::onExit()
{
    GameBaseScene::onExit();
}
void MarketScene::initUI()
{
    m_pBG = Sprite::create("images/market/mart_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    m_vRect.clear();
    for (int i = 0; i<10; i++) {
        
//        LayerColor* test = LayerColor::create(Color4B(0, 0, 0, 100));
//        test->setAnchorPoint(Vec2(0, 0));
//        test->setContentSize(rectSize[i]);
//        test->setPosition(m_pBG->getPosition()+rectSub[i]);
//        this->addChild(test,100);
        
        Rect rect = Rect((m_pBG->getPosition()+rectSub[i]).x,(m_pBG->getPosition()+rectSub[i]).y,rectSize[i].width,rectSize[i].height);
        
        m_vRect.push_back(rect);
    
    }

    m_pCart = Sprite::create("images/market/cart0.png");
    kAdapterScreen->setExactPosition(m_pCart, 480,-60,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToContentLayer(m_pCart);
    
    auto cart=Sprite::create("images/market/cart1.png");
    kAdapterScreen->setExactPosition(cart, 480,-60,Vec2(0.5,0),kBorderNone,kBorderBottom);
//    cart->setAnchorPoint(Vec2::ZERO);
    this->addToContentLayer(cart,10);
    
    

    m_pListBtn = ToolSprite::create("images/market/btn_list@2x.png");
    kAdapterScreen->setExactPosition(m_pListBtn, 10, 10,Vec2::ZERO,kBorderLeft,kBorderBottom);
    this->addToUILayer(m_pListBtn);
    m_pListBtn->setTag(100);
    m_pListBtn->setDelegate(this);
    m_pListBtn->setIsMove(false);
    
    
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_pListBtn->setPositionY(m_pListBtn->getPositionY()+100);
#endif
    
    m_pLayerColor = LayerColor::create(Color4B(0,0,0,180));
    m_pLayerColor->setVisible(false);
    this->addToUILayer(m_pLayerColor,5);
    
    m_pListBg = Sprite::create("images/market/list.png");
    kAdapterScreen->setExactPosition(m_pListBg, 480, 320);
    this->addToUILayer(m_pListBg,10);
//    m_pListBg->setVisible(false);
    m_pListBg->setPosition(m_pListBtn->getPosition() + m_pListBtn->getContentSize()/2);
    m_pListBg->setScale(0);
    
    m_pListClose = ToolSprite::create("images/market/btn_close@2x.png");
    m_pListClose->setPosition(Vec2(614,444));
    m_pListClose->setTag(200);
    m_pListClose->setDelegate(this);
    m_pListClose->setIsMove(false);
 
    m_pListBg->addChild(m_pListClose);
    
    
    for (int i = 0;i<6;i++) {
        
        auto check = Sprite::create("images/market/icon_tick@2x.png");
        check->setPosition(tickPos[i]);
        check->setTag(i);
        m_pListBg->addChild(check);
        check->setVisible(false);
    }
    
//    g_choose_waste="chocolate";
    
    auto waste = Sprite::create(__String::createWithFormat("images/market/icon_%s@2x.png",g_choose_waste.c_str())->getCString());
    waste->setPosition(Vec2(445,138));
    m_pListBg->addChild(waste);
    
    addNeedCartTag();
    
    auto lister = EventListenerTouchOneByOne::create();
    lister->onTouchBegan = CC_CALLBACK_2(MarketScene::TouchBegan, this);
    lister->onTouchEnded = CC_CALLBACK_2(MarketScene::TouchEnded, this);
    
    lister->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);
    
    showListBg(false, -1);
}
void MarketScene::addNeedCartTag()
{
    string checkWaste = __String::createWithFormat("cart_%s.png",g_choose_waste.c_str())->getCString();
    
    int tag = -1;
    for (int i = 0 ; i<10;i++) {
        
        if (checkWaste == cartPath[i].c_str()) {
            
            tag = i;
            break;
        }
        
    }
    m_vCartNeedTag.push_back(0);
    m_vCartNeedTag.push_back(1);
    m_vCartNeedTag.push_back(2);
    m_vCartNeedTag.push_back(3);
    m_vCartNeedTag.push_back(4);
    m_vCartNeedTag.push_back(tag);
    
}
//一定时间未点击，播放随机提示
void MarketScene::checkUnTouch(float f)
{
    

}
bool MarketScene::TouchBegan(Touch *touch, Event *unused_event)
{
    if(!m_bIsTouch)
        return false;
    
    int tag = checkTouchRect(touch->getLocation());
    if (tag != -1) {
        
        if(checkNeedCartTag(tag)){
            AudioHelp::getInstance()->playEffect("tap.mp3");
            m_bIsTouch = false;
            cartAction(tag,touch->getLocation());
            
        }else{
        
            
         
        }
        
    }else{
    
        AudioHelp::getInstance()->playEffect("tapcloud.mp3");
    
    }
    
   
    return true;
}
void MarketScene::TouchEnded(Touch *touch, Event *unused_event)
{
    log("touch");
    
    
}

void MarketScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    switch (tag) {
        case 100:
        {
            showListBg(false, -1);
        }
            break;
        case 200:
        {
            m_pListClose->cancelEvent(true);
            hideListBg();
        }
            break;
    }

}
void MarketScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{

}

void MarketScene::cartAction(int tag,Vec2 pos)
{
    
    auto cart = Sprite::create(__String::createWithFormat("images/market/%s",cartPath[tag].c_str())->getCString());
    cart->setPosition(pos);
    cart->setScale(0.8);
    cart->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addToContentLayer(cart);
    
    m_pListBtn->cancelEvent(true);
    
    auto func = CallFunc::create([=](){
        
        showListBg(true,tag);
    });
    
    
    MoveTo* move = MoveTo::create(0.25,m_pCart->convertToWorldSpace(carPos[tag])+ Vec2(0,200));
    MoveBy* move1 = MoveBy::create(0.75,Vec2(0,50));
//    CircleBy* c = CircleBy::create(2.0, Vec2(50, 50), 0, 360);
    RotateBy* rota = RotateBy::create(0.75, 360*2);
    Spawn* sp =Spawn::create(move1,rota,NULL);
    
    MoveBy* move2 = MoveBy::create(0.25, Vec2(0,-250));
    EaseSineIn* esi = EaseSineIn::create(move2);

    
    cart->runAction(Sequence::create(move,sp,esi, DelayTime::create(0.5),func,NULL));
    
}
void MarketScene::showListBg(bool isShowCheck,int tag)
{
    log("showListBg");
    m_pLayerColor->setVisible(true);
    
    m_pListBtn->cancelEvent(true);
    
    m_bIsTouch = false;
    
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    MoveTo*   move = MoveTo::create(0.5,kAdapterScreen->getExactPostion(Vec2(480,320)));
    Spawn* sp = Spawn::create(scale,move, NULL);
    EaseSineIn* esi =EaseSineIn::create(sp);
    
    
    auto func = CallFunc::create([=](){
    
        //显示check
        if(isShowCheck){
        
            showCheckIcon(tag);
            
            
        }
        
    });
    
    m_pListBg->runAction(Sequence::create(esi,func,NULL));
    
    
}
void MarketScene::showCheckIcon(int tag)
{
    if (tag == -1) {
        return;
    }
    if (tag > 4) {
    
        m_pListBg->getChildByTag(5)->setVisible(true);
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
        
        emitter1->setPosition(m_pListBg->convertToWorldSpace(m_pListBg->getChildByTag(5)->getPosition()));
        
        emitter1->setAutoRemoveOnFinish(true);
        this->addChild(emitter1, 30);
        AudioHelp::getInstance()->playEffect("all star .mp3");
        
        
    
    }else{

        m_pListBg->getChildByTag(tag)->setVisible(true);
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
        
        emitter1->setPosition(m_pListBg->convertToWorldSpace(m_pListBg->getChildByTag(tag)->getPosition()));
        
        emitter1->setAutoRemoveOnFinish(true);
        this->addChild(emitter1, 30);
        
        AudioHelp::getInstance()->playEffect("all star .mp3");

    }
    
    m_iCarItemNum++;
    
    log("---=->%d",m_iCarItemNum);
    if (m_iCarItemNum == 6) {
        
        AudioHelp::getInstance()->playEffect("childern happy.mp3");
        showNextButton(true);
        
    }
    
}
void MarketScene::hideListBg()
{
    ScaleTo* scale = ScaleTo::create(0.5, 0);
    MoveTo*   move = MoveTo::create(0.5,m_pListBtn->getPosition() + m_pListBtn->getContentSize()/2);
    Spawn* sp = Spawn::create(scale,move, NULL);
    EaseSineOut* esi =EaseSineOut::create(sp);

    auto func = CallFunc::create([=](){
        
        m_pListBtn->cancelEvent(false);
        
        m_bIsTouch = true;
        
        m_pListClose->cancelEvent(false);
       
        
         log("hideListBg");
        m_pLayerColor->setVisible(false);

        
    });
    
    m_pListBg->runAction(Sequence::create(esi,func,NULL));
    
}
int MarketScene::checkTouchRect(Vec2 pos)
{
    for (int i = 0; i< m_vRect.size(); i++) {
        if (m_vRect.at(i).containsPoint(pos)){

            if (checkCartTag(i)) {
                m_vCartTag.push_back(i);
                return  i;
            }
            
        }
        
    }
    
    return -1;
}
bool MarketScene::checkNeedCartTag(int tag)
{
    vector<int>::iterator v_tag;
    for (v_tag=m_vCartNeedTag.begin();v_tag!=m_vCartNeedTag.end() ; v_tag++) {
        
        if (*v_tag == tag) {
            return true;
        }
        
    }
    return false;


}
bool MarketScene::checkCartTag(int tag)
{
    vector<int>::iterator v_tag;
    for (v_tag=m_vCartTag.begin();v_tag!=m_vCartTag.end() ; v_tag++) {
        
        if (*v_tag == tag) {
            return false;
        }
        
    }
    return true;
}

void MarketScene::nextStep()
{
    log("next");
    hideNextButton();
    SceneChangeManager->enterMakeScene();
}