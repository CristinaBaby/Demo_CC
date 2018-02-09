//
//  UnitSelView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#include "UnitSelView.h"
#include "GameMaster.h"
#include "IAPManager.h"
//#include "ShopLayer.h"
#include "AudioController.h"
#include "MakeConfig.h"
#include "KSVisibleRect.h"
#include "Config.h"
#define cellWidth 150
#define cellHeight 154
#define btnCountInCell 1

UnitSelView*  UnitSelView::create(int orTypeIdx)
{
    UnitSelView *pRet = new UnitSelView();
    if (pRet && pRet->init(orTypeIdx))
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

bool UnitSelView::init(int orTypeIdx)
{
    if (!Node::init())
    {
        return false;
    }
    
    isClicked_ = false;
    
//    auto unitSelBg = Sprite::create("");
//    this -> addChild(unitSelBg);
    auto winSize = Director::getInstance()->getWinSize();
    auto *layerColor = CCLayerColor::create();
    layerColor -> setColor(Color3B(0, 0, 0));
    layerColor -> setOpacity(150);
    layerColor -> setContentSize(Size(9999,9999));
    layerColor -> setPosition(-320, -480);
    addChild(layerColor,0);
    
    this -> setTag(kTagReady);
    
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
    
    

    
    this -> initWithTableView(orTypeIdx);



    return true;
}

void UnitSelView::initWithTableView(int orTypeIdx)
{
    float delta = 100;
    
    MakeConfig::getInstance() -> loadUnitSelConfig("MAKEUnit.plist");
    unitSelTableView_ = RETableView::create(this, Size((cellWidth+(640-cellWidth*4)/5)*4, cellHeight*4.3+delta));
    unitSelTableView_ -> setDelegate(this);
    unitSelTableView_ -> setDataSource(this);
    unitSelTableView_ -> setPosition(Vec2(0,-delta));
    unitSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    unitSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    unitSelTableView_ -> setContentOffset(Vec2::ZERO);
    unitSelTableView_ -> reloadData();
    addChild(unitSelTableView_,1);
}


//datesource
Size UnitSelView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size((cellWidth)*btnCountInCell, cellHeight+20);
}

ssize_t UnitSelView::numberOfCellsInTableView(TableView *table)
{
    log("%s",MakeConfig::getInstance() -> getTypeName()-> getCString());
    log("%zd",MakeConfig::getInstance() -> getUnitCount());
    
    int a =MakeConfig::getInstance() -> getUnitCount();
    if (a%4 == 0)
    {
        log("%d row", a/4);
        return a/4;
    }
    else
    {
        log("%d row", a/4+1);
        return a/4+1;
    }
    
    
}

TableViewCell* UnitSelView::tableCellAtIndex(TableView *table, ssize_t idx)
{

    TableViewCell *cell;
    
    cell = new TableViewCell();
    cell -> autorelease();
    
    for (int i=0; i<4; i++)
    {
        
        MakeConfig::getInstance() -> setUnit(4*idx+i);
        auto str = MakeConfig::getInstance() ->getUnitIconName() ;
        if (!str)
        {
            break;
        }
        log("%sMakeConfig::getInstance() ->getTypeName() -> getCString()",MakeConfig::getInstance() ->getTypeName() -> getCString());
        log("%str",str -> getCString());
        
        auto unitPath = String::createWithFormat("decoration/icon_%s/%s",MakeConfig::getInstance() ->getTypeName() -> getCString() ,str -> getCString());

        float delta =(640-cellWidth*4)/5;
        
        auto btnBg = ui::ImageView::create("button/unit_bg.png");
        btnBg -> setAnchorPoint(Vec2::ZERO);
        btnBg -> setPosition(Vec2((delta + cellWidth)*i+delta, 0));
        
        auto btn = ui::ImageView::create();
        btn -> ui::ImageView::loadTexture(unitPath -> getCString());
        if (!strcmp(MakeConfig::getInstance() -> getTypeName()->getCString() ,"extras"))
        {
            btn -> setScale(0.9);
        }
        btn -> setPosition(Vec2(cellWidth/2,cellHeight/2));
        btnBg ->  addChild(btn,10);
        
        auto lock = ui::ImageView::create("button/lock.png");
        lock -> setAnchorPoint(Vec2(0.5,0));
        lock -> setPosition(Vec2(50, 10));
        if (!MakeConfig::getInstance() -> getIsFree())
        {
//            if(!IAPManager::getInstance()->shoulUnlockUnitInIdx()
            {
                btnBg -> addChild(lock,12,kBtnLock);
            }
        }
        
        cell -> addChild(btnBg,10,kBtnTypeCell+(int)idx*4+i);

    }
    
    return cell;
    
}



//delegate
void UnitSelView::tableCellTouched(TableView* table, TableViewCell* cell)
{
////    
//    if (cell ->getChildByTag(kBtnLock))
//    {
//        __NotificationCenter::getInstance() -> postNotification("ShopTouched");
//        return;
//    }
//    else
//    {
////        AudioController::getInstance() -> playChosenEffect();
//    auto dic = MakeConfig::getInstance() -> getUnitArrayAtIdx() -> getObjectAtIndex(cell -> getIdx());
//        auto unitName = dynamic_cast<__Dictionary *>(dic) -> objectForKey("ImageName");
//        
//        __NotificationCenter::getInstance() -> postNotification("unitSelTouched",unitName);
//    }

}

void UnitSelView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
{
//    if (cell ->getChildByTag(kBtnLock))
//    {
//        delegate_ -> shopClicked();
//        return;
//    }
//        auto point = cell -> convertToNodeSpace(pTouch ->getLocation());
//        auto img = dynamic_cast<ui::ImageView *>(cell -> getChildByTag(kBtnTypeCell+(int)cell -> getIdx()));
//        if (img -> getBoundingBox().containsPoint(point) )
//        {
//            auto dic = MakeConfig::getInstance() -> getUnitArrayAtIdx() -> getObjectAtIndex(cell -> getIdx());
//            auto unitName = dynamic_cast<__Dictionary *>(dic) -> objectForKey("ImageName");
//            
//            delegate_ -> unitSelected(dynamic_cast<__String*>(unitName) -> getCString());
//        }
    
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
    for (int i = 0; i<4; i++)
    {
        auto img = dynamic_cast<ui::ImageView *>(cell -> getChildByTag(kBtnTypeCell+(int)cell -> getIdx()*4+i));
        if (img && img -> getBoundingBox().containsPoint(point) )
        {
            if (img -> getChildByTag(kBtnLock) )
            {
                    delegate_ -> shopClicked();
                    return;
            }
            else
            {
                auto dic = MakeConfig::getInstance() -> getUnitArrayAtIdx() -> getObjectAtIndex(cell -> getIdx()*4+i);
                auto unitName = dynamic_cast<__Dictionary *>(dic) -> objectForKey("ImageName");
                if(!strcmp(MakeConfig::getInstance() ->getTypeName() -> getCString(), "syrup"))
                {
                    MakeConfig::getInstance() -> setHue(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("H")) -> intValue());
                    MakeConfig::getInstance() -> setSaturation(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("S")) -> intValue());
                    MakeConfig::getInstance() -> setValue(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("V")) -> intValue());
                }

                
                delegate_ -> unitSelected(dynamic_cast<__String*>(unitName) -> getCString());
                break;
            }
        }

    }
    
    
}





