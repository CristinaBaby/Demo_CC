//
//  ScaleScrollView.cpp
//  CSnowman
//
//  Created by QAMAC01 on 14-11-14.
//
//

#include "ScaleScrollView.h"
#include "FavManager.h"
#include "Config.h"
#include "REScrollView.h"
#include "SimpleAudioEngine.h"
#include "KSVisibleRect.h"
#define maxScale 1

Scene* ScaleScrollView::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ScaleScrollView::create());
    return pScene;
}

bool ScaleScrollView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    FavManager::getInstance() -> getAllFavImage();
    
    favArray_ = FavManager::getInstance() -> getFavArray();
    favArray_ -> retain();


    deleteBtn_ = ui::Button::create("button/btn_delete.png");
    deleteBtn_ -> setPosition(Vec2(320+465/2-45, 480+594/2-45));
    addChild(deleteBtn_,99,kBtnDelete);
    deleteBtn_ -> addTouchEventListener(CC_CALLBACK_2(ScaleScrollView::touchEvent, this));

    
    
    this -> initScroll();

    return true;
}


void ScaleScrollView::initScroll()
{

    if (favArray_ -> count() <=0)
    {
        deleteBtn_ -> setVisible(false);
        return;
    }
    
    if(sp_Vec.size() >0)
    {
        sp_Vec.clear();
    }
    
    scroll_layer = Layer::create();//创建scrollView中的容器层
    scroll_layer->setPosition(Point::ZERO);
    scroll_layer->setAnchorPoint(Point::ZERO);
    scroll_layer->setContentSize(Size((200+300)*(favArray_ -> count())+400,700));
    scroll_layer -> setColor(Color3B(0, 0, 0));
    
    scroll_ = REScrollView::create(Size(Vec2(800, 700)));
    scroll_ -> setDelegate(this);
    scroll_ -> setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    scroll_ -> setContainer(scroll_layer);
    scroll_ -> setPosition(-40, 100);

    scroll_ -> setBounceable(false);
    addChild(scroll_);
    
    if (favArray_ -> count() > 0)
    {
        for (int i=0 ;i<favArray_ -> count();i++)
        {
            auto str = FavManager::getInstance() -> getFavArray() -> getObjectAtIndex(i);
            std::string fullpath = FileUtils::getInstance()->getWritablePath() + dynamic_cast<__String *>(str) -> getCString();
            if(FileUtils::getInstance() -> isFileExist(fullpath.c_str()))
            {
                auto btn = Sprite::create("shop/myfav_frame.png");
                btn -> setPosition(Vec2((200+50+250)*i+350,350));
                sp_Vec.pushBack(btn);
                scroll_layer -> addChild(btn,4,50+i);
                
                ClippingNode* node = ClippingNode::create();
                auto btnImage = Sprite::create(fullpath.c_str());
                node -> addChild(btnImage,4,50);
                btn -> addChild(node,4,50);
                btn -> setTexture("shop/myfav_frame.png");
                
                auto mask = Sprite::create("shop/frame.png");
                mask -> setPosition(btn -> getContentSize().width/2, btn -> getContentSize().height/2);
                node -> setStencil(mask);
                node -> setInverted(false);
                node -> setAlphaThreshold(0.1);
                mask -> setScale(0.98);
                btn -> setZOrder(10);
                btnImage -> setPosition(btn -> getContentSize().width/2, btn -> getContentSize().height/2);
                btnImage -> setScale(0.7);

                if (i != 0)
                {
                    float percent = checkOffset(btn);
                    btn -> setScale(maxScale-percent);
                }
                
                int a = btn -> getTag();
                int delta = abs(0 -a);
                btn -> setZOrder(10-delta);

            }
        }
    }

    auto p = scroll_layer-> convertToWorldSpaceAR(sp_Vec.at(0) -> getPosition());

}


void ScaleScrollView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    deleteBtn_ -> setVisible(false);

    for( auto e : sp_Vec )
    {
        float percent = checkOffset(e);
        e -> setScale(maxScale-percent);
        int a = e -> getTag();
        int delta = abs(0 -a);
        e -> setZOrder(10-delta);
    }
    

}

void ScaleScrollView::scrollViewDidZoom(REScrollView* view)
{


}

