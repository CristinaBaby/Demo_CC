//
//  ChooseShapeView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#include "ChooseShapeView.h"
#include "GameMaster.h"
#include "IAPManager.h"
//#include "ShopLayer.h"
#include "AudioController.h"
#include "MakeConfig.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "TypeSprite.h"
#define cellWidth 234
#define cellHeight 270
#define btnCountInCell 2

ChooseShapeView*  ChooseShapeView::create(int orTypeIdx)
{
    ChooseShapeView *pRet = new ChooseShapeView();
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

bool ChooseShapeView::init(int orTypeIdx)
{
    if (!Node::init())
    {
        return false;
    }
    
    
    chosen_ = Sprite::create();
    this -> addChild(chosen_);
    chosen_ -> setVisible(false);
    chosen_ -> retain();
    
    chosenP_ = ParticleSystemQuad::create("selectshape3.plist");
    this -> addChild(chosenP_,333);
    chosenP_ -> setVisible(false);
    chosenP_ -> retain();

    
    chosenIdx_ = 0;
    isClicked_ = true;
    curTypeSprite_ = nullptr;
    
    auto title = Sprite::create("start/chooseashape.png");
    title -> setPosition(320, 870+300);
    this -> addChild(title);
    
    
    this -> runAction(Sequence::create(
                                       TargetedAction::create(title, MoveBy::create(0.8, Vec2(0, -300))),
                                       CallFunc::create([&]
    {
        this -> initWithTableView(orTypeIdx);
    }),
                                       NULL));
    

    return true;
}

void ChooseShapeView::initWithTableView(int orTypeIdx)
{
    MakeConfig::getInstance() -> setTypeName(__String::create("shape"));
    MakeConfig::getInstance() -> loadUnitSelConfig("MAKEUnit.plist");
    unitSelTableView_ = RETableView::create(this, Size((cellWidth+10)*2, cellHeight*2.7));
    unitSelTableView_ -> setDelegate(this);
    unitSelTableView_ -> setDataSource(this);
    unitSelTableView_ -> setPosition(Vec2(640/2-(cellWidth+10),480-cellHeight*2.7/2-20));
    unitSelTableView_ -> setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    unitSelTableView_ -> setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    unitSelTableView_ -> setContentOffset(Vec2(0,600));
    addChild(unitSelTableView_,1);
    isClicked_ = false;

    this -> runAction(Sequence::create(
                                       DelayTime::create(0.1),
                                       CallFunc::create([=]
                                                        {
                                                            float y = unitSelTableView_ -> getContentSize().height - cellHeight*2.7;
                                                            unitSelTableView_ -> setContentOffsetInDuration(Vec2(0, -y), 2);
                                                        }),
                                       DelayTime::create(0.5),
                                       CallFunc::create([=]
                                                        {
                                                            isClicked_ = false;
                                                        }),
                                       
                                       NULL));
    

}


//datesource
Size ChooseShapeView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size((cellWidth)*btnCountInCell, cellHeight);
}

ssize_t ChooseShapeView::numberOfCellsInTableView(TableView *table)
{
    
    int a =MakeConfig::getInstance() -> getUnitCount();
    if (a%btnCountInCell == 0)
    {
        return a/btnCountInCell;
    }
    else
    {
        return a/btnCountInCell+1;
    }
    
    
}

