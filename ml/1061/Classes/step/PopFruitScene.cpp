//
//  PopFruitScene.cpp
//  test-cocos2dx
//
//  Created by wusonglin on 15/12/12.
//
//

#include "PopFruitScene.h"
#include "AdapterScreen.h"
#include "ActionUtils.h"
#include "AudioHelp.h"
#include　"VictoryLayer.h"
#include　"SceneManager.h"
#include "ChangeLoading.h"
#include "IAPManager.h"
#include "AdsManager.h"

string fruitPointPath[5] ={"kiwi","strawberry","lemon","orange","blueberry"};
string fruitBottomPath[5]={"bottom_kiwi.png","bottom_strawberry.png","bottom_lemon.png","bottom_orange.png","bottom_blueberry.png"};
string fontPath[3]={"amazing","awesome","cool"};

Color4F colorParticle[5]={Color4F(0/255,255/255,0/255,1),Color4F(255/255,0/255,0/255,1),Color4F(255/255,255/255,0/255,1),Color4F(255/255,0.64,11/255,1),Color4F(0/255,0/255,255/255,1)};

float PopFruitScene::ONE_CLEAR_TIME = 0.1f;
PopFruitScene::PopFruitScene()
{
    clearSumTime = 0;
    needClear=false;
    m_bIsEnable=true;
    m_iStep = 0;
    m_bIsOver = false;
    //检测用户5S不操作
    m_iIsNoResponse = 0;
    m_bIsStart = false;
    
    //是否开始5s计时
    m_bIsStartTime = false;
    
    m_pHand = nullptr;
}
PopFruitScene::~PopFruitScene()
{
    
    
}
cocos2d::Scene* PopFruitScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PopFruitScene::create();
    scene->addChild(layer);
    
    return scene;
}
//测试完成界面
//void PopFruitScene::createL()
//{
//}
bool PopFruitScene::init()
{
    if(!Layer::init()){
    
        return false;
    }
    
    //测试
//    createL();
    
    
    //分配空间
    memset(m_pFruits, 0, sizeof(FruitSprite*) * FRUIT_ROW_NUM * FRUIT_COL_NUM);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("images/pop_fruit/minigame_bg.jpg");
    bg->setPosition(visibleSize/2);
    this->addChild(bg);
    
    m_pFruitFream = Sprite::create("images/pop_fruit/1.png");
    kAdapterScreen->setExactPosition(m_pFruitFream,20 - 500, 320,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addChild(m_pFruitFream,10);

    auto fream = Sprite::create("images/pop_fruit/bottom.png");
    fream->setPosition(visibleSize/2);
    this->addChild(fream);
    fream->setPositionX(fream->getPositionX()+50);
    
    m_pFream = LayerColor::create();
    fream->addChild(m_pFream);
    m_pFream->ignoreAnchorPointForPosition(false);
    m_pFream->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pFream->setContentSize(Size(FruitSprite::WIDTH*8,FruitSprite::HEIGHT*5));
    m_pFream->setPosition(fream->getContentSize()/2);

    m_pTitle = Sprite::create("images/pop_fruit/11.png");
    kAdapterScreen->setExactPosition(m_pTitle,480,20,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(m_pTitle);
    m_pTitle->setPositionY(m_pTitle->getPositionY()+500);

    //初始化手指
    m_pHand = Sprite::create("images/finger2.png");
    this->addChild(m_pHand);
    m_pHand->setVisible(false);
    m_pHand->setAnchorPoint(Vec2(0.78,0.12));
    //各种初始化
    initFreamFruit();
//    initFruit();
    initView();
    
    
    return true;
}
void PopFruitScene::onEnter()
{
    Layer::onEnter();
//    showParticle(0);
    
    //调用统计模块
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent("Mini game");
    
    if (kIAPManager->isShowAds()) {
        //        STAds st;
        //        st.requestAds();
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    
//    AudioHelp::getInstance()->StopBackgroundMusic();
//    AudioHelp::getInstance()->playBackgroundMusic("bg_music.mp3");
    
    AudioHelp::getInstance()->playEffect("pop_up_open.mp3");
    m_pFruitFream->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(500,0)), 1.0),CallFunc::create([=](){
    
        initFruit();
    
        ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
            
            showTitle();
            
        }));
    }),nullptr));
}
void PopFruitScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void PopFruitScene::onExit()
{
    Layer::onExit();
    
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent("Mini game");
}
#pragma mark ---- 初始化
void PopFruitScene::initFreamFruit()
{
    float width  = m_pFruitFream->getContentSize().width;
    float height = m_pFruitFream->getContentSize().height;
    float detalH = (height - 10*4 - 76*5)/2;
    
    for (int i = 0; i<5; i++) {

        auto fruit = Sprite::create(__String::createWithFormat("images/pop_fruit/%s_glass.png",fruitPointPath[i].c_str())->getCString());
        fruit->setPosition(Vec2(width/2,detalH + 76/2 + i*(76 + 15)));
        fruit->setTag(i);
        m_pFruitFream->addChild(fruit);
        
        Sprite* sp = Sprite::create(__String::createWithFormat("images/pop_fruit/%s.png",fruitPointPath[i].c_str())->getCString());
        auto mold = ProgressTimer::create(sp);
        mold->setType(ProgressTimer::Type::BAR);
        mold->setBarChangeRate(Point(0,1));
        mold->setMidpoint(Point(0.5,0));
        mold->setTag(i);
        
        mold->setAnchorPoint(Vec2::ZERO);
        fruit->addChild(mold);
        
        m_vTime.pushBack(mold);
        
    }
    AudioHelp::getInstance()->playEffect("candy in.mp3");
}
void PopFruitScene::initFruit()
{
    srand((int)time(0));
//    int color = rand()%5;
    for(int i=0;i<FRUIT_ROW_NUM;i++){
        for(int j=0;j<FRUIT_COL_NUM;j++){
            int color = abs(rand()%FruitSprite::COLOR_MAX_NUM);
            
            FruitSprite* fruit = FruitSprite::create(color);
            m_pFruits[i][j] = fruit;
            fruit->setPosition(getPosByRowCol(i,j) + Vec2(0,100));
            fruit->setDesPosition(getPosByRowCol(i,j));
            fruit->setIndex_RowCol(i,j);
            m_pFream->addChild(fruit);
        }
    }
    
    
    
    
    
    
    
    
}
void PopFruitScene::initView()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *unused_event){
    
        if (!m_bIsStart) {
            return false;
        }
        
        if(m_bIsOver)
            return false;
        
        if(!m_bIsStartTime){
            m_bIsStartTime = true;
            this->schedule(schedule_selector(PopFruitScene::updateResponse), 1.0);
        }
        m_iIsNoResponse = 5;
        
        //所有水果移动完成，才可以点击
        if (checkFruitIsAdjust()) {
            
            Vec2 pos = m_pFream->convertToNodeSpace(touch->getLocation());
            
            auto fruit = getFruitByTouch(pos);
            if (fruit) {
                
                if (m_pHand) {
                    m_pHand->removeFromParent();
                    m_pHand = nullptr;
                }
                
                //得到待删除队列
                this->genSelectedList(fruit);
//                log("delete size -- %d",(int)selectedList.size());
                this->deleteSelectedList();
            }
        }
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    this->scheduleUpdate();
}
//检测用户5s未操作
void PopFruitScene::updateResponse(float dt)
{
    
    if (m_iIsNoResponse == 0) {
        
        //提示用户点击
        //停止检测
        this->unschedule(schedule_selector(PopFruitScene::updateResponse));
        //提示用户,
        m_bIsStartTime = false;
        showPointTouch();
        
    }
    m_iIsNoResponse--;
}
//提示用户，哪一些可以点
void PopFruitScene::showPointTouch()
{
    for(int i = 0;i < FRUIT_ROW_NUM;i++){
        for(int j = 0;j< FRUIT_COL_NUM;j++){
            if(m_pFruits[i][j]!=nullptr){
                _genSelectedList(m_pFruits[i][j]);
                
                if (_selectedList.size()>1) {
                    
                    log("----%d",(int)_selectedList.size());
                    break;
                    
                }
            }
        }
        if (_selectedList.size()>1) {
            
            log("----%d",(int)_selectedList.size());
            break;
            
        }
    }
    int index = 0;
    
    showHand(0);
    
    for (auto fruit:_selectedList) {
        fruit->setSelected(false);
        index++;
        fruit->setLocalZOrder(100);
        
        float timeScale = 1.8;
        ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale*0.5, 1.0, 0.80);
        ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale*0.5, 0.82, 1.0);
        ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale*0.5, 1.0, 0.86);
        ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
        ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
        ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
        
        
        fruit->runAction(Sequence::create(lScale1,lScale2,lScale3,lScale4,lScale5,lScale6,CallFunc::create([=](){
            if (index == _selectedList.size()) {
                _selectedList.clear();
                if(!m_bIsStartTime){
                    m_bIsStartTime = true;
                    m_iIsNoResponse=5;
                    this->schedule(schedule_selector(PopFruitScene::updateResponse), 1.0);
                }
            }
        }), NULL));
    }
}
void PopFruitScene::showTitle()
{
    float detal = m_pTitle->getPositionY() - Director::getInstance()->getVisibleSize().height/2;
    
    ActionUtils::moveByElasticOut(m_pTitle,Vec2(0,-detal),1.0,CallFunc::create([=](){
        ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
            ActionUtils::moveByElasticIn(m_pTitle,Vec2(0,detal-500),1.0,CallFunc::create([=](){
                //开始检测用户5s没有操作
                this->schedule(schedule_selector(PopFruitScene::updateResponse), 1.0);
                
                m_bIsStart = true;
                
            }));
        }));
    }));
}
//刷新坐标
void PopFruitScene::update(float dt)
{
    for(int i = 0;i < FRUIT_ROW_NUM;i++){
        for(int j = 0;j< FRUIT_COL_NUM;j++){
            if(m_pFruits[i][j]!=nullptr){
                m_pFruits[i][j]->updatePosition();
            }
        }
    }
    
    if(needClear){
        clearSumTime += dt;
        if(clearSumTime > ONE_CLEAR_TIME){
            clearFruitOneByOne();
            clearSumTime = 0;
        }
    }
    
}
Vec2 PopFruitScene::getPosByRowCol(int row,int col)
{
    float x = row * FruitSprite::WIDTH + FruitSprite::WIDTH/2;
    float y = (col)*FruitSprite::HEIGHT + FruitSprite::HEIGHT/2;
    return Vec2(x,y);
}
void PopFruitScene::showHand(float dt)
{
    if (m_pHand) {
        
        if (_selectedList.size()>1) {
            m_pHand->setVisible(true);
            
            m_pHand->setPosition(m_pFream->convertToWorldSpace(_selectedList.at(0)->getPosition())+Vec2(71,-86));
            
            m_pHand->runAction(Sequence::create(RotateTo::create(0.5,-10),RotateTo::create(0,0),RotateTo::create(0.5,-10),RotateTo::create(0.5,-10),RotateTo::create(0,0),RotateTo::create(0.5,-10),CallFunc::create([=](){
                
                m_pHand->setVisible(false);
                
            }), NULL));
            
            
//            m_pHand->runAction(Sequence::create(MoveBy::create(0,Vec2(10,0)),MoveBy::create(0.5,Vec2(-10,0)),MoveBy::create(0.5,Vec2(10,0)),MoveBy::create(0.5,Vec2(-10,0)),CallFunc::create([=](){
//            
//                m_pHand->setVisible(false);
//            
//            }), NULL));
            
        }
        
        
        
    }
    

}
#pragma mark ---- 点击水果，处理事件
FruitSprite* PopFruitScene::getFruitByTouch(Vec2 pos)
{
    int row = pos.x/FruitSprite::WIDTH;
    int col = pos.y/FruitSprite::HEIGHT;
    if(row >= 0 && row < FRUIT_ROW_NUM &&
       col >= 0 && col < FRUIT_COL_NUM &&
       m_pFruits[row][col] != nullptr){
//        CCLOG("i=%d,j=%d",row,col);
        return m_pFruits[row][col];
    }else{
        return nullptr;
    }
}

