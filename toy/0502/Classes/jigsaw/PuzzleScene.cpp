//
//  PuzzleScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//  
#include "PuzzleScene.h"
#include "JPSideBar.h"
#include "../AppGlobal.h"
#include "../AppConfig.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/UICpp.h"
#include "../modules/STSystemFunction.h"
#include "../modules/AnalyticX.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "../utilities/ImageSplitFactory.h"
#include "../utilities/Utils.h"
#include "../utilities/LYString.h"
#include "../gameconfig/GameConfig.h"
#include "../gameconfig/MyGameConfig.h"
#include "../gameconfig/BalloonManager.h"


#define GAME_SCENE_TAG   88
#define GAME_MAP_LAYER_TAG   99
#define MOVE_ANIMATION_TIME 0.15
#define GAME_TOP_LAYER_TAG 1000
#define GAME_BOTTOM_LAYER_TAG 1001
#define PARTICLE_PATH "jigsaw_correct.plist"

#define EXIT_DIALOG_TAG 700

//tag
enum UI_TAG{
    T_FRAME = 120,
    T_SIDEBAR = 141,
    T_LISTVIEW = 255,
};


Scene* PuzzleScene::scene()
{
    auto scene = Scene::create();
    
    auto layer = PuzzleScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool PuzzleScene::init()
{
    if (Layer::init()) {
        
        _sideLen = xGame->getSideLen();
        
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui03_puzzle_1.csb"));
        addChild(_root, 1);
        
        quickButton("ui06_back_0", _root, CC_CALLBACK_2(PuzzleScene::onButton, this));
        
        auto text = (Text*)Helper::seekWidgetByName(_root, "ui03_timebox_label");
        text->setFontName("Times BoldItalic.ttf");
        
        //side bar
        Layout * sidebar = (Layout*)Helper::seekWidgetByTag(_root, T_SIDEBAR);
        //sidebar->setLocalZOrder(10);
        
        _scroll = JPSideBar::create(0.f, 0.f, _root);
        
        _scroll->setContentSize(sidebar->getContentSize());
        _scroll->setBounceEnabled(true);
        _scroll->setDirection(ui::ScrollView::Direction::VERTICAL);
        
        // set all items layout gravity
        _scroll->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        // set items margin
        //_scroll->setItemsMargin(2.0f);
        //_scroll->setPosition(Vec2(sidebar->getContentSize().width/2, sidebar->getContentSize().height/2));
        
        _scroll->setPosition(Vec2(_scroll->getContentSize().width/2, _scroll->getContentSize().height/2));
        _scroll->setClippingEnabled(false);
        sidebar->addChild(_scroll, 100);
        
        
        _frame = (ImageView*)Helper::seekWidgetByTag(_root, T_FRAME);
        
        //将瓦块切割好
        prepareTiles();
        
        schedule(schedule_selector(PuzzleScene::timing), 1, kRepeatForever, 0);
        
        return true;
    }
    
    return false;
}

PuzzleScene* PuzzleScene::create()
{
    PuzzleScene *pRet = new PuzzleScene();
    if (pRet && pRet->init())
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


void PuzzleScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    if (name.compare("ui06_back_0") == 0) {
        xDialog->showQuit("You will lose your current progress if you quit the game,are you sure?", CC_CALLBACK_1(PuzzleScene::onDialog, this));
    }
    else if (name.compare("") == 0) {
        
    }
}


void PuzzleScene::timing(float delta)
{
    if (xDialog->isVisible() == false)
    {
        _seconds++;
        quickSetString("ui03_timebox_label", _root, Util::formatTime(_seconds));
    }
}

void PuzzleScene::prepareTiles()
{
    //兼容以前的config
    ParkDataConfig *infoConfig = (ParkDataConfig*)GameConfig::sharedGameConfig()->getParkConfigs()->objectAtIndex(0);
    
    if(!infoConfig->m_isReady)
        return;
    GameConfig::sharedGameConfig()->setCurrentPack(infoConfig);
    GameConfig::sharedGameConfig()->user_last_pieces_number = _sideLen*_sideLen;
    
    ImageSplitFactory* factory = ImageSplitFactory::sharedImageSplitFactory();
    
    if(_sideLen%2==0)
        factory->loadTileConfigs("TileConfig_s.csv");
    else
        factory->loadTileConfigs("TileConfig_1.csv");
    
    factory->loadRenderSplitByConfigs(Util::getMapFilePath(_sideLen).c_str());
    
    //前2个参数是包名+张数名, 后面合为1个参数.
    factory->splitImage(xGame->getFullFileName().c_str(), sprites);
    
    row = factory->row;
    column = factory->column;
    int count = 0;
    
    Vec2 topleft = _frame->convertToWorldSpaceAR(Vec2::ZERO);
    topleft = convertToNodeSpace(topleft);
    
    //12 和 9 是 线框的偏移量
    topleft.x -= _frame->getContentSize().width/2 - 12;
    topleft.y += _frame->getContentSize().height/2 - 13;
    
    factory->resetOriginalPosition(sprites, topleft);
    _scroll->setEndScale(sprites.at(0)->getScale());
    factory->recycle();
    
    //乱序
    random_shuffle(sprites.begin(), sprites.end());
    
    float width = _scroll->getContentSize().width;
    for (auto tile : sprites) {
        tile->setTag(count);
        tile->isTouch = true;
        
        tile->recoverCenterAnchpoint();     //不知道什么地方改变了锚点, 莫名其妙.
        
        //添加到ScrollView 里面, 不然会挂掉.
        Layout * layout = Layout::create();
        layout->setContentSize(Size(width, width * 0.8));
#if 0
        tile->setPosition(tile->getOriginalPoint());
        quickLogPosition(tile);
        addChild(tile, 1);
        
        auto size = tile->getContentSize();
        Color4B clr;
        
        int flag = count %4;
        if (flag == 0) {
            clr = Color4B::GRAY;
        }
        else if (flag == 1)
        {
            clr = Color4B::GREEN;
        }
        else if (flag == 2)
        {
            clr = Color4B::ORANGE;
        }
        else if (flag == 3)
        {
            clr = Color4B::BLUE;
        }
        
        LayerColor * color = LayerColor::create(clr, size.width, size.height);
        
        Vec2 mid = V::getRealPositionForDesignPosition(topleft);
        Size si = _frame->getContentSize();
        
        //以宽度作为标准
        //CCASSERT(si.height == si.width, "");
        DrawNode *draw = DrawNode::create();
        
        float half = si.width/2 - 6;
        
        draw->drawSegment(Vec2(mid.x, mid.y + half), Vec2(mid.x, mid.y - half), 1, Color4F::RED);
        draw->drawSegment(Vec2(mid.x - half, mid.y), Vec2(mid.x + half, mid.y), 1, Color4F::RED);
        draw->drawSegment(Vec2(mid.x - half, mid.y + half), Vec2(mid.x - half, mid.y - half), 1, Color4F::RED);
        draw->drawSegment(Vec2(mid.x + half, mid.y + half), Vec2(mid.x + half, mid.y - half), 1, Color4F::RED);
        draw->drawSegment(Vec2(mid.x - half, mid.y + half), Vec2(mid.x + half, mid.y + half), 1, Color4F::RED);
        draw->drawSegment(Vec2(mid.x - half, mid.y - half), Vec2(mid.x + half, mid.y - half), 1, Color4F::RED);
        //addChild(draw, 100);
        
        if (count  == 0 || count == 3)
        {
            //tile->addChild(color);
        }

#else
        tile->setScale(0.2 * _sideLen / 2);
        tile->setStartScale(tile->getScale());
        tile->setPosition(Vec2(width/2, width/2));
        
        float sideBarBorder = _scroll->getWorldPosition().x;
        tile->setSideBarBorder(sideBarBorder, _sideLen);
        tile->setJPSideBar(_scroll);
        
        layout->addChild(tile);
        _scroll->addChild(layout);
#endif
        count++;
    }
    
    _compare_distance = _compare_distance * 2 / _sideLen;
    count = 0;
    
    cur_pic_index = 0;
    
    //_frame->setScale(1.02);
}

void PuzzleScene:: onExit()
{
    log("unschedule");
    unschedule(schedule_selector(PuzzleScene::timing));
    unscheduleAllSelectors();
    
    Layer::onExit();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}


//这个方法应该放到TileSprite里面的.

void PuzzleScene::checkCommbine(TileSprite *sprite)
{
    if(isVicotry)
        return;
    
    if(((int)sprite->getRotation())%360!=0)
        return;
    
    log("%d", sprite->getGroupIndex());
    if(sprite->getGroupIndex()==-1){
        checkSingleSpriteCombine(sprite);
        checkIsSuccess();
    }
    else
    {
        //循环检测group中所有瓦片
        for (vector<TileSprite*>::iterator tileSprite = groupArray[sprite->getGroupIndex()].begin(); tileSprite!=groupArray[sprite->getGroupIndex()].end(); tileSprite++)
        {
            if(checkSingleSpriteCombine(*tileSprite))
                break;
        }
        checkIsSuccess();
    }
    
    
}

bool  PuzzleScene::isCoundInsertMap(int index){
    map<int, int>::iterator   my_Itr;
    //    for   (my_Itr=map_lock_tile.begin();   my_Itr!=map_lock_tile.end();   ++my_Itr)   {
    //        log("………………………………………………………… %d", my_Itr->first);
    //    }
    
    int top = index  - column;
    if(top >= 0 && map_lock_tile.find(top) != map_lock_tile.end())
    {
        return true;
    }
    
    int left = index - 1;
    if(left >= 0  )
    {
        if(left/column == index/column)
        {
            if(map_lock_tile.find(left) != map_lock_tile.end())
                return true;
        }
        
    }
    
    int right = index + 1;
    if(right <= row*column  && right/column == index/column && map_lock_tile.find(right) != map_lock_tile.end())
    {
        return true;
    }
    
    int bottom = index + column;
    if(bottom <= row*column  &&  map_lock_tile.find(bottom) != map_lock_tile.end())
    {
        return true;
    }
    return false;
}

//上锁处理
void PuzzleScene::lock_deal(TileSprite *sprite ,float frameDelayX, float frameDelayY){
    if(sprite->getGroupIndex()  == -1)
    {
        //不知道为什么, 用moveTo, 就有问题. 难道是move时间太少了?
        sprite->setPosition(sprite->getOriginalPoint());
//        sprite->runAction(CCMoveTo::create(MOVE_ANIMATION_TIME, sprite->getOriginalPoint()));
        sprite->lockSprite();
        map_lock_tile.insert(map<int,   int>::value_type(sprite->get_m_Index(),1));
        
        map<int, int>::iterator   my_Itr;
        
    }else{
        for (vector<TileSprite*>::iterator tileSprite = groupArray[sprite->getGroupIndex()].begin(); tileSprite!=groupArray[sprite->getGroupIndex()].end(); tileSprite++)
        {
            (*tileSprite)->runAction(CCMoveTo::create(MOVE_ANIMATION_TIME, (*tileSprite)->getOriginalPoint()));
            (*tileSprite)->lockSprite();
            map_lock_tile.insert(map<int,   int>::value_type((*tileSprite)->get_m_Index(),1));
        }
        
    }
    
    xAudio->playSound("sfx_jigsaw_correct.mp3");
}

bool PuzzleScene::checkSingleSpriteCombine(TileSprite *sprite)
{
    
    Vec2 targetSpritePoint = sprite->getLeftTopPoint();
    
    //是否在白色框框内
    float frameDelayX = targetSpritePoint.x - sprite->getCombinePoint().x;
    float frameDelayY = targetSpritePoint.y - sprite->getCombinePoint().y;
    log("frameDelayX=%f   frameDelayY=%f",frameDelayX , frameDelayY);
    //简单模式
    if(false){
        if(abs(frameDelayX)<_compare_distance && abs(frameDelayY)<_compare_distance)
        {
            lock_deal(sprite ,-frameDelayX, -frameDelayY);
            return true;
        }
        
    }
    else{
        //定位在 白色frame内
        if(checkIsLock(sprite))
        {
            CCParticleFlower  *m_emitter = CCParticleFlower::create();
            m_emitter->retain();
            
            addChild(m_emitter, 100);
            
            m_emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("Animation/stars2.png"));
            // duration
            m_emitter->setDuration(0);
            
            // gravity
            //    m_emitter->setGravity();
            
            // angle
            m_emitter->setAngle(90);
            m_emitter->setAngleVar(360);
            
            // speed of particles
            m_emitter->setSpeed(300* 6.f/ _sideLen);
            m_emitter->setSpeedVar(250);
            
            m_emitter->setStartSize(20*6.f - _sideLen*15);
            // life of particles
            m_emitter->setLife(0.4);
            m_emitter->setLifeVar(0.2);
            
            // color of particles
            ccColor4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
            m_emitter->setStartColor(startColor);
            
            //    ccColor4F startColorVar = {0.5f, 0.5f, 0.5f, 1.0f};
            //    m_emitter->setStartColorVar(startColorVar);
            
            ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
            m_emitter->setEndColor(endColor);
            
            //    ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};
            //    m_emitter->setEndColorVar(endColorVar);
            
            // emits per second
            m_emitter->setEmissionRate(m_emitter->getTotalParticles()*3);
            
            // additive
            m_emitter->setBlendAdditive(false);
            
            
            m_emitter->setPosition(sprite->getPosition());
            
            //lock_deal(sprite ,-frameDelayX, -frameDelayY);
            
            return true;
        }
    }
    
    return false;
    
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        //        log("正在检测 %d" , (*tileSprite)->getTag());
        //比较瓦片已经固定在地图中  则无需比较
        if((*tileSprite)->isLock())
            continue;
        
        if(((int)(*tileSprite)->getRotation())%360!=0)
            continue;
        
        //若用户选择的瓦片与比较瓦片相同  则返回
        if((*tileSprite)->getTag() == sprite->getTag())
            continue;
        //若用户选择的瓦片已经在该group中  则返回
        if((*tileSprite)->getGroupIndex()!=-1 && (*tileSprite)->getGroupIndex() == sprite->getGroupIndex())
            continue;
        
        int tag = sprite->getTag();  //只做上下左右
        if((*tileSprite)->getTag()!= (tag+1) && (*tileSprite)->getTag()!= (tag-1) && (*tileSprite)->getTag() != tag - column&& (*tileSprite)->getTag()!= tag+column)
            continue;
        
        if(((*tileSprite)->getTag()/column != tag/column) && (((*tileSprite)->getTag()== (tag+1)) || (*tileSprite)->getTag()== (tag-1)))
            continue;
        
        Vec2 leftTopPoint = (*tileSprite)->getLeftTopPoint();
        
        float delayX = targetSpritePoint.x - leftTopPoint.x;
        float delayY = targetSpritePoint.y - leftTopPoint.y;
        //        log("delayx = %f  delayy = %f" , delayX , delayY);
        //在地图上  瓦片与瓦片的距离  和在移动后  瓦片与瓦片的距离  比较
        float originalDelayX1  =  sprite->getCombinePoint().x - (*tileSprite)->getCombinePoint().x;
        float originalDelayY1  =  sprite->getCombinePoint().y - (*tileSprite)->getCombinePoint().y;
        //        log("originalDelayX = %f  originalDelayY = %f" , originalDelayX , originalDelayY);
        
        float distanceDelayX1 = delayX - originalDelayX1;
        float distanceDelayY1 = delayY - originalDelayY1;
        
        if(abs(distanceDelayX1)<_compare_distance && abs(distanceDelayY1)<_compare_distance)
        {
            if(CCUserDefault::sharedUserDefault()->getBoolForKey(GAME_SOUND_STATUS, true)){
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(sound_combine, false);
            }
            //            log("靠近, 可以合并");
            //            CCLOG("targetSpritePoint.x=%f",targetSpritePoint.x);
            //            log("delayx = %f  delayy = %f" , delayX , delayY);
            //            log("originalDelayX = %f  originalDelayY = %f" , originalDelayX1 , originalDelayY1);
            //合并在这里
            
            float originalDelayX  =  abs(sprite->getCombinePoint().x - (*tileSprite)->getCombinePoint().x);
            float originalDelayY  =  abs(sprite->getCombinePoint().y - (*tileSprite)->getCombinePoint().y);
            
            float distanceDelayX = abs(delayX) - originalDelayX;
            float distanceDelayY = abs(delayY) - originalDelayY;
            
            
            float moveX , moveY;
            if(delayX > 0){
                moveX = (distanceDelayX) >0 ? -abs(distanceDelayX):abs(distanceDelayX);
            }else{
                moveX = (distanceDelayX) >0 ? abs(distanceDelayX):-abs(distanceDelayX);
            }
            
            //固定用户选择的瓦片， 通过瓦片在其上  和  在其下 两种情况进行运算
            if(delayY<0)
            {
                if(distanceDelayY<0)
                {
                    moveY  = -abs(distanceDelayY);
                }else{
                    moveY = abs(distanceDelayY);
                }
            }else{
                if(distanceDelayY>0)
                {
                    moveY = -abs(distanceDelayY);
                }else{
                    moveY = abs(distanceDelayY);
                }
            }
            //target 被拖动瓦片移动 不是集合
            if(sprite->getGroupIndex()==-1)
                sprite->runAction(CCMoveBy::create(MOVE_ANIMATION_TIME, Vec2(moveX, moveY)));
            else
            {
                //说明被拖动瓦片是一个集合
                for(vector<TileSprite*>::iterator iter = groupArray[sprite->getGroupIndex()].begin(); iter != groupArray[sprite->getGroupIndex()].end() ; iter++)
                {
                    (*iter)->runAction(CCMoveBy::create(MOVE_ANIMATION_TIME, Vec2(moveX, moveY)));
                    
                }
                (*tileSprite)->setGroupIndex(sprite->getGroupIndex());
            }
            
            //是否添加到已有的group
            
            int groupIndex = 0;
            int targetGroupIndex = -1;
            int tileSpriteGroupIndex = -1;
            for(vector< vector<TileSprite*> >::iterator iter = groupArray.begin(); iter != groupArray.end() ; iter++)
            {
                
                for(vector<TileSprite*>::iterator iterVector = (*iter).begin(); iterVector != (*iter).end() ; iterVector++)
                {
                    //如果group上有tag和 被拖动瓦片的tag相同的  说明 别拖动的瓦片本身就是一个group 集合
                    if((*iterVector)->getTag()==sprite->getTag() ){
                        
                        //                        log("被拖动瓦片是集合");
                        //                        (*iterVector)->groupIndex = groupIndex;
                        targetGroupIndex = groupIndex;
                        break;
                    }
                    //如果group上有tag和 对比的瓦片的tag相同的  说明对比的瓦片本身就是一个group 集合
                    if((*iterVector)->getTag() == (*tileSprite)->getTag())
                    {
                        //                        log("对比瓦片是集合");
                        //                        sprite->groupIndex = groupIndex;
                        
                        tileSpriteGroupIndex = groupIndex;
                        break;
                    }
                }
                
                groupIndex++;
            }
            
            
            if(targetGroupIndex != -1 && tileSpriteGroupIndex != -1)
            {
                //                log("合并group");  //将被比较的瓦片 合并到 拖拉瓦片中
                for(vector<TileSprite*>::iterator iter = groupArray[tileSpriteGroupIndex].begin(); iter != groupArray[tileSpriteGroupIndex].end() ; iter++)
                {
                    (*iter)->setGroupIndex(targetGroupIndex);
                    
                    groupArray[targetGroupIndex].push_back((*iter));
                }
                groupArray[tileSpriteGroupIndex].clear();
                
            }else if(targetGroupIndex!=-1){
                //                log("添加到已有的  被拖动瓦片集合");
                (*tileSprite)->setGroupIndex(targetGroupIndex);
                groupArray[targetGroupIndex].push_back((*tileSprite));
            }else if(tileSpriteGroupIndex!=-1){
                //                log("添加到已有的  对比瓦片集合");
                sprite->setGroupIndex( tileSpriteGroupIndex);
                groupArray[tileSpriteGroupIndex].push_back(sprite);
            }else{
                //                log("添加新group");
                (*tileSprite)->setGroupIndex(groupIndex);
                sprite->setGroupIndex(groupIndex);
                vector<TileSprite*> newGroup;
                newGroup.push_back((*tileSprite));
                newGroup.push_back(sprite);
                groupArray.push_back(newGroup);
                
            }
            return true;
        }
    }
    return false;
}



