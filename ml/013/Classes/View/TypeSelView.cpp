//
//  TypeSelView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-4.
//
//

#define cellWidth  144
#define cellHeight 144
#include "TypeSelView.h"
#include "ui/cocosGUI.h"
#include "MakeConfig.h"
#include "UnitSelView.h"
#include "KSVisibleRect.h"
#include "TypeSprite.h"
#include "Config.h"

Vector<TypeSprite* > vecTypeSprites_;
TypeSprite* chosenTypeSprite_;


TypeSelView*  TypeSelView::create(int orTypeIdx)
{
    TypeSelView *pRet = new TypeSelView();
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

bool TypeSelView::init(int orTypeIdx)
{
    if (!Node::init())
    {
        return false;
    }
    
    isClicked_ = false;
    
    //记录typesel从哪个数据开始读取
    orIdx_ = orTypeIdx;
    
    auto typeBg = Sprite::create("start/banner.png");
    typeBg -> setPosition(320, -186/2+10);
    this -> addChild(typeBg);
    
//    auto winSize = Director::getInstance()->getWinSize();
//    auto *layerColor = CCLayerColor::create();
//    layerColor -> setColor(Color3B(255, 255, 255));
//    layerColor -> setOpacity(255);
//    layerColor -> setContentSize(Size(9999,cellHeight+30));
//    layerColor -> setPosition(-320, -160);
//    addChild(layerColor,0);

    
    this -> initWithTableView();
    
    return true;
}

void TypeSelView::initWithTableView()
{
    chosenBtnTag_ = 0;
    chosenTypeSprite_ = nullptr;
    
    MakeConfig::getInstance() -> loadTypeSelConfig("MakeTypesConfig.plist");
    typeArray_ = MakeConfig::getInstance() -> getTypeArray();
    typeArray_ -> retain();
    
    if (orIdx_ == 0)
    {
        typeSelTableView_ = RETableView::create(this, Size((cellWidth+25)*3.7, cellHeight));
        typeSelTableView_ -> setPosition(Vec2(0, -cellHeight/2-80+5));
    }
    
    else
    {
        typeSelTableView_ = RETableView::create(this, Size((cellWidth+100)*4,cellHeight ));
    }
    typeSelTableView_ -> setDelegate(this);
    typeSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    typeSelTableView_ -> reloadData();
    typeSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    

    addChild(typeSelTableView_);
}

//datesource
Size TypeSelView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (orIdx_ == 0)
    {
        return Size(cellWidth+25, cellHeight);
    }
    else
    {
        return Size(cellWidth+100, cellHeight);
    }

}

ssize_t TypeSelView::numberOfCellsInTableView(TableView *table)
{
    if (orIdx_ == 0)
    {
        return 5;
    }
    else
    {
        return 4;
    }

}

TableViewCell* TypeSelView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    
    MakeConfig::getInstance() -> loadTypeAtIdx(idx+orIdx_);
    auto iconName =   MakeConfig::getInstance() -> getTypeIconName();
    

    TableViewCell *cell = NULL;
    
    log("chosenBtnTag_%d",chosenBtnTag_);
    {
        cell = new TableViewCell();
        cell -> autorelease();
        
        auto btn = TypeSprite::create(iconName->getCString());
        btn -> setAnchorPoint(Vec2::ZERO);
        btn -> setPosition(Vec2::ZERO);
        log("btn -> pzFileName_%s",btn -> pzFileName_.c_str() );

        cell -> addChild(btn,10,kBtnTypeCell+(int)idx);
        if (chosenBtnTag_ ==  btn -> getTag())
        {
            btn -> setHighlighted(true);
            chosenTypeSprite_ = btn;
        }
    }

    
    return cell;
}



//delegate

void TypeSelView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
{
    
    if (isClicked_ )
    {
        return;
    }
    isClicked_ = true;
    
    this -> runAction(Sequence::create(DelayTime::create(0.2),
                                       CallFunc::create([&]{isClicked_ = false;})
                                       , NULL));
    
    auto point = cell -> convertToNodeSpace(pTouch ->getLocation());
    auto btn = dynamic_cast<TypeSprite *>(cell -> getChildByTag(kBtnTypeCell+(int)cell -> getIdx()));
    log("btn -> pzFileName_ c%s",btn -> pzFileName_.c_str() );

    //先重置之前选中的按钮
    if (btn -> getBoundingBox().containsPoint(point) && chosenTypeSprite_ && chosenTypeSprite_ != btn)
    {
        chosenTypeSprite_ -> setHighlighted(false);
    }

    //记录选中的按钮
    if (btn -> getBoundingBox().containsPoint(point) )
    {
        btn -> setHighlighted(true);
        if (btn -> getSelected())
        {
            chosenBtnTag_ = btn -> getTag();
            chosenTypeSprite_ = btn;
            MakeConfig::getInstance() -> setTypeName(chosenBtnTag_-kBtnTypeCell+orIdx_);
            delegate_ -> showUnitView();
        }
        else
        {
            chosenBtnTag_ = 0;
            chosenTypeSprite_ = nullptr;
            delegate_ -> removeUnitView();
        }
    }
}

void TypeSelView::resetTypeSelView()
{
//    chosenBtnTag_ = 0;
//    typeSelTableView_ -> reloadData();
    
    isClicked_ = false;
    if (chosenTypeSprite_)
    {
        chosenTypeSprite_ -> setHighlighted(false);
        chosenBtnTag_ = 0;
    }
    
}

void TypeSelView::onExit()
{
    Node::onExit();
    typeArray_ -> retain();
}



void TypeSelView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}




