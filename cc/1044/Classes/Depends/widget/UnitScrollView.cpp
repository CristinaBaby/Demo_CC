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
    if(CHOOSE_TYPE == 1)
    {
        m_pType=G_IceCream_MenuItem[index].c_str();
        m_vDecorates.clear();
        for (int i = 0; i<G_IceCream_DecNum[index]; i++) {
            __String* str = __String::createWithFormat("images/dec/icon/icon_%s/icon_%s%d.png",
                                                       m_pType.c_str(),m_pType.c_str(),i);

//            log("%s",str->getCString());
//            Sprite* bg = Sprite::create("images/button/box@2x.png");
            Sprite* icon = Sprite::create(str->getCString());
            icon->setScale(0.5);
            if(icon&&!kIAPManager->isFree(m_pType,i))
            {

                Sprite* lock = Sprite::create("images/dec/lock.png");
                lock->setPosition(kAdapterScreen->getCenterFromNode(icon));
                icon->addChild(lock);
            }


            m_vDecorates.pushBack(icon);
        }
        m_pAdapter = KSSimpleAdapter::create(m_vDecorates);
        m_pAdapter->setType(1);
        this->setAdapter(m_pAdapter);
        this->setVisible(true);
        
    }else if (CHOOSE_TYPE == 2)
    {
        m_pType=G_IcePop_MenuItem[index].c_str();
        m_vDecorates.clear();
        for (int i = 0; i<G_IcePop_DecNum[index]; i++) {
            __String* str = nullptr;
            if(index == 0){
                //特殊装饰物
                if (G_IcePop_MouleNum == 1) {

                    str = __String::createWithFormat("images/dec/icon/icon_coating/icon_mould1/icon_coating_mould1_%d.png",i);
                    
                    
                }else if(G_IcePop_MouleNum == 2 || G_IcePop_MouleNum == 3 || G_IcePop_MouleNum ==4){
  
                    str = __String::createWithFormat("images/dec/icon/icon_coating/icon_mould2_3_4/icon_coating_mould2_3_4_%d.png",i);
                    
                
                }else if(G_IcePop_MouleNum == 5){

                    str = __String::createWithFormat("images/dec/icon/icon_coating/icon_mould5/icon_coating_mould5_%d.png",i);
                    
                }
            }else{
                str = __String::createWithFormat("images/dec/icon/icon_%s/icon_%s%d.png",
                                                 m_pType.c_str(),m_pType.c_str(),i);
                
            }
            
            Sprite* icon = Sprite::create(str->getCString());
            icon->setScale(0.5);
            if(icon&&!kIAPManager->isFree(m_pType,i))
            {
           
                Sprite* lock = Sprite::create("images/dec/lock.png");
                lock->setPosition(kAdapterScreen->getCenterFromNode(icon));
                icon->addChild(lock);
            }
           
            m_vDecorates.pushBack(icon);
        }
        m_pAdapter = KSSimpleAdapter::create(m_vDecorates);
         m_pAdapter->setType(1);
        this->setAdapter(m_pAdapter);
        this->setVisible(true);
        
    }else if (CHOOSE_TYPE == 0)
    {
        m_pType=G_Snow_MenuItem[index].c_str();
        m_vDecorates.clear();
        for (int i = 0; i<G_Snow_DecNum[index]; i++) {
            __String* str = __String::createWithFormat("images/dec/icon/icon_%s/icon_%s%d.png",
                                                       m_pType.c_str(),m_pType.c_str(),i);
            
            Sprite* icon = Sprite::create(str->getCString());
            icon->setScale(0.5);
            if(icon&&!kIAPManager->isFree(m_pType,i))
            {
           
                Sprite* lock = Sprite::create("images/dec/lock.png");
                lock->setPosition(kAdapterScreen->getCenterFromNode(icon));
                icon->addChild(lock);
            }
            
            m_vDecorates.pushBack(icon);
        }
        m_pAdapter = KSSimpleAdapter::create(m_vDecorates);
         m_pAdapter->setType(1);
        this->setAdapter(m_pAdapter);
        this->setVisible(true);
    }
 
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