void PuzzleScene::moveGroupSprite(Vec2 delayPointDelay, int groupIndex , int m_index)
{
    if(curMoveGroupMap.size() != 0){
        int count = 0;
        for (map<int, int>::iterator i= curMoveGroupMap.begin();
             i!=curMoveGroupMap.end(); i++)
        {
            if( i->first == groupIndex)
            {
                count++;
                if(i->second == m_index)
                    break;
                else
                    return;
            }
            
        }
        if(count == 0){
            curMoveGroupMap.insert(map<int , int>::value_type(groupIndex , m_index));
        }
        
    }else{
        curMoveGroupMap.insert(map<int , int>::value_type(groupIndex , m_index));
    }
    
    for(vector<TileSprite*>::iterator iter = groupArray[groupIndex].begin(); iter != groupArray[groupIndex].end() ; iter++)
    {
        (*iter)->setPosition(ccpAdd((*iter)->getPosition(),delayPointDelay)) ;
    }
}


void PuzzleScene::removeGroupMap(int groupIndex)
{
    if(curMoveGroupMap.size()!=0)
        curMoveGroupMap.erase(groupIndex);
}

void PuzzleScene::removeCurHuitanGroupMap(int groupIndex)
{
    if(curHuitanGroupMap.size()!=0)
        curHuitanGroupMap.erase(groupIndex);
}

