//
//  PageControl.cpp
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/20/15.
//
//

#include "PageControl.h"
USING_NS_KD;

PageControl::PageControl() {
   pageChangeing = nullptr;
    changeEnd = nullptr;
    curentPage = nullptr;
    minScale = 0.8;
}

PageControl* PageControl::create(float w,float h) {
    auto scol = new PageControl();
    if(scol&& scol->init()) {
        scol->setViewSize(Size(w,h));
        scol->autorelease();
        return scol;
    }
    delete scol;
    scol = nullptr;
    return scol;
}

void PageControl::showPage(EventSprite* page,bool flag) {
    if(page == nullptr||nodeCache.size()==0 )
        return;
    unschedule(schedule_selector(PageControl::deaccelerateScrolling));
    unschedule(schedule_selector(PageControl::performedAnimatedScroll));
    getContainer()->stopAllActions();
    stopAllActions();
    curentPage = page;
    auto off = -page->getPosition() +getViewSize()*.5;
    setContentOffset(Vec2(off.x,0),flag);
    if(flag) {
        unschedule(schedule_selector(PageControl::scaleNode));
        schedule(schedule_selector(PageControl::scaleNode),0.01);
        runAction(Sequence::create(DelayTime::create(0.15f), CallFunc::create([this]{
           // scaleNode();
            if(changeEnd != nullptr) {
                for(size_t i =0; i< nodeCache.size();i++){
                    if(curentPage == nodeCache.at(i)) {
                        changeEnd(i,curentPage);
                    break;
                }
            }
            unschedule(schedule_selector(PageControl::scaleNode));
        }
    }),nullptr));
    } else {
        scaleNode();
        if(changeEnd != nullptr) {
            for(size_t i =0; i< nodeCache.size();i++){
                if(curentPage == nodeCache.at(i)) {
                    changeEnd(i,curentPage);
                    break;
                }
            }
        }

    }
}

void PageControl::showPage(size_t i,bool flag){
    if(i>=nodeCache.size())
        return;
    showPage(nodeCache.at(i),flag);
}

void PageControl::showPage(size_t i,float time){
    if(i>=nodeCache.size())
        return;
    stopAllActions();
    auto page = nodeCache.at(i);
    curentPage = page;
    auto off = -page->getPosition() +getViewSize()*.5;
    setContentOffsetInDuration(Vec2(off.x,0), time);
    runAction(Sequence::create(DelayTime::create(time), CallFunc::create([&]{
        if(changeEnd != nullptr) {
            for(size_t i =0; i< nodeCache.size();i++){
                if(curentPage == nodeCache.at(i)) {
                    changeEnd(i,curentPage);
                    break;
                }
            }
        }
    }),nullptr));
}

void PageControl::setAdapter(MyBaseAdapter* base) {
    MyScollView::setAdapter(base);
//    for(auto node:nodeCache)
//        dynamic_cast<Button*>(node)->setDownColor(Color3B::WHITE);
    if(nodeCache.size()>0)
        itemSize  = nodeCache.at(0)->getContentSize();
    unschedule(schedule_selector(PageControl::deaccelerateScrolling));
    unschedule(schedule_selector(PageControl::performedAnimatedScroll));
    stopAllActions();
    getContainer()->stopAllActions();
    curentPage = nullptr;
    showPage(size_t(0));
}

void PageControl::onTouchMoved(Touch* touch, Event* event){
    if (!this->isVisible())
    {
        return;
    }
    while (_touches.size()>1)
        _touches.erase(_touches.begin());
    ScrollView::onTouchMoved(touch, event);
    if(!isMove){
        auto distance = std::abs((touch->getLocation() - touch->getStartLocation()).x);
        if(distance>8)
            isMove = true;
    }
    if(!isScheduled(schedule_selector(PageControl::scaleNode)))
        schedule(schedule_selector(PageControl::scaleNode),0.01);
    if(isMove && nullptr != pageChangeing)
        pageChangeing();
}

void PageControl::scaleNode(float time){
    for(auto node:nodeCache) {
        float dis = std::abs(convertToNodeSpace(_container->convertToWorldSpace(node->getPosition())).x-getViewSize().width*.5);
        if(dis>getViewSize().width*.5)
            dis = getViewSize().width*.5;
        float scale = 1-(dis/(getViewSize().width*.5))*(1-minScale);
        node->setScale(scale);
        float disPos = dis*0.2;
        node->setPositionY(_viewSize.height*.5+disPos);
    }
}

bool PageControl::onTouchBegan(Touch *touch, Event *event) {
    bool b = MyScollView::onTouchBegan(touch, event);
    if(b){
        startPo = touch->getLocation();
    }
    return b;
}

EventSprite* PageControl::adjustCurentPage(Touch*touch){
    EventSprite* curentPage = nullptr;
    float distance = touch->getLocation().x-startPo.x;
    auto off = Vec2(99999,999);
    for(auto node :nodeCache) {
        auto vec = convertToNodeSpace(_container->convertToWorldSpace(node->getPosition()));
        if(std::abs(distance)<discardMove) {
            if(vec.distance(getViewSize()*.5) <=off.distance(getViewSize()*.5)) {
                curentPage = node;
                off = vec;
            }
        } else if(distance>0) {
            if((vec+Vec2(itemSize.width*.5,0)).distance(getViewSize()*.5) <=(off+Vec2(itemSize.width*.5,0)).distance(getViewSize()*.5)){
                curentPage = node;
                off = vec;
            }
        } else {
            if((vec-Vec2(itemSize.width*.5,0)).distance(getViewSize()*.5) <=(off-Vec2(itemSize.width*.5,0)).distance(getViewSize()*.5)){
                curentPage = node;
                off = vec;
            }
        }
    }
    return curentPage;
}

void PageControl::touchEnd(EventSprite* btn,int tag, Touch *pTouch){
    
}

void PageControl::onTouchEnded(Touch* touch, Event* event)
{
    MyScollView::onTouchEnded(touch, event);
    unschedule(schedule_selector(PageControl::deaccelerateScrolling));
    unschedule(schedule_selector(PageControl::performedAnimatedScroll));
    unschedule(schedule_selector(PageControl::scaleNode));
    if(!isMove)
        for (auto node:nodeCache )
            if(node->getBoundingBox().containsPoint(node->getParent()->convertToNodeSpace(touch->getLocation()))){
                if(node == curentPage)
                    MyScollView::touchEnd(node, node->getTag(), touch);
                else
                    showPage(node,true);
                return;
        }
    showPage(adjustCurentPage(touch),true);
}

void PageControl::onTouchCancelled(Touch* touch, Event* event)
{
    MyScollView::onTouchCancelled(touch, event);
    unschedule(schedule_selector(PageControl::deaccelerateScrolling));
    unschedule(schedule_selector(PageControl::performedAnimatedScroll));
    unschedule(schedule_selector(PageControl::scaleNode));
    showPage(curentPage);
}

void PageControl::onExitTransitionDidStart(){
    MyScollView::onExitTransitionDidStart();
    unschedule(schedule_selector(PageControl::scaleNode));
}

void PageControl::onEnterTransitionDidFinish(){
    MyScollView::onEnterTransitionDidFinish();
    schedule(schedule_selector(PageControl::scaleNode),0.01);
}
