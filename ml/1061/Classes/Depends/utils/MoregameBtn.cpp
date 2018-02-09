//
//  MoregameBtn.cpp
//  JapaneseFood
//
//  Created by youjinchao1 on 15-3-5.
//
//

#include "MoregameBtn.h"
#include "CFSystemFunction.h"
#include "IAPManager.h"
#include "AdapterScreen.h"

MoregameBtn::MoregameBtn(){
    _direction = HORIZONTAL;
    _viewModel = HomeView;
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MoregameBtn::onLoadmoregameIconSucceed), kMoreGameLoadSucceed, nullptr);
    if (getBannerHeight() == 0) {
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MoregameBtn::onBannerDidload), kDidLoadBanner, nullptr);
    }
}

MoregameBtn::~MoregameBtn(){
    NotificationCenter::getInstance()->removeObserver(this, kMoreGameLoadSucceed);
    NotificationCenter::getInstance()->removeObserver(this, kDidLoadBanner);
}

bool MoregameBtn::init(){
    if (Layer::init()) {
        Sprite* moreGameFrame = Sprite::create("images/home/btn_moregame.png");
        container = Node::create();
        container->setContentSize(moreGameFrame->getContentSize());
        this->setContentSize(moreGameFrame->getContentSize());
        
        
        container->addChild(moreGameFrame, 10);
        moreGameFrame->setPosition(Point(moreGameFrame->getContentSize().width/2.0, moreGameFrame->getContentSize().height/2.0));
        container->setPosition(Point(0,0));
        
        this->addChild(container);
        return true;
    }
    return false;
}

void MoregameBtn::addtoParentLayer(cocos2d::Layer *layer) {
    if (this->getParent()) {
        log("has been added to other layer!!!");
        return;
    }
    Vec2 oringnalPP = Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize();
    
    this->setAnchorPoint(Point(1, 0));
    //    this->setPosition(Vec2(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleOrigin().y) + Point(0, -20 + getBannerHeight()));
    kAdapterScreen->setExactPosition(this, 0 + this->getContentSize().width + 20,20 + getBannerHeight(),Vec2(1,0),kBorderRight,kBorderBottom);
    
    
    originalPoint = this->getPosition();
    layer->addChild(this, 100);
    //    this->setVisible(false);
}

void MoregameBtn::startLoading(){
    MoreGameLoader::getInstance()->loadIcon();
}

void MoregameBtn::moregameClicked(cocos2d::Ref *pObj) {
    CFSystemFunction st;
    st.showMoreGame();
}

void MoregameBtn::onBannerDidload(cocos2d::Ref *pRef) {
    NotificationCenter::getInstance()->removeObserver(this, kDidLoadBanner);
    if (_viewModel == GameView) {
        this->runAction(EaseSineInOut::create(MoveBy::create(0, Vec2(0, getBannerHeight()))));
    }
}

void MoregameBtn::onEnter(){
    
    Layer::onEnter();
}

void MoregameBtn::onExit(){
    Layer::onExit();
}

void MoregameBtn::onLoadmoregameIconSucceed(cocos2d::Ref *pRef) {
    Image* img = dynamic_cast<Image*>(pRef);
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(img);
    Sprite* currentSprite = Sprite::createWithTexture(texture);
    
    Sprite* spriteGray = Sprite::createWithTexture(texture);
    spriteGray->setColor(Color3B::GRAY);
    
    log("==============加载moregame图标＝＝＝＝＝＝＝＝＝＝＝＝");
    
    MenuItemSprite* moregameBtn = MenuItemSprite::create(currentSprite, spriteGray);
    moregameBtn->setCallback(CC_CALLBACK_1(MoregameBtn::moregameClicked, this));
    moregameBtn->setAnchorPoint(Point(1.0, 1.0));
    moregameBtn->setScale(0.9);
    moregameBtn->setPosition(Point(container->getContentSize().width - 10, container->getContentSize().height - 12));
    moregameBtn->setPosition(moregameBtn->getPosition() );
    
    Menu* menu = Menu::create(moregameBtn, NULL);
    menu->setAnchorPoint(Point(0, 0));
    menu->setPosition(Point(0, 0));
    container->addChild(menu, 5);
    
    //    img->release();
    texture->release();
    
    switch (_direction) {
        case HORIZONTAL:
        {
            container->runAction(Sequence::create(MoveBy::create(0, Vec2(-300, 0)),CallFunc::create([=]{
                this->setVisible(true);
            }), MoveBy::create(0.3f, Point(300, 0)),NULL));
        }
            break;
        case VERTICAL:
        {
            container->runAction(Sequence::create(MoveBy::create(0, Vec2(0, -300)),CallFunc::create([=]{
                this->setVisible(true);
            }), MoveBy::create(0.3f, Point(0, 300)),NULL));
        }
        default:
            break;
    }
}

float MoregameBtn::getBannerHeight(){
    if (_viewModel == HomeView) {
        return 0;
    }
    
    if (!kIAPManager->isShowAds()) {
        return 0;
    }
    float bannerSize = 0;
    CFSystemFunction st;
    bool istablet = st.isTablet();
    if (istablet == false) {
        bannerSize = 50;
    }else {
        bannerSize = 90;
        
    }
    return bannerSize;
}