void PuzzleScene::setGroupTouchPriority(int groupIndex)
{
    
    for(vector<TileSprite*>::iterator iter = groupArray[groupIndex].begin(); iter != groupArray[groupIndex].end() ; iter++)
    {
        (*iter)->setMyPriority();
    }
}

void PuzzleScene::groupToTop(int groupIndex)
{
    for(vector<TileSprite*>::iterator iter = groupArray[groupIndex].begin(); iter != groupArray[groupIndex].end() ; iter++)
    {
        (*iter)->goTop();
    }
}

void PuzzleScene::runMoveAction()
{
    //    CCLOG("UI_SCALE=%f",UI_SCALE);
    
    if(row == 2){
        
        int i=0;
        
        for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
        {
            int delay = getRandomFromRange(-80, 80 , 0);;
            int baseMoveDistance =(*tileSprite)->getContentSize().width*0.7;
            (*tileSprite)->isTouch = false;
            if(i==0){
                (*tileSprite)->runAction(CCMoveBy::create(0.4, Vec2(baseMoveDistance+delay, -baseMoveDistance+delay)));
                
            }else if(i==1){
                (*tileSprite)->runAction(CCMoveBy::create(0.4, Vec2(-baseMoveDistance+delay, -baseMoveDistance+delay)));
            }else if(i==2){
                (*tileSprite)->runAction(CCMoveBy::create(0.4, Vec2(baseMoveDistance+delay, baseMoveDistance+delay)));
                
            }else{
                (*tileSprite)->runAction(CCMoveBy::create(0.3, Vec2(-baseMoveDistance+delay, baseMoveDistance+delay)));
            }
            i++;
        }
        return ;
        
    }
    
    int maxY1 = (VISIBLE_RECT.getMaxY() - 1024)/2 - 104;
    int maxY2 = VISIBLE_RECT.getMaxY();
    int minY2 = maxY1 + 1024 + 3*104;
    int maxX = VISIBLE_RECT.getMaxX()-164;
    
    int i = 0;
    
    srand((int)time(0));
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        (*tileSprite)->isTouch = false;
        int x = getRandomFromRange(164, maxX , 0);;
        
        int y1 = getRandomFromRange(0, maxY1 , 0);
        //        log("x=%d y = %d" , x ,y);
        if(i % 2 == 0)
            (*tileSprite)->runAction(CCMoveTo::create(0.4, Vec2(x, y1)));
        
        i++;
    }
    srand((int)time(0)+200);
    i =0;
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        int x = getRandomFromRange(164, maxX , 0);;
        
        int y2 = getRandomFromRange(minY2, maxY2 , 0);
        //        log("x=%d y = %d" , x ,y);
        if(i % 2 == 1){
            (*tileSprite)->runAction(CCMoveTo::create(0.4, Vec2(x, y2)));
            
        }
        
        i++;
    }
    
    scheduleOnce(schedule_selector(PuzzleScene::runRotateAction), 0.4);
    
}

