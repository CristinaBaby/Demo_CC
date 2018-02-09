//
//  MyScene.cpp
//  ColorBook
//
//  Created by huangwen on 15/8/12.
//
//

#include "../Scene/CBPictureScene.h"
#include "../Model/CBUtils.h"
#include "../Model/AppConfigs.h"
#include "../Model/PackManager.h"
#include "../Scene/CBPackScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBAppManager.h"
#include "../Scene/GameScene.h"
#include "CBAppManager.h"
#include "UICpp.h"

USING_NS_ST;

#define TAG_PAGE_CONTROLLER  1234

#define RES_BG               "pack/bg57.png"

#define RES_CONTROL_NORMAL   "choose/ui03_page_dot2.png"
#define RES_CONTROL_SELECT   "choose/ui03_page_dot1.png"

#define RES_BACK_BUTTON      "public/back.png"
#define PACK_BG_WIDTH         382
#define PACK_BG_HEIGHT        466

PictureScene::~PictureScene(){
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* PictureScene::createSceneWithPackIndex(const int packIndex){
    Scene* scene = Scene::create();
    PictureScene* layer = create();
    layer->setPackIndex(packIndex);
    scene->addChild(layer);
    return scene;
}

bool PictureScene::init(){
    if (!Layer::init()) {
        return  false;
    }
    
    //back ground
    Sprite *background = Sprite::create(RES_BG);
    background->setPosition(Utils::getInstance()->getScreenCenter());
    addChild(background);
    
    //back
    auto winSize = Director::getInstance()->getWinSize();
    _back = ui::Button::create(RES_BACK_BUTTON);
    _back->setTag(999);
    _back->addTouchEventListener(CC_CALLBACK_2(PictureScene::touchDown, this));
    _back->setPosition(xApp->getBackPosition());
    addChild(_back, 2);
    
    /*listView*/
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _listView = cocos2d::ui::ListView::create();
    _listView->setContentSize(Size(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height/2 + 120));
//    _listView->setScrollBarEnabled(false);

    _listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    _listView->setItemsMargin(150);
    _listView->setClippingEnabled(true);
    _listView->setBounceEnabled(false);
    _listView->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    _listView->setAnchorPoint(Point::ANCHOR_MIDDLE);
    addChild(_listView);
    
    /*no use layout*/
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(1,385));
    _listView->pushBackCustomItem(layout);
    
    
    return true;
}