/**
    获取连串水果，存入队列容器。。。（利用广度优先遍历）
 **/
void PopFruitScene::genSelectedList(FruitSprite* f)
{
    //首先清空
    selectedList.clear();
    //_selectedList为临时的队列，用于存放需要提示的水果
    
    deque<FruitSprite*> travelList;
    //将点击到的水果存入遍历队列，以此水果进行广度遍历
    travelList.push_back(f);
    deque<FruitSprite*>::iterator it;
    for (it = travelList.begin();it!=travelList.end();) {
        FruitSprite* fruit = *it;
        FruitSprite* linkFruit = nullptr;
        int index_row = fruit->getIndexRow();
        int index_col = fruit->getIndexCol();
        //左
        if(index_row - 1>= 0 && (linkFruit = m_pFruits[index_row-1][index_col])){
        
            //首先判断是否已经被选择，然后再判断颜色是否相同，相同则存放入队列
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row-1][index_col]);
            }
        }
        //右
        if(index_row + 1<FRUIT_ROW_NUM && (linkFruit = m_pFruits[index_row+1][index_col])){
        
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row+1][index_col]);
            }
        
        }
        //下
        if(index_col-1>=0 && (linkFruit = m_pFruits[index_row][index_col-1])){
        
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row][index_col-1]);
            }
        
        }
        //上
        if (index_col+1<FRUIT_COL_NUM && (linkFruit = m_pFruits[index_row][index_col+1])) {
            
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row][index_col+1]);
            }
        }
        //处理当前点击的水果
        if(!fruit->isSelected()){
        
            fruit->setSelected(true);
            //假如到选择队列
            selectedList.push_back(fruit);
            
            
        }
        //队头出队
        travelList.pop_front();
        //得到新的队头
        it = travelList.begin();
        
    }
}
//用于检测需要提示的水果
void PopFruitScene::_genSelectedList(FruitSprite* f)
{
    //首先清空
    _selectedList.clear();
    //_selectedList为临时的队列，用于存放需要提示的水果
    
    deque<FruitSprite*> travelList;
    //将点击到的水果存入遍历队列，以此水果进行广度遍历
    travelList.push_back(f);
    deque<FruitSprite*>::iterator it;
    for (it = travelList.begin();it!=travelList.end();) {
        FruitSprite* fruit = *it;
        FruitSprite* linkFruit = nullptr;
        int index_row = fruit->getIndexRow();
        int index_col = fruit->getIndexCol();
        //左
        if(index_row - 1>= 0 && (linkFruit = m_pFruits[index_row-1][index_col])){
            
            //首先判断是否已经被选择，然后再判断颜色是否相同，相同则存放入队列
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row-1][index_col]);
            }
        }
        //右
        if(index_row + 1<FRUIT_ROW_NUM && (linkFruit = m_pFruits[index_row+1][index_col])){
            
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row+1][index_col]);
            }
            
        }
        //下
        if(index_col-1>=0 && (linkFruit = m_pFruits[index_row][index_col-1])){
            
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row][index_col-1]);
            }
            
        }
        //上
        if (index_col+1<FRUIT_COL_NUM && (linkFruit = m_pFruits[index_row][index_col+1])) {
            
            if(!linkFruit->isSelected() && linkFruit->getColor() == fruit->getColor()){
                travelList.push_back(m_pFruits[index_row][index_col+1]);
            }
        }