void PuzzleScene::runRotateAction(float dt)
{
    
    srand((int)time(0)+200);
    int i =0;
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        int randomIndex = getRandomFromRange(0, 3, 0);
        if(GameConfig::sharedGameConfig()->game_rotated_status == 1 && randomIndex != 0){
            (*tileSprite)->runAction(CCRotateBy::create(0.2, randomIndex*90%360));
        }
        
        
        i++;
    }
}
bool  PuzzleScene::checkIsSuccess()
{
    
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        if((*tileSprite)->getTag() != LOCK_STATUS ){
            //            log("不是定位在 %d " ,(*tileSprite)->get_m_Index());
            return false;
        }
    }
    //    log("完成本关");
    
    scheduleOnce(schedule_selector(PuzzleScene::victory), 0.8);
    isVicotry = true;
    return true;
}

void PuzzleScene::victory(float dt)
{
    string path;
    path = xGame->getFullFileName();
    
    if(CCUserDefault::sharedUserDefault()->getBoolForKey(GAME_SOUND_STATUS, true)){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(sound_vitory, false);
    }
    GET_RESUME_DATA->isResumeMode = false;
    
    string time = Util::formatTime(_seconds);
    
    int starCount;
    if (_seconds < 100) {
        starCount = 3;
    }
    else if (_seconds < 180) {
        starCount = 2;
    }
    else
        starCount = 1;

    xDialog->showOver(time, starCount, CC_CALLBACK_1(PuzzleScene::onDialog, this));
    
    xBalloon->start();
}

