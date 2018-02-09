//
//  UnitScrollView.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-5.
//
//

#include "UnitScrollView.h"
#include "Global.h"
#include "IAPManager.h"
#include "AdapterScreen.h"


UnitScrollView::UnitScrollView():m_fOffsetX(0),m_fOffsetY(0),m_bIsClickEffect(false),m_fScale(1.0f)
{
    
}

UnitScrollView::UnitScrollView(const std::string& path, const std::string& lock, const std::string& unitBg):KDGridView()
{
    m_fOffsetX = 0;
    m_fOffsetY = 0;
    m_sDecorationImagesPath = path;
    m_sLockPath = lock;
    m_sUnitBgPath = unitBg;
    m_fScale = 1.0f;
}

UnitScrollView::~UnitScrollView()
{
    
}

bool UnitScrollView::init(Size size)
{
    bool isInit=false;
    
    do{
        CC_BREAK_IF(!UnitScrollView::initWithSize(size));
        
        CC_BREAK_IF(!initUI());
        
        isInit=true;
        
    }while(0);
    return isInit;
}

UnitScrollView* UnitScrollView::create(Size size, const std::string& path, const std::string& lock, const std::string& unitBg)
{
    UnitScrollView* unitScroll = new UnitScrollView(path, lock, unitBg);
    
    if(unitScroll && unitScroll->init(size))
    {
        unitScroll->autorelease();
        
        return unitScroll;
    }
    CC_SAFE_RELEASE_NULL(unitScroll);
    
    return nullptr;
}

bool UnitScrollView::initUI()
{
    this->setVisible(false);
    
    return true;
    
}

void UnitScrollView::loadingUnitScrollViewWithType(int index, bool isPng)
{
    m_pType = CATEGORY_NAME[index].c_str();
    
    m_vDecorates.clear();
    
    for (int i = 0; i < CATEGORY_NUMS[index]; i++) {
        
        Sprite* icon = nullptr;
        if (isPng) {
            icon = Sprite::create(__String::createWithFormat("icon_%s%d.png",m_pType.c_str(),i)->getCString());
        }else{
            icon = Sprite::create(__String::createWithFormat("icon_%s%d.jpg",m_pType.c_str(),i)->getCString());
        }
        
        Sprite* lock = nullptr;
        
        if(icon)
        {
            if (!kIAPManager->isPackagePurchased(0))
            {
                if (m_pType == "icecream") {
                    if (!kIAPManager->isPackagePurchased(1))
                    {
                        if (!kIAPManager->isFree(m_pType, i))
                        {
                            lock = Sprite::create(m_sLockPath);
                            
                            lock->setAnchorPoint(Vec2(0, 1));
                        }
                    }
                }else if(m_pType == "syrup" || m_pType == "sprinkle" || m_pType == "sticker" || m_pType == "candy" || m_pType == "fruit" || m_pType == "wafer"){
                    if (!kIAPManager->isPackagePurchased(2))
                    {
                        if (!kIAPManager->isFree(m_pType, i))
                        {
                            lock = Sprite::create(m_sLockPath);
                            
                            lock->setAnchorPoint(Vec2(0, 1));
                        }
                    }
                }else{
                    if (!kIAPManager->isFree(m_pType, i))
                    {
                        lock = Sprite::create(m_sLockPath);
                        
                        lock->setAnchorPoint(Vec2(0, 1));
                    }
                }
            }
        }
        else
        {
            lock = nullptr;
        }
        
        if ("" == m_sUnitBgPath)
        {
            if (lock)
            {
                lock->setPosition(icon->getContentSize() / 2 + Size(m_fOffsetX, m_fOffsetY));
                
                icon->addChild(lock);
            }
            
            m_vDecorates.pushBack(icon);
        }
        else
        {
            Sprite* bg = Sprite::create(m_sUnitBgPath);
            
            icon->setPosition(bg->getContentSize() / 2 + Size(0, 21));
            
            bg->addChild(icon);
            
            // add lock
            if (lock)
            {
                lock->setPosition(bg->getContentSize() / 2 + Size(m_fOffsetX, m_fOffsetY));
                
                bg->addChild(lock);
            }
            
            m_vDecorates.pushBack(bg);
        }
    }
    
    if (m_bIsClickEffect)
    {
        setClickEffect(index, isPng);
        m_pAdapter = KSSimpleAdapter::create(m_vDecorates, m_vDecorates_h);
        m_pAdapter->setScaleValue(m_fScale);
    }
    else
    {
        m_pAdapter = KSSimpleAdapter::create(m_vDecorates);
        m_pAdapter->setScaleValue(m_fScale);
    }
    
    this->setAdapter(m_pAdapter);
    
    this->setVisible(true);
}

