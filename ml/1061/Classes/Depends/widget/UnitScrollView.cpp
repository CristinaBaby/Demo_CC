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

UnitScrollView::UnitScrollView()
{

}

UnitScrollView::~UnitScrollView()
{

}

bool UnitScrollView::init(Size size)
{
    bool isInit=false;
    do{
        CC_BREAK_IF(!initUI());
        CC_BREAK_IF(!UnitScrollView::initWithSize(size));
        isInit=true;
    }while(0);
    return isInit;
}

UnitScrollView* UnitScrollView::create(Size size)
{
    UnitScrollView* unitScroll = new UnitScrollView();
    if(unitScroll&&unitScroll->init(size))
    {
        unitScroll->autorelease();
        return unitScroll;
    }
    CC_SAFE_DELETE(unitScroll);
    return nullptr;
}

bool UnitScrollView::initUI()
{
    this->setVisible(false);

    return true;

}
void UnitScrollView::loadingUnitScrollViewWithType(int index)
{
    
    m_pType=DEC_ITEM[index].c_str();
    m_vDecorates.clear();
    for (int i = 0; i<DEC_ITEM_Num[index]; i++) {
        
        __String* str = __String::createWithFormat("images/dec/icon_%s/%s%d.png",
                                                       m_pType.c_str(),m_pType.c_str(),i);
       
        Sprite* icon = Sprite::create(str->getCString());
        if(icon&&!kIAPManager->isFree(m_pType,i))
        {
            Sprite* lock = Sprite::create("images/make_mold/select/choosemold_lock.png");
            lock->setPosition(kAdapterScreen->getCenterFromNode(icon));
            icon->addChild(lock);
            lock->setScale(0.5);
//            lock->setPositionY(lock->getPositionY()-icon->getContentSize().height/2);
        }
        m_vDecorates.pushBack(icon);
    }
    //设置数据集
    m_pAdapter = KSDecorationAdapter::create(m_vDecorates);
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
std::string UnitScrollView::getDecType()
{
    return m_pType;
}
