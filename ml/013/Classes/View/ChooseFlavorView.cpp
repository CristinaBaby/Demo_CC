//
//  ChooseFlavorView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#include "ChooseFlavorView.h"
#include "GameMaster.h"
#include "IAPManager.h"
//#include "ShopLayer.h"
#include "AudioController.h"
#include "MakeConfig.h"
#include "KSVisibleRect.h"
#include "Config.h"
#define cellWidth 160
#define cellHeight 165
#define btnCountInCell 1

ChooseFlavorView*  ChooseFlavorView::create(int orTypeIdx)
{
    ChooseFlavorView *pRet = new ChooseFlavorView();
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

bool ChooseFlavorView::init(int orTypeIdx)
{
    if (!Node::init())
    {
        return false;
    }
    
//    auto typeBg = Sprite::create("start/banner.png");
//    typeBg -> setPosition(320, -186/2+20);
//    this -> addChild(typeBg);
    
    isClicked_ = false;
    
    this -> initWithTableView(orTypeIdx);
    
    
    {
        
        auto listener = EventListenerTouchOneByOne::create();
        listener -> setSwallowTouches(true);
        listener -> onTouchBegan = [&](Touch* touch, Event* event)
        {
            auto target = static_cast<RETableView*>(event->getCurrentTarget());//获取的当前触摸的目标
            
            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
            Size s = target->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            
            if (rect.containsPoint(locationInNode))
            {
                    return true;
            }
            return false;
        };
        listener->onTouchMoved =[=](Touch* touch, Event* event){
            
            
        };
        listener->onTouchEnded =[=](Touch* touch, Event* event){
            
            
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, unitSelTableView_);
    }
    
    
    return true;
}

void ChooseFlavorView::initWithTableView(int orTypeIdx)
{
    
    MakeConfig::getInstance() -> setTypeName(__String::create("syrup"));
    MakeConfig::getInstance() -> loadUnitSelConfig("MAKEUnit.plist");
    unitSelTableView_ = RETableView::create(this, Size((cellWidth+40)*3.2, cellHeight+20));
    unitSelTableView_ -> setDelegate(this);
    unitSelTableView_ -> setDataSource(this);
    unitSelTableView_ -> setPosition(Vec2(640/2-(cellWidth+40)*3.2/2,-cellHeight));
    unitSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    unitSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    unitSelTableView_ -> setContentOffset(Vec2::ZERO);
    unitSelTableView_ -> reloadData();
    addChild(unitSelTableView_,1);
}


//datesource
Size ChooseFlavorView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    
    return Size((cellWidth)*btnCountInCell+30, cellHeight+20);
}

ssize_t ChooseFlavorView::numberOfCellsInTableView(TableView *table)
{
    return MakeConfig::getInstance() -> getUnitCount();
}

TableViewCell* ChooseFlavorView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    
    MakeConfig::getInstance() -> setTypeName(__String::create("syrup"));
    
    TableViewCell *cell;
    
    cell = new TableViewCell();
    cell -> autorelease();
    
    MakeConfig::getInstance() -> setUnit(idx);
    auto str = MakeConfig::getInstance() ->getUnitIconName() ;
    log("%s getTypeName()",MakeConfig::getInstance() ->getTypeName() -> getCString());
    log("%s  str",str -> getCString());
    
    auto unitPath = String::createWithFormat("decoration/icon_%s/%s",MakeConfig::getInstance() ->getTypeName() -> getCString() ,str -> getCString());
    
    
    auto btnBg = ui::ImageView::create(unitPath -> getCString());
    btnBg -> setAnchorPoint(Vec2::ZERO);
    btnBg -> setPosition(Vec2(30/2, 10));
    
    auto lock = ui::ImageView::create("button/lock.png");
    lock -> setAnchorPoint(Vec2(0.5,0));
    lock -> setPosition(Vec2(30, 10));
    if (!MakeConfig::getInstance() -> getIsFree())
    {
//        if(!IAPManager::getInstance()->shoulUnlockUnitInIdx(MakeConfig::getInstance() ->getTypeName() ->getCString()))
        {
            btnBg -> addChild(lock,12,kBtnLock);
        }
    }
    
    cell -> addChild(btnBg,10,kBtnTypeCell+idx);
    
    return cell;
    
}



//delegate
void ChooseFlavorView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

void ChooseFlavorView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
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
    auto img = dynamic_cast<ui::ImageView *>(cell -> getChildByTag(kBtnTypeCell+(int)cell -> getIdx()));
    if (img -> getBoundingBox().containsPoint(point) )
    {
        if (img -> getChildByTag(kBtnLock) )
        {
            delegate_ -> shopClicked();
            return;
        }
        else
        {
            auto dic = MakeConfig::getInstance() -> getUnitArrayAtIdx() -> getObjectAtIndex(cell -> getIdx());
            auto unitName = dynamic_cast<__Dictionary *>(dic) -> objectForKey("ImageName");
            MakeConfig::getInstance() -> setHue(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("H")) -> intValue());
            MakeConfig::getInstance() -> setSaturation(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("S")) -> intValue());
            MakeConfig::getInstance() -> setValue(dynamic_cast<__String *>(dynamic_cast<__Dictionary *>(dic) -> objectForKey("V")) -> intValue());
            MakeConfig::getInstance() -> setTypeName(__String::create("syrup"));
            delegate_ -> unitSelected(dynamic_cast<__String*>(unitName) -> getCString());
        }
    }
    
}

void ChooseFlavorView::reloadData()
{
    unitSelTableView_ -> reloadData();
    
}