void UnitScrollView::_onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    
    if (tag==0) {
        
        Vec2 point = m_pScrollView->getContentOffset();
        
        point.y = point.y+m_pScrollView->getViewSize().height;
        
        if (point.y<=0) {
            
            m_pScrollView->setContentOffset(point);
            
        }else{
            
            point.y = 0;
            
            m_pScrollView->setContentOffset(point);
        }
    }else{
        Vec2 point = m_pScrollView->getContentOffset();
        
        point.y = point.y-m_pScrollView->getViewSize().height;
        
        if (point.y>(m_pScrollView->getContentSize().height+m_pScrollView->getViewSize().height)) {
            
            m_pScrollView->setContentOffset(point);
            
        }else{
            
            point.y = 0;
            
            m_pScrollView->setContentOffset(point);
        }
    }
    
}

void UnitScrollView::setClickEffect(int index, bool isPng)
{
    auto pType = CATEGORY_NAME[index].c_str();
    
    m_vDecorates_h.clear();
    
    for (int i = 0; i < CATEGORY_NUMS[index]; i++)
    {
        Sprite* icon_h = nullptr;
        if (isPng) {
            icon_h = Sprite::create(__String::createWithFormat("icon_%s%d.png", pType, i)->getCString());
            icon_h->setColor(Color3B(icon_h->getColor().r / 2.0f, icon_h->getColor().g / 2.0f, icon_h->getColor().b / 2.0f));
        }else{
            icon_h = Sprite::create(__String::createWithFormat("icon_%s%d.jpg", pType, i)->getCString());
            icon_h->setColor(Color3B(icon_h->getColor().r / 2.0f, icon_h->getColor().g / 2.0f, icon_h->getColor().b / 2.0f));
        }
        
        Sprite* lock_h = nullptr;
        
        if(icon_h)
        {
            if (!kIAPManager->isPackagePurchased(0))
            {
                if (m_pType == "icecream") {
                    if (!kIAPManager->isPackagePurchased(1))
                    {
                        if (!kIAPManager->isFree(m_pType, i))
                        {
                            lock_h = Sprite::create(m_sLockPath);
                            
                            lock_h->setAnchorPoint(Vec2(0, 1));
                        }
                    }
                }else if(m_pType == "syrup" || m_pType == "sprinkle" || m_pType == "sticker" || m_pType == "candy" || m_pType == "fruit" || m_pType == "wafer"){
                    if (!kIAPManager->isPackagePurchased(2))
                    {
                        if (!kIAPManager->isFree(m_pType, i))
                        {
                            lock_h = Sprite::create(m_sLockPath);
                            
                            lock_h->setAnchorPoint(Vec2(0, 1));
                        }
                    }
                }else{
                    if (!kIAPManager->isFree(m_pType, i))
                    {
                        lock_h = Sprite::create(m_sLockPath);
                        
                        lock_h->setAnchorPoint(Vec2(0, 1));
                    }
                }
            }
        }
        else
        {
            lock_h = nullptr;
        }

        
        if ("" == m_sUnitBgPath)
        {
            if (lock_h)
            {
                lock_h->setPosition(icon_h->getContentSize() / 2 + Size(m_fOffsetX, m_fOffsetY));
                
                icon_h->addChild(lock_h);
                lock_h->setColor(Color3B(lock_h->getColor().r / 2.0f, lock_h->getColor().g / 2.0f, lock_h->getColor().b / 2.0f));
            }
            
            m_vDecorates_h.pushBack(icon_h);
        }
        else
        {
            Sprite* bg = Sprite::create(m_sUnitBgPath);
            
            icon_h->setPosition(bg->getContentSize() / 2 + Size(0, 21));
            
            bg->addChild(icon_h);
            
            // add lock
            if (lock_h)
            {
                lock_h->setPosition(bg->getContentSize() / 2 + Size(m_fOffsetX, m_fOffsetY));
                
                bg->addChild(lock_h);
            }
            
            bg->setColor(Color3B(bg->getColor().r / 2.0f, bg->getColor().g / 2.0f, bg->getColor().b / 2.0f));
            for (int i = 0; i < bg->getChildrenCount(); i++) {
                bg->getChildren().at(i)->setColor(Color3B(bg->getChildren().at(i)->getColor().r / 2.0f, bg->getChildren().at(i)->getColor().g / 2.0f, bg->getChildren().at(i)->getColor().b / 2.0f));
            }
            m_vDecorates_h.pushBack(bg);
        }
    }
}

void UnitScrollView::setIsNeedClickEffect(bool isNeed)
{
    m_bIsClickEffect = isNeed;
}

std::string UnitScrollView::getDecType()
{
    return m_pType;
}

void UnitScrollView::setOffsetX(float x)
{
    m_fOffsetX = x;
}

void UnitScrollView::setOffsetY(float y)
{
    m_fOffsetY = y;
}

void UnitScrollView::setScaleValue(float value)
{
    m_fScale = value;
}