void ScaleScrollView::scrollViewTouchEnd(Vec2 worldPoint)
{
    
    deleteBtn_ -> setVisible(true);

    //停止scroll按照方向的滑动
    scroll_ -> unscheduleAllSelectors();

    log("offset %f",scroll_ -> getContentOffset().x);
    float delta = (scroll_ -> getContentOffset().x)/ -(250+250);
    
    log("delta%f",delta);

    curIdx_ = (int)delta;
    if(delta - (float)curIdx_ > 0.5)
        curIdx_++;
    if (curIdx_ >= sp_Vec.size())
    {
        curIdx_ = sp_Vec.size()-1;
    }
    log("curIdx_%d",curIdx_);

    if (curIdx_ < sp_Vec.size())
    {
        scroll_ -> setContentOffsetInDuration(Vec2(curIdx_*-(250+250), 0),0.2);
    }

    for( auto e : sp_Vec )
    {
        if (e-> getTag() == curIdx_+50)
        {
            
            e -> runAction( ScaleTo::create(0.2, 1));
            e -> setZOrder(10);
            
            ClippingNode* node = dynamic_cast<ClippingNode*>(e -> getChildByTag(50));
            Sprite* image = dynamic_cast<Sprite*>(node -> getChildByTag(50));
            Sprite* mask = dynamic_cast<Sprite*>(node ->  getStencil()) ;
            mask -> setTexture("shop/frame.png");
            mask -> setPosition(e -> getContentSize().width/2, e -> getContentSize().height/2);
            image -> setPosition(e -> getContentSize().width/2, e -> getContentSize().height/2);

            image -> setScale(0.7);

            runAction(Sequence::create(DelayTime::create(0.2),CallFunc::create(CC_CALLBACK_0(ScaleScrollView::showDeleteBtn, this)), NULL));
            
        }
        else
        {
            int a = e -> getTag();
            int delta = abs(curIdx_+50 -a);
            e -> setZOrder(10-delta);
            e -> setScale(0.8);
        }
    }

    
}

void ScaleScrollView::showDeleteBtn()
{
    deleteBtn_ -> setVisible(true);

}

void ScaleScrollView::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            
            
            switch (btn -> getTag())
            {
                case kBtnDelete:
                {
                    
                    if(favArray_ -> count() <= 0 )
                    {
                        return;
                    }

                    
                    Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogNotice, Dialog::DIALOG_TYPE_NEGATIVE);
//                    dialog -> setAnchorPoint(Vec2(0,0));
                    //                    STVisibleRect::setPosition(dialog, -size.width/2, -size.height/2);
                    dialog -> setPosition(Vec2(-KSVisibleRect::getDesignOffset().x, -KSVisibleRect::getDesignOffset().y));
                    dialog->setContentText("Do you want to delete your Snow Cone?");
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    addChild(dialog, 99999,129);


                    
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
void ScaleScrollView::onNegativeClick(void*)
{
    
}

void ScaleScrollView::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
    }
    if (lType == kDialogNotice)
    {
        if(favArray_ -> count() <=0 || curIdx_ >  favArray_ -> count())
        {
            return;
        }
        auto str = FavManager::getInstance() -> getFavArray() -> getObjectAtIndex(curIdx_);
        std::string fullpath = FileUtils::getInstance()->getWritablePath() + dynamic_cast<__String *>(str) -> getCString();
        if(FileUtils::getInstance() -> isFileExist(fullpath.c_str()))
        {
            remove(fullpath.c_str());
            
            scroll_ -> removeFromParent();
            FavManager::getInstance() -> getAllFavImage();
            curIdx_ = 0;
            initScroll();
        }
        CocosDenshion::SimpleAudioEngine::getInstance() ->  playEffect("sound/delete.mp3");

  
    }
    
}

void ScaleScrollView::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    CCLOG("%d", keyCode);
    
    if (getChildByTag(129))
    {
        return;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        log("worked");
        
        Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to start over?");
        dialog->setPositiveBtnText("YES");
        dialog->setNegativeBtnText("NO");
        dialog->setCallback(this);
        dialog->setFuncType(kDialogReturnHome);
        KSVisibleRect::setPosition(dialog, 0, 0);
        addChild(dialog, 9999,129);
    }
    
}

float ScaleScrollView::checkOffset(Sprite* sp)
{
    
    auto p = scroll_layer -> convertToWorldSpace(sp -> getPosition());

    float x = 420;
    float spX = p.x;
    float percent =abs(x - spX)/ (sp -> getChildByTag(50) -> getChildByTag(50) ->  getContentSize().width*0.35*10);
    log("percentrrr   %f",percent);

    return abs(percent);

}

