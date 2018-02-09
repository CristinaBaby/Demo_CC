//
//  BookView.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/18.
//
//

#include "BookView.h"

BookView::BookView():
_adapter(nullptr)
,_lis(nullptr)
,pageControl(nullptr)
,pageChanged(false)
,isMove(false)
,pageMove(nullptr){
    
}
BookView::~BookView(){
    clearPage();
}


BookView* BookView::create(const Size& _size){
    auto book = new BookView();
    if(book&&book->init()){
        book->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        book->setContentSize(_size);
        book->autorelease();
        return book;
    }
    CC_SAFE_RELEASE_NULL(book);
    return nullptr;
}


void BookView::setAdapter(ViewAdapter* viewAdapter){
    CC_SAFE_RETAIN(viewAdapter);
    CC_SAFE_RELEASE_NULL(_adapter);
    _adapter = viewAdapter;
    removeAllChildren();
    clearPage();
    auto _page = getPage(0);
    if(_page != nullptr){
        addChild(_page,1);
        pageControl = new BookPageStruct;
        pageControl->data = _page;
        if(_page->getLeftPage() != nullptr)
            _page->getLeftPage()->setVisible(false);
    }
    
}

BookPage*  BookView::getPage(size_t _index){
    BookPage* _page = nullptr;
    if(_adapter!=nullptr&& 2*_index<_adapter->getCount()){
        _page = BookPage::create(getContentSize());
        _page->setPosition(getContentSize()*.5);
        _page->setPageIndex(_index);
        _page->addRightPage(_adapter->getItem(2*_index));
        if((2*_index+1) < _adapter->getCount()){
            _page->addLeftPage(_adapter->getItem(2*_index+1));
        }
    }
    return _page;
}

bool BookView::caculateTouchPage(BookPageStruct *control, Vec2 wordpos){
    bool flag = false;;
    if(control != nullptr){
        control->data->CaculateCurrentPage(wordpos);
        auto operate = control->data->getCurrentPage();
        if(operate != nullptr){
            pageControl = control;
            flag = true;
        }
    }
    return flag;
}

bool BookView::onTouchBegan(Touch *pTouch, Event *pEvent) {
    if(pageControl == nullptr||isScheduled(schedule_selector(BookView::turnUpdate)))
        return false;
    else{
        isMove = false;
        bool flag = caculateTouchPage(pageControl, pTouch->getLocation());
        if(!flag)
            flag = caculateTouchPage(pageControl->nextPage, pTouch->getLocation());
        if(!flag)
            flag = caculateTouchPage(pageControl->prePage, pTouch->getLocation());
        return flag;
    }
}
void BookView::onTouchMoved(Touch *pTouch, Event *pEvent) {
    if(pageControl != nullptr){
        auto distance = pTouch->getLocation().distanceSquared(pTouch->getStartLocation());
        if(!isMove&&distance>36) {
            isMove = true;
            pageChanged = false;
            if(pageControl->data->getCurrentPage() == pageControl->data->getRightPage()){
                if(pageControl->nextPage == nullptr){
                    auto _page = getPage(pageControl->data->getPageIndex()+1);
                    if(_page != nullptr){
                        addChild(_page);
                        if(_page->getLeftPage() != nullptr)
                            _page->getLeftPage()->setVisible(false);
                        BookPageStruct * temp = new BookPageStruct;
                        temp->data = _page;
                        temp->prePage = pageControl;
                        pageControl->nextPage  = temp;
                    }
                }
                if(pageControl->nextPage != nullptr){
                    if(pageControl->nextPage->data->getRightPage())
                        pageControl->nextPage->data->getRightPage()->setVisible(true);
                    reorderChild(pageControl->nextPage->data, 0);
                }
            } else if(pageControl->data->getCurrentPage() == pageControl->data->getLeftPage()){
                if(pageControl->prePage == nullptr){
                    auto _page = getPage(pageControl->data->getPageIndex()-1);
                    if(_page != nullptr){
                        addChild(_page);
                        if(_page->getRightPage() != nullptr)
                            _page->getRightPage()->setVisible(false);
                        BookPageStruct * temp = new BookPageStruct;
                        temp->data = _page;
                        temp->nextPage = pageControl;
                        pageControl->prePage  = temp;
                    }
                }
                if(pageControl->prePage != nullptr){
                    if(pageControl->prePage->data->getLeftPage())
                        pageControl->prePage->data->getLeftPage()->setVisible(true);
                    reorderChild(pageControl->prePage->data, 0);
                }
                
            }
            reorderChild(pageControl->data,1);
            if(pageMove != nullptr)
                pageMove();
        } else if(isMove){
            pageControl->data->setScaleEnd(false);
            pageControl->data->scaleCurrentPage(pTouch->getDelta());
        }
    }
}

