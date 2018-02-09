//
//  ShopSelView.cpp
//  Make
//
//  Created by QAMAC01 on 15-4-13.
//
//

#include "ShopSelView.h"
#include "IAPManager.h"
//#include "ShopLayer.h"
#include "AudioController.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "GameMaster.h"
#include "AdsManager.h"
#define cellWidth 535
#define cellHeight 120
#define btnCountInCell 1

bool ShopSelView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    isClicked_ = false;
    
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 155), 9999, 9999);
    colorLayer -> setPosition(-500, -500);
    this -> addChild(colorLayer);
    
    auto bg = Sprite::create("shopsel/shop_bg.png");
    bg -> setPosition(Vec2(320, 480));
    this -> addChild(bg);
    
    
    ui::Button* restoreBtn  = ui::Button::create("shopsel/btn_restore.png");
    restoreBtn -> setPosition(Vec2(57, 58));
    restoreBtn -> addTouchEventListener(CC_CALLBACK_2(ShopSelView::touchEvent, this));
    bg -> addChild(restoreBtn,999,kBtnRestore);
    
    ui::Button* backBtn  = ui::Button::create("shopsel/close.png");
    backBtn -> setPosition(Vec2(588, 844));
    backBtn -> addTouchEventListener(CC_CALLBACK_2(ShopSelView::touchEvent, this));
    bg -> addChild(backBtn,999,kBtnBack);

    
    this -> initWithTableView();
    
    IAPManager::getInstance()->m_fnAfterPurchase = CC_CALLBACK_1(ShopSelView::purchaseCall, this);
    IAPManager::getInstance()->m_fnAfterRestore = CC_CALLBACK_2(ShopSelView::restoreCall, this);

    
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event){
            
            return true;
        };
        listener->onTouchMoved =[=](Touch* touch, Event* event){
            
            
        };
        listener->onTouchEnded =[=](Touch* touch, Event* event){
            
            
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    
    
    return true;
}


void ShopSelView::touchEvent(Ref *obj , ui::Widget::TouchEventType type)
{
    
    auto btn = dynamic_cast<ui::Button*>(obj);
    
    switch (type) {
        case ui::Widget::TouchEventType::ENDED :
        {
            switch (btn -> getTag())
            {
                case kBtnRestore:
                    IAPManager::getInstance() -> restore();
                    break;
                case kBtnBack:
                    if (callback)
                    {
                        this -> callback();
                    }
                    
                    this -> removeFromParent();
                    
                    break;
                default:
                    break;
            }
        
        }
            break;
            
        default:
            break;
    }
}


void ShopSelView::initWithTableView()
{
    unitSelTableView_ = RETableView::create(this, Size(cellWidth, cellHeight*6));
    unitSelTableView_ -> setDelegate(this);
    unitSelTableView_ -> setDataSource(this);
    unitSelTableView_ -> setPosition(Vec2(320-cellWidth/2,110));
    unitSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    unitSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    unitSelTableView_ -> reloadData();
    unitSelTableView_ -> setContentOffset(Vec2(0,500));
    unitSelTableView_ -> setBounceable(false);
    addChild(unitSelTableView_,1);
    
    this -> runAction(Sequence::create(
                                       CallFunc::create([=]
                                                        {
                                                            
                                                            float y = unitSelTableView_ -> getContentSize().height - cellHeight*6;
                                                            
                                                            unitSelTableView_ -> setContentOffsetInDuration(Vec2(0, -y), 0.7);
                                                        }),
                                       NULL));
}


//datesource
Size ShopSelView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size((cellWidth)*btnCountInCell, cellHeight);
}

ssize_t ShopSelView::numberOfCellsInTableView(TableView *table)
{
    return 6;
}

TableViewCell* ShopSelView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = NULL;
    
    {
        cell = new TableViewCell();
        cell -> autorelease();
        __String* str = __String::createWithFormat("shopsel/shop_%zd.png",idx);
        auto btn = Sprite::create(str -> getCString());
        btn -> setAnchorPoint(Vec2::ZERO);
        float deltaX = idx==0?0:(535-517)/2;
        
        btn -> setPosition(Vec2::ZERO + Vec2(deltaX, 0));
        
        cell -> addChild(btn,10,(int)idx);
    }
    
    return cell;
    
}


void ShopSelView::purchaseCall(bool flag)
{
    if(flag)
    {
        checkUnlock();
    }

}

void ShopSelView::restoreCall(bool isScucess, bool isHasData)
{
    if(isScucess)
    {
        if (isHasData) {
            checkUnlock();
        }
    }

}


void ShopSelView::checkUnlock()
{
    if (IAPManager::getInstance()->isPackagePurchased("com.blackbeltclown.snowcone.master"))
    {
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.christmaspack");
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.decoration");
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.flavors");
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.cups");
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.spoons");
    }
    if (IAPManager::getInstance()->isPackagePurchased("com.blackbeltclown.snowcone.decoration"))
    {
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.flavors");
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.spoons");
    }
    if (IAPManager::getInstance()->isPackagePurchased("com.blackbeltclown.snowcone.decoration") && IAPManager::getInstance()->isPackagePurchased("com.blackbeltclown.snowcone.cups"))
    {
        IAPManager::getInstance()->freePackge("com.blackbeltclown.snowcone.christmaspack");
    }


}


//delegate
void ShopSelView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ShopSelView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
{
    if (isClicked_)
    {
        return;
    }
    
    isClicked_ = true;
    this -> runAction(Sequence::create(DelayTime::create(0.1),
                                       CallFunc::create([&]
                                                        
                                                        {
                                                            isClicked_ = false;
                                                        })
                                       , NULL));
    
    
    auto point = cell -> convertToNodeSpace(pTouch ->getLocation());
    auto img = dynamic_cast<Sprite *>(cell -> getChildByTag((int)cell -> getIdx()));
    if (img && img -> getBoundingBox().containsPoint(point) )
    {
        
        if (IAPManager::getInstance() -> isPackagePurchased(cell -> getIdx()) )
        {
            MessageBox("You have already purchased this package!", "");
        }
        else
        {
            IAPManager::getInstance() -> purchase(cell -> getIdx());
        }
    }
}

void ShopSelView::onEnter()
{
    Node::onEnter();
    GameMaster::getInstance() -> setHidebanner(true);
    AdsManager::getInstance() -> removeAds(kTypeBannerAds);

}
void ShopSelView::onExit()
{
    Node::onExit();
    GameMaster::getInstance() -> setHidebanner(false);

}