//        //处理当前点击的水果
        if(!fruit->isSelected()){
            
            fruit->setSelected(true);
            //假如到选择队列
            _selectedList.push_back(fruit);
            
            
        }
//         _selectedList.push_back(fruit);
        //队头出队
        travelList.pop_front();
        //得到新的队头
        it = travelList.begin();
        
    }
    
    for (auto fruit:_selectedList) {
        fruit->setSelected(false);
    }
    
    
}
/**
    删除符合条件的连续水果
 **/
void PopFruitScene::deleteSelectedList()
{
    if(selectedList.size() == 0){
        
        return;
    }
    //如果点击的水果只有一个，去除选中状态
    if(selectedList.size() == 1){
        selectedList.at(0)->setSelected(false);
        
        AudioHelp::getInstance()->playEffect("mini game_miss.mp3");
        
        return;
    }
    
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
  
    //遍历删除
    int num = 0;
    
    for(int i = 1;i<8;i++){
//
//        auto sp = Sprite::create(__String::createWithFormat("images/pop_fruit/zzz/cut_blueberry%d.png",fruitPointPath[selectedList.at(0)->getColor()].c_str())->getCString());
        auto sp = Sprite::create(__String::createWithFormat("images/pop_fruit/cut/cut_%s%d.png",fruitPointPath[selectedList.at(0)->getColor()].c_str(),i)->getCString());
        ParticleExplosion* effect = ParticleExplosion::create();
        //    effect->setTexture(Director::getInstance()->getTextureCache()->addImage(__String::createWithFormat("images/pop_fruit/%s",fruitBottomPath[selectedList.at(0)->getColor()].c_str())->getCString()));
        effect->setTexture(sp->getTexture());
        effect->setTotalParticles(80 + 10*(int)selectedList.size());
        effect->setStartSize(50.0f);
        effect->setGravity(Point(0,-900));
        effect->setLife(10.0f);
        effect->setSpeed(100);
        effect->setSpeedVar(500);
        effect->setStartColor(Color4F(1, 1, 1, 1));
        effect->setEndColor(Color4F(1, 1, 1, 0));
        effect->setStartColorVar(Color4F(0, 0, 0, 0));
        effect->setEndColorVar(Color4F(0, 0, 0, 0));
        effect->setPosition(m_pFream->convertToWorldSpace(selectedList.at(0)->getPosition()));
        this->addChild(effect,30);
    }

    int size = (int)selectedList.size();
    int deleteNum = 0;
    for (auto it = selectedList.begin();it!=selectedList.end(); it++) {
        deleteNum++;
        num++;
        if(num > 7)
            num=7;
        
        FruitSprite* fruit = *it;
        m_pFruits[fruit->getIndexRow()][fruit->getIndexCol()] = nullptr;
        
        
        
//        ScaleTo* s1 = ScaleTo::create(0.05f,1.08f,0.96f);
//        ScaleTo* s2 = ScaleTo::create(0.05f,1.0f,1.0f);
//        ScaleTo* s3 = ScaleTo::create(0.05f,0.980,1.06f);
        
        float timeScale = 1.8;
      
        ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale*0.5, 1.0, 0.70);
        ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale*0.5, 0.72, 1.0);
        ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale*0.5, 1.0, 0.76);
        ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.78, 1.0);
        ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.79);
        ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
        
