//
//  PictureLayer.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "PictureLayer.h"
#include "PackLayer.h"
#include "AppConfigs.h"
#include "UICpp.h"
#include "CBPackManager.h"
#include "GameScene.h"
#include <math.h>
#include <sstream>
#include "AsyncManager.h"
#include "ContinueLayer.h"

#define PICTURE_WIDTH       315
#define PICTURE_HEIGHT      235
#define PAGE_FIRST          1
#define PICTURE_NONE_TAG    0
#define PAGE(x)             (4*(x)-3)
#define PAGE_PICTURE        4


PictureLayer* PictureLayer::create(const int index, const int count)
{
    PictureLayer *pRet = new PictureLayer();
    if (pRet && pRet->init(index, count))
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

PictureLayer::PictureLayer(){
    _packIndex = 0;
    _pageIndex = 1;
    _pageCount = 1;
    _loadIndex = 0;
}

void PictureLayer::initPageData(){
    
    if(_count % PAGE_PICTURE > 0){
        _pageCount = ceil((float)_count/PAGE_PICTURE);
    }
    else
    {
        _pageCount = floor(_count/PAGE_PICTURE);
    }
}

bool PictureLayer::init(const int index, const int count){

    if(Layer::init())
    {
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("choose-2.json"));
        addChild(_root);
        
        _backBtn = quickButton("back-2", _root, CC_CALLBACK_2(PictureLayer::onButton, this));
        _preBtn = quickButton("previous page", _root, CC_CALLBACK_2(PictureLayer::onButton, this));
        _nextBtn = quickButton("next page", _root, CC_CALLBACK_2(PictureLayer::onButton, this));
        
        _categorys.push_back(quickButton("paper-1", _root, CC_CALLBACK_2(PictureLayer::onButton, this)));
        _categorys.push_back(quickButton("paper-3", _root, CC_CALLBACK_2(PictureLayer::onButton, this)));
        _categorys.push_back(quickButton("paper-2", _root, CC_CALLBACK_2(PictureLayer::onButton, this)));
        _categorys.push_back(quickButton("paper-4", _root, CC_CALLBACK_2(PictureLayer::onButton, this)));
        
        ArmatureDataManager::getInstance()->addArmatureFileInfo("Choose_2.ExportJson");
        
        auto pos = Vec2(Director::getInstance()->getWinSize()/2) + Vec2(12, 40);
        
        auto butterfly = quickGetAnimation(0, "Choose_2", pos);
        _root->addChild(butterfly);
        
        auto flower = quickGetAnimation(1, "Choose_2", pos);
        _root->addChild(flower);
        
        setPackIndex(index, count);
        
        //////////////////////////////////////////////////////////// add next scene resource ////////////////////////////////////////////////////////////
        //xAsync->add("mm.json");
        xAsync->add("MM.ExportJson");
        xAsync->add("pen.ExportJson");
        
        xAudio->playMusic("bg/bg_2.mp3");
        
//        auto listener = EventListenerKeyboard::create();
//        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
//        {
//            if(code == EventKeyboard::KeyCode::KEY_BACK)
//            {
//                auto parent = getParent();
//                auto continueLayer = parent->getChildByName("continueLayer");
//                if (continueLayer != nullptr) {
//                    parent->removeChild(continueLayer);
//                    _blockAll = false;
//                    for (auto cava: _categorys) {
//                        cava->setTouchEnabled(true);
//                    }
//                }
//                else
//                {
//                    auto node = Node::create();
//                    node->setName("back-2");
//                    onButton(node, Widget::TouchEventType::ENDED);
//                }
//            }
//        };
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    
    return false;
}

void PictureLayer::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED || _blockAll) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name.compare("back-2") == 0) { 
        _blockAll = true;
        EventCustom event("event_from_picture_to_pack");
        event.setUserData((void*)name.c_str());
        _eventDispatcher->dispatchEvent(&event);
        xAudio->playMusic("bg/bg_1.mp3");
        xAudio->playDefaultSound();
    }
    else if(name.compare("previous page") == 0)
    {
        turnTopage(name);
        xAudio->playDefaultSound();
    }
    else if(name.compare("next page") == 0)
    {
        turnTopage(name);
        xAudio->playDefaultSound();
    }
    else
    {
     
        for (auto cava: _categorys) {
            cava->setTouchEnabled(false);
        }
        
        auto target = ((Widget*)pSender)->getChildren().at(0);
        auto sprite = (Sprite*)(target);
        
        if (PICTURE_NONE_TAG != sprite->getTag())
        {
            nextScene(sprite->getTag());
            xAudio->playSound("sfx/sfx_select.mp3");
        }
        else
        {
            for (auto cava: _categorys) {
                cava->setTouchEnabled(true);
            }
        }
    }
}