void BookView::onTouchEnded(Touch *pTouch, Event *pEvent) {
    if(pageControl != nullptr){
        if(isMove) {
            auto changeEnd =  [this](){
                pageControl->data->scaleEnd = nullptr;
                unschedule(schedule_selector(BookView::turnUpdate));
                auto currnet  = pageControl;
                if(pageControl->data->getCurrentPage() == pageControl->data->getLeftPage()){
                    
                    if(pageControl->prePage != nullptr && pageControl->prePage->data->getLeftPage() !=nullptr)
                        pageControl->prePage->data->getLeftPage()->setVisible(false);
                    if(pageControl->nextPage&&pageChanged)
                        pageControl = pageControl->nextPage;
                }else if(pageControl->data->getCurrentPage() == pageControl->data->getRightPage()) {
                    if(pageControl->nextPage != nullptr && pageControl->nextPage->data->getRightPage() != nullptr)
                        pageControl->nextPage->data->getRightPage()->setVisible(false);
                    if(pageControl->prePage&&pageChanged)
                        pageControl = pageControl->prePage;
                }
                currnet->data->clearCurrentPage();
            };
            
            if(!pageControl->data->IsScaleEnd()){
                if(abs(pTouch->getLocation().x-pTouch->getStartLocation().x)>40){
                    accel = pTouch->getDelta();
                    pageChanged = true;
                }
                else{
                    pageChanged = false;
                    accel =   -pTouch->getDelta();
                }
                float judge = 45;
                if(accel.x<-(judge+13))
                    accel.x = -(judge+13);
                else if(accel.x<0&&accel.x>-judge)
                    accel.x = -judge;
                else if(accel.x>=0&& accel.x<judge)
                    accel.x = judge;
                else if(accel.x>=judge&& accel.x>(judge+13))
                    accel.x = judge+13;
                pageControl->data->scaleEnd = changeEnd;
                if(!isScheduled(schedule_selector(BookView::turnUpdate)))
                    schedule(schedule_selector(BookView::turnUpdate));
            }else{
                pageChanged = true;
                changeEnd();
                pageControl->data->clearCurrentPage();
            }
        } else {
            auto _click = _adapter->getItemClick();
            if(_click != nullptr){
                int index = 2*pageControl->data->getPageIndex();
                if(pageControl->data->getCurrentPage() == pageControl->data->getLeftPage())
                    index  = index +1;
                _click(pageControl->data->getCurrentPage(),index);
            }
        }
    }
}

void BookView::turnUpdate(float){
    if(pageControl != nullptr){
        accel = accel*0.9;
        pageControl->data->scaleCurrentPage(accel);
    }
}

void BookView::onTouchCancle(Touch *pTouch, Event *pEvent){
    if(pageControl != nullptr){
        pageControl->data->scaleCurrentPage(Vec2(getContentSize().width,0));
    }
}

void BookView::onEnterTransitionDidFinish(){
    Node::onEnterTransitionDidFinish();
    if(_lis == nullptr){
        _lis = EventListenerTouchOneByOne::create();
        _lis->setSwallowTouches(true);
        _lis->onTouchBegan = CC_CALLBACK_2(BookView::onTouchBegan,this);
        _lis->onTouchMoved = CC_CALLBACK_2(BookView::onTouchMoved,this);
        _lis->onTouchEnded = CC_CALLBACK_2(BookView::onTouchEnded,this);
        _lis->onTouchCancelled = CC_CALLBACK_2(BookView::onTouchCancle,this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_lis, this);
    }
}

void  BookView::clearPage(){
    if(pageControl != nullptr){
        auto per = pageControl->prePage;
        auto next = pageControl->nextPage;
        while (per) {
            auto temp = per->prePage;
            delete per;
            per = temp;
        }
        while (next) {
            auto temp = next->nextPage;
            delete next;
            next = temp;
        }
        pageControl->data = nullptr;
        pageControl->prePage = nullptr;
        pageControl->nextPage = nullptr;
    }
}