void PuzzleScene::onDialog(const string& name)
{
    xAudio->playDefaultSound();
    
    if (name.compare("ui07_btn_quit") == 0) {
        
        unscheduleAllSelectors();
        xScene->back(S_RECORD);
        xBalloon->stop();
    }
    else if (name.compare("ui07_btn_close") == 0 || name.compare("ui07_close") == 0)
    {
        xDialog->hide();
    }
    else if (name.compare("ui07_btn_replay") == 0) {
        //把地图上面的方块 加到sidebar 上面, 后面来优化
        
        random_shuffle(sprites.begin(), sprites.end());
        float width = _scroll->getContentSize().width;
        int count = 0;
        for (auto tile : sprites) {
            tile->retain();
            tile->removeFromParent();
            
            tile->setTag(count);
            tile->isTouch = true;
            
            tile->recoverCenterAnchpoint();     //不知道什么地方改变了锚点, 莫名其妙.
            
            //添加到ScrollView 里面, 不然会挂掉.
            Layout * layout = Layout::create();
            layout->setContentSize(Size(width, width * 0.8));
            
            tile->setScale(0.2 * _sideLen / 2);
            tile->setStartScale(tile->getScale());
            tile->setPosition(Vec2(width/2, width/2));
            
            float sideBarBorder = _scroll->getWorldPosition().x;
            tile->setSideBarBorder(sideBarBorder, _sideLen);
            tile->setJPSideBar(_scroll);
            
            layout->addChild(tile);
            _scroll->addChild(layout);
            count++;
            
            tile->release();
            
            //这里要和开始时一致, 包括乱序
        }
        
        _seconds = 0;
        
        xDialog->hide();
        xBalloon->stop();
        
        isVicotry = false;
    }
    else if (name.compare("ui07_btn_yes") == 0) {
        ImageSplitFactory::destoryInstance();
        recycleRes();
        
        unscheduleAllSelectors();
        xScene->back(S_RECORD);
    }
}