TableViewCell* ChooseShapeView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    MakeConfig::getInstance() -> setTypeName(__String::create("shape"));

    TableViewCell *cell;
    
    cell = new TableViewCell();
    cell -> autorelease();
    
    for (int i=0; i<btnCountInCell; i++)
    {
        
        MakeConfig::getInstance() -> setUnit(btnCountInCell*idx+i);
        auto str = MakeConfig::getInstance() ->getUnitIconName() ;
        if (!str)
        {
            break;
        }
        
        auto unitPath = String::createWithFormat("decoration/icon_%s/%s",MakeConfig::getInstance() ->getTypeName() -> getCString() ,str -> getCString());
        
        auto btnBg = TypeSprite::create(unitPath -> getCString());
        btnBg -> setAnchorPoint(Vec2::ZERO);
        btnBg -> setPosition(Vec2((cellWidth+10)*i+50/2, 0));
        btnBg -> setScale(0.9);
        
        auto lock = Sprite::create("button/lock.png");
        lock -> setAnchorPoint(Vec2(0.5,0));
        lock -> setPosition(Vec2(80, 10));
        if(!IAPManager::getInstance()->isFree("shape",(int)idx*btnCountInCell+i))
        {
            btnBg -> addChild(lock,12,kBtnLock);
        }
        
        cell -> addChild(btnBg,10,kBtnTypeCell+(int)idx*btnCountInCell+i);
        if (chosenIdx_ == btnBg -> getTag())
        {
                        if (chosen_ -> getParent())
                        {
                            chosen_ -> removeFromParent();
                        }
                        chosen_ -> setPosition(140, 70);
                        btnBg -> addChild(chosen_,3);
            
                        if (chosenP_ -> getParent())
                        {
                            chosenP_ -> removeFromParent();
                        }
                        chosenP_ -> setPosition(cellWidth/2+10, cellHeight/2);
                        btnBg -> addChild(chosenP_,4);
            
            btnBg -> setHighlighted(true);
            curTypeSprite_ = btnBg;
        }

    }
    
    return cell;
    
}



//delegate
void ChooseShapeView::tableCellTouched(TableView* table, TableViewCell* cell)
{

    
}

void ChooseShapeView::tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)
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
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/particles_sparkle_select.mp3");
    
    auto point = cell -> convertToNodeSpace(pTouch ->getLocation());
    
    for (int i = 0; i<btnCountInCell; i++)
    {
        TypeSprite* btn = dynamic_cast<TypeSprite *>(cell -> getChildByTag(kBtnTypeCell+(int)cell -> getIdx()*btnCountInCell+i));
        
        log("btn -> pzFileName_%s",btn -> pzFileName_.c_str() );

        if (btn && btn -> getBoundingBox().containsPoint(point) )
        {
            if (btn -> getChildByTag(kBtnLock) )
            {
                delegate_ -> shopClicked();
                return;
            }
            else
            {
                if (chosen_ -> getParent())
                {
                    chosen_ -> removeFromParent();
                }
                chosen_ -> setVisible(true);
                chosen_ -> setPosition(140, 70);
                btn -> addChild(chosen_,3);
                
                if (chosenP_ -> getParent())
                {
                    chosenP_ -> removeFromParent();
                }
                chosenP_ -> setVisible(true);
                chosenP_ -> setPosition(cellWidth/2+10, cellHeight/2);
                btn -> addChild(chosenP_,4);
                if (curTypeSprite_)
                {
                    curTypeSprite_ -> setHighlighted(false);
                }
                
                btn -> setHighlighted(true);
                curTypeSprite_ = btn;
                
                chosenIdx_ = kBtnTypeCell+(int)cell -> getIdx()*btnCountInCell+i;
                auto dic = MakeConfig::getInstance() -> getUnitArrayAtIdx() -> getObjectAtIndex(cell -> getIdx()*btnCountInCell+i);
                auto unitName = dynamic_cast<__Dictionary *>(dic) -> objectForKey("ImageName");
                delegate_ -> unitSelected(dynamic_cast<__String*>(unitName) -> getCString() ,btn->getTag()- kBtnTypeCell);
                break;
            }
        }
        
    }
    
    
}
void ChooseShapeView::reloadData()
{
    if (unitSelTableView_)
    {
        unitSelTableView_ -> reloadData();
    }
    
    
}



void ChooseShapeView::hideUnitSel()
{
    this -> setVisible(false);
    unitSelTableView_ -> removeFromParent();
}