void PictureLayer::nextScene(int pageIndex){
    xAds->setVisiable(kTypeBannerAds, false);
    xAdsEx->setBannerShow(false);
    _blockAll = true;
    string filePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex, pageIndex, true);
    bool isExit = FileUtils::getInstance()->isFileExist(filePath);
    log("%s",filePath.c_str());
    log("packindex =  %d  ,  pictureIndex = %d",_packIndex,pageIndex);
    if (isExit) {
        auto scene = ContinueLayer::create(_packIndex, pageIndex);
        Director::getInstance()->getRunningScene()->addChild(scene);
    }
    else
    {
        auto scene = GameScene::create(_packIndex, pageIndex);
        Director::getInstance()->replaceScene(scene);
    }
}

void PictureLayer::turnTopage(const string& name){
    if (name.compare("previous page") == 0 && _pageIndex != PAGE_FIRST)
    {
        _pageIndex--;
        log("previois page");
        setTouchEnable(false);
        turnPageAction("previois page");
    }
    else if(name.compare("next page") == 0 && _pageIndex != _pageCount)
    {
        _pageIndex++;
        log("next page");
        setTouchEnable(false);
        turnPageAction("next page");
    }
}

void PictureLayer::turnPageAction(const string &name){
    if (name.compare("previois page") == 0 )
    {
        book=(ImageView*)Helper::seekWidgetByName(_root, "book ");
        
        tempLeft = (Sprite*)(Helper::seekWidgetByName(_root, "left"))->clone();
        tempLeft->setZOrder(10);
        tempLeft->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        tempLeft->setPosition(tempLeft->getPosition()+Vec2(tempLeft->getContentSize().width/2, 0));
        book->addChild(tempLeft);
        
        tempRight = (Sprite*)(Helper::seekWidgetByName(_root, "right"))->clone();
        tempRight->setZOrder(1);
        book->addChild(tempRight);              // 单独克隆两个imageView 来遮挡已经改变的页面
        
        OrbitCamera* orbit1 = OrbitCamera::create(0.6, 0.3, 0, 0, -90, 0, 0);
        tempLeft->runAction(Sequence::create(CallFunc::create([&](){
            
            loadIofo(PAGE(_pageIndex),PAGE_PICTURE);
            
        }),orbit1, CallFunc::create([&](){
            
            auto temp = (Sprite*)Helper::seekWidgetByName(_root, "right")->clone();
            temp->setPosition(tempLeft->getPosition());
            temp->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            book->addChild(temp,10);
            OrbitCamera* orbit2 = OrbitCamera::create(0.2, 0.3, 0, 270, 90, 0, 0);
            temp->runAction(Sequence::create(orbit2, CallFuncN::create([&](Node* node){
                node->removeFromParent();
                tempRight->removeFromParent();
                setTouchEnable(true);
            }),NULL));
        }), CallFunc::create([&](){
            tempLeft->removeFromParent();
        }),NULL));
    }
    else if(name.compare("next page") == 0)
    {
        book = (ImageView*)Helper::seekWidgetByName(_root, "book ");
        
        tempRight = (Sprite*)(Helper::seekWidgetByName(_root, "right"))->clone();
        tempRight->setZOrder(10);
        tempRight->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        tempRight->setPosition(tempRight->getPosition()-Vec2(tempRight->getContentSize().width/2, 0));
        book->addChild(tempRight);
        
        tempLeft = (Sprite*)(Helper::seekWidgetByName(_root, "left"))->clone();
        tempLeft->setZOrder(1);
        book->addChild(tempLeft);
                                                                            // 单独克隆两个Sprite 来遮挡已经改变的页面,右边的页面是要准备翻页的
        OrbitCamera* orbit1 = OrbitCamera::create(0.6, 0.3, 0, 0, 90, 0, 0);
        tempRight->runAction(Sequence::create(CallFunc::create([&](){
            
            loadIofo(PAGE(_pageIndex),PAGE_PICTURE);
            
        }),orbit1, CallFunc::create([&](){
            
            auto temp = (Sprite*)Helper::seekWidgetByName(_root, "left")->clone();
            temp->setPosition(tempRight->getPosition());
            temp->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
            book->addChild(temp,10);
            OrbitCamera* orbit2 = OrbitCamera::create(0.2, 0.3, 0, 270, 90, 0, 0);
            temp->runAction(Sequence::create(orbit2, CallFuncN::create([&](Node* node){
                tempLeft->removeFromParent();
                node->removeFromParent();
                setTouchEnable(true);
            }),NULL));
        }), CallFunc::create([&](){
            tempRight->removeFromParent();
        }),NULL));
        
    }
}

