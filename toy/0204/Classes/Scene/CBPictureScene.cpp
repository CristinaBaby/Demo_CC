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
#include "../Model/CBPackManager.h"
#include "../Scene/CBPackScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBAppManager.h"
#include "../Scene/CBGameScene.h"
#include "../Layer/CBContinueLayer.h"

USING_CB_NAMESPACE;
USING_NS_ST;

#define TAG_PAGE_CONTROLLER  1234

#define RES_BG               "ui03_pack_bg.jpg"
#define RES_PEN_BG           "ui03_pen.png"

#define RES_CONTROL_NORMAL   "ui03_page_dot2.png"
#define RES_CONTROL_SELECT   "ui03_page_dot1.png"

#define RES_BACK_BUTTON      "ui07_public_back.png"

PictureScene::~PictureScene(){
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
    auto back = ui::Button::create(RES_BACK_BUTTON);
    back->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    back->setTag(999);
    back->addTouchEventListener(CC_CALLBACK_2(PictureScene::touchDown, this));
    back->setPosition(Vec2(back->getContentSize().width/4, Utils::SCREEN_HEIGHT - back->getContentSize().width/4));
    addChild(back);
    
    /*listView*/
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _listView = cocos2d::ui::ListView::create();
    _listView->setContentSize(Size(visibleSize.width,400));
    _listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    _listView->setItemsMargin(40);
    _listView->setClippingEnabled(true);
    _listView->setBounceEnabled(true);
    _listView->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2+20));
    _listView->setAnchorPoint(Point::ANCHOR_MIDDLE);
    addChild(_listView);
    
    /*no use layout*/
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(30,385));
    _listView->pushBackCustomItem(layout);
    AppManager::getInstance()->showBannerAd();
    
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
            if( i == 1 && j == 2) continue;
            
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
            
            ui::Button *logo= nullptr;
            
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
                logo = ui::Button::create(historyLogo, historyLogo);
            }
            else
            {
                logo = ui::Button::create(pictureName, pictureName);
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
            auto bg = Sprite::create("Pack1_1.png");
            
            float scaleX,scaleY;
            if(isExsit)
            {
                scaleX = 385 / logo->getContentSize().width;
                scaleY = 385 / logo->getContentSize().height;
            }
            else
            {
                scaleX = 385 / (logo->getContentSize().width-223);
                scaleY = 385 / logo->getContentSize().height;
            }
            float scale = scaleX > scaleY ? scaleY : scaleX;
            
            scaleX = 385 / bg->getContentSize().width;
            scaleY = 385 / bg->getContentSize().height;
            bg->setScale(scaleX + 0.04,scaleY + 0.04);
            
            auto layout = ui::Layout::create();
            layout->setContentSize(Size(385,385));
            logo->setTag(index);
            if(isExsit)
            {
                logo->setScale(scale-0.04);
                logo->setPosition(Vec2(layout->getContentSize().width/2,layout->getContentSize().height/2));
            }
            else
            {
                logo->setScale(scale);
                logo->setPosition(Vec2(layout->getContentSize().width/2+(123*scale),layout->getContentSize().height/2));
            }
            bg->setPosition(layout->getContentSize()/2);
            layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
            layout->addChild(logo,2);
            layout->addChild(bg,1);
            logo->addTouchEventListener(CC_CALLBACK_2(PictureScene::touchDown, this));
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
    if (type != ui::Widget::TouchEventType::ENDED) return;
    auto layout = (ui::Button*)pSender;
    int index = layout->getTag();
    
    if (999 == index) {
        backAction();
        return;
    }
    
    
    std::string historyPath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, index, true);
    bool isExsit = FileUtils::getInstance()->isFileExist(historyPath);
    
    if (isExsit)
    {
        ContinueLayer *layer = ContinueLayer::create(_packIndex, index);
        Director::getInstance()->getRunningScene()->addChild(layer);
    }
    else
    {
        if (!_hasChoosed)
        {
            auto scene = GameScene::createScene(_packIndex, index);
            Director::getInstance()->replaceScene(scene);
            _hasChoosed = true;
        }
    }
    
}

void PictureScene::setPackIndex(const int index){
    _packIndex = index;
    reload(_packIndex);
}

void PictureScene::backAction(){
    AppManager::getInstance()->playButtonEffect();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    auto scene = PackScene::createScene();
    Director::getInstance()->replaceScene(scene);
}