void PuzzleScene::removeBatchNode()
{
}

void PuzzleScene::pauseGame()
{
}

void PuzzleScene::resumeGame()
{
}

void PuzzleScene::recycleRes()
{
    if(!map_lock_tile.empty())
        map_lock_tile.clear();
    
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        (*tileSprite)->removeFromParent();
    }
    sprites.clear();
    
    removeAllChildrenWithCleanup(true);
    
    //    log("rt_batch count =%d",rt_batch->retainCount());
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

int  PuzzleScene::getRandomFromRange(int a , int b , int index)
{
    if(index!=0)
        srand(index);
    return (rand()%(b-a+1))+a;
}

bool PuzzleScene::SortByM1(TileSprite *v1, TileSprite *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return v1->get_m_Index() < v2->get_m_Index();//升序排列
}

void PuzzleScene::saveUserResumeData(int time)
{
    //若需要保存数据，则对于数组进行排序，适配 resume方法
    std::sort(sprites.begin(),sprites.end(),SortByM1);
    int successCount = 0;
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        if((*tileSprite)->getTag() == LOCK_STATUS ){
            successCount++;
        }
    }
    //    CCLOG("successCount=%d",successCount);
    //只有符合保存条件才可以保存  暂定在正确位置一个以上
    if(successCount>0){
        
        std::string userDataString;
        for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
        {
            userDataString.append(Util::convertInt((*tileSprite)->getTag()));
            userDataString.append(",");
            userDataString.append(Util::convertFloat((*tileSprite)->getPositionX()));
            userDataString.append(",");
            userDataString.append(Util::convertFloat((*tileSprite)->getPositionY()));
            userDataString.append(",");
            userDataString.append(Util::convertInt((*tileSprite)->get_m_Index()));
            userDataString.append(",");
            userDataString.append(Util::convertInt((*tileSprite)->getGroupIndex()));
            userDataString.append(",");
            userDataString.append(Util::convertInt((*tileSprite)->isInMap));
            userDataString.append(";");
        }
//        GameConfig::sharedGameConfig()->saveUserGameData(cur_pic_index==201 ,userDataString , cur_pic_index , time , groupArray.size() , (int)rotateStatus , _sideLen);
        
        
    }else{
        //则删除prepare照片
        remove(GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_IMAGE_NAME).c_str());
        remove(GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_IMAGE_THUMB_NAME).c_str());
    }
    
    
    GET_RESUME_DATA->isResumeMode = false;
}