void PictureLayer::setTouchEnable(bool touch){
    if (!touch) {
        _preBtn->setTouchEnabled(false);
        _nextBtn->setTouchEnabled(false);
        for (auto cava : _categorys) {
            cava->setTouchEnabled(false);
        }
    }
    else
    {
        _preBtn->setTouchEnabled(true);
        _nextBtn->setTouchEnabled(true);
        for (auto cava : _categorys) {
            cava->setTouchEnabled(true);
        }
    }
}


void PictureLayer::loadIofo(const int pageIndex, int times){    // 开始更新的图的编号，更新的图的数量
    PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex - 1);
    if (!pack)
        return;
    
    int index = pageIndex;    //index   12 34    56 78
    if(1 == times)
    {
        _categorysIndex = pageIndex - 1; // 第一次加载书本页面，一页一页加载，不然会卡
    }else
    {
        _categorysIndex = 0;
    }
    for (int i = 0; i < times; ++i)
    {
        PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex - 1);
        std::string pictureName;
        std::string historyLogo;
        
        if (pack->isServerPack)
        {
            pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex, index, true);
        }
        else
        {
            pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex, index, false);
        }
        
        historyLogo = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex, index, true);
        ImageView* logo = nullptr;
        
        bool isExsit = FileUtils::getInstance()->isFileExist(historyLogo);
        if (isExsit)
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey(historyLogo);
            logo = ImageView::create(historyLogo);
            logo->setTag(index);
        }
        else
        {
            if (!isPictrueExit(pictureName))
            {
                logo = ImageView::create();
                logo->setTag(PICTURE_NONE_TAG);
            }
            else
            {
                logo = ImageView::create(pictureName);
                logo->setTag(index);
            }
            
        }
        auto scaleX = PICTURE_WIDTH / logo->getContentSize().width;
        auto scaleY = PICTURE_HEIGHT / logo->getContentSize().height;
        auto scale = scaleX > scaleY ? scaleY : scaleX;
        
        logo->setScale(scale - 0.09);//留出边框
        if(_categorys.at(_categorysIndex)) _categorys.at(_categorysIndex)->removeAllChildrenWithCleanup(true);
        
        _categorys.at(_categorysIndex)->addChild(logo);
        logo->setPosition(logo->getParent()->getContentSize()/2);
        
        index++;
        _categorysIndex++;
    }
}

bool PictureLayer::isPictrueExit(const string &pictureName){
    return FileUtils::getInstance()->isFileExist(pictureName);
}

void PictureLayer::setPackIndex(int index ,int count){
    _packIndex = index;
    _count = count;
    initPageData();
    loadIofo(PAGE_FIRST,PAGE_PICTURE);
    UserDefault::getInstance()->setDoubleForKey("count", _count);
    UserDefault::getInstance()->flush();
}
