//
//  UIUnitScrollView.cpp
//  Make
//
//  Created by liji on 15/8/6.
//
//

#include "UIUnitScrollView.h"
#include "MyMakeConfig.h"
#include "Config.h"
#include "IAPManager.h"
#include "MyMakeConfig.h"
#include "KSVisibleRect.h"

UIUnitScrollView* UIUnitScrollView::createWithTypeName(std::string pTypeName)
{
    UIUnitScrollView* pRet = new UIUnitScrollView();
    if (pRet && pRet -> initWithTypeName(pTypeName))
    {
        pRet -> autorelease();
        return pRet;
    }
        delete pRet;
        return nullptr;
}


bool UIUnitScrollView::initWithTypeName(std::string pTypeName)
{
    if (!Layer::init())
    {
        return false;
    }
    
    this -> loadUnitList(pTypeName);
    
    return true;
}

bool UIUnitScrollView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

UIUnitScrollView* UIUnitScrollView::createWithTypeNameAndColumn(std::string pTypeName,int Column)
{
    UIUnitScrollView* pRet = new UIUnitScrollView();
    if (pRet && pRet -> initWithTypeNameAndColumn(pTypeName,Column))
    {
        pRet -> autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool UIUnitScrollView::initWithTypeNameAndColumn(std::string pTypeName,int Column)
{

    if (!Layer::init())
    {
        return false;
    }
    _columnNumber = Column;
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 155), 9999, 9999);
    colorLayer -> setPosition(-500, -500);
    this -> addChild(colorLayer);
    
    auto bg = Sprite::create("button/decorate1_bg.png");
    bg -> setAnchorPoint(Vec2(0.5, 0));
    bg -> setPosition(Vec2(320, 100));
    this -> addChild(bg);
    
    MyMakeConfig::getInstance() -> setTypeName(pTypeName);
    int rowNumber = getRowNumber();
    
    Size unitIconSize = MyMakeConfig::getInstance()->getUnitIconSize();
    
    unitScrollView_ = ui::ScrollView::create();
    unitScrollView_ -> setDirection(ui::ScrollView::Direction::VERTICAL);
    unitScrollView_ -> setContentSize(Size((unitIconSize.width+_margin)*3+unitIconSize.width, (unitIconSize.height+20)*3.8));
    unitScrollView_ -> setInnerContainerSize(Size((unitIconSize.width+_margin)*3+unitIconSize.width , (unitIconSize.height+20)*rowNumber + 200 /*底部遮挡*/));
    unitScrollView_ -> setPosition(Vec2((640-(unitIconSize.width+_margin)*3-unitIconSize.width)/2, 120));
    this -> addChild(unitScrollView_);
    
    Vector<__String*> iconVec = MyMakeConfig::getInstance() -> vec_UnitIconName;
    vec_Button.clear();
    
    for (int i = 0; i<rowNumber; i++)
    {
        for (int j = 0; j<_columnNumber; j++)
        {
            if (i*_columnNumber+j >= iconVec.size())
            {
                break;
            }
            
            ui::Button* btn = ui::Button::create("button/unit_bg.png");
            btn -> loadTexturePressed("button/unit_bg.png");
            unitScrollView_ -> getInnerContainer() -> addChild(btn,10,i*_columnNumber+j);
            btn -> setPosition(Vec2((unitIconSize.width+_margin)*j+unitIconSize.width/2+5, unitIconSize.height/2 +(unitIconSize.height+20)*(rowNumber - 1 - i )+ 200 /*底部遮挡*/));
            btn -> addTouchEventListener(CC_CALLBACK_2(UIUnitScrollView::touchEvent, this));
            vec_Button.pushBack(btn);

            ui::ImageView* btnImage = ui::ImageView::create(iconVec.at(i*_columnNumber+j) -> getCString());
            btnImage -> setAnchorPoint(Vec2::ZERO);
            btn -> addChild(btnImage,1,100);

            auto lock = ui::ImageView::create("button/lock.png");
            lock -> setAnchorPoint(Vec2(0.5,0));
            lock -> setScale(0.7f);
            lock -> setPosition(Vec2(30, 10));
            if(!IAPManager::getInstance()->isFree(MyMakeConfig::getInstance() -> typeName_,i*_columnNumber+j))
            {
                btn -> addChild(lock,12,kBtnLock);
            }
        }
    }
    
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

int UIUnitScrollView::getRowNumber()
{
    int row;
    int unitCount = MyMakeConfig::getInstance()->getUnitCount();
    
    if (unitCount % _columnNumber == 0)
    {
        row = unitCount/_columnNumber;
    }
    else
    {
        row = unitCount/_columnNumber +1 ;
    }
    return row;
}

void UIUnitScrollView::loadUnitList(std::string pTypeName)
{

    if(pTypeName.empty())
    {
        return;
    }
    
    MyMakeConfig::getInstance() -> setTypeName(pTypeName);
    float deltaX = 0;
    float deltaY = 0;
    
    if (!strcmp(pTypeName.c_str(), "icecream"))
    {
        deltaX =10; deltaY = 20;
    }
    
    if (unitScrollView_)
    {
        if (unitScrollView_ -> getParent())
        {
            unitScrollView_ -> removeFromParent();
        }
        unitScrollView_ = nullptr;
    }
    
    int unitCount = MyMakeConfig::getInstance()->getUnitCount();
    Size unitIconSize = MyMakeConfig::getInstance()->getUnitIconSize();
    
    unitScrollView_ = ui::ScrollView::create();
    vec_Button.clear();

    unitScrollView_ -> setDirection(ui::ScrollView::Direction::HORIZONTAL);
    unitScrollView_ -> setContentSize(Size(600+KSVisibleRect::getDesignOffset().x, unitIconSize.height+60));
    unitScrollView_ -> setInnerContainerSize(Size((unitIconSize.width)*unitCount, unitIconSize.height+60));
    unitScrollView_ -> setPosition(Vec2((640-600-KSVisibleRect::getDesignOffset().x)/2, 0));
    this -> addChild(unitScrollView_);
        
    Vector<__String*> iconVec = MyMakeConfig::getInstance() -> vec_UnitIconName;
    
    for (int i = 0; i<unitCount; i++)
    {
        ui::Button* btn = ui::Button::create(iconVec.at(i) -> getCString());
        btn -> loadTexturePressed(iconVec.at(i) -> getCString());
        unitScrollView_ -> getInnerContainer() -> addChild(btn,10,i);
        btn -> setAnchorPoint(Vec2(0, 0.5));
        btn -> setPosition(Vec2((unitIconSize.width)*i+deltaX, unitIconSize.height/2));
        btn -> addTouchEventListener(CC_CALLBACK_2(UIUnitScrollView::touchEvent, this));
        vec_Button.pushBack(btn);
        
//        ui::ImageView* btnImage = ui::ImageView::create(iconVec.at(i) -> getCString());
//        btnImage -> setAnchorPoint(Vec2::ZERO);
//        btnImage -> setPosition(Vec2(0, deltaY));
//        btn -> addChild(btnImage,1,100);
        
        auto lock = ui::ImageView::create("button/lock.png");
        lock -> setAnchorPoint(Vec2(0.5,0));
        lock -> setPosition(Vec2(50, 10));
        if(!IAPManager::getInstance()->isFree(MyMakeConfig::getInstance() -> typeName_,i))
        {
            btn -> addChild(lock,12,kBtnLock);
        }


    }

    log("done?");

}


void UIUnitScrollView::loadUnitListFromTo(std::string pTypeName,int idxFrom,int idxTo )
{
    MyMakeConfig::getInstance() -> setTypeName(pTypeName);
    
    if (unitScrollView_)
    {
        if (unitScrollView_ -> getParent())
        {
            unitScrollView_ -> removeFromParent();
        }
        unitScrollView_ = nullptr;
    }
    
    log("crash?");
    
    int unitCount = idxTo - idxFrom +1;
    
    
    Size unitIconSize = MyMakeConfig::getInstance()->getUnitIconSize();
    
    vec_Button.clear();

    unitScrollView_ = ui::ScrollView::create();
    unitScrollView_ -> setDirection(ui::ScrollView::Direction::HORIZONTAL);
    unitScrollView_ -> setContentSize(Size(500, unitIconSize.height+10));
    unitScrollView_ -> setInnerContainerSize(Size((unitIconSize.width)*unitCount, unitIconSize.height));
    unitScrollView_ -> setPosition(Vec2(0, 0));
    this -> addChild(unitScrollView_);
    
    Vector<__String*> iconVec = MyMakeConfig::getInstance() -> vec_UnitIconName;
    
    
    for (int i = idxFrom; i<=idxTo; i++)
    {
        
        ui::Button* btn = ui::Button::create("button/shadow.png");
        btn -> loadTexturePressed("button/shadow.png");
        unitScrollView_ -> getInnerContainer() -> addChild(btn,10,i);
        btn -> setPosition(Vec2((unitIconSize.width)*(i-idxFrom)+unitIconSize.width/2-20/2, unitIconSize.height/2-10));
        btn -> addTouchEventListener(CC_CALLBACK_2(UIUnitScrollView::touchEvent, this));
        vec_Button.pushBack(btn);

        ui::ImageView* btnImage = ui::ImageView::create(iconVec.at(i) -> getCString());
        btnImage -> setAnchorPoint(Vec2::ZERO);
        btn -> addChild(btnImage);

        auto lock = ui::ImageView::create("button/lock.png");
        lock -> setAnchorPoint(Vec2(0.5,0));
        lock -> setPosition(Vec2(50, 10));
        if (i>=MyMakeConfig::getInstance()->iapLockedFrom())
        {
//            if(!IAPManager::getInstance()->shoulUnlockUnitInIdx(MyMakeConfig::getInstance() -> typeName_))
            if(!IAPManager::getInstance() -> isFree(MyMakeConfig::getInstance() -> typeName_));
            {
                btn -> addChild(lock,12,kBtnLock);
            }
        }
    }
    
    log("done?");
}


void UIUnitScrollView::touchEvent(Ref* obj, ui::Widget::TouchEventType type)
{
    ui::Button* btn = dynamic_cast<ui::Button*>(obj);
    
    Vector<__String*> iconVec = MyMakeConfig::getInstance() -> vec_UnitIconName;
    
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
        if (_isClicked )
        {
            return;
        }
        _isClicked = true;
        
        this -> runAction(Sequence::create(DelayTime::create(0.2),
                                           CallFunc::create([&]{_isClicked = false;})
                                           , NULL));
        log("tag %d", btn -> getTag());

        
            if (btn -> getChildByTag(kBtnLock))
            {
                if (lockedUnitClicked)
                {
                    this -> lockedUnitClicked();
                }
            }
            else
            {
                if(MyMakeConfig::getInstance() -> vec_UnitName.size()>0)
                {
                    
                    if(freeUnitClicked)
                    {
                        
                        if (btn -> getTag()<MyMakeConfig::getInstance() -> vec_UnitName.size())
                        {
                            this -> freeUnitClicked(MyMakeConfig::getInstance() -> vec_UnitName.at(btn -> getTag()) -> getCString(),btn -> getTag());
                        }
                        else
                        {
                            this -> freeUnitClicked("null",btn -> getTag());
                        }
                    }
                }
            }
    }
}