void PuzzleScene::saveUserVictoryData()
{
    if(GameConfig::sharedGameConfig()->isShowResumeStatus(cur_pic_index)){
        GameConfig::sharedGameConfig()->removeResumeModeData();
    }
    GameConfig::sharedGameConfig()->saveUserVictoryData(cur_pic_index, "");
}

void PuzzleScene::resumeUserData()
{
    ResumeData  *resumeData ;
    if (cur_pic_index == 201) {
        resumeData = GET_GAME_CONFIG->getUserSelectPictureResumeData();
    }else{
        resumeData = GET_RESUME_DATA;
    }
    for(int i=0;i< resumeData->resume_group_count ; i++){
        
        vector<TileSprite*> newGroup;
        groupArray.push_back(newGroup);
    }
    
    
    cocos2d::liyang::LYString *problem_ids_str_ = cocos2d::liyang::LYString::create(resumeData->resume_data_str);
    CCArray *array =  problem_ids_str_->componentsSeparatedByString(";");
    
    int count = 0;
    for(int i= 0; i<array->count() ; i++)
    {
        Ref  *obj =  array->objectAtIndex(i);
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)obj;
        log("%s" , problem_id_string->getCString());
        
        
        CCArray *array1 = problem_id_string->componentsSeparatedByString(",");
        for(int j= 0; j<array1->count() ; j++)
        {
            Ref  *obj1 =  array1->objectAtIndex(j);
            cocos2d::liyang::LYString *problem_id_string1 = (cocos2d::liyang::LYString*)obj1;
            
            if(j==0){
                if(problem_id_string1->intValue() == LOCK_STATUS){
                    
                    //                    float separationSpace = sprites[count]->getContentSize().width*(_sideLen/6)+50*UI_SCALE;
                    //                    scroll_layer->setContentSize(Size(scroll_layer->getContentSize().width - separationSpace, scroll_layer->getContentSize().height));
                    //
                    //                    sprites[count]->retain();
                    //                    scroll_layer->removeChild(sprites[count], true);
                    //
                    //                    addChild(sprites[count]);
                    //
                    sprites[count]->lockSprite();
                    //                    sprites[count]->setScale(1.0f);
                }
            }else
                if(j == 1){
                    sprites[count]->setPositionX(problem_id_string1->floatValue());
                }else if(j == 2){
                    sprites[count]->setPositionY(problem_id_string1->floatValue());
                }else if(j == 3){
                    sprites[count]->set_m_Index(problem_id_string1->intValue());
                }else if(j == 4){
                    int group_index = problem_id_string1->intValue();
                    sprites[count]->setGroupIndex(group_index);
                    if(group_index!=-1){
                        groupArray[group_index].push_back(sprites[count]);
                    }
                    if(sprites[count]->isLock()){
                        map_lock_tile.insert(map<int,   int>::value_type(sprites[count]->get_m_Index(),1));
                    }
                }else if(j == 5){
                    ////                    log("roate = %d  isrot=%d" , problem_id_string1->intValue() ,resumeData->resume_rotate);
                    ////                    if(problem_id_string->intValue()!=0)
                    ////                        sprites[count]->setRotation(problem_id_string->intValue());
                    //                    if(problem_id_string1->intValue()!=0){
                    //                        sprites[count]->setRotation(problem_id_string1->intValue());
                    //                    }
                    //                    if(resumeData->resume_rotate == rotated){
                    //                        sprites[count]->supportRotate(true);
                    //                        rotateStatus = rotated;
                    //                    }
                    if(problem_id_string->intValue()==1){
                        //                        float separationSpace = sprites[count]->getContentSize().width*(_sideLen/6)+50*UI_SCALE;
                        //                        scroll_layer->setContentSize(Size(scroll_layer->getContentSize().width - separationSpace, scroll_layer->getContentSize().height));
                        //                        Vec2 worldPoint = scroll_layer->convertToWorldSpace(sprites[count]->getPosition());
                        //
                        //                        sprites[count]->retain();
                        //                        scroll_layer->removeChild(sprites[count], true);
                        //
                        //                        addChild(sprites[count]);
                        //                        sprites[count]->setPosition(worldPoint);
                        //                        sprites[count]->openTouchEvent();
                    }
                }
            
        }
        count++;
    }
    
    for (vector<TileSprite*>::iterator tileSprite = sprites.begin(); tileSprite!=sprites.end(); tileSprite++)
    {
        (*tileSprite)->isTouch = false;
    }
    
}