//        this->runAction(Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5,EaseElasticOut::create(lScale6,1.0), NULL));
        fruit->runAction(Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5,lScale6,CallFunc::create([=](){
            auto point = Sprite::create(__String::createWithFormat("images/pop_fruit/cut/cut_%s%d.png",fruitPointPath[fruit->getColor()].c_str(),num)->getCString());
            this->addChild(point,30);
            point->setPosition(m_pFream->convertToWorldSpace(fruit->getPosition()));
            point->setOpacity(0);
            point->setScale(0.6);
            
            int index = fruit->getColor();
            ActionInterval *CardinalSplineTo = CatmullRomTo::create(1.0 + num*0.2, doPointArray(m_pFream->convertToWorldSpace(fruit->getPosition()),m_pFruitFream->convertToWorldSpace(m_pFruitFream->getChildByTag(fruit->getColor())->getPosition()),num));
            point->runAction(Sequence::create(DelayTime::create(0.2),FadeIn::create(0.1),CardinalSplineTo,CallFunc::create([=](){
                
                auto sp    =  m_pFruitFream->getChildByTag(index);
                auto pross = dynamic_cast<ProgressTimer*>(sp->getChildByTag(index));
                
                int prossNum = 10;
                if (pross->getPercentage()>60) {
                    prossNum = 40;
                }
                
                if (pross->getPercentage() == 100) {
                    
                    
                    
                }else{
                    
                    pross->runAction(Sequence::create(ProgressFromTo::create(0.2, pross->getPercentage(), pross->getPercentage()+prossNum),CallFunc::create([=](){
                        
                        if(deleteNum == size-1){
                            
                            AudioHelp::getInstance()->playEffect("draw_success.mp3");
                            
                            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/colorEndParticle.plist");
                            emitter1->setPosition(sp->getContentSize()/2);
                            emitter1->setAutoRemoveOnFinish(true);
                            sp->addChild(emitter1, 30);
                        }
                        
                        if (pross->getPercentage() == 100) {
                            
                            m_iStep++;
                            //增加效果
                            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                            emitter1->setPosition(sp->getContentSize()/2);
                            emitter1->setAutoRemoveOnFinish(true);
                            sp->addChild(emitter1, 30);
                            AudioHelp::getInstance()->playEffect("all star .mp3");
                            //                        sp
                            auto shadow = Sprite::create(__String::createWithFormat("images/pop_fruit/%s_bg.png",fruitPointPath[sp->getTag()].c_str())->getCString());
                            shadow->setPosition(sp->getContentSize()/2);
                            sp->addChild(shadow,-1);
                            shadow->setScale(0);
                            shadow->runAction(Sequence::create(ScaleTo::create(0.5,1.2),ScaleTo::create(0.1,1.0), NULL));
                            
                            if(m_iStep==5){
                                
                                for (auto time:m_vTime) {
                                    
                                    time->setPercentage(100);
                                
                                }
                                m_bIsOver = true;
                                auto layerColor = LayerColor::create(Color4B(0,0,0,180));
                                this->addChild(layerColor,40);
                                layerColor->setTag(1008);
//                                layerColor->setOpacity(0);
                                layerColor->runAction(Sequence::create(CallFunc::create([=](){
                                
                                showParticle(0);
                                    
                                
                                }), NULL));
                                log("----over");
                            }
                        }
                    }), NULL));
                }
                
            }),RemoveSelf::create(),nullptr));
            
            if (deleteNum == size-1) {
                
                if(size==3){
                
                    showMagicFont(1);
                
                }else if(size>3){
                    
                    showMagicFont(0);
                    
                    
                }
                //调整坐标
                adjustFruitPos();
                //是否完成
                if (isOver()) {
                    log("Over");
                    needClear=true;
                }
            }
        
        
        }),RemoveSelf::create(), NULL));
      
        
