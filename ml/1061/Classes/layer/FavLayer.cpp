//
//  FavLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "FavLayer.h"
#include "SceneManager.h"
#include "FavoriteManager.h"
#include "AudioHelp.h"
#include "Global.h"
FavLayer::FavLayer()
{
    
}
FavLayer::~FavLayer()
{
    
}

void FavLayer::onEnter()
{
    BaseLayer::onEnter();
    //    kPlayBackground("Sounds/bg.mp3");
}

void FavLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void FavLayer::onExit()
{
    BaseLayer::onExit();
}


bool FavLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        isFirstIn=true;
        initData();
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}
void FavLayer::initData()
{
    m_vFavorites.clear();
    m_vFavoriteScales.clear();
    m_vTypes.clear();
    FavoriteManager::getInstance()->obtainFavoritePaths(m_vFavorites, m_vFavoriteScales,FAVORITE_DIR);
    for(auto i = m_vFavoriteScales.begin();i<m_vFavoriteScales.end();i++)
    {
        string favoritepath = (*i).c_str();
        Sprite* data = Sprite::create(favoritepath);
        m_vTypes.pushBack(data);
    }
}


void FavLayer::initUI()
{
    Sprite* m_pBgSprite=Sprite::create("images/shop_fav/fav.jpg");
    kAdapterScreen->setExactPosition(m_pBgSprite, 320, 480);
    this->addToBackGroundLayer(m_pBgSprite, 1);

    auto backBtn = ui::Button::create("images/button/btn_back@2x.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(FavLayer::touchEvent, this));
    backBtn->setTag(30);
    kAdapterScreen->setExactPosition(backBtn, 10, 10,Vec2(0, 1),kBorderLeft,kBorderTop);
    this->addToUILayer(backBtn, 20);
    
    auto title = Sprite::create("images/shop_fav/mf.png");
    kAdapterScreen->setExactPosition(title, 320, 20,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToUILayer(title, 10);
    
    m_pAdapter = KFavoriteAdapter::create(m_vTypes);
    log("--->%d",m_vTypes.size());
    m_pAdapter->setDeleteCallback(CC_CALLBACK_2(FavLayer::onDeleteCallback,this));
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getWinSize().width, 720));
    m_pGirdView->m_fHSpace = 50;
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fMarginTop = 30;
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    kAdapterScreen->setExactPosition(m_pGirdView, 320, 480);
    m_pGirdView->m_fMarginLeft =  50;
//    m_pGirdView->m_fMarginRight = Director::getInstance()->getVisibleSize().width/2 + m_pAdapter->getItemWidth()/2;
    m_pGirdView->setScrollCallback(this);

    m_pGirdView->setAdapter(m_pAdapter);
    this->addToContentLayer(m_pGirdView, 1);
    
//    if(m_pGirdView->m_pMenuItemsVector.size()>0){
//        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
//            Point pos = m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->convertToWorldSpace(m_pGirdView->m_pMenuItemsVector.at(i)->getPosition());
//            float dis = pos.distance(m_pGirdView->getPosition());
//            float scale = 1 - dis / (m_pGirdView->getContentSize().width/2);
//            if(scale < 0.5)
//                scale = 0.5;
//            (m_pGirdView->m_pMenuItemsVector).at(i)->setScale(scale);
//        }
//        
//        m_pGirdView->m_pMenuItemsVector.at(0)->getParent()->reorderChild(m_pGirdView->m_pMenuItemsVector.at(0), 1);
//        m_pGirdView->m_pMenuItemsVector.at(0)->getChildByTag(101)->setVisible(true);
//    }
//    m_pGirdView->m_pScrollView->setBounceable(false);
}

void FavLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    if(type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 30)
    {
        SceneManager::getInstance()->popScene();
        AudioHelp::getInstance()->playEffect("previous page.mp3");
    }
}

void FavLayer::onScrolling(float offsetX,float offsetY)
{
//    if(m_pGirdView->m_pMenuItemsVector.size() > 1){
//        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
//            m_pGirdView->m_pMenuItemsVector.at(i)->getChildByTag(101)->setVisible(false);
//            Point pos = m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->convertToWorldSpace(m_pGirdView->m_pMenuItemsVector.at(i)->getPosition());
//            float dis = pos.distance(m_pGirdView->getPosition());
//            float scale = 1 - dis / (m_pGirdView->getContentSize().width/2);
//            if(scale < 0.5)
//                scale = 0.5;
//            (m_pGirdView->m_pMenuItemsVector).at(i)->setScale(scale);
//            if(dis <= 70){
//                m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->reorderChild(m_pGirdView->m_pMenuItemsVector.at(i), 1);
//            }
//        }
//    }

}
void FavLayer::onScrollEnd(float offsetX,float offsetY)
{
    if(isFirstIn){
        isFirstIn = false;
        return;
    }
    else{
        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
            log("scale----->%f",(m_pGirdView->m_pMenuItemsVector.at(i)->getScale()));
            if(m_pGirdView->m_pMenuItemsVector.at(i)->getScale() >= 0.80f)
            {
                m_pGirdView->m_pMenuItemsVector.at(i)->getChildByTag(101)->setVisible(true);
            }
        }
    }

}

void FavLayer::onDeleteCallback(int tag,cocos2d::ui::Button* button)
{
    
    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setContentText("Are you sure you want to delete your food?");
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
    m_pDeleteIndex = tag;

}

void FavLayer::onNegativeClick(void* type)
{
    
}

void FavLayer::onPositiveClick(void* type)
{
    AudioHelp::getInstance()->playDeleteEffect();
    FavoriteManager::getInstance()->deleteFile(m_vFavoriteScales.at(m_pDeleteIndex));
    FavoriteManager::getInstance()->deleteFile(m_vFavorites.at(m_pDeleteIndex));
    initData();
    m_pAdapter->setData(m_vTypes);
    m_pGirdView->refresh();
//    
//    if(m_pGirdView->m_pMenuItemsVector.size()>0){
//        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
//            m_pGirdView->m_pMenuItemsVector.at(i)->getChildByTag(101)->setVisible(false);
//            Point pos = m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->convertToWorldSpace(m_pGirdView->m_pMenuItemsVector.at(i)->getPosition());
//            float dis = pos.distance(m_pGirdView->getPosition());
//            float scale = 1 - dis / (m_pGirdView->getContentSize().width/2);
//            if(scale < 0.5)
//                scale = 0.5;
//            (m_pGirdView->m_pMenuItemsVector).at(i)->setScale(scale);
//        }
//        
//        m_pGirdView->m_pMenuItemsVector.at(0)->getParent()->reorderChild(m_pGirdView->m_pMenuItemsVector.at(0), 1);
//        m_pGirdView->m_pMenuItemsVector.at(0)->getChildByTag(101)->setVisible(true);
//    }
    m_pGirdView->m_pScrollView->setBounceable(false);

}