void  PictureScene::reload(const int picturePageIndex)
{
    PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex);
    if (!pack)
        return;
    
    _picturePageIndex = picturePageIndex;
    
    float intervalX = getContentSize().width / 3;
    float intervalY = getContentSize().height / 2;
    
    float offsetX = 0;
 
    float offsetY = 0;
    
    //int index = 1 + (picturePageIndex * 6);
    int index = 1;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex);
            std::string pictureName;
            std::string historyLogo;
            
            if (pack->isServerPack)
            {
                pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex+1, index, true);
            }
            else
            {
                pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex+1, index, false);
            }
            
            Sprite *logo= nullptr;
            
            /* --------IMPORTANT-------
             history logo stored in device writeable path, so local pack same as server pack
             */
            historyLogo = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex+1, index, true);
            
            bool isExsit = FileUtils::getInstance()->isFileExist(historyLogo);
            if (isExsit)
            {
                
                /* --------IMPORTANT-------
                 remove the last history logo in texturecache
                 */
                Director::getInstance()->getTextureCache()->removeTextureForKey(historyLogo);
                logo = Sprite::create(historyLogo);
            }
            else
            {
                logo = Sprite::create(pictureName, Rect(0, 0, 1536, 1775));
                /* --------IMPORTANT-------
                 original picture is 1136 * 768, so we must set the texture rect to adapt
                 device screen
                 */
                //                logo->setTextureRect(Rect(namespaceST::STVisibleRect::getDesignOffset().x, namespaceST::STVisibleRect::getDesignOffset().y, Utils::SCREEN_WIDTH - (310 - (1136 - Utils::SCREEN_WIDTH)/2.0), Utils::SCREEN_HEIGHT));
            }
            
            if (!logo) {
                log("WARING: create pack picture logo sprite failed");
                continue;
            }
            
            offsetX = ((intervalX - logo->getContentSize().width)/2) * (2 * j + 1) + logo->getContentSize().width * j;
            offsetY = ((intervalY - logo->getContentSize().height)/2) * (2 * i + 1) + logo->getContentSize().height * i;
            
            //white background
            auto bg = ui::Button::create("choose/Pack1_1.png", "choose/Pack1_1.png");
            
            float scaleX,scaleY;
            if(isExsit)
            {
                scaleX = PACK_BG_WIDTH / logo->getContentSize().width;
                scaleY = PACK_BG_HEIGHT / logo->getContentSize().height;
            }
            else
            {
                scaleX = PACK_BG_WIDTH / (logo->getContentSize().width);
                scaleY = PACK_BG_HEIGHT / logo->getContentSize().height;
            }
            float scale = scaleX > scaleY ? scaleY : scaleX;
            
            auto layout = ui::Layout::create();
            layout->setContentSize(Size(PACK_BG_WIDTH,PACK_BG_HEIGHT));
            bg->setTag(index);
            
            if(isExsit)
            {
                logo->setScale(scale * 0.97);
                logo->setPosition(Vec2(layout->getContentSize()/2) + Vec2(0, 5));
            }
            else
            {
                logo->setScale(scale * 0.97);
                logo->setPosition(Vec2(layout->getContentSize()/2) + Vec2(0, 5));
            }
            bg->setPosition(layout->getContentSize()/2);
            layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
            layout->addChild(logo,2);
            layout->addChild(bg,1);
            bg->addTouchEventListener(CC_CALLBACK_2(PictureScene::touchDown, this));
            _listView->pushBackCustomItem(layout);
            
            index++;
        }
    }
    
    /*no use layout*/ //stupid
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(30,385));
    _listView->pushBackCustomItem(layout);
}


void PictureScene::touchDown(cocos2d::Ref *pSender, ui::Widget::TouchEventType type){
    if (type != ui::Widget::TouchEventType::ENDED || _block) return;
    auto layout = (ui::Button*)pSender;
    _pictureIndex = layout->getTag();
    
    if (999 == _pictureIndex) {
        _block = true;
        _back->setTouchEnabled(false);
        scheduleOnce(schedule_selector(PictureScene::backAction), 0);
        return;
    }
    
    std::string historyPath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, _pictureIndex, true);
    bool isExsit = FileUtils::getInstance()->isFileExist(historyPath);
    
    if (isExsit)
    {
        xDialog->show("text", "restart", "continue", CC_CALLBACK_1(PictureScene::onDialog, this));
    }
    else
    {
        if (!_hasChoosed)
        {
            auto scene = GameScene::create(_packIndex + 1, _pictureIndex);
            Director::getInstance()->replaceScene(scene);
            _hasChoosed = true;
        }
    }
    
}

void PictureScene::setPackIndex(const int index){
    _packIndex = index;
    reload(_packIndex);
}

void PictureScene::backAction(float dt){
    auto scene = PackScene::create();
    scene->beBack();
    Director::getInstance()->replaceScene(scene);
}

void PictureScene::onDialog(const string& name)
{
    if (name == "restart") {
        std::string filePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex + 1, _pictureIndex, true);
        bool isExit = FileUtils::getInstance()->isFileExist(filePath);
        if (isExit) {
            std::remove(filePath.c_str());
        }
        
        std::string logoPath = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex + 1, _pictureIndex, true);
        bool exist = FileUtils::getInstance()->isFileExist(logoPath);
        if (exist) {
            std::remove(filePath.c_str());
        }
        
        auto scene = GameScene::create(_packIndex + 1, _pictureIndex);
        Director::getInstance()->replaceScene(scene);
    }
    else if (name == "continue")
    {
        auto scene = GameScene::create(_packIndex + 1, _pictureIndex);
        Director::getInstance()->replaceScene(scene);
    }
}