//        fruit->removeFromParent();
    }
}
void PopFruitScene::showParticle(float dt)
{
    AudioHelp::getInstance()->playEffect("cheer_star.mp3");
    for(int i=0;i<9;i++){
        
        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([=](){
            
            auto* effect = ParticleSystemQuad::create("particle/powerStar.plist");
            
            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
            this->addChild(effect,50);
            
            if(i == 8){
                
                if (this->getChildByTag(1008)) {
                    
                    this->getChildByTag(1008)->removeFromParent();
                    
                }
                VictoryLayer* vic = VictoryLayer::create(kVictoryFruit);
                this->addChild(vic,100);
                vic->btnNext=[=](){
                    
                    
//                    vic->removeFromParent();
                    ChangeLoading::loading([](){
                    
                        SceneChangeManager->enterCookMakeScene();
                    
                    });
                    
                    
                };
                vic->btnAgain=[=](){
                    
                    SceneChangeManager->enterPopFruitScene();
                    
                };
                
            }
            
        }), nullptr));
    }

    

}
//展示文字
void PopFruitScene::showMagicFont(int num)
{
    
    __String* str = nullptr;
    int index = -1;
    if (num == 1) {
        index = 2;
        
    }else{
        index = rand()%2;

    }
    str = __String::createWithFormat("images/pop_fruit/%s.png",fontPath[index].c_str());
    auto font = Sprite::create(str->getCString());
    font->setPosition(m_pFream->convertToWorldSpace(m_pFream->getContentSize()/2));
    this->addChild(font,100);
    font->setScale(0);
    
    MoveBy* move = MoveBy::create(0.25,Vec2(0,-5));
    ScaleTo* scale = ScaleTo::create(0.25,1.0);
    Spawn* sp = Spawn::create(move,scale, NULL);
    font->runAction(Sequence::create(ScaleTo::create(0.25,1.2),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect(__String::createWithFormat("%s.mp3",fontPath[index].c_str())->getCString());
    
        ParticleSystemQuad *emitter = ParticleSystemQuad::create("particle/goodParticle.plist");
        emitter->setDuration(0.5);
        emitter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        emitter->setAutoRemoveOnFinish(true);
        emitter->setPosition(m_pFream->convertToWorldSpace(m_pFream->getContentSize()/2));
        this->addChild(emitter, 130);

    }),sp,DelayTime::create(0.5),RemoveSelf::create(),NULL));
}
/**
     删除符合条件的连续水果之后，调整剩下的水果的位置
 **/