void UIUnitScrollView::enableTouch(bool enable)
{
    if(unitScrollView_)
    {
        unitScrollView_ -> setEnabled(enable);
    }
}

void UIUnitScrollView::checkUnlock()
{
//    if(IAPManager::getInstance()->shoulUnlockUnitInIdx(MyMakeConfig::getInstance() -> typeName_))
    {
        for (ui::Button* btn : vec_Button)
        {
            if (btn -> getChildByTag(kBtnLock))
            {
                btn -> getChildByTag(kBtnLock) -> removeFromParent();
            }
        }
    }
    
    if(IAPManager::getInstance() -> isShowAds())
    {
        
    }
}



UIUnitScrollView* UIUnitScrollView::createWithNodeVector(Vector<Node*> vec_node, int Column)
{
    UIUnitScrollView* pRet = new UIUnitScrollView();
    if (pRet && pRet -> initWithNodeVector(vec_node, Column))
    {
        pRet -> autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;


}

bool UIUnitScrollView::initWithNodeVector(Vector<Node*> vec_node,int column)
{
    
    if (!Layer::init())
    {
        return false;
    }

    
    if(vec_node.size()>0)
    {
        auto size = vec_node.at(0) ->getContentSize();
        
        int row = this -> getRowNumber((int)vec_node.size(), column);
        
        float marginX = (640-size.width*3)/4;
        _margin = marginX;
        
        float displayRows = row>3.4?3.4:row;
        
        unitScrollView_ = ui::ScrollView::create();
        unitScrollView_ -> setDirection(ui::ScrollView::Direction::VERTICAL);
        unitScrollView_ -> setContentSize(Size((size.width+_margin)*column, (size.height+20)*displayRows));
        unitScrollView_ -> setInnerContainerSize(Size((size.width+_margin)*column, (size.height+20)*row+20));
        
        log("%f height, ",unitScrollView_ ->getContentSize().height);
        unitScrollView_ -> setPosition(Vec2((640-(size.width+_margin)*column - _margin)/2, (820-(size.height+20)*displayRows)));
        this -> addChild(unitScrollView_);
        
        this -> loadScrollList(vec_node , column, row);
    }
    
    
    return true;
}

void UIUnitScrollView::loadScrollList(Vector<Node*> vec_node, int column, int row)
{
    
    auto size = vec_node.at(0) ->getContentSize();

    
    for (int i = row ; i>0; i--)
    {
        for (int j = 0; j<column; j++)
        {
            if ((row - i)*column+j >= vec_node.size())
            {
                break;
            }
            int idx = (row - i)*column+j;
            vec_node.at(idx) -> setPosition(Vec2((size.width+_margin)*j+_margin, 40+(size.height+20) *(i-1) ));
            unitScrollView_ -> getInnerContainer() -> addChild(vec_node.at(idx),10,idx);

        }
    }
    
//    for (int i = 0 ; i<row; i++)
//    {
//        for (int j = 0; j<column; j++)
//        {
//            if (i*column+j >= vec_node.size())
//            {
//                break;
//            }
//            int idx = i*column+j;
//            vec_node.at(idx) -> setPosition(Vec2((size.width+_margin)*j+_margin, 40+(size.height+20) *i ));
//            unitScrollView_ -> getInnerContainer() -> addChild(vec_node.at(idx),10,idx);
//            
//        }
//    }



}


int UIUnitScrollView::getRowNumber(int total,int column)
{
    
    if (total%column == 0)
    {
        return total/column;
    }
    else
    {
        return total/column+1;
    }

}


void UIUnitScrollView::loadUnitListColumn(std::string pTypeName,int column)
{
    if (!_frameBg.empty())
    {
        auto bg = Sprite::create("button/decorate1_bg.png");
        bg -> setAnchorPoint(Vec2(0.5, 0));
        bg -> setPosition(Vec2(320, 100));
        this -> addChild(bg);
    }
    _columnNumber = column;
    
    MyMakeConfig::getInstance() -> setTypeName(pTypeName);
    int rowNumber = getRowNumber();
    
    Size unitIconSize = MyMakeConfig::getInstance()->getUnitIconSize();
    
    unitScrollView_ = ui::ScrollView::create();
    unitScrollView_ -> setDirection(ui::ScrollView::Direction::VERTICAL);
    unitScrollView_ -> setContentSize(Size((unitIconSize.width+_margin)*3+unitIconSize.width, (unitIconSize.height+20)*3.8));
    unitScrollView_ -> setInnerContainerSize(Size((unitIconSize.width+_margin)*3+unitIconSize.width , (unitIconSize.height+20)*rowNumber + 200 /*底部遮挡*/));
    unitScrollView_ -> setPosition(Vec2((640-(unitIconSize.width+_margin)*3-unitIconSize.width)/2, 120));
    this -> addChild(unitScrollView_);
    
    Vector<__String*> iconVec = MyMakeConfig::getInstance() -> vec_UnitIconName;
    vec_Button.clear();
    
    for (int i = 0; i<rowNumber; i++)
    {
        for (int j = 0; j<_columnNumber; j++)
        {
            if (i*_columnNumber+j >= iconVec.size())
            {
                break;
            }
            ui::ImageView* bg;
            
            if (!_unitBg.empty())
            {
                bg = ui::ImageView::create(_unitBg);
            }
            else
                bg = ui::ImageView::create();
            
            ui::Button* btn = ui::Button::create(iconVec.at(i*_columnNumber+j) -> getCString());
            btn -> loadTexturePressed(iconVec.at(i*_columnNumber+j) -> getCString());
            unitScrollView_ -> getInnerContainer() -> addChild(btn,10,i*_columnNumber+j);
            btn -> setPosition(Vec2((unitIconSize.width+_margin)*j+unitIconSize.width/2+5, unitIconSize.height/2 +(unitIconSize.height+20)*(rowNumber - 1 - i )+ 200 /*底部遮挡*/));
            btn -> addTouchEventListener(CC_CALLBACK_2(UIUnitScrollView::touchEvent, this));
            vec_Button.pushBack(btn);
            
            bg -> addChild(btn,1,100);
            
            auto lock = ui::ImageView::create("button/lock.png");
            lock -> setAnchorPoint(Vec2(0.5,0));
            lock -> setPosition(Vec2(30, 10));
            if(!IAPManager::getInstance()->isFree(MyMakeConfig::getInstance() -> typeName_,i*_columnNumber+j))
            {
                bg -> addChild(lock,12,kBtnLock);
            }
        }
    }
    
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
    

}


