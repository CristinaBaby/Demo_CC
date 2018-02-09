//
//  ESScrollView.cpp
//  Make
//
//  Created by liji on 15/9/18.
//
//

#include "ESScrollView.h"


ESScrollView::ESScrollView():
_adapter(nullptr)
{
};

ESScrollView::~ESScrollView()
{
   
    CC_SAFE_RELEASE_NULL(_adapter);
};

void ESScrollView::initGrayLayer()
{
    auto gray = LayerColor::create(Color4B(255, 255, 255, 155));
    this->addChild(gray,-10);
    gray->setContentSize(Size(9999,9999));

}

void ESScrollView::setItemAdapter(ESUnitAdapter* base )
{
    CC_SAFE_RETAIN(base);
    CC_SAFE_RELEASE_NULL(_adapter);
    _adapter = base;
    this->setBounceEnabled(true);
    {
//        auto listener = EventListenerTouchOneByOne::create();
//        listener->setSwallowTouches(true);
//        listener->onTouchBegan = [=](Touch* touch, Event* event){
//            auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
//            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//            Size s = target->getContentSize();
//            Rect rect = Rect(0, 0, s.width, s.height);
//            
//            if (rect.containsPoint(locationInNode))
//            {
//                return true;
//            }
//            return false;
//
//        };
//        listener->onTouchMoved =[=](Touch* touch, Event* event){
//            ui::ScrollView::onTouchMoved(touch, event);
//            
//        };
//        listener->onTouchEnded =[=](Touch* touch, Event* event){
//            
//            ui::ScrollView::onTouchEnded(touch, event);
//
//        };
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
}
void ESScrollView::loadScrollView(int column)
{
    this->setScrollBarEnabled(false);
    
    if(_adapter->getUnitCount() == 0)
    {
        log("unit count error, please check");
    }
    
    this->getInnerContainer()->removeAllChildren();
    

    int rowNumber = getRowNumber(column);

    switch (this->getDirection())
    {
        case ui::ScrollView::Direction::VERTICAL:
        default:
        {
            if (column == 1)
            {
                float totalWidth = 0.f;
                for (int i = 0; i<column; i++)
                {
                    Size unitSize = _adapter->getItemSize(i);
                    totalWidth += unitSize.width+_marginX;
                }
                
                float totalHeight = 0.f;
                float displayHeight = 0.f;
                for (int i = 0; i<rowNumber; i++)
                {
                    Size unitSize = _adapter->getItemSize(i);
                    totalHeight += unitSize.height+_marginY;
                    if (_disPlayCount>=i+1)
                    {
                        displayHeight = totalHeight;
                        if (_disPlayCount != i+1 && _disPlayCount<i+2)
                        {
                            displayHeight += (i+2-_disPlayCount)*unitSize.height;
                        }
                    }
                }
                
                
                this->setContentSize(Size(totalWidth, displayHeight));
                log("ESScrollView %f,%f ",getContentSize().width,getContentSize().height);
                this->setInnerContainerSize(Size(totalWidth, totalHeight));
                log("getInnerContainerSize %f,%f ",getInnerContainerSize().width,getInnerContainerSize().height);
                
                float totalH = 0.f;
                
                for (int i = 0; i<rowNumber; i++)
                {
                    for (int j = 0; j<column; j++)
                    {
                        if (i*column+j >= _adapter->getUnitCount())
                        {
                            break;
                        }
                        ui::Widget* image = _adapter->getUnit(i*column+j);
                        Size unitSize = _adapter->getItemSize(i*column+j);
                        image->setPosition(Vec2((unitSize.width+_marginX)*j+unitSize.width/2, getInnerContainerSize().height-totalH-unitSize.height/2));
                        totalH += unitSize.height+_marginY;
                        log("%f,%f unit %f,test",image->getPositionX(),image->getPositionY(),unitSize.height/2 +(unitSize.height+_marginY)*(rowNumber - 1 - i ));
                        this->getInnerContainer()->addChild(image);
                        image->setTag(i*column+j);
                        vec_Item.pushBack(image);
                    }
                }
                
                this->setContentSize(Size(totalWidth, displayHeight));
                this->setInnerContainerSize(Size(totalWidth, totalHeight));
                log("ESScrollView %f,%f ",getContentSize().width,getContentSize().height);
                log("getInnerContainerSize %f,%f ",getInnerContainerSize().width,getInnerContainerSize().height);
            }
            else
            {
                
                Size unitSize = _adapter->getItemSize();

                this->setContentSize(Size((unitSize.width+_marginX)*column-_marginX, (unitSize.height+_marginY)*(_disPlayCount>rowNumber?rowNumber:_disPlayCount)));
                log("ESScrollView %f,%f ",getContentSize().width,getContentSize().height);
                this->setInnerContainerSize(Size((unitSize.width+_marginX)*column, (unitSize.height+_marginY)*rowNumber));
                
                for (int i = 0; i<rowNumber; i++)
                {
                    for (int j = 0; j<column; j++)
                    {
                        if (i*column+j >= _adapter->getUnitCount())
                        {
                            break;
                        }
                        ui::Widget* image = _adapter->getUnit(i*column+j);
                        image->setPosition(Vec2((unitSize.width+_marginX)*j+unitSize.width/2, unitSize.height/2 +(unitSize.height+_marginY)*(rowNumber - 1 - i )));
                        this->getInnerContainer()->addChild(image);
                        image->setTag(i*column+j);
                        vec_Item.pushBack(image);
                    }
                }
            }

        }
            break;
        case ui::ScrollView::Direction::HORIZONTAL:
        {
            this->setInnerContainerSize(Size(_marginX*rowNumber,_marginY));

            for (int i = 0; i<rowNumber; i++)
            {
                ui::Widget* image = _adapter->getUnit(_adapter->getStartIndex()+i);
                image->setPosition(Vec2(_marginX*i+_marginX/2, _marginY/2));
                this->getInnerContainer()->addChild(image);
                image->setTag(_adapter->getStartIndex()+i);
                vec_Item.pushBack(image);
            
                log("image->getPosition %f,%f ",image->getPosition().x,image->getPosition().y);
                log("image->getBoundingBox %f,%f ",image->getBoundingBox().origin.x,image->getBoundingBox().origin.y);
            }
        }
            break;
    }
    
}

int ESScrollView::getRowNumber(int column)
{
    int row;
    int unitCount = _adapter->getUnitCount();
    
    if (unitCount % column == 0)
    {
        row = unitCount/column;
    }
    else
    {
        row = unitCount/column +1 ;
    }
    return row;
}

Point ESScrollView::scrollToIdx(int idx, float duration)
{
    
//    Size unitSize = _adapter->getItemSize();
    
    switch (this->getDirection())
    {
        case ui::ScrollView::Direction::VERTICAL:
            break;
        case ui::ScrollView::Direction::HORIZONTAL:
            float deltaX = (_marginX)*(idx-_adapter->getStartIndex())-getContentSize().width/2+_marginX/2;
            log("deltaX %f" ,deltaX);
            
            this->getInnerContainer()->runAction(Sequence::create(
                                                                      CallFunc::create([=]
            {
                this->setEnabled(false);
            }),
                                                                      MoveTo::create(duration, Vec2(-(deltaX), 0)),
                                                                      CallFunc::create([=]
                                                                                       {
                                                                                           this->setEnabled(true);
                                                                                           log("after %f",getInnerContainer()->convertToWorldSpace(vec_Item.at(idx)->getPosition()).x);
                                                                                           
                                                                                       }),
                                                                       NULL));
            Point p = convertToWorldSpace(Vec2(vec_Item.at(idx)->getPosition().x - deltaX, vec_Item.at(idx)->getPosition().y));

            
            return (p);
            break;
    }
}

void ESScrollView::reload()
{
    this->getInnerContainer()->removeAllChildrenWithCleanup(true);
    this->loadScrollView();
}


//pageflow

void ESScrollView::setEnablePageflow()
{
    this->addEventListener(CC_CALLBACK_2(ESScrollView::scrolled, this));
    
}

void ESScrollView::scrolled(Ref* obj, EventType type)
{
    switch (type)
    {
        case EventType::CONTAINER_MOVED:
        {
            auto lenth = getInnerContainer()->getContentSize().width;
            auto posX = getInnerContainer()->getPositionX();
            auto curTag = int(-posX/(lenth-_marginLeft*2)*vec_Item.size()+0.5);
            log("lenth %f",lenth-_marginLeft-_marginRight-_adapter->getItemSize().width-_marginX*vec_Item.size());
            log("posX %f ", posX);
            log("curtag %f",posX/(lenth-_marginLeft*2)*vec_Item.size());
            

        
        }
            break;
        case EventType::SCROLLING:
        {
            
        }
            break;
//        case EventType::SCROLL_TO_LEFT:
//            log("SCROLL_TO_LEFT");
//            break;
//        case EventType::BOUNCE_LEFT:
//            log("BOUNCE_LEFT");
//            break;
//   
//        default:
//            break;
    }

}

void ESScrollView::removeItem(ui::Widget* image)
{
    int tag = image->getTag();
    auto unitSize = _adapter->getItemSize();
    for (auto temp : vec_Item)
    {
        log("%d", temp->getTag());
        log("%f" , -temp->getContentSize().width-_marginX);

        if (temp->getTag()>tag)
        {
            temp->runAction(MoveBy::create(0.2f, Vec2(-_marginX,0)));
        }
    }
    auto size = this->getInnerContainerSize();
    this->setInnerContainerSize(Size(size.width-_marginX,_marginY));
    vec_Item.eraseObject(image);
    image->removeFromParentAndCleanup(true);

}

void ESScrollView::show(std::function<void()> callback)
{
    this->scrollToRight(0.f, true);
//    this->setInnerContainerPosition(Vec2(-this->getInnerContainerSize().width/2,getInnerContainerPosition().y));
    this->setEnabled(false);
    this->runAction(Sequence::create(DelayTime::create(0.3f),
                                     CallFunc::create([=]
    {
        this->scrollToLeft(1.2f, true);
    }),
                                     DelayTime::create(0.8f),
                                     CallFunc::create([=]
    {
        this->setEnabled(true);
        if(callback)
            callback();
    }),
                                     NULL));

}


