//
//  FavLayer.cpp
//  CSnowman
//
//  Created by QAMAC01 on 14-11-14.
//
//

#include "FavLayer.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "FavManager.h"
#include "GameMaster.h"
Scene* FavLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(FavLayer::create());
    return pScene;
}

bool FavLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto bg = Sprite::create("myfav/bg_fav.png");
    bg-> setPosition(KSVisibleRect::getPosition(KSVisibleRect::getDesignSize().width/2, KSVisibleRect::getDesignSize().height/2));
    addChild(bg);
    
    auto title = Sprite::create("myfav/logo_snowcone.png");
    title -> setAnchorPoint(Vec2(0.5, 1));
    title -> setPosition(KSVisibleRect::getPosition(320, 50+KSVisibleRect::getDesignOffset().y/2,kBorderNone,kBorderTop));
    addChild(title,2);
    
    auto backBtn = ui::Button::create("myfav/back.png");
    backBtn -> setAnchorPoint(Vec2(0, 1));
    backBtn -> setPosition(KSVisibleRect::getPosition(20, 50+KSVisibleRect::getDesignOffset().y/2,kBorderLeft,kBorderTop));
    addChild(backBtn,10,kBtnBack);
    backBtn -> addTouchEventListener(CC_CALLBACK_2(FavLayer::touchEvent, this));

    
    auto trashBtn = ui::Button::create("myfav/btn_delete.png");
    trashBtn -> setAnchorPoint(Vec2(1, 1));
    trashBtn -> setPosition(KSVisibleRect::getPosition(20, 50+KSVisibleRect::getDesignOffset().y/2,kBorderRight,kBorderTop));
    addChild(trashBtn,10,kBtnTrash);
    trashBtn -> addTouchEventListener(CC_CALLBACK_2(FavLayer::touchEvent, this));

//    scaleView_ = ScaleScrollView::create();
//    scaleView_ -> setPosition(KSVisibleRect::getPosition(0, 0));
//    addChild(scaleView_,99);
    
    this -> initScrollView();
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(FavLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif

    
    
    
    return true;
}

void FavLayer::initScrollView()
{
    if (_scrollView)
    {
        _scrollView -> removeFromParent();
        _scrollView = nullptr;
    }
    
    _scrollView = UIUnitScrollView::createWithNodeVector(initFavBtn(), 3);
    _scrollView -> setPosition(KSVisibleRect::getPosition(0, -100,kBorderNone,kBorderBottom));
    this -> addChild(_scrollView);

}



Vector<Node*> FavLayer::initFavBtn()
{
    FavManager::getInstance() -> getAllFavImage();
    auto vec_path = FavManager::getInstance()->vec_favStr;
    vec_btn.clear();
    
    for (int i = 0 ; i<vec_path.size() ; i++)
    {
        Sprite* image = Sprite::create(vec_path.at(i));
        image -> setScale(0.2);
        image -> setContentSize(Size(640,960)*0.2f);
        auto size =image -> getContentSize();
        auto whitelayer = LayerColor::create(Color4B::WHITE, size.width+10, size.height+10);
        whitelayer -> addChild(image);
        image -> setAnchorPoint(Vec2::ZERO);
        image -> setPosition(5,5);
        Vec2 offset = Director::getInstance()->getVisibleOrigin();
        Rect rect = Rect(offset.x, offset.y, 640, 960);
       
        image->setTextureRect(rect);

        auto deleteBtn = ui::Button::create("shopsel/close.png");
        deleteBtn -> loadTexturePressed("shopsel/close.png");
        deleteBtn -> addTouchEventListener(CC_CALLBACK_2(FavLayer::touchEvent, this));
        deleteBtn -> setAnchorPoint(Vec2(1, 1));
        deleteBtn -> setPosition(whitelayer->getContentSize());
        deleteBtn -> setVisible(false);
        deleteBtn -> setScale(0.6);
        whitelayer -> addChild(deleteBtn,90,i);
        
        vec_btn.pushBack(whitelayer);

    }
    
    return vec_btn;
}

void FavLayer::showDeleteBtn(bool show)
{
    for (Node* p : vec_btn)
    {
        if (p->getChildren().size()>0)
        {
            p->getChildren().at(1)->setVisible(show);
        }
    }
}



void FavLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn -> getTag())
            {
                case kBtnBack:
                {
                    Director::getInstance() -> popScene();
                    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/previouspage.mp3");
                }
                    break;
                case kBtnTrash:
                {
                    _showBtn = !_showBtn;
                    showDeleteBtn(_showBtn);
                }
                    break;
                default:
                {
                    FavManager::getInstance() -> deleteImageByIdx(btn->getTag());
                    this -> initScrollView();
                    _showBtn = false;
                }
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
}





#pragma mark dialog
void FavLayer::onNegativeClick(void*)
{
    
}

void FavLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        Director::getInstance() -> popScene();
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/previouspage.mp3");

        
    }
    if (lType == kDialogReset)
    {

    }
    
}

void FavLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    CCLOG("%d", keyCode);
    
    if (getChildByTag(129))
    {
        this -> removeChildByTag(129);

        return;
    }
    
//    if (scaleView_&& scaleView_ -> getChildByTag(129))
//    {
//        scaleView_ -> removeChildByTag(129);
//        return;
//    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        log("worked");
        
        Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to start over?");
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        dialog->setFuncType(kDialogReturnHome);
        dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
        addChild(dialog, 9999,129);
    }
    
}
void FavLayer::onEnter()
{
    Layer::onEnter();
    GameMaster::getInstance() -> setHidebanner(true);
}
void FavLayer::onExit()
{
    Layer::onExit();
    GameMaster::getInstance() -> setHidebanner(false);

}

