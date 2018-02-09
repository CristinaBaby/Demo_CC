//
//  TweenScrollView.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-4.
//
//

#include "TweenScrollView.h"

TweenScrollView* TweenScrollView::createScrollViewWithVec(Vector<__String*> vec)
{
    TweenScrollView* pRet = new TweenScrollView();
    if(pRet&& pRet -> initScrollViewWithVec(vec))
    {
        pRet -> autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
    
}

bool TweenScrollView::initScrollViewWithVec(Vector<__String*> vec)
{
    if (!Node::init())
    {
        return false;
    }
    
    sp_Vec = vec;
    this -> initScroll();

    return true;
}


void TweenScrollView::initScroll()
{
    scroll_layer = Layer::create();//创建scrollView中的容器层
    scroll_layer->setPosition(Point::ZERO);
    scroll_layer->setAnchorPoint(Point::ZERO);
    scroll_layer->setContentSize(Size((493+40)*(sp_Vec.size()+1)+400,600));
    scroll_layer -> setColor(Color3B(0, 0, 0));
    
    scroll_ = REScrollView::create(Size(Vec2(800, 600)));
    scroll_ -> setDelegate(this);
    scroll_ -> setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    scroll_ -> setContainer(scroll_layer);
    scroll_ -> setPosition(0, 150);
    
    scroll_ -> setBounceable(false);
    addChild(scroll_);
    
    if (sp_Vec.size() > 0)
    {
        for (int i=0 ;i<sp_Vec.size();i++)
        {

                Sprite* btn = Sprite::create(sp_Vec.at(i) -> getCString());
                btn -> setPosition(Vec2((463)*i+250+50+80,300));
                btn_Vec.pushBack(btn);
                scroll_layer -> addChild(btn,4,50+i);
            
                if (i == 0)
                {
                    btn -> setScale(1);
                }
                else
                    btn -> setScale(0.75);
        }
    }

}

void TweenScrollView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    delegate_ -> scrollBegan();
    
    for( auto e : btn_Vec )
    {
        float percent = checkOffset(e);
        log("percent%f",percent);
        e -> setScale(1-percent>0.75?1-percent:0.75);
    }
}

void TweenScrollView::scrollViewDidZoom(REScrollView* view)
{
    
    
}

void TweenScrollView::scrollViewTouchEnd(Vec2 worldPoint)
{
    
    
    //停止scroll按照方向的滑动
    scroll_ -> unscheduleAllSelectors();
    
    log("offset %f",scroll_ -> getContentOffset().x);
    curIdx_ = (scroll_ -> getContentOffset().x - 463/2)/ -463;
    if (curIdx_ >= sp_Vec.size())
    {
        curIdx_ = sp_Vec.size()-1;
    }
    log("%d",curIdx_);
    log("size%zd",sp_Vec.size());
    
    if (curIdx_ < sp_Vec.size())
    {
        scroll_ -> setContentOffsetInDuration(Vec2(curIdx_*(-463), 0),0.2);
        this -> runAction(Sequence::create(DelayTime::create(0.2),
                                           CallFunc::create([&]
        {
            delegate_ -> scrollEndWithIdx(curIdx_);
        }),
                                           nullptr));
    }
    log("offsetafter %f",scroll_ -> getContentOffset().x);
    
    for( auto e : btn_Vec )
    {
        if (e-> getTag() == curIdx_+50)
        {
            
            log("curIdx_%d",curIdx_);
            e -> runAction( ScaleTo::create(0.2, 1));
            e -> setZOrder(10);
            
        }
        else
        {
            if (e-> getTag() == curIdx_+49)
            {
                e -> setScale(0.75);
            }
            
        }
    }
}

float TweenScrollView::checkOffset(Sprite* sp)
{
    auto p = scroll_layer -> convertToWorldSpace(sp -> getPosition());
    float x = 420;
    float spX = p.x;
    float percent =abs(x - spX)/ (sp ->  getContentSize().width*0.35*10);
    return abs(percent);
}