//边界检测   若超出则回弹
void PuzzleScene::checkGroupBorder(int groupIndex , int m_index)
{
    
    
    
    removeGroupMap(groupIndex);
    
    
    if(curHuitanGroupMap.size() != 0){
        int count = 0;
        for (map<int, int>::iterator i= curHuitanGroupMap.begin();
             i!=curHuitanGroupMap.end(); i++)
        {
            if( i->first == groupIndex)
            {
                //                count++;
                //                if(i->second == m_index)
                //                    break;
                //                else
                //                    return;
            }
            
        }
        if(count == 0){
            curHuitanGroupMap.insert(map<int , int>::value_type(groupIndex , m_index));
        }
        
    }else{
        curHuitanGroupMap.insert(map<int , int>::value_type(groupIndex , m_index));
    }
    
    
    
    float minX =0 ,minY =0 , maxX = 0, maxY = 0;
    int i = 0;
    for (vector<TileSprite*>::iterator tileSprite = groupArray[groupIndex].begin(); tileSprite!=groupArray[groupIndex].end(); tileSprite++)
    {
        if(i == 0){
            minX = (*tileSprite)->getPositionX();
            maxX= (*tileSprite)->getPositionX();
            minY = (*tileSprite)->getPositionY();
            maxY = (*tileSprite)->getPositionY();
        }else{
            
            if( (*tileSprite)->getPositionX() < minX)
                minX = (*tileSprite)->getPositionX();
            else if((*tileSprite)->getPositionX() > maxX)
                maxX = (*tileSprite)->getPositionX();
            
            
            if( (*tileSprite)->getPositionY() < minY)
                minY = (*tileSprite)->getPositionY();
            else if((*tileSprite)->getPositionY() > maxY)
                maxY = (*tileSprite)->getPositionY();
            
        }
        i++;
    }
    
    float midX = (maxX - minX)/2 + minX;
    float midY = (maxY - minY)/2 + minY;
    float moveX = maxX - minX;
    if(moveX<=60)
        moveX = 100*ImageSplitFactory::sharedImageSplitFactory()->mScale;
    float moveY = maxY - minY;
    if(moveY <= 60)
        moveY = 100*ImageSplitFactory::sharedImageSplitFactory()->mScale;
    Vec2 point = Util::checkBorder(midX  , midY , moveX , moveY);
    if(point.x!=0 || point.y !=0)
    {
        for (vector<TileSprite*>::iterator tileSprite = groupArray[groupIndex].begin(); tileSprite!=groupArray[groupIndex].end(); tileSprite++)
        {
            (*tileSprite)->kickBackAction(point);
        }
    }
}

int PuzzleScene::getGroupMaxY(int groupIndex)
{
    float maxY = 0;
    for (vector<TileSprite*>::iterator tileSprite = groupArray[groupIndex].begin(); tileSprite!=groupArray[groupIndex].end(); tileSprite++)
    {
        if((*tileSprite)->getPositionY() > maxY)
            maxY = (*tileSprite)->getPositionY();
        
    }
    return maxY;
}

bool PuzzleScene::isInGameItem(int tag){
    ResumeData  *resumeData = GET_RESUME_DATA;
    
    
    cocos2d::liyang::LYString *problem_ids_str_ = cocos2d::liyang::LYString::create(resumeData->resume_data_str);
    CCArray *array =  problem_ids_str_->componentsSeparatedByString(";");
    
    bool inResumeItem = true;
    for(int i= 0; i<array->count() ; i++)
    {
        Ref  *obj =  array->objectAtIndex(i);
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)obj;
        log("正在比较%s" , problem_id_string->getCString());
        
        if (problem_id_string->compare("") == 0) {
            break;
        }
        
        CCArray *array1 = problem_id_string->componentsSeparatedByString(",");
        
        Ref  *obj1 =  array1->objectAtIndex(0);
        cocos2d::liyang::LYString *problem_id_string1 = (cocos2d::liyang::LYString*)obj1;
        
        if (problem_id_string1->intValue() == tag) {
            inResumeItem = false;
            Ref  *obj5 =  array1->objectAtIndex(5);
            cocos2d::liyang::LYString *problem_id_string5 = (cocos2d::liyang::LYString*)obj5;
            if (problem_id_string5->intValue() == 1) {
                return true;
            }
        }
    }
    
    if (inResumeItem) {
        return true;
    }
    return false;
}


bool PuzzleScene::checkIsLock(TileSprite * tile)
{
    log("%f %f", tile->getPosition().x, tile->getPosition().y);
    log("%f %f", tile->getOriginalPoint().x, tile->getOriginalPoint().y);
    
    Vec2 offset = tile->getPosition() - tile->getOriginalPoint();
    
    if (abs(offset.x) + abs(offset.y) < _compare_distance) {
        lock_deal(tile, 0, 0);
        
        return true;
    }
    
    return false;
}