void PopFruitScene::adjustFruitPos()
{
    //垂直方向调整
    for (int i = FRUIT_ROW_NUM - 1; i>=0; i--) {
        for (int j = FRUIT_COL_NUM; j>=0; j--) {
            if(m_pFruits[i][j] == nullptr){
                int up  = j;
                int dis = 0;
                while (m_pFruits[i][up] == nullptr) {
                    dis++;
                    up++;
                    if (up>FRUIT_COL_NUM) {
                        break;
                    }
                }
                for (int begin_j = j + dis; begin_j < FRUIT_COL_NUM; begin_j++) {
                    
                    if (m_pFruits[i][begin_j] == nullptr) {
                        continue;
                    }
                    
                    FruitSprite* fruit = m_pFruits[i][begin_j - dis] = m_pFruits[i][begin_j];;
                    fruit->setIndex_RowCol(i, begin_j - dis);
                    fruit->setDesPosition(getPosByRowCol(i,begin_j - dis));
                    m_pFruits[i][begin_j] = nullptr;
                    
                    
                    
                }
            }else{
                continue;
            }
        }
    }
    
    //水平方向调整
    for(int j = 0 ; j<FRUIT_ROW_NUM;j++){
    
        if(m_pFruits[j][0] == nullptr){
            int des = 0;
            int right = j;
            while(m_pFruits[right][0] == nullptr){
                des++;
                right++;
            }
            for(int begin_i = j + des;begin_i<FRUIT_ROW_NUM;begin_i++){
                for(int begin_j = 0;begin_j < FRUIT_COL_NUM;begin_j++){
                    if(m_pFruits[begin_i][begin_j] == nullptr)
                        continue;
                    
                    FruitSprite* fruit = m_pFruits[begin_i - des][begin_j] = m_pFruits[begin_i][begin_j];
                    fruit->setIndex_RowCol(begin_i - des,begin_j);
                    fruit->setDesPosition(getPosByRowCol(begin_i - des,begin_j));
                    m_pFruits[begin_i][begin_j] = nullptr;
                }
            }
        }
    }
}
//检测是否还有可以消灭的水果
bool PopFruitScene::isOver()
{
    bool bRet = true;
    for(int i=0;i<FRUIT_ROW_NUM;i++){
        for(int j=0;j<FRUIT_COL_NUM;j++){
            if(m_pFruits[i][j] == nullptr)
                continue;
            int curColor = m_pFruits[i][j]->getColor();
            
            if(i-1>=0 && m_pFruits[i-1][j]!=nullptr && m_pFruits[i-1][j]->getColor() ==  curColor)
                return false;
            
            if(i+1<FRUIT_ROW_NUM && m_pFruits[i+1][j]!=nullptr && m_pFruits[i+1][j]->getColor() == curColor)
                return false;
            
            if(j-1>=0 && m_pFruits[i][j-1]!=nullptr && m_pFruits[i][j-1]->getColor() == curColor)
                return false;
            
            if(j+1<FRUIT_COL_NUM && m_pFruits[i][j+1]!=nullptr && m_pFruits[i][j+1]->getColor() == curColor)
                return false;
        }
    }
    return bRet;
}
//清除剩下的不能点击的水果
void PopFruitScene::clearFruitOneByOne()
{
    for(int i=FRUIT_ROW_NUM-1;i>=0;i--){
        for(int j=FRUIT_COL_NUM-1;j>=0;j--){
            if(m_pFruits[i][j] == nullptr)
                continue;
            
            m_pFruits[i][j] -> removeFromParentAndCleanup(true);
            m_pFruits[i][j] = nullptr;
            return;
        }
    }
    initFruit();
    needClear = false;
}
//检测是否所有的水果都已经移动到目标位置
bool PopFruitScene::checkFruitIsAdjust()
{
    bool bRet = true;
    for(int i=FRUIT_ROW_NUM-1;i>=0;i--){
        for(int j=FRUIT_COL_NUM-1;j>=0;j--){
            if(m_pFruits[i][j] == nullptr)
                continue;
            
            if(!m_pFruits[i][j]->isCheckPosAndDes())
                return false;
           
        }
    }
    return bRet;
}
PointArray* PopFruitScene::doPointArray(Vec2 startPos,Vec2 endPos,int detal)
{
    float detalX = abs((startPos.x - endPos.x)/4);
    int randI = -1 + 2*detal%2;
    int detalY = rand()%10 + 10 + detal*20;
    
    Vec2 pos = Vec2(startPos.x,startPos.y  + randI* detalY*2);
    Vec2 pos1 = Vec2(startPos.x - detalX - detal*10,startPos.y  + randI* detalY);
    Vec2 pos2 = Vec2(startPos.x - detalX*3 - detal*5,startPos.y - randI* detalY);
    
    PointArray *array = PointArray::create(7);
    array->addControlPoint(startPos);
    array->addControlPoint(pos);
    array->addControlPoint(pos1);
    array->addControlPoint(pos2);
    array->addControlPoint(endPos);
    return array;
}
