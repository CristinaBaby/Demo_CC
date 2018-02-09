//
//  CupChooseView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#include "CupChooseView.h"
#include "GameMaster.h"
#include "IAPManager.h"
//#include "ShopLayer.h"
#include "AudioController.h"
#include "MakeConfig.h"
#include "KSVisibleRect.h"
#include "Config.h"
#define cellWidth 140
#define cellHeight 225
#define btnCountInCell 1

CupChooseView*  CupChooseView::create(int orTypeIdx)
{
    CupChooseView *pRet = new CupChooseView();
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

bool CupChooseView::init(int orTypeIdx)
{
    if (!Node::init())
    {
        return false;
    }
    
    isClicked_ = false;
    
    this -> initWithTableView(orTypeIdx);
    
    return true;
}

void CupChooseView::initWithTableView(int orTypeIdx)
{
    
    MakeConfig::getInstance() -> setTypeName(__String::create("cups"));
    MakeConfig::getInstance() -> loadUnitSelConfig("MAKEUnit.plist");
    unitSelTableView_ = RETableView::create(this, Size((cellWidth+40)*3.8, cellHeight+20));
    unitSelTableView_ -> setDelegate(this);
    unitSelTableView_ -> setDataSource(this);
    unitSelTableView_ -> setPosition(Vec2(640/2-(cellWidth+40)*3.8/2,-cellHeight-20));
    unitSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    unitSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    unitSelTableView_ -> setContentOffset(Vec2::ZERO);
    unitSelTableView_ -> reloadData();
    addChild(unitSelTableView_,1);
}


//datesource
Size CupChooseView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    
    return Size((cellWidth)*btnCountInCell+40, cellHeight+20);
}

ssize_t CupChooseView::numberOfCellsInTableView(TableView *table)
{
    return MakeConfig::getInstance() -> getUnitCount();
}

TableViewCell* CupChooseView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    
    MakeConfig::getInstance() -> setTypeName(__String::create("cups"));

    
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
    btnBg -> setPosition(Vec2(30, 10));

    auto lock = ui::ImageView::create("button/lock.png");
    lock -> setAnchorPoint(Vec2(0.5,0));
    lock -> setPosition(Vec2(30, 10));
    if (!MakeConfig::getInstance() -> getIsFree())
    {
        {
            btnBg -> addChild(lock,12,kBtnLock);
        }
    }
        
    cell -> addChild(btnBg,10,kBtnTypeCell+idx);
        
    return cell;
    
}



//delegate
void CupChooseView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

void CupChooseView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
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
                delegate_ -> unitSelected(dynamic_cast<__String*>(unitName) -> getCString());
            }
        }
    
}

void CupChooseView::reloadData()
{
    unitSelTableView_ -> reloadData();

}







